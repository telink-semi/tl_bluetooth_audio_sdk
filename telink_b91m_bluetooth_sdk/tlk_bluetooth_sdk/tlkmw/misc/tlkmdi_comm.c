/********************************************************************************************************
 * @file    tlkmdi_comm.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/sys_dev/tlkmw_sysdev.h"
#if (TLK_CFG_UART_TOOL_ENABLE)
#include "tlklib/spto/tlkspto.h"
#include "tlkmw/misc/tlkmw_misc.h"
#include "drivers.h"
#include <stdio.h>
#include <stdarg.h>
#include "tlkmw/userCtrl/ota/tlk_ota_protocol_common.h"


void           tlkmdi_comm_input(uint8_t *pData, uint16_t dataLen);
static int     tlkmdi_comm_recvCB(uint32_t param0, uint32_t param1, uint8_t *pData, uint16_t dataLen);
static uint8_t tlkmdi_comm_mtype2TaskID(uint8_t mtype);

__attribute__((aligned(4))) static uint8_t sTlkMdiCommRecvBuffer[TLKMDI_COMM_SERIAL_RBUFF_NUMB * (TLKMDI_COMM_SERIAL_RBUFF_SIZE + 4)];
__attribute__((aligned(4))) static uint8_t sTlkMdiCommSendBuffer[TLKMDI_COMM_SERIAL_SBUFF_NUMB * (TLKMDI_COMM_SERIAL_SBUFF_SIZE + 4)];

typedef struct
{
    TlkMdiCommDatCB dataList[TLKPRT_COMM_DAT_PORT_NUM];
} tlkmdi_comm_ctrl_t;

static tlkmdi_comm_ctrl_t sTlkMdiCommCtrl;

#if (TLK_MW_OTA_ENABLE)
#include "tlkmw/userCtrl/tlkmw_user_ctrl.h"
static void tlkmdi_recv_user_ctrl_data(uint8_t datID, uint32_t number, uint8_t *pData, uint16_t dataLen);
#endif


/**
 * @brief       Initializes the communication module.
 * @return      TLK_ENONE if successful, otherwise a negative error code.    
 */
int tlkmdi_comm_init(void)
{
    tlkspto_recv_regCB(tlkmdi_comm_recvCB);
    memset(&sTlkMdiCommCtrl, 0, sizeof(tlkmdi_comm_ctrl_t));
#if (TLK_DEV_SERIAL_ENABLE)

#if (!MCU_CORE_TL752X_TEMP)
    tlkdrv_serial_mount(TLKDEV_SERIAL1_PORT, TLKMDI_COMM_SERIAL_BAUDRATE, TLKDEV_SERIAL1_TX_PIN, TLKDEV_SERIAL1_RX_PIN, tlkhal_dma_malloc_ex(1), tlkhal_dma_malloc_ex(1));
#else
    tlkdrv_serial_mount(TLKDEV_SERIAL1_PORT, TLKMDI_COMM_SERIAL_BAUDRATE, TLKDEV_SERIAL1_TX_PIN, TLKDEV_SERIAL1_RX_PIN, 4, 0);
#endif

    tlkdrv_serial_setTxQFifo(TLKDEV_SERIAL1_PORT, TLKMDI_COMM_SERIAL_SBUFF_NUMB, TLKMDI_COMM_SERIAL_SBUFF_SIZE + 4, sTlkMdiCommSendBuffer,
                             TLKMDI_COMM_SERIAL_SBUFF_NUMB * (TLKMDI_COMM_SERIAL_SBUFF_SIZE + 4));
#if (!MCU_CORE_TL752X_TEMP)
    tlkdrv_serial_setRxQFifo(TLKDEV_SERIAL1_PORT, TLKMDI_COMM_SERIAL_RBUFF_NUMB, TLKMDI_COMM_SERIAL_RBUFF_SIZE + 4, sTlkMdiCommRecvBuffer,
                             TLKMDI_COMM_SERIAL_SBUFF_NUMB * (TLKMDI_COMM_SERIAL_RBUFF_SIZE + 4));
#else
    tlkdrv_serial_setRxFifo(TLKDEV_SERIAL1_PORT, sTlkMdiCommRecvBuffer, TLKMDI_COMM_SERIAL_RBUFF_NUMB * TLKMDI_COMM_SERIAL_RBUFF_SIZE);
#endif
    tlkdrv_serial_regCB(TLKDEV_SERIAL1_PORT, tlkmdi_comm_input);
    tlkdrv_serial_open(TLKDEV_SERIAL1_PORT);

    tlkspto_send_regCB(tlkmdi_comm_send);
#endif

#if (TLK_MW_OTA_ENABLE)
    uint32_t         tlkmdi_comm_get_ota_param(uint32_t taskID, uint8_t param_type, void *UserArg);
    sTlkMwUnitIntf_t interface = {
        .channel       = TLKMW_OTA_TRANS_CHN_UART,
        .send          = tlkmdi_comm_sendOTADat,
        .recv          = NULL,
        .get_ota_param = tlkmdi_comm_get_ota_param,
    };

    tlkmw_ota_register_chn_interface(&interface);
    tlkmdi_comm_regDatCB(TLKPRT_COMM_OTA_DAT_PORT, tlkmdi_recv_user_ctrl_data, true);
#endif

    return TLK_ENONE;
}

#if (TLKDBG_CFG_HPU_LOG_ENABLE)
/**
 * @brief       This function sends log data to the HPU.
 * @param[in]   pData   - pointer to the log data.
 * @param[in]   dataLen - length of the log data.
 * @return      The length of the data sent if successful, otherwise a negative error code.
 * @note        If the data length exceeds the maximum buffer size, it is truncated.
 */
int32_t tlkdbg_hpulog_send(uint8_t *pData, uint16_t dataLen)
{
    uint16_t maxLen = tlkspto_recv_getBufferSize();
    if (dataLen > maxLen) {
        dataLen = maxLen;
    }
    int ret = tlkmdi_comm_sendLogDat(pData, dataLen);
    if (ret >= 0) {
        return dataLen;
    }
    return ret;
}
#endif

#if (TLK_MW_OTA_ENABLE)

/**
 * @brief       This function gets the OTA parameter.
 * @param[in]   taskID      - task ID.
 * @param[in]   param_type  - parameter type.
 * @param[in]   UserArg     - user argument.
 * @return      The OTA parameter value if successful, otherwise 0.
 */
uint32_t tlkmdi_comm_get_ota_param(uint32_t taskID, uint8_t param_type, void *UserArg)
{
    (void)taskID;
    (void)UserArg;

    if (param_type == TLKMW_OTA_PARAM_MTU_SIZE) {
        return TLKMDI_COMM_SERIAL_OTA_MTU;
    } else if (param_type == TLKMW_OTA_PARAM_SHAKE_INTV) {
        return TLKMDI_COMM_SERIAL_OTA_SHAKE_INTV;
    } else {
        return 0;
    }
}

/**
 * @brief       This function receives user control data and pushes it to the task.
 * @param[in]   datID   - identifier of the data.
 * @param[in]   number  - number of data items.
 * @param[in]   pData   - pointer to the data.
 * @param[in]   dataLen - length of the data.
 * @return      none.
 * @note        The datID and number parameters are not used in the function.
 */
static void tlkmdi_recv_user_ctrl_data(uint8_t datID, uint32_t number, uint8_t *pData, uint16_t dataLen)
{
    (void)datID;
    (void)number;

    tlkmw_userctrl_pushDataToTask(0xFFFFFFFF, 0, pData, dataLen);
}
#endif


/**
 * @brief       This function resets communication by closing, opening, and resetting the serial port if enabled, and resets the service protocol timeout.
 * @return      none.  
 */
void tlkmdi_comm_reset(void)
{
#if (TLK_DEV_SERIAL_ENABLE)
    tlkdrv_serial_close(TLKDEV_SERIAL1_PORT);
    tlkdrv_serial_open(TLKDEV_SERIAL1_PORT);
    tlkdrv_serial_reset(TLKDEV_SERIAL1_PORT);
#endif
    tlkspto_reset();
}

/**
 * @brief       This function returns the buffer size for communication.
 * @return      The buffer size. 
 */
int tlkmdi_comm_getBufferSize(void)
{
    return tlkspto_recv_getBufferSize();
}

/**
 * @brief       This function returns the data buffer size for communication.
 * @return      The data buffer size. 
 */
int tlkmdi_comm_getDataBufferSize(void)
{
    return tlkspto_recv_getBufferSize();
}

/**
 * @brief       This function checks if the serial FIFO has more than 60% bytes available.
 * @param[in]   dataLen - the length of data to check against.
 * @return      true if more than 60% bytes are available, false otherwise.
 */
bool tlkmdi_comm_sfifoIsMore60(uint16_t dataLen)
{
#if (TLK_DEV_SERIAL_ENABLE)
    return tlkdrv_serial_sfifoIsMore60(TLKDEV_SERIAL1_PORT, dataLen);
#else
    (void)dataLen;
    return true;
#endif
}

/**
 * @brief       This function checks if the serial FIFO has more than 80% bytes available.
 * @param[in]   dataLen - the length of data to check against.
 * @return      true if more than 80% bytes are available, false otherwise. 
 */
bool tlkmdi_comm_sfifoIsMore80(uint16_t dataLen)
{
#if (TLK_DEV_SERIAL_ENABLE)
    return tlkdrv_serial_sfifoIsMore85(TLKDEV_SERIAL1_PORT, dataLen);
#else
    (void)dataLen;
    return true;
#endif
}

/**
 * @brief       This function registers a callback for a specific data channel.
 * @param[in]   datID - the data channel ID.
 * @param[in]   datCB - the callback function to register.
 * @param[in]   isForce - if true, force registration even if a callback is already set.
 * @return      TLK_ENONE on success, -TLK_EQUOTA if the datID exceeds the maximum, -TLK_EREPEAT if a callback is already registered. 
 */
int tlkmdi_comm_regDatCB(uint8_t datID, TlkMdiCommDatCB datCB, bool isForce)
{
    if (datID > TLKPRT_COMM_DAT_PORT_NUM) {
        return -TLK_EQUOTA;
    }
    if (!isForce && datCB != NULL && sTlkMdiCommCtrl.dataList[datID] != NULL) {
        return -TLK_EREPEAT;
    }
    sTlkMdiCommCtrl.dataList[datID] = datCB;
    return TLK_ENONE;
}

/**
 * @brief       This function sends data over the serial port.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      TLK_ENONE on success, -TLK_ENOSUPPORT if the serial device is not enabled.   
 */
int tlkmdi_comm_send(uint8_t *pData, uint16_t dataLen)
{
#if (TLK_DEV_SERIAL_ENABLE)
    return tlkdrv_serial_send(TLKDEV_SERIAL1_PORT, pData, dataLen);
#else
    (void)dataLen;
    (void)pData;
    return -TLK_ENOSUPPORT;
#endif
}

/**
 * @brief       This function sends a command over the communication interface.
 * @param[in]   mType - the type of the message.
 * @param[in]   cmdID - the ID of the command.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      The result of the send operation.   
 */
int tlkmdi_comm_sendCmd(uint8_t mType, uint16_t cmdID, uint8_t *pData, uint8_t dataLen)
{
    uint32_t param;

    param = ((uint32_t)(cmdID & 0x0FFF) << 16) | ((uint32_t)mType << 8) | TLKPRT_COMM_PTYPE_CMD;
    return tlkspto_sendProc(param, 0, pData, dataLen);
}

/**
 * @brief       This function sends a response over the communication interface.
 * @param[in]   mType - the type of the message.
 * @param[in]   cmdID - the ID of the command.
 * @param[in]   status - the status of the response.
 * @param[in]   reason - the reason for the response.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      The result of the send operation.   
 */
int tlkmdi_comm_sendRsp(uint8_t mType, uint16_t cmdID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen)
{
    uint32_t param0;
    uint32_t param1;

    param0 = ((uint32_t)(cmdID & 0x0FFF) << 16) | ((uint32_t)mType << 8) | TLKPRT_COMM_PTYPE_RSP;
    param1 = ((uint16_t)reason << 8) | status;
    return tlkspto_sendProc(param0, param1, pData, dataLen);
}

/**
 * @brief       This function sends an event over the communication interface.
 * @param[in]   mType - the type of the message.
 * @param[in]   evtID - the ID of the event.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      The result of the send operation.
 */
int tlkmdi_comm_sendEvt(uint8_t mType, uint16_t evtID, uint8_t *pData, uint8_t dataLen)
{
    uint32_t param = ((uint32_t)(evtID & 0x0FFF) << 16) | ((uint32_t)mType << 8) | TLKPRT_COMM_PTYPE_EVT;
    return tlkspto_sendProc(param, 0, pData, dataLen);
}

/**
 * @brief       This function sends data packets over the communication interface.
 * @param[in]   datID - the ID of the data.
 * @param[in]   numb - the number associated with the data.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      The result of the send operation, -TLK_EPARAM if input parameters are invalid. 
 */
int tlkmdi_comm_sendDat(uint8_t datID, uint32_t numb, uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0) {
        return -TLK_EPARAM;
    }

    uint32_t param0 = ((uint32_t)(datID & 0xFF) << 8) | TLKPRT_COMM_PTYPE_DAT;
    uint32_t param1 = numb;
    return tlkspto_sendProc(param0, param1, pData, dataLen);
}

/**
 * @brief       This function sends log data over the communication interface.
 * @param[in]   pData - the pointer to the log data to send.
 * @param[in]   dataLen - the length of the log data to send.
 * @return      The result of the send operation, -TLK_EBUSY if the serial FIFO is busy.   
 */
int tlkmdi_comm_sendLogDat(uint8_t *pData, uint16_t dataLen)
{
#if (TLKDBG_CFG_HPU_LOG_ENABLE)
    if (tlkmdi_comm_sfifoIsMore60(dataLen)) {
        return -TLK_EBUSY;
    }
#else
    if (tlkmdi_comm_sfifoIsMore80(dataLen)) {
        return -TLK_EBUSY;
    }
#endif
    return tlkmdi_comm_sendDat(TLKPRT_COMM_SYS_DAT_PORT, TLKPRT_COMM_SYS_DAT_LOG, pData, dataLen);
}

/**
 * @brief       This function sends VCD data over the communication interface.
 * @param[in]   pData - the pointer to the VCD data to send.
 * @param[in]   dataLen - the length of the VCD data to send.
 * @return      The result of the send operation, -TLK_EBUSY if the serial FIFO is busy.   
 */
int tlkmdi_comm_sendVcdDat(uint8_t *pData, uint16_t dataLen)
{
    if (tlkmdi_comm_sfifoIsMore80(dataLen)) {
        return -TLK_EBUSY;
    }
    return tlkmdi_comm_sendDat(TLKPRT_COMM_SYS_DAT_PORT, TLKPRT_COMM_SYS_DAT_VCD, pData, dataLen);
}

/**
 * @brief       This function sends OTA data over the communication interface.
 * @param[in]   taskID - the ID of the task.
 * @param[in]   pData - the pointer to the OTA data to send.
 * @param[in]   dataLen - the length of the OTA data to send.
 * @param[in]   UserArg - the user argument.
 * @return      The result of the send operation, -TLK_EBUSY if the serial FIFO is busy.
 */
int tlkmdi_comm_sendOTADat(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    (void)taskID;
    (void)UserArg;

    if (tlkmdi_comm_sfifoIsMore80(dataLen)) {
        return -TLK_EBUSY;
    }
    return tlkmdi_comm_sendDat(TLKPRT_COMM_OTA_DAT_PORT, TLKPRT_COMM_SYS_DAT_OTA, pData, dataLen);
}

/**
 * @brief       This function sends cover data over the communication interface.
 * @param[in]   pData - the pointer to the cover data to send.
 * @param[in]   dataLen - the length of the cover data to send.
 * @return      The result of the send operation, -TLK_EBUSY if the serial FIFO is busy.
 */
int tlkmdi_comm_sendCoverDat(uint8_t *pData, uint16_t dataLen)
{
    if (tlkmdi_comm_sfifoIsMore80(dataLen)) {
        return -TLK_EBUSY;
    }
    return tlkmdi_comm_sendDat(TLKPRT_COMM_SYS_DAT_PORT, TLKPRT_COMM_SYS_DAT_COVER, pData, dataLen);
}

/**
 * @brief       This function processes incoming data by passing it to the receive procedure.
 * @param[in]   pData - the pointer to the data received.
 * @param[in]   dataLen - the length of the data received.
 * @return      none.   
 */
void tlkmdi_comm_input(uint8_t *pData, uint16_t dataLen)
{
    tlkspto_recvProc(pData, dataLen);
}

/**
 * @brief       This callback function handles received communication data.
 * @param[in]   param0 - the first parameter containing data type and message type.
 * @param[in]   param1 - the second parameter containing additional data.
 * @param[in]   pData - the pointer to the received data.
 * @param[in]   dataLen - the length of the received data.
 * @return      TLK_ENONE on success, -TLK_EFAIL if the message type is invalid.
 */
static int tlkmdi_comm_recvCB(uint32_t param0, uint32_t param1, uint8_t *pData, uint16_t dataLen)
{
    uint8_t ptype;

    ptype = param0 & 0xFF;
    if (ptype == TLKPRT_COMM_PTYPE_CMD) {
        uint8_t  mtype = (param0 & 0xFF00) >> 8;
        uint16_t msgID = (param0 & 0xFFFF0000) >> 16;
        if (mtype >= TLKPRT_COMM_MTYPE_MAX || mtype == 0) {
            return -TLK_EFAIL;
        }
        uint8_t taskID = tlkmdi_comm_mtype2TaskID(mtype);
        tlksys_sendMsg(taskID, msgID, pData, dataLen);
    } else if (ptype == TLKPRT_COMM_PTYPE_DAT) {
        uint8_t  dataID = (param0 & 0xFFFF00) >> 8;
        uint32_t number = (param1 & 0x0FFFFF);
        if (dataID <= TLKPRT_COMM_DAT_PORT_NUM && sTlkMdiCommCtrl.dataList[dataID] != NULL) {
            sTlkMdiCommCtrl.dataList[dataID](dataID, number, pData, dataLen);
        }
    }
    return TLK_ENONE;
}

/**
 * @brief       This function maps message type to task ID.
 * @param[in]   mtype - the type of the message.
 * @return      The task ID corresponding to the message type. 
 */
static uint8_t tlkmdi_comm_mtype2TaskID(uint8_t mtype)
{
    switch (mtype) {
    case TLKPRT_COMM_MTYPE_SYS:
    case TLKPRT_COMM_MTYPE_DBG:
    case TLKPRT_COMM_MTYPE_FILE:
        return TLKSYS_TASKID_SYSTEM;
    case TLKPRT_COMM_MTYPE_BT:
    case TLKPRT_COMM_MTYPE_CALL:
        return TLKSYS_TASKID_HOST;
#if TLKAPP_LEMGR_ENABLE
    case TLKPRT_COMM_MTYPE_LE:
        return TLKSYS_TASKID_LEMGR;
#endif
    case TLKPRT_COMM_MTYPE_AUDIO:
        return TLKSYS_TASKID_AUDIO;
    }
    return TLKSYS_TASKID_MAXNUM;
}
#endif // #if (TLK_CFG_UART_TOOL_ENABLE)


/**
 * @brief       This function handles communication by processing serial handlers if the serial device is enabled.
 * @return      none.
 */
#if (TLK_DEV_SERIAL_ENABLE)
void tlkmdi_serials_handler(void)
{
#if (!MCU_CORE_TL752X_TEMP)
    tlkdrv_serial_handler(UART0);
    tlkdrv_serial_handler(UART1);
#else
    tlkdrv_serial_handler(TLKDEV_SERIAL0_PORT);
    tlkdrv_serial_handler(TLKDEV_SERIAL1_PORT);
#endif //     #if (!MCU_CORE_TL752X_TEMP)
}
#endif
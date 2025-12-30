/********************************************************************************************************
 * @file    tlkdrv_sy5500.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapi/tlkapi.h"
#include "inner/tlkdrv_sy5500_ctrl.h"
#include "tlkdrv_sy5500_uart.h"
#include "tlkdrv_sy5500_i2c.h"
#if TLK_DEV_SY5500_ENABLE

static TlkDrvSy5500EvtCB  sTlkDrvSy5500EvtCB = NULL;
static TlkDrvSy5500Ctrl_t sTlkDrvSy5500Ctrl  = {0};

/**
 * @brief Temporary timer, used for testing or other purposes
 * @TODO Use GPIO_IRQ instead
 */
static TlkApiTimer_t tempTimer; //TODO: USE GPIO_IRQ

/**
 * @brief Calculate checksum of data
 * @param[in] pData Data pointer
 * @param[in] dataLen Data length
 * @return Checksum result
 */
static inline uint8_t tlkdrv_sy5500_getSum(uint8_t *pData, uint16_t dataLen)
{
    uint8_t sum = 0;
    for (uint16_t i = 0; i < dataLen; i++) {
        sum += pData[i]; //sum crc
    }
    return sum;
}

/**
 * @brief Send event to upper layer application
 * @param[in] evtID Event ID
 * @param[in] pData Event data pointer
 * @param[in] dataLen Event data length
 * @param[in] isNeedAck Whether acknowledgment is needed
 */
static void tlkdrv_sy5500_sendEvt(uint16_t evtID, uint8_t *pData, uint16_t dataLen, bool isNeedAck)
{
    if (sTlkDrvSy5500EvtCB == NULL) {
        return;
    }
    sTlkDrvSy5500EvtCB(evtID, pData, dataLen, isNeedAck);
}

/**
 * @brief Handle box open event
 */
static inline void tlkdrv_sy5500_openBoxDeal(void)
{
    if (sTlkDrvSy5500Ctrl.isBoxOpen == TLKDRV_SY5500_CTRL_STATE_TRUE) {
        return;
    }
    sTlkDrvSy5500Ctrl.isBoxOpen = TLKDRV_SY5500_CTRL_STATE_TRUE;
    tlkdrv_sy5500_sendEvt(TLKDRV_SY5500_EVT_BOX_OPEN, NULL, 0, true);
}

/**
 * @brief Handle box close event
 */
static inline void tlkdrv_sy5500_closeBoxDeal(void)
{
    if (sTlkDrvSy5500Ctrl.isBoxOpen == TLKDRV_SY5500_CTRL_STATE_FALSE) {
        return;
    }
    sTlkDrvSy5500Ctrl.isBoxOpen = TLKDRV_SY5500_CTRL_STATE_FALSE;
    tlkdrv_sy5500_sendEvt(TLKDRV_SY5500_EVT_BOX_CLOSE, NULL, 0, true);
}

/**
 * @brief Handle enter box event
 */
static inline void tlkdrv_sy5500_enterBoxDeal(void)
{
    //feed watch dog
    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, &sTlkDrvSy5500Ctrl.inboxTimer);
    if (sTlkDrvSy5500Ctrl.isInbox == TLKDRV_SY5500_CTRL_STATE_TRUE) {
        return;
    }
    sTlkDrvSy5500Ctrl.isInbox = TLKDRV_SY5500_CTRL_STATE_TRUE;
    tlkdrv_sy5500_sendEvt(TLKDRV_SY5500_EVT_ENTER_BOX, NULL, 0, true);
}

/**
 * @brief Handle leave box event
 */
static inline void tlkdrv_sy5500_leaveBoxDeal(void)
{
    if (sTlkDrvSy5500Ctrl.isInbox == TLKDRV_SY5500_CTRL_STATE_FALSE) {
        return;
    }
    sTlkDrvSy5500Ctrl.isInbox = TLKDRV_SY5500_CTRL_STATE_FALSE;
    tlkdrv_sy5500_sendEvt(TLKDRV_SY5500_EVT_LEAVE_BOX, NULL, 0, true);
}

/**
 * @brief Handle 10-second press event
 * @param[in] pData Received data pointer
 * @param[in] dataLen Received data length
 */
static inline void tlkdrv_sy5500_10sPressDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (dataLen != 9 || pData[8] != tlkdrv_sy5500_getSum(pData, 8)) {
        return;
    }
    uint8_t rsp[3] = {0x55, 0xca, 0x1f};
    tlkdrv_sy5500_sendUartData(rsp, 3);
    tlkdrv_sy5500_sendEvt(TLKDRV_SY5500_EVT_BOX_FACTORY_RESET, &pData[2], 6, false);
}

/**
 * @brief Handle 5V power on event
 */
static inline void tlkdrv_sy5500_5vOnDeal(void)
{
    tlkdrv_sy5500_enterBoxDeal();
    if (sTlkDrvSy5500Ctrl.is5vOn == TLKDRV_SY5500_CTRL_STATE_TRUE) {
        return;
    }
    sTlkDrvSy5500Ctrl.is5vOn = TLKDRV_SY5500_CTRL_STATE_TRUE;
    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, &sTlkDrvSy5500Ctrl.powerTimer);
}

/**
 * @brief Handle 5V power off event
 */
static inline void tlkdrv_sy5500_5vOffDeal(void)
{
    if (sTlkDrvSy5500Ctrl.is5vOn == TLKDRV_SY5500_CTRL_STATE_FALSE) {
        return;
    }
    sTlkDrvSy5500Ctrl.is5vOn = TLKDRV_SY5500_CTRL_STATE_FALSE;
    tlksys_timer_stop(TLKSYS_TASKID_SYSTEM, &sTlkDrvSy5500Ctrl.powerTimer);
}

/**
 * @brief Timer callback function, handle I2C state changes
 * @param[in] pTimer Timer handle
 * @param[in] userArg User argument
 */
static void tlkdrv_sy5500_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    tlkdrv_sy5500_i2c_stateChgHandler();
}

/**
 * @brief Power timer callback function, handle power related events
 * @param[in] pTimer Timer handle
 * @param[in] userArg User argument
 */
static void tlkdrv_sy5500_powerTimer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    if (sTlkDrvSy5500Ctrl.is5vOn == TLKDRV_SY5500_CTRL_STATE_TRUE) {
        tlkdrv_sy5500_sendEvt(TLKDRV_SY5500_EVT_NEED_POWER_OFF, NULL, 0, false);
    }
}

/**
 * @brief In-box heartbeat timer callback function, detect if earphones are still in charging box
 * @param[in] pTimer Timer handle
 * @param[in] userArg User argument
 */
static void tlkdrv_sy5500_inBoxHeartTimer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    if (sTlkDrvSy5500Ctrl.isInbox != TLKDRV_SY5500_CTRL_STATE_TRUE) {
        return;
    }
    if (sTlkDrvSy5500Ctrl.is5vOn == TLKDRV_SY5500_CTRL_STATE_TRUE) {
        return;
    }
    tlkdrv_sy5500_leaveBoxDeal();
}

/**
 * @brief Handle received serial data
 * @param[in] pData Received data pointer
 * @param[in] dataLen Received data length
 */
static void tlkdrv_sy5500_recSerialData(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
    if (dataLen < 2) {
        return;
    }
    bool     validCommand = true;
    uint16_t cmd          = (pData[0] << 8) | pData[1];
    switch (cmd) {
    case TLKDRV_SY5500_UART_CMD_HEART_BEAT:
    {
        // Heartbeat packet command handling
    } break;
    case TLKDRV_SY5500_UART_CMD_OPEN_BOX:
    {
        // Open box command handling
        tlkdrv_sy5500_openBoxDeal();
    } break;
    case TLKDRV_SY5500_UART_CMD_CLOSE_BOX:
    {
        // Close box command handling
        tlkdrv_sy5500_closeBoxDeal();
    } break;
    case TLKDRV_SY5500_UART_CMD_3S_PRESS:
    {
        // 3-second button command handling
        uint8_t buffer[3] = {1, 2, 3};
        tlkdrv_sy5500_sendEvt(TLKDRV_SY5500_EVT_BOX_PAIR, buffer, 3, true);
    } break;
    case TLKDRV_SY5500_UART_CMD_10S_PRESS:
    {
        // 10-second button command handling
        tlkdrv_sy5500_10sPressDeal(pData, dataLen);
    } break;
    case TLKDRV_SY5500_UART_CMD_GET_MAC:
    {
        tlkdrv_sy5500_sendEvt(TLKDRV_SY5500_EVT_BOX_GET_MAC, NULL, 0, false);
    } break;
    default:
        validCommand = false;
        break;
    }
    if (validCommand) { //get uart valid cmd,so in box
        tlkdrv_sy5500_enterBoxDeal();
    }
}

/**
 * @brief Handle received I2C events
 * @param[in] evtId Event ID
 */
static void tlkdrv_sy5500_recI2cEvt(uint16_t evtId)
{
    switch (evtId) {
    case TLKDRV_SY5500_I2C_EVT_5V_ON:
    {
        // 5V power on event handling
        tlkdrv_sy5500_5vOnDeal();
    } break;
    case TLKDRV_SY5500_I2C_EVT_5V_OFF:
    {
        // 5V power off event handling
        tlkdrv_sy5500_5vOffDeal();
    } break;
    case TLKDRV_SY5500_I2C_EVT_ENTER_BOX:
    {
        // Enter charging box event handling
        tlkdrv_sy5500_enterBoxDeal();
    } break;
    case TLKDRV_SY5500_I2C_EVT_LEAVE_BOX:
    {
        // Leave charging box event handling
        tlkdrv_sy5500_leaveBoxDeal();
    } break;
    }
}

/**
 * @brief Initialize SY5500 driver
 * @param[in] cb Event callback function
 */
void tlkdrv_sy5500_init(TlkDrvSy5500EvtCB cb)
{
    sTlkDrvSy5500EvtCB = cb;
    tlkdrv_sy5500_uart_init(tlkdrv_sy5500_recSerialData);
    tlkdrv_sy5500_i2c_init(2, tlkdrv_sy5500_recI2cEvt);
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &tempTimer, 100 * 1000, true, tlkdrv_sy5500_timer, NULL);
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &sTlkDrvSy5500Ctrl.inboxTimer, 400 * 1000, false, tlkdrv_sy5500_inBoxHeartTimer, NULL);
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &sTlkDrvSy5500Ctrl.powerTimer, 1 * 1000 * 1000, false, tlkdrv_sy5500_powerTimer, NULL);
    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, &tempTimer);
}

/**
 * @brief Send state information to charging box
 * @param[in] state State value
 */
void tlkdrv_sy5500_sendStateToBox(uint8_t state)
{
    uint8_t dat[4] = {
        [0] = 0x55,
        [1] = 0xa5,
        [2] = state,
    };
    dat[3] = dat[0] + dat[1] + dat[2];
    tlkdrv_sy5500_sendUartData(dat, 4);
}

/**
 * @brief Send MAC address to charging box
 * @param[in] mac MAC address pointer
 */
void tlkdrv_sy5500_sendMacToBox(uint8_t *mac)
{
    uint8_t dat[9];
    dat[0] = (TLKDRV_SY5500_UART_CMD_GET_MAC + 1) >> 8;
    dat[1] = (TLKDRV_SY5500_UART_CMD_GET_MAC + 1) & 0xff;
    dat[8] = 0;
    tmemcpy(dat + 2, mac, 6);
    dat[8] = tlkdrv_sy5500_getSum(dat, 8); //sum crc
    tlkdrv_sy5500_sendUartData(dat, 9);
}

#endif
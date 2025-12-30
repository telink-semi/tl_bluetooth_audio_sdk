/********************************************************************************************************
 * @file    tlkusb_mscScsi.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#if (TLK_USB_MSC_ENABLE)

#include "tlklib/usb/msc/tlkusb_mscDefine.h"
#include "tlklib/usb/msc/tlkusb_msc.h"
#include "tlklib/usb/msc/tlkusb_mscDesc.h"
#include "tlklib/usb/msc/tlkusb_mscScsi.h"

/**
 * @brief       Send data via USB.
 * @param[in]   pData - Data pointer.
 * @param[in]   dataLen - Data length.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkusb_msc_scsiSendData(uint8_t *pData, uint8_t dataLen);

/**
 * @brief       Set status for SCSI command.
 * @param[in]   pTag - Tag pointer.
 * @param[in]   residue - Residue value.
 * @param[in]   status - Status value.
 * @return      none.
 */
static void tlkusb_msc_scsiSetStatus(uint8_t *pTag, uint32_t residue, uint8_t status);

/**
 * @brief       Receive data from USB endpoint.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvData(void);

/**
 * @brief       Handle idle stage of SCSI protocol.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageIdleDeal(void);

/**
 * @brief       Handle data transfer stage of SCSI protocol.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageDataDeal(void);

/**
 * @brief       Handle status stage of SCSI protocol.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageStatusDeal(void);

/**
 * @brief       Handle complete stage of SCSI protocol.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageCompleteDeal(void);

/**
 * @brief       Handle data sending in data stage.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageDataSendDeal(void);

/**
 * @brief       Handle data reading in data stage.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageDataReadDeal(void);

/**
 * @brief       Handle data receiving in data stage.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageDataRecvDeal(void);

/**
 * @brief       Handle TEST UNIT READY command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvTestUnitDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle REQUEST SENSE command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvRequestSenseDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle INQUIRY command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvInquiryDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle MODE SENSE(6) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvModeSense6Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle MODE SENSE(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvModeSense10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle START STOP UNIT command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvStartStopUnitDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle ALLOW MEDIUM REMOVAL command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvAllowMediumRemoveDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle READ FORMAT CAPACITIES command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvReadFormatCapacityDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle READ CAPACITY(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvReadCapacity10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle READ(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvRead10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle WRITE(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvWrite10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

/**
 * @brief       Handle VERIFY(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvVerify10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen);

static tlkusb_msc_scsi_t sTlkUsbMscScsi;

/**
 * @brief       Initialize the MSC SCSI module.
 * @param[in]   none.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkusb_msc_scsiInit(void)
{
    tmemset(&sTlkUsbMscScsi, 0, sizeof(tlkusb_msc_scsi_t));

    sTlkUsbMscScsi.cswBuff[0] = 'U';
    sTlkUsbMscScsi.cswBuff[1] = 'S';
    sTlkUsbMscScsi.cswBuff[2] = 'B';
    sTlkUsbMscScsi.cswBuff[3] = 'S';

    sTlkUsbMscScsi.enable = true;

    return TLK_ENONE;
}

/**
 * @brief       Reset the MSC SCSI module.
 * @param[in]   none.
 * @return      none.
 */
void tlkusb_msc_scsiReset(void)
{
    sTlkUsbMscScsi.enable = true;
    sTlkUsbMscScsi.stage  = TLKUSB_MSC_SCSI_STAGE_IDLE;
    sTlkUsbMscScsi.flags  = TLKUSB_MSC_SCSI_FLAG_NONE;

    sTlkUsbMscScsi.optLen = 0;
    sTlkUsbMscScsi.datLen = 0;
    sTlkUsbMscScsi.blkCnt = 0;
    sTlkUsbMscScsi.blkNum = 0;
    sTlkUsbMscScsi.blkOff = 0;
}

/**
 * @brief       Handler for the MSC SCSI module.
 * @param[in]   none.
 * @return      none.
 */
void tlkusb_msc_scsiHandler(void)
{
    if (sTlkUsbMscScsi.stage == TLKUSB_MSC_SCSI_STAGE_IDLE) {
        tlkusb_msc_scsiStageIdleDeal();
    }

    if (sTlkUsbMscScsi.stage == TLKUSB_MSC_SCSI_STAGE_DATA) {
        tlkusb_msc_scsiStageDataDeal();
    } else if (sTlkUsbMscScsi.stage == TLKUSB_MSC_SCSI_STAGE_STATUS) {
        tlkusb_msc_scsiStageStatusDeal();
    } else if (sTlkUsbMscScsi.stage == TLKUSB_MSC_SCSI_STAGE_COMPLETE) {
        tlkusb_msc_scsiStageCompleteDeal();
    }
    if (sTlkUsbMscScsi.stage != TLKUSB_MSC_SCSI_STAGE_IDLE) {
        tlkusb_hal_wakeup_usb_thread();
    }
}

/**
 * @brief       Set the SCSI status.
 * @param[in]   pTag - Tag.
 * @param[in]   residue - Residue.
 * @param[in]   status - Status.
 * @return      none.
 */
static void tlkusb_msc_scsiSetStatus(uint8_t *pTag, uint32_t residue, uint8_t status)
{
    if (pTag != NULL) {
        sTlkUsbMscScsi.cswBuff[4] = pTag[0];
        sTlkUsbMscScsi.cswBuff[5] = pTag[1];
        sTlkUsbMscScsi.cswBuff[6] = pTag[2];
        sTlkUsbMscScsi.cswBuff[7] = pTag[3];
    }
    sTlkUsbMscScsi.cswBuff[8]  = residue & 0xFF;
    sTlkUsbMscScsi.cswBuff[9]  = (residue >> 8) & 0xFF;
    sTlkUsbMscScsi.cswBuff[10] = (residue >> 16) & 0xFF;
    sTlkUsbMscScsi.cswBuff[11] = (residue >> 24) & 0xFF;
    sTlkUsbMscScsi.cswBuff[12] = status;
}

/**
 * @brief       Receive data.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvData(void)
{
    sTlkUsbMscScsi.rcvLen = 0;
    uint32_t irq          = tlkusb_hal_get_eps_irq(0);
    if ((irq & BIT(TLKUSB_MSC_EDP_OUT)) == 0) {
        return;
    }
    tlkusb_hal_clr_eps_irq(0, BIT(TLKUSB_MSC_EDP_OUT));
    sTlkUsbMscScsi.rcvLen = reg_usb_ep_ptr(TLKUSB_MSC_EDP_OUT);
    if (sTlkUsbMscScsi.rcvLen > TLKUSB_MSC_REC_BUFFER_LEN) {
        sTlkUsbMscScsi.rcvLen = TLKUSB_MSC_REC_BUFFER_LEN;
    }
    usbhw_reset_ep_ptr((uint8_t)TLKUSB_MSC_EDP_OUT);
    for (size_t index = 0; index < sTlkUsbMscScsi.rcvLen; index++) {
        sTlkUsbMscScsi.rcvBuff[index] = reg_usb_ep_dat(TLKUSB_MSC_EDP_OUT);
    }
    usbhw_data_ep_ack(TLKUSB_MSC_EDP_OUT); // set the ack
}

/**
 * @brief       Deal with the stage IDLE.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageIdleDeal(void)
{
    uint8_t            logNum;
    uint8_t            opcode;
    uint8_t            cmdLen;
    uint8_t           *pCmd;
    tlkusb_msc_disk_t *pUnit;

    tlkusb_msc_scsiRecvData();
    if (sTlkUsbMscScsi.rcvLen == 0) {
        return;
    }

    if (sTlkUsbMscScsi.rcvLen < TLKUSB_MSC_CBW_LENGTH || sTlkUsbMscScsi.rcvBuff[0] != 'U' || sTlkUsbMscScsi.rcvBuff[1] != 'S' || sTlkUsbMscScsi.rcvBuff[2] != 'B' ||
        sTlkUsbMscScsi.rcvBuff[3] != 'C') {
        return; //Error Length
    }

    pCmd = sTlkUsbMscScsi.rcvBuff + TLKUSB_MSC_CBW_CB_OFFSET;

    opcode = pCmd[0];
    logNum = pCmd[1] >> 5;
    cmdLen = pCmd[TLKUSB_MSC_CBW_BLEN_OFFSET] & 0x1F;
    pUnit  = tlkusb_msc_getDisk(logNum);
    if (pUnit == NULL) {
        tlkusb_msc_scsiSetStatus(sTlkUsbMscScsi.rcvBuff + 4, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
        sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        return;
    }

    sTlkUsbMscScsi.curLun = logNum;
    tlkusb_msc_scsiSetStatus(sTlkUsbMscScsi.rcvBuff + 4, 0, 0);

    switch (opcode) {
    case TLKUSB_MSC_SCSI_TEST_UNIT_READY:
        tlkusb_msc_scsiRecvTestUnitDeal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_REQUEST_SENSE:
        tlkusb_msc_scsiRecvRequestSenseDeal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_INQUIRY:
        tlkusb_msc_scsiRecvInquiryDeal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_MODE_SENSE6:
        tlkusb_msc_scsiRecvModeSense6Deal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_MODE_SENSE10:
        tlkusb_msc_scsiRecvModeSense10Deal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_START_STOP_UNIT:
        tlkusb_msc_scsiRecvStartStopUnitDeal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_ALLOW_MEDIUM_REMOVAL:
        tlkusb_msc_scsiRecvAllowMediumRemoveDeal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_READ_FORMAT_CAPACITIES:
        tlkusb_msc_scsiRecvReadFormatCapacityDeal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_READ_CAPACITY10:
        tlkusb_msc_scsiRecvReadCapacity10Deal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_READ10:
        tlkusb_msc_scsiRecvRead10Deal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_WRITE10:
        tlkusb_msc_scsiRecvWrite10Deal(pUnit, pCmd, cmdLen);
        break;
    case TLKUSB_MSC_SCSI_VERIFY10:
        tlkusb_msc_scsiRecvVerify10Deal(pUnit, pCmd, cmdLen);
        break;
    default:
        tlkusb_msc_scsiSetStatus(NULL, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
        sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        break;
    }
}

/**
 * @brief       Deal with the stage DATA.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageDataDeal(void)
{
    if ((sTlkUsbMscScsi.flags & TLKUSB_MSC_SCSI_FLAG_SEND_DATA) != 0) {
        tlkusb_msc_scsiStageDataSendDeal();
        if (sTlkUsbMscScsi.datLen == 0) {
            sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        }
    } else if ((sTlkUsbMscScsi.flags & TLKUSB_MSC_SCSI_FLAG_READ_DATA) != 0) {
        tlkusb_msc_scsiStageDataReadDeal();
    } else if ((sTlkUsbMscScsi.flags & TLKUSB_MSC_SCSI_FLAG_RECV_DATA) != 0) {
        tlkusb_msc_scsiStageDataRecvDeal();
    }
}

/**
 * @brief       Deal with the stage STATUS.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageStatusDeal(void)
{
    int ret;
    ret = tlkusb_msc_scsiSendData(sTlkUsbMscScsi.cswBuff, TLKUSB_MSC_CSW_LENGTH);
    if (ret == TLK_ENONE) {
        sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_COMPLETE;
    }
}

/**
 * @brief       Deal with the stage COMPLETE.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageCompleteDeal(void)
{
    if (usbhw_is_ep_busy(TLKUSB_MSC_EDP_IN)) {
        return;
    }

    sTlkUsbMscScsi.stage  = TLKUSB_MSC_SCSI_STAGE_IDLE;
    sTlkUsbMscScsi.flags  = TLKUSB_MSC_SCSI_FLAG_NONE;
    sTlkUsbMscScsi.optLen = 0;
    sTlkUsbMscScsi.datLen = 0;
    sTlkUsbMscScsi.blkCnt = 0;
    sTlkUsbMscScsi.blkNum = 0;
    sTlkUsbMscScsi.blkOff = 0;
}

/**
 * @brief       Deal with the stage DATA SEND.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageDataSendDeal(void)
{
    int     ret;
    uint8_t sendLen;

    if (usbhw_is_ep_busy(TLKUSB_MSC_EDP_IN)) {
        return;
    }

    if (sTlkUsbMscScsi.optLen + 64 <= sTlkUsbMscScsi.datLen) {
        sendLen = 64;
    } else {
        sendLen = sTlkUsbMscScsi.datLen - sTlkUsbMscScsi.optLen;
    }

    if (sendLen == 0) {
        ret = TLK_ENONE;
    } else {
        ret = tlkusb_msc_scsiSendData(sTlkUsbMscScsi.buffer + sTlkUsbMscScsi.optLen, sendLen);
    }
    if (ret == TLK_ENONE) {
        sTlkUsbMscScsi.optLen += sendLen;
    }
    if (sTlkUsbMscScsi.optLen < sTlkUsbMscScsi.datLen) {
        return;
    }

    sTlkUsbMscScsi.optLen = 0;
    sTlkUsbMscScsi.datLen = 0;
}

/**
 * @brief       Deal with the stage DATA READ.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_msc_scsiStageDataReadDeal(void)
{
    int                ret;
    tlkusb_msc_disk_t *pUnit;

    if (sTlkUsbMscScsi.datLen != 0) {
        tlkusb_msc_scsiStageDataSendDeal();
        return;
    }

    if (sTlkUsbMscScsi.blkNum >= sTlkUsbMscScsi.blkCnt) {
        sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        return;
    }

    pUnit = tlkusb_msc_getDisk(sTlkUsbMscScsi.curLun);
    if (pUnit == NULL || pUnit->Read == NULL || pUnit->blkSize > TLKUSB_MSC_BLOCK_SIZE) {
        tlkusb_msc_scsiSetStatus(NULL, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
        sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        return;
    }

    ret = pUnit->Read(sTlkUsbMscScsi.buffer, sTlkUsbMscScsi.blkOff + sTlkUsbMscScsi.blkNum, 1);
    if (ret < 0) {
        tlkusb_msc_scsiSetStatus(NULL, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
        sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        return;
    }

    sTlkUsbMscScsi.blkNum += 1;
    sTlkUsbMscScsi.datLen = pUnit->blkSize;
    sTlkUsbMscScsi.optLen = 0;
}

static void tlkusb_msc_scsiStageDataRecvDeal(void)
{
    tlkusb_msc_disk_t *pUnit;

    if (sTlkUsbMscScsi.blkNum >= sTlkUsbMscScsi.blkCnt) {
        sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        return;
    }

    tlkusb_msc_scsiRecvData();
    if (sTlkUsbMscScsi.rcvLen == 0) {
        return;
    }

    pUnit = tlkusb_msc_getDisk(sTlkUsbMscScsi.curLun);
    if (pUnit == NULL || pUnit->Write == NULL || pUnit->blkSize > TLKUSB_MSC_BLOCK_SIZE || sTlkUsbMscScsi.optLen + sTlkUsbMscScsi.rcvLen > pUnit->blkSize) {
        tlkusb_msc_scsiSetStatus(NULL, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
        sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        return;
    }

    tmemcpy(sTlkUsbMscScsi.buffer + sTlkUsbMscScsi.optLen, sTlkUsbMscScsi.rcvBuff, sTlkUsbMscScsi.rcvLen);
    sTlkUsbMscScsi.optLen += sTlkUsbMscScsi.rcvLen;

    if (sTlkUsbMscScsi.optLen == pUnit->blkSize) {
        int ret;
        ret = pUnit->Write(sTlkUsbMscScsi.buffer, sTlkUsbMscScsi.blkOff + sTlkUsbMscScsi.blkNum, 1);
        if (ret < 0) {
            tlkusb_msc_scsiSetStatus(NULL, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
            sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
        } else {
            sTlkUsbMscScsi.optLen = 0;
            sTlkUsbMscScsi.blkNum += 1;
        }
    }
}

/**
 * @brief       Handle TEST UNIT READY command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvTestUnitDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)pCmd;
    (void)cmdLen;
    if (!sTlkUsbMscScsi.enable) {
        tlkusb_msc_scsiSetStatus(NULL, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
    }
    sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
}

/**
 * @brief       Handle REQUEST SENSE command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvRequestSenseDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)pCmd;
    (void)cmdLen;
    uint8_t datLen                  = 0;
    sTlkUsbMscScsi.buffer[datLen++] = 0x70; //Error code, fixed as 0x70
    sTlkUsbMscScsi.buffer[datLen++] = 0x00; //Reserved
    sTlkUsbMscScsi.buffer[datLen++] = 0x05; //Sense Key 0x05,Indicates an invalid command opcode(ILLEGAL REQUEST)
    sTlkUsbMscScsi.buffer[datLen++] = 0x00; //Information as 0
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x0A; //The length of additional data is 10 bytes
    sTlkUsbMscScsi.buffer[datLen++] = 0x00; //Reserved
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x20; //Additional Sense Code(ASC) 0x20,Indicates an invalid command opcode(INVALID COMMAND OPERATION CODE)
    sTlkUsbMscScsi.buffer[datLen++] = 0x00; //Additional Sense Code Qualifier(ASCQ)
    sTlkUsbMscScsi.buffer[datLen++] = 0x00; //Reserved
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.datLen           = datLen;
    sTlkUsbMscScsi.stage            = TLKUSB_MSC_SCSI_STAGE_DATA;
    sTlkUsbMscScsi.flags |= TLKUSB_MSC_SCSI_FLAG_SEND_DATA;
}

/**
 * @brief       Handle INQUIRY command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvInquiryDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pCmd;
    (void)cmdLen;
    char   *pStr;
    uint8_t tmpLen = 0;
    uint8_t datLen = 0;

    sTlkUsbMscScsi.buffer[datLen++] = 0x00; //Disk device
    if (pUnit->hotPlug) {
        sTlkUsbMscScsi.buffer[datLen++] = 0x80; //The most significant D7 is RMB.  RMB=0, which means the device cannot be removed.  If RMB=1, it is a removable device.
    } else {
        sTlkUsbMscScsi.buffer[datLen++] = 0x00; //The most significant D7 is RMB.  RMB=0, which means the device cannot be removed.  If RMB=1, it is a removable device.
    }
    sTlkUsbMscScsi.buffer[datLen++] = 0x02;           //ISOVersion[7~6], ECMA Version[5~3], ANSI Version[2~1]-0x02 new version usb
    sTlkUsbMscScsi.buffer[datLen++] = 0x02;           //Data response format
    sTlkUsbMscScsi.buffer[datLen++] = 0x1F;           //Additional data length, 31 bytes
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;           //Reserved
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;           //Reserved
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;           //Reserved
    tmemset(sTlkUsbMscScsi.buffer + datLen, 0x20, 8); //Manufacturer: 8 bytes
    if (pUnit->pVendorStr == NULL) {
        pStr = TLKUSB_MSC_VENDOR_STRING_DEF;
    } else {
        pStr = pUnit->pVendorStr;
    }
    tmpLen = strlen(pStr);
    if (tmpLen > 8) {
        tmpLen = 8;
    }
    if (tmpLen != 0) {
        tmemcpy(sTlkUsbMscScsi.buffer + datLen, pStr, tmpLen);
    }
    datLen += 8;
    tmemset(sTlkUsbMscScsi.buffer + datLen, 0x20, 16); //Product: 16 Bytes
    if (pUnit->pProductStr == NULL) {
        pStr = TLKUSB_MSC_PRODUCE_STRING_DEF;
    } else {
        pStr = pUnit->pProductStr;
    }
    tmpLen = strlen(pStr);
    if (tmpLen > 16) {
        tmpLen = 16;
    }
    if (tmpLen != 0) {
        tmemcpy(sTlkUsbMscScsi.buffer + datLen, pStr, tmpLen);
    }
    datLen += 16;
    tmemset(sTlkUsbMscScsi.buffer + datLen, 0x20, 4); //Version: 4 Bytes
    if (pUnit->pVersionStr == NULL) {
        pStr = TLKUSB_MSC_VERSION_STRING_DEF;
    } else {
        pStr = pUnit->pVersionStr;
    }
    tmpLen = strlen(pStr);
    if (tmpLen > 4) {
        tmpLen = 4;
    }
    if (tmpLen != 0) {
        tmemcpy(sTlkUsbMscScsi.buffer + datLen, pStr, tmpLen);
    }
    datLen += 4;

    sTlkUsbMscScsi.datLen = datLen;
    sTlkUsbMscScsi.stage  = TLKUSB_MSC_SCSI_STAGE_DATA;
    sTlkUsbMscScsi.flags |= TLKUSB_MSC_SCSI_FLAG_SEND_DATA;
}

/**
 * @brief       Handle MODE SENSE(6) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvModeSense6Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)pCmd;
    (void)cmdLen;
    uint8_t datLen                  = 0;
    sTlkUsbMscScsi.buffer[datLen++] = 0x03;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    //	sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    //	sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    //	sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    //	sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.datLen = datLen;
    sTlkUsbMscScsi.stage  = TLKUSB_MSC_SCSI_STAGE_DATA;
    sTlkUsbMscScsi.flags |= TLKUSB_MSC_SCSI_FLAG_SEND_DATA;
}

/**
 * @brief       Handle MODE SENSE(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvModeSense10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)pCmd;
    (void)cmdLen;
    uint8_t datLen                  = 0;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x06;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    //	sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    //	sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    //	sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    //	sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.datLen = datLen;
    sTlkUsbMscScsi.stage  = TLKUSB_MSC_SCSI_STAGE_DATA;
    sTlkUsbMscScsi.flags |= TLKUSB_MSC_SCSI_FLAG_SEND_DATA;
}

/**
 * @brief       Handle START STOP UNIT command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvStartStopUnitDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)pCmd;
    (void)cmdLen;
    sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
}

/**
 * @brief       Handle ALLOW MEDIUM REMOVAL command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvAllowMediumRemoveDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)cmdLen;
    //pCmd[4] bit0~1 <PREVENT>
    //PREVENT = 00b: Volume removal shall be allowed
    //PREVENT = 01b: Volume removal shall be prevented.
    //PREVENT = 10b: Obsolete
    //PREVENT = 11b: Obsolete
    sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
    if ((pCmd[4] & 0x03) == 0x01) {
        tlkusb_msc_scsiSetStatus(NULL, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
    }
}

/**
 * @brief       Handle READ FORMAT CAPACITIES command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvReadFormatCapacityDeal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pCmd;
    (void)cmdLen;
    uint8_t  datLen                 = 0;
    uint32_t blkCount               = pUnit->getBlkCount();
    sTlkUsbMscScsi.buffer[datLen++] = 0x00; //Reserved
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x00;
    sTlkUsbMscScsi.buffer[datLen++] = 0x08;                    //Capacity list length
    sTlkUsbMscScsi.buffer[datLen++] = (blkCount >> 24) & 0xFF; //Number of blocks (maximum support 8GB)
    sTlkUsbMscScsi.buffer[datLen++] = (blkCount >> 16) & 0xFF;
    sTlkUsbMscScsi.buffer[datLen++] = (blkCount >> 8) & 0xFF;
    sTlkUsbMscScsi.buffer[datLen++] = (blkCount & 0xFF);
    sTlkUsbMscScsi.buffer[datLen++] = 0x02;                          //The descriptor code is 2, indicating the maximum formatted capacity supported
    sTlkUsbMscScsi.buffer[datLen++] = (pUnit->blkSize >> 16) & 0xFF; //Each block size
    sTlkUsbMscScsi.buffer[datLen++] = (pUnit->blkSize >> 8) & 0xFF;
    sTlkUsbMscScsi.buffer[datLen++] = (pUnit->blkSize & 0xFF);
    sTlkUsbMscScsi.datLen           = datLen;
    sTlkUsbMscScsi.stage            = TLKUSB_MSC_SCSI_STAGE_DATA;
    sTlkUsbMscScsi.flags |= TLKUSB_MSC_SCSI_FLAG_SEND_DATA;
}

/**
 * @brief       Handle READ CAPACITY(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvReadCapacity10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pCmd;
    (void)cmdLen;
    uint8_t  datLen                 = 0;
    uint32_t blkCount               = pUnit->getBlkCount();
    sTlkUsbMscScsi.buffer[datLen++] = (blkCount >> 24) & 0xFF; //Number of blocks (maximum support 8GB)
    sTlkUsbMscScsi.buffer[datLen++] = (blkCount >> 16) & 0xFF;
    sTlkUsbMscScsi.buffer[datLen++] = (blkCount >> 8) & 0xFF;
    sTlkUsbMscScsi.buffer[datLen++] = (blkCount & 0xFF);
    sTlkUsbMscScsi.buffer[datLen++] = (pUnit->blkSize >> 24) & 0xFF;
    sTlkUsbMscScsi.buffer[datLen++] = (pUnit->blkSize >> 16) & 0xFF; //Each block size
    sTlkUsbMscScsi.buffer[datLen++] = (pUnit->blkSize >> 8) & 0xFF;
    sTlkUsbMscScsi.buffer[datLen++] = (pUnit->blkSize & 0xFF);
    sTlkUsbMscScsi.datLen           = datLen;
    sTlkUsbMscScsi.stage            = TLKUSB_MSC_SCSI_STAGE_DATA;
    sTlkUsbMscScsi.flags |= TLKUSB_MSC_SCSI_FLAG_SEND_DATA;
}

/**
 * @brief       Handle READ(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvRead10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)cmdLen;
    ARRAY_TO_UINT32H(pCmd, 2, sTlkUsbMscScsi.blkOff);
    ARRAY_TO_UINT16H(pCmd, 7, sTlkUsbMscScsi.blkCnt);
    sTlkUsbMscScsi.blkNum = 0;
    sTlkUsbMscScsi.stage  = TLKUSB_MSC_SCSI_STAGE_DATA;
    sTlkUsbMscScsi.flags |= TLKUSB_MSC_SCSI_FLAG_READ_DATA;
}

/**
 * @brief       Handle WRITE(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvWrite10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)cmdLen;
    ARRAY_TO_UINT32H(pCmd, 2, sTlkUsbMscScsi.blkOff);
    ARRAY_TO_UINT16H(pCmd, 7, sTlkUsbMscScsi.blkCnt);
    sTlkUsbMscScsi.blkNum = 0;
    sTlkUsbMscScsi.stage  = TLKUSB_MSC_SCSI_STAGE_DATA;
    sTlkUsbMscScsi.flags |= TLKUSB_MSC_SCSI_FLAG_RECV_DATA;
}

/**
 * @brief       Handle VERIFY(10) command.
 * @param[in]   pUnit - Pointer to disk unit.
 * @param[in]   pCmd - Command buffer.
 * @param[in]   cmdLen - Command length.
 * @return      none.
 */
static void tlkusb_msc_scsiRecvVerify10Deal(tlkusb_msc_disk_t *pUnit, uint8_t *pCmd, uint8_t cmdLen)
{
    (void)pUnit;
    (void)cmdLen;
    if ((pCmd[1] & 0x01) != 0) { //Evpd is set
        tlkusb_msc_scsiSetStatus(NULL, 0, TLKUSB_MSC_CSW_STATUS_CMD_FAILED);
    }
    sTlkUsbMscScsi.stage = TLKUSB_MSC_SCSI_STAGE_STATUS;
}

/**
 * @brief       Send data to USB MSC endpoint
 * @param[in]   pData    - pointer to the data buffer to send
 * @param[in]   dataLen  - length of data to send
 * @return      TLK_ENONE indicates success, others indicate failure
 */
static int tlkusb_msc_scsiSendData(uint8_t *pData, uint8_t dataLen)
{
    if (pData == NULL || dataLen == 0) {
        return -TLK_EPARAM;
    }
    if (usbhw_is_ep_busy(TLKUSB_MSC_EDP_IN)) {
        return -TLK_EBUSY;
    }

    usbhw_reset_ep_ptr((uint8_t)TLKUSB_MSC_EDP_IN); // send status back to PC
    for (size_t index = 0; index < dataLen; index++) {
        reg_usb_ep_dat(TLKUSB_MSC_EDP_IN) = pData[index];
    }
    usbhw_data_ep_ack(TLKUSB_MSC_EDP_IN); // set the ack
    return TLK_ENONE;
}


#endif //#if (TLK_USB_MSC_ENABLE)

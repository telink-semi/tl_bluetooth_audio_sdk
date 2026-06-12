/********************************************************************************************************
 * @file    tlkmdi_btatt.c
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
#if (TLKBTP_CFG_ATTSRV_ENABLE)
#include "tlkmw/bt/tlkmw_bt.h"

#include "stack/bt/host/btp/btp_stdio.h"
#include "stack/bt/host/bth/bth_l2cap.h"
#include "tlkmw/tlkmw.h"

static const btp_gattItem_t scTlkMdiBtAttItems[];

/**
 * @brief       This function sends OTA data via ATT
 * @param[in]   taskID    - Task ID
 * @param[in]   pData     - Pointer to data to send
 * @param[in]   dataLen   - Length of data to send
 * @param[in]   UserArg   - User argument
 * @return      0 if success, otherwise error code
 */
int tlkmdi_btatt_otaSendData(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    (void)UserArg;
    uint16_t handle = taskID & 0xFFFF;
    uint16_t chnID  = btp_attsrv_getChnID(handle);
    return btp_att_sendHandleValueNtf1(handle, chnID, TLKMDI_BTATT_OTA_CMD_OUT_DP_H, NULL, 0, pData, dataLen);
}

uint32_t tlkmdi_btatt_get_ota_param(uint32_t taskID, uint8_t param_type, void *UserArg)
{
    (void)taskID;
    (void)UserArg;

    if (param_type == TLKMW_OTA_PARAM_MTU_SIZE) {
        bth_l2cap_channel_t *pChannel = bth_l2cap_getConnChannelByPsm(taskID & 0xFFFF, BTP_PSMID_ATT);
        if (pChannel != NULL) {
            return pChannel->mtuSize;
        }
    }

    return 0;
}

/**
 * @brief       This function initializes the BT ATT module
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_btatt_init(void)
{
    btp_attsrv_setTable(scTlkMdiBtAttItems, TLKMDI_BTATT_HANDLE_MAX);

    sTlkMwUnitIntf_t interface = {
        .channel       = TLKMW_OTA_TRANS_CHN_BT_ATT,
        .send          = tlkmdi_btatt_otaSendData,
        .recv          = NULL,
        .get_ota_param = tlkmdi_btatt_get_ota_param,
    };

    tlkmw_ota_register_chn_interface(&interface);
    return TLK_ENONE;
}

/**
 * @brief       This function handles OTA write operations
 * @param[in]   handle    - Connection handle
 * @param[in]   chnID     - Channel ID
 * @param[in]   pData     - Pointer to received data
 * @param[in]   dataLen   - Length of received data
 * @return      BTP_ATT_ECODE_NONE if success
 */
static uint8_t tlkmdi_btatt_otaWriteCB(uint16_t handle, uint16_t chnID, uint8_t *pData, uint16_t dataLen)
{
    tlkmw_userctrl_pushDataToTask(handle | (chnID << 16), 0, pData, dataLen);
    return BTP_ATT_ECODE_NONE;
}


#if 1
#define BTP_GATT_CONN_INTERVAL_10MS 8
#define BTP_GATT_CONN_TIMEOUT_5S    500
static uint8_t sLepGapsDevNameValue[]     = {'G', 'A', 'T', 'T', ' ', 'B', 'R', '/', 'E', 'D', 'R'};
static uint8_t sLepGapsAppearanceValue[2] = {0x00, 0x09}; // 384
static uint8_t sLepGapsConnParamValue[8]  = {
    (BTP_GATT_CONN_INTERVAL_10MS) & 0x00FF,
    ((BTP_GATT_CONN_INTERVAL_10MS) & 0xFF00) >> 8, // Min Interval
    (BTP_GATT_CONN_INTERVAL_10MS) & 0x00FF,
    ((BTP_GATT_CONN_INTERVAL_10MS) & 0xFF00) >> 8, // Max Interval
    99,
    00, // Latency
    (BTP_GATT_CONN_TIMEOUT_5S) & 0x00FF,
    ((BTP_GATT_CONN_TIMEOUT_5S) & 0xFF00) >> 8, // Timeout
};
static const uint8_t scLepGapsDevNameChar[5] = {
    BTP_GATT_PROPERTY_READ,
    SPLIT_WORD(TLKMDI_BTATT_GAP_DEVICE_NAME_DP_H),
    SPLIT_WORD(BTP_CHARACT_UUID_DEVICE_NAME),
};
static const uint8_t scLepGapsAppearanceChar[5] = {
    BTP_GATT_PROPERTY_READ,
    SPLIT_WORD(TLKMDI_BTATT_GAP_APPEARANCE_DP_H),
    SPLIT_WORD(BTP_CHARACT_UUID_APPEARANCE),
};
static const uint8_t scLepGapsConnParamChar[5] = {
    BTP_GATT_PROPERTY_READ,
    SPLIT_WORD(TLKMDI_BTATT_GAP_CONN_PARAM_DP_H),
    SPLIT_WORD(BTP_CHARACT_UUID_PERI_CONN_PARAM),
};
#endif
#if 1
static const uint8_t scTlkMdiBtAttGattServiceChangedChar[5] = {
    BTP_GATT_PROPERTY_INDICATE,
    SPLIT_WORD(TLKMDI_BTATT_GATT_SERVICE_CHANGED_DP_H),
    SPLIT_WORD(BTP_CHARACT_UUID_SERVICE_CHANGE),
};
static const uint8_t scTlkMdiBtAttGattServiceChangedValue[2] = {0x00, 0x00};
static uint16_t      scTlkMdiBtAttGattServiceChangedCCC      = 0;
#endif
#if 1
static const uint8_t scTlkMdiBtAttDevPnpIDChar[5] = {
    BTP_GATT_PROPERTY_READ,
    SPLIT_WORD(TLKMDI_BTATT_DEV_PNPID_CD_H),
    SPLIT_WORD(BTP_CHARACT_UUID_PNP_ID),
};
static const u8 scTlkMdiBtAttDevPnpIDValue[] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};
#endif
#if 1
static uint8_t       sTlkMdiBtAttOtaData         = 0x00;
static uint8_t       sTlkMdiBtAttOtaDataCCC[2]   = {0, 0};
static const uint8_t scTlkMdiBtAttOtaSrvUUID[16] = {0x12, 0x19, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static const uint8_t scTlkMdiBtAttOtaDatUUID[16] = {0x12, 0x2B, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00};
static const uint8_t sTlkMdiBtAttOtaName[]       = {'O', 'T', 'A'};
static const uint8_t sTlkMdiBtAttOtaCharVal[19]  = {BTP_GATT_PROPERTY_WRITE_WITHOUT_RSP | BTP_GATT_PROPERTY_NOTIFY,
                                                    SPLIT_WORD(TLKMDI_BTATT_OTA_CMD_OUT_DP_H),
                                                    0x12,
                                                    0x2B,
                                                    0x0d,
                                                    0x0c,
                                                    0x0b,
                                                    0x0a,
                                                    0x09,
                                                    0x08,
                                                    0x07,
                                                    0x06,
                                                    0x05,
                                                    0x04,
                                                    0x03,
                                                    0x02,
                                                    0x01,
                                                    0x00};
#endif

static const btp_gattItem_t scTlkMdiBtAttItems[TLKMDI_BTATT_HANDLE_MAX + 1] = {
    {TLKMDI_BTATT_HANDLE_MAX - 1, 0, 0, 0, 0, 0, 0, 0},
    //  Gap
    {7, BTP_ATT_PERMIT_READ, 2, 2, (uint8_t *)cBtpUUID16Primary, (uint8_t *)cBtpUUID16GapService, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(scLepGapsDevNameChar), (uint8_t *)cBtpUUID16Charact, (uint8_t *)scLepGapsDevNameChar, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(sLepGapsDevNameValue), (uint8_t *)cBtpUUID16DeviceName, (uint8_t *)sLepGapsDevNameValue, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(scLepGapsAppearanceChar), (uint8_t *)cBtpUUID16Charact, (uint8_t *)scLepGapsAppearanceChar, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(sLepGapsAppearanceValue), (uint8_t *)cBtpUUID16Appearance, (uint8_t *)sLepGapsAppearanceValue, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(scLepGapsConnParamChar), (uint8_t *)cBtpUUID16Appearance, (uint8_t *)scLepGapsConnParamChar, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(sLepGapsConnParamValue), (uint8_t *)cBtpUUID16PeriConnParam, (uint8_t *)sLepGapsConnParamValue, 0, 0},
    //  Gatt
    {4, BTP_ATT_PERMIT_READ, 2, 2, (uint8_t *)cBtpUUID16Primary, (uint8_t *)cBtpUUID16GattService, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(scTlkMdiBtAttGattServiceChangedChar), (uint8_t *)cBtpUUID16Charact, (uint8_t *)scTlkMdiBtAttGattServiceChangedChar, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(scTlkMdiBtAttGattServiceChangedValue), (uint8_t *)cBtpUUID16ServiceChanged, (uint8_t *)scTlkMdiBtAttGattServiceChangedValue, 0, 0},
    {0, BTP_ATT_PERMIT_RDWR, 2, sizeof(scTlkMdiBtAttGattServiceChangedCCC), (uint8_t *)cBtpUUID16ClientCharCfg, (uint8_t *)&scTlkMdiBtAttGattServiceChangedCCC, 0, 0},
    //  Device
    {3, BTP_ATT_PERMIT_READ, 2, 2, (uint8_t *)cBtpUUID16Primary, (uint8_t *)cBtpUUID16DevService, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(scTlkMdiBtAttDevPnpIDChar), (uint8_t *)cBtpUUID16Charact, (uint8_t *)scTlkMdiBtAttDevPnpIDChar, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(scTlkMdiBtAttDevPnpIDValue), (uint8_t *)cBtpUUID16PnpID, (uint8_t *)scTlkMdiBtAttDevPnpIDValue, 0, 0},
    //  OTA
    {5, BTP_ATT_PERMIT_READ, 2, 16, (uint8_t *)cBtpUUID16Primary, (uint8_t *)scTlkMdiBtAttOtaSrvUUID, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(sTlkMdiBtAttOtaCharVal), (uint8_t *)cBtpUUID16Charact, (uint8_t *)sTlkMdiBtAttOtaCharVal, 0, 0},
    {0, BTP_ATT_PERMIT_READ | BTP_ATT_PERMIT_WRITE_WITHOUT_RSP, 2, sizeof(sTlkMdiBtAttOtaData), (uint8_t *)scTlkMdiBtAttOtaDatUUID, (uint8_t *)&sTlkMdiBtAttOtaData, 0,
     tlkmdi_btatt_otaWriteCB},
    {0, BTP_ATT_PERMIT_RDWR, 2, sizeof(sTlkMdiBtAttOtaDataCCC), (uint8_t *)cBtpUUID16ClientCharCfg, (uint8_t *)sTlkMdiBtAttOtaDataCCC, 0, 0},
    {0, BTP_ATT_PERMIT_READ, 2, sizeof(sTlkMdiBtAttOtaName), (uint8_t *)cBtpUUID16CharUserDesc, (uint8_t *)sTlkMdiBtAttOtaName, 0, 0},
};


#endif // #if (TLKBTP_CFG_ATTSRV_ENABLE)
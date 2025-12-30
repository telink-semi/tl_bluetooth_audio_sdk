/********************************************************************************************************
 * @file    tlkmdi_btatt.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef TLKMDI_BTATT_H
#define TLKMDI_BTATT_H

typedef enum
{
    TLKMDI_BTATT_HANDLE_START = 0,

    TLKMDI_BTATT_GAP_PS_H,
    TLKMDI_BTATT_GAP_DEVICE_NAME_CD_H,
    TLKMDI_BTATT_GAP_DEVICE_NAME_DP_H,
    TLKMDI_BTATT_GAP_APPEARANCE_CD_H,
    TLKMDI_BTATT_GAP_APPEARANCE_DP_H,
    TLKMDI_BTATT_GAP_CONN_PARAM_CD_H,
    TLKMDI_BTATT_GAP_CONN_PARAM_DP_H,

    TLKMDI_BTATT_GATT_PS_H,
    TLKMDI_BTATT_GATT_SERVICE_CHANGED_CD_H,
    TLKMDI_BTATT_GATT_SERVICE_CHANGED_DP_H,
    TLKMDI_BTATT_GATT_SERVICE_CHANGED_CCB_H,

    TLKMDI_BTATT_DEV_PS_H,
    TLKMDI_BTATT_DEV_PNPID_CD_H,
    TLKMDI_BTATT_DEV_PNPID_DP_H,

    TLKMDI_BTATT_OTA_PS_H,
    TLKMDI_BTATT_OTA_CMD_OUT_CD_H,
    TLKMDI_BTATT_OTA_CMD_OUT_DP_H,
    TLKMDI_BTATT_OTA_CMD_INPUT_CCB_H,
    TLKMDI_BTATT_OTA_CMD_OUT_DESC_H,

    TLKMDI_BTATT_HANDLE_MAX,
} TLKMDI_BTATT_HANDLE_ENUM;

typedef void (*TlkMdiBtAttOtaWriteCB)(uint16_t handle, uint16_t chnID, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function initializes the BT ATT module
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_btatt_init(void);

/**
 * @brief       This function sends OTA data via ATT
 * @param[in]   taskID    - Task ID
 * @param[in]   pData     - Pointer to data to send
 * @param[in]   dataLen   - Length of data to send
 * @param[in]   UserArg   - User argument
 * @return      0 if success, otherwise error code
 */
int tlkmdi_btatt_otaSendData(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg);


#endif // #ifndef TLKMDI_BTATT_H

/********************************************************************************************************
 * @file    tlkmdi_btsco.h
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
#ifndef TLKMDI_BTSCO_H
#define TLKMDI_BTSCO_H

#define TLKMDI_BTSCO_MAX_NUM       2
#define TLKMDI_BTSCO_CODEC_ID_CVSD 1
#define TLKMDI_BTSCO_CODEC_ID_MSBC 2

enum TLKMDI_BTSCO_ROLE_ENUM
{
    TLKMDI_BTSCO_ROLE_NONE = 0,
    TLKMDI_BTSCO_ROLE_SCO,
    TLKMDI_BTSCO_ROLE_DSCO,
};

typedef void (*TlkMdiBtScoConnCB)(uint16_t aclHandle, uint16_t scoHandle, bool isConn);

typedef struct
{
    uint16_t aclHandle;
    uint16_t scoHandle;
    uint8_t codec;
    uint8_t resv[3];
} tlkmdi_btsco_handle_t;

typedef struct
{
    uint32_t                scoCnt;
    tlkmdi_btsco_handle_t pItem[TLKMDI_BTSCO_MAX_NUM];
} tlkmdi_btsco_ctrl_t;

/**
 * @brief       This function is used to initialize the SCO module
 * @param       none.
 * @return      TLK_ENONE if success.
 */
int tlkmdi_btsco_init(void);

/**
 * @brief       This function is used to register SCO connection callback
 * @param[in]   connCB - the connection callback function
 * @return      none.
 */
void tlkmdi_btsco_regCB(TlkMdiBtScoConnCB connCB);




#endif // TLKMDI_BTSCO_H

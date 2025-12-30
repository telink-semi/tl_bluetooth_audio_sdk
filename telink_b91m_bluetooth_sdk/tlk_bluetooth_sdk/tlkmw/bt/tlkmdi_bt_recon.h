/********************************************************************************************************
 * @file    tlkmdi_bt_recon.h
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
#ifndef TLKMDI_BT_RECON_H
#define TLKMDI_BT_RECON_H


#define TLKMDI_BTRECON_CHECK_INTERVAL 5000     //5ms
#define TLKMDI_BTRECON_PAGE_INTERVAL  10000000 //10S
#define TLKMDI_BTRECON_INTERVAL_TIME  2000000  //2S the wait time between two page actions, open sacn!

typedef enum
{
    TLKMDI_BTRECON_STATE_IDLE = 0,
    TLKMDI_BTRECON_STATE_START,
    TLKMDI_BTRECON_STATE_PAGE,

    TLKMDI_BTRECON_STATE_WAIT_PAGE_CANCEL,

    TLKMDI_BTRECON_STATE_INTERVAL,
    TLKMDI_BTRECON_CANCEL_SCAN,
    TLKMDI_BTRECON_STATE_WAIT_PROFILE,

    TLKMDI_BTRECON_STATE_WAIT_STOP,
} TLKMDI_BTRECON_STATE_ENUM;

typedef struct
{
    uint8_t         retry_num;
    uint8_t         state;
    uint8_t         pageAddr[6]; //The device to be connected back.
    
    uint32_t         devClass;
    TlkApiTimer_t  timer_recon;
    TlkApiTimer_t  timer_state_check;
} tlkmdi_btrecon_t;

/**
 * @brief       This function checks the Bluetooth reconnection is in a busy state.
 * @return      true is busy. If the state is between TLKMDI_BTRECON_STATE_IDLE and TLKMDI_BTRECON_STATE_WAIT_STOP.
 *              false not busy.
 * @note        
 */
bool tlkmdi_btRecon_isInBusy(void);

/**
 * @brief       This function get current reconnect state.
 * @param[in]   none;
 * @return      sTlkMdiBtReconCtrl.state.
 * @note        none;
 */
uint8_t tlkmdi_get_btRecon_state(void);

/**
 * @brief       This function get current page addr of remote device.
 * @param[in]   none;
 * @return      BT addr pointer.
 * @note        none;        
 */
uint8_t *tlkmdi_btRecon_getPageAddr(void);

/**
 * @brief       This function is start BT reconnection process call by app layer.
 * @param[in]   pPageAddr : Remote device addr pointer. 
 * @param[in]   devClass  : The class of device of remote device.
 * @param[in]   retry_num : Reconnection retry number(number 1 is about 12s,10s page+2s open page_sacn)
 * @return      The process result.
 * @note        none.
 */
int tlkmdi_btRecon_start(uint8_t *pPageAddr, uint32_t devClass, uint8_t retry_num);

/**
 * @brief       This function is stop BT reconnection process call by app layer.
 * @param[in]   none
 * @return      The process result.
 * @note        none.
 */
int tlkmdi_btRecon_close(void);

/**
 * @brief       This function returns the remind retry_num from the recon control structure.
 * @return      The remaining retry_num.
 * @note        
 */
uint8_t tlkmdi_bt_recon_getRemindRetryNum(void);

#endif

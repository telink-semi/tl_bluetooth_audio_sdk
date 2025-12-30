/********************************************************************************************************
 * @file    tlkmdi_btscan.h
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
#ifndef TLKMDI_BTSCAN_H
#define TLKMDI_BTSCAN_H

#define TLKMDI_BTSCAN_TIME_UNIT 1000000 //1S

typedef enum
{
    TLKMDI_BTSCAN_MODE_BOTH_DISABLE,
    TLKMDI_BTSCAN_MODE_INQUIRY_SCAN,
    TLKMDI_BTSCAN_MODE_PAGE_SCAN,
    TLKMDI_BTSCAN_MODE_BOTH_SCAN,
} TLKMDI_BTSCAN_ACTIVE_MODE_ENUM;

typedef struct
{
    uint8_t         cur_scan_state;
    uint8_t         waiting_confirm_value : 4; /* waiting hci set scan command complete */
    uint8_t         waiting_flag          : 2; /* waiting hci set scan command complete */
    uint8_t         pending_scanflag      : 2;
    uint16_t       waiting_confirm_value_time;
    uint8_t         pending_scan_value;        /* during waiting hci set scan command complete, system set a new value */
    uint8_t         reserve[3];
    uint16_t       pending_scan_value_time;
    uint16_t         timeout;                   /* the number of 1s unit */
    TlkApiTimer_t  timer;
} tlkmdi_bt_scan_t;

/**
 * @brief       This function initializes the Bluetooth scan process by creating a static timer.
 * @return      none.
 * @note        The timer is configured to run in the HOST task, with a time unit defined by TLKMDI_BTSCAN_TIME_UNIT.
 *              It uses the tlkmdi_btSetScan_timer function as its callback.
 */
void tlkmdi_btScan_process_init(void);

/**
 * @brief       This function processes the HCI command complete event callback for setting the scan.
 * @param[in]   pData   - pointer to the data received in the HCI command event. Only include result status.
 * @param[in]   dataLen - length of hci event buffer.
 * @return      none.
 * @note        The function checks the status of the HCI command and updates the scan state accordingly.
 */
int tlkmdi_btSetScan_hciCmdEvt_cb(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function is called by the application to set the Bluetooth scan mode.
 * @param[in]   scan_value - the scan mode to be set.
 * @param[in]   time       - time duration, if scan_value is TLKMDI_BTSCAN_MODE_BOTH_DISABLE time will be neglect. Unit:S.
 * 					         If value is not TLKMDI_BTSCAN_MODE_BOTH_DISABLE and time is set as 0, the scan will always on.
 *                           If time is set as 0xFFFF, then only update the scan value, timeout continues to count down.
 * @return      none.
 * @note        If the scan mode is already set to the same value, the function returns mode immediately. If the scan mode is different,
 *              the function sends a HCI command to set the scan mode. If the HCI command is sent successfully, the function sets a timer
 *              to wait for the HCI command complete event. If the HCI command fails, the function sets the scan mode as pending and
 *              waits for the next scan mode change. If the scan mode is set to TLKMDI_BTSCAN_MODE_BOTH_DISABLE, the function stops the timer.
 *              If the scan mode is set to a value other than TLKMDI_BTSCAN_MODE_BOTH_DISABLE, the function starts the timer with the specified time.
 */
void tlkmdi_btSet_scan(uint8_t scan_value, uint16_t time);

/**
 * @brief       This function get the current scan state based on the scan control flags.
 * @return      The current state or pending state(**consider will be set successful) .
 * @note        If waiting_flag is not set, it returns cur_scan_state.
 *              If pending_scan_value_time is set, it returns pending_scan_value.
 *              Otherwise, it returns waiting_confirm_value.
 */
uint8_t tlkmdi_btGetScan_state(void);

/**
 * @brief       This function returns the remaining scan time from the scan control structure.
 * @return      The remaining scan time.
 * @note        
 */
uint16_t tlkmdi_btscan_getRemainedScanTime(void);

/**
 * @brief       This function retrieves the current scan state.
 * @return      The current scan state.
 * @note        
 */
uint8_t tlkmdi_btscan_getCurScanState(void); 

#endif //TLKMDI_BTSCAN_H

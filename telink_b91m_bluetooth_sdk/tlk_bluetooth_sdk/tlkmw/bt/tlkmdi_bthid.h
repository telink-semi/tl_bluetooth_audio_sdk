/********************************************************************************************************
 * @file    tlkmdi_bthid.h
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
#ifndef TLKMDI_HID_H
#define TLKMDI_HID_H


#define TLKMDI_BTHID_REPORT_ID_KEYBOARD_INPUT 1 //!< Keyboard input report ID, Bond with gcBtpSdpHidReportMap.
#define TLKMDI_BTHID_REPORT_ID_CONSUMER_INPUT 2 //!< Consumer Control input report ID, Bond with gcBtpSdpHidReportMap.
#define TLKMDI_BTHID_REPORT_ID_GAMEPAD_INPUT  1 //!< Gamepad  input report ID

typedef struct
{
    uint8_t  enChg; // Enable SetReport
    uint8_t  bsize; // Buffer Size
    uint8_t  rtype; // Report Type
    uint8_t  rptID; // Report ID
    uint8_t  dlens; // Data Length
    uint8_t *pData;
} tlkmdi_bthid_report_t;

/**
 * @brief       This function initializes the HID module
 * @param[in]   none
 * @return      TLK_ENONE
 */
int tlkmdi_bthid_init(void);

/**
 * @brief       This function connects to HID device
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hidd_connect result
 */
int tlkmdi_bthid_connect(uint16_t aclHandle);

/**
 * @brief       This function disconnects from HID device
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hidd_disconn result
 */
int tlkmdi_bthid_disconn(uint16_t aclHandle);

/**
 * @brief       This function sends HID data with report ID
 * @param[in]   aclHandle    - ACL connection handle
 * @param[in]   reportID     - report ID
 * @param[in]   reportType   - report type
 * @param[in]   pData        - pointer to data to send
 * @param[in]   dataLen      - length of data to send
 * @return      btp_hidd_sendData result
 */
int tlkmdi_bthid_sendData(uint16_t aclHandle, uint8_t reportID, uint8_t reportType, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function sends HID data without report ID
 * @param[in]   aclHandle    - ACL connection handle
 * @param[in]   reportType   - report type
 * @param[in]   pData        - pointer to data to send
 * @param[in]   dataLen      - length of data to send
 * @return      btp_hidd_sendDataWithoutReportID result
 */
int tlkmdi_bthid_sendDataWithoutReportID(uint16_t aclHandle, uint8_t reportType, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function increases volume via HID consumer control
 * @param[in]   none
 * @return      TLK_ENONE if success, otherwise error code
 */
int tlkmdi_bthid_ctrlVolInc(void);

/**
 * @brief       This function decreases volume via HID consumer control
 * @param[in]   none
 * @return      TLK_ENONE if success, otherwise error code
 */
int tlkmdi_bthid_ctrlVolDec(void);

/**
 * @brief       This function tests XInput gamepad functionality
 * @param[in]   none
 * @return      TLK_ENONE if success, otherwise error code
 */
int tlkmdi_bthid_ctrlXinputTest(void);

#endif // TLKMDI_HID_H
/********************************************************************************************************
 * @file    tlkmdi_bthfphf.h
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
#ifndef TLKMDI_BTHFPHF_H
#define TLKMDI_BTHFPHF_H

#define TLKMDI_HFPHF_TIMEOUT               100000
#define TLKMDI_HFPHF_CALL_WAIT_TIMEOUT     (3000000 / TLKMDI_HFPHF_TIMEOUT)

#define TLKMDI_HFPHF_NUMBER_MAX_LEN        32

#define TLKMDI_HFPHF_THREE_WAY_CALL_ENABLE 1
#define TLKMDI_HFPHF_MAX_NUMBER            2

typedef enum
{
    TLKMDI_HFPHF_CALL_STATUS_NONE = 0,
    TLKMDI_HFPHF_CALL_STATUS_ESTABLISH,
} TLKMDI_HFPHF_CALL_STATUS_ENUM;

typedef struct
{
    uint8_t codec;
    uint8_t status;
    uint8_t setup_status;
    uint8_t held_status;
    uint8_t numbLen;
    uint8_t reserve;
    uint16_t handle;
    uint8_t number[TLKMDI_HFPHF_NUMBER_MAX_LEN];
} tlkmdi_hfphf_ctrl_t;

//send inner msg
typedef struct
{
    uint8_t codec;
    uint16_t handle;
} tlkmdi_hfphf_codecEvt_t;

typedef struct
{
    uint16_t handle;
    uint8_t callNum; // 0 or 1
    uint8_t callDir;
    uint8_t numbLen;
    uint8_t number[TLKMDI_HFPHF_NUMBER_MAX_LEN];
} tlkmdi_hfphf_statusEvt_t;

/**
 * @brief       This function initializes the HFP HF module
 * @param[in]   none
 * @return      Return TLK_ENONE is success, other's value is false.
 */
int  tlkmdi_bthfphf_init(void);

/**
 * @brief       This function resets HFP HF module by handle
 * @param[in]   aclHandle   - ACL connection handle
 * @return      none
 */
void tlkmdi_bthfphf_reset(uint16_t aclHandle);

/**
 * @brief       This function controls assistant (Siri) function
 * @param[in]   handle   - connection handle
 * @return      TLK_ENONE if success, otherwise error code
 */
int  tlkmdi_bthfphf_assistant(uint16_t handle);

/**
 * @brief       This function gets HFP control item by handle
 * @param[in]   handle   - connection handle
 * @return      pointer to HFP control item if found, NULL otherwise
 */
tlkmdi_hfphf_ctrl_t *tlkmdi_hfphf_getItem(uint16_t handle);

/**
 * @brief       This function gets idle HFP control item
 * @param[in]   none
 * @return      pointer to idle HFP control item if found, NULL otherwise
 */
tlkmdi_hfphf_ctrl_t *tlkmdi_hfphf_getIdleItem(void);

/**
 * @brief       This function gets call number by ACL handle
 * @param[in]   aclHandle   - ACL connection handle
 * @return      pointer to call number if found, NULL otherwise
 */
uint8_t *tlkmdi_bthfphf_getCallNumber(uint16_t aclHandle);

/**
 * @brief       This function rejects waiting call and keeps active call
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hfphf_rejectWaitAndKeepActive result
 */
int     tlkmdi_bthfphf_rejectWaitAndKeepActive(uint16_t aclHandle);

/**
 * @brief       This function accepts waiting call and holds active call
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hfphf_acceptWaitAndHoldActive result
 */
int     tlkmdi_bthfphf_acceptWaitAndHoldActive(uint16_t aclHandle);

/**
 * @brief       This function hangs up active call and resumes held call
 * @param[in]   aclHandle   - ACL connection handle
 * @return      btp_hfphf_hungUpActiveAndResumeHold result
 */
int     tlkmdi_bthfphf_hungupActiveAndResumeHold(uint16_t aclHandle);

/**
 * @brief       This function gets handle by index
 * @param[in]   index   - item index
 * @return      handle if success, otherwise error code
 */
uint16_t  tlkmdi_bthfphf_getHandle(int index);

/**
 * @brief       This function checks if HF is in active state
 * @param[in]   none
 * @return      handle if HF is active, otherwise 0
 */
uint16_t tlkmdi_byhfphf_checkHfIsActive(void);
#endif // TLKMDI_BTHFPHF_H

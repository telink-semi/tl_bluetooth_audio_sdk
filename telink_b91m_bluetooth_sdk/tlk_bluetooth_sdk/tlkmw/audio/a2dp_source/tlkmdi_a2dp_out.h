/********************************************************************************************************
 * @file    tlkmdi_a2dp_out.h
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
#ifndef TLKMDI_A2DP_OUT_H
#define TLKMDI_A2DP_OUT_H

#define TLKMDI_A2DP_OUT_TYPE_NONE            0x00
#define TLKMDI_A2DP_OUT_TYPE_LINEIN          0x01 //TODO: Rework complex sounds.
#define TLKMDI_A2DP_OUT_TYPE_USB_AUDIO       0x02
#define TLKMDI_A2DP_OUT_TYPE_SINE_WAVE       0x03


#define TLKMDI_A2DP_OUT_FRAME_SIZE           84 //85 //For 32bitPool
#define TLKMDI_A2DP_OUT_PKT_BUFF_SIZE        680
#define TLKMDI_A2DP_OUT_SBC_ENC_BUFF_SIZE    3368
#define TLKMDI_A2DP_OUT_FRAME_MAX_NUMB       7       //(TLKMDI_SRC_PKT_BUFF_SIZE-13)/TLKMDI_SRC_FRAME_SIZE
#define TLKMDI_A2DP_OUT_WAIT_RECFG_TIMEOUT   3000000 //Unit:us
#define TLKMDI_A2DP_OUT_WAIT_USBCTRL_TIMEOUT 2000000 //Unit:us

typedef struct
{
    uint8_t  startflag  : 1;
    uint8_t  isNeedAsrc : 1;
    uint8_t  running    : 1;
    uint8_t  enable     : 1;
    uint16_t resv12bit  : 12;
    uint16_t handle;

    uint8_t cacheFrm; //Cache Frames
    uint8_t waitFlag;
    uint8_t waitStart;

    uint8_t sndFrame;
    uint8_t pktNumb; //Frame number in one media packet
    uint8_t frmSize; //85

    uint16_t seqNumber;
    uint16_t pktTime;           //128*pktNumb
    uint16_t samples_per_frame; //The playback time of one frame of SBC(tick)
    uint16_t playtimePerFrame;  //The playback time of one frame of SBC(us)

    uint32_t refTime;
    uint32_t lagTime;
    uint32_t unitTime;
    uint32_t timeStamp;
    uint32_t waitTimer;
    uint32_t runTimer;

    uint8_t *pA2dpFramesBuff;
    uint8_t *pA2dpEncBuff;
#if (TLK_ALG_PPM_ENABLE && TLKALG_PPM_SPK_ENABLE)
    uint8_t *pA2dpPPMBuff;
#endif
} tlkmdi_a2dp_out_ctrl_t;

typedef enum
{
    TLKMDI_A2DP_OUT_WAIT_START_READY  = 0, // ready
    TLKMDI_A2DP_OUT_WAIT_START_SMPMIS = 1, // sample rate mismatch
    TLKMDI_A2DP_OUT_WAIT_START_BUSY   = 2, // A2DP reconfig busy
    TLKMDI_A2DP_OUT_WAIT_START_CLOSE  = 3, // abnormal state
} TLKMDI_A2DP_OUT_WAIT_START_ENUM;

typedef enum
{
    TLKMDI_A2DP_OUT_WAIT_FLAG_SEND      = 0x01,
    TLKMDI_A2DP_OUT_WAIT_FLAG_START_RSP = 0x02,
    TLKMDI_A2DP_OUT_WAIT_FLAG_USB_START = 0x04,
} TLKMDI_A2DP_OUT_WAIT_FLAG_ENUM;

/**
 * @brief      This function is used to handle specified instructions in this scenario.
 * @param[in]  handle - the ACL handle.
 * @param[in]  opcode - the operation code.
 * @param[in]  pdata - the parameter data.
 * @param[in]  dataLen - the length of the parameter data.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_operate(uint16_t handle, uint8_t opcode, uint8_t *pdata, uint16_t dataLen);

/**
 * @brief      This function is used to actively trigger play previous.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_previous(void);

/**
 * @brief      This function is used to actively trigger play next.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_next(void);

/**
 * @brief      This function is used to actively trigger the pause of music playback.
 * @param[in]  handle - the ACL handle.
 * @param[in]  param - the parameter of the pause.
 * @return     TLK_ENONE is success, others is fail.
*/
int tlkmdi_a2dp_out_close(uint16_t handle);

/**
 * @brief      This function is used to actively trigger the start of music playback.
 * @param[in]  handle - the ACL handle.
 * @param[in]  param - the parameter of the start.
 * @return     TLK_ENONE is success, others is fail.
*/
int tlkmdi_a2dp_out_start(uint16_t handle, uint32_t param);

/**
 * @brief      This function is used to get the audio task is busy or not.
 * @return     true is busy, false is not busy.
*/
bool tlkmdi_a2dp_out_isBusy(void);

/**
 * @brief      This function is used to get the audio task interval.
 * @return     the interval of the audio task.
*/
uint32_t tlkmdi_a2dp_out_intval(void);

/**
 * @brief      This function is used to start or stop the audio task.
 * @param[in]  handle - the ACL handle.
 * @param[in]  status - the status of the audio task.
 * @return     true is success, false is fail.
*/
bool tlkmdi_a2dp_out_switch(uint16_t handle, uint8_t status);

/**
 * @brief       This function handles A2DP output timer events, including timeout handling and sample rate configuration.
 * @return      none.
 */
void tlkmdi_a2dp_out_timer(void);

/**
 * @brief   Initialize the A2DP output module
 * @note    This function initializes the A2DP output control structure and registers
 *          the AVRCP key change callback function
 * @param   None
 * @return  TLK_ENONE - Success
 */
int tlkmdi_a2dp_out_init(void);

#endif //TLKMDI_A2DP_OUT_H

/********************************************************************************************************
 * @file    tlkmdi_audio_btif_volume.c
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
#include "tlkmw/tlkmw.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/host/btp/btp_stdio.h"
#if (TLK_MW_AUDIO_ENABLE && TLK_STK_BT_ENABLE)

/*note: for thread safe ,use msgq instead of api directly use*/

static const uint8_t c_avrcp_vol_tab_ios[17]     = {0x00, 0x07, 0x0f, 0x17, 0x1f, 0x27, 0x2f, 0x37, 0x3f, 0x47, 0x4f, 0x57, 0x5f, 0x67, 0x6f, 0x77, 0x7f};
static const uint8_t c_avrcp_vol_tab_android[16] = {0x00, 0x09, 0x11, 0x1a, 0x22, 0x2b, 0x33, 0x3c, 0x44, 0x4d, 0x55, 0x5e, 0x66, 0x6f, 0x77, 0x7f};

/**
 * @brief       Convert volume value to volume step for iOS or Android devices
 * @param[in]   is_ios - Flag indicating if device is iOS (1) or Android (0)
 * @param[in]   volume_value - Volume value to convert
 * @return      Volume step corresponding to the volume value
 */
static uint8_t tlkmdi_audio_get_music_vol_step(uint8_t is_ios, uint8_t volume_value)
{
    uint8_t *p_tab     = NULL;
    uint8_t  max_index = 0;

    if (is_ios) {
        p_tab     = (uint8_t *)c_avrcp_vol_tab_ios;
        max_index = 17;
    } else {
        p_tab     = (uint8_t *)c_avrcp_vol_tab_android;
        max_index = 16;
    }

    if (volume_value == 0) {
        return 0;
    }
    for (uint8_t i = 1; i < max_index; i++) {
        if (volume_value > p_tab[i - 1] && volume_value <= p_tab[i]) {
            return i;
        }
    }
    return max_index;
}

/**
 * @brief       Convert volume step to volume value for iOS or Android devices
 * @param[in]   is_ios - Flag indicating if device is iOS (1) or Android (0)
 * @param[in]   step - Volume step to convert
 * @return      Volume value corresponding to the volume step
 */
static uint8_t tlkmdi_audio_getBtpVolumeByStep(uint8_t is_ios, uint8_t step)
{
    uint8_t *p_tab = NULL;
    //uint8_t max_index = 0;
    uint8_t volume;

    if (is_ios) {
        p_tab = (uint8_t *)c_avrcp_vol_tab_ios;
        //max_index = 17;
    } else {
        p_tab = (uint8_t *)c_avrcp_vol_tab_android;
        // max_index = 16;
    }

    if (step == 0) {
        return 0;
    }
    volume = p_tab[step];

    return volume;
}

/**
 * @brief       Get volume step value for device
 * @param[in]   handle - ACL connection handle
 * @param[in]   isMusic - Music or voice flag
 * @param[out]  step - Pointer to store volume step value
 * @param[out]  isIos - Pointer to store iOS device flag
 * @return      TLK_ENONE: success; Others: failure
 */
int tlkmdi_audio_btif_getVolumeStep(uint16_t handle, uint8_t isMusic, uint8_t *step, uint8_t *isIos)
{
    uint8_t           vol  = 0;
    bth_acl_handle_t *item = bth_handle_getConnAcl(handle);
    if (item == NULL) {
        return -TLK_ESEEK;
    }
    int ret = tlkmdi_tinySql_getPairingDeviceVolume(item->btaddr, isMusic, &vol, isIos);
    if (ret != TLK_ENONE) {
        return ret;
    }
    if (isMusic) {
        vol = tlkmdi_audio_get_music_vol_step(*isIos, vol); //val -> step
    }
    if (step != NULL) {
        *step = vol;
    }
    return TLK_ENONE;
}

/**
 * @brief       Operate volume (increase or decrease)
 * @param[in]   handle - ACL connection handle
 * @param[in]   isInc - Increase flag (1: increase, 0: decrease)
 * @param[in]   isMusic - Music or voice flag
 * @return      TLK_ENONE: success; Others: failure
 */
int tlkmdi_audio_btif_VolumeOperate(uint16_t handle, uint8_t isInc, uint8_t isMusic)
{
    (void)isMusic;
    uint8_t isIos = 0;
    uint8_t isSrc = 0;
    uint8_t vol   = 0;

    if (isMusic) {
        isSrc = btp_a2dp_isSrc(handle);
    } else {
#if (TLKBTP_CFG_HFP_ENABLE)
        if (btp_hfp_isHfpHF(handle)) {
            isSrc = 0;
        } else if (btp_hfp_isHfpAG(handle)) {
            isSrc = 1;
        }
#endif
    }

    int ret = tlkmdi_audio_btif_getVolumeStep(handle, isMusic, &vol, &isIos);
    if (ret != TLK_ENONE) {
        return ret;
    }
    if (isInc) {
        vol++;
        if (isIos) {
            if (vol > 16) {
                vol = 16;
                return -TLK_EINVALID;
            }
        } else {
            if (vol > 15) {
                vol = 15;
                return -TLK_EINVALID;
            }
        }
    } else {
        if (vol == 0) {
            return -TLK_EINVALID;
        }
        vol--;
    }
    if (isMusic) {
        bt_audio_set_music_vol_percent(isIos, vol);
    } else {
        bt_audio_set_voice_vol_percent(vol);
    }

    if (isMusic) {
        vol             = tlkmdi_audio_getBtpVolumeByStep(isIos, vol);
        uint8_t buffLen = 0;
        uint8_t buffer[6];
        buffer[buffLen++] = (handle & 0xFF);
        buffer[buffLen++] = (handle & 0xFF00) >> 8;
        buffer[buffLen++] = isSrc; // isSrc
        buffer[buffLen++] = vol;   // AVRCP Volume
        return tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_SET_AVRCP_VOLUME, buffer, buffLen);
    }


    uint8_t buffLen = 0;
    uint8_t buffer[6];
    buffer[buffLen++] = (handle & 0xFF);
    buffer[buffLen++] = (handle & 0xFF00) >> 8;
    buffer[buffLen++] = isSrc;
    buffer[buffLen++] = vol;
    return tlksys_sendMsg(TLKSYS_TASKID_HOST, TLKSYS_BT_MSGID_SET_HFP_VOLUME, buffer, buffLen);
}

#endif // #if (TLK_MW_AUDIO_ENABLE)

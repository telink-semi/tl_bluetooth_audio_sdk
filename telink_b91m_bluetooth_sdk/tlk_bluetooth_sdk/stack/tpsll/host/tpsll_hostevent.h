/********************************************************************************************************
 * @file    tpsll_hostevent.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef TPSLL_HOSTEVENT_H_
#define TPSLL_HOSTEVENT_H_

#include "stack/tpsll/controller/tph/tph_host_interface.h"
#include "stack/tpsll/controller/tpt/tpt_host_interface.h"
#include "stack/tpsll/controller/tpd/tpd_host_interface.h"
#include "stack/tpsll/controller/tpmd/tpmd_host_interface.h"


typedef int (*tpsll_event_func)(uint8_t *pData, uint16_t dataLen);

#define TPSLL_EVT_REGISTER(evtID, func)                \
    int evtID##_FUNC(uint8_t *pData, uint16_t dataLen) \
    {                                                  \
        return func(pData, dataLen);                   \
    }

typedef enum
{
    TPSLL_EVTID_NONE = 0,

    TPSLL_EVTID_DONGLE_CONNECT,
    TPSLL_EVTID_DONGLE_DISCONNECT,
    TPSLL_EVTID_DONGLE_MAC_UPDATE,
    TPSLL_EVTID_DONGLE_GET_AC_CHNID,
    TPSLL_EVTID_DONGLE_CONN_STATUS_CHANGE,
    TPSLL_EVTID_DONGLE_IDLE_STATUS_CHANGE,
    TPSLL_EVTID_DONGLE_SPEAKER_DATA_UPDATE,
    TPSLL_EVTID_DONGLE_READ_HEADSET_SINGLE,
    TPSLL_EVTID_DONGLE_READ_HEADSET_ROLE,
    TPSLL_EVTID_DONGLE_READ_HEADSET_BT_AUDIO,
    TPSLL_EVTID_DONGLE_READ_HEADSET_LEFT,
    TPSLL_EVTID_DONGLE_READ_HEADSET_FNO,
    TPSLL_EVTID_DONGLE_READ_HEADSET_MODE,
    TPSLL_EVTID_HEADSET_STIMER_START,
    TPSLL_EVTID_HEADSET_MODE_CHANGE,
    TPSLL_EVTID_HEADSET_ACL_MSG_DEAL,

    //for mesh_audio_dongle
    TPSLL_EVTID_HEADSET_CONNECT,
    TPSLL_EVTID_HEADSET_DISCONNECT,

#if (TLKSTK_BTTPSLL_TWS_ENABLE)
    TPSLL_EVTID_TWS_ACL_SETUP_COMPLETE,
    TPSLL_EVTID_TWS_SLAVE_PROFILE_INFO_SYNC,
    TPSLL_EVTID_TWS_SLAVE_SAMPLE_SYNC,
    TPSLL_EVTID_TWS_LMP_MSG_DEAL,
    TPSLL_EVTID_TWS_ACL_MSG_DEAL,
    TPSLL_EVTID_TWS_CONNECTED,
    TPSLL_EVTID_TWS_DISCONNECTED,
    TPSLL_EVTID_TWS_UI_TIMER_SYNC,
    TPSLL_EVTID_TWS_DONGLE_MAC_UPDATE,
    TPSLL_EVTID_TWS_DONGLE_DISCONNECTED,
    TPSLL_EVTID_TWS_MASTER_SLAVE_HANDOVER_SUCC,
    TPSLL_EVTID_TWS_SLAVE_HANDOVER_HOST_STATUS_SYNC,
    TPSLL_EVTID_TWS_HEADSET_ROLE_NOTIFY,
    TPSLL_EVTID_TWS_HEADSET_HANDOVER_INFO_EXTRACT,
    TPSLL_EVTID_TWS_CONNECT_SETUP_TIMEOUT,
    TPSLL_EVTID_TWS_SLAVE_SCO_SETUP_COMPLETE,
    TPSLL_EVTID_TWS_MASTER_SYNC_PROFILE,
#endif

    TPSLL_EVTID_MAX,
} TPSLL_EVTID_ENUM;

typedef struct
{
    uint8_t  mac_addr[6];
    uint8_t  chn;
    uint8_t  rsvd;
    uint32_t accessCode;
} tpsll_connect_info_Evt_t;

typedef struct
{
    uint32_t accessCode;
    uint32_t chnID;
} tpsll_getAcChnidEvt_t;

void tpsll_hostIrqEvt_regCB(uint16_t evtID, tpsll_event_func func);
int  tpsll_exe_event(uint16_t evtID, uint8_t *pData, uint16_t dataLen);


#if MCU_DUAL_CORE_ENABLE && defined(MCU_CORE_N22)
int tpsll_send_event(uint16_t evtID, uint8_t *pData, uint16_t dataLen);

int tpsll_send_dongleConnectEvt(void);

int tpsll_send_dongleDisonnEvt(uint8_t reason);

int tpsll_send_dongleMacUpdateEvt(uint8_t pAddr[6]);

int tpsll_send_getAcAndChnidCompleteEvt(uint8_t chnID, uint32_t accessCode);

int tpsll_send_headsetConnectEvt(uint8_t pAddr[6], uint32_t tpsll_ac, uint8_t tpsll_chn);

int tpsll_send_headsetDisonnEvt(uint8_t reason);

int tpsll_send_getHeadsetIsSingleCompleteEvt(uint8_t isSingle);

int tpsll_send_getHeadsetCurRoleCompleteEvt(uint8_t curRole);

int tpsll_send_getHeadsetCurBtAudioCompleteEvt(uint8_t curAudio);

int tpsll_send_getHeadsetIsLeftCompleteEvt(uint8_t isLeft);

int tpsll_send_getHeadsetFnoCompleteEvt(uint16_t frameNum);

int tpsll_send_getHeadsetCurModeCompleteEvt(uint8_t curMode);

int tpsll_send_dongleSpeakerDataEvt(uint8_t rx_packet_id, uint32_t mix_tick, uint8_t pdu_format, uint8_t *speaker_data, uint8_t speaker_data_len);

int tpsll_send_stimerStartEvt(uint32_t clockTick, uint8_t latency_mode, uint8_t latest_ptr, uint8_t sco_num_in_bt);

int tpsll_send_HandoverMaskSetEvt(uint32_t clockTick);

int tpsll_send_HandoverMaskClearEvt(uint32_t clockTick);

int tpsll_send_headsetModeChangeEvt(uint16_t mode);

int tpsll_send_get_audioSyncData(uint32_t task_tick, uint32_t stimer_tick, uint16_t fno, uint8_t sco_value, uint8_t *p_data);

int tpsll_send_slaveUpdateAudioSyncInfo(uint32_t m_tick, uint32_t s_tick, uint8_t *p_data);

int tpsll_send_twsSyncTick(uint32_t m_tick, uint32_t s_tick, uint8_t *p_data);

int tpsll_send_tws_aclSetupCompleteEvt(uint16_t handle, uint8_t btAddr[6], uint8_t linkKey[16]);

int tpsll_send_tws_slaveSyncProfileInfoEvt(uint8_t pType, uint16_t Info, void *param, uint8_t paramLen);

// int tpsll_send_tws_slaveSyncSampleEvt(tws_music_sync_cfg_t *m_sync_sample_cfg, tws_music_sync_cfg_t *s_sync_sample_cfg);

int tpsll_send_tws_lmpMsgDealEvt(uint8_t dst_id, uint8_t msg_id, uint8_t cmd, uint8_t *pData, uint8_t dataLen);

int tpsll_send_tws_aclMsgDealEvt(uint8_t dst_id, uint8_t msg_id, uint8_t cmd, uint8_t *pData, uint8_t dataLen);

int tpsll_send_tws_connectEvt(void);

int tpsll_send_tws_disconnectEvt(uint8_t reason);

int tpsll_send_uiSyncInfoEvt(uint8_t type, uint16_t magicWord, uint32_t clockTick);

int tpsll_send_tws_dongleMacUpdateEvt(uint8_t *pAddr);

int tpsll_send_tws_dongleDisconnEvt(uint8_t reason);

int tpsll_send_tws_masterSlaveHandoverSuccessEvt(void);

int tpsll_send_tws_handoverHostStatusSyncEvt();

int tpsll_send_tws_slaveScoSetupCompleteEvt(uint8_t airmode, uint8_t *pAddr);

int tpsll_send_tws_masterSyncProfileEvt(uint8_t *pAddr, uint16_t s_profile_info, uint16_t m_profile_info);

int tpsll_send_tws_headsetRoleNotifyEvt(uint8_t role);
int tpsll_send_tws_headsetHandoverInfoExtract();
int tpsll_send_tws_connectSetUpTimeoutEvt();
int tpsll_send_headset_recvAclMsgDealEvt(uint8_t dst_id, uint8_t msg_id, uint8_t cmd, uint8_t *pData, uint8_t dataLen);
#else
int tpsll_send_tws_slaveScoSetupCompleteEvt(uint8_t airmode, uint8_t *pAddr);
#endif

#endif // #define TPSLL_HOSTEVENT_H_

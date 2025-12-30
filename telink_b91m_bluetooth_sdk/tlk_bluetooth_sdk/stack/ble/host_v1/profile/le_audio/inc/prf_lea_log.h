/********************************************************************************************************
 * @file    prf_lea_log.h
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

extern uint8_t g_ble_lea_log_enable;
extern uint8_t g_ble_bass_log_enable;
extern uint8_t g_ble_pacs_log_enable;
extern uint8_t g_ble_ascs_log_enable;
extern uint8_t g_ble_aics_log_enable;
extern uint8_t g_ble_mics_log_enable;
extern uint8_t g_ble_vocs_log_enable;
extern uint8_t g_ble_vcs_log_enable;
extern uint8_t g_ble_gmcs_log_enable;
extern uint8_t g_ble_gtbs_log_enable;
extern uint8_t g_ble_tbs_log_enable;
extern uint8_t g_ble_tmas_log_enable;
extern uint8_t g_ble_has_log_enable;
extern uint8_t g_ble_csis_log_enable;
extern uint8_t g_ble_bap_sink_log_enable;
extern uint8_t g_ble_bap_uc_log_enable;
extern uint8_t g_ble_cap_uc_log_enable;

#define BLE_LEA_LOG(log, en, str, ...) \
    do {                                         \
        if (en && g_ble_lea_log_enable) { \
            log("[LEA]" str, ##__VA_ARGS__);   \
        }                                        \
    } while (0)

#define BLE_BASS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_bass_log_enable, "[BASS]"str, ##__VA_ARGS__)
#define BLE_BASS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_bass_log_enable, "[BASS]"str, ##__VA_ARGS__)
#define BLE_BASS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_bass_log_enable, "[BASS]"str, ##__VA_ARGS__)
#define BLE_BASS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_bass_log_enable, "[BASS]"str, ##__VA_ARGS__)

#define BLE_PACS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_pacs_log_enable, "[PACS]"str, ##__VA_ARGS__)
#define BLE_PACS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_pacs_log_enable, "[PACS]"str, ##__VA_ARGS__)
#define BLE_PACS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_pacs_log_enable, "[PACS]"str, ##__VA_ARGS__)
#define BLE_PACS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_pacs_log_enable, "[PACS]"str, ##__VA_ARGS__)

#define BLE_ASCS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_ascs_log_enable, "[ASCS]"str, ##__VA_ARGS__)
#define BLE_ASCS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_ascs_log_enable, "[ASCS]"str, ##__VA_ARGS__)
#define BLE_ASCS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_ascs_log_enable, "[ASCS]"str, ##__VA_ARGS__)
#define BLE_ASCS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_ascs_log_enable, "[ASCS]"str, ##__VA_ARGS__)

#define BLE_AICS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_aics_log_enable, "[AICS]"str, ##__VA_ARGS__)
#define BLE_AICS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_aics_log_enable, "[AICS]"str, ##__VA_ARGS__)
#define BLE_AICS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_aics_log_enable, "[AICS]"str, ##__VA_ARGS__)
#define BLE_AICS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_aics_log_enable, "[AICS]"str, ##__VA_ARGS__)

#define BLE_MICS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_mics_log_enable, "[MICS]"str, ##__VA_ARGS__)
#define BLE_MICS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_mics_log_enable, "[MICS]"str, ##__VA_ARGS__)
#define BLE_MICS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_mics_log_enable, "[MICS]"str, ##__VA_ARGS__)
#define BLE_MICS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_mics_log_enable, "[MICS]"str, ##__VA_ARGS__)

#define BLE_VOCS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_vocs_log_enable, "[VOCS]"str, ##__VA_ARGS__)
#define BLE_VOCS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_vocs_log_enable, "[VOCS]"str, ##__VA_ARGS__)
#define BLE_VOCS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_vocs_log_enable, "[VOCS]"str, ##__VA_ARGS__)
#define BLE_VOCS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_vocs_log_enable, "[VOCS]"str, ##__VA_ARGS__)

#define BLE_VCS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_vcs_log_enable, "[VCS]"str, ##__VA_ARGS__)
#define BLE_VCS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_vcs_log_enable, "[VCS]"str, ##__VA_ARGS__)
#define BLE_VCS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_vcs_log_enable, "[VCS]"str, ##__VA_ARGS__)
#define BLE_VCS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_vcs_log_enable, "[VCS]"str, ##__VA_ARGS__)

#define BLE_GMCS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_gmcs_log_enable, "[GMCS]"str, ##__VA_ARGS__)
#define BLE_GMCS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_gmcs_log_enable, "[GMCS]"str, ##__VA_ARGS__)
#define BLE_GMCS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_gmcs_log_enable, "[GMCS]"str, ##__VA_ARGS__)
#define BLE_GMCS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_gmcs_log_enable, "[GMCS]"str, ##__VA_ARGS__)

#define BLE_GTBS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_gtbs_log_enable, "[GTBS]"str, ##__VA_ARGS__)
#define BLE_GTBS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_gtbs_log_enable, "[GTBS]"str, ##__VA_ARGS__)
#define BLE_GTBS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_gtbs_log_enable, "[GTBS]"str, ##__VA_ARGS__)
#define BLE_GTBS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_gtbs_log_enable, "[GTBS]"str, ##__VA_ARGS__)

#define BLE_TBS_ERROR(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_tbs_log_enable, "[TBS]"str, ##__VA_ARGS__)
#define BLE_TBS_WARN(str, ...)      BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_tbs_log_enable, "[TBS]"str, ##__VA_ARGS__)
#define BLE_TBS_INFO(str, ...)      BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_tbs_log_enable, "[TBS]"str, ##__VA_ARGS__)
#define BLE_TBS_DEBUG(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_tbs_log_enable, "[TBS]"str, ##__VA_ARGS__)

#define BLE_TMAS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_tmas_log_enable, "[TMAS]"str, ##__VA_ARGS__)
#define BLE_TMAS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_tmas_log_enable, "[TMAS]"str, ##__VA_ARGS__)
#define BLE_TMAS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_tmas_log_enable, "[TMAS]"str, ##__VA_ARGS__)
#define BLE_TMAS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_tmas_log_enable, "[TMAS]"str, ##__VA_ARGS__)

#define BLE_HAS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_has_log_enable, "[HAS]"str, ##__VA_ARGS__)
#define BLE_HAS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_has_log_enable, "[HAS]"str, ##__VA_ARGS__)
#define BLE_HAS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_has_log_enable, "[HAS]"str, ##__VA_ARGS__)
#define BLE_HAS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_has_log_enable, "[HAS]"str, ##__VA_ARGS__)

#define BLE_CSIS_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_csis_log_enable, "[CSIS]"str, ##__VA_ARGS__)
#define BLE_CSIS_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_csis_log_enable, "[CSIS]"str, ##__VA_ARGS__)
#define BLE_CSIS_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_csis_log_enable, "[CSIS]"str, ##__VA_ARGS__)
#define BLE_CSIS_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_csis_log_enable, "[CSIS]"str, ##__VA_ARGS__)

#define BLE_BAP_SINK_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_bap_sink_log_enable, "[SINK]"str, ##__VA_ARGS__)
#define BLE_BAP_SINK_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_bap_sink_log_enable, "[SINK]"str, ##__VA_ARGS__)
#define BLE_BAP_SINK_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_bap_sink_log_enable, "[SINK]"str, ##__VA_ARGS__)
#define BLE_BAP_SINK_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_bap_sink_log_enable, "[SINK]"str, ##__VA_ARGS__)

#define BLE_BAP_UC_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_bap_uc_log_enable, "[BAP-UC]"str, ##__VA_ARGS__)
#define BLE_BAP_UC_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_bap_uc_log_enable, "[BAP-UC]"str, ##__VA_ARGS__)
#define BLE_BAP_UC_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_bap_uc_log_enable, "[BAP-UC]"str, ##__VA_ARGS__)
#define BLE_BAP_UC_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_bap_uc_log_enable, "[BAP-UC]"str, ##__VA_ARGS__)

#define BLE_CAP_UC_ERROR(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_ERROR, g_ble_bap_uc_log_enable, "[CAP-UC]"str, ##__VA_ARGS__)
#define BLE_CAP_UC_WARN(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_WARN, g_ble_bap_uc_log_enable, "[CAP-UC]"str, ##__VA_ARGS__)
#define BLE_CAP_UC_INFO(str, ...)     BLE_LEA_LOG(BLE_HOST_SAL_LOG_INFO, g_ble_bap_uc_log_enable, "[CAP-UC]"str, ##__VA_ARGS__)
#define BLE_CAP_UC_DEBUG(str, ...)    BLE_LEA_LOG(BLE_HOST_SAL_LOG_DEBUG, g_ble_bap_uc_log_enable, "[CAP-UC]"str, ##__VA_ARGS__)


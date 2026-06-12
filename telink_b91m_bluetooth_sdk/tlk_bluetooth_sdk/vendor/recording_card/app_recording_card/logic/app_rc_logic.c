/********************************************************************************************************
 * @file    app_rc_logic.c
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
#include <stdio.h>
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "../data_path/app_rc_stream.h"
#include "../app_recording_card_api.h"
#include "stack/ble/ble.h"
#include "../../ble/app_ble_telink_server.h"
#include "../../ble/app_ble.h"
#include "../data_path/wifi/app_rc_wifi_api.h"
#include "tlkmw/file/tlkmw_usb_msc.h"
#include "../ui/app_rc_ui.h"

#define APP_RC_LOGIC_WIFI_STREAM_CTRL_MAX_TIME_S 15

enum
{
    APP_RC_LOGIC_MODE_NONE            = 0,
    APP_RC_LOGIC_MODE_REPORT_OPUS_TAB = BIT(0),
    APP_RC_LOGIC_MODE_REPORT_WAV_TAB  = BIT(1),
    APP_RC_LOGIC_MODE_SEND_OPUS_FILE  = BIT(2),
    APP_RC_LOGIC_MODE_SEND_WIFI_FILE  = BIT(3),
};

enum
{
    APP_RC_LOGIC_RDY_NONE                   = 0,
    APP_RC_LOGIC_RDY_REPORT_NEXT_OPUS       = BIT(0),
    APP_RC_LOGIC_RDY_REPORT_NEXT_WAV        = BIT(1),
    APP_RC_LOGIC_RDY_SEND_NEXT_BLE_PACKAGE  = BIT(2),
    APP_RC_LOGIC_RDY_SEND_NEXT_WIFI_PACKAGE = BIT(3),
};

typedef struct
{
    uint8_t       isRecording   : 2;
    uint8_t       isWifiPowerOn : 2;
    uint8_t       isMscModeOpen : 4;
    uint8_t       isWavSave;
    uint8_t       modeFlag;
    uint8_t       rdyFlag;
    uint32_t      lastWifiSendTime;
    uint32_t      wifiSendTimeOut;
    TlkOsFsIter_t opsIter;
    TlkOsFsIter_t wavIter;
    TlkApiTimer_t wifiFlowCtrlTimer;
} app_rc_logic_ctrl_t;

static app_rc_logic_ctrl_t app_rc_logic = {0};

static void app_rc_stream_mic_pcm_out_data_rdy(void)
{
    uint8_t  buf[500];
    uint32_t readLen = 0;
    while (app_rc_stream_read(RC_STREAM_TYPE_MIC_PCM_OUT, buf, sizeof(buf), &readLen) == TLK_ENONE) {
        app_rc_stream_write(RC_STREAM_TYPE_FILE_WAV_IN, buf, readLen);
    }
}

static void app_rc_stream_mic_opus_out_data_rdy(void)
{
    uint8_t  buf[500];
    uint32_t readLen = 0;
    while (app_rc_stream_read(RC_STREAM_TYPE_MIC_OPUS_OUT, buf, sizeof(buf), &readLen) == TLK_ENONE) {
        app_rc_stream_write(RC_STREAM_TYPE_FILE_OPUS_IN, buf, readLen);
        app_rc_stream_write(RC_STREAM_TYPE_BLE_OPUS_IN, buf, readLen);
    }
}

static void app_rc_logic_set_work_mode(uint8_t flag)
{
    uint8_t lastMode = app_rc_logic.modeFlag;
    app_rc_logic.modeFlag |= flag;
    if (lastMode == APP_RC_LOGIC_MODE_NONE && flag) {
        tlksys_pm_setChn(APP_RC_PM_CHN, 1, 1); //working:not allowed suspend
    }
}

static void app_rc_logic_clr_work_mode(uint8_t flag)
{
    app_rc_logic.modeFlag &= ~flag;
    if (app_rc_logic.modeFlag == APP_RC_LOGIC_MODE_NONE) {
        tlksys_pm_setChn(APP_RC_PM_CHN, 0, 0); //allowed suspend
    }
}

static void app_rc_logic_set_rdy_flag(uint16_t flag)
{
    tlksys_enter_critical();
    app_rc_logic.rdyFlag |= flag;
    tlksys_leave_critical();
    tlksys_task_setEvt(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_LOGIC_WORK);
}

static void app_rc_logic_clr_rdy_flag(uint16_t flag)
{
    tlksys_enter_critical();
    app_rc_logic.rdyFlag &= ~flag;
    tlksys_leave_critical();
}

#if TLK_CFG_FS_ENABLE
static void app_rc_logic_report_opus_finish_callback(uint16_t conn_handle, bool success)
{
    (void)conn_handle;
    (void)success; //temp not care
    app_rc_logic_set_rdy_flag(APP_RC_LOGIC_RDY_REPORT_NEXT_OPUS);
}

static void app_rc_logic_report_wav_finish_callback(uint16_t conn_handle, bool success)
{
    (void)conn_handle;
    (void)success; //temp not care
    app_rc_logic_set_rdy_flag(APP_RC_LOGIC_RDY_REPORT_NEXT_WAV);
}

static void app_rc_logic_send_ble_package_finish_callback(uint16_t conn_handle, bool success)
{
    (void)conn_handle;
    (void)success; //temp not care
    app_rc_logic_set_rdy_flag(APP_RC_LOGIC_RDY_SEND_NEXT_BLE_PACKAGE);
}

static void app_rc_logic_wifi_data_stream_open_ok_callback(void)
{
    app_rc_logic_set_rdy_flag(APP_RC_LOGIC_RDY_SEND_NEXT_WIFI_PACKAGE);
}

static void app_rc_logic_wifi_send_finish_callback(uint8_t success, uint8_t *pAckData, uint16_t dataLen)
{
    (void)success;
    (void)pAckData;
    (void)dataLen;
    app_rc_logic.lastWifiSendTime = clock_time() | 1;
    app_rc_logic_set_rdy_flag(APP_RC_LOGIC_RDY_SEND_NEXT_WIFI_PACKAGE);
}
#endif


static void app_rc_logic_rdy_send_opus(void)
{
#if TLK_CFG_FS_ENABLE
    if (((app_rc_logic.rdyFlag) & APP_RC_LOGIC_RDY_REPORT_NEXT_OPUS) == 0) {
        return;
    }
    app_rc_logic_clr_rdy_flag(APP_RC_LOGIC_RDY_REPORT_NEXT_OPUS);
    char name[256 + 1] = {0};
    char name_info[256 + 1];
    if (tlkmw_fs_dir_fileIterNext(app_rc_logic.opsIter, name, sizeof(name)) == TLK_ENONE) {
        uint32_t size = 0;
        sprintf(name_info, "0:/%s", name);
        int ret = tlkmw_fs_getFileSize(name_info, &size);
        if (ret != TLK_ENONE) {
            size = 0;
        }
        int len = sprintf(name_info, "0:/%s %lu -B", name, size);
        app_tlk_record_report_file_table((uint8_t *)name_info, len, app_rc_logic_report_opus_finish_callback);
        tlk_printf("ops: %s", name_info);
    } else {
        tlkmw_fs_dir_fileIterDestroy(app_rc_logic.opsIter);
        app_tlk_record_report_file_table(NULL, 0, NULL);
        app_rc_logic.opsIter = NULL;
        app_rc_logic_clr_work_mode(APP_RC_LOGIC_MODE_REPORT_OPUS_TAB);
    }
#endif
}

static void app_rc_logic_rdy_send_wav(void)
{
#if TLK_CFG_FS_ENABLE
    if (((app_rc_logic.rdyFlag) & APP_RC_LOGIC_RDY_REPORT_NEXT_WAV) == 0) {
        return;
    }
    app_rc_logic_clr_rdy_flag(APP_RC_LOGIC_RDY_REPORT_NEXT_WAV);
    char name[256 + 1] = {0};
    char name_info[256 + 1];
    if (tlkmw_fs_dir_fileIterNext(app_rc_logic.wavIter, name, sizeof(name)) == TLK_ENONE) {
        uint32_t size = 0;
        sprintf(name_info, "0:/%s", name);
        int ret = tlkmw_fs_getFileSize(name_info, &size);
        if (ret != TLK_ENONE) {
            size = 0;
        }
        int len = sprintf(name_info, "0:/%s %lu -B", name, size);
        app_tlk_record_report_file_table((uint8_t *)name_info, len, app_rc_logic_report_wav_finish_callback);
        tlk_printf("wav: %s", name_info);
    } else {
        tlkmw_fs_dir_fileIterDestroy(app_rc_logic.wavIter);
        app_tlk_record_report_file_table(NULL, 0, NULL);
        app_rc_logic.wavIter = NULL;
        app_rc_logic_clr_work_mode(APP_RC_LOGIC_MODE_REPORT_WAV_TAB);
    }
#endif
}

static void app_rc_logic_rdy_send_ble_package(void)
{
#if TLK_CFG_FS_ENABLE
    if (((app_rc_logic.rdyFlag) & APP_RC_LOGIC_RDY_SEND_NEXT_BLE_PACKAGE) == 0) {
        return;
    }
    app_rc_logic_clr_rdy_flag(APP_RC_LOGIC_RDY_SEND_NEXT_BLE_PACKAGE);
    uint32_t readLen = 0;
    uint8_t  read_data[200];
    int      res = app_rc_stream_read(RC_STREAM_TYPE_FILE_OPUS_OUT, read_data, sizeof(read_data), &readLen);
    if (readLen == 0) {
        app_rc_stream_close(RC_STREAM_TYPE_FILE_OPUS_OUT);
        app_rc_logic_clr_work_mode(APP_RC_LOGIC_MODE_SEND_OPUS_FILE);
        tlk_printf("file all send finished");
        app_tlk_record_report_file_transport_finish();
        return;
    } else if (res == TLK_ENONE) {
        int ble_ret = app_tlk_record_report_file_data(read_data, readLen, app_rc_logic_send_ble_package_finish_callback);
        if (ble_ret != 0) { //ble fail?
            app_rc_stream_close(RC_STREAM_TYPE_FILE_OPUS_OUT);
            app_rc_logic_clr_work_mode(APP_RC_LOGIC_MODE_SEND_OPUS_FILE);
            tlk_printf("ble send fail,make it end");
        }
    }
#endif
}

static void app_rc_logic_stop_wifi_task(void)
{
    app_rc_stream_close(RC_STREAM_TYPE_FILE_OPUS_OUT);
    app_rc_stream_close(RC_STREAM_TYPE_WIFI_DATA_IN);
    app_rc_logic_clr_work_mode(APP_RC_LOGIC_MODE_SEND_WIFI_FILE);
    app_tlk_record_report_file_transport_finish();
    app_rc_logic.lastWifiSendTime = 0;
}

static void app_rc_logic_rdy_send_wifi_package(void)
{
#if TLK_CFG_FS_ENABLE && TLK_CFG_WIFI_LOGIC_ENABLE
    if (((app_rc_logic.rdyFlag) & APP_RC_LOGIC_RDY_SEND_NEXT_WIFI_PACKAGE) == 0) {
        return;
    }
    app_rc_logic_clr_rdy_flag(APP_RC_LOGIC_RDY_SEND_NEXT_WIFI_PACKAGE);
    if (!app_rc_wifi_is_connected_to_app()) {
        app_rc_logic_stop_wifi_task();
        return;
    }
    uint8_t  buffer[500];
    uint32_t readLen = 0;
    app_rc_stream_read(RC_STREAM_TYPE_FILE_OPUS_OUT, buffer, sizeof(buffer), &readLen);
    if (readLen == 0) {
        tlk_printf("wifi send file finished");
        app_rc_logic_stop_wifi_task();
        return;
    }
    //spi send intv at least 250us
    while (app_rc_logic.lastWifiSendTime != 0 && !clock_time_exceed(app_rc_logic.lastWifiSendTime, 250));

    int ret = app_rc_stream_write(RC_STREAM_TYPE_WIFI_DATA_IN, buffer, readLen);
    if (ret != TLK_ENONE) {
        app_rc_logic_stop_wifi_task();
    }
#endif
}

static void app_rc_logic_evt_task(void)
{
    app_rc_logic_rdy_send_opus();
    app_rc_logic_rdy_send_wav();
    app_rc_logic_rdy_send_ble_package();
    app_rc_logic_rdy_send_wifi_package();

    if (app_rc_logic.modeFlag) {
        app_rc_logic.wifiSendTimeOut = 0;
        tlksys_timer_reStart(APP_RC_THREAD_ID, &app_rc_logic.wifiFlowCtrlTimer);
    }
}

static void app_rc_logic_wifi_flow_ctrl_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    if ((app_rc_logic.modeFlag) & APP_RC_LOGIC_MODE_SEND_WIFI_FILE) {
        app_rc_logic.wifiSendTimeOut++;

        if (app_rc_logic.wifiSendTimeOut >= APP_RC_LOGIC_WIFI_STREAM_CTRL_MAX_TIME_S) {
            tlk_printf("wifi io ctrl too long,more than %ds", APP_RC_LOGIC_WIFI_STREAM_CTRL_MAX_TIME_S);
            app_rc_logic_stop_wifi_task();
        } else {
            tlksys_timer_reStart(APP_RC_THREAD_ID, &app_rc_logic.wifiFlowCtrlTimer);
        }
    }
}

void app_rc_logic_init(void)
{
    (void)app_rc_logic_clr_rdy_flag;

    tlksys_pm_regChn(APP_RC_PM_CHN);
    tlksys_pm_setChn(APP_RC_PM_CHN, 0, 0);
    tlksys_timer_createStatic(APP_RC_THREAD_ID, &app_rc_logic.wifiFlowCtrlTimer, 1 * 1000 * 1000, false, app_rc_logic_wifi_flow_ctrl_timer, NULL);
    tlksys_task_regEvtCB(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_LOGIC_WORK, app_rc_logic_evt_task);
}

void app_rc_msc_switch(void)
{
#if TLK_USB_MSC_ENABLE
    if (app_rc_logic.isMscModeOpen == 0) {
        if (app_rc_logic.modeFlag) {
            tlk_printf("fs busy,refuse open usb msc");
            return;
        }
        tlksys_runFuncInTask(TLKSYS_TASKID_SYSTEM, tlkmw_msc_open);
        app_rc_logic.isMscModeOpen = 1;
    } else {
        tlksys_runFuncInTask(TLKSYS_TASKID_SYSTEM, tlkmw_msc_close);
        app_rc_logic.isMscModeOpen = 0;
    }
#endif
}

void app_rc_start_record(void)
{
    if (app_rc_logic.isRecording) {
        return;
    }
    if (app_rc_logic.isMscModeOpen) {
        tlk_printf("now in msc mode,refuse");
        return;
    }
    app_rc_logic.isRecording = 1;

    app_rc_stream_open(RC_STREAM_TYPE_BLE_OPUS_IN);
    if (app_rc_logic.isWavSave) {
        app_rc_stream_open(RC_STREAM_TYPE_FILE_WAV_IN);
    }
    app_rc_stream_open(RC_STREAM_TYPE_FILE_OPUS_IN);
    app_rc_stream_openEx(RC_STREAM_TYPE_MIC_PCM_OUT, app_rc_stream_mic_pcm_out_data_rdy);
    app_rc_stream_openEx(RC_STREAM_TYPE_MIC_OPUS_OUT, app_rc_stream_mic_opus_out_data_rdy);
    app_rc_ui_update_rec_state(1);
}

void app_rc_stop_record(void)
{
    if (!app_rc_logic.isRecording) {
        return;
    }
    app_rc_logic.isRecording = 0;
    app_rc_stream_close(RC_STREAM_TYPE_MIC_PCM_OUT);
    app_rc_stream_close(RC_STREAM_TYPE_MIC_OPUS_OUT);
    app_rc_stream_close(RC_STREAM_TYPE_FILE_WAV_IN);
    app_rc_stream_close(RC_STREAM_TYPE_FILE_OPUS_IN);
    app_rc_stream_close(RC_STREAM_TYPE_BLE_OPUS_IN);
    app_rc_ui_update_rec_state(0);
}

void app_rc_enable_wav_save(uint8_t en)
{
    tlksys_enter_critical();
    app_rc_logic.isWavSave = en;
    tlksys_leave_critical();
}

uint8_t app_rc_is_recording(void)
{
    return app_rc_logic.isRecording;
}

uint8_t app_rc_is_busy(void)
{
    if (app_rc_logic.isMscModeOpen) {
        return 1;
    }
    if (app_rc_logic.isRecording || app_rc_logic.modeFlag || app_rc_logic.isWifiPowerOn || app_ble_is_connected()) {
        return 1;
    }
    return 0;
}

void app_rc_report_opus_table(void)
{
    if (app_rc_logic.modeFlag & APP_RC_LOGIC_MODE_REPORT_OPUS_TAB) {
        return;
    }
    if (app_rc_logic.isMscModeOpen) {
        tlk_printf("now in msc mode,refuse");
        return;
    }
#if TLK_CFG_FS_ENABLE
    app_rc_logic.opsIter = tlkmw_fs_dir_fileIterCreate("0:/", "ops");
#else
    app_rc_logic.opsIter = NULL;
#endif
    if (app_rc_logic.opsIter == NULL) {
        app_tlk_record_report_file_table(NULL, 0, NULL);
        return;
    }
    app_rc_logic_set_work_mode(APP_RC_LOGIC_MODE_REPORT_OPUS_TAB);
    app_rc_logic_set_rdy_flag(APP_RC_LOGIC_RDY_REPORT_NEXT_OPUS);
}

void app_rc_report_wav_table(void)
{
    if (app_rc_logic.modeFlag & APP_RC_LOGIC_MODE_REPORT_WAV_TAB) {
        return;
    }
    if (app_rc_logic.isMscModeOpen) {
        tlk_printf("now in msc mode,refuse");
        return;
    }
#if TLK_CFG_FS_ENABLE
    app_rc_logic.wavIter = tlkmw_fs_dir_fileIterCreate("0:/", "wav");
#else
    app_rc_logic.wavIter = NULL;
#endif
    if (app_rc_logic.wavIter == NULL) {
        app_tlk_record_report_file_table(NULL, 0, NULL);
        return;
    }
    app_rc_logic_set_work_mode(APP_RC_LOGIC_MODE_REPORT_WAV_TAB);
    app_rc_logic_set_rdy_flag(APP_RC_LOGIC_RDY_REPORT_NEXT_WAV);
}

void app_rc_send_ops_file(char *path, uint32_t offset)
{
    (void)path;
    (void)offset;
#if TLK_CFG_FS_ENABLE
    if (app_rc_logic.modeFlag & APP_RC_LOGIC_MODE_SEND_OPUS_FILE) {
        return;
    }
    if (app_rc_logic.isMscModeOpen) {
        tlk_printf("now in msc mode,refuse");
        return;
    }
    uint32_t size = 0;
    int      ret  = tlkmw_fs_getFileSize((path), &size);

    if (ret != TLK_ENONE) {
        app_tlk_record_report_file_size(0);
        tlk_printf("get file size fail");
        return;
    }
    app_rc_stream_cfg_t cfg = {
        .arg0 = path,
        .arg1 = (void *)1,
        .arg2 = (void *)offset,
    };
    ret = app_rc_stream_openEx(RC_STREAM_TYPE_FILE_OPUS_OUT, &cfg);
    if (ret != TLK_ENONE) {
        app_tlk_record_report_file_size(0);
        tlk_printf("start send ops file fail");
        return;
    }

    app_tlk_record_report_file_size(size);
    app_rc_logic_set_work_mode(APP_RC_LOGIC_MODE_SEND_OPUS_FILE);
    app_rc_logic_set_rdy_flag(APP_RC_LOGIC_RDY_SEND_NEXT_BLE_PACKAGE);
#endif
}

void app_rc_send_file_by_Wifi(char *path, uint32_t offset)
{
    (void)path;
    (void)offset;
#if TLK_CFG_FS_ENABLE
    (void)app_rc_logic_wifi_data_stream_open_ok_callback;
    (void)app_rc_logic_wifi_send_finish_callback;
#endif
#if TLK_CFG_FS_ENABLE && TLK_CFG_WIFI_LOGIC_ENABLE
    if (app_rc_logic.modeFlag & APP_RC_LOGIC_MODE_SEND_WIFI_FILE) {
        return;
    }
    if (!app_rc_wifi_is_connected_to_app()) {
        return;
    }
    if (app_rc_logic.isMscModeOpen) {
        tlk_printf("now in msc mode,refuse");
        return;
    }
    if (app_rc_wifi_isInOta()) {
        tlk_printf("wifi is in ota logic,refuse file trans");
        return;
    }
    uint32_t size = 0;
    int      ret  = tlkmw_fs_getFileSize(path, &size);
    if (ret != TLK_ENONE) {
        app_tlk_record_report_file_size(0);
        tlk_printf("WIFI get file size fail");
        return;
    }
    app_rc_stream_cfg_t cfg = {
        .arg0 = path,
        .arg1 = (void *)0,
        .arg2 = (void *)offset,
    };
    ret = app_rc_stream_openEx(RC_STREAM_TYPE_FILE_OPUS_OUT, &cfg);
    if (ret != TLK_ENONE) {
        app_tlk_record_report_file_size(0);
        tlk_printf("WIFI OPEN file fail");
        app_rc_stream_close(RC_STREAM_TYPE_FILE_OPUS_OUT);
        return;
    }
    app_rc_stream_cfg_t wifi_cfg = {
        .arg0 = app_rc_logic_wifi_data_stream_open_ok_callback,
        .arg1 = app_rc_logic_wifi_send_finish_callback,
        .arg2 = NULL,
    };
    ret = app_rc_stream_openEx(RC_STREAM_TYPE_WIFI_DATA_IN, &wifi_cfg);
    if (ret != TLK_ENONE) {
        app_tlk_record_report_file_size(0);
        tlk_printf("wifi not connect,send fail");
        app_rc_stream_close(RC_STREAM_TYPE_WIFI_DATA_IN);
        return;
    }
    app_tlk_record_report_file_size(size);
    app_rc_logic_set_work_mode(APP_RC_LOGIC_MODE_SEND_WIFI_FILE);

#endif
}

void app_rc_rename_file(const char *oldPath, const char *newPath)
{
    (void)oldPath;
    (void)newPath;
#if TLK_CFG_FS_ENABLE
    if (app_rc_logic.isMscModeOpen) {
        tlk_printf("now in msc mode,refuse");
        app_tlk_record_report_rename_file_result(1);
        return;
    }
    int     ret = tlkmw_fs_renameFile(oldPath, newPath);
    uint8_t res = ret == TLK_ENONE ? 0 : 1;
    app_tlk_record_report_rename_file_result(res);
#endif
}

void app_rc_delete_file(const char *file)
{
    (void)file;
#if TLK_CFG_FS_ENABLE
    if (app_rc_logic.isMscModeOpen) {
        tlk_printf("now in msc mode,refuse");
        app_tlk_record_report_delete_file_result(1);
        return;
    }
    int     ret = tlkmw_fs_deleteFile(file);
    uint8_t res = ret == TLK_ENONE ? 0 : 1;
    app_tlk_record_report_delete_file_result(res);
#endif
}

void app_rc_wifi_power_on(uint16_t delayMs)
{
    (void)delayMs;
#if TLK_CFG_WIFI_LOGIC_ENABLE
    if (app_rc_logic.isWifiPowerOn) {
        return;
    }
    app_rc_logic.isWifiPowerOn = 1;
    //use timer later
    if (delayMs != 0) {
        tlkos_task_delayMs(delayMs);
    }
    app_rc_wifi_core_power_on();
#endif
}

void app_rc_wifi_power_off(void)
{
#if TLK_CFG_WIFI_LOGIC_ENABLE
    if (!app_rc_logic.isWifiPowerOn) {
        return;
    }
    app_rc_logic.isWifiPowerOn = 0;
    app_rc_wifi_core_power_off();
#endif
}

void app_rc_wifi_power_switch(void)
{
#if TLK_CFG_WIFI_LOGIC_ENABLE
    if (app_rc_logic.isWifiPowerOn) {
        app_rc_wifi_core_power_off();
    } else {
        app_rc_wifi_core_power_on();
    }
    app_rc_logic.isWifiPowerOn = !app_rc_logic.isWifiPowerOn;
#endif
}

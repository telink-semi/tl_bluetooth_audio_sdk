/********************************************************************************************************
 * @file    app_rc_wifi_ota.c
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
#include "tlkapi/tlkapi.h"
#include "tlkmw/tlkmw.h"
#include "../../app_recording_card_api.h"
#include "app_rc_wifi_api.h"
#include "tlkmw/userCtrl/ota/general_protocol/tlk_ota_general_protocol_example.h"

typedef enum
{
    WIFI_OTA_STATE_IDLE = 0,
    WIFI_OTA_STATE_WAIT_POWER_ON,
    WIFI_OTA_STATE_STARTING,
    WIFI_OTA_STATE_READ,
    WIFI_OTA_STATE_SENDING,
    WIFI_OTA_STATE_STOPING,
    WIFI_OTA_STATE_WAIT_CHECK,
    WIFI_OTA_STATE_END,
    WIFI_OTA_STATE_WIFI_REBOOT,
} WIFI_OTA_STATE_MACHINE_ENUM;

#define WIFI_MAX_OTA_DATA_LEN 500

typedef struct
{
    uint32_t dfu_bin_addr;
    uint32_t dfu_bin_size;
    uint32_t crc;
    uint32_t dfu_bin_offset;
} dfuOutImgCtrl_t;

typedef struct
{
    uint8_t         state;
    uint8_t         resv;
    uint16_t        timerCnt;
    dfuOutImgCtrl_t img;
    TlkApiTimer_t   timer;
} AppRcWifiOta_t;

AppRcWifiOta_t sAppRcWifiOta = {0};

static void app_rc_wifi_ota_fail_clean(void)
{
    tlk_printf("wifi ota fail");
    sAppRcWifiOta.state    = WIFI_OTA_STATE_IDLE;
    sAppRcWifiOta.timerCnt = 0;
    tlksys_timer_stop(APP_RC_THREAD_ID, &sAppRcWifiOta.timer);
}

static void app_rc_wifi_ota_send_finish_cb(uint8_t isSuccess, uint8_t *pAckData, uint16_t dataLen)
{
    (void)pAckData;
    (void)dataLen;
    (void)isSuccess;
    if (sAppRcWifiOta.state != WIFI_OTA_STATE_SENDING) {
        app_rc_wifi_ota_fail_clean();
        return;
    }
    sAppRcWifiOta.state = WIFI_OTA_STATE_READ;
    tlksys_task_setEvt(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_WIFI_OTA);
}

static void app_rc_wifi_startOtaCmdRspCb(uint8_t isSuccess, uint8_t *pAckData, uint16_t dataLen)
{
    if (sAppRcWifiOta.state != WIFI_OTA_STATE_STARTING) {
        app_rc_wifi_ota_fail_clean();
        return;
    }
    if (!isSuccess || dataLen != 1 || pAckData[0] != 0x01) {
        app_rc_wifi_ota_fail_clean();
        return;
    }
    sAppRcWifiOta.state = WIFI_OTA_STATE_READ;
    tlksys_task_setEvt(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_WIFI_OTA);
}

static void app_rc_wifi_stopOtaCmdRspCb(uint8_t isSuccess, uint8_t *pAckData, uint16_t dataLen)
{
    if (sAppRcWifiOta.state != WIFI_OTA_STATE_STOPING) {
        app_rc_wifi_ota_fail_clean();
        return;
    }
    if (!isSuccess || dataLen != 1 || pAckData[0] != 0x01) {
        app_rc_wifi_ota_fail_clean();
        return;
    }
    sAppRcWifiOta.state = WIFI_OTA_STATE_WAIT_CHECK;
    tlksys_task_setEvt(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_WIFI_OTA);
}

static void app_rc_wifi_checkOtaStatusCmdRspCb(uint8_t isSuccess, uint8_t *pAckData, uint16_t dataLen)
{
    if (sAppRcWifiOta.state != WIFI_OTA_STATE_END) {
        app_rc_wifi_ota_fail_clean();
        return;
    }
    if (!isSuccess || dataLen != 1 || pAckData[0] != 0x01) {
        app_rc_wifi_ota_fail_clean();
        return;
    }
    sAppRcWifiOta.state = WIFI_OTA_STATE_WIFI_REBOOT;
    app_rc_api_wifi_power_off();
    if (tlkos_task_delayMs(200) != TLK_ENONE) {
        delay_ms(200);
    }
    app_rc_api_wifi_power_on(0);
    sAppRcWifiOta.timerCnt = 0;
    tlksys_timer_reStart(APP_RC_THREAD_ID, &sAppRcWifiOta.timer);
}

static void app_rc_wifi_ota_read_process(void)
{
    tlk_printf("app_rc_wifi_ota_read_process %d %d", sAppRcWifiOta.img.dfu_bin_offset, sAppRcWifiOta.img.dfu_bin_size);
    if (sAppRcWifiOta.img.dfu_bin_offset >= sAppRcWifiOta.img.dfu_bin_size) {
        sAppRcWifiOta.state = WIFI_OTA_STATE_STOPING;
        tlkos_task_delayMs(300); //temp code
        app_rc_wifi_spi_send_cmd(WIFI_SPI_CMD_STOP_DFU, NULL, 0, app_rc_wifi_stopOtaCmdRspCb);
        memset(&sAppRcWifiOta.img, 0, sizeof(sAppRcWifiOta.img));
        app_rc_wifi_spi_cleanFrameNum();
        tlksys_task_setEvt(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_WIFI_OTA);
        return;
    }

    uint32_t bin_addr     = sAppRcWifiOta.img.dfu_bin_addr + sAppRcWifiOta.img.dfu_bin_offset;
    uint32_t remainingLen = sAppRcWifiOta.img.dfu_bin_size - sAppRcWifiOta.img.dfu_bin_offset;
    uint16_t dataLen      = (remainingLen > WIFI_MAX_OTA_DATA_LEN) ? WIFI_MAX_OTA_DATA_LEN : (uint16_t)remainingLen;
    uint8_t  data[WIFI_MAX_OTA_DATA_LEN];
    flash_dread((bin_addr), dataLen, data);

    sAppRcWifiOta.state = WIFI_OTA_STATE_SENDING;

    sAppRcWifiOta.img.dfu_bin_offset += dataLen;
    tlkos_task_delayMs(5);
    app_rc_wifi_spi_send_data(data, dataLen, app_rc_wifi_ota_send_finish_cb);
}

static void app_rc_wifi_ota_process(void)
{
    if (sAppRcWifiOta.state == WIFI_OTA_STATE_READ) {
        app_rc_wifi_ota_read_process();
    } else if (sAppRcWifiOta.state == WIFI_OTA_STATE_WAIT_CHECK) {
        sAppRcWifiOta.state = WIFI_OTA_STATE_END;
        app_rc_wifi_spi_send_cmd(WIFI_SPI_CMD_GET_DFU_STATUS, NULL, 0, app_rc_wifi_checkOtaStatusCmdRspCb);
    }
}

static void app_rc_wifi_ota_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    sAppRcWifiOta.timerCnt++;
    if (sAppRcWifiOta.state == WIFI_OTA_STATE_WAIT_POWER_ON) {
        if (sAppRcWifiOta.timerCnt < 10) {
            return;
        }
        uint8_t otaInfo[8];
        uint8_t bufferLen = 0;
        UINT32L_TO_ARRAY(sAppRcWifiOta.img.dfu_bin_size, otaInfo, bufferLen);
        bufferLen += 4;
        UINT32L_TO_ARRAY(sAppRcWifiOta.img.crc, otaInfo, bufferLen);
        bufferLen += 4;
        sAppRcWifiOta.state = WIFI_OTA_STATE_STARTING;
        app_rc_wifi_spi_send_cmd(WIFI_SPI_CMD_START_DFU, otaInfo, bufferLen, app_rc_wifi_startOtaCmdRspCb);
    } else if (sAppRcWifiOta.state == WIFI_OTA_STATE_WIFI_REBOOT) {
        if (sAppRcWifiOta.timerCnt < 40) {
            return;
        }
        uint8_t pBuffer[4] = {0};
        uint8_t buffLen    = 0;
        pBuffer[buffLen++] = TLK_FIRMWARE_OTA_SUCCESS;
        pBuffer[buffLen++] = 0;
        tlkmw_ota_update_ota_status(pBuffer, buffLen, NULL);
        sAppRcWifiOta.state    = WIFI_OTA_STATE_IDLE;
        sAppRcWifiOta.timerCnt = 0;
        tlksys_timer_stop(APP_RC_THREAD_ID, &sAppRcWifiOta.timer);
        app_rc_api_wifi_power_off();
    } else {
        sAppRcWifiOta.timerCnt = 0;
        tlksys_timer_stop(APP_RC_THREAD_ID, &sAppRcWifiOta.timer);
    }
}

void app_rc_wifi_startOtaDataCopy(void)
{
    if (sAppRcWifiOta.state != WIFI_OTA_STATE_IDLE) {
        return;
    }
    sAppRcWifiOta.state = WIFI_OTA_STATE_WAIT_POWER_ON;
    tlksys_task_regEvtCB(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_WIFI_OTA, app_rc_wifi_ota_process);
    app_rc_api_wifi_power_on(0);
    tlksys_timer_createStatic(APP_RC_THREAD_ID, &sAppRcWifiOta.timer, 1 * 1000 * 1000, true, app_rc_wifi_ota_timer, NULL);
    tlksys_timer_reStart(APP_RC_THREAD_ID, &sAppRcWifiOta.timer);
}

bool app_rc_wifi_isInOta(void)
{
#if TLK_CFG_WIFI_LOGIC_ENABLE
    return sAppRcWifiOta.state != WIFI_OTA_STATE_IDLE;
#else
    return false;
#endif
}

//all temp code

void tlkapp_set_wifi_startDfu(void)
{
#if TLK_CFG_WIFI_LOGIC_ENABLE
    app_rc_wifi_startOtaDataCopy();
#endif
}


#if TLK_CFG_WIFI_LOGIC_ENABLE && TLK_MW_OTA_ENABLE
int tlkmw_ota_start_customer_init(void)
{
    if (app_rc_wifi_isInOta()) {
        return -1;
    }
    return OTA_NONE;
}

/**
 * @brief      Handle special operations after OTA end acknowledgment
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - TLK_ENONE if success
 */
int tlkmw_ota_end_ack_deal_SpecialOperationsr(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;
    sTlk_total_fw_descriptors_t        *sTlk_total_fw_descriptors_s = NULL;
    extern sTlk_total_fw_descriptors_t *tlkmw_get_recvImgHeaderAddr(void);
    sTlk_total_fw_descriptors_s  = tlkmw_get_recvImgHeaderAddr();
    sTlkMwOta_t *sTlkMwOtaCtrl_t = NULL;
    sTlkMwOtaCtrl_t              = tlkmw_get_otaCtrl();
    if (sTlk_total_fw_descriptors_s->fw_descpts_list == NULL) {
        return TLK_EHANDLE;
    }
    if (sTlk_total_fw_descriptors_s->fw_number == 0) {
        return TLK_ENONE;
    }
    for (uint8_t index = 0; index < sTlk_total_fw_descriptors_s->fw_number; index++) {
        sTlk_fw_descriptors_node_t *fw_info = tlkmw_get_fw_descpts_node_by_index(sTlk_total_fw_descriptors_s->fw_descpts_list, index);
        if (fw_info == NULL) {
            return false;
        }
        if (fw_info->fw_type == BINX_CUSTOMER) //TODO
        {
            sAppRcWifiOta.img.dfu_bin_addr   = fw_info->start_addr + sTlkMwOtaCtrl_t->backAddr;
            sAppRcWifiOta.img.crc            = 0;
            sAppRcWifiOta.img.dfu_bin_size   = fw_info->fw_size - 32;
            sAppRcWifiOta.img.dfu_bin_offset = 0;

            uint8_t pBuffer[4] = {0};
            uint8_t buffLen    = 0;
            pBuffer[buffLen++] = TLK_FIRMWARE_OTA_SUCCESS;
            pBuffer[buffLen++] = 0;
            tlkmw_ota_update_ota_status(pBuffer, buffLen, NULL);

            uint32_t            img_crc = 0xFFFFFFFF;
            extern unsigned int calculate_CRC32(unsigned int crc, unsigned char *pStart, unsigned int uSize);
            sAppRcWifiOta.img.crc = calculate_CRC32(img_crc, (uint8_t *)((sAppRcWifiOta.img.dfu_bin_addr) + FLASH_R_BASE_ADDR), sAppRcWifiOta.img.dfu_bin_size);
            tlkapp_set_wifi_startDfu();
            return 1;
        }
    }
    return TLK_ENONE;
}
#endif
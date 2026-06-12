/********************************************************************************************************
 * @file    app_rc_wifi_uart.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapi/tlkapi.h"
#include "../app_rc_wifi_cfg.h"
#include "vendor/recording_card/app_recording_card/app_recording_card_api.h"

typedef enum
{
    APP_RC_WIFI_STATE_CLOSE = 0,
    APP_RC_WIFI_STATE_CONNECTING,
    APP_RC_WIFI_STATE_CONNECTED,
    APP_RC_WIFI_STATE_DISCONNECTED,
} app_rc_uart_wifi_state_e;

typedef struct _AppRcWifiUartSendItem_t
{
    struct _AppRcWifiUartSendItem_t *next;
    uint32_t                         dataLen;
    uint8_t                          data[0];
} AppRcWifiUartSendItem_t;

typedef struct
{
    uint8_t                  isOpen;
    uint8_t                  connectState;
    uint16_t                 timeCnt;
    AppRcWifiUartSendItem_t *sendListHead;
    AppRcWifiUartSendItem_t *sendListTail;
    TlkApiTimer_t            sendTimer;
    TlkApiTimer_t            checkTimer;
} AppRcWifiUart_t;

AppRcWifiUart_t sAppRcWifiUart = {0};

__attribute__((aligned(4))) static uint8_t sAppRcWifiUartRxBuffer[APP_WIFI_UART_RX_BUFFER_NUM * (APP_WIFI_UART_RX_BUFFER_SIZE + 4)];

__attribute__((weak)) void app_rc_wifi_uart_getDataDeal(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
}

static inline void app_rc_wifi_uart_set_close(void)
{
    sAppRcWifiUart.connectState = APP_RC_WIFI_STATE_CLOSE;
    sAppRcWifiUart.timeCnt      = 0;
#if TLK_CFG_RF_SHARED
    app_rc_api_wifi_power_off();
#endif
}

static void app_rc_wifi_uart_getData(uint8_t *pFrame, uint16_t frmLen)
{
    tlkapi_array(0xFFFFFFFF, "[WIFI]", "app_rc_wifi_uart_getData:", pFrame, max(frmLen, 8));
#if TLK_CFG_RF_SHARED
    if (APP_RC_WIFI_STATE_CLOSE == sAppRcWifiUart.connectState) {
        return;
    }
#endif
    if (frmLen < 14 || pFrame[0] != 0xAA || pFrame[1] != 0x55) {
        return;
    }
    uint16_t dataLen = pFrame[2] + pFrame[3] * 256;
    if (dataLen < 14) {
        return;
    }
    uint16_t payLoadLen = dataLen - 14;
    if (dataLen != frmLen) {
        return;
    }
    uint16_t checkSum = 0;
    for (uint16_t i = 0; i < dataLen - 2; i++) {
        checkSum += pFrame[i];
    }
    uint16_t validCheckSum = pFrame[dataLen - 1] * 256 + pFrame[dataLen - 2];
    if (validCheckSum != checkSum) {
        return;
    }
    tlkapi_trace(0xFFFFFFFF, "[WIFI]", "app_rc_wifi_uart_getData ok");
    // uint8_t isNeedFastIntv = pFrame[4];
    uint32_t devState = pFrame[8];
    if (devState) {
        sAppRcWifiUart.connectState = APP_RC_WIFI_STATE_CONNECTED;
    } else {
        if (sAppRcWifiUart.connectState == APP_RC_WIFI_STATE_CONNECTED) {
            sAppRcWifiUart.connectState = APP_RC_WIFI_STATE_DISCONNECTED;
        }
    }
    if (payLoadLen == 0) {
        return;
    }
    app_rc_wifi_uart_getDataDeal(pFrame + 12, payLoadLen);
}

int app_rc_wifi_uart_sendData(uint8_t *pData, uint16_t dataLen)
{
    tlkapi_array(0xffffffff, "[WIFI]", "app_rc_wifi_uart_sendData", pData, dataLen);
    if (dataLen > 256) {
        return -TLK_EPARAM;
    }
    if (!sAppRcWifiUart.isOpen) {
        return -TLK_EFAIL;
    }
    AppRcWifiUartSendItem_t *item = (AppRcWifiUartSendItem_t *)tlkos_malloc(sizeof(AppRcWifiUartSendItem_t) + dataLen);
    if (item == NULL) {
        return -TLK_ENOMEM;
    }
    item->dataLen = dataLen;
    memcpy(item->data, pData, dataLen);
    item->next = NULL;
    tlksys_enter_critical();
    if (sAppRcWifiUart.sendListTail == NULL) {
        sAppRcWifiUart.sendListHead = item;
        sAppRcWifiUart.sendListTail = item;
    } else {
        sAppRcWifiUart.sendListTail->next = item;
        sAppRcWifiUart.sendListTail       = item;
    }
    tlksys_leave_critical();
    tlksys_task_setEvt(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_WIFI_UART);
    return TLK_ENONE;
}

static void app_rc_wifi_uart_send_timer(TlkApiTimerHandle_t handle, void *userArg)
{
    (void)handle;
    (void)userArg;
    uint8_t  buffer[300];
    uint16_t bufferLen  = 0;
    buffer[bufferLen++] = 0xaa;
    buffer[bufferLen++] = 0x55;
    memset(buffer + bufferLen, 0, 10);
    bufferLen += 10;
    tlksys_enter_critical();
    if (sAppRcWifiUart.sendListHead != NULL) {
        AppRcWifiUartSendItem_t *item = sAppRcWifiUart.sendListHead;
        sAppRcWifiUart.sendListHead   = item->next;
        if (sAppRcWifiUart.sendListHead == NULL) {
            sAppRcWifiUart.sendListTail = NULL;
        }
        tlksys_leave_critical();
        memcpy(buffer + bufferLen, item->data, item->dataLen);
        bufferLen += item->dataLen;
        tlkos_free(item);
    } else {
        tlksys_leave_critical();
    }
    buffer[2]          = (bufferLen + 2);
    buffer[3]          = (bufferLen + 2) >> 8;
    uint16_t check_sum = 0;
    for (uint16_t i = 0; i < bufferLen; i++) {
        check_sum += buffer[i];
    }
    buffer[bufferLen++] = check_sum;
    buffer[bufferLen++] = check_sum >> 8;
    tlkdrv_serial_send(APP_WIFI_UART_PORT, buffer, bufferLen);
}

static void app_rc_wifi_uart_check_timer(TlkApiTimerHandle_t timer, void *userArg)
{
    (void)timer;
    (void)userArg;
    if (sAppRcWifiUart.connectState == APP_RC_WIFI_STATE_CLOSE || sAppRcWifiUart.connectState == APP_RC_WIFI_STATE_CONNECTED) {
        return;
    }
    sAppRcWifiUart.timeCnt++;
    if (sAppRcWifiUart.connectState == APP_RC_WIFI_STATE_CONNECTING) {
        if (sAppRcWifiUart.timeCnt > 80) {
            app_rc_wifi_uart_set_close();
        }
    } else if (sAppRcWifiUart.connectState == APP_RC_WIFI_STATE_DISCONNECTED) {
        if (sAppRcWifiUart.timeCnt > 2) {
            app_rc_wifi_uart_set_close();
        }
    }
}

static void app_rc_wifi_uart_send_handler(void) {}

void app_rc_wifi_uart_init(void)
{
    tlkdrv_serial_mount(APP_WIFI_UART_PORT, APP_WIFI_UART_BAUDRATE, APP_WIFI_UART_TX_PIN, APP_WIFI_UART_RX_PIN, 0, tlkhal_dma_malloc_ex(1));
    tlkdrv_serial_setRxQFifo(APP_WIFI_UART_PORT, APP_WIFI_UART_RX_BUFFER_NUM, APP_WIFI_UART_RX_BUFFER_SIZE + 4, sAppRcWifiUartRxBuffer,
                             APP_WIFI_UART_RX_BUFFER_NUM * (APP_WIFI_UART_RX_BUFFER_SIZE + 4));

    tlkdrv_serial_regCB(APP_WIFI_UART_PORT, app_rc_wifi_uart_getData);
    tlksys_task_regEvtCB(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_WIFI_UART, app_rc_wifi_uart_send_handler);
    tlksys_timer_createStatic(APP_RC_THREAD_ID, &sAppRcWifiUart.sendTimer, 1 * 1000 * 1000, true, app_rc_wifi_uart_send_timer, NULL);
    tlksys_timer_createStatic(APP_RC_THREAD_ID, &sAppRcWifiUart.checkTimer, 1 * 1000 * 1000, true, app_rc_wifi_uart_check_timer, NULL);
}

void app_rc_wifi_uart_open(void)
{
    if (sAppRcWifiUart.isOpen) {
        return;
    }
    tlkdrv_serial_open(APP_WIFI_UART_PORT);
    sAppRcWifiUart.isOpen       = 1;
    sAppRcWifiUart.timeCnt      = 0;
    sAppRcWifiUart.connectState = APP_RC_WIFI_STATE_CONNECTING;
    tlksys_timer_start(APP_RC_THREAD_ID, &sAppRcWifiUart.checkTimer);
    tlksys_timer_setPeriod(APP_RC_THREAD_ID, &sAppRcWifiUart.sendTimer, 1 * 1000 * 1000);
    tlksys_timer_reStart(APP_RC_THREAD_ID, &sAppRcWifiUart.sendTimer);
}

void app_rc_wifi_uart_close(void)
{
    if (!sAppRcWifiUart.isOpen) {
        return;
    }
    sAppRcWifiUart.connectState = APP_RC_WIFI_STATE_CLOSE;
    tlksys_timer_stop(APP_RC_THREAD_ID, &sAppRcWifiUart.checkTimer);
    sAppRcWifiUart.isOpen = 0;
    tlkdrv_serial_close(APP_WIFI_UART_PORT);
    gpio_shutdown(APP_WIFI_UART_TX_PIN);
    gpio_shutdown(APP_WIFI_UART_RX_PIN);
    tlksys_timer_stop(APP_RC_THREAD_ID, &sAppRcWifiUart.sendTimer);
    tlksys_enter_critical();
    while (sAppRcWifiUart.sendListHead != NULL) {
        AppRcWifiUartSendItem_t *item = sAppRcWifiUart.sendListHead;
        sAppRcWifiUart.sendListHead   = item->next;
        tlkos_free(item);
    }
    sAppRcWifiUart.sendListTail = NULL;
    tlksys_leave_critical();
}

bool app_rc_wifi_is_connected_to_app(void)
{
    return sAppRcWifiUart.connectState == APP_RC_WIFI_STATE_CONNECTED;
}

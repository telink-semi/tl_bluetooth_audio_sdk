/********************************************************************************************************
 * @file    app_rc_wifi_spi.c
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
#include "../app_rc_wifi_cfg.h"
#include "../app_rc_wifi_api.h"
#include "vendor/recording_card/app_recording_card/app_recording_card_api.h"

#define TLKDRV_SPI_CMD_FORMAT 0XABA5
#define TLKDRV_SPI_RSP_FORMAT 0XBA5A

typedef enum
{
    WIFI_SPI_STATE_MACHINE_IDLE = 0,
    WIFI_SPI_STATE_MACHINE_CMD_BLOCK,
    WIFI_SPI_STATE_MACHINE_CMD_WAIT_RSP,
    WIFI_SPI_STATE_MACHINE_DATA_BLOCK,
} WIFI_SPI_STATE_MACHINE_ENUM;

typedef struct _AppRcWifiSpiItem_t
{
    uint32_t dataLen;
    uint8_t  data[500];
} AppRcWifiSpiItem_t;

typedef struct
{
    uint8_t            isInProcess;
    AppRcWifiSpiItem_t item[1];
} AppRcWifiSpiResend_t;

typedef struct
{
    uint8_t                        irqChn;
    uint8_t                        state;
    uint8_t                        cmd;
    uint8_t                        cmdLen;
    uint16_t                       dataLen;
    uint16_t                       resv16bit;
    uint32_t                       sendNum;
    rc_wifi_spi_send_finished_cb_t dataSendCB;
    rc_wifi_spi_send_finished_cb_t cmdSendCB;
    TlkApiTimer_t                  timer;
    uint8_t                        cmdCache[24];
    uint8_t                        dataCache[500];
#if APP_WIFI_SPI_ERR_RESEND_EN
    AppRcWifiSpiResend_t reSend;
#endif
} AppRcWifiSpi_t;

static AppRcWifiSpi_t sAppRcWifiSpi = {0};

static inline void app_rc_wifi_spi_hal_init(void)
{
    lspi_pin_config_t cfg = {
        .spi_csn_pin      = APP_WIFI_SPI_CSN_PIN,
        .spi_clk_pin      = APP_WIFI_SPI_CLK_PIN,
        .spi_mosi_io0_pin = APP_WIFI_SPI_MOSI_PIN,
        .spi_miso_io1_pin = APP_WIFI_SPI_MISO_PIN,
        .spi_io2_pin      = 0,
        .spi_io3_pin      = 0,
    };
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
    spi_master_init(LSPI_MODULE, SRC_CLK_XTAL_48M, SRC_CLK_XTAL_48M / APP_WIFI_SPI_CLK_HZ, SPI_MODE0);
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
    spi_master_init(LSPI_MODULE, sys_clk.pll_clk * 1000000 / APP_WIFI_SPI_CLK_HZ, SPI_MODE0);
#endif
    spi_clr_irq_status(LSPI_MODULE, SPI_END_INT);
    spi_set_irq_mask(LSPI_MODULE, SPI_END_INT_EN);
    spi_set_tx_dma_config(LSPI_MODULE, tlkhal_dma_malloc());
    lspi_set_pin(&cfg);
    spi_master_config(LSPI_MODULE, SPI_NORMAL);
}

static inline bool app_rc_wifi_spi_flow_is_busy(void)
{
    return gpio_get_level(APP_WIFI_SPI_FLOW_CTRL_PIN) != 0;
}

static inline void app_rc_wifi_spi_hal_send(uint8_t *pData, uint16_t dataLen)
{
    spi_clr_irq_status(LSPI_MODULE, SPI_END_INT);
    spi_master_write_dma_plus(LSPI_MODULE, SPI_WRITE_DATA_SINGLE_CMD, (unsigned int)NULL, pData, dataLen, SPI_MODE_WR_WRITE_ONLY);
    while (!spi_get_irq_status(LSPI_MODULE, SPI_END_INT));
    spi_clr_irq_status(LSPI_MODULE, SPI_END_INT);
}

static void app_rc_wifi_spi_core_send_cmd(uint8_t cmd, uint8_t *pData, uint8_t dataLen)
{
    uint8_t buffer[512];
    uint8_t bufferLen = 0;
    if (pData == NULL) {
        dataLen = 0;
    }
    buffer[bufferLen++] = 0xFF;
    buffer[bufferLen++] = 0xFF;
    buffer[bufferLen++] = (TLKDRV_SPI_CMD_FORMAT >> 8) & 0xff;
    buffer[bufferLen++] = TLKDRV_SPI_CMD_FORMAT & 0xff;
    buffer[bufferLen++] = dataLen + 1;
    buffer[bufferLen++] = cmd;
    if (dataLen != 0) {
        memcpy(buffer + bufferLen, pData, dataLen);
        bufferLen += dataLen;
    }
    app_rc_wifi_spi_hal_send(buffer, bufferLen);
}

static void app_rc_wifi_spi_core_send_data(uint32_t packageNum, uint8_t *data, uint16_t dataLen)
{
    uint8_t  buffer[512];
    uint16_t bufferLen  = 0;
    buffer[bufferLen++] = 0xFF;
    buffer[bufferLen++] = 0xFF;
    uint16_t totalLen   = dataLen + 2;
    buffer[bufferLen++] = (totalLen >> 8) & 0xff;
    buffer[bufferLen++] = totalLen & 0xff;
    buffer[bufferLen++] = (packageNum >> 8) & 0xff;
    buffer[bufferLen++] = packageNum & 0xff;

    memcpy(buffer + bufferLen, data, dataLen);
    bufferLen += dataLen;

    uint32_t checksum = 0;
    for (size_t i = 0; i < dataLen; i++) {
        checksum += data[i];
    }
    buffer[bufferLen++] = (checksum >> 16) & 0xff;
    buffer[bufferLen++] = (checksum >> 8) & 0xff;
    buffer[bufferLen++] = (checksum >> 0) & 0xff;
#if APP_WIFI_SPI_ERR_SIMU
    if (sAppRcWifiSpi.reSend.isInProcess == 0) {
        uint32_t rnd = trng_rand() % 20;
        if (rnd == 0) {
            return; //no send
        } else if (rnd == 1) {
            buffer[bufferLen - 1] = 0;
            buffer[bufferLen - 2] = 0;
            buffer[bufferLen - 3] = 0;
            buffer[2]             = 1;
            buffer[3]             = 2;
            buffer[4]             = 3;
            buffer[5]             = 4;
        } //error send
    }
#endif
    app_rc_wifi_spi_hal_send(buffer, bufferLen);
}

#if APP_WIFI_SPI_ERR_RESEND_EN
static inline void app_rc_wifi_spi_resend_delay(uint32_t ms)
{
    if (tlkos_task_delayMs(ms) == -TLK_ENOSUPPORT) {
        delay_ms(ms);
    }
}

static void app_rc_wifi_spi_resend_process(void)
{
    if (sAppRcWifiSpi.sendNum == 0) {
        return;
    }
    //spi slave toggle miso level when receive package
    if (gpio_get_level((gpio_pin_e)APP_WIFI_SPI_MISO_PIN) == (sAppRcWifiSpi.sendNum % 2)) {
        return;
    }
    tlkapi_error(0xFFFFFFFF, "[WIFI]", "spi start resend %d", sAppRcWifiSpi.sendNum - 1);
    sAppRcWifiSpi.reSend.isInProcess = 1;
    app_rc_wifi_spi_resend_delay(200);
    do {
        while (app_rc_wifi_spi_flow_is_busy());
        uint32_t            num  = sAppRcWifiSpi.sendNum - 1;
        AppRcWifiSpiItem_t *item = (AppRcWifiSpiItem_t *)&sAppRcWifiSpi.reSend.item[0];
        app_rc_wifi_spi_core_send_data(num, item->data, item->dataLen);
        app_rc_wifi_spi_resend_delay(10);
    } while (gpio_get_level((gpio_pin_e)APP_WIFI_SPI_MISO_PIN) != (sAppRcWifiSpi.sendNum % 2));
    while (app_rc_wifi_spi_flow_is_busy());
    sAppRcWifiSpi.reSend.isInProcess = 0;
}

static inline void app_rc_wifi_spi_reserve_package(uint32_t packageNum, uint8_t *data, uint16_t dataLen)
{
    (void)packageNum;
    AppRcWifiSpiItem_t *item = (AppRcWifiSpiItem_t *)&sAppRcWifiSpi.reSend.item[0];
    memcpy(item->data, data, dataLen);
    item->dataLen = dataLen;
}

static void app_rc_wifi_spi_try_send_data(uint32_t packageNum, uint8_t *data, uint16_t dataLen)
{
    //use miso io to check is need resend
    if (packageNum == 0) {
        gpio_function_en((gpio_pin_e)APP_WIFI_SPI_MISO_PIN);
        gpio_input_en((gpio_pin_e)APP_WIFI_SPI_MISO_PIN);
        gpio_set_up_down_res((gpio_pin_e)APP_WIFI_SPI_MISO_PIN, GPIO_PIN_PULLUP_1M);
        app_rc_wifi_spi_resend_delay(1);
    }
    app_rc_wifi_spi_resend_process();
    app_rc_wifi_spi_reserve_package(packageNum, data, dataLen);
    app_rc_wifi_spi_core_send_data(packageNum, data, dataLen);
}
#else
static void app_rc_wifi_spi_try_send_data(uint16_t packageNum, uint8_t *data, uint16_t dataLen)
{
    app_rc_wifi_spi_core_send_data(packageNum, data, dataLen);
}
#endif

static void app_rc_wifi_spi_cmd_rsp_timer(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)pTimer;
    (void)userArg;
    uint8_t last_state  = sAppRcWifiSpi.state;
    sAppRcWifiSpi.state = WIFI_SPI_STATE_MACHINE_IDLE;
    if (last_state != WIFI_SPI_STATE_MACHINE_CMD_WAIT_RSP) {
        return;
    }
    uint8_t rdata[10] = {0};
    spi_master_read(LSPI_MODULE, rdata, sizeof(rdata));
    spi_set_transmode(LSPI_MODULE, (spi_tans_mode_e)SPI_MODE_WR_WRITE_ONLY);
    tlkapi_array(0xFFFFFFFF, "[WIFI]", "spi rsp", rdata, sizeof(rdata));
    uint8_t dataLen = rdata[4];
    uint8_t cmd_ack = rdata[5];
    if (dataLen == 0 || (rdata[2] != ((TLKDRV_SPI_RSP_FORMAT >> 8) & 0xff)) || rdata[3] != (TLKDRV_SPI_RSP_FORMAT & 0xff) || cmd_ack != sAppRcWifiSpi.cmd) {
        if (sAppRcWifiSpi.cmdSendCB != NULL) {
            sAppRcWifiSpi.cmdSendCB(false, NULL, 0);
        }
        return;
    }
    if (sAppRcWifiSpi.cmdSendCB != NULL) {
        sAppRcWifiSpi.cmdSendCB(true, rdata + 6, dataLen - 1);
    }
}

static void app_rc_wifi_spi_gpioIrqHandler(void)
{
    tlkhal_gpio_stopIrqChn(sAppRcWifiSpi.irqChn);
    tlksys_task_setEvtFromIsr(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_SPI_IO_INTERRUPT);
}

static void app_rc_wifi_spi_flow_busy2okCB(void)
{
    if (app_rc_wifi_spi_flow_is_busy()) {
        uint32_t r = core_interrupt_disable();
        tlkhal_gpio_startIrqChn(sAppRcWifiSpi.irqChn);
        core_restore_interrupt(r);
        return;
    }
    if (sAppRcWifiSpi.state == WIFI_SPI_STATE_MACHINE_DATA_BLOCK) {
        sAppRcWifiSpi.state = WIFI_SPI_STATE_MACHINE_IDLE;
        app_rc_wifi_spi_try_send_data(sAppRcWifiSpi.sendNum, sAppRcWifiSpi.dataCache, sAppRcWifiSpi.dataLen);
        sAppRcWifiSpi.sendNum++;
        sAppRcWifiSpi.dataSendCB(true, NULL, 0);
    } else if (sAppRcWifiSpi.state == WIFI_SPI_STATE_MACHINE_CMD_BLOCK) {
        sAppRcWifiSpi.state = WIFI_SPI_STATE_MACHINE_CMD_WAIT_RSP;
        app_rc_wifi_spi_core_send_cmd(sAppRcWifiSpi.cmd, sAppRcWifiSpi.cmdCache, sAppRcWifiSpi.cmdLen);
        tlksys_timer_reStart(APP_RC_THREAD_ID, &sAppRcWifiSpi.timer);
    }
}

int app_rc_wifi_spi_send_cmd(uint8_t cmd, uint8_t *pData, uint16_t dataLen, rc_wifi_spi_send_finished_cb_t cb)
{
    if (dataLen > 24) {
        return -TLK_EPARAM;
    }
    if (sAppRcWifiSpi.state != WIFI_SPI_STATE_MACHINE_IDLE) {
        return -TLK_EFAIL;
    }
    sAppRcWifiSpi.cmd       = cmd;
    sAppRcWifiSpi.cmdSendCB = cb;
    if (app_rc_wifi_spi_flow_is_busy()) {
        sAppRcWifiSpi.cmdLen = dataLen;
        memcpy(sAppRcWifiSpi.cmdCache, pData, dataLen);
    }
    if (app_rc_wifi_spi_flow_is_busy()) {
        uint32_t r = core_interrupt_disable();
        tlkhal_gpio_startIrqChn(sAppRcWifiSpi.irqChn);
        sAppRcWifiSpi.state = WIFI_SPI_STATE_MACHINE_CMD_BLOCK;
        core_restore_interrupt(r);
        return TLK_ENONE;
    }
    app_rc_wifi_spi_core_send_cmd(cmd, pData, dataLen);
    sAppRcWifiSpi.state = WIFI_SPI_STATE_MACHINE_CMD_WAIT_RSP;
    tlksys_timer_reStart(APP_RC_THREAD_ID, &sAppRcWifiSpi.timer);
    return TLK_ENONE;
}

int app_rc_wifi_spi_send_data(uint8_t *pData, uint16_t dataLen, rc_wifi_spi_send_finished_cb_t cb)
{
    if (dataLen > 500) {
        return -TLK_EPARAM;
    }
    if (sAppRcWifiSpi.state != WIFI_SPI_STATE_MACHINE_IDLE) {
        return -TLK_EFAIL;
    }
    if (app_rc_wifi_spi_flow_is_busy()) {
        memcpy(sAppRcWifiSpi.dataCache, pData, dataLen);
        sAppRcWifiSpi.dataLen    = dataLen;
        sAppRcWifiSpi.dataSendCB = cb;
    }
    if (app_rc_wifi_spi_flow_is_busy()) {
        uint32_t r = core_interrupt_disable();
        tlkhal_gpio_startIrqChn(sAppRcWifiSpi.irqChn);
        sAppRcWifiSpi.state = WIFI_SPI_STATE_MACHINE_DATA_BLOCK;
        core_restore_interrupt(r);
        return TLK_ENONE;
    }
    app_rc_wifi_spi_try_send_data(sAppRcWifiSpi.sendNum, pData, dataLen);
    sAppRcWifiSpi.sendNum++;
    if (cb) {
        cb(true, NULL, 0);
    }
    return TLK_ENONE;
}

void app_rc_wifi_spi_cleanFrameNum(void)
{
    sAppRcWifiSpi.sendNum = 0;
#if APP_WIFI_SPI_ERR_RESEND_EN
    gpio_function_dis((gpio_pin_e)APP_WIFI_SPI_MISO_PIN);
    gpio_input_dis((gpio_pin_e)APP_WIFI_SPI_MISO_PIN);
    gpio_set_up_down_res((gpio_pin_e)APP_WIFI_SPI_MISO_PIN, GPIO_PIN_UP_DOWN_FLOAT);
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
    lspi_set_pin_mux((gpio_func_pin_e)APP_WIFI_SPI_MISO_PIN, LSPI_MISO_IO);
#else
    lspi_set_pin_mux((gpio_func_pin_e)APP_WIFI_SPI_MISO_PIN, 0);
#endif
    memset(&sAppRcWifiSpi.reSend, 0, sizeof(sAppRcWifiSpi.reSend));
#endif
}

void app_rc_wifi_spi_init(void)
{
    void app_rc_wifi_core_init(void);
    app_rc_wifi_core_init();
    app_rc_wifi_spi_hal_init();
    gpio_function_en(APP_WIFI_SPI_FLOW_CTRL_PIN);
    gpio_output_dis(APP_WIFI_SPI_FLOW_CTRL_PIN);
    gpio_input_en(APP_WIFI_SPI_FLOW_CTRL_PIN);
    gpio_set_up_down_res(APP_WIFI_SPI_FLOW_CTRL_PIN, GPIO_PIN_PULLUP_1M);
    TlkhalGpioIrqChnCfg_t cfg = {
        .cb          = app_rc_wifi_spi_gpioIrqHandler,
        .gpio        = APP_WIFI_SPI_FLOW_CTRL_PIN,
        .triggerMode = TLKHAL_GPIO_IRQ_TRIGGER_FALLING_EDGE,
    };
    tlkhal_gpio_mallocIrqChn(&sAppRcWifiSpi.irqChn, &cfg);
    tlksys_timer_createStatic(APP_RC_THREAD_ID, &sAppRcWifiSpi.timer, 100 * 1000, false, app_rc_wifi_spi_cmd_rsp_timer, NULL);
    tlksys_task_regEvtCB(APP_RC_THREAD_ID, APP_RC_THREAD_EVT_SPI_IO_INTERRUPT, app_rc_wifi_spi_flow_busy2okCB);
}

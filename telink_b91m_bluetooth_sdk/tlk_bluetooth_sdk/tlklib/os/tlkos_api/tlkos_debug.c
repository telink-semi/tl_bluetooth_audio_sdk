/********************************************************************************************************
 * @file    tlkos_debug.c
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
#include "tlklib/os/tlkos_config.h"
#include "tlklib/usb/tlkusb.h"

#if TLKOS_CFG_DEBUG_ENABLE
volatile uint32_t  AAA_OS_crashReason = TLKOS_CRASH_REASON_NONE;
extern const char *tlkos_debug_getCoreInfo(void);

/**
 * @brief       This function disables USB event mode and handles USB interrupts to print logs in FIFO.
 * @param[in]   none.
 * @return      none.
 */
static void tlkos_crash_startAPI(void)
{
    tlkusb_hal_disable_eventMode();
    for (uint32_t i = 0; i < 1000; i++) {
        tlk_udb_usb_handle_irq();
        delay_ms(1);
    } //print pre logs in fifo;
}

/**
 * @brief       This function prints log information via USB and handles USB interrupts.
 * @param[in]   log   - pointer to the log string to print.
 * @return      none.
 */
static void tlkos_crash_printAPI(const char *log)
{
    tlkapi_printf(TLKOS_CFG_DEBUG_ENABLE, log);
    for (uint32_t i = 0; i < 20; i++) {
        tlk_udb_usb_handle_irq();
        delay_ms(1);
    }
}

_attribute_ram_code_sec_noinline_
    /**
 * @brief       This function handles system crash events, disables interrupts and prints crash information.
 * @param[in]   info   - pointer to the crash information structure containing reason and detailed info.
 * @return      none.
 */
    void
    tlkos_crash(const TlkOsCrashInfo_t *info)
{
    if (AAA_OS_crashReason != TLKOS_CRASH_REASON_NONE || info->reason == TLKOS_CRASH_REASON_NONE) {
        return;
    }
    (void)tlkos_crash_startAPI;
    (void)tlkos_crash_printAPI;

    core_interrupt_disable();
#if TLKOS_CFG_DEBUG_INFO_OUT
    tlkos_crash_startAPI();
    AAA_OS_crashReason = info->reason;
    tlkos_crash_printAPI("[OS_CRASH]**********[OS_CRASH]");
    if (info->detailInfo) {
        tlkos_crash_printAPI(info->detailInfo);
    }
    const char *log = tlkos_debug_getCoreInfo();
    while (log) {
        tlkos_crash_printAPI(log);
        log = tlkos_debug_getCoreInfo();
    }
#endif
    while (1);
}

#endif

#if !TLKOS_CFG_DEBUG_ENABLE
_attribute_ram_code_sec_
    /**
 * @brief       This function handles system crash events when debug is disabled.
 * @param[in]   info   - pointer to the crash information structure.
 * @return      none.
 */
    void
    tlkos_crash(const TlkOsCrashInfo_t *info)
{
    (void)info;
}
#endif

/**
 * @brief       This function initializes the debug IOs according to configuration.
 * @param[in]   none.
 * @return      none.
 */
void tlkos_debug_ioInit(void)
{
#if TLKOS_CFG_DEBUG_IO_ENABLE
    gpio_function_en(GPIO_PF4 | GPIO_PF5);
    gpio_output_en(GPIO_PF4 | GPIO_PF5);
    gpio_function_en(GPIO_PA0 | GPIO_PA1 | GPIO_PA2 | GPIO_PA3 | GPIO_PA5);
    gpio_output_en(GPIO_PA0 | GPIO_PA1 | GPIO_PA2 | GPIO_PA3 | GPIO_PA5);
    gpio_function_en(GPIO_PB5 | GPIO_PB6 | GPIO_PB0);
    gpio_output_en(GPIO_PB5 | GPIO_PB6 | GPIO_PB0);
    gpio_function_en(GPIO_PC0 | GPIO_PC1);
    gpio_output_en(GPIO_PC0 | GPIO_PC1);
#endif
}

#if TLKOS_CFG_DEBUG_IO_ENABLE
_attribute_data_retention_sec_ static uint16_t tlkos_debug_ioTab[TLKOS_DEBUG_IO_NUM] = {
    [TLKOS_DEBUG_IO_MTI] = GPIO_PA0,         [TLKOS_DEBUG_IO_MSI] = GPIO_PA1,         [TLKOS_DEBUG_IO_EXCEPT] = GPIO_PB6,       [TLKOS_DEBUG_IO_FLASH_WRITE_OR_TICKLESS] = GPIO_PB5,
    [TLKOS_DEBUG_IO_SYS_THREAD] = GPIO_PA2,  [TLKOS_DEBUG_IO_HOST_THREAD] = GPIO_PC0, [TLKOS_DEBUG_IO_AUD_M_THREAD] = GPIO_PC1, [TLKOS_DEBUG_IO_AUD_IRQ_THREAD] = GPIO_PB0,
    [TLKOS_DEBUG_IO_GET_MAILBOX] = GPIO_PF4, [TLKOS_DEBUG_IO_SWITCH_TASK] = GPIO_PF5, [TLKOS_DEBUG_IO_TIMER_THREAD] = GPIO_PA3, [TLKOS_DEBUG_IO_AUD_HIGHEST_THREAD] = GPIO_PA5,
};
#endif

/**
 * @brief       This function controls the specified debug IO channel to set its level.
 * @param[in]   chn      - the debug IO channel.
 * @param[in]   ioLevel  - the level to set, 0 for low level and 1 for high level.
 * @return      none.
 */
_attribute_ram_code_sec_ void tlkos_debug_ioCtrl(uint16_t chn, uint8_t ioLevel)
{
    (void)chn;
    (void)ioLevel;
#if TLKOS_CFG_DEBUG_IO_ENABLE
    if (chn >= TLKOS_DEBUG_IO_NUM) {
        return;
    }
    gpio_set_level(tlkos_debug_ioTab[chn], ioLevel);
#endif
}

/**
 * @brief       This function controls the specified debug IO channel with extended features.
 * @param[in]   chn            - the debug IO channel.
 * @param[in]   ioLevel        - the level to set, 0 for low level and 1 for high level.
 * @param[in]   exFeature      - extended feature selection.
 * @param[in]   exFeatureArg   - argument for the extended feature.
 * @return      none.
 */
_attribute_ram_code_sec_ void tlkos_debug_ioCtrlEx(uint16_t chn, uint8_t ioLevel, uint32_t exFeature, void *exFeatureArg)
{
    (void)chn;
    (void)ioLevel;
    (void)exFeature;
    (void)exFeatureArg;
#if TLKOS_CFG_DEBUG_IO_ENABLE
    if (exFeature == TLKOS_DEBUG_IO_EX_FEATURE_AUD_IRQ) {
        TlkOsEventTabHandle_t tlkmdi_audio_getIrqEvtHandle(void);
        TlkOsEventTabHandle_t audIrqHandle = tlkmdi_audio_getIrqEvtHandle();
        TlkOsEventTabHandle_t handle       = *((TlkOsEventTabHandle_t *)exFeatureArg);
        if (handle != audIrqHandle) {
            return;
        }
        gpio_set_level(tlkos_debug_ioTab[chn], ioLevel);
    }
#endif
}

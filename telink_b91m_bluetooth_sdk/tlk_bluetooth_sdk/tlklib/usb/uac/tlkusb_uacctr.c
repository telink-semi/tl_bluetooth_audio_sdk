/********************************************************************************************************
 * @file    tlkusb_uacctr.c
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
#include "tlklib/usb/tlkusb_stdio.h"
#if (TLK_USB_UAC_ENABLE)
#include "drivers.h"
#include "tlklib/usb/uac/tlkusb_uacDefine.h"
#include "tlklib/usb/uac/tlkusb_uacDesc.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacctr.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "tlklib/usb/uac/tlkusb_uacDesc_dual.h"

#include "stack/stack.h"
#include "stack/bt/host/btp/hfp/btp_hfp.h"
#include "tlklib/usb/tlkusb_core.h"
#include "tlkalg/audio/asrc_24bit/tlkalg_ppm_calc.h"
#define TLKUSB_UAC_VCD_ENABLE    1

#define TLKUSB_UAC_KEY_QUEUE_LEN 4

typedef struct
{
    uint8_t       wptr;
    uint8_t       rptr;
    uint8_t       toggle;
    uint8_t       state;
    uint16_t      queue[TLKUSB_UAC_KEY_QUEUE_LEN];
    TlkApiTimer_t timer;
} tlkusb_uacctrl_key_t;

static tlkusb_uacctrl_key_t s_tlkusb_uacctrl_key = {0};

static int  tlkusb_uacctrl_init(void);
static void tlkusb_uacctrl_reset(void);
static void tlkusb_uacctrl_deinit(void);
static void tlkusb_uacctrl_handler(void);
static int  tlkusb_uacctrl_getClassInf(tlkusb_setup_req_t *pSetup, uint8_t infNumb);
static int  tlkusb_uacctrl_setClassInf(tlkusb_setup_req_t *pSetup, uint8_t infNumb);
static int  tlkusb_uacctrl_getClassEdp(tlkusb_setup_req_t *pSetup, uint8_t edpNumb);
static int  tlkusb_uacctrl_setClassEdp(tlkusb_setup_req_t *pSetup, uint8_t edpNumb);
static int  tlkusb_uacctrl_getInterface(tlkusb_setup_req_t *pSetup, uint8_t infNumb);
static int  tlkusb_uacctrl_setInterface(tlkusb_setup_req_t *pSetup, uint8_t infNumb);

void                   tlklib_uacctrl_keychk_handler(void);
const tlkusb_modCtrl_t sTlkUsbUacModCtrl = {
    tlkusb_uacctrl_init,         // Init
    tlkusb_uacctrl_reset,        // Reset
    tlkusb_uacctrl_deinit,       // Deinit
    tlkusb_uacctrl_handler,      // Handler
    tlkusb_uacctrl_getClassInf,  // GetClassInf
    tlkusb_uacctrl_setClassInf,  // SetClassInf
    tlkusb_uacctrl_getClassEdp,  // GetClassEdp
    tlkusb_uacctrl_setClassEdp,  // SetClassEdp
    tlkusb_uacctrl_getInterface, // GetInterface
    tlkusb_uacctrl_setInterface, // SetInterface
};

#if ((TLKUSB_AUD_MIC_RESOLUTION_BIT == 24) && (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24) || TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
static TlkApiTimer_t g_tlk_uac_timer = {0};

/**
 * @brief       This function is the timer callback for UAC control.
 * @param[in]   pTimer  - Timer handle.
 * @param[in]   userArg - User argument.
 * @return      none.
 */
static void tlk_uac_timer_callback(TlkApiTimerHandle_t pTimer, void *userArg)
{
    (void)userArg;
    (void)pTimer;
    /*Only tpsll dongle need check here.*/
    if ((g_tlk_usb_cfg.tick_in != 0) && clock_time_exceed(g_tlk_usb_cfg.tick_in, 5000)) {
        g_tlk_usb_cfg.tick_in   = 0;
        g_tlk_usb_cfg.iso_in_en = false;
    }

    if ((g_tlk_usb_cfg.tick_out != 0) && clock_time_exceed(g_tlk_usb_cfg.tick_out, 5000)) {
        g_tlk_usb_cfg.tick_out   = 0;
        g_tlk_usb_cfg.iso_out_en = false;
    }

    if ((g_tlk_usb_cfg.tick_out1 != 0) && clock_time_exceed(g_tlk_usb_cfg.tick_out1, 5000)) {
        g_tlk_usb_cfg.tick_out1   = 0;
        g_tlk_usb_cfg.iso_out1_en = false;
    }

    if (sTlkUsbReportUacStatusCB != NULL) {
        sTlkUsbReportUacStatusCB(g_tlk_usb_cfg.iso_in_en, g_tlk_usb_cfg.iso_out_en || g_tlk_usb_cfg.iso_out1_en);
    }

    if (!g_tlk_usb_cfg.iso_in_en && !g_tlk_usb_cfg.iso_out_en && !g_tlk_usb_cfg.iso_out1_en) {
        return;
    }

    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, pTimer);
}
#endif

/**
 * @brief       This function initializes the USB UAC control module.
 * @return      TLK_ENONE - success.
 */
static int tlkusb_uacctrl_init(void)
{
    uint8_t iso;
    uint8_t irqMsk = 0;
#if ((MCU_CORE_TYPE != MCU_CORE_TL751X) && (MCU_CORE_TYPE != MCU_CORE_TL753X))
// 0-0x80 is used by VCD;
// TODO:The initialization of endpoint 8 is because VDC uses endpoint 8 for data exchange, and VCD is opened by default
#if (TLKUSB_UAC_VCD_ENABLE)
    reg_usb_ep_max_size    = (128 >> 3);
    reg_usb_ep8_send_thres = 0x40;
    reg_usb_ep8_send_max   = 128 >> 3;
    reg_usb_ep_buf_addr(8) = 0;
    reg_usb_ep8_fifo_mode  = 1;
#endif
#endif

    usbhw_set_eps_en(BIT(TLKUSB_UAC_EDP_MIC) | BIT(TLKUSB_UAC_EDP_SPK) | BIT(TLKUSB_UAC_EDP_SPK1) | BIT(TLKUSB_UAC_EDP_HID));

    usbhw_set_ep_addr(TLKUSB_UAC_EDP_HID, 0x80);
    usbhw_set_ep_addr(TLKUSB_UAC_EDP_MIC, 0xA0);
    usbhw_set_ep_addr(TLKUSB_UAC_EDP_SPK, 0x130);
    usbhw_set_ep_addr(TLKUSB_UAC_EDP_SPK1, 0x250);
    reg_usb_ep_max_size = (288 >> 3);

    iso = reg_usb_iso_mode;
#if (TLKUSB_UAC_MIC_ENABLE)
    iso |= (1 << TLKUSB_UAC_EDP_MIC);
    irqMsk |= BIT(TLKUSB_UAC_EDP_MIC); // audio in interrupt enable
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
    iso |= (1 << TLKUSB_UAC_EDP_SPK);
    irqMsk |= BIT(TLKUSB_UAC_EDP_SPK);
#if TLK_USB_UAC_DUAL_SOUNDCARD_MODE
    iso |= (1 << TLKUSB_UAC_EDP_SPK1);
    irqMsk |= BIT(TLKUSB_UAC_EDP_SPK1);
#endif
#endif
    reg_usb_iso_mode      = iso;
    reg_usb_ep_irq_mask   = irqMsk;
    reg_usb_ep8_fifo_mode = 0; // no fifo mode


    plic_set_priority(IRQ_USB_ENDPOINT, 2);
    plic_interrupt_enable(IRQ_USB_ENDPOINT);
#if TLK_USB_UAC_DUAL_SOUNDCARD_MODE
    tlkusb_set_uac_mode(TLKUSB_UAC_MODE_DUAL_SOUNDCARD);
#endif
#if TLK_USB_REMOTEWAKEUP_EN
    tlkusb_set_usb_suspend_enable(0, 1);
#if MCU_CORE_TYPE == MCU_CORE_TL721X
    reg_wakeup_en |= 0x01;
    gpio_set_up_down_res(GPIO_PA5, GPIO_PIN_PULLDOWN_100K);
    gpio_set_up_down_res(GPIO_PA6, GPIO_PIN_PULLUP_10K);
    pm_set_suspend_power_cfg(FLD_PD_USB_EN, 1);
#elif MCU_CORE_TYPE == MCU_CORE_TL751X
    pm_set_usb_wakeup();
    gpio_set_up_down_res(GPIO_PF2, GPIO_PIN_PULLDOWN_100K); //DM
    gpio_set_up_down_res(GPIO_PF3, GPIO_PIN_PULLUP_10K);    //DP
    pm_ext_32k_rc_set_suspend_power_cfg(FLD_PD_USB_EN, 1);
#endif
#endif
#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X)
    usb_set_pin(1);
#endif

#if ((TLKUSB_AUD_MIC_RESOLUTION_BIT == 24) && (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24) || TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &g_tlk_uac_timer, 5 * 1000, false, tlk_uac_timer_callback, NULL); //5ms
#endif

    return TLK_ENONE;
}

/**
 * @brief       This function resets the USB UAC control module.
 * @return      none.
 */
static void tlkusb_uacctrl_reset(void)
{
#if (TLKUSB_UAC_SPK_ENABLE)
    reg_usb_ep_ctrl(TLKUSB_UAC_EDP_SPK) = FLD_EP_DAT_ACK;
#endif
    reg_usb_ep_ctrl(USB_EDP_KEYBOARD_IN) = 0;
}

/**
 * @brief       This function deinitializes the USB UAC control module.
 * @return      none.
 */
static void tlkusb_uacctrl_deinit(void)
{
    plic_interrupt_disable(IRQ_USB_ENDPOINT);
}

/**
 * @brief       This function handles USB UAC control events.
 * @return      none.
 */
static void tlkusb_uacctrl_handler(void)
{
    /*Report mic and spk status to upper layer.*/

    if (g_tlk_usb_cfg.tick_in != 0 && (g_tlk_usb_cfg.iso_in_en == false)) {
        g_tlk_usb_cfg.iso_in_en = true;
    }

    if (g_tlk_usb_cfg.tick_out != 0 && (g_tlk_usb_cfg.iso_out_en == false)) {
        g_tlk_usb_cfg.iso_out_en = true;
    }

    if (g_tlk_usb_cfg.tick_out1 != 0 && (g_tlk_usb_cfg.iso_out1_en == false)) {
        g_tlk_usb_cfg.iso_out1_en = true;
    }

    if (g_tlk_usb_cfg.pend_tick != 0 && !clock_time_exceed(g_tlk_usb_cfg.pend_tick, TLKUSB_AUD_MIC_PENDING_TIME)) {
        return;
    }

    if (sTlkUsbReportUacStatusCB != NULL) {
        g_tlk_usb_cfg.pend_tick = 0;
        sTlkUsbReportUacStatusCB(g_tlk_usb_cfg.iso_in_en, g_tlk_usb_cfg.iso_out_en || g_tlk_usb_cfg.iso_out1_en);
    }

#if ((TLKUSB_AUD_MIC_RESOLUTION_BIT == 24) && (TLKUSB_AUD_SPK_RESOLUTION_BIT == 24) || TLK_USB_UAC_AUDIO_LOCAL_ENABLE)
    /*Only tpsll dongle need check here.*/
    if ((g_tlk_usb_cfg.tick_in != 0) && clock_time_exceed(g_tlk_usb_cfg.tick_in, 5000)) {
        g_tlk_usb_cfg.tick_in   = 0;
        g_tlk_usb_cfg.iso_in_en = false;
#if TLKALG_PPM_CALC_BY_SAMPLE
        tlkalg_clear_sample_ppm_mode(&g_uac_ppm_ctrl, ASRC_VOICE_MODE);
#endif
    }

    if ((g_tlk_usb_cfg.tick_out != 0) && clock_time_exceed(g_tlk_usb_cfg.tick_out, 5000)) {
        g_tlk_usb_cfg.tick_out   = 0;
        g_tlk_usb_cfg.iso_out_en = false;
#if TLKALG_PPM_CALC_BY_SAMPLE
        tlkalg_clear_sample_ppm_mode(&g_uac_ppm_ctrl, ASRC_MUSIC_MODE);
#endif
    }

    if ((g_tlk_usb_cfg.tick_out1 != 0) && clock_time_exceed(g_tlk_usb_cfg.tick_out1, 5000)) {
        g_tlk_usb_cfg.tick_out1   = 0;
        g_tlk_usb_cfg.iso_out1_en = false;
#if TLKALG_PPM_CALC_BY_SAMPLE
        tlkalg_clear_sample_ppm_mode(&g_uac_ppm_ctrl, ASRC_MUSIC1_MODE);
#endif
    }

    tlksys_timer_reStart(TLKSYS_TASKID_SYSTEM, &g_tlk_uac_timer);
#endif

#if (TLKUSB_UAC_HID_ENABLE)
    tlklib_uacctrl_keychk_handler();
#endif
}

/**
 * @brief       This function handles USB UAC control GET_CLASS request for interface.
 * @param[in]   pSetup  - Setup packet.
 * @param[in]   infNumb - Interface number.
 * @return      0 - success, negative - failure.
 */
static int tlkusb_uacctrl_getClassInf(tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
    (void)infNumb;
    int     ret    = -TLK_ENOSUPPORT;
    uint8_t entity = (pSetup->wIndex >> 8);
    uint8_t valueH = (pSetup->wValue >> 8) & 0xff;
    switch (entity) {
#if (TLKUSB_UAC_SPK_ENABLE)
    case USB_SPEAKER_FEATURE_UNIT_ID:
        ret = tlkusb_uacspk_getInfCmdDeal(pSetup->bRequest, valueH);
        break;

    case TLKUSB_AUDID_SPK1_FEATURE_UNIT_ID:
        ret = tlkusb_uacspk1_getInfCmdDeal(pSetup->bRequest, valueH);
        break;

#endif
#if (TLKUSB_UAC_MIC_ENABLE)
    case USB_MIC_FEATURE_UNIT_ID:
        ret = tlkusb_uacmic_getInfCmdDeal(pSetup->bRequest, valueH);
        break;
#endif
    default:
        break;
    }
    return ret;
}

/**
 * @brief       This function handles USB UAC control SET_CLASS request for interface.
 * @param[in]   pSetup  - Setup packet.
 * @param[in]   infNumb - Interface number.
 * @return      0 - success, negative - failure.
 */
static int tlkusb_uacctrl_setClassInf(tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
    (void)infNumb;
    int     ret;
    uint8_t valueH = (pSetup->wValue >> 8) & 0xff;
    uint8_t entity = (pSetup->wIndex >> 8) & 0xff;

    ret = -TLK_ENOSUPPORT;
    switch (entity) {
#if (TLKUSB_UAC_SPK_ENABLE)
    case USB_SPEAKER_FEATURE_UNIT_ID:
        ret = tlkusb_uacspk_setInfCmdDeal(valueH);
        break;

    case TLKUSB_AUDID_SPK1_FEATURE_UNIT_ID:
        ret = tlkusb_uacspk1_setInfCmdDeal(valueH);
        break;

#endif
#if (TLKUSB_UAC_MIC_ENABLE)
    case USB_MIC_FEATURE_UNIT_ID:
        ret = tlkusb_uacmic_setInfCmdDeal(valueH);
        break;
#endif
    default:
        break;
    }
    return ret;
}

/**
 * @brief       This function handles USB UAC control GET_CLASS request for endpoint.
 * @param[in]   pSetup  - Setup packet.
 * @param[in]   edpNumb - Endpoint number.
 * @return      TLK_ENONE - success.
 */
static int tlkusb_uacctrl_getClassEdp(tlkusb_setup_req_t *pSetup, uint8_t edpNumb)
{
    (void)pSetup;
    (void)edpNumb;
    return TLK_ENONE;
}

/**
 * @brief       This function handles USB UAC control SET_CLASS request for endpoint.
 * @param[in]   pSetup  - Setup packet.
 * @param[in]   edpNumb - Endpoint number.
 * @return      0 - success, negative - failure.
 */
static int tlkusb_uacctrl_setClassEdp(tlkusb_setup_req_t *pSetup, uint8_t edpNumb)
{
    int     ret;
    uint8_t valueH = (pSetup->wValue >> 8) & 0xff;

    ret = -TLK_ENOSUPPORT;
    switch (edpNumb) {
#if (TLKUSB_UAC_SPK_ENABLE)
    case TLKUSB_UAC_EDP_SPK:
        ret = tlkusb_uacspk_setEdpCmdDeal(valueH);
        break;

    case TLKUSB_UAC_EDP_SPK1:
        ret = tlkusb_uacspk1_setEdpCmdDeal(valueH);
        break;
#endif
#if (TLKUSB_UAC_MIC_ENABLE)
    case TLKUSB_UAC_EDP_MIC:
        ret = tlkusb_uacmic_setEdpCmdDeal(valueH);
        break;
#endif
    default:
        break;
    }
    return ret;
}

/**
 * @brief       This function handles USB UAC control GET_INTERFACE request.
 * @param[in]   pSetup  - Setup packet.
 * @param[in]   infNumb - Interface number.
 * @return      TLK_ENONE - success.
 */
static int tlkusb_uacctrl_getInterface(tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
    (void)infNumb;
    uint8_t infNum = (pSetup->wIndex) & 0x07;
#if (TLKUSB_UAC_MIC_ENABLE)
    if (tlkusb_get_uac_mic_inf(infNum)) {
        tlkusb_hal_write_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX, tlkusb_uacmic_getEnable());
    }
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
    if (tlkusb_get_uac_spk_inf(infNum)) {
        tlkusb_hal_write_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX, tlkusb_uacspk_getEnable());
    }

    if (tlkusb_get_uac_spk1_inf(infNum)) {
        tlkusb_hal_write_ctrl_ep_data(TLK_CFG_USB_UAC_INDEX, tlkusb_uacspk1_getEnable());
    }
#endif
    return TLK_ENONE;
}

/**
 * @brief       This function handles USB UAC control SET_INTERFACE request.
 * @param[in]   pSetup  - Setup packet.
 * @param[in]   infNumb - Interface number.
 * @return      TLK_ENONE - success.
 */
static int tlkusb_uacctrl_setInterface(tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
    (void)infNumb;
    uint8_t enable = (pSetup->wValue) & 0xff;
    uint8_t infNum = (pSetup->wIndex) & 0x07;

#if (TLKUSB_UAC_MIC_ENABLE)
    if (tlkusb_get_uac_mic_inf(infNum)) {
        tlkusb_uacmic_setEnable(enable);
    }
#endif
#if (TLKUSB_UAC_SPK_ENABLE)
    if (tlkusb_get_uac_spk_inf(infNum)) {
        tlkusb_uacspk_setEnable(enable);
    }

    if (tlkusb_get_uac_spk1_inf(infNum)) {
        tlkusb_uacspk1_setEnable(enable);
    }
#endif
    return TLK_ENONE;
}

/**
 * @brief       This function reports HID data through USB audio.
 * @param[in]   report_id - Report ID.
 * @param[in]   data      - Data to report.
 * @param[in]   cnt       - Count of data bytes.
 * @return      1 - success.
 */
static int usbaudio_hid_report(uint8_t report_id, uint8_t *data, int cnt)
{
    if (usbhw_is_ep_busy(USB_EDP_KEYBOARD_IN)) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "EP isBusy");
    }
    reg_usb_ep_ptr(USB_EDP_KEYBOARD_IN) = 0;
    reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = report_id;
    for (int i = 0; i < cnt; i++) {
        reg_usb_ep_dat(USB_EDP_KEYBOARD_IN) = data[i];
    }
    reg_usb_ep_ctrl(USB_EDP_KEYBOARD_IN) = FLD_EP_DAT_ACK | (s_tlkusb_uacctrl_key.toggle ? FLD_USB_EP_DAT1 : FLD_USB_EP_DAT0); // ACK
    s_tlkusb_uacctrl_key.toggle ^= 1;
    return 1;
}

/**
 * @brief       This function reports control key state through USB audio.
 * @param[in]   key - Key code to report.
 * @return      Result of usbaudio_hid_report function.
 */
static void tlkusb_uacctrl_report_ctrl_key(uint16_t key)
{
    tlkapi_trace(0xFFFFFFFF, "[UAC_HID]", "************uac_key_press 0x%x", key);
    tlksys_enter_critical();
    if ((s_tlkusb_uacctrl_key.wptr + 1) % TLKUSB_UAC_KEY_QUEUE_LEN == s_tlkusb_uacctrl_key.rptr) {
        tlksys_leave_critical();
        return;
    }
    s_tlkusb_uacctrl_key.queue[s_tlkusb_uacctrl_key.wptr] = key;
    s_tlkusb_uacctrl_key.wptr                             = (s_tlkusb_uacctrl_key.wptr + 1) % TLKUSB_UAC_KEY_QUEUE_LEN;
    tlksys_leave_critical();
    tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB);
}

/**
 * @brief       This function simulates pressing the volume up key.
 * @return      none.
 */
void tlkusb_uacctrl_volume_up(void)
{
    tlkusb_uacctrl_report_ctrl_key(0x01);
}

/**
 * @brief       This function simulates pressing the volume down key.
 * @return      none.
 */
void tlkusb_uacctrl_volume_down(void)
{
    tlkusb_uacctrl_report_ctrl_key(0x02);
}

/**
 * @brief       This function simulates pressing the play next track key.
 * @return      none.
 */
void tlklib_uacctrl_play_next(void)
{
    tlkusb_uacctrl_report_ctrl_key(0x10);
}

/**
 * @brief       This function simulates pressing the play previous track key.
 * @return      none.
 */
void tlklib_uacctrl_play_prev(void)
{
    tlkusb_uacctrl_report_ctrl_key(0x20);
}

/**
 * @brief       This function simulates pressing the play/pause key.
 * @return      none.
 */
void tlklib_uacctrl_play_pause(void)
{
    tlkusb_uacctrl_report_ctrl_key(0x08);
}

static void tlklib_uacctrl_key_timer(TlkApiTimerHandle_t handle, void *arg)
{
    (void)handle;
    (void)arg;
    if (s_tlkusb_uacctrl_key.state != TLKUSB_UAC_KEY_STATE_PRESSED) {
        return;
    }
    tlkapi_trace(0xFFFFFFFF, "[UAC_HID]", "************uac_key_release");
    uint16_t key = 0;
    usbaudio_hid_report(USB_HID_KB_MEDIA, (uint8_t *)&key, sizeof(key));
    s_tlkusb_uacctrl_key.state = TLKUSB_UAC_KEY_STATE_RELEASED;
    if (s_tlkusb_uacctrl_key.wptr != s_tlkusb_uacctrl_key.rptr) {
        tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB);
    }
}

/**
 * @brief       This function handles USB UAC control key check.
 * @return      none.
 */
void tlklib_uacctrl_keychk_handler(void)
{
    if (s_tlkusb_uacctrl_key.wptr == s_tlkusb_uacctrl_key.rptr) {
        return;
    }
    if (s_tlkusb_uacctrl_key.state == TLKUSB_UAC_KEY_STATE_PRESSED) {
        return;
    }
    uint16_t key              = s_tlkusb_uacctrl_key.queue[s_tlkusb_uacctrl_key.rptr];
    s_tlkusb_uacctrl_key.rptr = (s_tlkusb_uacctrl_key.rptr + 1) % TLKUSB_UAC_KEY_QUEUE_LEN;
    usbaudio_hid_report(USB_HID_KB_MEDIA, (uint8_t *)&key, sizeof(key));
    s_tlkusb_uacctrl_key.state = TLKUSB_UAC_KEY_STATE_PRESSED;
    tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &s_tlkusb_uacctrl_key.timer, TLKLIB_UAC_KEY_TIMEOUT, 0, tlklib_uacctrl_key_timer, NULL);
    tlksys_timer_start(TLKSYS_TASKID_SYSTEM, &s_tlkusb_uacctrl_key.timer);
}

/**
 * @brief       This function accepts an incoming call.
 * @return      TLK_ENONE - success.
 */
int tlklib_uacctrl_callAccept(void)
{
    //todo: send usb hid to pc
    return TLK_ENONE;
}

/**
 * @brief       This function hangs up a call.
 * @return      TLK_ENONE - success.
 */
int tlklib_uacctrl_callHungUp(void)
{
    //todo: send usb hid to pc
    return TLK_ENONE;
}

#endif // #if (TLK_USB_UAC_ENABLE)

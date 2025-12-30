/********************************************************************************************************
 * @file    tlkmdi_audio_common.c
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

#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL322X)
#define TMR_IRQ_BIT FLD_TMR0_MODE_IRQ
#elif MCU_CORE_TL752X_TEMP
#define AUDIO_RX_FIFO0 (1U << 0)
#define TMR_IRQ_BIT    ((timer_irq_e)FLD_TMR0_MODE_IRQ)
#else
#define TMR_IRQ_BIT TMR_STA_TMR0
#endif

static TlkOsEventTabHandle_t sTlkMdiAudIrqEvtHandle              = 0;
static TlkOsMutexHandle_t    sTlkMdiAudCriticalMutexHandle       = 0;
static TlkOsSemphrHandle_t   sTlkMdiAudHighProTaskSemp           = 0;
static TlkMdiAudioCB_t       sTlkMdiAudioCB[TLKMDI_AUDIO_CB_NUM] = {0};
static TlkApiTimer_t         sTlkMdiAudioMainDelayRunTmr         = {0};

bool dsp_rdy_first_flag = true;

/**
 * @brief       Main delay run timer callback
 * @param[in]   handle - Timer handle
 * @param[in]   userArg - User argument
 * @return      None
 */
static void tlkmdi_audio_MainDelayRunTimer(TlkApiTimerHandle_t handle, void *userArg)
{
    (void)userArg;
    (void)handle;
    tlksys_task_setEvt(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_MAIN);
}

/**
 * @brief       Audio IRQ task
 * @param[in]   arg - Task argument
 * @return      None
 */
static void tlkmdi_audio_irq_task(void *arg)
{
    (void)arg;
    while (1) {
        tlkos_event_wait(sTlkMdiAudIrqEvtHandle, TLKOS_WAIT_FOREVER);
    }
}

/**
 * @brief       Audio high priority task
 * @param[in]   arg - Task argument
 * @return      None
 */
static void tlkmdi_audio_high_priority_task(void *arg)
{
    (void)arg;
    while (1) {
        tlkos_semphr_take(sTlkMdiAudHighProTaskSemp, TLKOS_WAIT_FOREVER);
        tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_AUD_HIGHEST_THREAD, 1);
        if (sTlkMdiAudioCB[TLKMDI_AUDIO_CB_HIGH_RIORITY_TASK]) {
            sTlkMdiAudioCB[TLKMDI_AUDIO_CB_HIGH_RIORITY_TASK]();
        }
        tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_AUD_HIGHEST_THREAD, 0);
    }
}

/**
 * @brief       Audio DSP main task
 * @return      None
 */
static void tlkmdi_audio_dsp_main_task(void)
{
// #if (!TLK_CFG_HRA_ENABLE && TLK_MW_DSP_COMM_ENABLE)
#if (TLK_MW_DSP_COMM_ENABLE)
    d25f_process_ipc_msg();
    d25f_process_dsp_log_msg();
#endif
}

/**
 * @brief       Audio DSP ready task
 * @return      None
 */
static void tlkmdi_audio_dsp_ready_task(void)
{
#if TLK_MW_DSP_COMM_ENABLE
    if (dsp_rdy_first_flag) {
        dsp_rdy_first_flag = false;
        return;
    }
#endif
#if (TLK_CFG_HRA_ENABLE)
    tlkmdi_audio_sendStartEvt(TLKAUD_TYPE_HRA, 0xffff);
#endif
#if (TLKHW_TYPE == TLKHW_TL751X_EVK_C1T368A110_V1_0)
    tlkmdi_interphone_api_mesh_enable();
#endif
}

/**
 * @brief       Audio main loop task
 * @return      None
 */
static void tlkmdi_audio_mainloop_task(void)
{
    if (sTlkMdiAudioCB[TLKMDI_AUDIO_CB_MAIN] == NULL) {
        return;
    }
    sTlkMdiAudioCB[TLKMDI_AUDIO_CB_MAIN]();
#if !TLK_CFG_RTOS_ENABLE
    tlksys_task_setEvt(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_MAIN);
#endif
}

/**
 * @brief       Audio handover handler
 * @return      None
 */
static void tlkmdi_audio_handover_handler(void)
{
    // TODO: Wangqing
    // Add switch codec mic to new Master and close other headset mic logic
}

/**
 * @brief       Get audio IRQ event handle
 * @return      Audio IRQ event handle
 */
TlkOsEventTabHandle_t tlkmdi_audio_getIrqEvtHandle(void)
{
    return sTlkMdiAudIrqEvtHandle;
}

/**
 * @brief       Get event task handler
 * @param[in]   evtID - Event ID
 * @return      Event task handler
 */
TlkOsEventDealCB tlkmdi_audio_getEvtTask(TLKSYS_TASK_EVT_ENUM evtID)
{
    static const TlkOsEventDealCB taskTab[TLKSYS_TASK_EVT_AUD_NUM] = {
        [TLKSYS_TASK_EVT_AUD_MAIN] = tlkmdi_audio_mainloop_task,      [TLKSYS_TASK_EVT_AUD_DSP_RDY] = tlkmdi_audio_dsp_ready_task,
        [TLKSYS_TASK_EVT_AUD_DSP] = tlkmdi_audio_dsp_main_task,       [TLKSYS_TASK_EVT_AUD_DEBUG] = tlkmdi_audio_debug_handler,
        [TLKSYS_TASK_EVT_AUD_HO_MIC] = tlkmdi_audio_handover_handler,
    };
    if (evtID >= TLKSYS_TASK_EVT_AUD_NUM) {
        return NULL;
    }
    return taskTab[evtID];
}

/**
 * @brief       Register audio callback
 * @param[in]   cbType - Callback type
 * @param[in]   cb - Callback function pointer
 * @return      None
 */
void tlkmdi_audio_register_cb(TLKMDI_AUDIO_CB_TYPE_ENUM cbType, TlkMdiAudioCB_t cb)
{
    if (cbType >= TLKMDI_AUDIO_CB_NUM) {
        return;
    }
    sTlkMdiAudioCB[cbType] = cb;
    if (cbType == TLKMDI_AUDIO_CB_MAIN) {
        tlksys_task_setEvt(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_MAIN);
    } else if (cbType == TLKMDI_AUDIO_CB_HIGH_RIORITY_TASK) {
        //resv
    } else {
        tlkos_event_regDealCB(sTlkMdiAudIrqEvtHandle, cbType, cb);
    }
}

/**
 * @brief       Initialize RTOS for audio module
 * @return      None
 */
void tlkmdi_audio_use_rtos(void)
{
    _attribute_os_heap_sec_ static uint8_t tlkmdi_audio_irq_task_buffer[TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE];
    TlkosTaskExtCfg_t                      cfg = {
                             .pStaticBuffer    = tlkmdi_audio_irq_task_buffer,
                             .staticBufferSize = TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE,
    };
    tlkos_task_create(tlkmdi_audio_irq_task, "AUD_IRQ", TLKMDI_AUDIO_IRQ_TASK_STASK_SIZE, TLKSYS_TASK_AUDIO_IRQ_PRIORITY, NULL, &cfg, NULL);

    tlkos_event_createTab(TLKMDI_AUDIO_CB_NUM, &sTlkMdiAudIrqEvtHandle);
    tlkos_recursiveMutex_create(&sTlkMdiAudCriticalMutexHandle);
    tlksys_timer_createStatic(TLKSYS_TASKID_AUDIO, &sTlkMdiAudioMainDelayRunTmr, 3 * 1000, false, tlkmdi_audio_MainDelayRunTimer, NULL);

    (void)tlkmdi_audio_high_priority_task;
#if TLKSTK_BT_TPS_ENABLE
    tlkos_task_create(tlkmdi_audio_high_priority_task, "AUD_H", 2 * 1024, TLKSYS_TASK_AUDIO_HIGH_TASK_PRIORITY, NULL, NULL, NULL);
    tlkos_semphr_createCounting(&sTlkMdiAudHighProTaskSemp, 10, 0);
#endif
}

/**
 * @brief       Enter critical section for audio operations
 * @return      None
 */
_always_inline void tlkmdi_audio_enterCritical(void)
{
#if TLK_CFG_RTOS_ENABLE
    tlkos_recursiveMutex_lock(sTlkMdiAudCriticalMutexHandle);
#else
    tlkos_enter_critical();
#endif
}

/**
 * @brief       Leave critical section for audio operations
 * @return      None
 */
_always_inline void tlkmdi_audio_leaveCritical(void)
{
#if TLK_CFG_RTOS_ENABLE
    tlkos_recursiveMutex_unlock(sTlkMdiAudCriticalMutexHandle);
#else
    tlkos_leave_critical();
#endif
}

/**
 * @brief       Run main task after specified time
 * @param[in]   ms - Delay time in milliseconds
 * @return      None
 */
void tlkmdi_audio_runMainAfterTimeMs(uint16_t ms)
{
    (void)ms;
#if TLK_CFG_RTOS_ENABLE
    tlksys_timer_stop(TLKSYS_TASKID_AUDIO, &sTlkMdiAudioMainDelayRunTmr);
    if (ms == 0) {
        tlksys_task_setEvt(TLKSYS_TASKID_AUDIO, TLKSYS_TASK_EVT_AUD_MAIN);
        return;
    }
    tlksys_timer_setPeriod(TLKSYS_TASKID_AUDIO, &sTlkMdiAudioMainDelayRunTmr, ms * 1000);
    tlksys_timer_start(TLKSYS_TASKID_AUDIO, &sTlkMdiAudioMainDelayRunTmr);
#endif
}

/**
 * @brief       Wake up high priority task
 * @return      None
 */
_attribute_ram_code_sec_ void tlkmdi_audio_wakeUpHighPriorityTask(void)
{
    if (sTlkMdiAudioCB[TLKMDI_AUDIO_CB_HIGH_RIORITY_TASK] == NULL) {
        return;
    }
    if (tlkos_get_irqState() == TLKOS_IRQ_STATE_IN_IRQ) {
        tlkos_semphr_giveFromISR(sTlkMdiAudHighProTaskSemp);
    } else {
        tlkos_semphr_give(sTlkMdiAudHighProTaskSemp);
    }
}

/**
 * @brief       Handle audio timer interrupt
 * @return      None
 */
_attribute_ram_code_sec_ void tlkmdi_audio_timer_irq_handler(void)
{
    if (!timer_get_irq_status(TMR_IRQ_BIT)) {
        return;
    }
#if !TLK_CFG_RTOS_ENABLE
    if (sTlkMdiAudioCB[TLKMDI_AUDIO_CB_TIMER]) {
        sTlkMdiAudioCB[TLKMDI_AUDIO_CB_TIMER]();
    }
#else
    tlkos_event_setFromIsr(sTlkMdiAudIrqEvtHandle, TLKMDI_AUDIO_CB_TIMER);
#endif
    timer_clr_irq_status(TMR_IRQ_BIT);
}

#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == CHIP_TYPE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL752X)
/**
 * @brief       Handle audio FIFO interrupt
 * @return      None
 */
_attribute_ram_code_sec_ void tlkmdi_audio_fifo_irq_handler(void)
{
#if TX_FIFO_IRQ_ENABLE
    if (audio_get_fifo_irq_status(AUDIO_TX_FIFO0)) {
        audio_clr_fifo_irq_status(AUDIO_TX_FIFO0);
#else
    if (audio_get_fifo_irq_status(AUDIO_RX_FIFO0)) {
        audio_clr_fifo_irq_status(AUDIO_RX_FIFO0);
#endif
#if !TLK_CFG_RTOS_ENABLE
        if (sTlkMdiAudioCB[TLKMDI_AUDIO_CB_FIFO]) {
            sTlkMdiAudioCB[TLKMDI_AUDIO_CB_FIFO]();
        }
#else
        tlkos_event_setFromIsr(sTlkMdiAudIrqEvtHandle, TLKMDI_AUDIO_CB_FIFO);
#endif
    }
}
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
/**
 * @brief       Handle audio FIFO interrupt
 * @return      None
 */
_attribute_ram_code_sec_ void tlkmdi_audio_fifo_irq_handler(void)
{
#if TX_FIFO_IRQ_ENABLE
    bool ret = audio_get_fifo_irq_status(AUDIO_TX_FIFO0);
    audio_clr_fifo_irq_status(AUDIO_TX_FIFO0);
#else
    bool ret = audio_get_irq_status(AUDIO_RX_FIFO0_IRQ); ///audio_get_fifo_irq_status(AUDIO_RX_FIFO0);
    audio_clr_irq_status(AUDIO_RX_FIFO0_IRQ);            //audio_clr_fifo_irq_status(AUDIO_RX_FIFO0);
#endif

    if (!ret) {
        return;
    }
#if !TLK_CFG_RTOS_ENABLE
    if (sTlkMdiAudioCB[TLKMDI_AUDIO_CB_FIFO]) {
        sTlkMdiAudioCB[TLKMDI_AUDIO_CB_FIFO]();
    }
#else
    tlkos_event_setFromIsr(sTlkMdiAudIrqEvtHandle, TLKMDI_AUDIO_CB_FIFO);
#endif
}
#endif

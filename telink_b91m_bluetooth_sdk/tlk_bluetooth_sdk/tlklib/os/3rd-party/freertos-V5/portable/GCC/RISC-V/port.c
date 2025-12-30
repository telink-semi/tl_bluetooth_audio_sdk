/*
 * FreeRTOS Kernel V10.4.2
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 * 1 tab == 4 spaces!
 */

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the RISC-V RV32 port.
 *----------------------------------------------------------*/

#include "../../../../../tlkos_config.h"
#if TLKOS_CFG_FREERTOS_ENABLE &&  !defined(MCU_CORE_N22)
/* Scheduler includes. */
#include "../../../include/FreeRTOS.h"
#include "../../../include/task.h"
#include "./portmacro.h"

/* Standard includes. */
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "string.h"
#include "drivers.h"

#include "stack/pm/pm_sys.h"
// #include "stack/debug/debug_internal.h"

#if configCLINT_BASE_ADDRESS
	#warning The configCLINT_BASE_ADDRESS constant has been deprecated.  configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS are currently being derived from the (possibly 0) configCLINT_BASE_ADDRESS setting.  Please update to define configMTIME_BASE_ADDRESS and configMTIMECMP_BASE_ADDRESS directly in place of configCLINT_BASE_ADDRESS.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifndef configMTIME_BASE_ADDRESS
	#warning configMTIME_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  If the target chip includes a memory-mapped mtime register then set configMTIME_BASE_ADDRESS to the mapped address.  Otherwise set configMTIME_BASE_ADDRESS to 0.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

#ifndef configMTIMECMP_BASE_ADDRESS
	#warning configMTIMECMP_BASE_ADDRESS must be defined in FreeRTOSConfig.h.  If the target chip includes a memory-mapped mtimecmp register then set configMTIMECMP_BASE_ADDRESS to the mapped address.  Otherwise set configMTIMECMP_BASE_ADDRESS to 0.  See https://www.FreeRTOS.org/Using-FreeRTOS-on-RISC-V.html
#endif

/* Let the user override the pre-loading of the initial LR with the address of
prvTaskExitError() in case it messes up unwinding of the stack in the
debugger. */
#ifdef configTASK_RETURN_ADDRESS
    #define portTASK_RETURN_ADDRESS configTASK_RETURN_ADDRESS
#else
    #define portTASK_RETURN_ADDRESS prvTaskExitError
#endif

/* The stack used by interrupt service routines.  Set configISR_STACK_SIZE_WORDS
to use a statically allocated array as the interrupt stack.  Alternative leave
configISR_STACK_SIZE_WORDS undefined and update the linker script so that a
linker variable names __freertos_irq_stack_top has the same value as the top
of the stack used by main.  Using the linker script method will repurpose the
stack that was used by main before the scheduler was started for use as the
interrupt stack after the scheduler has started. */
#ifdef configISR_STACK_SIZE_WORDS
    PRIVILEGED_DATA static __attribute__ ((aligned(16))) StackType_t xISRStack[ configISR_STACK_SIZE_WORDS ] = { 0 };
    PRIVILEGED_DATA StackType_t xISRStackTop = ( StackType_t ) &( xISRStack[ configISR_STACK_SIZE_WORDS & ~portBYTE_ALIGNMENT_MASK ] );

    /* Don't use 0xa5 as the stack fill bytes as that is used by the kernel for
    the task stacks, and so will legitimately appear in many positions within
    the ISR stack. */
    #define portISR_STACK_FILL_BYTE 0xee
#else
    extern const uint32_t __freertos_irq_stack_top[];
    PRIVILEGED_DATA StackType_t xISRStackTop = ( StackType_t ) __freertos_irq_stack_top;
#endif

/*
 * Setup the timer to generate the tick interrupts.  The implementation in this
 * file is weak to allow application writers to change the timer used to
 * generate the tick interrupt.
 */
void vPortSetupTimerInterrupt( void ) __attribute__(( weak ));

/*-----------------------------------------------------------*/

/* Used to program the machine timer compare register. */
PRIVILEGED_DATA uint64_t ullNextTime = 0ULL;
PRIVILEGED_DATA uint64_t *pullNextTime = &ullNextTime;
PRIVILEGED_DATA size_t uxTimerIncrementsForOneTick = ( size_t ) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ); /* Assumes increment won't go over 32-bits. */
PRIVILEGED_DATA uint32_t ullMachineTimerCompareRegisterBase = configMTIMECMP_BASE_ADDRESS;
PRIVILEGED_DATA volatile uint64_t * pullMachineTimerCompareRegister = NULL;

/* Set configCHECK_FOR_STACK_OVERFLOW to 3 to add ISR stack checking to task
stack checking.  A problem in the ISR stack will trigger an assert, not call the
stack overflow hook function (because the stack overflow hook is specific to a
task stack, not the ISR stack). */
#if defined( configISR_STACK_SIZE_WORDS ) && ( configCHECK_FOR_STACK_OVERFLOW > 2 )
    #warning This path not tested, or even compiled yet.

    static const uint8_t ucExpectedStackBytes[] = {
                                    portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE,     \
                                    portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE,     \
                                    portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE,     \
                                    portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE,     \
                                    portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE, portISR_STACK_FILL_BYTE };   \

    #define portCHECK_ISR_STACK() configASSERT( ( memcmp( ( void * ) xISRStack, ( void * ) ucExpectedStackBytes, sizeof( ucExpectedStackBytes ) ) == 0 ) )
#else
    /* Define the function away. */
    #define portCHECK_ISR_STACK()
#endif /* configCHECK_FOR_STACK_OVERFLOW > 2 */

/*-----------------------------------------------------------*/

#if( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 )
	_attribute_os_core_code_ram_sec_
	void vPortSetupTimerInterrupt( void )
	{
		uint32_t ulCurrentTimeHigh, ulCurrentTimeLow;
		volatile uint32_t * const pulTimeHigh = ( volatile uint32_t * const ) ( ( configMTIME_BASE_ADDRESS ) + 4UL ); /* 8-byte typer so high 32-bit word is 4 bytes up. */
		volatile uint32_t * const pulTimeLow = ( volatile uint32_t * const ) ( configMTIME_BASE_ADDRESS );
		volatile uint32_t ulHartId;

		__asm volatile( "csrr %0, mhartid" : "=r"( ulHartId ) );
		pullMachineTimerCompareRegister  = ( volatile uint64_t * ) ( ullMachineTimerCompareRegisterBase + ( ulHartId * sizeof( uint64_t ) ) );

		do
		{
			ulCurrentTimeHigh = *pulTimeHigh;
			ulCurrentTimeLow = *pulTimeLow;
		} while( ulCurrentTimeHigh != *pulTimeHigh );

		ullNextTime = ( uint64_t ) ulCurrentTimeHigh;
		ullNextTime <<= 32ULL; /* High 4-byte word is 32-bits up. */
		ullNextTime |= ( uint64_t ) ulCurrentTimeLow;
		ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
		*pullMachineTimerCompareRegister = ullNextTime;

		/* Prepare the time to use after the next tick interrupt. */
		ullNextTime += ( uint64_t ) uxTimerIncrementsForOneTick;
	}

#endif /* ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIME_BASE_ADDRESS != 0 ) */
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
extern void xPortStartFirstTask( void );

    #if( configASSERT_DEFINED == 1 )
    {
        volatile uint32_t mtvec = 0;

        /* Check the least significant two bits of mtvec are 00 - indicating
        single vector mode. */
        __asm volatile( "csrr %0, mtvec" : "=r"( mtvec ) );
//      configASSERT( ( mtvec & 0x03UL ) == 0 );

        /* Check alignment of the interrupt stack - which is the same as the
        stack that was being used by main() prior to the scheduler being
        started. */
        configASSERT( ( xISRStackTop & portBYTE_ALIGNMENT_MASK ) == 0 );

        #ifdef configISR_STACK_SIZE_WORDS
        {
            memset( ( void * ) xISRStack, portISR_STACK_FILL_BYTE, sizeof( xISRStack ) );
        }
        #endif   /* configISR_STACK_SIZE_WORDS */
    }
    #endif /* configASSERT_DEFINED */

	/* If there is a CLINT then it is ok to use the default implementation
	in this file, otherwise vPortSetupTimerInterrupt() must be implemented to
	configure whichever clock is to be used to generate the tick interrupt. */
	vPortSetupTimerInterrupt();

	#if( ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 ) )
	{
		/* Enable mtime and external interrupts.  1<<7 for timer interrupt, 1<<11
		for external interrupt.  _RB_ What happens here when mtime is not present as
		with pulpino? */
		__asm volatile( "csrs mie, %0" :: "r"(0x880) );
	}
	#else
	{
		/* Enable external interrupts. */
		__asm volatile( "csrs mie, %0" :: "r"(0x800) );
	}
	#endif /* ( configMTIME_BASE_ADDRESS != 0 ) && ( configMTIMECMP_BASE_ADDRESS != 0 ) */

    xPortStartFirstTask();

    /* Should not get here as after calling xPortStartFirstTask() only tasks
    should be executing. */
    return pdFAIL;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
    /* Not implemented. */
    for( ;; );
}

void vAssertCalled( const char * pcFile, unsigned long ulLine ){
    ( void ) pcFile; ( void ) ulLine; 
//  printf("assert fail: %s, %d\r\n", pcFile, ulLine);
}


#define MCAUSE_INT          	0x80000000UL
#define MCAUSE_CAUSE        	0x7FFFFFFFUL
#define IRQ_M_SOFT          	3
#define IRQ_M_TIMER         	7

static inline unsigned char mtimer_is_irq_enable(void)
{
	uint32_t mie = 0;
	__asm volatile ("csrr %0, mie" :"=r"(mie));
	return (mie & BIT(7)) ? 1:0;
}


_attribute_data_retention_sec_ volatile unsigned int  g_plic_switch_sp_flag=0;

_attribute_data_retention_sec_ static __attribute__ ((aligned(16))) unsigned long plicISRStack[ TLKOS_CFG_PLIC_STACK_SIZE_WORD ] = { 0 };
_attribute_data_retention_sec_ unsigned long tlk_plicISRStackTop = ( unsigned long ) &( plicISRStack[ TLKOS_CFG_PLIC_STACK_SIZE_WORD & ~0x000f ] );
extern void vPortRestoreActiveTask();


#define SYSTICK_TO_OSTICK		       (SYSTEM_TIMER_TICK_1S / configTICK_RATE_HZ)

#define OS_TICK_TO_SYSTIMER_TICK(x)    ((x) * (SYSTEM_TIMER_TICK_1S /configTICK_RATE_HZ))
#define SYSTIMER_TICK_TO_OS_TICK(x)    ((x) / (SYSTEM_TIMER_TICK_1S /configTICK_RATE_HZ))


PRIVILEGED_DATA static uint32_t tickBeforeSleep;
RAM_CODE
void vPortRestoreTick(void)
{
#if ( configUSE_TICKLESS_IDLE != 0 )
	unsigned int t = (unsigned int)(stimer_get_tick() - tickBeforeSleep);
	if(t < ((unsigned int)0xffffffff) / 2){
		vTaskStepTick((t + SYSTICK_TO_OSTICK/2) / SYSTICK_TO_OSTICK);
	}
#endif	
}

__attribute__(( weak )) void vPortWakeupNotify()
{

}

_attribute_os_core_code_ram_sec_
void vPortSuppressTicksAndSleep_i(uint32_t xExpectedIdleTime)
{
	( void ) xExpectedIdleTime;
	( void ) tickBeforeSleep;
#if ( configUSE_TICKLESS_IDLE != 0 )
    //DBG_CHN5_LOW;

    uint32_t stimer_tick = tickBeforeSleep = stimer_get_tick();

    if(xExpectedIdleTime > 10 * 1000){
        xExpectedIdleTime = 10 * 1000;
    }

    uint32_t os_wakeup_tick = (stimer_tick + OS_TICK_TO_SYSTIMER_TICK(xExpectedIdleTime))|1;

    /* If a context switch is pending then abandon the low power entry as the
    context switch might have been pended by an external interrupt that requires
    processing. */
    eSleepModeStatus eSleepAction = eTaskConfirmSleepModeStatus();
    if( eSleepAction == eAbortSleep )
    {
        //TODO
    }
    else
    {
        configPRE_SLEEP_PROCESSING( xExpectedIdleTime );

        uint32_t sleep_true = false;
        tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_FLASH_WRITE_OR_TICKLESS,1);
        if (tlksys_pm_isAllowedEnterSuspend())
        {
            sleep_true = !tlksdk_pm_enterSleep(SUSPEND_MODE, os_wakeup_tick); // 0: sleep success, need plus '!'
        }

        if (sleep_true) //SUCCESS
        {
            /* Restore OS count */
            uint32_t t = (uint32_t)(stimer_get_tick() - stimer_tick);
            vTaskStepTick(SYSTIMER_TICK_TO_OS_TICK(t));
            /* Allow the application to define some post sleep processing.  This is
            the standard configPOST_SLEEP_PROCESSING() macro, as described on the
            FreeRTOS.org website. */
            configPOST_SLEEP_PROCESSING( SYSTIMER_TICK_TO_OS_TICK(t));
        }else{
            extern void vApplicationIdleHook(void);
            vApplicationIdleHook();
        }
        tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_FLASH_WRITE_OR_TICKLESS,0);
    }
    //DBG_CHN5_HIGH;
#endif
}

void vPortRestoreTask()
{
    /*If you wake up from deepretion mode, you don't need to create task again.
     *Only enable interrupt and resume general registers need to be opened */
#if (TLK_CFG_SUSPEND_ENABLE)
    __asm volatile( "csrci   mstatus,8");

    vPortSetupTimerInterrupt();     //  reset the timer compare register to prevent irq triggered immediately

    /*enable mtime interrupt and software interrupt */
    core_mie_enable(FLD_MIE_MTIE | FLD_MIE_MSIE);

    /* plic_sw interrupt enable */
    plic_sw_interrupt_enable();

    // to reset IDLE task stack
    vPortRestoreActiveTask();
#endif
}

/**
 * @brief        vPreSleepProcessing
 * @param[in]    uxExpectedIdleTime
 * @return      none
 */
__attribute__((weak)) _attribute_os_core_code_ram_sec_
void vPreSleepProcessing(unsigned long uxExpectedIdleTime)
{
    /* Avoid compiler warnings about the unused parameter. */
    (void)uxExpectedIdleTime;
}

/**
 * @brief        vPostSleepProcessing
 * @param[in]    uxExpectedIdleTime
 * @return      none
 */
__attribute__((weak)) _attribute_os_core_code_ram_sec_
void vPostSleepProcessing(unsigned long uxExpectedIdleTime)
{
    (void) uxExpectedIdleTime;
}

/**
 * @brief        vApplicationIdleHook
 * @param[in]    none
 * @return      none
 */
//#include "stack/debug/debug_internal.h"
_attribute_os_core_code_ram_sec_
void vApplicationIdleHook(void)
{
    //DBG_COMMON_CHN3_TOGGLE;
#if (MCU_CORE_TYPE != MCU_CORE_B92) //Note: B92 WFI bug, when enter WFI, DMA clock stop
    core_entry_wfi_mode();
#endif
    //DBG_COMMON_CHN3_TOGGLE;
}


#if TLKOS_CFG_DEBUG_CPU_USAGE
static void mtime_getCpuUsage(void)
{
	static uint32_t mtime_cnt = 0;
	void tlkos_debug_setIdleTimeUs(uint8_t isEnter,uint32_t totalTimeMs);
	if(++mtime_cnt == 1000){
		tlkos_debug_setIdleTimeUs(0,mtime_cnt);
		mtime_cnt = 0;
	}
}
#endif

extern void vPortTimerInterrupt(void);

_attribute_os_core_code_ram_sec_
void mtime_handler(void)
{
	vPortTimerInterrupt();
	if(!mtimer_is_irq_enable()){
		return;
	}
	BaseType_t isNeedSwitchTask = xTaskIncrementTick();
	if(reg_irq_threshold == 0 && isNeedSwitchTask){
		vTaskSwitchContext();
	}	
	#if TLKOS_CFG_DEBUG_CPU_USAGE
	mtime_getCpuUsage();
	#endif
}

_attribute_os_core_code_ram_sec_
void mswi_handler(void)
{
	if(reg_irq_threshold != 0){
		return;//flash writing,can't switch task
	}
	vTaskSwitchContext();
}


#if (TLKOS_CFG_DEBUG_ENABLE)
volatile uint32_t AAA_OS_mcause  = 0;
volatile uint32_t AAA_OS_mdcause = 0;
volatile uint32_t AAA_OS_mepc    = 0;
volatile uint32_t AAA_OS_mtval   = 0;
volatile uint32_t AAA_OS_mstatus = 0;

#endif
_attribute_os_core_code_ram_sec_ __attribute__((weak)) 
void except_handler()
{
#if TLKOS_CFG_DEBUG_ENABLE
	AAA_OS_mcause  = read_csr(NDS_MCAUSE);
	AAA_OS_mepc    = read_csr(NDS_MEPC);
	AAA_OS_mdcause = read_csr(NDS_MDCAUSE);
	AAA_OS_mtval   = read_csr(NDS_MTVAL);
	AAA_OS_mstatus = read_csr(NDS_MSTATUS);

	TlkOsCrashInfo_t info = {
		.reason = TLKOS_CRASH_REASON_TRAP_EXCEPTION,
		.detailInfo = "EnterTrap",
	};
	tlkos_crash(&info);
#endif
	while(1);
}

_attribute_os_core_code_ram_sec_
void xPortIrqHandler(uint32_t mcause, uint32_t mepc)
{
	(void) mepc;
	if(mcause == (MCAUSE_INT + IRQ_M_TIMER)){
		tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_MTI,1);
		mtime_handler();
		tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_MTI,0);
    }else if(mcause == (MCAUSE_INT + IRQ_M_SOFT)){
		tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_MSI,1);
		plic_sw_interrupt_claim();
        mswi_handler();
        plic_sw_interrupt_complete();
		tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_MSI,0);
    }else{
		tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_EXCEPT,1);
		except_handler();
		tlkos_debug_ioCtrl(TLKOS_DEBUG_IO_EXCEPT,1);
    }
}

#endif


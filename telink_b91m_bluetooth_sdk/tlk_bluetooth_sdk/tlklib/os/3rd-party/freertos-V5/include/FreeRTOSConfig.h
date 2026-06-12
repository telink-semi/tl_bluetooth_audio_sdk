/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "tlklib/os/tlkos_config.h"
/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

#define configCLINT_BASE_ADDRESS        0x00000000

#define configCPU_CLOCK_HZ						TLKOS_CFG_HEART_TIMER_TICK_HZ
#define configUSE_TICKLESS_IDLE                 TLKOS_CFG_TICKLESS_ENABLE
#define configTICK_RATE_HZ                      TLKOS_CFG_OS_TICK_HZ
#define configUSE_PREEMPTION                    1
#define configMAX_PRIORITIES                    ( 12 )
#define configMINIMAL_STACK_SIZE                ( 256 )
#define configMAX_TASK_NAME_LEN                 ( 8 )
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 0
#define configUSE_TASK_NOTIFICATIONS            0
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               0
#define configUSE_QUEUE_SETS                    0

/* Memory allocation definitions. */
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1

#define configTOTAL_HEAP_SIZE                   TLKOS_CFG_OS_HEAP_SIZE

/* Hook function definitions. */
#define configUSE_IDLE_HOOK                     (1 && !(TLKOS_CFG_TICKLESS_ENABLE))
#define configUSE_TICK_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW          0

#if TLKOS_CFG_DEBUG_MALLOC_FAIL
#define configUSE_MALLOC_FAILED_HOOK            1
#else
#define configUSE_MALLOC_FAILED_HOOK            0
#endif
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering definitions. */
#define configUSE_APPLICATION_TASK_TAG          0
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1      
#define configUSE_STATS_FORMATTING_FUNCTIONS    0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                15
#define configTIMER_TASK_STACK_DEPTH            ( 128 * 3 )

#define configISR_STACK_SIZE_WORDS              512

#if TLKOS_CFG_TICKLESS_ENABLE
#define configIDLE_TASK_STACK_DEPTH             (512) //WORD: 2K byte
#else
#define configIDLE_TASK_STACK_DEPTH             configMINIMAL_STACK_SIZE
#endif

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 0
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  1
#define INCLUDE_xSemaphoreGetMutexHolder        0

/* The size of the global output buffer that is available for use when there
are multiple command interpreters running at once (for example, one on a UART
and one on TCP/IP).  This is done to prevent an output buffer being defined by
each implementation - which would waste RAM.  In this case, there is only one
command interpreter running. */
/* The buffer into which output generated by FreeRTOS+CLI is placed.  This must
be at least big enough to contain the output of the task-stats command, as the
example implementation does not include buffer overlow checking. */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE       2096
#define configINCLUDE_QUERY_HEAP_COMMAND        1

/* This file is included from assembler files - make sure C code is not included
in assembler files. */
#ifndef __ASSEMBLER__
    void vAssertCalled( const char * pcFile, unsigned long ulLine );
    void vConfigureTickInterrupt( void );
    void vClearTickInterrupt( void );
    void vPreSleepProcessing( unsigned long uxExpectedIdleTime );
    void vPostSleepProcessing( unsigned long uxExpectedIdleTime );
#endif /* __ASSEMBLER__ */



/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ );



/****** Hardware/compiler specific settings. *******************************************/

/* The following constant describe the hardware */

#if MCU_CORE_TYPE  == MCU_CORE_TL751X || MCU_CORE_TYPE  == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL721X ||  MCU_CORE_TYPE == MCU_CORE_TL322X
#define configMTIME_BASE_ADDRESS	( 0xC6000000 )
#define configMTIMECMP_BASE_ADDRESS	( 0xC6000008 )
#else
#define configMTIME_BASE_ADDRESS	( 0xE6000000 )
#define configMTIMECMP_BASE_ADDRESS	( 0xE6000008 )
#endif
#if 0
/*
 * The application must provide a function that configures a peripheral to
 * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
 * in FreeRTOSConfig.h to call the function.
 */
#define configSETUP_TICK_INTERRUPT() vConfigureTickInterrupt()
#define configCLEAR_TICK_INTERRUPT() vClearTickInterrupt()
#endif

/* The configPRE_SLEEP_PROCESSING() and configPOST_SLEEP_PROCESSING() macros
allow the application writer to add additional code before and after the MCU is
placed into the low power state respectively.  The empty implementations
provided in this demo can be extended to save even more power. */
#define configPRE_SLEEP_PROCESSING( uxExpectedIdleTime ) vPreSleepProcessing( uxExpectedIdleTime );
#define configPOST_SLEEP_PROCESSING( uxExpectedIdleTime ) vPostSleepProcessing( uxExpectedIdleTime );


/* Compiler specifics. */
#define fabs( x ) __builtin_fabs( x )

/* Enable Hardware Stack Protection and Recording mechanism. */
#define configHSP_ENABLE            TLKOS_CFG_DEBUG_STACK_OVERFLOW

/*Hardware stack protection for ISR can't support statically allocated IRQ_STACK*/
#if(configHSP_ENABLE == 1 && configISR_STACK_SIZE_WORDS == 0)
#define configISR_HSP_ENABLE    1
#endif

/* Record the highest address of stack. */
#if(configHSP_ENABLE == 1 )
#ifndef configRECORD_STACK_HIGH_ADDRESS
#define configRECORD_STACK_HIGH_ADDRESS     1
#endif
#endif

#endif /* FREERTOS_CONFIG_H */


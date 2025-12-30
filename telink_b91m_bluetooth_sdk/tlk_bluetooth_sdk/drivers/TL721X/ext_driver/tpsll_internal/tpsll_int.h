/********************************************************************************************************
 * @file    tpsll_int.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef DRIVERS_TL721X_EXT_DRIVER_EXT_INT_H_
#define DRIVERS_TL721X_EXT_DRIVER_EXT_INT_H_
#include "common/types.h"
#include "drivers.h"


#define GLOBAL_INT_DISABLE() u32 rie = core_interrupt_disable()

#define GLOBAL_INT_RESTORE() core_restore_interrupt(rie)


/* Machine mode MCAUSE */
#define TRAP_0_INSTRUCTION_ADDR_MISALIGNED  0
#define TRAP_1_INSTRUCTION_ADDR_FAULT       1
#define TRAP_2_ILLEGAL_INSTRUCTION          2
#define TRAP_3_BREAKPOINT                   3
#define TRAP_4_LOAD_ADDR_MISALIGNED         4
#define TRAP_5_LOAD_ACCESS_FAULT            5
#define TRAP_6_STORE_OR_AMO_ADDR_MISALIGNED 6
#define TRAP_7_STORE_OR_AMO_ACC_FAULT       7
#define TRAP_8_ENV_CALL_FROM_U_MODE         8
#define TRAP_11_ENV_CALL_FROM_M_MODE        11
#define TRAP_32_STACK_OVERFLOW_EXCEPTION    32
#define TRAP_33_STACK_UNDERFLOW_EXCEPTION   33
#define TRAP_40_ANDES_CUSTOM_EXT            40
#define TRAP_41_ANDES_CUSTOM_EXT            41
#define TRAP_42_ANDES_CUSTOM_EXT            42
#define TRAP_43_ANDES_CUSTOM_EXT            43
#define TRAP_44_ANDES_CUSTOM_EXT            44
#define TRAP_45_ANDES_CUSTOM_EXT            45
#define TRAP_46_ANDES_CUSTOM_EXT            46
#define TRAP_47_ANDES_CUSTOM_EXT            47

typedef struct EXCEPT_HANDLER_STATUS
{
    uint32_t pc;
    uint32_t lr;
    uint32_t sp;
    uint32_t gp;
    uint32_t cause;
    uint32_t irq_src;
} except_handler_status_t;

extern volatile except_handler_status_t g_except_handler_status;

typedef void (*trap_func_t)(u8 flag);

extern trap_func_t plic_trap_proc;

static inline void plic_trap_func_register(trap_func_t func)
{
    plic_trap_proc = func;
}

#endif /* DRIVERS_TL721X_EXT_DRIVER_EXT_INT_H_ */

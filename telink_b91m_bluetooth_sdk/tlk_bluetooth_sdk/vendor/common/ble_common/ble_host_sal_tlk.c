/********************************************************************************************************
 * @file    ble_host_sal_tlk.c
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
#include "stack/ble/ble.h"
#include "common/types.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "tlkmw/btble/tlkmdi_btble_btsnoop.h"
#include "tlklib/dbg/tlkdbg.h"
#include "vendor/common/ble_common/tlkble_hal_mode.h"
#include "tlkalg/crypt/ecc/tlkalg_ecc_ll.h"

extern void  tlk_malloc_init(void *base, size_t size);
extern void *tlk_malloc_buffer(void *base, size_t size, unsigned short usedType);
extern void  tlk_free_buffer(void *base, void *ptr);

/** ble host sal  */

/*************** list, queue, stack, or other data structure ***********/
// ble all data structure use sys/queue.h defined in kernel.
// #include <sys/queue.h>

/**************** memory pool(malloc/free) sal interface ***********/
#ifndef BLE_HOST_MEMORY_POOL_SIZE
#define BLE_HOST_MEMORY_POOL_SIZE (10 * 1024)
#endif

#ifndef BLE_HOST_MEMORY_POOL_RAM_SECTION
#if MCU_CORE_TL752X_TEMP
#define BLE_HOST_MEMORY_POOL_RAM_SECTION _attribute_iram_data_
#else
#define BLE_HOST_MEMORY_POOL_RAM_SECTION
#endif
#endif

BLE_HOST_MEMORY_POOL_RAM_SECTION
static uint8_t s_ble_host_memory[BLE_HOST_MEMORY_POOL_SIZE];

/**
 * @brief BLE host memory pool initialization.
 *        Setup heap used by BLE host components.
 */
void ble_host_sal_memory_pool_init(void)
{
    // memset(s_ble_host_memory,0,BLE_HOST_MEMORY_POOL_SIZE);
    tlk_malloc_init(s_ble_host_memory, BLE_HOST_MEMORY_POOL_SIZE);
}

/**
 * @brief BLE host memory malloc.
 *
 * @param[in] size     Allocation size.
 * @param[in] type_id  High layer type id (range 256-65535).
 *
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *ble_host_sal_memory_malloc(uint32_t size, uint16_t type_id)
{
    ble_host_sal_os_enter_critical_section();
    void *ret = tlk_malloc_buffer(s_ble_host_memory, size, type_id);
    ble_host_sal_os_exit_critical_section();
    return ret;
}

/**
 * @brief BLE host memory free.
 *
 * @param[in] ptr  Pointer previously returned by ble_host_sal_memory_malloc.
 */
void ble_host_sal_memory_free(void *ptr)
{
    ble_host_sal_os_enter_critical_section();
    tlk_free_buffer(s_ble_host_memory, ptr);
    ble_host_sal_os_exit_critical_section();
}

/****************** log sal interface ****************/
/**
 * @brief BLE host log initialization.
 *
 * @param[in] level  Initial log level.
 */
void ble_host_sal_log_init(enum ble_host_log_level level)
{
    (void)level;
}

#ifndef PROJ_RAZER_GAME_CONTROLLER
static uint32_t s_log_level = BLE_HOST_LOG_LEVEL_WARN;
#endif

/**
 * @brief BLE host log output.
 *
 * @param[in] level  Log level.
 * @param[in] fmt    Log format string.
 */
void ble_host_sal_log_output(enum ble_host_log_level level, const char *fmt, ...)
{
#ifndef PROJ_RAZER_GAME_CONTROLLER
    if (level <= s_log_level) {
        char    log_out_buffer[128];
        va_list args;
        va_start(args, fmt);
        int length = vsnprintf(log_out_buffer, sizeof(log_out_buffer) - 1, fmt, args);
        tlk_printf(log_out_buffer);
        va_end(args);

        switch (level) {
        case BLE_HOST_LOG_LEVEL_WARN:
            bluetooth_host_snoop_write_log_packet(BT_SNOOP_MSG_WAR, (const uint8_t *)log_out_buffer, length);
            break;
        case BLE_HOST_LOG_LEVEL_ERROR:
            bluetooth_host_snoop_write_log_packet(BT_SNOOP_MSG_ERR, (const uint8_t *)log_out_buffer, length);
            break;
        default:
            bluetooth_host_snoop_write_log_packet(BT_SNOOP_MSG_LOG, (const uint8_t *)log_out_buffer, length);
            break;
        }
    }
#else
    (void)level;
    (void)fmt;
#endif
}

/**
 * @brief BLE host log set output level.
 *
 * @param[in] level  Log level to output.
 */
void ble_host_sal_log_set_output_level(enum ble_host_log_level level)
{
    (void)level;
}

/****************** hci sal interface ****************/
/**
 * @brief BLE host HCI initialization hook.
 */
void ble_host_sal_hci_init(void) {}

/********************* RTOS or mainloop register interface ****************/

/********************* Host Encryption Module ****************/

/**
 * @brief Initialize BLE host P-256 ECC module.
 *        Prepare ECC engine for key generation and DH key computation.
 */
void ble_host_p256_initial(void)
{
    tlkalg_ecc_init();
}

/**
 * @brief Generate local P-256 key pair and report public key to host stack.
 *        The generated private/public key pair is passed to the SMP handler.
 */
void ble_host_p256_read_local_public_key(void)
{
    uint8_t s_public_key[64];
    uint8_t s_private_key[32];
    tlkalg_ecc_gen_key_pair(s_public_key, s_private_key, ECC_use_secp256r1, false);
    ble_host_p256_read_local_public_key_complete(true, s_private_key, s_public_key);
}

/**
 * @brief Generate local P-256 Diffie-Hellman key.
 *        Use local private key and remote public key to compute shared secret.
 *
 * @param[in] local_private_key  Local private key (32 bytes).
 * @param[in] remote_public_key  Remote public key (64 bytes, X||Y).
 */
void ble_host_p256_generate_local_dhkey(const uint8_t local_private_key[32], const uint8_t remote_public_key[64])
{
    uint8_t dhkey[32];
    tlkalg_ecc_gen_dhkey(remote_public_key, local_private_key, dhkey, ECC_use_secp256r1);
    ble_host_p256_generate_local_dhkey_complete(true, dhkey);
}

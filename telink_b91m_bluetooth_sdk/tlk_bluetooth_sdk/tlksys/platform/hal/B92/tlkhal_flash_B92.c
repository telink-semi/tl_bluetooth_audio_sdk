/********************************************************************************************************
 * @file    tlkhal_flash_B92.c
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#if MCU_CORE_TYPE == MCU_CORE_B92

typedef unsigned char (*flash_write_status_fp)(unsigned short, unsigned int);
typedef unsigned char (*flash_lock_fp)(unsigned int);
typedef unsigned char (*flash_unlock_fp)(void);
typedef unsigned int (*flash_get_lock_block_fp)(void);

typedef struct
{
    unsigned int mid;
    uint32_t     size;
    //flash_protect
    flash_get_lock_block_fp get_lock;
    flash_unlock_fp         unlock;
    flash_lock_fp           lock_func;
    unsigned int            lock_size[TLKHAL_FLASH_LOCK_ENUM_NUM];
    //flash qe enable(4line)
    flash_write_status_fp flash_write_status;
    unsigned int          flash_qe_mask;
    unsigned short        qe_en;
    unsigned short        qe_dis;
} tlkhal_flash_handler_t;

_attribute_data_retention_ uint8_t tlkhal_flash_mid_index = 0;

static uint8_t s_tlkhal_flash_lock_en = 0;

static const tlkhal_flash_handler_t sc_tlkhal_flash_handlers[6] = {
    //1M
    [0] =
        {
            .mid       = 0x146085,
            .size      = 0x100000,
            .get_lock  = flash_get_lock_block_mid146085,
            .unlock    = flash_unlock_mid146085,
            .lock_func = flash_lock_mid146085,
            .lock_size =
                {
                    [TLKHAL_FLASH_LOCK_ALL]              = FLASH_LOCK_ALL_1M_MID146085,
                    [TLKHAL_FLASH_LOCK_BOOT_AREA]        = FLASH_LOCK_LOW_64K_MID146085,
                    [TLKHAL_FLASH_LOCK_EXCEPT_HIGH_128K] = FLASH_LOCK_LOW_896K_MID146085,
                },
            .flash_write_status = flash_write_status_mid146085,
            .flash_qe_mask      = FLASH_WRITE_STATUS_QE_MID146085,
            .qe_en              = FLASH_QE_ENABLE_MID146085,
            .qe_dis             = FLASH_QE_DISABLE_MID146085,
        },

    [1] =
        {
            .mid       = 0x1460c8,
            .size      = 0x100000,
            .get_lock  = flash_get_lock_block_mid1460c8,
            .unlock    = flash_unlock_mid1460c8,
            .lock_func = flash_lock_mid1460c8,
            .lock_size =
                {
                    [TLKHAL_FLASH_LOCK_ALL]              = FLASH_LOCK_ALL_1M_MID1460C8,
                    [TLKHAL_FLASH_LOCK_BOOT_AREA]        = FLASH_LOCK_LOW_64K_MID1460C8,
                    [TLKHAL_FLASH_LOCK_EXCEPT_HIGH_128K] = FLASH_LOCK_LOW_896K_MID1460C8,
                },
            .flash_write_status = flash_write_status_mid1460c8,
            .flash_qe_mask      = FLASH_WRITE_STATUS_QE_MID1460C8,
            .qe_en              = FLASH_QE_ENABLE_MID1460C8,
            .qe_dis             = FLASH_QE_DISABLE_MID1460C8,
        },
    //2M
    [2] =
        {
            .mid       = 0x156085,
            .size      = 0x200000,
            .get_lock  = flash_get_lock_block_mid156085,
            .unlock    = flash_unlock_mid156085,
            .lock_func = flash_lock_mid156085,
            .lock_size =
                {
                    [TLKHAL_FLASH_LOCK_ALL]              = FLASH_LOCK_ALL_2M_MID156085,
                    [TLKHAL_FLASH_LOCK_BOOT_AREA]        = FLASH_LOCK_LOW_64K_MID156085,
                    [TLKHAL_FLASH_LOCK_EXCEPT_HIGH_128K] = FLASH_LOCK_LOW_1920K_MID156085,
                },
            .flash_write_status = flash_write_status_mid156085,
            .flash_qe_mask      = FLASH_WRITE_STATUS_QE_MID156085,
            .qe_en              = FLASH_QE_ENABLE_MID156085,
            .qe_dis             = FLASH_QE_DISABLE_MID156085,
        },

    [3] =
        {
            .mid       = 0x1560c8,
            .size      = 0x200000,
            .get_lock  = flash_get_lock_block_mid1560c8,
            .unlock    = flash_unlock_mid1560c8,
            .lock_func = flash_lock_mid1560c8,
            .lock_size =
                {
                    [TLKHAL_FLASH_LOCK_ALL]              = FLASH_LOCK_ALL_2M_MID1560C8,
                    [TLKHAL_FLASH_LOCK_BOOT_AREA]        = FLASH_LOCK_LOW_64K_MID1560C8,
                    [TLKHAL_FLASH_LOCK_EXCEPT_HIGH_128K] = FLASH_LOCK_LOW_1920K_MID1560C8,
                },
            .flash_write_status = flash_write_status_mid1560c8,
            .flash_qe_mask      = FLASH_WRITE_STATUS_QE_MID1560C8,
            .qe_en              = FLASH_QE_ENABLE_MID1560C8,
            .qe_dis             = FLASH_QE_DISABLE_MID1560C8,
        },

    //4M
    [4] =
        {
            .mid       = 0x166085,
            .size      = 0x400000,
            .get_lock  = flash_get_lock_block_mid166085,
            .unlock    = flash_unlock_mid166085,
            .lock_func = flash_lock_mid166085,
            .lock_size =
                {
                    [TLKHAL_FLASH_LOCK_ALL]              = FLASH_LOCK_ALL_4M_MID166085,
                    [TLKHAL_FLASH_LOCK_BOOT_AREA]        = FLASH_LOCK_LOW_64K_MID166085,
                    [TLKHAL_FLASH_LOCK_EXCEPT_HIGH_128K] = FLASH_LOCK_LOW_3968K_MID166085,
                },
            .flash_write_status = flash_write_status_mid166085,
            .flash_qe_mask      = FLASH_WRITE_STATUS_QE_MID166085,
            .qe_en              = FLASH_QE_ENABLE_MID166085,
            .qe_dis             = FLASH_QE_DISABLE_MID166085,
        },

    [5] =
        {
            .mid       = 0x1660c8,
            .size      = 0x400000,
            .get_lock  = flash_get_lock_block_mid1660c8,
            .unlock    = flash_unlock_mid1660c8,
            .lock_func = flash_lock_mid1660c8,
            .lock_size =
                {
                    [TLKHAL_FLASH_LOCK_ALL]              = FLASH_LOCK_ALL_4M_MID1660C8,
                    [TLKHAL_FLASH_LOCK_BOOT_AREA]        = FLASH_LOCK_LOW_64K_MID1660C8,
                    [TLKHAL_FLASH_LOCK_EXCEPT_HIGH_128K] = FLASH_LOCK_LOW_3968K_MID1660C8,
                },
            .flash_write_status = flash_write_status_mid1660c8,
            .flash_qe_mask      = FLASH_WRITE_STATUS_QE_MID1660C8,
            .qe_en              = FLASH_QE_ENABLE_MID1660C8,
            .qe_dis             = FLASH_QE_DISABLE_MID1660C8,
        },
};

void tlkhal_flash_init(uint8_t enable_lock)
{
    unsigned int mid = flash_read_mid();
    for (size_t i = 0; i < sizeof(sc_tlkhal_flash_handlers) / sizeof(tlkhal_flash_handler_t); i++) {
        if (mid == sc_tlkhal_flash_handlers[i].mid) {
            tlkhal_flash_mid_index = i;
            break;
        }
    }
    s_tlkhal_flash_lock_en = enable_lock;
    tlkhal_flash_unlock();
    tlkhal_flash_lock_ex(TLKHAL_FLASH_LOCK_EXCEPT_HIGH_128K);
}

uint32_t tlkhal_flash_get_size(void)
{
    return sc_tlkhal_flash_handlers[tlkhal_flash_mid_index].size;
}

void tlkhal_flash_lock_ex(tlkhal_flash_lock_size_e size)
{
    if (s_tlkhal_flash_lock_en == 0) {
        return;
    }
    uint8_t index = tlkhal_flash_mid_index;
    if (0 == sc_tlkhal_flash_handlers[index].get_lock()) {
        sc_tlkhal_flash_handlers[index].lock_func(sc_tlkhal_flash_handlers[index].lock_size[size]);
    }
}

void tlkhal_flash_lock(void)
{
    tlkhal_flash_lock_ex(TLKHAL_FLASH_LOCK_ALL);
}

void tlkhal_flash_unlock(void)
{
    if (s_tlkhal_flash_lock_en == 0) {
        return;
    }
    uint8_t index = tlkhal_flash_mid_index;
    if (0 != sc_tlkhal_flash_handlers[index].get_lock()) {
        sc_tlkhal_flash_handlers[index].unlock();
    }
}

void tlkhal_flash_4line_enable(void)
{
    uint8_t       index  = tlkhal_flash_mid_index;
    unsigned char status = sc_tlkhal_flash_handlers[index].flash_write_status(sc_tlkhal_flash_handlers[index].qe_en, sc_tlkhal_flash_handlers[index].flash_qe_mask);
    if (1 == status) {
        flash_read_page = flash_4read;
        flash_set_xip_config(FLASH_X4READ_CMD);
        flash_write_page = flash_quad_page_program;
    }
}

#endif

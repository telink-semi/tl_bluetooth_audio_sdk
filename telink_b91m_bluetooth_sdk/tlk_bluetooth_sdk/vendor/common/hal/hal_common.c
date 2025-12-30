/********************************************************************************************************
 * @file    hal_common.c
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
#include "core/mcu_type.h"
#include "hal_flash_prot.h"
#include "ext_driver/ext_flash.h"


/*
 * @note The flash protection size has been allocated by default, refer to the comment FLASH_PROTECT_MODIFY_CONFIG for details.
 */
flash_user_defined_list_t flash_init_list[] = {
#if (MCU_CORE_TYPE == MCU_CORE_B91)
    //1M
    {0x146085, FLASH_LOCK_LOW_512K_MID146085},
    //2M
    {0x156085, FLASH_LOCK_LOW_1M_MID156085},
    //4M
    {0x166085, FLASH_LOCK_LOW_2M_MID166085},
    //16M
    {0x182085, FLASH_LOCK_LOW_8M_MID182085}

#elif (MCU_CORE_TYPE == MCU_CORE_B92)
    //1M
    {0x146085, FLASH_LOCK_LOW_512K_MID146085},
    {0x1460c8, FLASH_LOCK_LOW_512K_MID1460C8},
    //2M
    {0x156085, FLASH_LOCK_LOW_512K_MID156085},
    {0x1560c8, FLASH_LOCK_LOW_512K_MID1560C8},
    //4M
    {0x166085, FLASH_LOCK_LOW_2M_MID166085},
    {0x1660c8, FLASH_LOCK_LOW_2M_MID1660C8},
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X)
    //1M
    {0x146085, FLASH_LOCK_LOW_512K_MID146085},
    //4M
    {0x166085, FLASH_LOCK_LOW_512K_MID166085},
    //8M
    {0x176085, FLASH_LOCK_LOW_4M_MID176085},
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
    //2M
    {0x156085, FLASH_LOCK_LOW_1M_MID156085},

#elif (MCU_CORE_TL752X_TEMP)
    //2M TODO: Platform Group
    {0x156085, 0x0034},
#else
    {0, 0}
#endif
};
#if FLASH_PROTECT_MODIFY_CONFIG
#define list_fp flash_init_list
#else
#define list_fp NULL
#endif


#if (MCU_CORE_TYPE == MCU_CORE_B91 || MCU_CORE_TYPE == MCU_CORE_B92 || MCU_CORE_TYPE == MCU_CORE_TL321X)

flash_hal_user_handler_t flash_handler = {
    .list      = list_fp,
    .flash_cnt = (sizeof(flash_init_list) / sizeof(flash_user_defined_list_t)),
};

#elif (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL721X)
flash_hal_user_handler_t flash_handler[SLAVE_CNT] = {
    {
        .list       = list_fp,
        .flash_cnt  = (sizeof(flash_init_list) / sizeof(flash_user_defined_list_t)),
        .slave_size = SLAVE_SIZE_64M,
    },
    {
        .list       = list_fp,
        .flash_cnt  = (sizeof(flash_init_list) / sizeof(flash_user_defined_list_t)),
        .slave_size = SLAVE_SIZE_NONE,
    },
    {
        .list       = list_fp,
        .flash_cnt  = (sizeof(flash_init_list) / sizeof(flash_user_defined_list_t)),
        .slave_size = SLAVE_SIZE_NONE,
    },
    {
        .list       = list_fp,
        .flash_cnt  = (sizeof(flash_init_list) / sizeof(flash_user_defined_list_t)),
        .slave_size = SLAVE_SIZE_NONE,
    },
};
#endif

void tlkapp_flash_enable_4line(unsigned char en)
{
    if (en) {
        ext_flash_4line_enable();
    }
}

void tlkapp_flash_prot_init(unsigned char flash_protect_en)
{
/**
    ===============================================================================
                        ##### driver sdk firmware protection #####
    ===============================================================================
    Flash write protection must be added, the size of the protected area is decided according to the application bin file,
    the principle is as follows:
    1.The program area is protected (to prevent the program area from being erased by mistake);
    2.the program will modify the data area is not protected (if write-protected, each time before erasing the need to unprotect,
      so that there is a risk that the status register of the flash has been mistakenly rewritten);

    @note if flash protection fails, LED1 lights up long, and keeps while.
    ===============================================================================
*/
// #if (!defined(DUT_TEST) && defined(MCU_STARTUP_FLASH))
#if (defined(MCU_STARTUP_FLASH))
#if (MCU_CORE_TYPE == MCU_CORE_B91 || MCU_CORE_TYPE == MCU_CORE_B92 || MCU_CORE_TYPE == MCU_CORE_TL321X)
    unsigned char flash_init_flag = hal_flash_init(&flash_handler);
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL751X)
    unsigned char flash_init_flag = hal_flash_init((flash_hal_user_handler_t *)flash_handler);
#else
    unsigned char flash_init_flag = 0;
#endif
    if (flash_init_flag != 0) {
        // gpio_set_high_level(LED1);
        while (1);
    }
    if (flash_protect_en) {
#if (MCU_CORE_TYPE == MCU_CORE_B91 || MCU_CORE_TYPE == MCU_CORE_B92 || MCU_CORE_TYPE == MCU_CORE_TL321X)
        unsigned char lock_flag = hal_flash_lock();
#elif (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X)
        unsigned char lock_flag = hal_flash_lock_with_device_num(SLAVE0);
#else
        unsigned char lock_flag = 0;
#endif
        if (!(lock_flag == 1)) {
            // gpio_set_high_level(LED1);
            while (1);
        }
    }
#else
    (void)flash_protect_en;
#endif
}

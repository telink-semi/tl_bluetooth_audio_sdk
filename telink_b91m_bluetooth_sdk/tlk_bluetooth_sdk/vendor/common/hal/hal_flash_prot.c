/********************************************************************************************************
 * @file    hal_flash_prot.c
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
#include "hal_flash_prot.h"
#include "tl_common.h"
#include "drivers.h"

#if (MCU_CORE_TYPE == MCU_CORE_B91 || MCU_CORE_TYPE == MCU_CORE_B92 || MCU_CORE_TYPE == MCU_CORE_TL321X)

extern const flash_hal_handler_t flash_list[];
extern const unsigned int        FLASH_CNT;
flash_hal_handler_t              g_flash_handler = {0};
/*
 * 1. Whether hal_flash_init parameter handler->list is NULL or not, as long as the default configuration of flash_list is successfully configured,
 *    g_mid_matched is true, which does not affect the use of hal_flash_4line_en/hal_flash_4line_dis function;
 * 2. hal_flash_init return value is given to the upper layer (flash_init_list/flash_list) flash mid whether the match is successful return status,
 *    the upper layer according to the actual situation to do the logical analysis and processing;
 */
bool g_mid_matched = false;

/**
 * @brief       This function is used to initializing flash configurations.
 *              it must be called before the hal_xx function can be called.
 * @param[in]   handler - provide member structures that can be modified by upper-level concerns,
 *              When the bin protection area uses the default size, then handler->list is NULL,
 *              if the customer needs to modify the size of the protection area, enable FLASH_PROTECT_MODIFY_CONFIG and modify it through flash_init_list.
 * @return      0:flash_init mid matches successfully, and handler(flash_init_list) mid matches successfully when FLASH_PROTECT_MODIFY_CONFIG is enabled;
 *              1:flash_init mid matches fail;
 *              2:handler(flash_init_list) mid matches fail;
 */
unsigned char hal_flash_init(flash_hal_user_handler_t *handler)
{
    g_flash_handler.mid = flash_read_mid();
    for (unsigned int i = 0; i < FLASH_CNT; i++) {
        if (g_flash_handler.mid == flash_list[i].mid) {
            g_mid_matched             = true;
            g_flash_handler.get_lock  = flash_list[i].get_lock;
            g_flash_handler.unlock    = flash_list[i].unlock;
            g_flash_handler.lock_func = flash_list[i].lock_func;
            g_flash_handler.lock_size = flash_list[i].lock_size;

            g_flash_handler.flash_write_status = flash_list[i].flash_write_status;
            g_flash_handler.flash_qe_mask      = flash_list[i].flash_qe_mask;
            g_flash_handler.qe_en              = flash_list[i].qe_en;
            g_flash_handler.qe_dis             = flash_list[i].qe_dis;
            break;
        }
    }
    if (g_mid_matched == false) {
        return 1;
    } else if (handler->list != NULL) {
        for (unsigned int j = 0; j < (handler->flash_cnt); j++) {
            if (g_flash_handler.mid == handler->list[j].mid) {
                g_flash_handler.lock_size = handler->list[j].block_size;
                return 0;
            }
        }
        return 2;
    } else {
        return 0;
    }
    return 0;
}

/**
 * @brief       This function is used to enable the 4line function of flash.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char hal_flash_4line_en(void)
{
    if (g_mid_matched) {
        return g_flash_handler.flash_write_status(g_flash_handler.qe_en, g_flash_handler.flash_qe_mask);
    }
    return 3;
}

/**
 * @brief       This function is used to disable the 4line function of flash.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char hal_flash_4line_dis(void)
{
    if (g_mid_matched) {
        return g_flash_handler.flash_write_status(g_flash_handler.qe_dis, g_flash_handler.flash_qe_mask);
    }
    return 3;
}

/**
 * @brief       This function is used to lock a flash through lock_fp,
 *              if the flash is not locked, it is locked, if the flash is locked, it is not executed.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_lock(void)
{
    if (g_mid_matched) {
        if (0 == g_flash_handler.get_lock()) {
            return g_flash_handler.lock_func(g_flash_handler.lock_size);
        }
        return 1;
    }
    return 3;
}

/**
 * @brief       This function is used to unlock a flash through lock_fp,
 *              if the flash is locked, it is unlocked, if the flash is unlocked, it is not executed.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_unlock(void)
{
    if (g_mid_matched) {
        if (0 != g_flash_handler.get_lock()) {
            return g_flash_handler.unlock();
        }
        return 1;
    }
    return 3;
}

#elif (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL753X)


extern const flash_hal_handler_t flash_list[];
extern const unsigned int        FLASH_CNT;

/*
 * 1. Whether hal_flash_init parameter handler->list is NULL or not, as long as the default configuration of flash_list is successfully configured,
 *    g_mid_matched[SLAVE_X] is true, which does not affect the use of hal_flash_4line_en/hal_flash_4line_dis function;
 * 2. hal_flash_init return value is given to the upper layer (flash_init_list/flash_list) flash mid whether the match is successful return status,
 *    the upper layer according to the actual situation to do the logical analysis and processing;
 */
bool                g_mid_matched[SLAVE_CNT]   = {};
flash_hal_handler_t g_flash_handler[SLAVE_CNT] = {0};

/**
 * @brief       This function is used to initializing flash configurations (Lock/unlock/4line).
 * @param[in]   handler - provide member structures that can be modified by upper-level concerns,
 *              1.When the bin protection area uses the default size, then handler->list is NULL,
 *              if the customer needs to modify the size of the protection area, enable FLASH_PROTECT_MODIFY_CONFIG and modify it through flash_init_list.
 *              2.handler->slave_size:slave_x space division, refer to interface notes(mspi_slave_device_addr_space_config).
 * @return      0:flash_init mid matches successfully, and handler(flash_init_list) mid matches successfully when FLASH_PROTECT_MODIFY_CONFIG is enabled;
 *              return (slave_id*3)+1:slave_id flash_init mid matches fail;
 *              (slave_id*3)+2:slave_id handler(flash_init_list) mid matches fail;
 */
unsigned char hal_flash_init(flash_hal_user_handler_t *handler)
{
    if (mspi_slave_device_addr_space_config(handler[SLAVE0].slave_size, handler[SLAVE1].slave_size, handler[SLAVE2].slave_size, handler[SLAVE3].slave_size)) {
        for (unsigned char slave_id = 0; slave_id < SLAVE_CNT; slave_id++) {
            if (handler[slave_id].slave_size != SLAVE_SIZE_NONE) {
                g_flash_handler[slave_id].mid = flash_read_mid_with_device_num(slave_id);
                for (unsigned int i = 0; i < FLASH_CNT; i++) {
                    if (g_flash_handler[slave_id].mid == flash_list[i].mid) {
                        g_flash_handler[slave_id].get_lock  = flash_list[i].get_lock;
                        g_flash_handler[slave_id].unlock    = flash_list[i].unlock;
                        g_flash_handler[slave_id].lock_func = flash_list[i].lock_func;
                        g_flash_handler[slave_id].lock_size = flash_list[i].lock_size;

                        g_flash_handler[slave_id].flash_write_status = flash_list[i].flash_write_status;
                        g_flash_handler[slave_id].flash_qe_mask      = flash_list[i].flash_qe_mask;
                        g_flash_handler[slave_id].qe_en              = flash_list[i].qe_en;
                        g_flash_handler[slave_id].qe_dis             = flash_list[i].qe_dis;
                        g_mid_matched[slave_id]                      = true;
                        if (handler[slave_id].list != NULL) {
                            unsigned int j = 0;
                            for (j = 0; j < (handler[slave_id].flash_cnt); j++) {
                                if (g_flash_handler[slave_id].mid == handler[slave_id].list[j].mid) {
                                    g_flash_handler[slave_id].lock_size = handler[slave_id].list[j].block_size;
                                    break;
                                }
                            }
                            if (j == handler[slave_id].flash_cnt) {
                                return (slave_id * 3) + 2;
                            }
                        }
                        break;
                    }
                }
                if (g_mid_matched[slave_id] == false) {
                    return (slave_id * 3) + 1;
                }
            }
        }
    }
    return 0;
}

/**
 * @brief       This function is used to enable the 4line function of flash.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char hal_flash_4line_en_with_device_num(mspi_slave_device_num_e device_num)
{
    if (g_mid_matched[device_num]) {
        return g_flash_handler[device_num].flash_write_status(device_num, g_flash_handler[device_num].qe_en, g_flash_handler[device_num].flash_qe_mask);
    }
    return 3;
}

/**
 * @brief       This function is used to disable the 4line function of flash.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char hal_flash_4line_dis_with_device_num(mspi_slave_device_num_e device_num)
{
    if (g_mid_matched[device_num]) {
        return g_flash_handler[device_num].flash_write_status(device_num, g_flash_handler[device_num].qe_dis, g_flash_handler[device_num].flash_qe_mask);
    }
    return 3;
}

/**
 * @brief       This function is used to lock a flash through lock_fp,
 *              if the flash is not locked, it is locked, if the flash is locked, it is not executed.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_lock_with_device_num(mspi_slave_device_num_e device_num)
{
    if (g_mid_matched[device_num]) {
        if (0 == g_flash_handler[device_num].get_lock(device_num)) {
            return g_flash_handler[device_num].lock_func(device_num, g_flash_handler[device_num].lock_size);
        }
        return 1;
    }
    return 3;
}

/**
 * @brief       This function is used to unlock a flash through lock_fp,
 *              if the flash is locked, it is unlocked, if the flash is unlocked, it is not executed.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_unlock_with_device_num(mspi_slave_device_num_e device_num)
{
    if (g_mid_matched[device_num]) {
        if (0 != g_flash_handler[device_num].get_lock(device_num)) {
            return g_flash_handler[device_num].unlock(device_num);
        }
        return 1;
    }
    return 3;
}
#endif

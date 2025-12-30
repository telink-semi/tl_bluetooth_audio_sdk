/********************************************************************************************************
 * @file    hal_flash_prot.h
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
#include <string.h>
#include <stdbool.h>
#include "flash/flash_common.h"
#include "core/mcu_type.h"
/**
 *  flash protection mechanism description:
 *  default Protection Size(modify flash_init_list and turn FLASH_PROTECT_MODIFY_CONFIG on if modifying the flash protection size):
 *    - protect half of the flash space size;
 *    - if there is no choice of half the flash space size, the minimum enumeration is selected;
 *  @attention:
 *     flash_init_list(block_size),the application layer is modified as required,the suggestion:
 *     Only the firmware area will be locked, the data area will not be locked.
 *       - If the frequency of data area writing is very high, it is necessary to add protection frequently,
 *         resulting in the number of times to write the flash status register quickly reached the upper limit,
 *         which may cause the read flash status register read wrong;
 *       - during the power-down process, if the flash status register is operated,
 *         there is a risk of mistakenly operating the flash status register, resulting in the failure of the qe enable,
 *         as well as causing the flash to be in a non-software protection, which cannot be unlocked,
 *         and the probability of the above risk is increased if the data area is written more frequently,
 *         and cancel protection and adding protection is done continuously.
 */
#define FLASH_PROTECT_MODIFY_CONFIG   0


#if(MCU_CORE_TYPE == MCU_CORE_B91 || MCU_CORE_TYPE == MCU_CORE_B92 )
extern flash_hal_handler_t g_flash_handler;
extern bool g_mid_matched;
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
unsigned char hal_flash_init(flash_hal_user_handler_t* handler);

/**
 * @brief       This function is used to enable the 4line function of flash.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char hal_flash_4line_en(void);

/**
 * @brief       This function is used to disable the 4line function of flash.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char hal_flash_4line_dis(void);

/**
 * @brief       This function is used to lock a flash through lock_fp,
 *              if the flash is not locked, it is locked, if the flash is locked, it is not executed.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_lock(void);

/**
 * @brief       This function is used to unlock a flash through lock_fp,
 *              if the flash is locked, it is unlocked, if the flash is unlocked, it is not executed.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_unlock(void);
#elif (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL721X|| MCU_CORE_TYPE == MCU_CORE_TL753X)
extern bool g_mid_matched[SLAVE_CNT];
extern flash_hal_handler_t g_flash_handler[SLAVE_CNT];
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
unsigned char hal_flash_init(flash_hal_user_handler_t* handler);
/**
 * @brief       This function is used to enable the 4line function of flash.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char hal_flash_4line_en_with_device_num(mspi_slave_device_num_e device_num);

/**
 * @brief       This function is used to disable the 4line function of flash.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported.
 */
unsigned char hal_flash_4line_dis_with_device_num(mspi_slave_device_num_e device_num);

/**
 * @brief       This function is used to lock a flash through lock_fp,
 *              if the flash is not locked, it is locked, if the flash is locked, it is not executed.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_lock_with_device_num(mspi_slave_device_num_e device_num);

/**
 * @brief       This function is used to unlock a flash through lock_fp,
 *              if the flash is locked, it is unlocked, if the flash is unlocked, it is not executed.
 * @param[in]   device_num  - the number of slave device.
 * @return      1: success, 0: error, 2: parameter error, 3: mid is not supported
 */
unsigned char hal_flash_unlock_with_device_num(mspi_slave_device_num_e device_num);
#endif

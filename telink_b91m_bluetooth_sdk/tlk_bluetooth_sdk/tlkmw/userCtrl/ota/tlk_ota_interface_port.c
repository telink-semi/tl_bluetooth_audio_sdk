/********************************************************************************************************
 * @file    tlk_ota_interface_port.c
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
#include "drivers.h"
#include <stdlib.h>
#include "tlk_ota_types.h"

/*Nvds ota interface, User can implement.*/

/**
 * @brief      Allocate memory for OTA operations
 * @param[in]  dataLen - Size of memory to allocate
 * @return     void* - Pointer to allocated memory, NULL if allocation failed
 * @note       User needs to implement this function
 */
__attribute__((weak)) void *nvds_ota_malloc(uint32_t dataLen)
{
    return malloc(dataLen);
}

/**
 * @brief      Free previously allocated memory
 * @param[in]  pBuffer - Pointer to memory to be freed
 * @return     none
 * @note       User needs to implement this function
 */
__attribute__((weak)) void nvds_ota_free(void *pBuffer)
{
    free(pBuffer);
}

/**
 * @brief      Write data to flash memory
 * @param[in]  addr - Flash address to write to
 * @param[in]  len  - Length of data to write
 * @param[in]  buf  - Pointer to data buffer
 * @return     none
 * @note       User needs to implement this function
 */
__attribute__((weak)) void tlk_nvds_ota_write(unsigned long addr, unsigned long len, unsigned char *buf)
{
    flash_page_program(addr, len, buf);
}

/**
 * @brief      Read data from flash memory
 * @param[in]  addr - Flash address to read from
 * @param[in]  len  - Length of data to read
 * @param[out] buf  - Pointer to buffer to store read data
 * @return     none
 * @note       User needs to implement this function
 */
__attribute__((weak)) void tlk_nvds_ota_read(unsigned long addr, unsigned long len, unsigned char *buf)
{
#if (MCU_CORE_TL752X_TEMP)
    flash_4read(addr, len, buf);
#else
    flash_dread(addr, len, buf);
#endif
}

/**
 * @brief      Erase flash sector
 * @param[in]  addr - Address of sector to erase
 * @return     none
 * @note       User needs to implement this function
 */
__attribute__((weak)) void tlk_nvds_ota_eraseSector(unsigned long addr)
{
    flash_erase_sector(addr);
}

/**
 * @brief      Load user configuration data
 * @param[out] pBuffer - Pointer to buffer to store loaded data
 * @param[in]  buffLen - Size of buffer
 * @param[in]  UserArg - User argument
 * @return     int - OTA_DEFINEDERR as default implementation
 * @note       User needs to implement this function
 */
__attribute__((weak)) int tlk_nvds_ota_usercfg_load(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    (void)pBuffer;
    (void)buffLen;
    (void)UserArg;

    //The user needs to implement.
    return -OTA_DEFINEDERR;
}

/**
 * @brief      Save user configuration data
 * @param[in]  pBuffer - Pointer to data buffer to save
 * @param[in]  buffLen - Size of data buffer
 * @param[in]  UserArg - User argument
 * @return     int - OTA_DEFINEDERR as default implementation
 * @note       User needs to implement this function
 */
__attribute__((weak)) int tlk_nvds_ota_usercfg_save(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    (void)pBuffer;
    (void)buffLen;
    (void)UserArg;
    //The user needs to implement.
    return -OTA_DEFINEDERR;
}

/**
 * @brief      Save OTA status information
 * @param[in]  pBuffer - Pointer to status data buffer
 * @param[in]  buffLen - Size of status data
 * @param[in]  UserArg - User argument
 * @return     int - OTA_DEFINEDERR as default implementation
 * @note       User needs to implement this function
 */
__attribute__((weak)) int tlk_nvds_ota_status_save(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    (void)pBuffer;
    (void)buffLen;
    (void)UserArg;
    //The user needs to implement.
    return -OTA_DEFINEDERR;
}

/**
 * @brief      Save backup address information
 * @param[in]  pBuffer - Pointer to backup address data
 * @param[in]  buffLen - Size of backup address data
 * @param[in]  UserArg - User argument
 * @return     int - OTA_DEFINEDERR as default implementation
 * @note       User needs to implement this function
 */
__attribute__((weak)) int tlk_nvds_ota_backup_addr_save(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    (void)pBuffer;
    (void)buffLen;
    (void)UserArg;

    //The user needs to implement.
    return -OTA_DEFINEDERR;
}

/**
 * @brief      Save user area address information
 * @param[in]  pBuffer - Pointer to user area address data
 * @param[in]  buffLen - Size of user area address data
 * @param[in]  UserArg - User argument
 * @return     int - OTA_DEFINEDERR as default implementation
 * @note       User needs to implement this function
 */
__attribute__((weak)) int tlk_nvds_ota_userarea_addr_save(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    //The user needs to implement.
    (void)pBuffer;
    (void)buffLen;
    (void)UserArg;
    return -OTA_DEFINEDERR;
}

/**
 * @brief      Initialize NVDS OTA interface
 * @param[out] p_interface - Pointer to OTA interface structure to initialize
 * @return     none
 */
void tlk_nvds_ota_interface_init(nvds_ota_Interface_t *p_interface)
{
    if (p_interface != NULL) {
        p_interface->nvds_ota_malloc             = nvds_ota_malloc;
        p_interface->nvds_ota_free               = nvds_ota_free;
        p_interface->nvds_ota_read               = tlk_nvds_ota_read;
        p_interface->nvds_ota_write              = tlk_nvds_ota_write;
        p_interface->nvds_ota_eraseSector        = tlk_nvds_ota_eraseSector;
        p_interface->nvds_ota_user_load          = tlk_nvds_ota_usercfg_load;
        p_interface->nvds_ota_user_save          = tlk_nvds_ota_usercfg_save;
        p_interface->nvds_ota_status_save        = tlk_nvds_ota_status_save;
        p_interface->nvds_ota_backup_addr_save   = tlk_nvds_ota_backup_addr_save;
        p_interface->nvds_ota_userarea_addr_save = tlk_nvds_ota_userarea_addr_save;
    }
}

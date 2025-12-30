/********************************************************************************************************
 * @file    tlk_ota_interface_port.h
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
#ifndef TLK_OTA_INTERFACE_PORT_H
#define TLK_OTA_INTERFACE_PORT_H

typedef struct
{
    void *(*nvds_ota_malloc)(uint32_t dataLen);
    void (*nvds_ota_free)(void *pBuffer);
    void (*nvds_ota_write)(unsigned long addr, unsigned long len, unsigned char *buf);
    void (*nvds_ota_read)(unsigned long addr, unsigned long len, unsigned char *buf);
    void (*nvds_ota_eraseSector)(unsigned long addr);
    int (*nvds_ota_user_load)(uint8_t *pBuffer, uint32_t buffLen, void *UserArg);
    int (*nvds_ota_user_save)(uint8_t *pBuffer, uint32_t buffLen, void *UserArg);
    int (*nvds_ota_status_save)(uint8_t *pBuffer, uint32_t buffLen, void *UserArg);
    int (*nvds_ota_backup_addr_save)(uint8_t *pBuffer, uint32_t buffLen, void *UserArg);
    int (*nvds_ota_userarea_addr_save)(uint8_t *pBuffer, uint32_t buffLen, void *UserArg);
} nvds_ota_Interface_t;

/**
 * @brief      Initialize NVDS OTA interface
 * @param[out] p_interface - Pointer to OTA interface structure to initialize
 * @return     none
 */
void tlk_nvds_ota_interface_init(nvds_ota_Interface_t *p_interface);


#endif // #ifndef TLK_OTA_INTERFACE_PORT_H

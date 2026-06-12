/********************************************************************************************************
 * @file    tlkhal_flash_B91.c
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
#if MCU_CORE_TYPE == MCU_CORE_B91

_attribute_data_retention_ uint32_t s_tlkhal_flash_size = 0x200000;

void tlkhal_flash_init(uint8_t enable_lock)
{
    (void)enable_lock;
    /*
		Flash Type	uid CMD		MID		    Company
		P25Q80U		0x4b		0x146085	PUYA
		P25Q16SU    0x4b        0x156085    PUYA
		P25Q32SU    0x4b        0x166085    PUYA
		P25Q128L    0x4b        0x186085    PUYA
		P25Q64SL    0x4b        0x176085    PUYA
	    GD25LQ16E   0x4b        0x1560c8    GD
	 */

    unsigned int mid      = flash_read_mid();
    uint8_t      capacity = ((mid & 0x00ff0000) >> 16);
    switch (capacity) {
    case FLASH_SIZE_1M:
        s_tlkhal_flash_size = 0x100000;
        break;
    case FLASH_SIZE_2M:
        s_tlkhal_flash_size = 0x200000;
        break;
    case FLASH_SIZE_4M:
        s_tlkhal_flash_size = 0x400000;
        break;
    case FLASH_SIZE_8M:
        s_tlkhal_flash_size = 0x800000;
        break;
    case FLASH_SIZE_16M:
        s_tlkhal_flash_size = 0x1000000;
        break;
    default:
        break;
    }
}

uint32_t tlkhal_flash_get_size(void)
{
    return s_tlkhal_flash_size;
}

void tlkhal_flash_lock_ex(tlkhal_flash_lock_size_e size)
{
    (void)size;
}

void tlkhal_flash_lock(void) {}

void tlkhal_flash_unlock(void) {}

void tlkhal_flash_4line_enable(void)
{
    unsigned int  mid    = flash_read_mid();
    unsigned char status = flash_4line_en(mid);
    if (1 == status) {
        flash_read_page = flash_4read;
        flash_set_xip_config(0x65EB);
        flash_write_page = flash_quad_page_program;
    }
}

#endif

/********************************************************************************************************
 * @file    app_flash.c
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
#include "app_flash.h"

/* global variable for some flash sector address, initial value use 2M flash.
 * attention that they will be changed in function "blc readFlashSize_autoConfigCustomFlashSector"  */
_attribute_data_retention_ unsigned int flash_full_size             = 0x20000;
_attribute_data_retention_ unsigned int flash_sector_mac_address    = CFG_ADR_MAC_2M_FLASH;
_attribute_data_retention_ unsigned int flash_sector_calibration    = CFG_ADR_CALIBRATION_2M_FLASH;
_attribute_data_retention_ unsigned int flash_sector_smp_storage    = FLASH_ADR_SMP_PAIRING_2M_FLASH;
_attribute_data_retention_ unsigned int flash_sector_simple_sdp_att = FLASH_SDP_ATT_ADDRESS_2M_FLASH;

_attribute_data_retention_ unsigned int  blc_flash_mid    = 0;
_attribute_data_retention_ unsigned int  blc_flash_vendor = 0;
_attribute_data_retention_ unsigned char blc_flash_capacity;

/**
 * @brief		This function is used to read flash mid, get flash vendor, and set flash capacity
 * @param[in]	none
 * @return      none
 */
void tlk_flash_read_mid_get_vendor_set_capacity(void)
{
    /*
		Flash Type	uid CMD		MID		    Company
		P25Q80U		0x4b		0x146085	PUYA
		P25Q16SU    0x4b        0x156085    PUYA
		P25Q32SU    0x4b        0x166085    PUYA
		P25Q128L    0x4b        0x186085    PUYA
		P25Q64SL    0x4b        0x176085    PUYA
	    GD25LQ16E   0x4b        0x1560c8    GD
	 */
    /* attention: blc_flash_mid/blc_flash_vendor/blc_flash_capacity will be used by application and stack later
	 * so do not change code here */
#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL322X)
    blc_flash_mid = flash_read_mid_with_device_num(SLAVE0);
#else
    blc_flash_mid = flash_read_mid();
#endif
    blc_flash_vendor   = flash_get_vendor(blc_flash_mid);
    blc_flash_capacity = ((blc_flash_mid & 0x00ff0000) >> 16);

    tlkapi_send_string_u32s(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] flash initialization", blc_flash_mid, blc_flash_vendor, blc_flash_capacity);
}

/**
 * @brief		This function can automatically recognize the flash size,
 * 				and the system selects different customized sector according
 * 				to different sizes.
 * @param[in]	none
 * @return      none
 */
void tlk_readFlashSize_autoConfigCustomFlashSector(void)
{
    tlk_flash_read_mid_get_vendor_set_capacity();

    if (0) {
    }
#if (FLASH_P25Q80U_SUPPORT_EN) //1M
    else if (blc_flash_capacity == FLASH_SIZE_1M) {
        flash_full_size          = 0x100000;
        flash_sector_mac_address = CFG_ADR_MAC_1M_FLASH;
        flash_sector_smp_storage = FLASH_ADR_SMP_PAIRING_1M_FLASH;

#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
        flash_sector_simple_sdp_att = FLASH_SDP_ATT_ADDRESS_1M_FLASH;
#endif

        tlkapi_printf(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] 1M Flash, MAC on %x", flash_sector_mac_address);
    }
#endif
#if (FLASH_P25Q16SU_SUPPORT_EN || FLASH_GD25LQ16E_SUPPORT_EN) //2M
    else if (blc_flash_capacity == FLASH_SIZE_2M) {
        flash_full_size          = 0x200000;
        flash_sector_mac_address = CFG_ADR_MAC_2M_FLASH;
        flash_sector_smp_storage = FLASH_ADR_SMP_PAIRING_2M_FLASH;

#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
        flash_sector_simple_sdp_att = FLASH_SDP_ATT_ADDRESS_2M_FLASH;
#endif

        tlkapi_printf(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] 2M Flash, MAC on %x", flash_sector_mac_address);
    }
#endif
#if (FLASH_P25Q32SU_SUPPORT_EN) //4M
    else if (blc_flash_capacity == FLASH_SIZE_4M) {
        flash_full_size          = 0x400000;
        flash_sector_mac_address = CFG_ADR_MAC_4M_FLASH;
        flash_sector_smp_storage = FLASH_ADR_SMP_PAIRING_4M_FLASH;

#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
        flash_sector_simple_sdp_att = FLASH_SDP_ATT_ADDRESS_4M_FLASH;
#endif

        tlkapi_printf(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] 4M Flash, MAC on %x", flash_sector_mac_address);
    }
#endif
#if (FLASH_P25Q64SL_SUPPORT_EN) //8M
    else if (blc_flash_capacity == FLASH_SIZE_8M) {
        flash_full_size          = 0x800000;
        flash_sector_mac_address = CFG_ADR_MAC_8M_FLASH;
        flash_sector_smp_storage = FLASH_ADR_SMP_PAIRING_8M_FLASH;

#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
        flash_sector_simple_sdp_att = FLASH_SDP_ATT_ADDRESS_8M_FLASH;
#endif

        tlkapi_printf(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] 8M Flash, MAC on %x", flash_sector_mac_address);
    }
#endif
#if (FLASH_P25Q128L_SUPPORT_EN) //16M
    else if (blc_flash_capacity == FLASH_SIZE_16M) {
        flash_full_size          = 0x1000000;
        flash_sector_mac_address = CFG_ADR_MAC_16M_FLASH;
        flash_sector_smp_storage = FLASH_ADR_SMP_PAIRING_16M_FLASH;

#if (ACL_CENTRAL_SIMPLE_SDP_ENABLE)
        flash_sector_simple_sdp_att = FLASH_SDP_ATT_ADDRESS_16M_FLASH;
#endif

        tlkapi_printf(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] 16M Flash, MAC on  %x", flash_sector_mac_address);
    }
#endif
    else {
        /*This SDK do not support other flash size except what listed above
		  If code stop here, please check your Flash */
        tlkapi_send_string_data(APP_FLASH_INIT_LOG_EN, "[FLASH][INI] flash size do not support !!!", &blc_flash_capacity, 1);
    }
}

/*
 *B91: 	VVWWXX 775FD8 YYZZ
 *B92:  VVWWXX B4CF3C YYZZ

 * public_mac:
 * 				B91 : VVWWXX 775FD8
 * 				B92 : VVWWXX B4CF3C
 *
 * random_static_mac: VVWWXXYYZZ C0
 */
/**
 * @brief		This function is used to initialize the MAC address
 * @param[in]	flash_addr - flash address for MAC address
 * @param[in]	mac_public - public address
 * @param[in]	mac_random_static - random static MAC address
 * @return      none
 */
_attribute_no_inline_ void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static)
{
    if (flash_sector_mac_address == 0) {
        return;
    }

    int rand_mac_byte3_4_read_OK = 0;
    u8  mac_read[8];
    flash_read_page(flash_addr, 8, mac_read);

    u8 value_rand[5];
#if (!MCU_CORE_TL752X_TEMP)
    generateRandomNum(5, value_rand);
#else
    unsigned char randNums = 0;
    for (int i = 0; i < 5; i++) {
        randNums      = (trng_rand() % 0xFF);
        value_rand[i] = randNums;
    }
#endif

    u8 ff_six_byte[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    if (memcmp(mac_read, ff_six_byte, 6)) { //read MAC address on flash success
        tmemcpy(mac_public, mac_read, 6);   //copy public address from flash

        if (mac_read[6] != 0xFF && mac_read[7] != 0xFF) {
            mac_random_static[3]     = mac_read[6];
            mac_random_static[4]     = mac_read[7];
            rand_mac_byte3_4_read_OK = 1;
        }
    } else { //no MAC address on flash

#if (BUILT_IN_MAC_ON_EFUSE)
        if (efuse_get_mac_address(mac_read, 8)) //read MAC address on Efuse success
        {
            tmemcpy(mac_public, mac_read, 6); //copy public address from Efuse

            mac_random_static[3]     = mac_read[6];
            mac_random_static[4]     = mac_read[7];
            rand_mac_byte3_4_read_OK = 1;
        } else
#endif
        {
            mac_public[0] = value_rand[0];
            mac_public[1] = value_rand[1];
            mac_public[2] = value_rand[2];

            /* company id */
            mac_public[3] = U32_BYTE0(PDA_COMPANY_ID);
            mac_public[4] = U32_BYTE1(PDA_COMPANY_ID);
            mac_public[5] = U32_BYTE2(PDA_COMPANY_ID);

            flash_write_page(flash_addr, 6, mac_public); //store public address on flash for future use
        }
    }


    mac_random_static[0] = mac_public[0];
    mac_random_static[1] = mac_public[1];
    mac_random_static[2] = mac_public[2];
    mac_random_static[5] = 0xC0; //for random static

    if (!rand_mac_byte3_4_read_OK) {
        mac_random_static[3] = value_rand[3];
        mac_random_static[4] = value_rand[4];

        flash_write_page(flash_addr + 6, 2, (u8 *)(mac_random_static + 3)); //store random address on flash for future use
    }
}

/********************************************************************************************************
 * @file    app_flash.h
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
#ifndef BLT_COMMON_H_
#define BLT_COMMON_H_

#include "vendor/common/user_config.h"


/* flash initialization log enable */
#ifndef APP_FLASH_INIT_LOG_EN
#define APP_FLASH_INIT_LOG_EN 1
#endif


/**
 * @brief	Flash using area default Configuration, user can change some of them in app_config.h
 * 			CFG_ADR_MAC:  		  BLE MAC address stored in flash, can not change this value
 * 			CFG_ADR_CALIBRATION:  some calibration data stored in flash, can not change this value
 * 			FLASH_ADR_SMP_PAIRING & FLASH_SMP_PAIRING_MAX_SIZE:
 * 									If Peripheral or Central SMP enable, use 16K flash for SMP pairing information storage.
 * 									First 8K is for normal use, second 8K is a backup to guarantee SMP information never lose.
 * 									use API blc_smp_configPairingSecurityInfoStorageAddressAndSize(flash_sector_smp_storage, FLASH_SMP_PAIRING_MAX_SIZE)
 * 									to set the two value.
 * 			FLASH_SDP_ATT_ADDRESS & FLASH_SDP_ATT_MAX_SIZE
 * 									If ACL central role use service discovery, use this flash area to store some critical information of peer GATT server.
 */

#ifndef FLASH_SMP_PAIRING_MAX_SIZE
#define FLASH_SMP_PAIRING_MAX_SIZE (2 * 4096)
#endif

#ifndef FLASH_SDP_ATT_MAX_SIZE
#define FLASH_SDP_ATT_MAX_SIZE (2 * 4096) //8K flash for ATT HANDLE storage
#endif                                    //if LE Audio used, additional internal backup 8K flash (total 16K)


/**************************** 1 M Flash *******************************/
#ifndef CFG_ADR_MAC_1M_FLASH
#define CFG_ADR_MAC_1M_FLASH 0xFF000
#endif

#ifndef CFG_ADR_CALIBRATION_1M_FLASH
#define CFG_ADR_CALIBRATION_1M_FLASH 0xFE000
#endif

#ifndef CFG_ADR_SECURE_BOOT_1M_FLASH
#define CFG_ADR_SECURE_BOOT_1M_FLASH 0xF8000 //F8000-FB000
#endif

/* SMP pairing and key information area */
#ifndef FLASH_ADR_SMP_PAIRING_1M_FLASH
#define FLASH_ADR_SMP_PAIRING_1M_FLASH 0xF4000 //F4000 & F5000 & F6000 & F7000
#endif


/* for ACL Central simple SDP: bonding ACL Peripheral GATT service critical information area */
#ifndef FLASH_SDP_ATT_ADDRESS_1M_FLASH
#define FLASH_SDP_ATT_ADDRESS_1M_FLASH 0xF2000 //F2000 & F3000
#endif


/**************************** 2 M Flash *******************************/
#ifndef CFG_ADR_MAC_2M_FLASH
#define CFG_ADR_MAC_2M_FLASH 0x1FF000
#endif

#ifndef CFG_ADR_CALIBRATION_2M_FLASH
#define CFG_ADR_CALIBRATION_2M_FLASH 0x1FE000
#endif

#ifndef CFG_ADR_SECURE_BOOT_2M_FLASH
#define CFG_ADR_SECURE_BOOT_2M_FLASH 0x1F8000 //1F8000-1FB000
#endif
/* SMP pairing and key information area */
#ifndef FLASH_ADR_SMP_PAIRING_2M_FLASH
#define FLASH_ADR_SMP_PAIRING_2M_FLASH 0x1EC000 //1EC000 & 1ED000 & 1EE000 & 1EF000
#endif

/* for ACL Central simple SDP: bonding ACL Peripheral GATT service critical information area */
#ifndef FLASH_SDP_ATT_ADDRESS_2M_FLASH
#define FLASH_SDP_ATT_ADDRESS_2M_FLASH 0x1EA000 //1EA000 & 1EB000
#endif


/**************************** 4 M Flash *******************************/
#ifndef CFG_ADR_MAC_4M_FLASH
#define CFG_ADR_MAC_4M_FLASH 0x3FF000
#endif

#ifndef CFG_ADR_CALIBRATION_4M_FLASH
#define CFG_ADR_CALIBRATION_4M_FLASH 0x3FE000
#endif

#ifndef CFG_ADR_SECURE_BOOT_4M_FLASH
#define CFG_ADR_SECURE_BOOT_4M_FLASH 0x3F8000 //3F8000-3FB000
#endif
/* SMP pairing and key information area */
#ifndef FLASH_ADR_SMP_PAIRING_4M_FLASH
#define FLASH_ADR_SMP_PAIRING_4M_FLASH 0x3EC000 //3EC000 & 3ED000 & 3EE000 & 3EF000
#endif


/* for ACL Central simple SDP: bonding ACL Peripheral GATT service critical information area */
#ifndef FLASH_SDP_ATT_ADDRESS_4M_FLASH
#define FLASH_SDP_ATT_ADDRESS_4M_FLASH 0x3EA000 //3EA000 & 3EB000
#endif

/**************************** 8 M Flash *******************************/
#ifndef CFG_ADR_MAC_8M_FLASH
#define CFG_ADR_MAC_8M_FLASH 0x7FF000
#endif

#ifndef CFG_ADR_CALIBRATION_8M_FLASH
#define CFG_ADR_CALIBRATION_8M_FLASH 0x7FE000
#endif

#ifndef CFG_ADR_SECURE_BOOT_8M_FLASH
#define CFG_ADR_SECURE_BOOT_8M_FLASH 0x7F8000 //7F8000-7FB000
#endif

/* SMP pairing and key information area */
#ifndef FLASH_ADR_SMP_PAIRING_8M_FLASH
#define FLASH_ADR_SMP_PAIRING_8M_FLASH 0x7EC000 //7EC000 & 7ED000 & 7EE000 & 7EF000
#endif


/* for ACL Central simple SDP: bonding ACL Peripheral GATT service critical information area */
#ifndef FLASH_SDP_ATT_ADDRESS_8M_FLASH
#define FLASH_SDP_ATT_ADDRESS_8M_FLASH 0x7EA000 //7EA000 & 7EB000
#endif
/**************************** 16 M Flash *******************************/
#ifndef CFG_ADR_MAC_16M_FLASH
#define CFG_ADR_MAC_16M_FLASH 0xFFF000
#endif

#ifndef CFG_ADR_CALIBRATION_16M_FLASH
#define CFG_ADR_CALIBRATION_16M_FLASH 0xFFE000
#endif

#ifndef CFG_ADR_SECURE_BOOT_16M_FLASH
#define CFG_ADR_SECURE_BOOT_16M_FLASH 0xFF8000 //FF8000-FFB000
#endif

/* SMP pairing and key information area */
#ifndef FLASH_ADR_SMP_PAIRING_16M_FLASH
#define FLASH_ADR_SMP_PAIRING_16M_FLASH 0xFEC000 //FEC000 & FED000 & FEE000 & FEF000
#endif

/* for ACL Central simple SDP: bonding ACL Peripheral GATT service critical information area */
#ifndef FLASH_SDP_ATT_ADDRESS_16M_FLASH
#define FLASH_SDP_ATT_ADDRESS_16M_FLASH 0xFEA000 //FEA000 & FEB000
#endif


/******************************************************************************
 * Macro: Flash offset
 * Descr: The default capacity of Flash is 2M. The following macros
 * define the relative offset of Flash. For details about flash
 * capacity configuration/address configuration, see the function
 * tlkcfg_getFLashCap/tlkcfg_getFlashAddr.
 *******************************************************************************/
#ifndef TLK_CFG_FLASH_MAC_ADDR
#define TLK_CFG_FLASH_MAC_ADDR                                               \
    (0xFF000) // BLE &BT MAC ADDRESS ,and can not change. BLE 0XFF000 BT 0XFF100
#endif

#ifndef TLK_CFG_FLASH_USER_SETTING_ADDR
#define TLK_CFG_FLASH_USER_SETTING_ADDR (0xD0000) // 8K 0XD0000-0XD1FFF
#endif

#ifndef TLK_CFG_FLASH_BT_DEVICES_ADDR
#define TLK_CFG_FLASH_BT_DEVICES_ADDR (0xD2000) // 8k 0XD2000-0XD3FFF
#endif

#ifndef TLK_CFG_FLASH_LE_ADDR
#define TLK_CFG_FLASH_LE_ADDR (0xD4000) // 8k 0XD4000-0XD5FFF
#endif

#ifndef TLK_CFG_FLASH_AUDIO_ADDR
#define TLK_CFG_FLASH_AUDIO_ADDR (0xD8000) // 8k 0XD8000-0XD9FFF
#endif

#ifndef TLK_CFG_FLASH_PBAP_LIST_ADDR
#define TLK_CFG_FLASH_PBAP_LIST_ADDR (0xC0000) // 64k 0XC0000-0XCFFFF
#endif

/** Calibration Information FLash Address Offset of  CFG_ADR_CALIBRATION_xx_FLASH ***/
#ifndef CALIB_OFFSET_CAP_INFO
#define CALIB_OFFSET_CAP_INFO 0x00
#endif

#ifndef CALIB_OFFSET_TP_INFO
#define CALIB_OFFSET_TP_INFO 0x40
#endif

#ifndef CALIB_OFFSET_ADC_VREF
#define CALIB_OFFSET_ADC_VREF 0xC0
#endif

#ifndef CALIB_OFFSET_FIRMWARE_SIGNKEY
#define CALIB_OFFSET_FIRMWARE_SIGNKEY 0x180
#endif


/**
 * @brief		This function can automatically recognize the flash size,
 * 				and the system selects different customized sector according
 * 				to different sizes.
 * @param[in]	none
 * @return      none
 */
void tlk_readFlashSize_autoConfigCustomFlashSector(void);


/**
 * @brief		This function is used to initialize the MAC address
 * @param[in]	flash_addr - flash address for MAC address
 * @param[in]	mac_public - public address
 * @param[in]	mac_random_static - random static MAC address
 * @return      none
 */
void blc_initMacAddress(int flash_addr, u8 *mac_public, u8 *mac_random_static);


extern unsigned int  blc_flash_mid;
extern unsigned int  blc_flash_vendor;
extern unsigned char blc_flash_capacity;

extern unsigned int flash_full_size;
extern unsigned int flash_sector_mac_address;
extern unsigned int flash_sector_smp_storage;
extern unsigned int flash_sector_simple_sdp_att;


#endif /* BLT_COMMON_H_ */

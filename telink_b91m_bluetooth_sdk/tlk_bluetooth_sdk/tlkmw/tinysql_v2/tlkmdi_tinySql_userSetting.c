/********************************************************************************************************
 * @file    tlkmdi_tinySql_userSetting.c
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
#include "tlkapi/tlkapi.h"
#include "tlkmdi_tinySql.h"
#include "tlkmdi_tinySql_inner.h"

#if (TLK_MW_TINYSQL_V2_ENABLE)

typedef enum
{
    MAC_INDEX_PEER_TPSLL = 0,
    MAC_INDEX_PEER_TWS_BT,
    MAC_INDEX_TWS_ALLOC_MAC,
    MAC_INDEX_TWS_AC_CODE,
    MAC_INDEX_NUM,
} MAC_INDEX_ENUM;

/**
 * @brief Structure for saving MAC addresses.
 */
typedef struct
{
    uint8_t macAddr[MAC_INDEX_NUM][6];
} addrSave_t;

typedef enum
{
    TINYSQL_USER_SETTING_KEY_WORKMODE = 0,
    TINYSQL_USER_SETTING_KEY_ADDRS,
    TINYSQL_USER_SETTING_KEY_KEYCFG,
    TINYSQL_USER_SETTING_KEY_BTNAME,

    TINYSQL_USER_SETTING_KEY_NUM
} TINYSQL_USER_SETTING_KEY_ENUM;

const char *const sc_tlkmdi_tinysql_usersetting_key[TINYSQL_USER_SETTING_KEY_NUM] = {
    [TINYSQL_USER_SETTING_KEY_WORKMODE] = "us_00",
    [TINYSQL_USER_SETTING_KEY_ADDRS]    = "us_01",
    [TINYSQL_USER_SETTING_KEY_KEYCFG]   = "us_02",
    [TINYSQL_USER_SETTING_KEY_BTNAME]   = "us_03",
};

static keyConfigs_t s_tlkmdi_tinysql_keyConfigs = {0};
static uint16_t     s_tlkmdi_tinysql_usbid      = 0;

/**
 * @brief       Reset Bluetooth name based on MAC address.
 * @param[in]   macAddress - MAC address to generate BT name.
 * @return      none.
 */
static void tlkmdi_tinySql_BTNameReset(uint8_t buf[TINYSQL_BT_NAME_LENS], uint8_t macAddress[6])
{
    //default name Telink-BT-(MAC address)
    memcpy(buf, "Telink-BT-", sizeof("Telink-BT-") - 1);
    int arrOffset = sizeof("Telink-BT-") - 1;
    for (int i = 0; i < 6; i++) {
        char highNum = (macAddress[i] >> 4);
        if (highNum <= 9) {
            highNum += '0';
        } else {
            highNum += 'A' - 10;
        }
        char lowNum = (macAddress[i] & 0x0f);
        if (lowNum <= 9) {
            lowNum += '0';
        } else {
            lowNum += 'A' - 10;
        }
        buf[arrOffset++] = highNum;
        buf[arrOffset++] = lowNum;
        if (i != 5) {
            buf[arrOffset++] = ':';
        } else {
            buf[arrOffset++] = '\0';
        }
    }
}

/**
 * @brief       Reset user settings to default values.
 * @return      none.
 */
static void tlkmdi_tinySql_userSettingReset(void)
{
    for (size_t i = 0; i < TINYSQL_USER_SETTING_KEY_NUM; i++) {
        const char *key = sc_tlkmdi_tinysql_usersetting_key[i];
        tlkmdi_tinySql_key_delete(key);
    }
}

/**
 * @brief       Set MAC address at specified index.
 * @param[in]   index - Index of the MAC address to set.
 * @param[in]   addr - Buffer containing the MAC address to be set.
 * @return      none.
 */
static void tlkmdi_tinySql_userSettingSetAddr(uint8_t index, uint8_t *addr)
{
    if (addr == NULL) {
        return;
    }
    const char *key = sc_tlkmdi_tinysql_usersetting_key[TINYSQL_USER_SETTING_KEY_ADDRS];
    addrSave_t  addrs;
    uint32_t    len = tlkmdi_tinySql_key_get(key, &addrs, sizeof(addrs));
    if (len == 0) { //key not found ,reset to default
        memset(&addrs, 0xFF, sizeof(addrs));
    }
    memcpy(addrs.macAddr[index], addr, 6);
    tlkmdi_tinySql_key_set(key, &addrs, sizeof(addrs));
}

/**
 * @brief       Get MAC address at specified index.
 * @param[in]   index - Index of the MAC address to get.
 * @param[out]  pBuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
static int tlkmdi_tinySql_userSettingGetAddr(uint8_t index, uint8_t *pBuffer)
{
    if (pBuffer == NULL) {
        return -TLK_EPARAM;
    }
    const char *key = sc_tlkmdi_tinysql_usersetting_key[TINYSQL_USER_SETTING_KEY_ADDRS];
    addrSave_t  addrs;
    uint32_t    len = tlkmdi_tinySql_key_get(key, &addrs, sizeof(addrs));
    if (len == 0) { //key not found ,reset to default
        memset(&addrs, 0xFF, sizeof(addrs));
        tlkmdi_tinySql_key_set(key, &addrs, sizeof(addrs));
    }
    memcpy(pBuffer, addrs.macAddr[index], 6);
    return TLK_ENONE;
}

/**
 * @brief       Initialize user setting disk.
 * @return      none.
 */
static void tlkmdi_tinySql_UserSettingDiskInit(void)
{
    uint8_t usbFlag = 0;
    flash_read_page((TLK_CFG_FLASH_USBID_ADDR + tlkhal_flash_get_size() - 0x100000), 1, (uint8_t *)&usbFlag);
    s_tlkmdi_tinysql_usbid = ((0xff == usbFlag) ? 0x120 : (0x0100 + usbFlag));
}

/**
 * @brief       Restore user settings to factory defaults.
 * @return      none.
 */
static void tlkmdi_tinySql_UserSettingDiskRestore(void)
{
    tlkmdi_tinySql_userSettingReset();
}

const tinySqlDisk_t tinySql_userSetting_disk = {
    .init           = tlkmdi_tinySql_UserSettingDiskInit,
    .restoreFactory = tlkmdi_tinySql_UserSettingDiskRestore,
};

/**
 * @brief       Get the current working mode of the system.
 * @return      Current work mode as defined by TLK_WORK_MODE_ENUM.
 */
uint8_t tlkmdi_tinySql_getWorkMode(void)
{
    const char *key  = sc_tlkmdi_tinysql_usersetting_key[TINYSQL_USER_SETTING_KEY_WORKMODE];
    uint8_t     mode = 0;
    uint32_t    len  = tlkmdi_tinySql_key_get(key, &mode, sizeof(mode));
    if (len == 0) { //key not found ,reset to default
        mode = 0;
        tlkmdi_tinySql_key_set(key, &mode, sizeof(mode));
    }
    return mode;
}

/**
 * @brief       Set the working mode of the system.
 * @param[in]   mode - The new work mode to be set as per TLK_WORK_MODE_ENUM.
 * @return      none.
 */
void tlkmdi_tinySql_setWorkMode(uint8_t mode)
{
    if (mode > TLK_WORK_MODE_TEST_ATS) {
        return;
    }
    const char *key = sc_tlkmdi_tinysql_usersetting_key[TINYSQL_USER_SETTING_KEY_WORKMODE];
    tlkmdi_tinySql_key_set(key, &mode, sizeof(mode));
}

/**
 * @brief       Get the USB ID.
 * @return      Current USB ID.
 */
uint16_t tlkmdi_tinySql_getUsbID(void)
{
    return s_tlkmdi_tinysql_usbid;
}

/**
 * @brief       Get TPH MAC address.
 * @param[out]  pBuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getTphMacAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_userSettingGetAddr(MAC_INDEX_PEER_TPSLL, pBuffer);
}

/**
 * @brief       Set TPH MAC address.
 * @param[in]   macAddr - Buffer containing the MAC address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setTphMacAddr(uint8_t *macAddr)
{
    tlkmdi_tinySql_userSettingSetAddr(MAC_INDEX_PEER_TPSLL, macAddr);
}

/**
 * @brief       Get TPD MAC address.
 * @param[out]  pBuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getTpdMacAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_userSettingGetAddr(MAC_INDEX_PEER_TPSLL, pBuffer);
}

/**
 * @brief       Set TPD MAC address.
 * @param[in]   macAddr - Buffer containing the MAC address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setTpdMacAddr(uint8_t *macAddr)
{
    tlkmdi_tinySql_userSettingSetAddr(MAC_INDEX_PEER_TPSLL, macAddr);
}

/**
 * @brief       Get peer address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getPeerAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_userSettingGetAddr(MAC_INDEX_PEER_TWS_BT, pBuffer);
}

/**
 * @brief       Set peer address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setPeerAddr(uint8_t *addr)
{
    tlkmdi_tinySql_userSettingSetAddr(MAC_INDEX_PEER_TWS_BT, addr);
}

/**
 * @brief       Get newly allocated address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getNewAllocAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_userSettingGetAddr(MAC_INDEX_TWS_ALLOC_MAC, pBuffer);
}

/**
 * @brief       Set newly allocated address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setNewAllocAddr(uint8_t *addr)
{
    tlkmdi_tinySql_userSettingSetAddr(MAC_INDEX_TWS_ALLOC_MAC, addr);
}

/**
 * @brief       Get generated AC address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getGenAcAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_userSettingGetAddr(MAC_INDEX_TWS_AC_CODE, pBuffer);
}

/**
 * @brief       Set generated AC address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setGenAcAddr(uint8_t *addr)
{
    tlkmdi_tinySql_userSettingSetAddr(MAC_INDEX_TWS_AC_CODE, addr);
}

/**
 * @brief       Reset dongle pairing.
 * @return      none.
 */
void tlkmdi_tinySql_donglePairingReset(void)
{
    uint8_t nullAddr[6];
    memset(nullAddr, 0xff, 6);
    tlkmdi_tinySql_setTpdMacAddr(nullAddr);
}

/**
 * @brief       Reset bttpt pairing.
 * @return      none.
 */
void tlkmdi_tinySql_bttptPairingReset(void)
{
    uint8_t nullAddr[6];
    memset(nullAddr, 0xff, 6);
    tlkmdi_tinySql_setGenAcAddr(nullAddr);
    tlkmdi_tinySql_setNewAllocAddr(nullAddr);
    tlkmdi_tinySql_setPeerAddr(nullAddr);
}

/**
 * @brief       Reset TPD headset pairing.
 * @return      none.
 */
void tlkmdi_tinySql_tpdHeadsetPairingReset(void)
{
    uint8_t nullAddr[6];
    tmemset(nullAddr, 0xff, 6);
    tlkmdi_tinySql_setTphMacAddr(nullAddr);
}

/**
 * @brief       Get the Bluetooth device name.
 * @param[out]  recBuffer - Buffer to store the retrieved device name.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getBtName(uint8_t *recBuffer)
{
    if (recBuffer == NULL) {
        return -TLK_EPARAM;
    }
    uint8_t     name[TINYSQL_BT_NAME_LENS] = {0};
    const char *key                        = sc_tlkmdi_tinysql_usersetting_key[TINYSQL_USER_SETTING_KEY_BTNAME];
    uint32_t    len                        = tlkmdi_tinySql_key_get(key, name, sizeof(name));
    if (len == 0) { //key not found ,reset to default
        uint8_t mac[6];
        tlkmdi_tinySql_getBtMacAddress(mac);
        tlkmdi_tinySql_BTNameReset(name, mac);
        tlkmdi_tinySql_key_set(key, name, sizeof(name));
    }
    memcpy(recBuffer, name, TINYSQL_BT_NAME_LENS);
    return TLK_ENONE;
}

/**
 * @brief       Set the Bluetooth device name.
 * @param[in]   inBuffer - Buffer containing the new device name to be set.
 * @param[in]   datalen - Length of the data to be copied.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_setBtName(uint8_t *inBuffer, uint32_t datalen)
{
    if (inBuffer == NULL || datalen == 0) {
        return -TLK_EPARAM;
    }
    if (datalen >= TINYSQL_BT_NAME_LENS) {
        datalen = TINYSQL_BT_NAME_LENS - 1;
    }
    uint8_t name[TINYSQL_BT_NAME_LENS];
    memcpy(name, inBuffer, datalen);
    name[datalen]   = 0;
    const char *key = sc_tlkmdi_tinysql_usersetting_key[TINYSQL_USER_SETTING_KEY_BTNAME];
    tlkmdi_tinySql_key_set(key, name, sizeof(name));

    return TLK_ENONE;
}

/**
 * @brief       Get key configuration information.
 * @param[out]  key_config_info - Pointer to store the key configuration information.
 * @return      none.
 */
void tlkmdi_tinySql_getKeyCofnig(keyConfigs_t **key_config_info)
{
    *key_config_info = &s_tlkmdi_tinysql_keyConfigs;
}

/**
 * @brief       Update key configuration information.
 * @param[in]   key_config_info - Pointer to the new key configuration information.
 * @return      none.
 */
void tlkmdi_tinySql_updateKeyCofnig(keyConfigs_t *key_config_info)
{
    if (key_config_info == NULL) {
        return;
    }
    memcpy(&s_tlkmdi_tinysql_keyConfigs, key_config_info, sizeof(keyConfigs_t));
}

#endif
/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_userSetting.c
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
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_inner.h"

#if (TLK_MW_TINYSQL_ENABLE)
/******************************************************************************
                           private code begin
******************************************************************************/
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

/**
 * @brief Structure for user settings.
 */
typedef struct
{
    uint8_t      workMode;
    uint8_t      usbMode;
    uint16_t     usbID;
    addrSave_t   addrs;
    keyConfigs_t keyConfigs;
    uint8_t      btName[TINYSQL_BT_NAME_LENS];
} userSettings_t;

_attribute_extend_low_speed_bss_sec_ static userSettings_t sTlkmdiSqlUserSettings = {0};

_attribute_extend_low_speed_bss_sec_ static tlkapi_save_ctrl_t sTlkmdiSqlUserSettingCtrl = {0};

/**
 * @brief       Operate mutex for user setting disk.
 * @param[in]   isLock - Lock or unlock flag.
 * @return      none.
 */
static void tlkmdi_tinySql_userSettingDisk_mutexOperate(uint8_t isLock)
{
    tlkmdi_tinySql_mutex_operate(isLock);
}

/**
 * @brief       Reset Bluetooth name based on MAC address.
 * @param[in]   macAddress - MAC address to generate BT name.
 * @return      none.
 */
static inline void tlkmdi_tinySql_BTNameReset(uint8_t macAddress[6])
{
    //default name Telink-BT-(MAC address)
    tmemcpy(sTlkmdiSqlUserSettings.btName, "Telink-BT-", sizeof("Telink-BT-") - 1);
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
        sTlkmdiSqlUserSettings.btName[arrOffset++] = highNum;
        sTlkmdiSqlUserSettings.btName[arrOffset++] = lowNum;
        if (i != 5) {
            sTlkmdiSqlUserSettings.btName[arrOffset++] = ':';
        } else {
            sTlkmdiSqlUserSettings.btName[arrOffset++] = '\0';
        }
    }
}

/**
 * @brief       Reset user settings to default values.
 * @return      none.
 */
static void tlkmdi_tinySql_UserSettingReset(void)
{
    sTlkmdiSqlUserSettings.workMode = TLK_WORK_MODE_NORMAL;
    sTlkmdiSqlUserSettings.usbMode  = 0;

    uint8_t btmac[6];
    tlkmdi_tinySql_getBtMacAddress(btmac);
    tlkmdi_tinySql_BTNameReset(btmac);

    memset(&sTlkmdiSqlUserSettings.addrs.macAddr[0], 0XFFFFFFFF, sizeof(sTlkmdiSqlUserSettings.addrs));

    memset(&sTlkmdiSqlUserSettings.keyConfigs, 0, sizeof(keyConfigs_t));
}

/**
 * @brief       Set MAC address at specified index.
 * @param[in]   index - Index of the MAC address to set.
 * @param[in]   addr - Buffer containing the MAC address to be set.
 * @return      none.
 */
static void tlkmdi_tinySql_UserSettingSetAddr(uint8_t index, uint8_t *addr)
{
    if (addr == NULL) {
        return;
    }
    tlkmdi_tinySql_userSettingDisk_mutexOperate(true);
    if (tmemcmp(addr, sTlkmdiSqlUserSettings.addrs.macAddr[index], 6) != 0) {
        tmemcpy(sTlkmdiSqlUserSettings.addrs.macAddr[index], addr, 6);
        tlkmdi_tinySql_requestSave(tinySql_userSettingsSaveIndex);
    }
    tlkmdi_tinySql_userSettingDisk_mutexOperate(false);
}

/**
 * @brief       Get MAC address at specified index.
 * @param[in]   index - Index of the MAC address to get.
 * @param[out]  pBuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
static int tlkmdi_tinySql_UserSettingGetAddr(uint8_t index, uint8_t *pBuffer)
{
    if (pBuffer == NULL) {
        return -TLK_EPARAM;
    }
    tlkmdi_tinySql_userSettingDisk_mutexOperate(true);
    tmemcpy(pBuffer, sTlkmdiSqlUserSettings.addrs.macAddr[index], 6);
    tlkmdi_tinySql_userSettingDisk_mutexOperate(false);
    return TLK_ENONE;
}

/**
 * @brief       Initialize user setting disk.
 * @return      none.
 */
static void tlkmdi_tinySql_UserSettingDiskInit(void)
{
    //get UserSetting data from flash,if nodata/not right version/crc fail -> reset data
    unsigned int saveAddress = tlkmdi_tinySql_getSaveAddr(TLKMDI_TINYSQL_DISK0_ADDR);
    tlkapi_save3_init(&sTlkmdiSqlUserSettingCtrl, TLKMDI_TINYSQL_SAVE_SIGN, TLKMDI_TINYSQL_VER, sizeof(userSettings_t), saveAddress);
    int ret = tlkapi_save3_load(&sTlkmdiSqlUserSettingCtrl, (uint8_t *)&sTlkmdiSqlUserSettings, sizeof(sTlkmdiSqlUserSettings));
    if (ret < (int)sizeof(userSettings_t)) {
        tlkmdi_tinySql_UserSettingReset();
    }
    uint8_t usbFlag = 0;
    flash_read_page((TLK_CFG_FLASH_USBID_ADDR + tlkhal_flash_get_size() - 0x100000), 1, (uint8_t *)&usbFlag);
    sTlkmdiSqlUserSettings.usbID = ((0xff == usbFlag) ? 0x120 : (0x0100 + usbFlag));
}

/**
 * @brief       Save user settings to disk.
 * @return      none.
 */
static void tlkmdi_tinySql_UserSettingDiskSave(void)
{
    tlkapi_save3_smartSave(&sTlkmdiSqlUserSettingCtrl, (uint8_t *)&sTlkmdiSqlUserSettings, sizeof(sTlkmdiSqlUserSettings));
}

/**
 * @brief       Restore user settings to factory defaults.
 * @return      none.
 */
static void tlkmdi_tinySql_UserSettingDiskRestore(void)
{
    tlkapi_save3_clean(&sTlkmdiSqlUserSettingCtrl);
    tlkmdi_tinySql_UserSettingReset();
    tlkmdi_tinySql_requestSave(tinySql_userSettingsSaveIndex);
}

const tinySqlDisk_t tinySql_userSetting_disk = {
    .init           = tlkmdi_tinySql_UserSettingDiskInit,
    .restoreFactory = tlkmdi_tinySql_UserSettingDiskRestore,
    .save           = tlkmdi_tinySql_UserSettingDiskSave,
};

/******************************************************************************
                           private code end
******************************************************************************/


/**
 * @brief       Get the current working mode of the system.
 * @return      Current work mode as defined by TLK_WORK_MODE_ENUM.
 */
_always_inline uint8_t tlkmdi_tinySql_getWorkMode(void)
{
    return sTlkmdiSqlUserSettings.workMode;
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
    tlkmdi_tinySql_userSettingDisk_mutexOperate(true);
    if (mode != sTlkmdiSqlUserSettings.workMode) {
        sTlkmdiSqlUserSettings.workMode = mode;
        tlkmdi_tinySql_requestSave(tinySql_userSettingsSaveIndex);
    }
    tlkmdi_tinySql_userSettingDisk_mutexOperate(false);
}

/**
 * @brief       Get the USB ID.
 * @return      Current USB ID.
 */
uint16_t tlkmdi_tinySql_getUsbID(void)
{
    return sTlkmdiSqlUserSettings.usbID;
}

/**
 * @brief       Get TPH MAC address.
 * @param[out]  pBuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getTphMacAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_UserSettingGetAddr(MAC_INDEX_PEER_TPSLL, pBuffer);
}

/**
 * @brief       Set TPH MAC address.
 * @param[in]   macAddr - Buffer containing the MAC address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setTphMacAddr(uint8_t *macAddr)
{
    tlkmdi_tinySql_UserSettingSetAddr(MAC_INDEX_PEER_TPSLL, macAddr);
}

/**
 * @brief       Get TPD MAC address.
 * @param[out]  pBuffer - Buffer to store the retrieved MAC address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getTpdMacAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_UserSettingGetAddr(MAC_INDEX_PEER_TPSLL, pBuffer);
}

/**
 * @brief       Set TPD MAC address.
 * @param[in]   macAddr - Buffer containing the MAC address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setTpdMacAddr(uint8_t *macAddr)
{
    tlkmdi_tinySql_UserSettingSetAddr(MAC_INDEX_PEER_TPSLL, macAddr);
}

/**
 * @brief       Get peer address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getPeerAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_UserSettingGetAddr(MAC_INDEX_PEER_TWS_BT, pBuffer);
}

/**
 * @brief       Set peer address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setPeerAddr(uint8_t *addr)
{
    tlkmdi_tinySql_UserSettingSetAddr(MAC_INDEX_PEER_TWS_BT, addr);
}

/**
 * @brief       Get newly allocated address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getNewAllocAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_UserSettingGetAddr(MAC_INDEX_TWS_ALLOC_MAC, pBuffer);
}

/**
 * @brief       Set newly allocated address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setNewAllocAddr(uint8_t *addr)
{
    tlkmdi_tinySql_UserSettingSetAddr(MAC_INDEX_TWS_ALLOC_MAC, addr);
}

/**
 * @brief       Get generated AC address.
 * @param[out]  pBuffer - Buffer to store the retrieved address.
 * @return      Status code, 0 for success, negative for error.
 */
int tlkmdi_tinySql_getGenAcAddr(uint8_t *pBuffer)
{
    return tlkmdi_tinySql_UserSettingGetAddr(MAC_INDEX_TWS_AC_CODE, pBuffer);
}

/**
 * @brief       Set generated AC address.
 * @param[in]   addr - Buffer containing the address to be set.
 * @return      none.
 */
void tlkmdi_tinySql_setGenAcAddr(uint8_t *addr)
{
    tlkmdi_tinySql_UserSettingSetAddr(MAC_INDEX_TWS_AC_CODE, addr);
}

/**
 * @brief       Reset dongle pairing.
 * @return      none.
 */
void tlkmdi_tinySql_donglePairingReset(void)
{
    uint8_t nullAddr[6];
    tmemset(nullAddr, 0xff, 6);
    tlkmdi_tinySql_setTpdMacAddr(nullAddr);
}

/**
 * @brief       Reset bttpt pairing.
 * @return      none.
 */
void tlkmdi_tinySql_bttptPairingReset(void)
{
    uint8_t nullAddr[6];
    tmemset(nullAddr, 0xff, 6);
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
    tlkmdi_tinySql_userSettingDisk_mutexOperate(true);
    tmemcpy(recBuffer, sTlkmdiSqlUserSettings.btName, TINYSQL_BT_NAME_LENS);
    tlkmdi_tinySql_userSettingDisk_mutexOperate(false);
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
    tlkmdi_tinySql_userSettingDisk_mutexOperate(true);
    tmemcpy(sTlkmdiSqlUserSettings.btName, inBuffer, datalen);
    sTlkmdiSqlUserSettings.btName[datalen] = 0; //str end
    tlkmdi_tinySql_requestSave(tinySql_userSettingsSaveIndex);
    tlkmdi_tinySql_userSettingDisk_mutexOperate(false);
    return TLK_ENONE;
}

/**
 * @brief       Get key configuration information.
 * @param[out]  key_config_info - Pointer to store the key configuration information.
 * @return      none.
 */
void tlkmdi_tinySql_getKeyCofnig(keyConfigs_t **key_config_info)
{
    *key_config_info = &sTlkmdiSqlUserSettings.keyConfigs;
}

/**
 * @brief       Update key configuration information.
 * @param[in]   key_config_info - Pointer to the new key configuration information.
 * @return      none.
 */
void tlkmdi_tinySql_updateKeyCofnig(keyConfigs_t *key_config_info)
{
    if (key_config_info == NULL || key_config_info == &sTlkmdiSqlUserSettings.keyConfigs) {
        return;
    }
    tlkmdi_tinySql_userSettingDisk_mutexOperate(true);
    tmemcpy(&sTlkmdiSqlUserSettings.keyConfigs, key_config_info, sizeof(keyConfigs_t));
    tlkmdi_tinySql_requestSave(tinySql_userSettingsSaveIndex);
    tlkmdi_tinySql_userSettingDisk_mutexOperate(false);
}

#endif

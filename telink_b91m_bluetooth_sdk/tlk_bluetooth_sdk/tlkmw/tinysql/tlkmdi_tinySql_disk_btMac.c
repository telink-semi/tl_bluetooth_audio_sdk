/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_btMac.c
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
_attribute_extend_low_speed_bss_sec_ static uint8_t tlkmdi_tinySql_btMacAddress[6] = {0}; //tpd use bt mac
_attribute_extend_low_speed_bss_sec_ static uint8_t tlkmdi_tinySql_leMacAddress[6] = {0};

/**
 * @brief   Operates the mutex for MAC disk operations.
 * @param[in] isLock - true to lock the mutex, false to unlock.
 * @return  none.
 */
static void tlkmdi_tinySql_macDisk_mutexOperate(uint8_t isLock)
{
    tlkmdi_tinySql_mutex_operate(isLock);
}

/**
 * @brief   Initializes MAC addresses from flash storage.
 * @return  none.
 * @note    Reads BT and LE MAC addresses from flash. If BT MAC is all 0xFF or 0x00, generates a random one.
 */
static void tlkmdi_tinySql_macDiskInit(void)
{
    //get BT MAC from flash,if all 0XFF->random
    unsigned int  macSaveAddress = tlkmdi_tinySql_getSaveAddr(TLKMDI_TINYSQL_DISK2_ADDR);
    unsigned char FFcnt          = 0;
    sql_flash_read(macSaveAddress + 0, tlkmdi_tinySql_leMacAddress, 6);     //le in 0xFF000;
    sql_flash_read(macSaveAddress + 0x100, tlkmdi_tinySql_btMacAddress, 6); //bt in 0xFF100;
    for (int i = 0; i < 6; i++) {
        if (tlkmdi_tinySql_btMacAddress[i] == 0xFF || tlkmdi_tinySql_btMacAddress[i] == 0) {
            FFcnt += 1;
        }
    }
    if (FFcnt == 6) {
        tlkapi_printf(TLKMDI_TINYSQL_LOG_ENABLE, "[SQL]<ERROR> BT/TPD MAC ERROR,NOW RESET!");
        tlkapi_random(tlkmdi_tinySql_btMacAddress, 6);
        tlkmdi_tinySql_requestSave(tinySql_macSaveIndex);
    }
}

/**
 * @brief   Saves MAC addresses to flash storage.
 * @return  none.
 * @note    Erases the sector before writing MAC addresses.
 */
static void tlkmdi_tinySql_macDiskSave(void)
{
    unsigned int macSaveAddress = tlkmdi_tinySql_getSaveAddr(TLKMDI_TINYSQL_DISK2_ADDR);
    sql_flash_read(macSaveAddress + 0, tlkmdi_tinySql_leMacAddress, 6); //le in 0xFF000;
    sql_flash_eraseSector(macSaveAddress);

    sql_flash_write(macSaveAddress, tlkmdi_tinySql_leMacAddress, 6);         //le in 0xFF000;
    sql_flash_write(macSaveAddress + 0x100, tlkmdi_tinySql_btMacAddress, 6); //bt in 0xFF100;
}

/**
 * @brief   Restores MAC addresses to factory defaults.
 * @return  none.
 * @note    MAC addresses are not allowed to be restored to factory values, so this function does nothing.
 */
static void tlkmdi_tinySql_macDiskRestore(void)
{
    //note : mac not allowed restore to factory.
    //need this none function do nothing.
}

/**
 * @brief   MAC disk operations structure.
 * @note    Implements the tinySqlDisk_t interface for MAC address storage.
 */
const tinySqlDisk_t tinySql_mac_disk = {
    .init           = tlkmdi_tinySql_macDiskInit,
    .restoreFactory = tlkmdi_tinySql_macDiskRestore,
    .save           = tlkmdi_tinySql_macDiskSave,
};

/******************************************************************************
                           private code end
******************************************************************************/

/**
 * @brief   Retrieves the MAC address of the Classic Bluetooth device.
 * @param[out] recBuffer The buffer to store the retrieved MAC address of the Classic Bluetooth device.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkmdi_tinySql_getBtMacAddress(uint8_t *recBuffer)
{
    if (recBuffer == NULL) {
        return -TLK_EPARAM;
    }
    tlkmdi_tinySql_macDisk_mutexOperate(true);
    tmemcpy(recBuffer, tlkmdi_tinySql_btMacAddress, 6);
    tlkmdi_tinySql_macDisk_mutexOperate(false);
    return TLK_ENONE;
}

/**
 * @brief   Sets the MAC address of the Classic Bluetooth device.
 * @param[in] inBuffer The buffer containing the new MAC address for the Classic Bluetooth device.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkmdi_tinySql_SetBtMacAddress(uint8_t *inBuffer)
{
    if (inBuffer == NULL) {
        return -TLK_EPARAM;
    }
    tlkmdi_tinySql_macDisk_mutexOperate(true);
    if (tmemcmp(tlkmdi_tinySql_btMacAddress, inBuffer, 6) != 0) {
        tmemcpy(tlkmdi_tinySql_btMacAddress, inBuffer, 6);
        tlkmdi_tinySql_requestSave(tinySql_macSaveIndex);
    }
    tlkmdi_tinySql_macDisk_mutexOperate(false);
    return TLK_ENONE;
}

/**
 * @brief   Retrieves the MAC address of the Low Energy Bluetooth (BLE) device.
 * @param[out] recBuffer The buffer to store the retrieved MAC address of the BLE device.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkmdi_tinySql_getLeMacAddress(uint8_t *recBuffer)
{
    if (recBuffer == NULL) {
        return -TLK_EPARAM;
    }
    tlkmdi_tinySql_macDisk_mutexOperate(true);
    tmemcpy(recBuffer, tlkmdi_tinySql_leMacAddress, 6);
    tlkmdi_tinySql_macDisk_mutexOperate(false);
    return TLK_ENONE;
}

/**
 * @brief   Sets the MAC address of the Low Energy Bluetooth (BLE) device.
 * @param[in] inBuffer The buffer containing the new MAC address for the BLE device.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 */
int tlkmdi_tinySql_setLeMacAddress(uint8_t *inBuffer)
{
    if (inBuffer == NULL) {
        return -TLK_EPARAM;
    }
    tlkmdi_tinySql_macDisk_mutexOperate(true);
    if (tmemcmp(tlkmdi_tinySql_leMacAddress, inBuffer, 6) != 0) {
        tmemcpy(tlkmdi_tinySql_leMacAddress, inBuffer, 6);
        tlkmdi_tinySql_requestSave(tinySql_macSaveIndex);
    }
    tlkmdi_tinySql_macDisk_mutexOperate(false);
    return TLK_ENONE;
}

/**
 * @brief   Retrieves the MAC address of the tpsll slave device.
 * @param[out] recBuffer The buffer to store the retrieved MAC address.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note    Tps shares mac with bt.
 */
int tlkmdi_tinySql_getTpsAddr(uint8_t *recBuffer)
{
    return tlkmdi_tinySql_getBtMacAddress(recBuffer);
}

/**
 * @brief   Retrieves the MAC address of the tpsll dongle device.
 * @param[out] recBuffer The buffer to store the retrieved MAC address.
 * @returns   0 indicates success, other values indicate corresponding error codes.
 * @note    Tpd shares mac with bt.
 */
int tlkmdi_tinySql_getTpdAddr(uint8_t *recBuffer)
{
    return tlkmdi_tinySql_getBtMacAddress(recBuffer);
}
#endif

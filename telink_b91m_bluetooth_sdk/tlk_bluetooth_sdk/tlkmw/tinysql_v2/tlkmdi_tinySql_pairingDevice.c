/********************************************************************************************************
 * @file    tlkmdi_tinySql_pairingDevice.c
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
#include "tlkapi/tlkapi.h"
#include "tlkmdi_tinySql.h"
#include "tlkmdi_tinySql_inner.h"
#include "tlkmw/bt/tlkmdi_btacl.h"
#if (TLK_MW_TINYSQL_V2_ENABLE)
#if (TLK_STK_BT_ENABLE)
/******************************************************************************
                           private code begin
******************************************************************************/

/**
 * @brief Structure for storing RFC channel IDs of pairing device.
 */
typedef struct
{
    uint8_t  hfChannel;        /*!< Hands-free profile channel */
    uint8_t  sppChannel;       /*!< Serial Port Profile channel */
    uint8_t  iapChannel;       /*!< iPod Access Profile channel */
    uint8_t  pbapChannel;      /*!< Phone Book Access Profile channel */
    uint8_t  bipChannel;       /*!< Basic Imaging Profile channel */
    uint8_t  gatt_support : 1; /*!< GATT support flag */
    uint8_t  reserve      : 7; /*!< Reserved bits for alignment */
    uint16_t avrcpCoverArtPsm; /*!< AVRCP cover art PSM */
} pairingDevice_item_RfcId_t;

/**
 * @brief Structure for storing volume settings.
 */
typedef struct
{
    uint8_t isIos; /*!< Flag indicating if device is iOS */
    uint8_t resv;  /*!< Reserved byte */
    uint8_t music; /*!< Music volume level */
    uint8_t voice; /*!< Voice volume level */
} volumesSettings_t;

/**
 * @brief Structure for storing pairing device information.
 */
typedef struct
{
    uint32_t                   devClass;                                 /*!< Device class */
    uint8_t                    linkKey[16];                              /*!< Link key */
    uint8_t                    devAddr[6];                               /*!< Device address */
    uint16_t                   isDevClassSaved : 1;                      /*!< Device class saved flag */
    uint16_t                   isLinkKeySaved  : 1;                      /*!< Link key saved flag */
    uint16_t                   isnameSaved     : 1;                      /*!< Name saved flag */
    uint16_t                   resv13bit       : 13;                     /*!< Reserved bits */
    volumesSettings_t          volume;                                   /*!< Volume settings */
    pairingDevice_item_RfcId_t RfcChId;                                  /*!< RFC channel IDs */
    uint8_t                    devName[TINYSQL_PARING_DEVICE_NAME_LENS]; /*!< Device name */
} pairingDevice_item_t;                                                  //one paring device item size 48

/**
 * @brief Structure for storing all pairing devices.
 */
typedef struct
{
    uint32_t             count;    /*!< Count of pairing devices */
    pairingDevice_item_t items[0]; /*!< Array of pairing device items */
} pairingDevices_t;                //all paring devices;

typedef struct
{
    uint32_t             count;                             /*!< Count of pairing devices */
    pairingDevice_item_t items[TINYSQL_PARING_DEVICE_NUMB]; /*!< Array of pairing device items */
} pairingDevicesMax_t;                                      //max paring devices;

/**
 * @brief       Get pairing device information by item pointer.
 * @param[in]   pItem       - Pointer to pairing device item.
 * @param[out]  devClass    - Device class pointer, can be NULL.
 * @param[out]  pLinkKey    - Link key pointer, can be NULL.
 * @param[out]  pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
static int tlkmdi_tinySql_getPairingDeviceByItemPtr(pairingDevice_item_t *pItem, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName)
{
    if (devClass != NULL) {
        if (pItem->isDevClassSaved == 0) {
            return -TLK_ENODATA;
        }
        *devClass = pItem->devClass;
    }
    if (pLinkKey != NULL) {
        if (pItem->isLinkKeySaved == 0) {
            return -TLK_ENODATA;
        }
        memcpy(pLinkKey, pItem->linkKey, 16);
    }
    if (pDevName != NULL) {
        if (pItem->isnameSaved == 0) {
            return -TLK_ENODATA;
        }
        memcpy(pDevName, pItem->devName, TINYSQL_PARING_DEVICE_NAME_LENS);
    }
    return TLK_ENONE;
}

static void tlkmdi_tinySql_pairingDevicesReset(void)
{
    tlkmdi_tinySql_key_delete("bt_pd");
}

static void tlkmdi_tinySql_pairingDevicesGet(pairingDevicesMax_t *buf)
{
    uint32_t len    = tlkmdi_tinySql_key_get("bt_pd", buf, sizeof(pairingDevicesMax_t));
    uint32_t expect = sizeof(pairingDevices_t) + buf->count * sizeof(pairingDevice_item_t);
    if (len == 0 || len < expect || buf->count > TINYSQL_PARING_DEVICE_NUMB) { //not found or error ,set to default
        pairingDevices_t item = {
            .count = 0,
        };
        tlkmdi_tinySql_key_set_ex("bt_pd", &item, sizeof(pairingDevices_t), 3 * 1000 * 1000);
        memcpy(buf, &item, sizeof(pairingDevices_t));
    }
}

static void tlkmdi_tinySql_pairingDeviceSet(void *buf, uint32_t len)
{
    tlkmdi_tinySql_key_set_ex("bt_pd", buf, len, 3 * 1000 * 1000);
}

const tinySqlDisk_t tinySql_pairingDevice_disk = {
    .restoreFactory = tlkmdi_tinySql_pairingDevicesReset,
};

/******************************************************************************
                           private code end
******************************************************************************/

/**
 * @brief       Get the count of pairing devices.
 * @param[in]   none.
 * @return      The count of pairing devices.
 */
uint32_t tlkmdi_tinySql_getPairingDevicesCount(void)
{
    pairingDevicesMax_t devices;
    tlkmdi_tinySql_pairingDevicesGet(&devices);
    return devices.count;
}

/**
 * @brief       Update pairing device information without thread safety.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   devClass    - Device class pointer, can be NULL.
 * @param[in]   pLinkKey    - Link key pointer, can be NULL.
 * @param[in]   pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 * @note        This function is not thread-safe, should be called with mutex protection.
 */
int tlkmdi_tinySql_updatePairingDeviceThreadUnsafe(uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName)
{
    if (pDevAddr == NULL) {
        return -TLK_EPARAM;
    }
    pairingDevicesMax_t oldDevices;
    pairingDevicesMax_t newDevices;
    tlkmdi_tinySql_pairingDevicesGet(&oldDevices);

    //copy update data.
    pairingDevice_item_t *pItem = &newDevices.items[0];
    memset(pItem, 0, sizeof(pairingDevice_item_t));
    memcpy(pItem->devAddr, pDevAddr, 6);
    if (devClass != NULL) {
        pItem->devClass        = *devClass;
        pItem->isDevClassSaved = 1;
    }
    if (pLinkKey != NULL) {
        memcpy(pItem->linkKey, pLinkKey, 16);
        pItem->isLinkKeySaved = 1;
    }
    if (pDevName != NULL) {
        uint8_t datalen = pDevName[0];
        if (datalen == 0) {
            memset(pItem->devName, 0xFF, TINYSQL_PARING_DEVICE_NAME_LENS);
        } else {
            if (datalen > TINYSQL_PARING_DEVICE_NAME_LENS - 1) {
                datalen = TINYSQL_PARING_DEVICE_NAME_LENS - 1;
            }
            pItem->devName[0] = datalen;
            memcpy(&(pItem->devName[1]), &pDevName[1], datalen);
        }
        pItem->isnameSaved = 1;
    }
    newDevices.count = 1;

    for (size_t i = 0; i < oldDevices.count; i++) {
        if (memcmp(oldDevices.items[i].devAddr, pDevAddr, 6) == 0) {
            continue;
        }
        memcpy(&newDevices.items[newDevices.count], &oldDevices.items[i], sizeof(pairingDevice_item_t));
        newDevices.count++;
        if (newDevices.count == TINYSQL_PARING_DEVICE_NUMB) {
            break;
        }
    }

    tlkmdi_tinySql_pairingDeviceSet(&newDevices, newDevices.count * sizeof(pairingDevice_item_t) + sizeof(pairingDevices_t));
    return TLK_ENONE;
}

/**
 * @brief       Update pairing device information.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   devClass    - Device class pointer, can be NULL.
 * @param[in]   pLinkKey    - Link key pointer, can be NULL.
 * @param[in]   pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_updatePairingDevice(uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName)
{
    tlkmdi_tinySql_mutex_operate(true);
    int res = tlkmdi_tinySql_updatePairingDeviceThreadUnsafe(pDevAddr, devClass, pLinkKey, pDevName);
    tlkmdi_tinySql_mutex_operate(false);
    return res;
}

/**
 * @brief       Set RFC channel ID of pairing device without thread safety.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   val         - Value to set.
 * @param[in]   type        - Type of RFC channel ID.
 * @return      0 if success, otherwise error code.
 * @note        This function is not thread-safe, should be called with mutex protection.
 */
int tlkmdi_tinySql_setPairingDeviceRfcChidThreadUnsafe(uint8_t *pDevAddr, uint16_t val, uint8_t type)
{
    pairingDevice_item_t *pItem = NULL;
    pairingDevicesMax_t   devices;
    tlkmdi_tinySql_pairingDevicesGet(&devices);
    for (size_t i = 0; i < devices.count; i++) {
        if (memcmp(devices.items[i].devAddr, pDevAddr, 6) == 0) {
            pItem = &devices.items[i];
            break;
        }
    }
    if (pItem == NULL) {
        return -TLK_ESEEK;
    }
    if (type == TLKMDI_BT_AVRCP_ArtPsm) {
        if (pItem->RfcChId.avrcpCoverArtPsm == val) {
            return TLK_ENONE; //when same no need save
        }
        pItem->RfcChId.avrcpCoverArtPsm = val;
    } else if (type < TLKMDI_BT_RFC_CHID_MAX) {
        uint8_t *p = (uint8_t *)&(pItem->RfcChId);
        if (*(p + type) == (val & 0xff)) {
            return TLK_ENONE; //when same no need save
        }
        *(p + type) = val & 0xff;
    } else if (type == TLKMDI_BT_RFC_GATT_SUPPORT) {
        uint8_t *p = (uint8_t *)&(pItem->RfcChId);
        *(p + type) |= (val & 0x01);
    }
    tlkmdi_tinySql_pairingDeviceSet(&devices, devices.count * sizeof(pairingDevice_item_t) + sizeof(pairingDevices_t));
    return TLK_ENONE;
}

/**
 * @brief       Set RFC channel ID of pairing device.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   val         - Value to set.
 * @param[in]   type        - Type of RFC channel ID.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_setPairingDeviceRfcChid(uint8_t *pDevAddr, uint16_t val, uint8_t type)
{
    tlkmdi_tinySql_mutex_operate(true);
    int res = tlkmdi_tinySql_setPairingDeviceRfcChidThreadUnsafe(pDevAddr, val, type);
    tlkmdi_tinySql_mutex_operate(false);
    return res;
}

/**
 * @brief       Get RFC channel ID of pairing device.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[out]  val         - Value pointer to store the result.
 * @param[in]   type        - Type of RFC channel ID.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getPairingDeviceRfcChid(uint8_t *pDevAddr, void *val, uint8_t type)
{
    if (val == NULL) {
        return -TLK_EPARAM;
    }
    pairingDevice_item_t *pItem = NULL;
    pairingDevicesMax_t   devices;
    tlkmdi_tinySql_mutex_operate(true);
    tlkmdi_tinySql_pairingDevicesGet(&devices);
    for (size_t i = 0; i < devices.count; i++) {
        if (memcmp(devices.items[i].devAddr, pDevAddr, 6) == 0) {
            pItem = &devices.items[i];
            break;
        }
    }
    if (pItem == NULL) {
        tlkmdi_tinySql_mutex_operate(false);
        return -TLK_ESEEK;
    }
    if (type == TLKMDI_BT_AVRCP_ArtPsm) {
        uint16_t *avrcpCoverArtPsmVal = (uint16_t *)val;
        *avrcpCoverArtPsmVal          = pItem->RfcChId.avrcpCoverArtPsm;
    } else if (type < TLKMDI_BT_RFC_CHID_MAX) {
        uint8_t *rfcChIdVal = (uint8_t *)val;
        uint8_t *p          = (uint8_t *)&(pItem->RfcChId);
        *rfcChIdVal         = *(p + type);
    } else if (type == TLKMDI_BT_RFC_GATT_SUPPORT) {
        uint8_t *rfcChIdVal = (uint8_t *)val;
        uint8_t *p          = (uint8_t *)&(pItem->RfcChId);
        *rfcChIdVal         = *(p + type) & 0x01;
    }
    tlkmdi_tinySql_mutex_operate(false);
    return TLK_ENONE;
}

/**
 * @brief       Set volume setting of pairing device.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   isMusic     - Music volume flag (true for music, false for voice).
 * @param[in]   val         - Volume value to set.
 * @param[in]   isIos       - iOS device flag.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_setPairingDeviceVolume(uint8_t *pDevAddr, uint8_t isMusic, uint8_t val, uint8_t isIos)
{
    tlkmdi_tinySql_mutex_operate(true);
    pairingDevice_item_t *pItem = NULL;
    pairingDevicesMax_t   devices;
    tlkmdi_tinySql_pairingDevicesGet(&devices);
    for (size_t i = 0; i < devices.count; i++) {
        if (memcmp(devices.items[i].devAddr, pDevAddr, 6) == 0) {
            pItem = &devices.items[i];
            break;
        }
    }
    if (pItem == NULL) {
        tlkmdi_tinySql_mutex_operate(false);
        return -TLK_ESEEK;
    }
    pItem->volume.isIos = isIos;
    if (isMusic) {
        pItem->volume.music = val;
    } else {
        pItem->volume.voice = val;
    }
    tlkmdi_tinySql_pairingDeviceSet(&devices, devices.count * sizeof(pairingDevice_item_t) + sizeof(pairingDevices_t));
    tlkmdi_tinySql_mutex_operate(false);
    return TLK_ENONE;
}

/**
 * @brief       Get volume setting of pairing device.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   isMusic     - Music volume flag (true for music, false for voice).
 * @param[out]  val         - Volume value pointer, can be NULL.
 * @param[out]  isIos       - iOS device flag pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getPairingDeviceVolume(uint8_t *pDevAddr, uint8_t isMusic, uint8_t *val, uint8_t *isIos)
{
    tlkmdi_tinySql_mutex_operate(true);
    pairingDevice_item_t *pItem = NULL;
    pairingDevicesMax_t   devices;
    tlkmdi_tinySql_pairingDevicesGet(&devices);
    for (size_t i = 0; i < devices.count; i++) {
        if (memcmp(devices.items[i].devAddr, pDevAddr, 6) == 0) {
            pItem = &devices.items[i];
            break;
        }
    }
    if (pItem == NULL) {
        tlkmdi_tinySql_mutex_operate(false);
        return -TLK_ESEEK;
    }
    if (isIos != NULL) {
        *isIos = pItem->volume.isIos;
    }
    if (val != NULL) {
        if (isMusic) {
            *val = pItem->volume.music;
        } else {
            *val = pItem->volume.voice;
        }
    }
    tlkmdi_tinySql_mutex_operate(false);
    return TLK_ENONE;
}

/**
 * @brief       Delete a pairing device by device address.
 * @param[in]   pDevAddr    - Device address pointer.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_deletePairingDevice(uint8_t *pDevAddr)
{
    if (pDevAddr == NULL) {
        return -TLK_EPARAM;
    }
    tlkmdi_tinySql_mutex_operate(true);
    pairingDevicesMax_t devices;
    tlkmdi_tinySql_pairingDevicesGet(&devices);
    size_t i = 0;
    for (; i < devices.count; i++) {
        if (memcmp(devices.items[i].devAddr, pDevAddr, 6) == 0) {
            break;
        }
    }
    if (i == devices.count) {
        tlkmdi_tinySql_mutex_operate(false);
        return -TLK_ESEEK;
    }
    if (i != devices.count - 1) {
        memmove(&devices.items[i], &devices.items[i + 1], (devices.count - 1 - i) * sizeof(pairingDevice_item_t));
    }

    devices.count -= 1;
    tlkmdi_tinySql_pairingDeviceSet(&devices, devices.count * sizeof(pairingDevice_item_t) + sizeof(pairingDevices_t));
    tlkmdi_tinySql_mutex_operate(false);
    return TLK_ENONE;
}

/**
 * @brief       Clean all pairing devices.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_tinySql_cleanPairingDevices(void)
{
    tlkmdi_tinySql_pairingDevicesReset();
    tlkmdi_tinySql_donglePairingReset();
    tlkmdi_tinySql_bttptPairingReset();
}

/**
 * @brief       Get pairing device information by device address.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[out]  devClass    - Device class pointer, can be NULL.
 * @param[out]  pLinkKey    - Link key pointer, can be NULL.
 * @param[out]  pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getPairingDeviceByAddr(uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName)
{
    if (pDevAddr == NULL) {
        return -TLK_EPARAM;
    }
    tlkmdi_tinySql_mutex_operate(true);
    pairingDevice_item_t *pItem = NULL;
    pairingDevicesMax_t   devices;
    tlkmdi_tinySql_pairingDevicesGet(&devices);
    for (size_t i = 0; i < devices.count; i++) {
        if (memcmp(devices.items[i].devAddr, pDevAddr, 6) == 0) {
            pItem = &devices.items[i];
            break;
        }
    }
    if (pItem == NULL) {
        tlkmdi_tinySql_mutex_operate(false);
        return -TLK_ESEEK;
    }

    int res = tlkmdi_tinySql_getPairingDeviceByItemPtr(pItem, devClass, pLinkKey, pDevName);
    tlkmdi_tinySql_mutex_operate(false);
    return res;
}

/**
 * @brief       Get pairing device information by index.
 * @param[in]   index       - Index of the device.
 * @param[out]  pDevAddr    - Device address pointer, can be NULL.
 * @param[out]  devClass    - Device class pointer, can be NULL.
 * @param[out]  pLinkKey    - Link key pointer, can be NULL.
 * @param[out]  pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getPairingDeviceByIndex(uint32_t index, uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName)
{
    tlkmdi_tinySql_mutex_operate(true);
    pairingDevicesMax_t devices;
    tlkmdi_tinySql_pairingDevicesGet(&devices);

    if (index >= devices.count) {
        tlkmdi_tinySql_mutex_operate(false);
        return -TLK_ESEEK;
    }
    pairingDevice_item_t *pItem = &devices.items[index];
    if (pDevAddr != NULL) {
        memcpy(pDevAddr, pItem->devAddr, 6);
    }
    int res = tlkmdi_tinySql_getPairingDeviceByItemPtr(pItem, devClass, pLinkKey, pDevName);
    tlkmdi_tinySql_mutex_operate(false);
    return res;
}

/**
 * @brief       Get the last paired device information.
 * @param[out]  pDevAddr    - Device address pointer, can be NULL.
 * @param[out]  devClass    - Device class pointer, can be NULL.
 * @param[out]  pLinkKey    - Link key pointer, can be NULL.
 * @param[out]  pDevName    - Device name pointer, can be NULL.
 * @return      0 if success, otherwise error code.
 */
int tlkmdi_tinySql_getLastPairingDevice(uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName)
{
    return tlkmdi_tinySql_getPairingDeviceByIndex(0, pDevAddr, devClass, pLinkKey, pDevName);
}

/**
 * @brief       Print all pairing devices for debugging.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_tinySql_printPairingDevices(void)
{
    pairingDevicesMax_t devices;
    tlkmdi_tinySql_pairingDevicesGet(&devices);
    tlkapi_trace(0xffffffff, "[TinySQL]", "devices: %d", devices.count);
    for (size_t i = 0; i < devices.count; i++) {
        pairingDevice_item_t *pItem = &devices.items[i];
        tlkapi_trace(0xffffffff, "[TinySQL]", "   (index-%d,class-0x%x,addr-0x%x,linkey-0x%x): ", i, pItem->devClass, *(uint32_t *)(pItem->devAddr), *(uint32_t *)(pItem->linkKey));
    }
}
#else
int tlkmdi_tinySql_updatePairingDevice(uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName)
{
    (void)pDevAddr;
    (void)devClass;
    (void)pLinkKey;
    (void)pDevName;
    return -TLK_ENOSUPPORT;
}
#endif
#endif

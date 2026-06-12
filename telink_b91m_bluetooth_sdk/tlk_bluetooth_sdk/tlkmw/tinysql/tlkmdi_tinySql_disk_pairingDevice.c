/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_pairingDevice.c
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
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_inner.h"
#include "tlkmw/bt/tlkmdi_btacl.h"
#if (TLK_MW_TINYSQL_ENABLE)
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
    uint16_t                   nextItemIndex;                            /*!< Point to next item */
    uint16_t                   preItemIndex;                             /*!< Point to previous item */
    uint32_t                   devClass;                                 /*!< Device class */
    uint8_t                    linkKey[16];                              /*!< Link key */
    uint8_t                    devAddr[6];                               /*!< Device address */
    uint16_t                   isUsed          : 1;                      /*!< Used flag */
    uint16_t                   isDevClassSaved : 1;                      /*!< Device class saved flag */
    uint16_t                   isLinkKeySaved  : 1;                      /*!< Link key saved flag */
    uint16_t                   isnameSaved     : 1;                      /*!< Name saved flag */
    uint16_t                   resv12bit       : 12;                     /*!< Reserved bits */
    uint32_t                   magicWordForUser;                         /*!< User magic word */
    volumesSettings_t          volume;                                   /*!< Volume settings */
    pairingDevice_item_RfcId_t RfcChId;                                  /*!< RFC channel IDs */
    uint8_t                    devName[TINYSQL_PARING_DEVICE_NAME_LENS]; /*!< Device name */
} pairingDevice_item_t;                                                  //one paring device item size 48

/**
 * @brief Structure for storing all pairing devices.
 */
typedef struct
{
    uint16_t             headItemIndex;                     /*!< Head of list, point to the most far pairing device */
    uint16_t             lastItemIndex;                     /*!< Tail of list, point to the last pairing device */
    uint32_t             count;                             /*!< Count of pairing devices */
    pairingDevice_item_t items[TINYSQL_PARING_DEVICE_NUMB]; /*!< Array of pairing device items */
} pairingDevices_t;                                         //all paring devices;

static pairingDevices_t   sTlkmdiPairingDevices     = {0};
static tlkapi_save_ctrl_t sTlkmdiPairingDevicesCtrl = {0};

/**
 * @brief       Operate pairing devices mutex lock.
 * @param[in]   isLock    - Lock flag (true to lock, false to unlock).
 * @return      none.
 */
static void tlkmdi_tinySql_pairingDevices_mutexOperate(uint8_t isLock)
{
    tlkmdi_tinySql_mutex_operate(isLock);
}

/**
 * @brief       Check validity of pairing devices data.
 * @param[in]   none.
 * @return      true if valid, false if invalid.
 */
static bool tlkmdi_tinySql_PairingDevicesCheck(void)
{
    if (sTlkmdiPairingDevices.count > TINYSQL_PARING_DEVICE_NUMB) {
        return false;
    }
    uint32_t count     = 0;
    uint16_t itemIndex = sTlkmdiPairingDevices.headItemIndex;
    while (itemIndex != tinySql_nullptr) {
        if (itemIndex >= TINYSQL_PARING_DEVICE_NUMB) {
            return false;
        }
        pairingDevice_item_t *pItem = &sTlkmdiPairingDevices.items[itemIndex];
        itemIndex                   = pItem->nextItemIndex;
        count++;
        if (count > sTlkmdiPairingDevices.count) {
            return false;
        }
    }
    count     = 0;
    itemIndex = sTlkmdiPairingDevices.lastItemIndex;
    while (itemIndex != tinySql_nullptr) {
        if (itemIndex >= TINYSQL_PARING_DEVICE_NUMB) {
            return false;
        }
        pairingDevice_item_t *pItem = &sTlkmdiPairingDevices.items[itemIndex];
        itemIndex                   = pItem->preItemIndex;
        count++;
        if (count > sTlkmdiPairingDevices.count) {
            return false;
        }
    }
    return true;
}

/**
 * @brief       Reset pairing devices data.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmdi_tinySql_PairingDevicesReset(void)
{
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    memset(&sTlkmdiPairingDevices, 0, sizeof(pairingDevices_t));
    sTlkmdiPairingDevices.headItemIndex = tinySql_nullptr;
    sTlkmdiPairingDevices.lastItemIndex = tinySql_nullptr;
    tlkmdi_tinySql_requestSave(tinySql_pairingDevicesSaveIndex);
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
}

//PD = PairingDevice, Only in private code(static) use this abbreviated name
/**
 * @brief       Get pairing device index by device address.
 * @param[in]   pDevAddr    - Device address pointer.
 * @return      Index of the device, or tinySql_notFind if not found.
 */
static uint16_t tlkmdi_tinySql_getPDIndex(uint8_t *pDevAddr)
{
    uint16_t itemIndex = sTlkmdiPairingDevices.headItemIndex;
    while (itemIndex != tinySql_nullptr) {
        pairingDevice_item_t *pItem = &sTlkmdiPairingDevices.items[itemIndex];
        if (tmemcmp(pItem->devAddr, pDevAddr, 6) == 0) {
            return itemIndex;
        }
        itemIndex = pItem->nextItemIndex;
    }
    return tinySql_notFind;
}

/**
 * @brief       Get idle pairing device index.
 * @param[in]   none.
 * @return      Index of idle device, or tinySql_full if all devices are used.
 */
static uint16_t tlkmdi_tinySql_getIdlePDIndex(void)
{
    for (uint16_t index = 0; index < TINYSQL_PARING_DEVICE_NUMB; index++) {
        pairingDevice_item_t *pItem = &sTlkmdiPairingDevices.items[index];
        if (pItem->isUsed == false) {
            return index;
        }
    }
    return tinySql_full;
}

//PD = PairingDevice, Only in private code(static) use this abbreviated name
/**
 * @brief       Check if pairing device is same as given parameters.
 * @param[in]   itemIndex   - Index of the device item.
 * @param[in]   pDevAddr    - Device address pointer.
 * @param[in]   devClass    - Device class pointer, can be NULL.
 * @param[in]   pLinkKey    - Link key pointer, can be NULL.
 * @param[in]   pDevName    - Device name pointer, can be NULL.
 * @return      true if same, false if different.
 */
static inline bool tlkmdi_tinySql_isPDSame(uint16_t itemIndex, uint8_t *pDevAddr, uint32_t *devClass, uint8_t *pLinkKey, uint8_t *pDevName)
{
    pairingDevice_item_t *items = sTlkmdiPairingDevices.items;

    if (tmemcmp(pDevAddr, items[itemIndex].devAddr, 6) != 0) {
        return false;
    }
    if (devClass != NULL && *devClass != items[itemIndex].devClass) {
        return false;
    }
    if (pLinkKey != NULL && tmemcmp(pLinkKey, items[itemIndex].linkKey, 16) != 0) {
        return false;
    }
    if (pDevName != NULL) {
        uint8_t datalen = pDevName[0];
        if (datalen > TINYSQL_PARING_DEVICE_NAME_LENS - 1) {
            datalen = TINYSQL_PARING_DEVICE_NAME_LENS - 1;
        }
        if (datalen == 0) {
            for (int i = 0; i < TINYSQL_PARING_DEVICE_NAME_LENS; i++) {
                if (items[itemIndex].devName[i] != 0xFF) {
                    return false;
                }
            }
        } else {
            if (items[itemIndex].devName[0] != datalen) {
                return false;
            }
            if (tmemcmp(pDevName + 1, items[itemIndex].devName + 1, datalen) != 0) {
                return false;
            }
        }
    }
    return true;
}

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
        tmemcpy(pLinkKey, pItem->linkKey, 16);
    }
    if (pDevName != NULL) {
        if (pItem->isnameSaved == 0) {
            return -TLK_ENODATA;
        }
        tmemcpy(pDevName, pItem->devName, TINYSQL_PARING_DEVICE_NAME_LENS);
    }
    return TLK_ENONE;
}

/**
 * @brief       Initialize pairing device disk storage.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmdi_tinySql_pairingDeviceDiskInit(void)
{
    //get airingDevices data from flash,if nodata/not right version/crc fail -> reset data
    unsigned int saveAddress = tlkmdi_tinySql_getSaveAddr(TLKMDI_TINYSQL_DISK1_ADDR);
    tlkapi_save3_init(&sTlkmdiPairingDevicesCtrl, TLKMDI_TINYSQL_SAVE_SIGN, TLKMDI_TINYSQL_VER, sizeof(pairingDevices_t), saveAddress);
    int ret = tlkapi_save3_load(&sTlkmdiPairingDevicesCtrl, (uint8_t *)&sTlkmdiPairingDevices, sizeof(sTlkmdiPairingDevices));
    if (ret < (int)sizeof(pairingDevices_t) || tlkmdi_tinySql_PairingDevicesCheck() == false) {
        tlkmdi_tinySql_PairingDevicesReset();
    }
}

/**
 * @brief       Save pairing device data to disk.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmdi_tinySql_pairingDeviceDiskSave(void)
{
    tlkapi_save3_smartSave(&sTlkmdiPairingDevicesCtrl, (uint8_t *)&sTlkmdiPairingDevices, sizeof(sTlkmdiPairingDevices));
}

/**
 * @brief       Restore pairing device data to factory defaults.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmdi_tinySql_pairingDeviceDiskRestore(void)
{
    tlkapi_save3_clean(&sTlkmdiPairingDevicesCtrl);
    tlkmdi_tinySql_PairingDevicesReset();
    tlkmdi_tinySql_requestSave(tinySql_pairingDevicesSaveIndex);
}

const tinySqlDisk_t tinySql_pairingDevice_disk = {
    .init           = tlkmdi_tinySql_pairingDeviceDiskInit,
    .restoreFactory = tlkmdi_tinySql_pairingDeviceDiskRestore,
    .save           = tlkmdi_tinySql_pairingDeviceDiskSave,
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
    return sTlkmdiPairingDevices.count;
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
    pairingDevice_item_t *items = sTlkmdiPairingDevices.items;
    //itemIndex used as a data ptr
    uint16_t itemIndex = tlkmdi_tinySql_getPDIndex(pDevAddr);
    if (itemIndex == tinySql_notFind) {                                        //not find in the list
        itemIndex = tlkmdi_tinySql_getIdlePDIndex();                           //new one
        if (itemIndex == tinySql_full) {                                       //devices are full ,so 'new' failed
            itemIndex                   = sTlkmdiPairingDevices.headItemIndex; //take off the head point
            pairingDevice_item_t *pItem = &items[itemIndex];
            pItem->isDevClassSaved      = 0;
            pItem->isLinkKeySaved       = 0;
            pItem->isnameSaved          = 0;
        }
    } //By above steps,we can get a itemIndex(ptr).
    uint16_t tail = sTlkmdiPairingDevices.lastItemIndex;
    uint16_t head = sTlkmdiPairingDevices.headItemIndex;
    if (items[itemIndex].isUsed == false) { //malloc a new node
        if (head == tinySql_nullptr) {      //the list is empty
            sTlkmdiPairingDevices.headItemIndex = itemIndex;
        } else {
            items[tail].nextItemIndex = itemIndex;
        }
        items[itemIndex].preItemIndex       = tail;            //new node->pre is old list->tail
        items[itemIndex].nextItemIndex      = tinySql_nullptr; //new node->next is NULL
        sTlkmdiPairingDevices.lastItemIndex = itemIndex;       //list->tail now is new node
        items[itemIndex].isUsed             = true;
        items[itemIndex].volume.music       = TLKBTP_A2DP_DEFAULT_VOL;
        items[itemIndex].volume.voice       = TLKBTP_HFP_DEFAULT_VOL;
        sTlkmdiPairingDevices.count += 1;
    } else { //replace one node
        if (tail == itemIndex) {
            //if is tail node,no need to remove and re_insert
            if (tlkmdi_tinySql_isPDSame(itemIndex, pDevAddr, devClass, pLinkKey, pDevName) == true) { //same
                return TLK_ENONE;                                                                     //no need save
            }
            //check if same as past
        } else {
            if (head != itemIndex) {
                items[items[itemIndex].preItemIndex].nextItemIndex = items[itemIndex].nextItemIndex;
                //nownode->pre->next = nownode->next
            } else {
                sTlkmdiPairingDevices.headItemIndex = items[itemIndex].nextItemIndex;
                //when now node is head,we need update the headptr extrally
            }
            items[items[itemIndex].nextItemIndex].preItemIndex = items[itemIndex].preItemIndex;
            //nownode->next->pre = nownode->pre

            //now we remove the node from list,we need re_insert it to the tail of list

            items[tail].nextItemIndex           = itemIndex;
            items[itemIndex].preItemIndex       = tail;            //new node->pre is old list->tail
            items[itemIndex].nextItemIndex      = tinySql_nullptr; //new node->next is NULL
            sTlkmdiPairingDevices.lastItemIndex = itemIndex;       //list->tail now is new node
        }
    }
    //final: we copy data
    pairingDevice_item_t *pItem = &items[itemIndex];
    tmemcpy(pItem->devAddr, pDevAddr, 6);
    if (devClass != NULL) {
        pItem->devClass        = *devClass;
        pItem->isDevClassSaved = 1;
    }
    if (pLinkKey != NULL) {
        tmemcpy(pItem->linkKey, pLinkKey, 16);
        pItem->isLinkKeySaved = 1;
    }
    if (pDevName != NULL) {
        uint8_t datalen = pDevName[0];
        if (datalen == 0) {
            STATIC_ASSERT_THIS_FILE(IS_4BYTE_ALIGN(TINYSQL_PARING_DEVICE_NAME_LENS));
            memset(pItem->devName, 0xFFFFFFFF, TINYSQL_PARING_DEVICE_NAME_LENS);
        } else {
            if (datalen > TINYSQL_PARING_DEVICE_NAME_LENS - 1) {
                datalen = TINYSQL_PARING_DEVICE_NAME_LENS - 1;
            }
            pItem->devName[0] = datalen;
            tmemcpy(&(pItem->devName[1]), &pDevName[1], datalen);
        }
        pItem->isnameSaved = 1;
    }
    tlkmdi_tinySql_requestSave(tinySql_pairingDevicesSaveIndex);
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
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    int res = tlkmdi_tinySql_updatePairingDeviceThreadUnsafe(pDevAddr, devClass, pLinkKey, pDevName);
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
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
    uint16_t itemIndex = tlkmdi_tinySql_getPDIndex(pDevAddr);
    if (itemIndex == tinySql_notFind) { //not find in the list
        return -TLK_ESEEK;
    }
    if (type == TLKMDI_BT_AVRCP_ArtPsm) {
        if (sTlkmdiPairingDevices.items[itemIndex].RfcChId.avrcpCoverArtPsm == val) {
            return TLK_ENONE; //when same no need save
        }
        sTlkmdiPairingDevices.items[itemIndex].RfcChId.avrcpCoverArtPsm = val;
    } else if (type < TLKMDI_BT_RFC_CHID_MAX) {
        uint8_t *p = (uint8_t *)&sTlkmdiPairingDevices.items[itemIndex].RfcChId;
        if (*(p + type) == (val & 0xff)) {
            return TLK_ENONE; //when same no need save
        }
        *(p + type) = val & 0xff;
    } else if (type == TLKMDI_BT_RFC_GATT_SUPPORT) {
        uint8_t *p = (uint8_t *)&sTlkmdiPairingDevices.items[itemIndex].RfcChId;
        *(p + type) |= (val & 0x01);
    }
    tlkmdi_tinySql_requestSave(tinySql_pairingDevicesSaveIndex);
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
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    int res = tlkmdi_tinySql_setPairingDeviceRfcChidThreadUnsafe(pDevAddr, val, type);
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
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
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    uint16_t itemIndex = tlkmdi_tinySql_getPDIndex(pDevAddr);
    if (itemIndex == tinySql_notFind) { //not find in the list
        tlkmdi_tinySql_pairingDevices_mutexOperate(false);
        return -TLK_ESEEK;
    }
    if (type == TLKMDI_BT_AVRCP_ArtPsm) {
        uint16_t *avrcpCoverArtPsmVal = (uint16_t *)val;
        *avrcpCoverArtPsmVal          = sTlkmdiPairingDevices.items[itemIndex].RfcChId.avrcpCoverArtPsm;
    } else if (type < TLKMDI_BT_RFC_CHID_MAX) {
        uint8_t *rfcChIdVal = (uint8_t *)val;
        uint8_t *p          = (uint8_t *)&sTlkmdiPairingDevices.items[itemIndex].RfcChId;
        *rfcChIdVal         = *(p + type);
    } else if (type == TLKMDI_BT_RFC_GATT_SUPPORT) {
        uint8_t *rfcChIdVal = (uint8_t *)val;
        uint8_t *p          = (uint8_t *)&sTlkmdiPairingDevices.items[itemIndex].RfcChId;
        *rfcChIdVal         = *(p + type) & 0x01;
    }
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
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
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    uint16_t itemIndex = tlkmdi_tinySql_getPDIndex(pDevAddr);
    if (itemIndex == tinySql_notFind) { //not find in the list
        tlkmdi_tinySql_pairingDevices_mutexOperate(false);
        return -TLK_ESEEK;
    }
    sTlkmdiPairingDevices.items[itemIndex].volume.isIos = isIos;
    if (isMusic) {
        sTlkmdiPairingDevices.items[itemIndex].volume.music = val;
    } else {
        sTlkmdiPairingDevices.items[itemIndex].volume.voice = val;
    }
    tlkmdi_tinySql_requestSave(tinySql_pairingDevicesSaveIndex);
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
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
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    uint16_t itemIndex = tlkmdi_tinySql_getPDIndex(pDevAddr);
    if (itemIndex == tinySql_notFind) { //not find in the list
        tlkmdi_tinySql_pairingDevices_mutexOperate(false);
        return -TLK_ESEEK;
    }
    if (isIos != NULL) {
        *isIos = sTlkmdiPairingDevices.items[itemIndex].volume.isIos;
    }
    if (val != NULL) {
        if (isMusic) {
            *val = sTlkmdiPairingDevices.items[itemIndex].volume.music;
        } else {
            *val = sTlkmdiPairingDevices.items[itemIndex].volume.voice;
        }
    }
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
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
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    pairingDevice_item_t *items     = sTlkmdiPairingDevices.items;
    uint16_t              itemIndex = tlkmdi_tinySql_getPDIndex(pDevAddr);
    if (itemIndex == tinySql_notFind) {
        tlkmdi_tinySql_pairingDevices_mutexOperate(false);
        return -TLK_ESEEK;
    }
    if (items[itemIndex].preItemIndex != tinySql_nullptr) { //if not the head node
        items[items[itemIndex].preItemIndex].nextItemIndex = items[itemIndex].nextItemIndex;
        //nownode->pre->next = nownode->next
    } else {
        sTlkmdiPairingDevices.headItemIndex = items[itemIndex].nextItemIndex;
        //when now node is head,we need update the headptr extrally
    }
    if (items[itemIndex].nextItemIndex != tinySql_nullptr) { //if not the tail node
        items[items[itemIndex].nextItemIndex].preItemIndex = items[itemIndex].preItemIndex;
        //nownode->next->pre = nownode->pre
    } else {
        sTlkmdiPairingDevices.lastItemIndex = items[itemIndex].preItemIndex;
        //when now node is tail,we need update the tailptr extrally
    }
    memset(&items[itemIndex], 0, sizeof(pairingDevice_item_t));
    sTlkmdiPairingDevices.count -= 1;
    tlkmdi_tinySql_requestSave(tinySql_pairingDevicesSaveIndex);
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
    return TLK_ENONE;
}

/**
 * @brief       Clean all pairing devices.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_tinySql_cleanPairingDevices(void)
{
    tlkmdi_tinySql_PairingDevicesReset();
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
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    uint16_t itemIndex = tlkmdi_tinySql_getPDIndex(pDevAddr);
    if (itemIndex == tinySql_notFind) { //not find in the list
        tlkmdi_tinySql_pairingDevices_mutexOperate(false);
        return -TLK_ESEEK;
    }
    pairingDevice_item_t *pItem = &(sTlkmdiPairingDevices.items[itemIndex]);
    int                   res   = tlkmdi_tinySql_getPairingDeviceByItemPtr(pItem, devClass, pLinkKey, pDevName);
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
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
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    if (index >= sTlkmdiPairingDevices.count) {
        tlkmdi_tinySql_pairingDevices_mutexOperate(false);
        return -TLK_ESEEK;
    }
    uint32_t itemIndex = sTlkmdiPairingDevices.headItemIndex;
    while (itemIndex != tinySql_nullptr && index != 0) {
        pairingDevice_item_t *pItem = &sTlkmdiPairingDevices.items[itemIndex];
        itemIndex                   = pItem->nextItemIndex;
        index -= 1;
    }
    if (itemIndex == tinySql_nullptr) {
        tlkmdi_tinySql_pairingDevices_mutexOperate(false);
        return -TLK_ESEEK;
    }
    pairingDevice_item_t *pItem = &(sTlkmdiPairingDevices.items[itemIndex]);
    if (pDevAddr != NULL) {
        tmemcpy(pDevAddr, pItem->devAddr, 6);
    }
    int res = tlkmdi_tinySql_getPairingDeviceByItemPtr(pItem, devClass, pLinkKey, pDevName);
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
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
    uint32_t lastindex = sTlkmdiPairingDevices.count - 1;
    //if count == 0,lastindex will become 0xffffffff,then return -TLK_ESEEK;
    return tlkmdi_tinySql_getPairingDeviceByIndex(lastindex, pDevAddr, devClass, pLinkKey, pDevName);
}

/**
 * @brief       Print all pairing devices for debugging.
 * @param[in]   none.
 * @return      none.
 */
void tlkmdi_tinySql_printPairingDevices(void)
{
    tlkmdi_tinySql_pairingDevices_mutexOperate(true);
    uint16_t itemIndex = sTlkmdiPairingDevices.headItemIndex;
    tlkapi_trace(0xffffffff, "[TinySQL]", "devices: %d", sTlkmdiPairingDevices.count);
    while (itemIndex != tinySql_nullptr) {
        pairingDevice_item_t *pItem = &sTlkmdiPairingDevices.items[itemIndex];
        tlkapi_trace(0xffffffff, "[TinySQL]", "   (index-%d,class-0x%x,addr-0x%x,linkey-0x%x): ", itemIndex, pItem->devClass, *(uint32_t *)(pItem->devAddr),
                     *(uint32_t *)(pItem->linkKey));
        itemIndex = pItem->nextItemIndex;
    }
    tlkmdi_tinySql_pairingDevices_mutexOperate(false);
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

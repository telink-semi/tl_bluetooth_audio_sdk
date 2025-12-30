/********************************************************************************************************
 * @file    tlkusb_module.c
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
#include "tlklib/usb/tlkusb_stdio.h"
#if (TLK_CFG_USB_ENABLE)
#include "tlklib/usb/tlkusb_desc.h"
#include "tlklib/usb/tlkusb_module.h"

#if (TLK_USB_UDB_ENABLE)
extern const tlkusb_module_t gTlkUsbUdbModule;
#endif
#if (TLK_USB_UAC_ENABLE)
extern const tlkusb_module_t gTlkUsbUacModule;
#endif
#if (TLK_USB_MSC_ENABLE)
extern const tlkusb_module_t gTlkUsbMscModule;
#endif


static const tlkusb_module_t *sTlkUsbModule[TLKUSB_MODTYPE_MAX] = {
#if (TLK_USB_UDB_ENABLE)
    [TLKUSB_MODTYPE_UDB] = &gTlkUsbUdbModule,
#endif
#if (TLK_USB_UAC_ENABLE)
    [TLKUSB_MODTYPE_UAC] = &gTlkUsbUacModule,
#endif
#if (TLK_USB_MSC_ENABLE)
    [TLKUSB_MODTYPE_MSC] = &gTlkUsbMscModule,
#endif
};


/**
 * @brief       This function initializes the USB module of the specified type.
 * @param[in]   modType  - the USB module type to initialize.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_init(uint8_t modType)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return -TLK_EPARAM;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->Init == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sTlkUsbModule[modType]->pCtrl->Init();
}

/**
 * @brief       This function resets the USB module of the specified type.
 * @param[in]   modType  - the USB module type to reset.
 * @returns     none.
 */
void tlkusb_module_reset(uint8_t modType)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->Reset == NULL) {
        return;
    }
    sTlkUsbModule[modType]->pCtrl->Reset();
}

/**
 * @brief       This function deinitializes the USB module of the specified type.
 * @param[in]   modType  - the USB module type to deinitialize.
 * @returns     none.
 */
void tlkusb_module_deinit(uint8_t modType)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->Deinit == NULL) {
        return;
    }
    sTlkUsbModule[modType]->pCtrl->Deinit();
}

/**
 * @brief       This function handles events for the USB module of the specified type.
 * @param[in]   modType  - the USB module type to handle.
 * @returns     none.
 */
void tlkusb_module_handler(uint8_t modType)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->Handler == NULL) {
        return;
    }
    sTlkUsbModule[modType]->pCtrl->Handler();
}

/**
 * @brief       This function gets class information for the specified interface of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   infNumb   - the interface number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_getClassInf(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return -TLK_EPARAM;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->GetClassInf == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sTlkUsbModule[modType]->pCtrl->GetClassInf(pSetup, infNumb);
}

/**
 * @brief       This function sets class information for the specified interface of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   infNumb   - the interface number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_setClassInf(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return -TLK_EPARAM;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->SetClassInf == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sTlkUsbModule[modType]->pCtrl->SetClassInf(pSetup, infNumb);
}

/**
 * @brief       This function gets class information for the specified endpoint of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   edpNumb   - the endpoint number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_getClassEdp(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t edpNumb)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return -TLK_EPARAM;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->GetClassEdp == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sTlkUsbModule[modType]->pCtrl->GetClassEdp(pSetup, edpNumb);
}

/**
 * @brief       This function sets class information for the specified endpoint of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   edpNumb   - the endpoint number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_setClassEdp(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t edpNumb)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return -TLK_EPARAM;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->SetClassEdp == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sTlkUsbModule[modType]->pCtrl->SetClassEdp(pSetup, edpNumb);
}

/**
 * @brief       This function gets interface information of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   infNumb   - the interface number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_getInterface(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return -TLK_EPARAM;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->GetInterface == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sTlkUsbModule[modType]->pCtrl->GetInterface(pSetup, infNumb);
}

/**
 * @brief       This function sets interface of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   infNumb   - the interface number.
 * @returns     TLK_ENONE is success, otherwise is failure.
 */
int tlkusb_module_setInterface(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return -TLK_EPARAM;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pCtrl == NULL || sTlkUsbModule[modType]->pCtrl->SetInterface == NULL) {
        return -TLK_ENOSUPPORT;
    }
    return sTlkUsbModule[modType]->pCtrl->SetInterface(pSetup, infNumb);
}

/**
 * @brief       This function gets the length of the device descriptor of the USB module.
 * @param[in]   modType   - the USB module type.
 * @returns     the length of the device descriptor, or 0 if module is invalid or not supported.
 */
uint16_t tlkusb_module_getDeviceLens(uint8_t modType)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return 0;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pDesc == NULL || sTlkUsbModule[modType]->pDesc->GetDeviceLens == NULL) {
        return 0;
    }
    return sTlkUsbModule[modType]->pDesc->GetDeviceLens();
}

/**
 * @brief       This function gets the length of the configuration descriptor of the USB module.
 * @param[in]   modType   - the USB module type.
 * @returns     the length of the configuration descriptor, or 0 if module is invalid or not supported.
 */
uint16_t tlkusb_module_getConfigLens(uint8_t modType)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return 0;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pDesc == NULL || sTlkUsbModule[modType]->pDesc->GetConfigLens == NULL) {
        return 0;
    }
    return sTlkUsbModule[modType]->pDesc->GetConfigLens();
}

/**
 * @brief       This function gets the length of the string descriptor of the USB module.
 * @param       modType  - the USB module type.
 * @param       index    - the string descriptor index.
 * @returns     the length of the string descriptor, or 0 if module is invalid or not supported.
 */
uint16_t tlkusb_module_getStringLens(uint8_t modType, uint8_t index)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return 0;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pDesc == NULL || sTlkUsbModule[modType]->pDesc->GetStringLens == NULL) {
        return 0;
    }
    return sTlkUsbModule[modType]->pDesc->GetStringLens(index);
}

/**
 * @brief       This function gets the length of the interface descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @param       pSetup    - pointer to the USB setup request.
 * @returns     the length of the interface descriptor, or 0 if module is invalid or not supported.
 */
uint16_t tlkusb_module_getInfDesLens(uint8_t modType, tlkusb_setup_req_t *pSetup)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return 0;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pDesc == NULL || sTlkUsbModule[modType]->pDesc->GetInfDesLens == NULL) {
        return 0;
    }
    return sTlkUsbModule[modType]->pDesc->GetInfDesLens(pSetup);
}

/**
 * @brief       This function gets the device descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @returns     pointer to the device descriptor, or NULL if module is invalid or not supported.
 */
uint8_t *tlkusb_module_getDeviceDesc(uint8_t modType)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return NULL;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pDesc == NULL || sTlkUsbModule[modType]->pDesc->GetDeviceDesc == NULL) {
        return NULL;
    }
    return sTlkUsbModule[modType]->pDesc->GetDeviceDesc();
}

/**
 * @brief       This function gets the configuration descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @returns     pointer to the configuration descriptor, or NULL if module is invalid or not supported.
 */
uint8_t *tlkusb_module_getConfigDesc(uint8_t modType)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return NULL;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pDesc == NULL || sTlkUsbModule[modType]->pDesc->GetConfigDesc == NULL) {
        return NULL;
    }
    return sTlkUsbModule[modType]->pDesc->GetConfigDesc();
}

/**
 * @brief       This function gets the string descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @param       index     - the string descriptor index.
 * @returns     pointer to the string descriptor, or NULL if module is invalid or not supported.
 */
uint8_t *tlkusb_module_getStringDesc(uint8_t modType, uint8_t index)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return NULL;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pDesc == NULL || sTlkUsbModule[modType]->pDesc->GetStringDesc == NULL) {
        return NULL;
    }
    return sTlkUsbModule[modType]->pDesc->GetStringDesc(index);
}

/**
 * @brief       This function gets the interface descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @param       pSetup    - pointer to the USB setup request.
 * @returns     pointer to the interface descriptor, or NULL if module is invalid or not supported.
 */
uint8_t *tlkusb_module_getInfDesDesc(uint8_t modType, tlkusb_setup_req_t *pSetup)
{
    if (modType >= TLKUSB_MODTYPE_MAX) {
        return NULL;
    }
    if (sTlkUsbModule[modType] == NULL || sTlkUsbModule[modType]->pDesc == NULL || sTlkUsbModule[modType]->pDesc->GetInfDesDesc == NULL) {
        return NULL;
    }
    return sTlkUsbModule[modType]->pDesc->GetInfDesDesc(pSetup);
}


#endif

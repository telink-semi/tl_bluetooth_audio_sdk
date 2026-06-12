/********************************************************************************************************
 * @file    tlkusb_module.h
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
#ifndef TLKUSB_MODULE_H
#define TLKUSB_MODULE_H


#define TLKUSB_MODULE_UINT08_PTR uint8_t *

typedef struct
{
    int (*Init)(void);                                                // Init
    void (*Reset)(void);                                              // Reset
    void (*Deinit)(void);                                             // Deinit
    void (*Handler)(void);                                            // Handler
    int (*GetClassInf)(tlkusb_setup_req_t *pSetup, uint8_t infNumb);  // GetClassInf
    int (*SetClassInf)(tlkusb_setup_req_t *pSetup, uint8_t infNumb);  // SetClassInf
    int (*GetClassEdp)(tlkusb_setup_req_t *pSetup, uint8_t edpNumb);  // GetClassEdp
    int (*SetClassEdp)(tlkusb_setup_req_t *pSetup, uint8_t edpNumb);  // SetClassEdp
    int (*GetInterface)(tlkusb_setup_req_t *pSetup, uint8_t infNumb); // GetInterface
    int (*SetInterface)(tlkusb_setup_req_t *pSetup, uint8_t infNumb); // SetInterface
} tlkusb_modCtrl_t;

typedef struct
{
    uint16_t (*GetDeviceLens)(void);                       // GetDeviceLens
    uint16_t (*GetConfigLens)(void);                       // GetConfigLens
    uint16_t (*GetStringLens)(uint8_t index);              // GetStringLens
    uint16_t (*GetInfDesLens)(tlkusb_setup_req_t *pSetup); // GetInterfaceLens
    TLKUSB_MODULE_UINT08_PTR(*GetDeviceDesc)
    (void); // Get device descriptor
    TLKUSB_MODULE_UINT08_PTR(*GetConfigDesc)
    (void); // Get config descriptor
    TLKUSB_MODULE_UINT08_PTR(*GetStringDesc)
    (uint8_t index); // Get Serial String Descriptor
    TLKUSB_MODULE_UINT08_PTR(*GetInfDesDesc)
    (tlkusb_setup_req_t *pSetup); // Get interface descriptor
} tlkusb_modDesc_t;

typedef struct
{
    uint8_t                 modType;
    uint8_t                 resv[3];
    const tlkusb_modDesc_t *pDesc;
    const tlkusb_modCtrl_t *pCtrl;
} tlkusb_module_t;

/**
 * @brief       This function initializes the USB module of the specified type.
 * @param[in]   modType  - the USB module type to initialize.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_init(uint8_t modType);

/**
 * @brief       This function resets the USB module of the specified type.
 * @param[in]   modType  - the USB module type to reset.
 * @returns     none.
 */
void tlkusb_module_reset(uint8_t modType);

/**
 * @brief       This function deinitializes the USB module of the specified type.
 * @param[in]   modType  - the USB module type to deinitialize.
 * @returns     none.
 */
void tlkusb_module_deinit(uint8_t modType);

/**
 * @brief       This function handles events for the USB module of the specified type.
 * @param[in]   modType  - the USB module type to handle.
 * @returns     none.
 */
void tlkusb_module_handler(uint8_t modType);

/**
 * @brief       This function gets class information for the specified interface of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   infNumb   - the interface number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_getClassInf(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t infNumb);

/**
 * @brief       This function sets class information for the specified interface of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   infNumb   - the interface number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_setClassInf(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t infNumb);

/**
 * @brief       This function gets class information for the specified endpoint of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   edpNumb   - the endpoint number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_getClassEdp(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t edpNumb);

/**
 * @brief       This function sets class information for the specified endpoint of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   edpNumb   - the endpoint number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_setClassEdp(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t edpNumb);

/**
 * @brief       This function gets interface information of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   infNumb   - the interface number.
 * @returns     TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_module_getInterface(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t infNumb);

/**
 * @brief       This function sets interface of the USB module.
 * @param[in]   modType   - the USB module type.
 * @param[in]   pSetup    - pointer to the USB setup request.
 * @param[in]   infNumb   - the interface number.
 * @returns     TLK_ENONE is success, otherwise is failure.
 */
int tlkusb_module_setInterface(uint8_t modType, tlkusb_setup_req_t *pSetup, uint8_t infNumb);

/**
 * @brief       This function gets the length of the device descriptor of the USB module.
 * @param[in]   modType   - the USB module type.
 * @returns     the length of the device descriptor, or 0 if module is invalid or not supported.
 */
uint16_t tlkusb_module_getDeviceLens(uint8_t modType);

/**
 * @brief       This function gets the length of the configuration descriptor of the USB module.
 * @param[in]   modType   - the USB module type.
 * @returns     the length of the configuration descriptor, or 0 if module is invalid or not supported.
 */
uint16_t tlkusb_module_getConfigLens(uint8_t modType);

/**
 * @brief       This function gets the length of the string descriptor of the USB module.
 * @param       modType  - the USB module type.
 * @param       index    - the string descriptor index.
 * @returns     the length of the string descriptor, or 0 if module is invalid or not supported.
 */
uint16_t tlkusb_module_getStringLens(uint8_t modType, uint8_t index);

/**
 * @brief       This function gets the length of the interface descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @param       pSetup    - pointer to the USB setup request.
 * @returns     the length of the interface descriptor, or 0 if module is invalid or not supported.
 */
uint16_t tlkusb_module_getInfDesLens(uint8_t modType, tlkusb_setup_req_t *pSetup);

/**
 * @brief       This function gets the device descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @returns     pointer to the device descriptor, or NULL if module is invalid or not supported.
 */
uint8_t *tlkusb_module_getDeviceDesc(uint8_t modType);

/**
 * @brief       This function gets the configuration descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @returns     pointer to the configuration descriptor, or NULL if module is invalid or not supported.
 */
uint8_t *tlkusb_module_getConfigDesc(uint8_t modType);

/**
 * @brief       This function gets the string descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @param       index     - the string descriptor index.
 * @returns     pointer to the string descriptor, or NULL if module is invalid or not supported.
 */
uint8_t *tlkusb_module_getStringDesc(uint8_t modType, uint8_t index);

/**
 * @brief       This function gets the interface descriptor of the USB module.
 * @param       modType   - the USB module type.
 * @param       pSetup    - pointer to the USB setup request.
 * @returns     pointer to the interface descriptor, or NULL if module is invalid or not supported.
 */
uint8_t *tlkusb_module_getInfDesDesc(uint8_t modType, tlkusb_setup_req_t *pSetup);


#endif // TLKUSB_MODULE_H

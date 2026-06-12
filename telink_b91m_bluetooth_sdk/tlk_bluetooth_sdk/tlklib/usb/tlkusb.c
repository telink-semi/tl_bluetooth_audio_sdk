/********************************************************************************************************
 * @file    tlkusb.c
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
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/tlkusb_core.h"
#include "tlklib/usb/tlkusb_desc.h"
#if (TLK_CFG_USB_ENABLE)
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "tlklib/usb/uac/tlkusb_uac.h"


extern uint8_t gTlkUsbCurModType[TLK_CFG_USB_NUMB];

/**
 * @brief       This function sets the USB module core with the specified mode type.
 * @param[in]   index           - the USB port index.
 * @param[in]   modType         - the USB mode type to set.
 * @param[in]   isLeaveSuspend  - whether to leave suspend mode.
 * @returns     true if the module is successfully set, false otherwise.
 */
static bool tlkusb_setModuleCore(uint8_t index, TLKHAL_USB_MODE_ENUM modType, uint8_t isLeaveSuspend)
{
    if (index >= TLK_CFG_USB_NUMB) {
        return false;
    }
    if (gTlkUsbCurModType[index] == modType && isLeaveSuspend == 0) {
        return true;
    }
    tlkusb_hal_dp_pullup_en(index, 0);
    tlkusb_hal_gpio_enable(index);

    tlkusb_module_deinit(gTlkUsbCurModType[index]);
    gTlkUsbCurModType[index] = modType;
    if (tlkusb_module_init(modType) != TLK_ENONE) {
        return false;
    }
    tlkusb_hal_dp_pullup_en(index, 1);
    return true;
}

/**
 * @brief       This function initializes the USB module with the specified index and USB ID.
 * @param[in]   index   - the USB port index to initialize.
 * @param[in]   usbID   - the USB ID to set.
 * @returns     TLK_ENONE if successful, -TLK_EPARAM if the index is invalid.
 */
int tlkusb_init(uint8_t index, uint16_t usbID)
{
    if (index >= TLK_CFG_USB_NUMB) {
        return -TLK_EPARAM;
    }
#if (MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X)
    if (index == 0) {
        usbhw_init();
        usbhw_set_ctrl_ep_size(SIZE_64_BYTE);
    }
#endif
#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_TL752X)
    if (index == 1) {
        usb1hw_init();
    }
#endif
    tlkusb_hal_dp_pullup_en(index, 0);
    tlkusb_hal_gpio_disable(index);

    tlkhal_usb_hw_init(index, NULL);

    gTlkUsbCurModType[index] = 0xFF;

    tlkusb_core_init(index, usbID);

#if (TLK_USB_UAC_ENABLE)
    tlkusb_uac_init();
#endif

    tlkusb_hal_wakeup_usb_thread();
    return TLK_ENONE;
}

/**
 * @brief       This function deinitializes the USB module with the specified index.
 * @param[in]   index   - the USB port index to deinitialize.
 * @returns     none.
 */
void tlkusb_deInit(uint8_t index)
{
    tlkusb_hal_dp_pullup_en(index, 0);
    tlkusb_hal_gpio_disable(index);
}

/**
 * @brief       This function opens the USB module with the specified mode type.
 * @param[in]   index    - the USB port index to open.
 * @param[in]   modType  - the USB mode type to set.
 * @returns     TLK_ENONE if successful, error code otherwise.
 */
int tlkusb_open(uint8_t index, TLKHAL_USB_MODE_ENUM modType)
{
    tlkusb_hal_wakeup_usb_thread();
    return tlkusb_setModule(index, modType);
}

/**
 * @brief       This function closes the USB module with the specified index.
 * @param[in]   index   - the USB port index to close.
 * @returns     TLK_ENONE if successful.
 */
int tlkusb_close(uint8_t index)
{
    tlkusb_hal_dp_pullup_en(index, 0);
    tlkusb_hal_gpio_disable(index);
    tlkusb_module_deinit(gTlkUsbCurModType[index]);
    return TLK_ENONE;
}

/**
 * @brief       This function handles USB events for all USB ports.
 * @param       none.
 * @returns     none.
 */
void tlkusb_handler(void)
{
    tlkusb_core_handler(0);
#if MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL322X || MCU_CORE_TYPE == MCU_CORE_TL752X
    tlkusb_core_handler(1);
#endif
#if (MCU_CORE_TYPE == MCU_CORE_TL322X)
    extern void tlkusb_msg_handler(void);
    tlkusb_msg_handler();
#endif
}

/**
 * @brief       This function sets the USB module with the specified mode type.
 * @param[in]   index    - the USB port index.
 * @param[in]   modType  - the USB mode type to set.
 * @returns     true if the module is successfully set, false otherwise.
 */
bool tlkusb_setModule(uint8_t index, TLKHAL_USB_MODE_ENUM modType)
{
    return tlkusb_setModuleCore(index, modType, 0);
}

/**
 * @brief       This function handles USB interrupt requests.
 * @param       index   - the USB port index.
 * @returns     none.
 */
_attribute_retention_code_ void tlkusb_irqHandler(uint8_t index)
{
    (void)index;
#if (TLK_USB_UAC_ENABLE)
    if (gTlkUsbCurModType[index] == TLKUSB_MODTYPE_UAC) {
        tlkusb_uacirq_handler();
    }
#endif
#if (TLK_USB_UDB_ENABLE)
    if (gTlkUsbCurModType[index] == TLKUSB_MODTYPE_UDB) {
        tlkusb_eps_irq_handler(index);
    }
#endif
#if (TLK_USB_MSC_ENABLE)
    if (gTlkUsbCurModType[index] == TLKUSB_MODTYPE_MSC) {
        tlkusb_eps_irq_handler(index);
    }
#endif
}

/**
 * @brief       This function handles UDB USB interrupt requests.
 * @param       none.
 * @returns     none.
 */
void tlk_udb_usb_handle_irq(void)
{
    tlkusb_handler();
    tlkdbg_handler();
}
#else
/**
 * @brief       This function handles UDB USB interrupt requests (dummy implementation).
 * @param       none.
 * @returns     none.
 */
void tlk_udb_usb_handle_irq(void) {}

#endif // #if (TLK_CFG_USB_ENABLE)

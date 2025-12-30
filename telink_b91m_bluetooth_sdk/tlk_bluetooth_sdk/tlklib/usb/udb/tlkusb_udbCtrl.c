/********************************************************************************************************
 * @file    tlkusb_udbCtrl.c
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
#if (TLK_USB_UDB_ENABLE)
#include "drivers.h"
#include "tlklib/usb/udb/tlkusb_udbDefine.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "tlklib/usb/udb/tlkusb_udbDesc.h"
#include "tlklib/usb/udb/tlkusb_udbCtrl.h"

// #include "stack/inner/tlkstk_myudb.h" syzhou to 0603


extern void tlkusb_udb_recvHandler(void);

/**
 * @brief   Initialize USB UDB control module.
 * @param   None.
 * @return  TLK_ENONE if successful.
 */
static int tlkusb_udbctrl_init(void);

/**
 * @brief   Reset USB UDB control module.
 * @param   None.
 * @return  None.
 */
static void tlkusb_udbctrl_reset(void);

/**
 * @brief   Deinitialize USB UDB control module.
 * @param   None.
 * @return  None.
 */
static void tlkusb_udbctrl_deinit(void);

/**
 * @brief   Handle USB UDB control operations.
 * @param   None.
 * @return  None.
 */
static void tlkusb_udbctrl_handler(void);

const tlkusb_modCtrl_t sTlkUsbUdbModCtrl = {
    tlkusb_udbctrl_init,    // Init
    tlkusb_udbctrl_reset,   // Reset
    tlkusb_udbctrl_deinit,  // Deinit
    tlkusb_udbctrl_handler, // Handler
    NULL,                   // GetClassInf
    NULL,                   // SetClassInf
    NULL,                   // GetClassEdp
    NULL,                   // SetClassEdp
    NULL,                   // GetInterface
    NULL,                   // SetInterface
};

/**
 * @brief   Initialize USB UDB control module.
 * @param   None.
 * @return  TLK_ENONE if successful.
 */
static int tlkusb_udbctrl_init(void)
{
#if (MCU_CORE_TYPE != MCU_CORE_TL751X && MCU_CORE_TYPE != MCU_CORE_TL753X && MCU_CORE_TYPE != MCU_CORE_TL322X && MCU_CORE_TYPE != MCU_CORE_TL752X)
#if (USB_CNT == 0)
    uint8_t iso;

    reg_usb_ep_buf_addr(TLKUSB_UDB_EDP_DBG_IN)  = 0x80;
    reg_usb_ep_buf_addr(TLKUSB_UDB_EDP_DBG_OUT) = 0xC0;
#if (TLKUSB_UDB_VCD_ENABLE)
    reg_usb_ep_max_size                        = (128 >> 3);
    reg_usb_ep8_send_thres                     = 0x40;
    reg_usb_ep8_send_max                       = 128 >> 3;
    reg_usb_ep_buf_addr(TLKUSB_UDB_EDP_VCD_IN) = 0;
    reg_usb_ep8_fifo_mode                      = 1;
#endif

    reg_usb_mdev &= ~BIT(3); // vendor command: bRequest[7] = 0

    usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC);
    reg_usb_ep_ctrl(TLKUSB_UDB_EDP_DBG_OUT) = FLD_EP_DAT_ACK;

    iso = reg_usb_iso_mode;
    iso &= ~(1 << TLKUSB_UDB_EDP_DBG_OUT);
    reg_usb_iso_mode = iso;
#elif (USB_CNT == 1)
    uint8_t iso;

    extern void usb1hw_init(void);
    usb1hw_init();

    reg_usb1_ep_buf_addr(TLKUSB_UDB_EDP_DBG_IN)  = 0x80;
    reg_usb1_ep_buf_addr(TLKUSB_UDB_EDP_DBG_OUT) = 0xC0;
#if (TLKUSB_UDB_VCD_ENABLE)
    reg_usb1_ep_max_size                        = (128 >> 2);
    reg_usb1_ep8_send_thres                     = 0x40;
    reg_usb1_ep8_send_max                       = 128 >> 3;
    reg_usb1_ep_buf_addr(TLKUSB_UDB_EDP_VCD_IN) = 0;
    reg_usb1_ep8_fifo_mode                      = 1;
#endif

    reg_usb1_mdev &= ~BIT(3); // vendor command: bRequest[7] = 0

    usb1hw_enable_manual_interrupt(FLD_USB1_CTRL_EP_AUTO_STD | FLD_USB1_CTRL_EP_AUTO_DESC);
    reg_usb1_ep_ctrl(TLKUSB_UDB_EDP_DBG_OUT) = FLD_USB1_EP_DAT_ACK;

    iso = reg_usb1_iso_mode;
    iso &= ~(1 << TLKUSB_UDB_EDP_DBG_OUT);
    reg_usb1_iso_mode = iso;

#endif
#else
    reg_usb1_ep_max_size    = (128 >> 3);
    reg_usb1_ep8_send_thres = 0x40;
    usb1hw_set_ep_addr(TLKUSB_UDB_EDP_DBG_IN, 128);
    usb1hw_set_ep_addr(TLKUSB_UDB_EDP_DBG_OUT, 192);
    usb1hw_set_ep_addr(TLKUSB_UDB_EDP_VCD_IN, 0);
    reg_usb1_ep8_fifo_mode = 1;
    reg_usb1_mdev &= ~BIT(3); /* vendor command: bRequest[7] = 0. */

#if (USB_CNT == 0)
    reg_usb_ep_ctrl(TLKUSB0_UDB_EDP_DBG_OUT) = FLD_EP_DAT_ACK;
    usbhw_set_eps_en(BIT(TLKUSB0_UDB_EDP_DBG_IN) | BIT(TLKUSB0_UDB_EDP_DBG_OUT) | BIT(TLKUSB0_UDB_EDP_VCD_IN & 0x07));
    usb_set_pin(1);
#elif (USB_CNT == 1)
    reg_usb1_ep_ctrl(TLKUSB_UDB_EDP_DBG_OUT) = FLD_USB1_EP_DAT_ACK;

    uint8_t iso = reg_usb1_iso_mode;
    iso &= ~(1 << TLKUSB_UDB_EDP_DBG_OUT);
    reg_usb1_iso_mode = iso;

    usb1hw_set_eps_en(BIT(TLKUSB_UDB_EDP_DBG_IN) | BIT(TLKUSB_UDB_EDP_DBG_OUT) | BIT(TLKUSB_UDB_EDP_VCD_IN & 0x07));
#if MCU_CORE_TYPE == MCU_CORE_TL752X
    usb1_set_pin_en();
#else
    usb1_set_pin(); //TODO
#endif
#endif


#endif
    return TLK_ENONE;
}

/**
 * @brief   Reset USB UDB control module.
 * @param   None.
 * @return  None.
 */
static void tlkusb_udbctrl_reset(void)
{
    tlkusb_hal_ep_bulkout_ready(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_OUT);
}

/**
 * @brief   Deinitialize USB UDB control module.
 * @param   None.
 * @return  None.
 */
static void tlkusb_udbctrl_deinit(void) {}

/**
 * @brief   Handle USB UDB control operations.
 * @param   None.
 * @return  None.
 */
static void tlkusb_udbctrl_handler(void)
{
    tlkusb_udb_recvHandler();
}


#endif
/********************************************************************************************************
 * @file    tlkhal_usb_TL322X.c
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
#include "../../api/tlkhal_api.h"
#include "drivers.h"
#include "tlklib/usb/tlkusb_hal.h"
#include "tlklib/usb/tlkusb_struct.h"
#include "tlklib/usb/tlkusb_define.h"
#include "tlklib/usb/udb/tlkusb_udbDefine.h"
#include "tlklib/usb/uac/tlkusb_uac.h"
#include "tlklib/usb/uac/tlkusb_uacSpk.h"
#include "tlklib/usb/uac/tlkusb_uacMic.h"
#include "tlklib/usb/uac/tlkusb_uacDefine.h"

#include "tlklib/usb/tlkusb_msg.h"

#if MCU_CORE_TYPE == MCU_CORE_TL322X
/*Support two fs usbs. USB0 is used for audio, USB1 is used for debug.*/
/*TODO: Optimize processing logic later.*/
static unsigned char                      ep0_out_data[64];
__attribute__((aligned(4))) unsigned char audio_buffer[192];
__attribute__((aligned(4))) unsigned char in_audio_buffer[32];

static signed short sin_16k[] __attribute__((aligned(4))) = {
    0, 12539, 23169, 30272, 32767, 30272, 23169, 12539, 4, -12539, -23169, -30272, -32767, -30272, -23169, -12539,
};

/**
 * @brief  Initialize USB hardware
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_hw_init(uint8_t port_id, void *userArg)
{
    (void)userArg;
    if (port_id >= TLK_CFG_USB_NUMB) {
        return;
    }
    if (port_id == 0) {
        usb0hw_init(USB0_SPEED_HIGH);
    } else if (port_id == 1) {
        usb1hw_init();
    }
}

/**
 * @brief  Initialize USB function
 * @param[in] port_id : USB port ID
 * @param[in] usb_mode : USB mode type
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_function_init(uint8_t port_id, TLKHAL_USB_MODE_ENUM usb_mode, void *userArg)
{
    (void)userArg;
    if (port_id >= TLK_CFG_USB_NUMB) {
        return;
    }

    switch (usb_mode) {
    case TLKUSB_MODTYPE_UDB:
    {
        usb1hw_set_eps_max_size(128);
        usb1hw_set_ep_addr(TLKUSB_UDB_EDP_VCD_IN, 0x00);
        usb1hw_set_ep_addr(TLKUSB_UDB_EDP_DBG_IN, 0x80);
        usb1hw_set_ep_addr(TLKUSB_UDB_EDP_DBG_OUT, 0xC0);

        usb1hw_set_printer_threshold(0x40);
        reg_usb1_ep8_max0 = (128 >> 3);
        usb1hw_ep8_fifo_mode_en();
        reg_usb1_mdev &= ~FLD_USB1_USB_MDEV_VEND_CMD;

        usb1hw_enable_manual_interrupt(FLD_USB1_CTRL_EP_AUTO_STD | FLD_USB1_CTRL_EP_AUTO_DESC);
        reg_usb1_iso_mode &= ~BIT(TLKUSB_UDB_EDP_DBG_OUT);
        usb1hw_data_ep_ack(TLKUSB_UDB_EDP_DBG_OUT);

        usb1hw_set_eps_en(BIT(TLKUSB_UDB_EDP_DBG_IN) | BIT(TLKUSB_UDB_EDP_DBG_OUT) | BIT(TLKUSB_UDB_EDP_VCD_IN & 0x07));
        usb1_set_pin();
        break;
    }
    case TLKUSB_MODTYPE_UAC_HS:
    {
        usb0hw_init(USB0_SPEED_HIGH);
        usb0hw_set_grxfsiz(0x100);
        usb0hw_set_epin_size(USB0_EP0, 0x100, 64);
        usb0hw_set_epin_size(USB0_EP6, 0x100 + 64, 256);
        usb0hw_set_epin_size(USB0_EP7, 0x100 + 64 + 64, 256);

        // plic_set_priority(IRQ_USB0, 2);
        plic_interrupt_enable(IRQ_USB0);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief  Stall USB endpoint
 * @param[in] port_id : USB port ID
 * @param[in] ep_addr : Endpoint address
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_ep_stall(uint8_t port_id, uint8_t ep_addr, void *userArg)
{
    (void)userArg;
    if (port_id == 0) {
        unsigned char const ep_dir = (ep_addr & USB_DIR_IN_MASK) ? USB_DIR_IN : USB_DIR_OUT;
        unsigned char const ep_num = (ep_addr & (~USB_DIR_IN_MASK));

        if (ep_dir == USB_DIR_IN) {
            usb0hw_set_inep_stall(ep_num);
        } else {
            usb0hw_set_outep_stall(ep_num);
        }

        if (ep_num == 0) {
            /* receive next setup. */
            usb0hw_read_ep_data(0, ep0_out_data, sizeof(ep0_out_data) / sizeof(ep0_out_data[0]));
        }
    }
}

/**
 * @brief  Read data from USB endpoint
 * @param[in] port_id : USB port ID
 * @param[in] ep_addr : Endpoint address
 * @param[in] data : Data buffer to read into
 * @param[in] len : Length of data to read
 * @param[in] userArg : User argument
 * @returns  Result of read operation.
 */
int tlkhal_usb_read_ep_data(uint8_t port_id, uint8_t ep_addr, uint8_t *data, uint16_t len, void *userArg)
{
    (void)userArg;
    if (port_id == 0) {
        uint8_t ep_num = (ep_addr & (~USB_DIR_IN_MASK));
        if (ep_num == 0) {
            usb0hw_read_ep_data(ep_num, ep0_out_data, sizeof(ep0_out_data) / sizeof(ep0_out_data[0]));
        } else {
            usb0hw_read_ep_data(ep_num, data, len);
        }

        return len;
    }

    return 0;
}

/**
 * @brief  Write data to USB endpoint
 * @param[in] port_id : USB port ID
 * @param[in] ep_addr : Endpoint address
 * @param[in] data : Data buffer to write from
 * @param[in] len : Length of data to write
 * @param[in] userArg : User argument
 * @returns  Result of write operation.
 */
int tlkhal_usb_write_ep_data(uint8_t port_id, uint8_t ep_addr, uint8_t *data, uint16_t len, void *userArg)
{
    (void)userArg;

    if (port_id == 0) {
        gpio_toggle(GPIO_PC4);
        gpio_toggle(GPIO_PC4);
        uint8_t ep_num = (ep_addr & (~USB_DIR_IN_MASK));
        usb0hw_write_ep_data(ep_num, data, len);
        return len;
    }
    return 0;
}

static signed short sin_48k_stereo_spk[] __attribute__((aligned(4))) = {
    0,     0,     1069,  1069,  2120,  2120,  3134,  3134,  4095,  4095,  4986,  4986,  5792,  5792,  6499,  6499,  7094,  7094,  7568,  7568,  7912,  7912,  8121,  8121,
    8191,  8191,  8121,  8121,  7912,  7912,  7568,  7568,  7094,  7094,  6499,  6499,  5792,  5792,  4986,  4986,  4096,  4096,  3134,  3134,  2120,  2120,  1069,  1069,
    0,     0,     -1069, -1069, -2120, -2120, -3134, -3134, -4095, -4095, -4986, -4986, -5792, -5792, -6499, -6499, -7094, -7094, -7568, -7568, -7912, -7912, -8121, -8121,
    -8191, -8191, -8121, -8121, -7912, -7912, -7568, -7568, -7094, -7094, -6499, -6499, -5792, -5792, -4986, -4986, -4096, -4096, -3134, -3134, -2120, -2120, -1069, -1069,
}; //96

static void usbd_epout_complete_handler(unsigned char ep_addr, unsigned int len)
{
    // gpio_set_high_level(GPIO_PA0);
    usb0hw_read_ep_data(ep_addr & (~0x80), audio_buffer, len);
#if (TLK_USB_UAC_HS_ENABLE)
    int16_t *pBuffer = (int16_t *)audio_buffer;
    tlkusb_uac_write_iso_out_samples(pBuffer, TLKUSB_AUD_SPK_SAMPLES * 2);
    g_tlk_usb_cfg.iso_out_en = true;
    tlkusb_hal_wakeup_uachs_thread_fromIsr();
#endif
    // gpio_set_low_level(GPIO_PA0);
    (void)sin_48k_stereo_spk;
}

static void usbd_epin_complete_handler(unsigned char ep_addr, unsigned int len)
{
// gpio_set_high_level(GPIO_PA1);
#if (TLK_USB_UAC_HS_ENABLE)
    tlkusb_uac_read_iso_in_samples((int16_t *)in_audio_buffer, 16);
    g_tlk_usb_cfg.iso_in_en = true;
    tlkusb_hal_wakeup_uachs_thread_fromIsr();
#endif

    (void)sin_16k;
    // usb0hw_write_ep_data(ep_addr & (~0x80), (uint8_t*)sin_16k, len);
    usb0hw_write_ep_data(ep_addr & (~0x80), in_audio_buffer, len);
    // gpio_set_low_level(GPIO_PA1);
}

static void usb_irq_handler_epout(void)
{
    dcd_event_t event = {0};

    for (unsigned char epnum = 0; epnum < 9; epnum++) {
        if ((usb0hw_get_daint() >> 16) & BIT(epnum)) {
            unsigned int doepint = usb0hw_get_doepint(epnum);
            if (doepint & FLD_USB_DOEPINT_XFERCOMPL) {
                usb0hw_clear_doepint(epnum, FLD_USB_DOEPINT_XFERCOMPL);
                unsigned short xfered_len = usb0hw_get_epout_len(epnum);

                if ((epnum == 0)) {
                    if ((xfered_len == 0)) {
                        usb0hw_read_ep_data(0, ep0_out_data, sizeof(ep0_out_data) / sizeof(ep0_out_data[0]));
                    }
                } else {
                    usbd_epout_complete_handler(epnum, xfered_len);
                }
            }

            if (doepint & FLD_USB_DOEPINT_SETUP) {
                usb0hw_clear_doepint(epnum, FLD_USB_DOEPINT_SETUP);

                gpio_toggle(GPIO_PA0);
                gpio_toggle(GPIO_PA0);

                event.rhport      = 0;
                event.setup_stage = 1;
                event.event_id    = DCD_EVENT_SETUP_RECEIVED;
                tmemcpy(&event.setup_received, ep0_out_data, sizeof(usb_control_request_t));
                tlkusb_fifo_push((uint8_t *)&event, sizeof(dcd_event_t));
            }

            if (doepint & FLD_USB_DOEPINT_STSPHSERCVD) {
                usb0hw_clear_doepint(epnum, FLD_USB_DOEPINT_STSPHSERCVD);

                event.rhport      = 0;
                event.setup_stage = 0;
                event.event_id    = USBD_EVENT_STSPHSERCVD;
                tmemcpy(&event.setup_received, ep0_out_data, sizeof(usb_control_request_t));
                tlkusb_fifo_push((uint8_t *)&event, sizeof(dcd_event_t));
            }
        }
    }
}

static void usb_irq_handler_epin(void)
{
    for (unsigned char epnum = 0; epnum < 9; epnum++) {
        if ((usb0hw_get_daint()) & BIT(epnum)) {
            unsigned int diepint = usb0hw_get_diepint(epnum);
            if (diepint & FLD_USB_DIEPINT_XFERCOMPL) {
                usb0hw_clear_diepint(epnum, FLD_USB_DIEPINT_XFERCOMPL);
                unsigned short xfered_len = usb0hw_get_epin_len(epnum);
                (void)xfered_len;
                if (epnum == 0) {
                    usb0hw_read_ep_data(0, ep0_out_data, sizeof(ep0_out_data) / sizeof(ep0_out_data[0]));
                    BM_SET(reg_usb_diepctl(0), FLD_USB_DIEPCTL_STALL);
                } else {
                    usbd_epin_complete_handler(epnum, xfered_len);
                }
            }
        }
    }
}

volatile uint32_t AAAA_usb_hs_handler = 0;
volatile uint32_t AAAA_usb_fs_handler = 0;

/**
 * @brief  USB interrupt handler
 * @param[in] port_id : USB port ID
 * @returns  None.
 */
void tlkhal_usb_irq_handler(uint8_t port_id)
{
    if (port_id == 0) {
        AAAA_usb_hs_handler++;

        // gpio_toggle(GPIO_PA0);
        // gpio_toggle(GPIO_PA0);

        dcd_event_t event = {0};

        unsigned int status = usb0hw_get_gintsts() & reg_usb_gintmsk;

        if (status == 0) {
            return;
        }

        if (status & FLD_USB_GINTSTS_ENUMDONE) {
            // gpio_toggle(GPIO_PA1);
            // gpio_toggle(GPIO_PA1);
            usb0hw_clear_gintsts(FLD_USB_GINTSTS_ENUMDONE);
        }

        if (status & FLD_USB_GINTSTS_OEPINT) {
            // gpio_toggle(GPIO_PA2);
            // gpio_toggle(GPIO_PA2);
            usb_irq_handler_epout();
        }

        if (status & FLD_USB_GINTSTS_IEPINT) {
            // gpio_toggle(GPIO_PC7);
            // gpio_toggle(GPIO_PC7);
            usb_irq_handler_epin();
        }

        if (status & FLD_USB_GINTSTS_SOF) {
            // gpio_toggle(GPIO_PC6);
            // gpio_toggle(GPIO_PC6);
            usb0hw_clear_gintsts(FLD_USB_GINTSTS_SOF);
        }

        if (status & FLD_USB_GINTSTS_USBSUSP) {
            // gpio_toggle(GPIO_PC5);
            // gpio_toggle(GPIO_PC5);
            usb0hw_clear_gintsts(FLD_USB_GINTSTS_USBSUSP);

            // event.rhport = port_id;
            // event.event_id = DCD_EVENT_SUSPEND;
            // tlkusb_fifo_push((uint8_t*)&event, sizeof(dcd_event_t));
        }

        if (status & FLD_USB_GINTSTS_WKUPINT) {
            // gpio_toggle(GPIO_PC4);
            // gpio_toggle(GPIO_PC4);
            usb0hw_clear_gintsts(FLD_USB_GINTSTS_WKUPINT);
        }

        if (status & FLD_USB_GINTSTS_USBRST) {
            // gpio_toggle(GPIO_PC3);
            // gpio_toggle(GPIO_PC3);
            usb0hw_clear_gintsts(FLD_USB_GINTSTS_USBRST);

            /*do hw reset here.*/
            // tlkhal_usb_reset(port_id, NULL);

            event.rhport   = port_id;
            event.event_id = DCD_EVENT_BUS_RESET;

            tlkusb_fifo_push((uint8_t *)&event, sizeof(dcd_event_t));
        }
    }
}

/**
 * @brief  Reset USB
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_reset(uint8_t port_id, void *userArg)
{
    (void)userArg;
    if (port_id == 0) {
        usb0hw_reset();
        usb0hw_read_ep_data(0, ep0_out_data, sizeof(ep0_out_data) / sizeof(ep0_out_data[0]));
    }
}

/**
 * @brief  Handle USB unplug event
 * @param[in] port_id : USB port ID
 * @param[in] userArg : User argument
 * @returns  None.
 */
void tlkhal_usb_unpluged(uint8_t port_id, void *userArg)
{
    (void)port_id;
    (void)userArg;
}


#endif

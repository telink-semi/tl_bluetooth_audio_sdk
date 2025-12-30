/********************************************************************************************************
 * @file    tlkusb_msg.c
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
#include "tlkusb_msg.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_hal.h"
#include "tlksys/platform/api/tlkhal_usb.h"
#include "drivers.h"


#if (TLK_CFG_USB_ENABLE)

#define TLKUSB_CTRL_BUFF_NUMB 32
#define TLKUSB_CTRL_BUFF_SIZE 12 //sizeof(dcd_event_t)

static uint8_t usb_control_packet[TLKUSB_CTRL_BUFF_NUMB * TLKUSB_CTRL_BUFF_SIZE]; //TODO: malloc

extern unsigned char usbd_control_request_process(const unsigned char bus, const usb_control_request_t *setup, unsigned char setup_stage);

typedef struct
{
    uint8_t *pBuffer;
    uint32_t size;
    uint32_t wptr;
    uint32_t rptr;
} tlk_usb_fifo_t;

tlk_usb_fifo_t usb_control_fifo; //qfifo

/**
 * @brief       This function initializes the USB message system.
 * @param       none.
 * @returns     none.
 */
void tlkusb_msg_init(void)
{
    (void)usb_control_packet;
    tmemset(&usb_control_fifo, 0, sizeof(tlk_usb_fifo_t));
    usb_control_fifo.pBuffer = usb_control_packet;
    usb_control_fifo.size    = TLKUSB_CTRL_BUFF_SIZE * TLKUSB_CTRL_BUFF_NUMB;
}

/**
 * @brief       This function enters a critical section for USB operations.
 * @param       none.
 * @returns     none.
 */
_always_inline static inline void tlk_usb_enterCritical(void)
{
    tlksys_enter_critical();
}

/**
 * @brief       This function leaves a critical section for USB operations.
 * @param       none.
 * @returns     none.
 */
_always_inline static inline void tlk_usb_leaveCritical(void)
{
    tlksys_leave_critical();
}

/**
 * @brief       This function pushes data to the USB control FIFO.
 * @param[in]   pBuffer  - pointer to the buffer containing data to push.
 * @param[in]   size     - size of the data to push.
 * @returns     TLK_ENONE on success, TLK_EPARAM if size is invalid, TLK_ENOSPACE if no space available.
 */
_attribute_ram_code_ int tlkusb_fifo_push(uint8_t *pBuffer, uint32_t size)
{
    if (size != sizeof(dcd_event_t)) {
        return -TLK_EPARAM;
    }

    uint16_t idleLen = 0;

    if (usb_control_fifo.rptr > usb_control_fifo.wptr) {
        idleLen = usb_control_fifo.rptr - usb_control_fifo.wptr;
    } else {
        idleLen = usb_control_fifo.size + usb_control_fifo.rptr - usb_control_fifo.wptr;
    }

    if (idleLen < size) {
        return -TLK_ENOSPACE;
    }
    tlk_usb_enterCritical();
    tmemcpy(usb_control_fifo.pBuffer + usb_control_fifo.wptr, pBuffer, size);
    usb_control_fifo.wptr = (usb_control_fifo.wptr + size) % usb_control_fifo.size;
    tlk_usb_leaveCritical();

    tlkusb_hal_wakeup_usb_thread_fromIsr();

    return TLK_ENONE;
}

static usb_control_request_t sTlkUsbCtrlReq;

/**
 * @brief       This function processes all events in the USB control FIFO.
 * @param       none.
 * @returns     none.
 */
void tlk_usb_fifo_pop_all(void)
{
    tlk_usb_enterCritical();

    while (usb_control_fifo.wptr != usb_control_fifo.rptr) {
        dcd_event_t event;
        tmemcpy(&event, usb_control_fifo.pBuffer + usb_control_fifo.rptr, sizeof(dcd_event_t));
        usb_control_fifo.rptr = (usb_control_fifo.rptr + sizeof(dcd_event_t)) % usb_control_fifo.size;

        switch (event.event_id) {
        case DCD_EVENT_BUS_RESET:
        {
            tlkhal_usb_reset(event.rhport, NULL);
            break;
        }
        case DCD_EVENT_UNPLUGGED:
        {
            tlkhal_usb_unpluged(event.rhport, NULL);
            break;
        }
        case DCD_EVENT_SETUP_RECEIVED:
        {
            tmemcpy(&sTlkUsbCtrlReq, &event.setup_received, sizeof(usb_control_request_t));
            if (!usbd_control_request_process(event.rhport, &sTlkUsbCtrlReq, event.setup_stage)) {
                tlkhal_usb_ep_stall(0, 0, NULL);
                tlkhal_usb_ep_stall(0, 0 | USB_DIR_IN_MASK, NULL);
            }
            break;
        }
        case USBD_EVENT_STSPHSERCVD:
        {
            tmemcpy(&sTlkUsbCtrlReq, &event.setup_received, sizeof(usb_control_request_t));

            if (!usbd_control_request_process(0, &sTlkUsbCtrlReq, event.setup_stage)) {
                tlkhal_usb_ep_stall(0, 0, NULL);
                tlkhal_usb_ep_stall(0, 0 | USB_DIR_IN_MASK, NULL);
            } else {
                tlkhal_usb_write_ep_data(0, 0, 0, 0, NULL);
            }
            break;
        }
        case DCD_EVENT_XFER_COMPLETE:
        {
            // Invoke the class callback associated with the endpoint address

            break;
        }
        case DCD_EVENT_SUSPEND:
        {
            break;
        }
        case DCD_EVENT_RESUME:
        {
            break;
        }
        case USBD_EVENT_FUNC_CALL:
        {
            if (event.func_call.func) {
                event.func_call.func(event.func_call.param);
            }
            break;
        }
        case DCD_EVENT_SOF:
        {
            break;
        }
        case USBD_EVENT_CTRL_IRQ_STA:
        {
            break;
        }
        default:
            break;
        }
    }
    tlk_usb_leaveCritical();
}

/**
 * @brief       This function handles USB messages by processing all events in the FIFO.
 * @param       none.
 * @returns     none.
 */
void tlkusb_msg_handler(void)
{
    tlk_usb_fifo_pop_all();
}

#endif // #if (TLK_CFG_USB_ENABLE)

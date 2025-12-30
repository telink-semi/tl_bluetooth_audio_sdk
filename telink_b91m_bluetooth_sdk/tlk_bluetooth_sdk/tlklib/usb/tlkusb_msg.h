/********************************************************************************************************
 * @file    tlkusb_msg.h
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
#ifndef TLKUSB_MSG_H
#define TLKUSB_MSG_H

#include "tlkusb_struct.h"

typedef enum
{
    TUSB_SPEED_FULL    = 0,
    TUSB_SPEED_LOW     = 1,
    TUSB_SPEED_HIGH    = 2,
    TUSB_SPEED_AUTO    = 0xaa,
    TUSB_SPEED_INVALID = 0xff,
} tusb_speed_t;

typedef enum
{
    DCD_EVENT_INVALID = 0,    // 0
    DCD_EVENT_BUS_RESET,      // 1
    DCD_EVENT_UNPLUGGED,      // 2
    DCD_EVENT_SOF,            // 3
    DCD_EVENT_SUSPEND,        // 4 TODO LPM Sleep L1 support
    DCD_EVENT_RESUME,         // 5
    DCD_EVENT_SETUP_RECEIVED, // 6
    DCD_EVENT_XFER_COMPLETE,  // 7
    USBD_EVENT_FUNC_CALL,     // 8 Not an DCD event, just a convenient way to defer ISR function
    USBD_EVENT_STSPHSERCVD,   // 9 Status Phase Received for Control Write, Only for Control out Endpoint.
    USBD_EVENT_CTRL_IRQ_STA,  // 10
    DCD_EVENT_COUNT
} dcd_eventid_t;

typedef struct __attribute__((aligned(4)))
{
    uint8_t rhport;
    uint8_t event_id;
    uint8_t setup_stage;
    uint8_t resv;

    union
    {
        // BUS RESET
        struct
        {
            tusb_speed_t speed;
        } bus_reset;

        // SOF
        struct
        {
            uint32_t frame_count;
        } sof;

        // SETUP_RECEIVED
        usb_control_request_t setup_received;

        // XFER_COMPLETE
        struct
        {
            uint8_t  ep_addr;
            uint8_t  result;
            uint32_t len;
        } xfer_complete;

        // FUNC_CALL
        struct
        {
            void (*func)(void *);
            void *param;
        } func_call;
    };
} dcd_event_t;

/**
 * @brief       This function initializes the USB message system.
 * @param       none.
 * @returns     none.
 */
void tlkusb_msg_init(void);

/**
 * @brief       This function pushes data to the USB control FIFO.
 * @param[in]   pBuffer  - pointer to the buffer containing data to push.
 * @param[in]   size     - size of the data to push.
 * @returns     TLK_ENONE on success, TLK_EPARAM if size is invalid, TLK_ENOSPACE if no space available.
 */
_attribute_ram_code_ int tlkusb_fifo_push(uint8_t *pBuffer, uint32_t size);

/**
 * @brief       This function handles USB messages by processing all events in the FIFO.
 * @param       none.
 * @returns     none.
 */
void tlkusb_msg_handler(void);

#endif // TLKUSB_MSG_H

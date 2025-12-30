/********************************************************************************************************
 * @file    tlkusb_udb.c
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
#if (TLK_USB_UDB_ENABLE)
#include "tlklib/usb/udb/tlkusb_udbDefine.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include <stdio.h>
#include <stdarg.h>
#include "tlklib/dbg/tlkdbg.h"


#define TLKUSB_UDB_BUFF_SIZE    320

#define TLKUSB_UDB_EDP_OUT_FLAG (1 << (TLKUSB_UDB_EDP_DBG_OUT & 7))

static bool tlkusb_udb_recvDatDeal(void);
static void tlkusb_udb_recvCmdProc(uint8_t *pData, uint16_t dataLen, bool *pIsDown);

static uint16_t sTlkUsbUdbCmdLength = 0;
static uint8_t  sTlkUsbUdbCmdBuffer[TLKUSB_UDB_BUFF_SIZE];

extern const tlkusb_modDesc_t sTlkUsbUdbModDesc;
extern const tlkusb_modCtrl_t sTlkUsbUdbModCtrl;
const tlkusb_module_t         gTlkUsbUdbModule = {
            .modType = TLKUSB_MODTYPE_UDB,
            .pDesc   = &sTlkUsbUdbModDesc,
            .pCtrl   = &sTlkUsbUdbModCtrl,
};

/**
 * @brief   Check if USB sending is busy.
 * @param   None.
 * @return  Returns true if USB sending is busy, false otherwise.
 */
bool tlkusb_udb_isSendBusy(void)
{
    return tlkusb_hal_is_ep_busy(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_IN);
}

/**
 * @brief   Send data through USB.
 * @param   pData Pointer to the data to send.
 * @param   dataLen Length of the data to send.
 * @return  TLK_ENONE if successful, error code otherwise.
 */
_attribute_ram_code_ int tlkusb_udb_sendData(uint8_t *pData, uint8_t dataLen)
{
    if (dataLen > 64) {
        return -TLK_ELENGTH;
    }
    if (tlkusb_hal_is_ep_busy(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_IN)) {
        return -TLK_EBUSY;
    }
    tlkusb_hal_reset_ep_ptr(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_IN);
    tlkusb_hal_write_ep_data(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_IN, pData, dataLen);
    tlkusb_hal_data_ep_ack(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_IN);
    return TLK_ENONE;
}

/**
 * @brief   Handle received USB data.
 * @param   None.
 * @return  None.
 */
_attribute_ram_code_ void tlkusb_udb_recvHandler(void)
{
    bool ready;
    bool isDown = false;
    do {
        ready = tlkusb_udb_recvDatDeal();
        if (ready) {
            tlkusb_udb_recvCmdProc(sTlkUsbUdbCmdBuffer, sTlkUsbUdbCmdLength, &isDown);
            if (isDown) {
                tlkusb_hal_disable_eventMode();
                sTlkUsbUdbCmdLength = 0;
            }
        }
#if (TLKDBG_CFG_UDB_LOG_ENABLE)
        if (isDown) {
            tlkdbg_stream_download_handler();
        }
#endif
    } while (isDown);
    if (ready) {
        if (sTlkUsbUdbCmdBuffer[0] == 0x11) {
            tlkusb_debug_shell_hook(sTlkUsbUdbCmdBuffer, sTlkUsbUdbCmdLength);
        }
        sTlkUsbUdbCmdLength = 0;
    }
}

/**
 * @brief   Deal with received USB data.
 * @param   None.
 * @return  Returns true if data is ready, false otherwise.
 */
_attribute_ram_code_ static bool tlkusb_udb_recvDatDeal(void)
{
    uint8_t buffer[64];
    if (tlkusb_hal_get_eps_irq(TLK_CFG_USB_UDB_INDEX) & TLKUSB_UDB_EDP_OUT_FLAG) {
        tlkusb_hal_clr_eps_irq(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_OUT_FLAG);
        uint16_t length = tlkusb_hal_get_ep_ptr(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_OUT);
        tlkusb_hal_reset_ep_ptr(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_OUT);

        tlkusb_hal_read_ep_data(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_OUT, buffer, length);
        tlkusb_hal_data_ep_ack(TLK_CFG_USB_UDB_INDEX, TLKUSB_UDB_EDP_DBG_OUT);
        if (length != 0 && length + sTlkUsbUdbCmdLength < TLKUSB_UDB_BUFF_SIZE) {
            tmemcpy(sTlkUsbUdbCmdBuffer + sTlkUsbUdbCmdLength, buffer, length);
            sTlkUsbUdbCmdLength += length;
        }
        if (length != 0 && length < 64) {
            return true;
        }
    }
    return false;
}

/**
 * @brief   Process received USB command.
 * @param   pData Pointer to the received data.
 * @param   dataLen Length of the received data.
 * @param   pIsDown Pointer to a flag indicating download state.
 * @return  None.
 */
_attribute_ram_code_ static void tlkusb_udb_recvCmdProc(uint8_t *pData, uint16_t dataLen, bool *pIsDown)
{
    uint8_t rsp[256 + 6];
    uint8_t cmd = pData[0];

    //////////////////////////  Memory Read ////////////////////////////////////
    if (cmd == 0x28 && dataLen >= 8) {
#if (TLKDBG_CFG_UDB_LOG_ENABLE)
        tlkdbg_send_status(0x81, 8, pData, 12);
#endif
        rsp[0] = 0x29;
        tmemcpy(rsp + 1, pData + 1, 5);
        int      type = pData[1];
        uint32_t adr  = pData[2] | (pData[3] << 8) | (pData[4] << 16) | (pData[5] << 24);
        int      n    = pData[6] | (pData[7] << 8);
        if (n > 256) {
            n = 256;
        }

        if (type == 0) {
            tmemcpy(rsp + 6, (void *)(adr | 0), n);
        } else if (type == 1) {
            for (int i = 0; i < n; i++) {
#if (MCU_CORE_TYPE != CHIP_TYPE_TL752X)
                rsp[i + 6] = analog_read_reg8(adr + i); //TODO ZBQ
#endif
            }
        } else if (type == 2 || type == 3) { // flash
            flash_read_page(adr, n, rsp + 6);
        }
#if (TLKDBG_CFG_UDB_LOG_ENABLE)
        tlkdbg_send_status(0x82, 8, rsp, n + 6);
#endif
    }
    //////////////////////////  Memory Write ////////////////////////////////////
    else if (cmd == 0x2a && dataLen > 6) {
        uint8_t  type = pData[1];
        uint32_t addr = pData[2] | (pData[3] << 8) | (pData[4] << 16) | (pData[5] << 24);
        uint32_t temp = dataLen - 6;

        rsp[0] = 0x2b;
        tmemcpy(rsp + 1, pData + 1, 16);

#if (TLKDBG_CFG_UDB_LOG_ENABLE)
        tlkdbg_send_status(0x81, 8, pData, 12);
#endif
        if (type == 0) { // RAM
            tmemcpy((void *)addr, pData + 6, temp);
        } else if (type == 1) { // Analog Register
            for (unsigned int i = 0; i < temp; i++) {
                analog_write_reg8(addr + i, pData[i + 6]);
            }
        } else if (type == 2) { // flash
            if ((*pIsDown) && (addr & 0xFFF) == 0) {
                flash_erase_sector(addr);
            }
            flash_write_page(addr, temp, pData + 6);

        } else if (type == 3) {
            // tlksys_disable_interrupt();
            uint32_t index;
            uint32_t nbyte = pData[6];
            if (temp > 1) {
                nbyte += pData[7] << 8;
            }
            if (temp > 2) {
                nbyte += pData[8] << 16;
            }
            if (temp > 3) {
                nbyte += pData[9] << 24;
            }
            if (nbyte == 0) {
                nbyte = 0x100000;
            }
            for (index = 0; index < nbyte; index += 4096) {
                flash_erase_sector(addr + index);
            }
        } else if (type == 0xFE) { // FW_DOWNLOAD
            core_interrupt_disable();
            tlkusb_hal_disable_eventMode();
#if (TLKDBG_CFG_UDB_LOG_ENABLE)
            tlkdbg_stream_reset();
#endif
            *pIsDown = true;
        }
#if (TLKDBG_CFG_UDB_LOG_ENABLE)
        tlkdbg_send_status(0x82, 8, rsp, 14);
#endif
    } else if (cmd == 0x05) {
        tlkusb_eq_cfg_hook(pData, dataLen);
    }
}

/**
 * @brief     This function serves as a hook for the USB shell, allowing users to process USB input data.
 * @param[in] pData Pointer to the USB input data buffer.
 * @param[in] dataLen Length of the USB input data.
 * @returns   None.
 * @note      Users can overload this function to implement custom processing logic for USB input data.
 */
__attribute__((weak)) void tlkusb_debug_shell_hook(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
}

/**
 * @brief   Hook function for EQ configuration.
 * @param   pData Pointer to the data.
 * @param   dataLen Length of the data.
 * @return  None.
 */
__attribute__((weak)) void tlkusb_eq_cfg_hook(uint8_t *pData, uint16_t dataLen)
{
    (void)pData;
    (void)dataLen;
}

#endif // #if (TLK_USB_UDB_ENABLE)

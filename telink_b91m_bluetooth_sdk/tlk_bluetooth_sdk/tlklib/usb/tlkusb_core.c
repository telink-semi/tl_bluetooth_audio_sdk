/********************************************************************************************************
 * @file    tlkusb_core.c
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
#include "tlklib/usb/tlkusb_desc.h"
#include "tlklib/usb/tlkusb_core.h"
#include "tlklib/usb/udb/tlkusb_udbDefine.h"
#if (TLK_CFG_USB_ENABLE)

#include "drivers.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "tlklib/usb/uac/tlkusb_uac.h"


static void tlkusb_ctrlTranSetupProc(uint8_t index);
static void tlkusb_ctrlTranDataProc(uint8_t index);
static void tlkusb_ctrlTranStatusProc(uint8_t index);
static void tlkusb_ctrlTranSetupReqProc(uint8_t index, bool isSetupReq);
#if TLK_USB_REMOTEWAKEUP_EN
static void tlkusb_ctrlPowerDownStatusProc(uint8_t index, uint8_t type);
#endif
static void tlkusb_stdD2HDevReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_stdD2HInfReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_stdH2DInfReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_classD2HInfDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_classD2HEdpDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_classH2DInfDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_classH2DEdpDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_vendorD2HInfDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_vendorD2HDevDeal(uint8_t index, tlkusb_setup_req_t *pSetup);


static void tlkusb_getInfReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_setInfReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_getDeviceDescDeal(uint8_t index, tlkusb_setup_req_t *pSetup);
static void tlkusb_getReportDescDeal(uint8_t index, tlkusb_setup_req_t *pSetup);

typedef struct
{
    uint16_t usbID; // usb id
    uint8_t  stall; // stall
    uint8_t  rptr;
    uint16_t cmdLen;   // command length
    uint16_t rspLen;   // response length
    uint8_t *pRspData; // response data
} tlkusb_ctrl_t;

uint8_t                   gTlkUsbCurModType[TLK_CFG_USB_NUMB];
static tlkusb_ctrl_t      sTlkUsbCtrl[TLK_CFG_USB_NUMB];
static tlkusb_setup_req_t sTlkUsbCtrlReq;

static uint8_t gTlkUsbSuspend[TLK_CFG_USB_NUMB] = {0};
static uint8_t gTlkUsbSuspendEnable[TLK_CFG_USB_NUMB];

/**
 * @brief       This function initializes the USB core module with the specified USB ID.
 * @param[in]   index   - the USB instance index.
 * @param[in]   usbID   - the USB identifier.
 * @return      TLK_ENONE on success, otherwise is failure.
 */
int tlkusb_core_init(uint8_t index, uint16_t usbID)
{
    memset(&sTlkUsbCtrl[index], 0, sizeof(tlkusb_ctrl_t));

    sTlkUsbCtrl[index].usbID = usbID;
    tlkusb_module_init(gTlkUsbCurModType[index]);

    tlkusb_hal_core_init(index);
    return TLK_ENONE;
}

/**
 * @brief       This function gets the current mode of the USB module.
 * @param[in]   index   - the USB instance index.
 * @return      the current mode of the USB module.
 */
uint8_t tlkusb_get_curMode(uint8_t index)
{
    return gTlkUsbCurModType[index];
}

/**
 * @brief       This function enable or disable usb suspend.
 * @param[in]   index   - the USB instance index.
 * @return
 */
void tlkusb_set_usb_suspend_enable(uint8_t index, uint8_t en)
{
    if (index >= TLK_CFG_USB_NUMB) {
        return;
    }
    gTlkUsbSuspendEnable[index] = en;
}

/**
 * @brief       This function handles USB core events and processes control transfers.
 * @param[in]   index   - the USB instance index.
 * @return      none.
 */
void tlkusb_core_handler(uint8_t index)
{
#if (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X)
    uint32_t irq = tlkusb_hal_get_ctrl_ep_irq(index);
    if (irq & FLD_USB1_CTRL_EP_IRQ_SETUP) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_USB1_CTRL_EP_IRQ_SETUP);
        tlkusb_ctrlTranSetupProc(index);
    }
    if (irq & FLD_USB1_CTRL_EP_IRQ_DATA) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_USB1_CTRL_EP_IRQ_DATA);
        tlkusb_ctrlTranDataProc(index);
    }
    if (irq & FLD_USB1_CTRL_EP_IRQ_STA) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_USB1_CTRL_EP_IRQ_STA);
        tlkusb_ctrlTranStatusProc(index);
    }
#if (MCU_CORE_TYPE == CHIP_TYPE_TL752X)
    if (usb1hw_get_irq_status(USB1_IRQ_RESET_STATUS)) {
        usb1hw_clr_irq_status(USB1_IRQ_RESET_STATUS); /* clear USB reset flag */
                                                      //            myudb_usb_bulkout_ready();
        reg_usb1_ep_ctrl(TLKUSB_UDB_EDP_DBG_OUT) = FLD_USB1_EP_DAT_ACK;
    }
#endif
#else
    uint32_t irq = tlkusb_hal_get_ctrl_ep_irq(index);
    if (irq & FLD_CTRL_EP_IRQ_SETUP) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_CTRL_EP_IRQ_SETUP);
        tlkusb_ctrlTranSetupProc(index);
    }
    if (irq & FLD_CTRL_EP_IRQ_DATA) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_CTRL_EP_IRQ_DATA);
        tlkusb_ctrlTranDataProc(index);
    }
    if (irq & FLD_CTRL_EP_IRQ_STA) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_CTRL_EP_IRQ_STA);
        tlkusb_ctrlTranStatusProc(index);
    }
#if TLK_USB_REMOTEWAKEUP_EN
    if (irq & FLD_USB_IRQ_SUSPEND_STATUS) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_USB_IRQ_SUSPEND_STATUS);
        if (usbhw_get_host_conn_status() && usbhw_get_wkup_feature() && gTlkUsbSuspend[index]) {
            tlkusb_ctrlPowerDownStatusProc(index, 1);
        } else {
            //    		plic_interrupt_enable(IRQ_USB_PWDN);
        }
    }
#endif
#endif

#if (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X)
    if (irq & FLD_USB1_IRQ_RESET_STATUS) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_USB1_IRQ_RESET_STATUS);
        tlkusb_module_reset(gTlkUsbCurModType[index]);
    }
#elif MCU_CORE_TYPE != MCU_CORE_B91
    if (irq & FLD_USB_IRQ_RESET_STATUS) {
        tlkusb_hal_clr_ctrl_ep_irq(index, FLD_USB_IRQ_RESET_STATUS);
        tlkusb_module_reset(gTlkUsbCurModType[index]);
        gTlkUsbSuspend[index] = 0;
    }
#endif
    sTlkUsbCtrl[index].stall = 0;
    tlkusb_module_handler(gTlkUsbCurModType[index]);
}

/**
 * @brief       This function sends response data to the USB control endpoint.
 * @param[in]   index   - the USB instance index.
 * @return      none.
 */
static void tlkusb_ctrlSendResponse(uint8_t index)
{
    uint16_t len;

    if (sTlkUsbCtrl[index].rspLen == 0) {
        return;
    }
    if (sTlkUsbCtrl[index].rspLen >= 8) {
        len = 8;
    } else {
        len = sTlkUsbCtrl[index].rspLen;
    }
    sTlkUsbCtrl[index].rspLen -= len;
    tlkusb_hal_reset_ctrl_ep_ptr(index);
    while (len-- > 0) {
        tlkusb_hal_write_ctrl_ep_data(index, *sTlkUsbCtrl[index].pRspData);
        ++sTlkUsbCtrl[index].pRspData;
    }
}

/**
 * @brief       This function processes USB control transfer setup stage.
 * @param[in]   index   - the USB instance index.
 * @return      none.
 */
static void tlkusb_ctrlTranSetupProc(uint8_t index)
{
    (void)index;
    sTlkUsbCtrl[index].stall = 0;
    tlkusb_hal_reset_ctrl_ep_ptr(index);
    sTlkUsbCtrlReq.bReqType = tlkusb_hal_read_ctrl_ep_data(index);
    sTlkUsbCtrlReq.bRequest = tlkusb_hal_read_ctrl_ep_data(index);
    sTlkUsbCtrlReq.wValue   = tlkusb_hal_read_ctrl_ep_u16(index);
    sTlkUsbCtrlReq.wIndex   = tlkusb_hal_read_ctrl_ep_u16(index);
    sTlkUsbCtrlReq.wLength  = tlkusb_hal_read_ctrl_ep_u16(index);
    tlkusb_ctrlTranSetupReqProc(index, true);
#if (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X)
    if (sTlkUsbCtrl[index].stall) {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_USB1_EP_DAT_STALL);
    } else {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_USB1_EP_DAT_ACK);
    }
#else
    if (sTlkUsbCtrl[index].stall) {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_EP_DAT_STALL);
    } else {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_EP_DAT_ACK);
    }
#endif
}

/**
 * @brief       This function processes USB control transfer data stage.
 * @param[in]   index   - the USB instance index.
 * @return      none.
 */
static void tlkusb_ctrlTranDataProc(uint8_t index)
{
    (void)index;
    sTlkUsbCtrl[index].stall = 0;
    tlkusb_hal_reset_ctrl_ep_ptr(index);
    tlkusb_ctrlTranSetupReqProc(index, false);
#if (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X)
    if (sTlkUsbCtrl[index].stall) {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_USB1_EP_DAT_STALL);
    } else {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_USB1_EP_DAT_ACK);
    }
#else
    if (sTlkUsbCtrl[index].stall) {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_EP_DAT_STALL);
    } else {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_EP_DAT_ACK);
    }
#endif
}

/**
 * @brief       This function processes USB control transfer status stage.
 * @param[in]   index   - the USB instance index.
 * @return      none.
 */
static void tlkusb_ctrlTranStatusProc(uint8_t index)
{
    (void)index;
#if (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X)
    if (sTlkUsbCtrl[index].stall) {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_USB1_EP_STA_STALL);
    } else {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_USB1_EP_STA_ACK);
    }
#else
    if (sTlkUsbCtrl[index].stall) {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_EP_STA_STALL);
    } else {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_EP_STA_ACK);
    }
#endif
}
#if TLK_USB_REMOTEWAKEUP_EN
/**
 * @brief       This function processes USB power down status stage.
 * @param[in]   index   - the USB instance index.
 * @return      none.
 */
static void tlkusb_ctrlPowerDownStatusProc(uint8_t index, uint8_t type)
{
    (void)index;
    (void)type;

    if (gTlkUsbSuspendEnable[index]) {
        tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB_SUSPEND);
    }
#if (MCU_CORE_TYPE == MCU_CORE_TL322X) || (MCU_CORE_TYPE == MCU_CORE_TL752X)
    if (sTlkUsbCtrl[index].stall) {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_USB1_EP_STA_STALL);
    } else {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_USB1_EP_STA_ACK);
    }
#else
    if (sTlkUsbCtrl[index].stall) {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_EP_STA_STALL);
    } else {
        tlkusb_hal_write_ctrl_ep_ctrl(index, FLD_EP_STA_ACK);
    }
#endif
}
#endif
/**
 * @brief       This function processes USB control transfer setup requests based on request type.
 * @param[in]   index       - the USB instance index.
 * @param[in]   isSetupReq  - flag indicating if this is a setup request.
 * @return      none.
 */
static void tlkusb_ctrlTranSetupReqProc(uint8_t index, bool isSetupReq)
{
    uint8_t bmReqType = sTlkUsbCtrlReq.bReqType;
    uint8_t bmRequest = sTlkUsbCtrlReq.bRequest;

    tlkusb_hal_reset_ctrl_ep_ptr(index);
    switch (bmReqType) {
    case (TLKUSB_REQTYPE_DIR_DEV2HOST | TLKUSB_REQTYPE_MAJ_STAND | TLKUSB_REQTYPE_REC_DEVICE):
        if (isSetupReq) {
            tlkusb_stdD2HDevReqDeal(index, &sTlkUsbCtrlReq);
        }
        tlkusb_ctrlSendResponse(index);
        break;
    case (TLKUSB_REQTYPE_DIR_DEV2HOST | TLKUSB_REQTYPE_MAJ_STAND | TLKUSB_REQTYPE_REC_INTERFACE):
        if (isSetupReq) {
            tlkusb_stdD2HInfReqDeal(index, &sTlkUsbCtrlReq);
        }
        tlkusb_ctrlSendResponse(index);
        break;
    case (TLKUSB_REQTYPE_DIR_HOST2DEV | TLKUSB_REQTYPE_MAJ_STAND | TLKUSB_REQTYPE_REC_INTERFACE):
        if (isSetupReq) {
            tlkusb_stdH2DInfReqDeal(index, &sTlkUsbCtrlReq);
        }
        break;
    case (TLKUSB_REQTYPE_DIR_HOST2DEV | TLKUSB_REQTYPE_MAJ_STAND | TLKUSB_REQTYPE_REC_DEVICE):
    {
        if (bmRequest == 3) {
            gTlkUsbSuspend[index] = 1;
        } else if (bmRequest == 1) {
            gTlkUsbSuspend[index] = 0;
#if TLK_USB_REMOTEWAKEUP_EN
            if (gTlkUsbSuspendEnable[index]) {
                tlksys_task_setEvt(TLKSYS_TASKID_SYSTEM, TLKSYS_TASK_EVT_SYS_USB_EXIT_SUSPEND);
            }
#endif
        }

    } break;
    case (TLKUSB_REQTYPE_DIR_DEV2HOST | TLKUSB_REQTYPE_MAJ_CLASS | TLKUSB_REQTYPE_REC_INTERFACE):
        //        if (isSetupReq)
        {
            tlkusb_classD2HInfDeal(index, &sTlkUsbCtrlReq);
        }
        break;
    case (TLKUSB_REQTYPE_DIR_DEV2HOST | TLKUSB_REQTYPE_MAJ_CLASS | TLKUSB_REQTYPE_REC_ENDPOINT):
        if (isSetupReq) {
            tlkusb_classD2HEdpDeal(index, &sTlkUsbCtrlReq);
        }
        break;
    case (TLKUSB_REQTYPE_DIR_HOST2DEV | TLKUSB_REQTYPE_MAJ_CLASS | TLKUSB_REQTYPE_REC_INTERFACE):
        if (!isSetupReq) {
            tlkusb_classH2DInfDeal(index, &sTlkUsbCtrlReq);
        }
        break;
    case (TLKUSB_REQTYPE_DIR_HOST2DEV | TLKUSB_REQTYPE_MAJ_CLASS | TLKUSB_REQTYPE_REC_ENDPOINT):
        if (!isSetupReq) {
            tlkusb_classH2DEdpDeal(index, &sTlkUsbCtrlReq);
        }
        break;
    case (TLKUSB_REQTYPE_DIR_DEV2HOST | TLKUSB_REQTYPE_MAJ_VENDOR | TLKUSB_REQTYPE_REC_INTERFACE):
        if (!isSetupReq) {
            return;
        }
        tlkusb_vendorD2HInfDeal(index, &sTlkUsbCtrlReq);
        break;
    case (TLKUSB_REQTYPE_DIR_DEV2HOST | TLKUSB_REQTYPE_MAJ_VENDOR | TLKUSB_REQTYPE_REC_DEVICE):
        if (!isSetupReq) {
            return;
        }
        tlkusb_vendorD2HDevDeal(index, &sTlkUsbCtrlReq);
        break;
    case (TLKUSB_REQTYPE_DIR_HOST2DEV | TLKUSB_REQTYPE_MAJ_VENDOR | TLKUSB_REQTYPE_REC_DEVICE): // 0x40
        sTlkUsbCtrl[index].stall = 1;
        break;
    default:
        sTlkUsbCtrl[index].stall = 1;
        break;
    }
}

/**
 * @brief       This function deals with standard device to host requests for device recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_stdD2HDevReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    sTlkUsbCtrl[index].rspLen = 0;
    if (pSetup->bRequest == TLKUSB_SETUP_REQ_GET_DESCRIPT) {
        tlkusb_getDeviceDescDeal(index, pSetup);
    }

    return;
}

/**
 * @brief       This function deals with standard device to host requests for interface recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_stdD2HInfReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    sTlkUsbCtrl[index].rspLen = 0;
    if (pSetup->bRequest == TLKUSB_SETUP_REQ_GET_DESCRIPT) {
        tlkusb_getReportDescDeal(index, pSetup);
    } else if (pSetup->bRequest == TLKUSB_SETUP_REQ_GET_INTERFACE) {
        tlkusb_getInfReqDeal(index, pSetup);
    }
}

/**
 * @brief       This function deals with standard host to device requests for interface recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_stdH2DInfReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    if (pSetup->bRequest == TLKUSB_SETUP_REQ_SET_INTERFACE) {
        tlkusb_setInfReqDeal(index, pSetup);
    }
}

/**
 * @brief       This function deals with class device to host requests for interface recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_classD2HInfDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    int     ret;
    uint8_t infNum;

    infNum = (pSetup->wIndex & 0xFF);
    if (pSetup->bRequest == 0x00) {
        tlkusb_hal_write_ctrl_ep_data(index, 0x00);
        return;
    }

    ret = tlkusb_module_getClassInf(gTlkUsbCurModType[index], pSetup, infNum);
    if (ret != TLK_ENONE) {
        sTlkUsbCtrl[index].stall = true;
    } else {
        sTlkUsbCtrl[index].stall = false;
    }
}

/**
 * @brief       This function deals with class device to host requests for endpoint recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_classD2HEdpDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    int     ret;
    uint8_t edpNum;

    edpNum = (pSetup->wIndex & 0x0F);
    ret    = tlkusb_module_getClassEdp(gTlkUsbCurModType[index], pSetup, edpNum);
    if (ret != TLK_ENONE) {
        sTlkUsbCtrl[index].stall = true;
    } else {
        sTlkUsbCtrl[index].stall = false;
    }
}

/**
 * @brief       This function deals with class host to device requests for interface recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_classH2DInfDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    int     ret;
    uint8_t infNum;

    infNum = (pSetup->wIndex & 0xFF);
    ret    = tlkusb_module_setClassInf(gTlkUsbCurModType[index], pSetup, infNum);
    if (ret != TLK_ENONE) {
        sTlkUsbCtrl[index].stall = true;
    } else {
        sTlkUsbCtrl[index].stall = false;
    }
}

/**
 * @brief       This function deals with class host to device requests for endpoint recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_classH2DEdpDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    int     ret;
    uint8_t edpNum;

    edpNum = (pSetup->wIndex & 0x0F);
    ret    = tlkusb_module_setClassEdp(gTlkUsbCurModType[index], pSetup, edpNum);
    if (ret != TLK_ENONE) {
        sTlkUsbCtrl[index].stall = true;
    } else {
        sTlkUsbCtrl[index].stall = false;
    }
}

/**
 * @brief       This function deals with vendor device to host requests for interface recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_vendorD2HInfDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    if (pSetup->bRequest == 0xc0) { // Get board version
        tlkusb_hal_reset_ctrl_ep_ptr(index);
        tlkusb_hal_write_ctrl_ep_data(index, sTlkUsbCtrl[index].usbID);
        tlkusb_hal_write_ctrl_ep_data(index, sTlkUsbCtrl[index].usbID >> 8);
    } else {
        sTlkUsbCtrl[index].stall = 1;
    }
}

/**
 * @brief       This function deals with vendor device to host requests for device recipient.
 * @param[in]   index    - the USB instance index.
 * @param[in]   pSetup   - pointer to the USB setup request.
 * @return      none.
 */
static void tlkusb_vendorD2HDevDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    if (pSetup->bRequest == 0xc0) { // Get board version
        tlkusb_hal_reset_ctrl_ep_ptr(index);
        tlkusb_hal_write_ctrl_ep_data(index, 0x40);
        tlkusb_hal_write_ctrl_ep_data(index, 0x25);
        tlkusb_hal_write_ctrl_ep_data(index, 0x40);
        tlkusb_hal_write_ctrl_ep_data(index, 0x05);
        tlkusb_hal_write_ctrl_ep_data(index, 0x03);
        tlkusb_hal_write_ctrl_ep_data(index, 0x00);
        tlkusb_hal_write_ctrl_ep_data(index, 0x01);
        tlkusb_hal_write_ctrl_ep_data(index, 0x00);
    } else if (pSetup->bRequest == 0xc6) {
        tlkusb_hal_reset_ctrl_ep_ptr(index);
        tlkusb_hal_write_ctrl_ep_data(index, 0x04);
    } else {
        sTlkUsbCtrl[index].stall = 1;
    }
}

/**
 * @brief       This function handles the USB GET_INTERFACE request.
 * @param[in]   index   - the USB instance index.
 * @param[in]   pSetup  - pointer to the USB setup request containing request details.
 * @return      None
 */
static void tlkusb_getInfReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    int     ret;
    uint8_t infNum = (pSetup->wIndex) & 0x07;
    ret            = tlkusb_module_getInterface(gTlkUsbCurModType[index], pSetup, infNum);
    if (ret == TLK_ENONE) {
        sTlkUsbCtrl[index].stall = 0;
    } else {
        sTlkUsbCtrl[index].stall = 1;
    }
}

/**
 * @brief       This function handles the USB SET_INTERFACE request for a specific interface.
 * @param[in]   index   - the USB controller index.
 * @param[in]   pSetup  - pointer to the USB setup request structure containing request details.
 * @returns     None.
 */
static void tlkusb_setInfReqDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    int     ret;
    uint8_t infNum = (pSetup->wIndex) & 0x07;
    ret            = tlkusb_module_setInterface(gTlkUsbCurModType[index], pSetup, infNum);
    if (ret == TLK_ENONE) {
        sTlkUsbCtrl[index].stall = 0;
    } else {
        sTlkUsbCtrl[index].stall = 1;
    }
}

/**
 * @brief       This function handles USB device descriptor requests based on the specified descriptor type.
 * @param[in]   index   - the USB instance index.
 * @param[in]   pSetup  - pointer to the USB setup request containing the descriptor type and other parameters.
 * @return      none.
 */
static void tlkusb_getDeviceDescDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    uint8_t mode    = gTlkUsbCurModType[index];
    uint8_t value_l = (pSetup->wValue) & 0xff;
    uint8_t value_h = (pSetup->wValue >> 8) & 0xff;
    switch (value_h) {
    case TLKUSB_TYPE_DEVICE:
        sTlkUsbCtrl[index].pRspData = tlkusb_getDeviceDesc(mode);
        sTlkUsbCtrl[index].rspLen   = tlkusb_getDeviceLens(mode);
        break;
    case TLKUSB_TYPE_CONFIGURE:
        sTlkUsbCtrl[index].pRspData = tlkusb_getConfigDesc(mode);
        sTlkUsbCtrl[index].rspLen   = tlkusb_getConfigLens(mode);
        break;
    case TLKUSB_TYPE_STRING:
        if (TLKUSB_STRING_INDEX_LANGUAGE == value_l) {
            sTlkUsbCtrl[index].pRspData = tlkusb_getLanguageDesc(mode);
            sTlkUsbCtrl[index].rspLen   = tlkusb_getLanguageLens(mode);
        } else if (TLKUSB_STRING_INDEX_VENDOR == value_l) {
            sTlkUsbCtrl[index].pRspData = tlkusb_getVendorDesc(mode);
            sTlkUsbCtrl[index].rspLen   = tlkusb_getVendorLens(mode);
        } else if (TLKUSB_STRING_INDEX_PRODUCT == value_l) {
            sTlkUsbCtrl[index].pRspData = tlkusb_getProductDesc(mode);
            sTlkUsbCtrl[index].rspLen   = tlkusb_getProductLens(mode);
        } else if (TLKUSB_STRING_INDEX_SERIAL == value_l) {
            sTlkUsbCtrl[index].pRspData = tlkusb_getSerialDesc(mode);
            sTlkUsbCtrl[index].rspLen   = tlkusb_getSerialLens(mode);
        } else if (TLKUSB_STRING_INDEX_SERIAL1 == value_l) {
            sTlkUsbCtrl[index].pRspData = tlkusb_getSerial1Desc(mode);
            sTlkUsbCtrl[index].rspLen   = tlkusb_getSerial1Lens(mode);
        } else {
            sTlkUsbCtrl[index].stall = 1;
        }
        break;
    default:
        sTlkUsbCtrl[index].stall = 1;
        break;
    }
    if (pSetup->wLength < sTlkUsbCtrl[index].rspLen) {
        sTlkUsbCtrl[index].rspLen = pSetup->wLength;
    }
}

/**
 * @brief       This function handles the GET_REPORT_DESCRIPTOR request for USB HID devices.
 * @param       index   - the USB instance index.
 * @param       pSetup  - pointer to the USB setup request containing request details.
 * @return      None
 */
static void tlkusb_getReportDescDeal(uint8_t index, tlkusb_setup_req_t *pSetup)
{
    sTlkUsbCtrl[index].pRspData = tlkusb_module_getInfDesDesc(gTlkUsbCurModType[index], pSetup);
    sTlkUsbCtrl[index].rspLen   = tlkusb_module_getInfDesLens(gTlkUsbCurModType[index], pSetup);
    if (sTlkUsbCtrl[index].rspLen == 0 || sTlkUsbCtrl[index].pRspData == NULL) {
        sTlkUsbCtrl[index].stall = 1;
    } else {
        if (pSetup->wLength < sTlkUsbCtrl[index].rspLen) {
            sTlkUsbCtrl[index].rspLen = pSetup->wLength;
        }
    }
}


#endif // #if (TLK_CFG_USB_ENABLE)

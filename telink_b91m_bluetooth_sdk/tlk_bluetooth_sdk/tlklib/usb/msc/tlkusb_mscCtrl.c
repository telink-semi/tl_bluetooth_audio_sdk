/********************************************************************************************************
 * @file    tlkusb_mscCtrl.c
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
#if (TLK_USB_MSC_ENABLE)
#include "tlklib/usb/msc/tlkusb_mscDefine.h"
#include "tlklib/usb/msc/tlkusb_msc.h"
#include "tlklib/usb/msc/tlkusb_mscDesc.h"
#include "tlklib/usb/msc/tlkusb_mscScsi.h"


static int  tlkusb_mscctrl_init(void);
static void tlkusb_mscctrl_reset(void);
static void tlkusb_mscctrl_handler(void);
static int  tlkusb_mscctrl_getClassInf(tlkusb_setup_req_t *pSetup, uint8_t infNumb);

/**
 * @brief       Set class interface.
 * @param[in]   pSetup - Setup request.
 * @param[in]   infNumb - Interface number.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkusb_mscctrl_setClassInf(tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
	(void) pSetup;
	(void) infNumb;
	return -TLK_ENOSUPPORT;
}

/**
 * @brief       Get class endpoint.
 * @param[in]   pSetup - Setup request.
 * @param[in]   edpNumb - Endpoint number.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkusb_mscctrl_getClassEdp(tlkusb_setup_req_t *pSetup, uint8_t edpNumb)
{
	(void) pSetup;
	(void) edpNumb;
	return TLK_ENONE;
}

/**
 * @brief       Set class endpoint.
 * @param[in]   pSetup - Setup request.
 * @param[in]   edpNumb - Endpoint number.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkusb_mscctrl_setClassEdp(tlkusb_setup_req_t *pSetup, uint8_t edpNumb)
{
	(void) pSetup;
	(void) edpNumb;
	return TLK_ENONE;
}

/**
 * @brief       Get interface.
 * @param[in]   pSetup - Setup request.
 * @param[in]   infNumb - Interface number.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkusb_mscctrl_getInterface(tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
	(void) pSetup;
	(void) infNumb;
	return TLK_ENONE;
}

/**
 * @brief       Set interface.
 * @param[in]   pSetup - Setup request.
 * @param[in]   infNumb - Interface number.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkusb_mscctrl_setInterface(tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
	(void) pSetup;
	(void) infNumb;
	return TLK_ENONE;
}


const tlkusb_modCtrl_t sTlkUsbMscModCtrl = {
	.Init         = tlkusb_mscctrl_init, 
	.Reset        = tlkusb_mscctrl_reset, 
	.Handler      = tlkusb_mscctrl_handler,
	.GetClassInf  = tlkusb_mscctrl_getClassInf,
	.SetClassInf  = tlkusb_mscctrl_setClassInf,
	.GetClassEdp  = tlkusb_mscctrl_getClassEdp,
	.SetClassEdp  = tlkusb_mscctrl_setClassEdp,
	.GetInterface = tlkusb_mscctrl_getInterface,
	.SetInterface = tlkusb_mscctrl_setInterface,
};

/**
 * @brief       Initialize the MSC control module.
 * @param[in]   none.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkusb_mscctrl_init(void)
{
	int index;
	uint8_t isoMode;
	tlkusb_msc_disk_t *pDisk;
	
	tlkusb_msc_scsiInit();
	
	
	for(index=0; index<TLKUSB_MSC_UNIT_COUNT; index++){
		pDisk = tlkusb_msc_getDisk(index);
		if(pDisk != NULL && pDisk->Init != NULL){
			pDisk->Init();
		}
	}
	
	usbhw_enable_manual_interrupt(FLD_CTRL_EP_AUTO_STD | FLD_CTRL_EP_AUTO_DESC | FLD_CTRL_EP_AUTO_INTF);
	usbhw_set_eps_max_size(TLKUSB_MSC_REC_BUFFER_LEN);
	usbhw_set_ep_addr((uint8_t)TLKUSB_MSC_EDP_OUT, 0);
    usbhw_set_ep_addr((uint8_t)TLKUSB_MSC_EDP_IN, TLKUSB_MSC_REC_BUFFER_LEN);
    

	isoMode = reg_usb_iso_mode;
	isoMode &= ~ BIT(TLKUSB_MSC_EDP_IN);
	isoMode &= ~ BIT(TLKUSB_MSC_EDP_OUT);
	reg_usb_iso_mode = isoMode;
	reg_usb_ep8_fifo_mode = 0;
	usbhw_reset_ep_ptr((uint8_t)TLKUSB_MSC_EDP_OUT);
	reg_usb_ep_ctrl(TLKUSB_MSC_EDP_OUT) = FLD_EP_DAT_ACK;
	usb_set_pin(1);
	usbhw_set_eps_irq_mask(FLD_USB_EDP5_IRQ);
	return TLK_ENONE;
}

/**
 * @brief       Reset the MSC control module.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_mscctrl_reset(void)
{
	reg_usb_ep_ctrl(TLKUSB_MSC_EDP_OUT) = FLD_EP_DAT_ACK; 
	tlkusb_msc_scsiReset();
}

/**
 * @brief       Handler for the MSC control module.
 * @param[in]   none.
 * @return      none.
 */
static void tlkusb_mscctrl_handler(void)
{
	tlkusb_msc_scsiHandler();
}


/**
 * @brief       Get class interface.
 * @param[in]   pSetup - Setup request.
 * @param[in]   infNumb - Interface number.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
static int tlkusb_mscctrl_getClassInf(tlkusb_setup_req_t *pSetup, uint8_t infNumb)
{
	if(infNumb != TLKUSB_MSC_INF_MSC){
		return -TLK_ENOSUPPORT;
	}
	if(pSetup->bRequest == MS_REQ_GetMaxLUN){
		uint8_t count = tlkusb_msc_getDiskCount();
		if(count != 0){
			usbhw_write_ctrl_ep_data(count-1);
			return TLK_ENONE;
		}
	}else if(pSetup->bRequest == MS_REQ_MassStorageReset){
		return TLK_ENONE;
	}
	return -TLK_ENOSUPPORT;
}

#endif //#if (TLK_USB_MSC_ENABLE)



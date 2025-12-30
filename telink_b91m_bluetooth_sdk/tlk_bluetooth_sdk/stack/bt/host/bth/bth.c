/********************************************************************************************************
 * @file    bth.c
 *
 * @brief   This is the source file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "drivers.h"
#include "tlkapi/tlkapi.h"
#include "stack/bt/host/bth/bth_stdio.h"
#include "stack/bt/common/co_bt_defines.h"

/******************************************************************************
 * Function: BTH Init interface
 * Descript: This interface be used to initial bth layer.
 * Params:
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int bth_init(void)
{
    int ret;

    ret = TLK_ENONE;
    if (ret == TLK_ENONE) {
        ret = bth_hcievt_init();
    }
    if (ret == TLK_ENONE) {
        ret = bth_handle_init(TLK_STK_BTACL_NUMB, TLK_STK_BTSCO_NUMB);
    }
    if (ret == TLK_ENONE) {
        ret = bth_l2cap_init(TLK_STK_BTPSM_NUMB, TLK_STK_BTCHN_NUMB, TLK_STK_BTACL_NUMB);
    }
    if (ret == TLK_ENONE) {
        ret = bth_sco_setTxRxBandwidth(SYNC_BANDWIDTH_DONT_CARE, SYNC_BANDWIDTH_DONT_CARE);
    }
    if (ret != TLK_ENONE) {
        bth_deinit();
    }
    return TLK_ENONE;
}

void bth_deinit(void)
{
    bth_handle_deinit();
    bth_l2cap_deinit();
}

int bth_needMemLen(void)
{
    int needLen = 0;
    needLen += bth_handle_getMemLen(TLK_STK_BTACL_NUMB, TLK_STK_BTSCO_NUMB);
    needLen += bth_l2cap_getMemLen(TLK_STK_BTPSM_NUMB, TLK_STK_BTCHN_NUMB, TLK_STK_BTACL_NUMB);
    return needLen;
}

bool bth_isBusy(void)
{
    return false;
}

bool bth_pmIsBusy(void)
{
    uint8_t           count;
    bth_acl_handle_t *pHandle;

    count = bth_handle_getUsedAclCount();
    if (count == 0) {
        return false;
    }
    if (count != 1) {
        return true;
    }

    pHandle = bth_handle_getFirstConnAcl();
    if (pHandle != NULL && pHandle->curRole == BTH_ROLE_SLAVE) {
        return false;
    }
    return true;
}

uint bth_getAclCount(void)
{
    return bth_handle_getUsedAclCount();
}

/******************************************************************************
 * Function: BTH Destroy interface
 * Descript: This interface be used to release the resource which allocate.
 * Params: @aclhandle[IN]--The acl link handle.
 * Return: None.
 *******************************************************************************/
void bth_destroy(uint16_t aclHandle)
{
    bth_acl_destroy(aclHandle);
    bth_sco_destroyByAclHandle(aclHandle);
    bth_l2cap_aclDisconn(aclHandle);
}

/******************************************************************************
 * Function: bth_isHeadset
 * Descript: This interface be used to verify peer device is headset or
 *           others device.
 * Params: @aclhandle[IN]--The acl link handle.
 * Return: Return true means peer device type headset, false means others .
 *******************************************************************************/
bool bth_isHeadset(uint16_t aclHandle)
{
    uint8_t           devType;
    bth_acl_handle_t *pHandle;

    pHandle = bth_handle_getUsedAcl(aclHandle);
    if (pHandle == NULL) {
        return false;
    }

    devType = bth_devClassToDevType(pHandle->devClass);
    if (devType == BTH_REMOTE_DTYPE_HEADSET) {
        return true;
    }
    return false;
}

/******************************************************************************
 * Function: bth_devClassToDevType
 * Descript: This interface be used to reflect from devClass to device type.
 * Params: @devClass[IN]--The device class.
 * Return: The Device type.
 *******************************************************************************/
uint8_t bth_devClassToDevType(uint32_t devClass)
{
    uint8_t dtype;

    if (devClass == 0) {
        return BTH_REMOTE_DTYPE_OTHER;
    }

    uint32_t majorDType = (devClass & BTH_COD_MAJOR_CLASS_MASK);

    if (majorDType == BTH_COD_MAJOR_CLASS_COMPUTER) {
        dtype = BTH_REMOTE_DTYPE_COMPUTER; // TLKMDI_BTINQ_DTYPE_PC;
    } else if (majorDType == BTH_COD_MAJOR_CLASS_PHONE) {
        dtype = BTH_REMOTE_DTYPE_PHONE; // TLKMDI_BTINQ_DTYPE_PHONE;
    } else if (majorDType == BTH_COD_MAJOR_CLASS_AUDIO) {
        dtype = BTH_REMOTE_DTYPE_HEADSET; // TLKMDI_BTINQ_DTYPE_HEADSET;
    } else if (majorDType != BTH_COD_MAJOR_CLASS_MISC || (devClass & BTH_COD_SERVICE_CLASS_AUDIO) == 0 || (devClass & BTH_COD_SERVICE_CLASS_RENDER) == 0) {
        dtype = BTH_REMOTE_DTYPE_OTHER; // TLKMDI_BTINQ_DTYPE_UNKNOWN;
    } else {
        dtype = BTH_REMOTE_DTYPE_OTHER;
    }

#ifdef TLK_CFG_CLASS_FILTER_FOR_UART_TOOL
    // In order to be able to search for and connect the controller device(Game Pad) on the uart tool.
    if (majorDType == BTH_COD_MAJOR_CLASS_PERIPHERAL) {
        dtype = BTH_REMOTE_DTYPE_COMPUTER; // select 'PC' when use uart tool search Game Pad.
    }
#endif
    return dtype;
}

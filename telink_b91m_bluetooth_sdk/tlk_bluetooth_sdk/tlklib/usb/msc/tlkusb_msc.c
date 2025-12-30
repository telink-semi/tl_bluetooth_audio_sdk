/********************************************************************************************************
 * @file    tlkusb_msc.c
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

extern const tlkusb_modCtrl_t sTlkUsbMscModCtrl;
extern const tlkusb_modDesc_t sTlkUsbMscModDesc;
const tlkusb_module_t         gTlkUsbMscModule = {
            .modType = TLKUSB_MODTYPE_MSC,
            .pDesc   = &sTlkUsbMscModDesc,
            .pCtrl   = &sTlkUsbMscModCtrl,
};
static uint8_t            sTlkUsbMscUnitCount = 0;
static tlkusb_msc_disk_t *spTlkUsbMscUnit[TLKUSB_MSC_UNIT_COUNT];

/**
 * @brief       Append a disk to the MSC module.
 * @param[in]   pUnit - Pointer to the disk unit to append.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkusb_msc_appendDisk(tlkusb_msc_disk_t *pUnit)
{
    if (pUnit == NULL || pUnit->blkSize == 0 || (pUnit->blkSize & 0x3F) != 0 || pUnit->Read == NULL || pUnit->Write == NULL) {
        return -TLK_EPARAM;
    }
    spTlkUsbMscUnit[sTlkUsbMscUnitCount++] = pUnit;
    return TLK_ENONE;
}

/**
 * @brief       Get the count of disks in the MSC module.
 * @param[in]   none.
 * @return      Number of disks.
 */
uint8_t tlkusb_msc_getDiskCount(void)
{
    return sTlkUsbMscUnitCount;
}

/**
 * @brief       Get a disk by volume number.
 * @param[in]   volNum - Volume number.
 * @return      Pointer to the disk unit.
 */
tlkusb_msc_disk_t *tlkusb_msc_getDisk(uint8_t volNum)
{
    if (volNum >= sTlkUsbMscUnitCount) {
        return NULL;
    }
    return spTlkUsbMscUnit[volNum];
}

#endif //#if (TLK_USB_MSC_ENABLE)

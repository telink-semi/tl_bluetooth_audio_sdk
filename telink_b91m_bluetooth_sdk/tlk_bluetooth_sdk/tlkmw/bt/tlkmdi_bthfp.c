/********************************************************************************************************
 * @file    tlkmdi_bthfp.c
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
#if (TLKBTP_CFG_HFP_ENABLE)
#include "tlkmw/bt/tlkmw_bt.h"
#include "stack/bt/host/btp/btp_stdio.h"


#define TLKMDI_BTHFP_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_BT << 24) | (TLK_MINOR_DBGID_MDI_BT_HFP << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_BTHFP_DBG_SIGN "[MHFP]"

/**
 * @brief       This function initializes the HFP module
 * @param[in]   none.
 * @return      TLK_ENONE is success, others is failure.
 */
int tlkmdi_bthfp_init(void)
{
#if (TLKBTP_CFG_HFPAG_ENABLE)
    tlkmdi_bthfpag_init();
#endif

#if (TLKBTP_CFG_HFPHF_ENABLE)
    tlkmdi_bthfphf_init();
#endif

    return TLK_ENONE;
}

/**
 * @brief       This function destroys the HFP module by handle
 * @param[in]   aclHandle   - ACL connection handle
 * @return      none.
 */
void tlkmdi_bthfp_destroy(uint16_t aclHandle)
{
#if (TLKBTP_CFG_HFPAG_ENABLE)
    (void)aclHandle;
    tlkmdi_bthfpag_reset();
#endif
#if (TLKBTP_CFG_HFPHF_ENABLE)
    tlkmdi_bthfphf_reset(aclHandle);
#endif
}

#endif // #if (TLKBTP_CFG_HFP_ENABLE)
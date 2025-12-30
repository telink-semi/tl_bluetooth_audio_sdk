/********************************************************************************************************
 * @file    tlkmw_test_mode.c
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
#include "tlkmw/tlkmw.h"
#include "tlkmw_test_mode.h"

/**
 * @brief       This function checks EMI test mode.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_test_mode_emi_check(void)
{
    uint8_t mode = tlkmdi_tinySql_getWorkMode();
    if (mode != TLK_WORK_MODE_TEST_EMI) {
        return;
    }
    tlk_test_mode_hook(TLK_WORK_MODE_TEST_EMI);
}

/**
 * @brief       This function checks BQB test mode.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_test_mode_bqb_check(void)
{
    uint8_t mode = tlkmdi_tinySql_getWorkMode();
    if (mode != TLK_WORK_MODE_TEST_BQB) {
        return;
    }
    tlk_test_mode_hook(TLK_WORK_MODE_TEST_BQB);
}

/**
 * @brief       This function is a hook for test mode.
 * @param[in]   mode    - the work mode.
 * @return      none.
 */
__attribute__((weak)) void tlk_test_mode_hook(uint8_t mode)
{
    (void)mode;
}
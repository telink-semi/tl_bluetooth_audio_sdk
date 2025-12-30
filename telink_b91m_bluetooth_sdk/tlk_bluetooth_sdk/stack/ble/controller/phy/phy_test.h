/********************************************************************************************************
 * @file    phy_test.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef PHY_TEST_H_
#define PHY_TEST_H_

#include "tl_common.h"

#define BLC_PHYTEST_DISABLE 0
#define BLC_PHYTEST_ENABLE  1

/**
 * @brief      for user to initialize PHY test module
 * @param      none
 * @return     none
 */
void blc_phy_initPhyTest_module(void);


/**
 * @brief      for user to set PHY test enable or disable
 * @param[in]  en - 1: enable; 0:disable
 * @return     status: 0x00 command OK, no other rvalue
 */
ble_sts_t blc_phy_setPhyTestEnable(u8 en);


/**
 * @brief      for user to get PHY test status: enable or disable
 * @param      none
 * @return     1: PHY test is enable; 0: PHY test is disable
 */
bool blc_phy_isPhyTestEnable(void);

/**
 * @brief      This function is used to manipulate "phytest"
 * @param[in]  *p - the phy test Command packet
 * @param[in]  n  - Reserved for future use
 * @return     0
 */
int blc_phytest_cmd_handler(u8 *p, int n);

#endif /* PHY_TEST_H_ */

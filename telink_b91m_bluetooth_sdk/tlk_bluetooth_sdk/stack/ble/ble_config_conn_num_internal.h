/********************************************************************************************************
 * @file    ble_config_conn_num_internal.h
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
/******************************* conn_config start ******************************************************************/

#define CONN_MAX_NUM_C0_P1 1
#define CONN_MAX_NUM_C0_P2 2
#define CONN_MAX_NUM_C0_P4 3
#define CONN_MAX_NUM_C1_P0 4
#define CONN_MAX_NUM_C1_P1 5
#define CONN_MAX_NUM_C1_P2 6
#define CONN_MAX_NUM_C1_P4 7
#define CONN_MAX_NUM_C2_P0 8
#define CONN_MAX_NUM_C2_P2 9
#define CONN_MAX_NUM_C2_P4 10
#define CONN_MAX_NUM_C4_P0 11
#define CONN_MAX_NUM_C4_P2 12
#define CONN_MAX_NUM_C4_P4 13


#ifndef CONN_MAX_NUM_CONFIG
#define CONN_MAX_NUM_CONFIG CONN_MAX_NUM_C4_P4
#endif

#if (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C0_P1)
#define LL_MAX_ACL_CEN_NUM 0
#define LL_MAX_ACL_PER_NUM 1

#define LL_ACL_CEN_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C0_P2)
#define LL_MAX_ACL_CEN_NUM 0
#define LL_MAX_ACL_PER_NUM 2

#define LL_ACL_CEN_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C0_P4)
#define LL_MAX_ACL_CEN_NUM 0
#define LL_MAX_ACL_PER_NUM 4

#define LL_ACL_CEN_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P0)
#define LL_MAX_ACL_CEN_NUM 1
#define LL_MAX_ACL_PER_NUM 0

#define LL_ACL_PER_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P1)
#define LL_MAX_ACL_CEN_NUM 1
#define LL_MAX_ACL_PER_NUM 1
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P2)
#define LL_MAX_ACL_CEN_NUM 1
#define LL_MAX_ACL_PER_NUM 2
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C1_P4)
#define LL_MAX_ACL_CEN_NUM 1
#define LL_MAX_ACL_PER_NUM 4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P0)
#define LL_MAX_ACL_CEN_NUM 2
#define LL_MAX_ACL_PER_NUM 0

#define LL_ACL_PER_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P2)
#define LL_MAX_ACL_CEN_NUM 2
#define LL_MAX_ACL_PER_NUM 2
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C2_P4)
#define LL_MAX_ACL_CEN_NUM 2
#define LL_MAX_ACL_PER_NUM 4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P0)
#define LL_MAX_ACL_CEN_NUM 4
#define LL_MAX_ACL_PER_NUM 0

#define LL_ACL_PER_EN      0
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P2)
#define LL_MAX_ACL_CEN_NUM 2
#define LL_MAX_ACL_PER_NUM 4
#elif (CONN_MAX_NUM_CONFIG == CONN_MAX_NUM_C4_P4)
#define LL_MAX_ACL_CEN_NUM 4
#define LL_MAX_ACL_PER_NUM 4
#else
#error "unsupported CONN_MAX_NUM_CONFIG"
#endif


#define LL_MAX_ACL_CONN_NUM (LL_MAX_ACL_CEN_NUM + LL_MAX_ACL_PER_NUM)


#define ACL_CONN_IDX_CEN0   0
#define ACL_CONN_IDX_PER0   LL_MAX_ACL_CEN_NUM


#ifndef LL_ACL_CEN_EN
#define LL_ACL_CEN_EN 1
#endif

#ifndef LL_ACL_PER_EN
#define LL_ACL_PER_EN 1
#endif

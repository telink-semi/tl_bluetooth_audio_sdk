/********************************************************************************************************
 * @file    em_map_config.h
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


#ifndef _EM_MAP_CONFIG_H_
#define _EM_MAP_CONFIG_H_


#ifndef MAX_NB_SYNC
#define MAX_NB_SYNC (2)
#endif

/// Number of RX descriptors
#ifndef BT_RXDESC_NB
#define BT_RXDESC_NB (3)
#endif

/// Number of RX/TX ACL data buffers
#ifndef ACL_DATA_BUF_NB_RX
#define ACL_DATA_BUF_NB_RX (BT_RXDESC_NB)
#endif

#ifndef ACL_DATA_BUF_NB_TX
#define ACL_DATA_BUF_NB_TX (MAX_NB_ACTIVE_ACL + 2)
#endif

/// Number of TX LMP buffers
#ifndef BT_LMP_BUF_NB_TX
#if (MAX_NB_ACTIVE_ACL > 3)
#define BT_LMP_BUF_NB_TX (2 * MAX_NB_ACTIVE_ACL)
#else // (MAX_NB_ACTIVE_ACL > 3)
#define BT_LMP_BUF_NB_TX (10)
#endif // (MAX_NB_ACTIVE_ACL > 3)
#endif

/// Maximum ACL Data Packet Size
#ifndef ACL_DATA_BUF_SIZE
#define ACL_DATA_BUF_SIZE ((DH5_3_PACKET_SIZE + 1) & (~0x01))
#endif

/******************************************************************************************/
/* --------------------------     CSB SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Support of Connectionless Slave Broadcast
#ifndef CSB_SUPPORT
#define CSB_SUPPORT 0
#endif

#ifndef PCA_SUPPORT
#define PCA_SUPPORT 0
#endif

#ifndef RW_BT_MWS_COEX
#define RW_BT_MWS_COEX 0
#endif

#endif

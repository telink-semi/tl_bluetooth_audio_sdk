/********************************************************************************************************
 * @file    ble_host_acl.h
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

// BLE Host ACL parameters defined.

// All BLE ACL Data Length and PDU times.
#define BLE_HOST_ACL_TX_OCTETS_MIN 0x001B // 27 Octets
#define BLE_HOST_ACL_TX_OCTETS_MAX 0x00FB // 251 Octets
#define BLE_HOST_ACL_TX_TIME_MIN   0x0148 // 328 us
#define BLE_HOST_ACL_TX_TIME_MAX   0x4290 // 17040 us

#define BLE_HOST_ACL_RX_OCTETS_MIN 0x001B // 27 Octets
#define BLE_HOST_ACL_RX_OCTETS_MAX 0x00FB // 251 Octets
#define BLE_HOST_ACL_RX_TIME_MIN   0x0148 // 328 us
#define BLE_HOST_ACL_RX_TIME_MAX   0x4290 // 17040 us

// BLE ACL PDU time calculation by payload size.
// 8us is BLE_1M PHY transmit 1 octet time, 112us is BLE_1M PHY transmit base time.
// preamble(1 octet) + AA(4 octets) + header(2 octets) + MIC(4 octets) + CRC(3 octets)
#define BLE_HOST_CALC_ACL_PDU_TIME_BLE_1M(octets) (8 * (octets) + 112)
// 4us is BLE_2M PHY transmit 1 octet time, 60us is BLE_2M PHY transmit base time.
// preamble(2 octet) + AA(4 octets) + header(2 octets) + MIC(4 octets) + CRC(3 octets)
#define BLE_HOST_CALC_ACL_PDU_TIME_BLE_2M(octets) (4 * (octets) + 60)
// 2us is BLE_CODED S2 PHY transmit 1 bit time, 376us is BLE_CODED PHY transmit base time.
// Preamble(80us) + AA(256us) + CI(16us) + TERM1(16us)
// Header(16bit) + PDU(octets) + MIC(32bit) + CRC(24bit) + TERM2(3bit)
#define BLE_HOST_CALC_ACL_PDU_TIME_BLE_CODED_S2(octets) (2 * (16 + octets * 8 + 32 + 24 + 3) + 376)
// 8us is BLE_CODED S8 PHY transmit 1 octet time, 376us is BLE_CODED PHY transmit base time.
// Preamble(80us) + AA(256us) + CI(16us) + TERM1(24us)
// Header(16bit) + PDU(octets) + MIC(32bit) + CRC(24bit) + TERM2(3bit)
#define BLE_HOST_CALC_ACL_PDU_TIME_BLE_CODED_S8(octets) (8 * (16 + octets * 8 + 32 + 24 + 3) + 376)

// All BLE TX PHY & RX PHY are LE 1M, 2M, or Coded.
#define BLE_HOST_ACL_PHY_LE_1M            0x01
#define BLE_HOST_ACL_PHY_LE_2M            0x02
#define BLE_HOST_ACL_PHY_LE_CODED         0x03

#define BLE_HOST_ACL_TX_PHY_DEFAULT       BLE_HOST_ACL_PHY_LE_1M
#define BLE_HOST_ACL_RX_PHY_DEFAULT       BLE_HOST_ACL_PHY_LE_1M

#define BLE_HOST_ACL_PHY_CHECK_VALID(phy) ((phy == BLE_HOST_ACL_PHY_LE_1M) || (phy == BLE_HOST_ACL_PHY_LE_2M) || (phy == BLE_HOST_ACL_PHY_LE_CODED))

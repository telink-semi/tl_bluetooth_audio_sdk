/********************************************************************************************************
 * @file    ascs.h
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
// ASCS: Audio Stream Control Service.

/* <<ASCS_v1.0.pdf>> Page 24, Table 4.7: Format of ASE Control Point characteristic value when notified */

struct lea_ascs_ase_control_point_notify_payload
{
    uint8_t ase_id;
    uint8_t response_code;
    uint8_t reason;
};

struct lea_ascs_ase_control_point_notify
{
    uint8_t                                          opcode;
    uint8_t                                          ase_num;
    struct lea_ascs_ase_control_point_notify_payload payload[0];
};

#define LEA_ASCS_ASE_CONTROL_NOTIFY_SIZE(ase_num) (sizeof(struct lea_ascs_ase_control_point_notify) + (ase_num) * sizeof(struct lea_ascs_ase_control_point_notify_payload))

/** ASE Type enumeration */
enum lea_ascs_ase_type
{
    ASCS_ASE_TYPE_SINK   = 1U << 0, /** < Sink ASE */
    ASCS_ASE_TYPE_SOURCE = 1U << 1, /** < Source ASE */
    ASCS_ASE_TYPE_ANY    = ASCS_ASE_TYPE_SINK | ASCS_ASE_TYPE_SOURCE,
};

enum lea_ascs_target_latency
{
    ASCS_TARGET_LOW_LATENCY                      = 0x01,
    ASCS_TARGET_BALANCED_LATENCY_AND_RELIABILITY = 0x02,
    ASCS_TARGET_HIGH_RELIABILITY                 = 0x03,
};

#define ASCS_CHECK_TARGET_LATENCY(latency) \
    ((latency) == ASCS_TARGET_HIGH_RELIABILITY || (latency) == ASCS_TARGET_BALANCED_LATENCY_AND_RELIABILITY || (latency) == ASCS_TARGET_LOW_LATENCY)

enum lea_ascs_target_phy
{
    ASCS_TARGET_LE_1M_PHY    = 0x01,
    ASCS_TARGET_LE_2M_PHY    = 0x02,
    ASCS_TARGET_LE_CODED_PHY = 0x03,
};

#define ASCS_CHECK_TARGET_PHY(phy) ((phy) == ASCS_TARGET_LE_1M_PHY || (phy) == ASCS_TARGET_LE_2M_PHY || (phy) == ASCS_TARGET_LE_CODED_PHY)

enum lea_ascs_framing
{
    LEA_ASCS_UNFRAMED_SUPPORTED = 0,
    LEA_ASCS_UNFRAMED_NOT_SUPPORTED,
};

#define LEA_ASCS_CHECK_FRAMING(framing) ((framing) == LEA_ASCS_UNFRAMED_SUPPORTED || (framing) == LEA_ASCS_UNFRAMED_NOT_SUPPORTED)

enum lea_ascs_preperred_phy
{
    LEA_ASCS_PREPERRED_PHY_1M    = 1U << 0,
    LEA_ASCS_PREPERRED_PHY_2M    = 1U << 1,
    LEA_ASCS_PREPERRED_PHY_CODED = 1U << 2,
};

#define LEA_ASCS_CHECK_PREPERRED_PHY(phy) ((phy) & (LEA_ASCS_PREPERRED_PHY_1M | LEA_ASCS_PREPERRED_PHY_2M | LEA_ASCS_PREPERRED_PHY_CODED))

enum lea_ascs_max_transport_latency
{
    LEA_ASCS_LATENCY_MIN_VALUE = 0x0005,
    LEA_ASCS_LATENCY_MAX_VALUE = 0x0FA0,
};

#define LEA_ASCS_CHECK_MAX_TRANSPORT_LATENCY(latency) ((latency) >= LEA_ASCS_LATENCY_MIN_VALUE && (latency) <= LEA_ASCS_LATENCY_MAX_VALUE)

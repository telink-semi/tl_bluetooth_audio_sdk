/********************************************************************************************************
 * @file    bt_classic_dbg_int.h
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
#ifndef BT_CLASSIC_DBG_INT_H
#define BT_CLASSIC_DBG_INT_H

#ifndef DUMP_HCI_MSG
    #define DUMP_HCI_MSG 1
#endif

#ifndef DUMP_ACL_MSG
    #define DUMP_ACL_MSG 1
#endif

#if BT_ERROR_CODE
    /// LM task hci cmd handler log enable
    #ifndef BREDR_CTRL_LM_HCI_LOG_EN
        #define BREDR_CTRL_LM_HCI_LOG_EN 1
    #endif

    /// LM task hci cmd handler vcd enable
    #ifndef BREDR_CTRL_LM_HCI_VCD_EN
        #define BREDR_CTRL_LM_HCI_VCD_EN 0
    #endif

    /// LC task hci cmd handler log enable
    #ifndef BREDR_CTRL_LC_HCI_LOG_EN
        #define BREDR_CTRL_LC_HCI_LOG_EN 1
    #endif

    /// LC task hci cmd handler vcd enable
    #ifndef BREDR_CTRL_LC_HCI_VCD_EN
        #define BREDR_CTRL_LC_HCI_VCD_EN 0
    #endif

    /// LC task lmp msg handler log enable
    #ifndef BREDR_CTRL_LC_LMP_LOG_EN
        #define BREDR_CTRL_LC_LMP_LOG_EN 1
    #endif

    /// LC task lmp msg handler vcd enable
    #ifndef BREDR_CTRL_LC_LMP_VCD_EN
        #define BREDR_CTRL_LC_LMP_VCD_EN 0
    #endif

    /// LD log enable
    #ifndef BREDR_CTRL_LD_LOG_EN
        #define BREDR_CTRL_LD_LOG_EN 1
    #endif

    /// LD vcd enable
    #ifndef BREDR_CTRL_LD_VCD_EN
        #define BREDR_CTRL_LD_VCD_EN 1
    #endif
#else
    /// LM task hci cmd handler log enable
    #ifndef BREDR_CTRL_LM_HCI_LOG_EN
        #define BREDR_CTRL_LM_HCI_LOG_EN 0
    #endif

    /// LM task hci cmd handler vcd enable
    #ifndef BREDR_CTRL_LM_HCI_VCD_EN
        #define BREDR_CTRL_LM_HCI_VCD_EN 0
    #endif

    /// LC task hci cmd handler log enable
    #ifndef BREDR_CTRL_LC_HCI_LOG_EN
        #define BREDR_CTRL_LC_HCI_LOG_EN 0
    #endif

    /// LC task hci cmd handler vcd enable
    #ifndef BREDR_CTRL_LC_HCI_VCD_EN
        #define BREDR_CTRL_LC_HCI_VCD_EN 0
    #endif

    /// LC task lmp msg handler log enable
    #ifndef BREDR_CTRL_LC_LMP_LOG_EN
        #define BREDR_CTRL_LC_LMP_LOG_EN 0
    #endif

    /// LC task lmp msg handler vcd enable
    #ifndef BREDR_CTRL_LC_LMP_VCD_EN
        #define BREDR_CTRL_LC_LMP_VCD_EN 0
    #endif

    /// LD log enable
    #ifndef BREDR_CTRL_LD_LOG_EN
        #define BREDR_CTRL_LD_LOG_EN 0
    #endif

    /// LD vcd enable
    #ifndef BREDR_CTRL_LD_VCD_EN
        #define BREDR_CTRL_LD_VCD_EN 1
    #endif
#endif

#endif /* BT_CLASSIC_DBG_INT_H */

/********************************************************************************************************
 * @file    usb1_reg.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
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
#ifndef __USB1_REG_H_
#define __USB1_REG_H_
#include "soc.h"

/******************************* usb1 registers: 0x188000 ******************************/
#define REG_USB1_BASE         0x188000

#define reg_usb1_ctrl_ep_ptr  REG_ADDR8(REG_USB1_BASE + 0X00)
#define reg_usb1_ctrl_ep_dat  REG_ADDR8(REG_USB1_BASE + 0x01)

#define reg_usb1_ctrl_ep_ctrl REG_ADDR8(REG_USB1_BASE + 0x02)

enum
{
    FLD_USB1_EP_DAT_ACK   = BIT(0),
    FLD_USB1_EP_DAT_BUSY  = BIT(0), /* read only. */
    FLD_USB1_EP_DAT_STALL = BIT(1),
    FLD_USB1_EP_STA_ACK   = BIT(2),
    FLD_USB1_EP_STA_STALL = BIT(3),
    FLD_USB1_IRQ_SET_ADDR = BIT(6), /**< W1C. */
};

#define reg_usb1_ctrl_ep_irq_sta REG_ADDR8(REG_USB1_BASE + 0x03)

enum
{
    FLD_USB1_IRQ_RESET_STATUS   = BIT(0),
    FLD_USB1_IRQ_250US_STATUS   = BIT(1),
    FLD_USB1_IRQ_SUSPEND_STATUS = BIT(2),
    FLD_USB1_IRQ_SOF_STATUS     = BIT(3),
    FLD_USB1_CTRL_EP_IRQ_SETUP  = BIT(4),
    FLD_USB1_CTRL_EP_IRQ_DATA   = BIT(5),
    FLD_USB1_CTRL_EP_IRQ_STA    = BIT(6),
    FLD_USB1_CTRL_EP_IRQ_INTF   = BIT(7),
};

#define reg_usb1_ctrl_ep_irq_mode REG_ADDR8(REG_USB1_BASE + 0x04)

enum
{
    FLD_USB1_CTRL_EP_AUTO_ADDR = BIT(0),
    FLD_USB1_CTRL_EP_AUTO_CFG  = BIT(1),
    FLD_USB1_CTRL_EP_AUTO_INTF = BIT(2),
    FLD_USB1_CTRL_EP_AUTO_STA  = BIT(3),
    FLD_USB1_CTRL_EP_AUTO_SYN  = BIT(4),
    FLD_USB1_CTRL_EP_AUTO_DESC = BIT(5),
    FLD_USB1_CTRL_EP_AUTO_FEAT = BIT(6),
    FLD_USB1_CTRL_EP_AUTO_STD  = BIT(7),
};

#define reg_usb1_usb_ctrl REG_ADDR8(REG_USB1_BASE + 0x05)

enum
{
    FLD_USB1_HALT_CLR   = BIT(0),
    FLD_USB1_HALT_STALL = BIT(1),
    FLD_USB1_HALT_TGL   = BIT(2),
    FLD_USB1_WEAKUP_FEA = BIT(3),
};

#define reg_usb1_pid_l     REG_ADDR8(REG_USB1_BASE + 0x06)
#define reg_usb1_pid_h     REG_ADDR8(REG_USB1_BASE + 0x07)
#define reg_usb1_edp0_udc  REG_ADDR8(REG_USB1_BASE + 0x08)
#define reg_usb1_edp0_extn REG_ADDR8(REG_USB1_BASE + 0x09)

enum
{
    FLD_USB1_IRQ_RESET_EDGE   = BIT(4),
    FLD_USB1_IRQ_250US_EDGE   = BIT(5),
    FLD_USB1_SET_ADDRESS_MASK = BIT(6),
    FLD_USB1_IRQ_SOF_EDGE     = BIT(7),
};

#define reg_usb1_mdev REG_ADDR8(REG_USB1_BASE + 0x0a)

enum
{
    FLD_USB1_USB_MDEV_SELF_PWR     = BIT(0),
    FLD_USB1_USB_MDEV_SET_WAKE_FEA = BIT(1),
    FLD_USB1_USB_MDEV_WAKE_FEA     = BIT(2),
    FLD_USB1_USB_MDEV_VEND_CMD     = BIT(3),
    FLD_USB1_USB_MDEV_VEND_DIS     = BIT(4),
    FLD_USB1_MDEV_MODE_SEL         = BIT_RNG(5, 6),
    FLD_USB1_CONFIG_NOW            = BIT(7),
};

#define reg_usb1_host_conn     REG_ADDR8(REG_USB1_BASE + 0x0b)
#define reg_usb1_sups_cyc_cali REG_ADDR8(REG_USB1_BASE + 0x0c)
#define reg_usb1_intf_alt      REG_ADDR8(REG_USB1_BASE + 0x0d)

#define reg_usb1_edp_en        REG_ADDR8(REG_USB1_BASE + 0x0e)

typedef enum
{
    FLD_USB1_EDP8_EN = BIT(0),
    FLD_USB1_EDP1_EN = BIT(1),
    FLD_USB1_EDP2_EN = BIT(2),
    FLD_USB1_EDP3_EN = BIT(3),
    FLD_USB1_EDP4_EN = BIT(4),
    FLD_USB1_EDP5_EN = BIT(5),
    FLD_USB1_EDP6_EN = BIT(6),
    FLD_USB1_EDP7_EN = BIT(7),
} usb1_ep_en_e;

#define reg_usb1_irq_mask REG_ADDR8(REG_USB1_BASE + 0x0f)

enum
{
    FLD_USB1_IRQ_RESET_MASK   = BIT(0),
    FLD_USB1_IRQ_250US_MASK   = BIT(1),
    FLD_USB1_IRQ_SUSPEND_MASK = BIT(2),
    FLD_USB1_IRQ_SOF_MASK     = BIT(3),
    FLD_USB1_IRQ_SETUP_MASK   = BIT(4),
    FLD_USB1_IRQ_DATA_MASK    = BIT(5),
    FLD_USB1_IRQ_STATUS_MASK  = BIT(6),
    FLD_USB1_IRQ_SET_INF_MASK = BIT(7),
};

/* Registers after 0x0f can be read or written byte, half-word or word. */
#define reg_usb1_ep8_ptr          REG_ADDR8(REG_USB1_BASE + 0x10)
#define reg_usb1_ep1_ptr          REG_ADDR8(REG_USB1_BASE + 0x11)
#define reg_usb1_ep2_ptr          REG_ADDR8(REG_USB1_BASE + 0x12)
#define reg_usb1_ep3_ptr          REG_ADDR8(REG_USB1_BASE + 0x13)
#define reg_usb1_ep4_ptr          REG_ADDR8(REG_USB1_BASE + 0x14)
#define reg_usb1_ep5_ptr          REG_ADDR8(REG_USB1_BASE + 0x15)
#define reg_usb1_ep6_ptr          REG_ADDR8(REG_USB1_BASE + 0x16)
#define reg_usb1_ep7_ptr          REG_ADDR8(REG_USB1_BASE + 0x17)
#define reg_usb1_ep_ptr(i)        REG_ADDR8(REG_USB1_BASE + 0x10 + ((i) & 0x07))

#define reg_usb1_ep8_hptr         REG_ADDR8(REG_USB1_BASE + 0x18)                /* bit[2:0]. */
#define reg_usb1_ep1_hptr         REG_ADDR8(REG_USB1_BASE + 0x19)                /* bit[2:0]. */
#define reg_usb1_ep2_hptr         REG_ADDR8(REG_USB1_BASE + 0x1a)                /* bit[2:0]. */
#define reg_usb1_ep3_hptr         REG_ADDR8(REG_USB1_BASE + 0x1b)                /* bit[2:0]. */
#define reg_usb1_ep4_hptr         REG_ADDR8(REG_USB1_BASE + 0x1c)                /* bit[2:0]. */
#define reg_usb1_ep5_hptr         REG_ADDR8(REG_USB1_BASE + 0x1d)                /* bit[2:0]. */
#define reg_usb1_ep6_hptr         REG_ADDR8(REG_USB1_BASE + 0x1e)                /* bit[2:0]. */
#define reg_usb1_ep7_hptr         REG_ADDR8(REG_USB1_BASE + 0x1f)                /* bit[2:0]. */
#define reg_usb1_ep_ptrh(i)       REG_ADDR8(REG_USB1_BASE + 0x18 + ((i) & 0x07)) /* bit[2:0]. */

#define reg_usb1_ep8_buf_addr     REG_ADDR8(REG_USB1_BASE + 0x20)
#define reg_usb1_ep1_buf_addr     REG_ADDR8(REG_USB1_BASE + 0x21)
#define reg_usb1_ep2_buf_addr     REG_ADDR8(REG_USB1_BASE + 0x22)
#define reg_usb1_ep3_buf_addr     REG_ADDR8(REG_USB1_BASE + 0x23)
#define reg_usb1_ep4_buf_addr     REG_ADDR8(REG_USB1_BASE + 0x24)
#define reg_usb1_ep5_buf_addr     REG_ADDR8(REG_USB1_BASE + 0x25)
#define reg_usb1_ep6_buf_addr     REG_ADDR8(REG_USB1_BASE + 0x26)
#define reg_usb1_ep7_buf_addr     REG_ADDR8(REG_USB1_BASE + 0x27)
#define reg_usb1_ep_buf_addr(i)   REG_ADDR8(REG_USB1_BASE + 0x20 + ((i) & 0x07))

#define reg_usb1_ep8_buf_h_addr   REG_ADDR8(REG_USB1_BASE + 0x28)                /* bit[2:0]. */
#define reg_usb1_ep1_buf_h_addr   REG_ADDR8(REG_USB1_BASE + 0x29)                /* bit[2:0]. */
#define reg_usb1_ep2_buf_h_addr   REG_ADDR8(REG_USB1_BASE + 0x2a)                /* bit[2:0]. */
#define reg_usb1_ep3_buf_h_addr   REG_ADDR8(REG_USB1_BASE + 0x2b)                /* bit[2:0]. */
#define reg_usb1_ep4_buf_h_addr   REG_ADDR8(REG_USB1_BASE + 0x2c)                /* bit[2:0]. */
#define reg_usb1_ep5_buf_h_addr   REG_ADDR8(REG_USB1_BASE + 0x2d)                /* bit[2:0]. */
#define reg_usb1_ep6_buf_h_addr   REG_ADDR8(REG_USB1_BASE + 0x2e)                /* bit[2:0]. */
#define reg_usb1_ep7_buf_h_addr   REG_ADDR8(REG_USB1_BASE + 0x2f)                /* bit[2:0]. */
#define reg_usb1_ep_buf_h_addr(i) REG_ADDR8(REG_USB1_BASE + 0x28 + ((i) & 0x07)) /* bit[2:0]. */

#define reg_usb1_ep8_ctrl         REG_ADDR8(REG_USB1_BASE + 0x30)
#define reg_usb1_ep1_ctrl         REG_ADDR8(REG_USB1_BASE + 0x31)
#define reg_usb1_ep2_ctrl         REG_ADDR8(REG_USB1_BASE + 0x32)
#define reg_usb1_ep3_ctrl         REG_ADDR8(REG_USB1_BASE + 0x33)
#define reg_usb1_ep4_ctrl         REG_ADDR8(REG_USB1_BASE + 0x34)
#define reg_usb1_ep5_ctrl         REG_ADDR8(REG_USB1_BASE + 0x35)
#define reg_usb1_ep6_ctrl         REG_ADDR8(REG_USB1_BASE + 0x36)
#define reg_usb1_ep7_ctrl         REG_ADDR8(REG_USB1_BASE + 0x37)
#define reg_usb1_ep_ctrl(i)       REG_ADDR8(REG_USB1_BASE + 0x30 + ((i) & 0x07))

enum
{
    FLD_USB1_EP_ACK   = BIT(0),
    FLD_USB1_EP_BUSY  = BIT(0), /* read only */
    FLD_USB1_EP_STALL = BIT(1),
    FLD_USB1_EP_DAT0  = BIT(2),
    FLD_USB1_EP_DAT1  = BIT(3),
};

#define reg_usb1_iso_mode      REG_ADDR8(REG_USB1_BASE + 0x38)
#define reg_usb1_ep_irq_status REG_ADDR8(REG_USB1_BASE + 0x39)
#define reg_usb1_ep_irq_mask   REG_ADDR8(REG_USB1_BASE + 0x3a)

typedef enum
{
    FLD_USB1_EDP8_IRQ = BIT(0),
    FLD_USB1_EDP1_IRQ = BIT(1),
    FLD_USB1_EDP2_IRQ = BIT(2),
    FLD_USB1_EDP3_IRQ = BIT(3),
    FLD_USB1_EDP4_IRQ = BIT(4),
    FLD_USB1_EDP5_IRQ = BIT(5),
    FLD_USB1_EDP6_IRQ = BIT(6),
    FLD_USB1_EDP7_IRQ = BIT(7),
} usb1_ep_irq_e;

#define reg_usb1_ep8_max0         REG_ADDR8(REG_USB1_BASE + 0x3b)

#define reg_usb1_ep8_send_thres   REG_ADDR8(REG_USB1_BASE + 0x3c)
#define reg_usb1_ep8_send_thres_h REG_ADDR8(REG_USB1_BASE + 0x61)

enum
{
    FLD_USB1_EDP8_MIN1 = BIT_RNG(0, 2),
};

#define reg_usb1_ep8_fifo_mode REG_ADDR8(REG_USB1_BASE + 0x3d)

enum
{
    FLD_USB1_EDP8_FIFO_MODE = BIT(0),
    FLD_USB1_EDP8_FULL_FLAG = BIT(1),
    FLD_USB1_MODE0          = BIT(2),
    FLD_USB1_EDP8_EOF       = BIT(3),
    FLD_USB1_EDP8_DMA_EOF   = BIT_RNG(4, 6),
};

#define reg_usb1_ep_max_size        REG_ADDR8(REG_USB1_BASE + 0x3e)
#define reg_usb1_ep_tick            REG_ADDR8(REG_USB1_BASE + 0x3f)

#define reg_usb1_ep8_dat            REG_ADDR32(REG_USB1_BASE + 0x40)
#define reg_usb1_ep8_dat0           REG_ADDR8(REG_USB1_BASE + 0x40)
#define reg_usb1_ep8_dat1           REG_ADDR8(REG_USB1_BASE + 0x41)
#define reg_usb1_ep8_dat2           REG_ADDR8(REG_USB1_BASE + 0x42)
#define reg_usb1_ep8_dat3           REG_ADDR8(REG_USB1_BASE + 0x43)

#define reg_usb1_ep1_dat            REG_ADDR32(REG_USB1_BASE + 0x44)
#define reg_usb1_ep1_dat0           REG_ADDR8(REG_USB1_BASE + 0x44)
#define reg_usb1_ep1_dat1           REG_ADDR8(REG_USB1_BASE + 0x45)
#define reg_usb1_ep1_dat2           REG_ADDR8(REG_USB1_BASE + 0x46)
#define reg_usb1_ep1_dat3           REG_ADDR8(REG_USB1_BASE + 0x47)

#define reg_usb1_ep2_dat            REG_ADDR32(REG_USB1_BASE + 0x48)
#define reg_usb1_ep2_dat0           REG_ADDR8(REG_USB1_BASE + 0x48)
#define reg_usb1_ep2_dat1           REG_ADDR8(REG_USB1_BASE + 0x49)
#define reg_usb1_ep2_dat2           REG_ADDR8(REG_USB1_BASE + 0x4a)
#define reg_usb1_ep2_dat3           REG_ADDR8(REG_USB1_BASE + 0x4b)

#define reg_usb1_ep3_dat            REG_ADDR32(REG_USB1_BASE + 0x4c)
#define reg_usb1_ep3_dat0           REG_ADDR8(REG_USB1_BASE + 0x4c)
#define reg_usb1_ep3_dat1           REG_ADDR8(REG_USB1_BASE + 0x4d)
#define reg_usb1_ep3_dat2           REG_ADDR8(REG_USB1_BASE + 0x4e)
#define reg_usb1_ep3_dat3           REG_ADDR8(REG_USB1_BASE + 0x4f)

#define reg_usb1_ep4_dat            REG_ADDR32(REG_USB1_BASE + 0x50)
#define reg_usb1_ep4_dat0           REG_ADDR8(REG_USB1_BASE + 0x50)
#define reg_usb1_ep4_dat1           REG_ADDR8(REG_USB1_BASE + 0x51)
#define reg_usb1_ep4_dat2           REG_ADDR8(REG_USB1_BASE + 0x52)
#define reg_usb1_ep4_dat3           REG_ADDR8(REG_USB1_BASE + 0x53)

#define reg_usb1_ep5_dat            REG_ADDR32(REG_USB1_BASE + 0x54)
#define reg_usb1_ep5_dat0           REG_ADDR8(REG_USB1_BASE + 0x54)
#define reg_usb1_ep5_dat1           REG_ADDR8(REG_USB1_BASE + 0x55)
#define reg_usb1_ep5_dat2           REG_ADDR8(REG_USB1_BASE + 0x56)
#define reg_usb1_ep5_dat3           REG_ADDR8(REG_USB1_BASE + 0x57)

#define reg_usb1_ep6_dat            REG_ADDR32(REG_USB1_BASE + 0x58)
#define reg_usb1_ep6_dat0           REG_ADDR8(REG_USB1_BASE + 0x58)
#define reg_usb1_ep6_dat1           REG_ADDR8(REG_USB1_BASE + 0x59)
#define reg_usb1_ep6_dat2           REG_ADDR8(REG_USB1_BASE + 0x5a)
#define reg_usb1_ep6_dat3           REG_ADDR8(REG_USB1_BASE + 0x5b)

#define reg_usb1_ep7_dat            REG_ADDR32(REG_USB1_BASE + 0x5c)
#define reg_usb1_ep7_dat0           REG_ADDR8(REG_USB1_BASE + 0x5c)
#define reg_usb1_ep7_dat1           REG_ADDR8(REG_USB1_BASE + 0x5d)
#define reg_usb1_ep7_dat2           REG_ADDR8(REG_USB1_BASE + 0x5e)
#define reg_usb1_ep7_dat3           REG_ADDR8(REG_USB1_BASE + 0x5f)

#define reg_usb1_ep_dat_word(ep)    REG_ADDR32(REG_USB1_BASE + 0x40 + (((ep) & 0x07) << 2))
#define reg_usb1_ep_dat0(ep)        REG_ADDR8(REG_USB1_BASE + 0x40 + (((ep) & 0x07) << 2))
#define reg_usb1_ep_dat1(ep)        REG_ADDR8(REG_USB1_BASE + 0x41 + (((ep) & 0x07) << 2))
#define reg_usb1_ep_dat2(ep)        REG_ADDR8(REG_USB1_BASE + 0x42 + (((ep) & 0x07) << 2))
#define reg_usb1_ep_dat3(ep)        REG_ADDR8(REG_USB1_BASE + 0x43 + (((ep) & 0x07) << 2))
#define reg_usb1_ep_dat_byte(ep, i) REG_ADDR8(REG_USB1_BASE + 0x40 + (((ep) & 0x07) << 2) + (i))

#define reg_usb1_ram_ctrl           REG_ADDR8(REG_USB1_BASE + 0x60)

enum
{
    FLD_USB1_CEN_PWR_DN  = BIT(0),
    FLD_USB1_CLK_PWR_DN  = BIT(1),
    FLD_USB1_CEN_FUNC    = BIT(3),
    FLD_USB1_WEN0_PWR_DN = BIT(4),
    FLD_USB1_WEN1_PWR_DN = BIT(5),
    FLD_USB1_WEN2_PWR_DN = BIT(6),
    FLD_USB1_WEN3_PWR_DN = BIT(7),
};

#define reg_usb1_sof_frame0 REG_ADDR8(REG_USB1_BASE + 0x62)
#define reg_usb1_sof_frame1 REG_ADDR8(REG_USB1_BASE + 0x63) /* bit[2:0] */

#define reg_usb1_edps_eptr  REG_ADDR16(REG_USB1_BASE + 0x64)
#define reg_usb1_edps_udc   REG_ADDR16(REG_USB1_BASE + 0x66)
#define reg_usb1_edps_s_ptr REG_ADDR16(REG_USB1_BASE + 0x68)

#define reg_usb1_edps_maxh0 REG_ADDR8(REG_USB1_BASE + 0x6a)

enum
{
    FLD_USB1_EDPS_MAXH0     = BIT(0),
    FLD_USB1_EDPS_FULL      = BIT(5),
    FLD_USB1_EDPS_NZ        = BIT(6),
    FLD_USB1_EDPS_FULL_THRD = BIT(7),
};

#define reg_usb1_edps_full_thrd REG_ADDR8(REG_USB1_BASE + 0x6b)

#define reg_usb1_tstamp         REG_ADDR32(REG_USB1_BASE + 0x6c)

#define reg_usb1_calcyc         REG_ADDR16(REG_USB1_BASE + 0x70)

#define reg_usb1_ct             REG_ADDR8(REG_USB1_BASE + 0x72)

enum
{
    FLD_USB1_CLK_SEL0  = BIT(0),
    FLD_USB1_LOW_SPEED = BIT(1),
    FLD_USB1_CLK_SEL2  = BIT(2),
    FLD_USB1_TEST_MODE = BIT(3),
    FLD_USB1_CLK_SEL_O = BIT_RNG(4, 7),
};

#define reg_usb1_ctrl REG_ADDR8(REG_USB1_BASE + 0X73)

enum
{
    FLD_USB1_RXDAT_ASYNC_EN = BIT(0),
    FLD_USB1_TIMEOUT_NAK_EN = BIT(1),
};

#endif

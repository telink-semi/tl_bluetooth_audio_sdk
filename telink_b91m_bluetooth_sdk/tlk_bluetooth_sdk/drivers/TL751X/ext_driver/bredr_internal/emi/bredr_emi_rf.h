/********************************************************************************************************
 * @file    bredr_emi_rf.h
 *
 * @brief   This is the header file for TLSR/TL
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
#ifndef VENDOR_BT_AUTOTEST_DEMO_BT_RF_BT_H_
#define VENDOR_BT_AUTOTEST_DEMO_BT_RF_BT_H_
#include "drivers.h"

#define DEBUG_JUNWEI  1 // Add by Junwei,20210115
#define SYNC_20210329 1 // Add by pengcheng,20210329

////////Add by pengcheng, sync with junwei 20210329/////////

#define BT_RADIO_RX_EIR_ADJ_EN 1 // for inquiry eir
#define BT_RADIO_SYNC_OFFSET   0
//#define   BT_RADIO_RX_PATH_DELAY      10
//    #define BT_RADIO_RX_POWER_UP      (100 - BT_RADIO_RX_PATH_DELAY) //CEVA bug
//    #define BT_RADIO_TX_PATH_DELAY(m) ((m == 0) ? 10 : 10)           //BR:0,EDR2:2,EDR3:3
//    #define BT_RADIO_TX_POWER_UP      57
//    #define BT_RADIO_POWER_DOWN       27
//////////////////////////////////////////////////////////
#define APBRG_BASE  0x140000
#define PSLV7_BASE  (APBRG_BASE + 0x1c0)
#define PSLV12_BASE (APBRG_BASE + 0x300)
#define HSLV13_BASE 0xd4160000 //0x80160000
//#define PDZBADDR                          ZB_AHBADDR + 0x300
#define SC_BASE               PSLV7_BASE
#define GPIO_BASE             PSLV12_BASE
#define ZB_BASE               HSLV13_BASE
#define IO_BASE_ADDR          0xc0000000
#define BUS_ADDR              0xc0200000

#define DM_ADDR               ZB_BASE
#define Expect_ADDR           (0xc0200000 + 0x1c00)
#define OFFSET_ADDR           (0)
#define EM_ADDR               (0xD0080000 + OFFSET_ADDR) //0x50080000 + 0x80000000
#define BT_ADDR               (DM_ADDR)
#define EMI_RADIOPWRUPDN_ADDR (BT_ADDR + 0x8C)
#define EMI_RADIOTXRXTIM_ADDR (BT_ADDR + 0x90)
#define CS_ADDR               (EM_ADDR + 0x200)
#define TD0_ADDR              (EM_ADDR + 0x2a0)
#define RD0_ADDR              (EM_ADDR + 0x2c0)
#define RXBUF0_ADDR           (EM_ADDR + 0x500)
#define RXBUF1_ADDR           (EM_ADDR + 0x900)
#define ET_ADDR               (EM_ADDR + 0x000)
#define E0_ADDR               (EM_ADDR + 0x160)
#define ErrNum_ADDR           (EM_ADDR + 0x450)
//#define TEST_CASE_ADDR              (EM_ADDR + 0x460)
//#define DEBUG_VALUE(i)                 ((*(volatile unsigned long*)(EM_ADDR + 0x464 + 4 * i)))

#define ET0                     0x00

#define EMI_CLKNINTSTAT_BIT     ((u32)0x00000001)
#define EMI_ENDFRMINTSTAT_BIT   ((u32)0x00000020)
#define EMI_STARTFRMINTSTAT_BIT ((u32)0x00000010)
#define EMI_RXINTSTAT_BIT       ((u32)0x00000004)
#define EMI_ERRORINTSTAT_BIT    ((u32)0x00000100)
#define EMI_RXDONEINTSTAT_BIT   ((u32)0x00400000)
#define EMI_TXDONEINTSTAT_BIT   ((u32)0x00800000)
#define EMI_SKIPFRMINTSTAT_BIT  ((u32)0x00000040)

#define write_mem8(addr, v)     (*(volatile unsigned char *)((addr)) = (unsigned char)(v))
#define write_mem16(addr, v)    (*(volatile unsigned short *)((addr)) = (unsigned short)(v))
#define read_mem8(addr)         (*(volatile unsigned char *)((addr)))
#define read_mem16(addr)        (*(volatile unsigned short *)((addr)))

#define RXEIRSTAT               ((u16)0x0100)
#define RXGUARDERR              ((u16)0x0080)
#define RXBADLT                 ((u16)0x0040)
#define RXFECERR                ((u16)0x0020)
#define RXSEQERR                ((u16)0x0010)
#define RXMICERR                ((u16)0x0008)
#define RXCRCERR                ((u16)0x0004) // BIT(2)
#define RXHECERR                ((u16)0x0002) // BIT(1)
#define RXSYNCERR               ((u16)0x0001) // BIT(0)

#define PARA_BUF_ADDR           0xc0000004
#define PARA_BUF_SIZE           0xc0000006
#define PARA_BUF_CNT            0xc0000007
#define PARA_BUF_WPTR           0xc0000008
#define PARA_BUF_RPTR           0xc0000009

#define RESU_BUF_ADDR           0xc000000a
#define RESU_BUF_SIZE           0xc000000c
#define RESU_BUF_CNT            0xc000000d
#define RESU_BUF_WPTR           0xc000000e
#define RESU_BUF_RPTR           0xc000000f

typedef enum
{
    ZB_MST_CLOCK_8M  = 8,
    ZB_MST_CLOCK_16M = 16,
    ZB_MST_CLOCK_24M = 24,
    ZB_MST_CLOCK_32M = 32
} ZB_mst_clock_e;

typedef enum
{
    CASE_IDLY = 0,
    CASE_RF_PER,
    CASE_RF_LEAKAGE,
    CASE_RF_TXPLL_PHASE_NOISE,
    CASE_RF_TP_SCAN,
    CASE_RF_EVM,
    CASE_RF_FDEV,
    CASE_RF_RX_FRE_OFFSET,
    CASE_RF_RX_PGA_SCAN,
    CASE_RF_RX_GAIN_SCAN,
    CASE_RF_TX_KVCO_SCAN,
    CASE_RF_RX_ACS_SCAN,
    CASE_RF_RX_INTER_MOD_SCAN,
    CASE_RF_TX_FRE_SCAN,
    CASE_RF_TX_PWR_SCAN,
    CASE_RF_TX_PLL_SPUR_SCAN,
    CASE_RF_TX_CD_MODE,
    CASE_RF_SPURIOUS_EMISSION,
    CASE_RF_ZIGBEE_PSD,
    CASE_RF_TX_BURST,
    RF_SNIFF_MODE,
    RF_RXRSSI_MODE
} TestCaseTypeDef;

enum
{
    FRAME_IDLE,
    FRAME_START_RX,
    FRAME_START_TX,
    FRAME_GOING,
    FRAME_DONE,
};

typedef struct br_rx_para
{
    u32 rawstep_1st;
    u32 winsize;
    u32 times;
    u32 duration;
    u32 freq;
    u32 power_lvl;
} br_rx_para_t;

typedef struct br_rx_result
{
    u32 status;
    u32 finish_times;
    u32 success_times;
    u32 error_times;
    u32 missing_times;
    u32 Hec_times;
    u32 Total_times;
    u32 timer;
    u32 invtimer;
    u8  invtimer_update;
} br_rx_result_t;

typedef struct br_tx_para
{
    u32 rawstep;
    u32 times;
    u32 duration;
    u32 freq;
    u32 power;
} br_tx_para_t;

typedef struct br_tx_result
{
    u32 status;
    u32 finish_times;
    u32 Total_times;
} br_tx_result_t;

#define GLOBAL_INT_START()                                         \
    do {                                                           \
        __builtin_riscv_csrrs(1 << 11 | 1 << 7 | 1 << 3, NDS_MIE); \
        __builtin_riscv_csrrs(0x8, NDS_MSTATUS);                   \
        reg_irq_feature |= FLD_FEATURE_VECTOR_MODE_EN;             \
    } while (0)

#define wr_reg(addr, data, len)      \
    do {                             \
        if (len == 1) {              \
            write_reg8(addr, data);  \
        } else if (len == 2) {       \
            write_reg16(addr, data); \
        } else if (len == 4) {       \
            write_reg32(addr, data); \
        }                            \
    } while (0)

static inline void rf_set_em_base_address(u32 addr) // exchange memory address
{
    reg_pdzb_em_base_address = addr;
}

static inline void bt_intcntl_intmsk_setf(u32 mask)
{
    write_reg32(BT_ADDR + 0x0c, mask);
}

#if SYNC_20210329
static inline void emi_radiopwrupdn_pack(unsigned char rxpwrupct, unsigned char txpwrdnct, unsigned char txpwrupct)
{
    write_reg32(EMI_RADIOPWRUPDN_ADDR, ((unsigned int)rxpwrupct << 16) | ((unsigned int)txpwrdnct << 8) | ((unsigned int)txpwrupct << 0));
}

static inline void emi_radiotxrxtim_pack(unsigned char syncposition, unsigned char rxeiradj, unsigned char rxpathdly, unsigned char txpathdly)
{
    write_reg32(EMI_RADIOTXRXTIM_ADDR, ((unsigned int)syncposition << 24) | ((unsigned int)rxeiradj << 16) | ((unsigned int)rxpathdly << 8) | ((unsigned int)txpathdly << 0));
}

static inline void emi_radiopwrupdn_txpwrupct_setf(unsigned char txpwrupct)
{
    write_reg32(EMI_RADIOPWRUPDN_ADDR, (read_reg32(EMI_RADIOPWRUPDN_ADDR) & ~((unsigned int)0x000000FF)) | ((unsigned int)txpwrupct << 0));
}
#endif

extern void br_rx_init(unsigned int rawstep, unsigned int winsize);

extern void br_tx_init(u32 rawstep);

extern void bt_init(void);

extern u32 BT_Mode_PKT_Init(unsigned char BTMode, unsigned char PktType);

extern void BT_Payload_Init(u8 PktLoad);

extern void BT_Clock_Init(ZB_mst_clock_e clk);

#endif /* VENDOR_BT_AUTOTEST_DEMO_BT_RF_BT_H_ */

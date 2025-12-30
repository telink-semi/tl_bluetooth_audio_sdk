/********************************************************************************************************
 * @file    ext_lib.h
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
#ifndef DRIVERS_TL721X_EXT_DRIVER_EXT_LIB_H_
#define DRIVERS_TL721X_EXT_DRIVER_EXT_LIB_H_


#include "types.h"
#include "../../uart.h"
#include "../../lib/include/pm/pm.h"
#include "lib/include/rf/rf_common.h"
#include <stdbool.h>
#include "../ext_pm.h"


/******************************* debug_start ******************************************************************/
void sub_wr_ana(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);
void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);
/******************************* debug_end ********************************************************************/


/******************************* dbgport start ******************************************************************/
#define reg_bb_dbg_sel   REG_ADDR16(0x140ca0)
#define reg_bb_dbg_sel_l REG_ADDR8(0x140ca0)
#define reg_bb_dbg_sel_h REG_ADDR8(0x140ca1)
#define bt_dbg_set_pin   dbg_bb_set_pin


void ble_dbg_port_init(int deg_sel0);

void dbg_bb_set_pin(gpio_pin_e pin);

void rf_enable_bb_debug(void);
/******************************* dbgport end ********************************************************************/


/******************************* dbgErrorCode start ******************************************************************/
/* for debug (write ram)*/
#define DBG_SRAM_ADDR            0x40014

#define PKE_OPERAND_MAX_WORD_LEN (0x08)
#define PKE_OPERAND_MAX_BIT_LEN  (0x100)
#define ECC_MAX_WORD_LEN         PKE_OPERAND_MAX_WORD_LEN
/*
 * addr - only 0x00012 ~ 0x00021 can be used !!! */
#define write_dbg32(addr, value) write_sram32(addr, value)

#define write_log32(err_code)    write_sram32(0x00014, err_code)
/******************************* dbgErrorCode end ********************************************************************/


/******************************* ext_aes start ******************************************************************/
#define HW_AES_CCM_ALG_EN 0

extern unsigned int aes_data_buff[8];


#define CV_LLBT_BASE (0x160000)

#define reg_rwbtcntl REG_ADDR32(CV_LLBT_BASE)

enum
{

    FLD_NWINSIZE     = BIT_RNG(0, 5),
    FLD_RWBT_RSVD6_7 = BIT_RNG(6, 7),

    FLD_RWBTEN      = BIT(8),
    FLD_CX_DNABORT  = BIT(9),
    FLD_CX_RXBSYENA = BIT(10),
    FLD_CX_TXBSYENA = BIT(11),
    FLD_SEQNDSB     = BIT(12),
    FLD_ARQNDSB     = BIT(13),
    FLD_FLOWDSB     = BIT(14),
    FLD_HOPDSB      = BIT(15),

    FLD_WHITDSB       = BIT(16),
    FLD_CRCDSB        = BIT(17),
    FLD_CRYPTDSB      = BIT(18),
    FLD_LMPFLOWDSB    = BIT(19),
    FLD_SNIFF_ABORT   = BIT(20),
    FLD_PAGEINQ_ABORT = BIT(21),
    FLD_RFTEST_ABORT  = BIT(22),
    FLD_SCAN_ABORT    = BIT(23),


    FLD_RWBT_RSVD24_25     = BIT_RNG(24, 25),
    FLD_CRYPT_SOFT_RST     = BIT(26), /**HW AES_CMM module reset*/
    FLD_SWINT_REQ          = BIT(27),
    FLD_RADIOCNTL_SOFT_RST = BIT(28),
    FLD_REG_SOFT_RST       = BIT(29),
    FLD_MASTER_TGSOFT_RST  = BIT(30),
    FLD_MASTER_SOFT_RST    = BIT(31),

};

void aes_encryption_le(u8 *key, u8 *plaintext, u8 *encrypted_data);
void aes_encryption_be(u8 *key, u8 *plaintext, u8 *encrypted_data);

bool aes_resolve_irk_rpa(u8 *key, u8 *addr);

void blt_ll_setAesCcmPara(u8 role, u8 *sk, u8 *iv, u8 aad, u64 enc_pno, u64 dec_pno, u8 lastTxLenFlag);


#define big_integer_compare   uint32_BigNumCmp
#define pke_eccp_point_mul    eccp_pointMul
#define pke_eccp_point_verify eccp_pointVerify
/******************************* ext_aes end ********************************************************************/


/******************************* ext_aoa start ******************************************************************/

/******************************* ext_aoa end ********************************************************************/


/******************************* ext_audio start ******************************************************************/

/******************************* ext_audio end ********************************************************************/


/******************************* ext_codec start ******************************************************************/

/******************************* ext_codec end ********************************************************************/


/******************************* ext_flash start ******************************************************************/

/******************************* ext_flash end ********************************************************************/


/******************************* ext_gpio start ******************************************************************/

/******************************* ext_gpio end ********************************************************************/


/******************************* ext_hci_uart start ******************************************************************/

/******************************* ext_hci_uart end ********************************************************************/


/******************************* ext_stimer start ******************************************************************/
#define SYSTICK_NUM_PER_US      24

#define SSLOT_TICK_NUM          1875 / 4 //attention: not use "()" for purpose !!!    625uS*24/32=625*3/4=1875/4=468.75
#define SSLOT_TICK_REVERSE      4 / 1875 //attention: not use "()" for purpose !!!


#define SYSTICK_NUM_PER_HALF_US (SYSTICK_NUM_PER_US / 2)


/*
 * @brief     This function performs to get system timer tick.
 * @return    system timer tick value.
 * @author    BLE group .
 */
#define bb_clock_time rf_bb_timer_get_tick

typedef enum
{
    STIMER_IRQ_MASK         = BIT(0),
    STIMER_32K_CAL_IRQ_MASK = BIT(1),
} ext_stimer_irq_mask_e;

typedef enum
{
    FLD_IRQ_SYSTEM_TIMER = BIT(0),
} system_timer_irq_mask_e;

typedef enum
{
    STIMER_IRQ_CLR         = BIT(0),
    STIMER_32K_CAL_IRQ_CLR = BIT(1),
} stimer_irq_clr_e;

/**
 * @brief define system clock tick per us/ms/s.
 */
enum
{
    SYSTEM_TIMER_TICK_78US125 = 1875,   //78.125uS = 78.125*24=1875
    SYSTEM_TIMER_TICK_125US   = 3000,   //125*24
    SYSTEM_TIMER_TICK_156US25 = 3750,   // 156.25uS = 156.25*24=7500
    SYSTEM_TIMER_TICK_40MS    = 960000, //24000*40=960000
};

/**
 * @brief    This function serves to enable system timer interrupt.
 * @return  none
 */
#define systimer_irq_enable() (reg_irq_src0 |= BIT(IRQ_SYSTIMER))

/**
 * @brief    This function serves to disable system timer interrupt.
 * @return  none
 */
#define systimer_irq_disable()         (reg_irq_src0 &= ~BIT(IRQ_SYSTIMER))

#define systimer_set_irq_mask()        (reg_system_irq_mask |= STIMER_IRQ_MASK)

#define systimer_clr_irq_mask()        (reg_system_irq_mask &= (~STIMER_IRQ_MASK))

#define systimer_get_irq_status()      (reg_system_cal_irq & FLD_IRQ_SYSTEM_TIMER)

#define systimer_clr_irq_status()      (reg_system_cal_irq = STIMER_IRQ_CLR)

#define systimer_set_irq_capture(tick) (reg_system_irq_level = tick)

#define systimer_get_irq_capture()     (reg_system_irq_level)

static _always_inline int tick1_exceed_tick2(u32 tick1, u32 tick2)
{
    return (u32)(tick1 - tick2) < BIT(30);
}

static _always_inline int tick1_closed_to_tick2(unsigned int tick1, unsigned int tick2, unsigned int tick_distance)
{
    return (unsigned int)(tick1 + tick_distance - tick2) < (tick_distance << 1);
}

static _always_inline int tick1_out_range_of_tick2(unsigned int tick1, unsigned int tick2, unsigned int tick_distance)
{
    return (unsigned int)(tick1 + tick_distance - tick2) > (tick_distance << 1);
}

/******************************* ext_stimer end ********************************************************************/


/******************************* ext_pm start ******************************************************************/
#ifndef PM_32k_RC_CALIBRATION_ALGORITHM_EN
#define PM_32k_RC_CALIBRATION_ALGORITHM_EN 1
#endif

#define SYS_NEED_REINIT_EXT32K   BIT(1)
#define WAKEUP_STATUS_TIMER_CORE (WAKEUP_STATUS_TIMER | WAKEUP_STATUS_CORE)
#define WAKEUP_STATUS_TIMER_PAD  (WAKEUP_STATUS_TIMER | WAKEUP_STATUS_PAD)

/**
 * @brief analog register below can store information when MCU in deepsleep mode
 *        store your information in these ana_regs before deepsleep by calling analog_write function
 *        when MCU wakeup from deepsleep, read the information by by calling analog_read function
 *        Reset these analog registers only by power cycle
 */
#define DEEP_ANA_REG0 PM_ANA_REG_POWER_ON_CLR_BUF0 //initial value =0x00  [Bit0][Bit1] is already occupied. The customer cannot change!
#define DEEP_ANA_REG1 PM_ANA_REG_POWER_ON_CLR_BUF1 //initial value =0x00
#define DEEP_ANA_REG2 PM_ANA_REG_POWER_ON_CLR_BUF2 //initial value =0xff
#define DEEP_ANA_REG3 PM_ANA_REG_POWER_ON_CLR_BUF3 //initial value =0x00
#define DEEP_ANA_REG4 PM_ANA_REG_POWER_ON_CLR_BUF4 //initial value =0x00
#define DEEP_ANA_REG5 PM_ANA_REG_POWER_ON_CLR_BUF5 //initial value =0x00
#define DEEP_ANA_REG6 PM_ANA_REG_POWER_ON_CLR_BUF6 //initial value =0x0f

/**
 * @brief these analog register can store data in deepsleep mode or deepsleep with SRAM retention mode.
 *        Reset these analog registers by watchdog, chip reset, RESET Pin, power cycle
 */

#define DEEP_ANA_REG7 PM_ANA_REG_WD_CLR_BUF0 //initial value =0xff    [Bit0] is already occupied. The customer cannot change!

//ana39 system used, user can not use
#define SYS_DEEP_ANA_REG PM_ANA_REG_POWER_ON_CLR_BUF0


extern unsigned char tl_24mrc_cal;
extern unsigned int  g_pm_mspi_cfg;

extern unsigned int g_sleep_32k_rc_cnt;
extern unsigned int g_sleep_stimer_tick;

extern unsigned int ota_program_bootAddr;
extern unsigned int ota_firmware_max_size;
extern unsigned int ota_program_offset;

#if (PM_32k_RC_CALIBRATION_ALGORITHM_EN)
/**
 * @brief   pm 32k rc calibration algorithm.
 */
typedef struct pmb_clock_drift
{
    unsigned int ref_tick;
    unsigned int ref_tick_32k;
    int          offset;
    int          offset_dc;
    //  int             offset_cur;         //BLE SDK use not
    unsigned int  offset_cal_tick; //BLE SDK use
    int           tc;
    int           rc32;
    int           rc32_wakeup;
    int           rc32_rt;
    int           s0;
    unsigned char calib;
    unsigned char ref_no; //BLE SDK use

} pm_clock_drift_t;

extern pm_clock_drift_t pmbcd;

/* pm 32k rc sw cal alg */

/**
 * @brief       Reset the 32k rc calibration function.
 * @return      none.
 */
void pm_ble_32k_rc_cal_reset(void);

/**
 * @brief      32k rc calibration function.
 * @param      offset_tick - the offset value of 32k rc calibration.
 * @param      rc32_cnt    - the 32k rc counter value.
 * @return     none.
 */
void pm_ble_32k_rc_cal_offset(int offset_tick, int rc32_cnt);

/**
 * @brief       32k rc calibration clock compensation.
 * @return      32k calibration value after compensation.
 */
unsigned int pm_ble_32k_rc_get_calib(void);

/**
 * @brief       Get the latest 32k rc calibration time.
 */
unsigned int pm_ble_32k_rc_get_latest_offset_cal_time(void);

/**
 * @brief       Update the reference 32k tick value and 24M system clock value when needed.
 * @param[in]   tick_32k    - the reference 32k tick value.
 * @param[in]   tick        - the reference 16M system clock value.
 * @return      none.
 */
void pm_ble_32k_rc_update_sleep_tick(unsigned int tick_32k, unsigned int tick);
#endif //#if (PM_32k_RC_CALIBRATION_ALGORITHM_EN)


#define PM_MIN_SLEEP_US 1500 //B92 todo

/**
 * @brief   internal oscillator or crystal calibration for environment change such as voltage, temperature
 *          to keep some critical PM or RF performance stable
 *          attention: this is a stack API, user can not call it
 * @param   none
 * @return  none
 */
void mcu_oscillator_crystal_calibration(void);

typedef int (*suspend_handler_t)(void);
typedef void (*check_32k_clk_handler_t)(void);
typedef unsigned int (*pm_get_32k_clk_handler_t)(void);
typedef void (*pm_tim_recover_handler_t)(void);

extern suspend_handler_t        func_before_suspend;
extern check_32k_clk_handler_t  pm_check_32k_clk_stable;
extern pm_get_32k_clk_handler_t pm_get_32k_tick;
extern pm_tim_recover_handler_t ext_pm_tim_recover;

#define DEFAULT_DEEPSLEEP_MODE_RET_SRAM_SIZE DEEPSLEEP_MODE_RET_SRAM_LOW64K
/******************************* ext_pm end ********************************************************************/


/******************************* mcu_security start ******************************************************************/
#define SECBOOT_DESC_SECTOR_NUM    1
#define SECBOOT_DESC_SIZE          0x1000 //4K for secure boot descriptor size

#define DESCRIPTOR_PUBKEY_OFFSET   0x0004

#define DESCRIPTOR_WATCHDOG_OFFSET 0x008C


#define DESC_SECTOR_DATA_LEN       144

typedef struct __attribute__((packed))
{
    unsigned char tlnk_mark[4];
    unsigned char public_key[64];
    unsigned char signature[64];
    unsigned int  run_code_adr; //4 byte
    unsigned int  run_code_size;
    unsigned char watdog_v[4];
} sb_desc_sector_t;

typedef struct __attribute__((packed))
{
    unsigned char  fw_enc_en;
    unsigned char  secboot_en;
    unsigned short sb_desc_adr_k; //unit: 4KB
} mcu_secure_t;

extern mcu_secure_t mcuSecur;

bool mcu_securuty_read_efuse(void);
bool mcu_securuty_read_idcode(void);
bool efuse_get_pubkey_hash(u8 *pHash);
/******************************* mcu_security end ********************************************************************/

#endif /* DRIVERS_TL721X_EXT_DRIVER_EXT_LIB_H_ */

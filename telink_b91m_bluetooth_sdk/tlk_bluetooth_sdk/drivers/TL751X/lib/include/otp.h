/********************************************************************************************************
 * @file    otp.h
 *
 * @brief   This is the header file for tl751x
 *
 * @author  Driver Group
 * @date    2023
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
/** @page OTP
 *
 *  Introduction
 *  ===============
 *  One Time Programmable (OTP)
 *
 *  API Reference
 *  ===============
 *  Header File: otp.h
 *
 *  How to use this driver
 *  ===============
 *   -# Init OTP clock using the function otp_clk_init().
 *   -# Power on OTP using the function otp_set_active_mode().
 *   -# OTP read/write, otp_read(), otp_write().
 *   -# OTP shutdown to save current using the function otp_set_deep_standby_mode().
 * 
 */
#ifndef OTP_H_
#define OTP_H_
#include "reg_include/register.h"
#include "compiler.h"

/**
 * @brief IP enable time, pwe=1 ---> enable, Tcs > 10(us).
 */
#define Tcs (15)

/**
 * @brief Program pulse width time, pwe high time, 20 < Tpw < 25(us).
 */
#define Tpw (22)

/**
 * @brief Program pulse interval time, pwe low time, 1 < Tpwi < 5(us).
 */
#define Tpwi (4)

/**
 * @brief Program mode recovery time, pporg=0 ---> pce=0, 5 < Tppr < 100(us).
 */
#define Tppr (10)

/**
 * @brief Program mode setup time, pprog=1 ---> pwe=1, 5 < Tpps < 20(us).
 */
#define Tpps (5)

/**
 * @brief Program mode hold time, pwe=0 ---> pprog=0, 5 < Tpph < 20(us).
 */
#define Tpph (10)

/**
 * @brief Deep standby to active mode setup time, deep standby to active mode setup time, Tsas >2(us).
 */
#define Tsas (5)

/**
 * @brief PTM mode setup time, ptm  -->  pce = 1, Tms>1(ns).
 */
#define Tms (1)

/**
 * @brief PTM mode hold time, ptm  -->  pce = 0, Tmh>1(ns).
 */
#define Tmh (1)

/**
 * @brief IP enable time in program, pce = 1 ---> pprog=1, 10 < Tcsp < 100(us).
 */
#define Tcsp (20)

/**
 * @brief LDO setup time, ldo setup time, Tpls > 10(us).
 */
#define Tpls (15)

/**
 * @brief PTM type.
 */
typedef enum
{
    OTP_PTM_READ             = 0x00, /**< PTM mode read */
    OTP_PTM_PROG             = 0x02, /**< PTM mode write */
    OTP_PTM_INIT_MARGIN_READ = 0x01, /**< PTM mode init margin read */
    OTP_PTM_PGM_MARGIN_READ  = 0x04, /**< PTM mode pgm margin read */
} otp_ptm_type_e;

/**
 * @brief Auto power up time config.
 */
typedef enum
{
    OTP_TIM_CONFIG_24M  = 0x00, /**< PCLK (0-24M] */
    OTP_TIM_CONFIG_48M  = 0x01, /**< PCLK (24-48M] */
    OTP_TIM_CONFIG_96M  = 0x02, /**< PCLK (48-96M] */
    OTP_TIM_CONFIG_192M = 0x03, /**< PCLK (96-192M] */
    OTP_TIM_CONFIG_384M = 0x04, /**< PCLK (192-384M] */
} otp_tim_config_e;

/**
 * @brief Auto load data index.
 */
typedef enum
{
    OTP_AUTO_LOAD_W0 = 0x00, /**< auto load word 0 */
    OTP_AUTO_LOAD_W1 = 0x01, /**< auto load word 1 */
    OTP_AUTO_LOAD_W2 = 0x02, /**< auto load word 2 */
} otp_auto_load_e;

/**********************************************************************************************************************
 *                                                External interface                                                  *
 *********************************************************************************************************************/
/*!
 * @name External functions
 * @{
 */

/**
 * @brief      This function serves to get otp auto load data. As soon as the system is powered on, \n
 *             OTP will automatically load the 3-word data from OTP address 0 to the auto_load register.
 * @param[in]  index - auto load data index.
 * @return     Auto load data(word).
 * @note       The auto load is triggered when the device is powered on for the first time, reset pin, or deep wake up.
 */
static inline unsigned int otp_get_auto_load_data(otp_auto_load_e index)
{
    return reg_otp_auto_load_data(index);
}

/**
 * @brief      This function serves to enable key lock. After key lock enable, the data in OTP word address 3~10 cannot be read.
 * @return     none
 * @note       key_lock with a reset value of 0 and can only be written from 0 to 1, not from 1 to 0.
 */
static inline void otp_key_lock(void)
{
    reg_otp_status |= FLD_OTP_KEYLOCK;
}

/**
 * @brief      This function serves to enable row test.
 * @return     none
 */
static inline void otp_test_row_en(void)
{
    reg_otp_ctrl0 |= FLD_OTP_PTR;
    reg_otp_ctrl0 &= ~(FLD_OTP_PTC);
}

/**
 * @brief      This function serves to enable column test.
 * @return     none
 */
static inline void otp_test_column_en(void)
{
    reg_otp_ctrl0 &= ~(FLD_OTP_PTR);
    reg_otp_ctrl0 |= FLD_OTP_PTC;
}

/**
 * @brief      This function serves to disable test mode.
 * @return     none
 */
static inline void otp_test_dis(void)
{
    reg_otp_ctrl0 &= ~(FLD_OTP_PTR | FLD_OTP_PTC);
}

/**
 * @brief      This function serves to init otp clk. This interface must be called to initialize the otp clock before using otp.
 * @return     none
 */
void otp_clk_init(void);

/**
 * @brief      This function serves to set otp active mode(manual), if otp is in deep mode, need to operate on the otp, set active mode.
 * @return     none
 */
void otp_set_active_mode(void);

/**
 * @brief      This function serves to set otp active mode(auto), if otp is in deep mode, need to operate on the otp, set active mode.
 * @return     none
 */
void otp_set_auto_active_mode(void);

/**
 * @brief      This function serves to otp set deep standby mode, can enter deep to save current.
 * @return     none
 */
void otp_set_deep_standby_mode(void);

/**
 * @brief      This function serves to read data from OTP memory, belong to otp normal read.
 *             otp has three kinds of read mode,in general,just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return     none
 */
void otp_read(unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @brief      This function serves to write data to OTP memory.
 *             the minimum unit of otp read-write operation is 4 bytes, that is a word. meanwhile, the otp cannot be burned repeatedly,
 *             this function is limited to writing only once,this function will determine if the otp is 0xffffffff, and if it is 0xffffffff,
 *             it will write the otp.
 * @param[in]  addr - the address of the data,it has to be a multiple of 4,the OTP memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return
 *             - 0 it means that the otp operation area is 0xffffffff or the write data,
 *                return 0 not mean that the burning was successful,need to use three kinds of read mode to check whether the writing was successful.
 *             - 1 it means that there is an operation value in the operation area,it is not 0xffffffff or the write data,no burning action is performed.
 */
unsigned char otp_write(unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @brief      This function serves to read data from OTP memory,belong to otp pgm margin read.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return     none
 */
void otp_pgm_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @brief      This function serves to read data from OTP memory,belong to otp initial margin read.
 *             otp has three kinds of read mode,in general, just use OTP_READ normal read operation, when the execution of burning operation,
 *             need to use margin read(otp_pgm_margin_read,otp_initial_margin_read),check whether the write is successful.
 * @param[in]  addr - the otp address of the data,it has to be a multiple of 4,the otp memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return     none
 *
 */
void otp_initial_margin_read(unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @}
 */

/**********************************************************************************************************************
 *                                                Internal interface                                                  *
 *********************************************************************************************************************/
/*!
 * @name Internal functions
 * @{
 */

/**
 * @brief      This function serves to select read mode as auto.
 * @return     none
 */
static inline void otp_read_auto_mode(void)
{
    reg_otp_ctrl3 &= ~(FLD_OTP_MAN_MODE);
}

/**
 * @brief      This function serves to select read mode as manual.
 * @return     none
 */
static inline void otp_read_manual_mode(void)
{
    reg_otp_ctrl3 |= FLD_OTP_MAN_MODE;
}

/**
 * @brief      This function serves to enable ecc.
 * @return     none
 */
static inline void otp_ecc_en(void)
{
    reg_otp_ctrl0 &= ~(FLD_OTP_ECC_RDB);
}

/**
 * @brief      This function serves to disable ecc.
 * @return     none
 */
static inline void otp_ecc_dis(void)
{
    reg_otp_ctrl0 |= FLD_OTP_ECC_RDB;
}

/**
 * @brief      This function serves to manual read data from OTP memory(for internal test).
 * @param[in]  ptm_mode - read mode.
 * @param[in]  addr - the address of the data,the otp memory that can access is from 0x0000-0x3FC,can't access other address.
 * @param[in]  word_len  - the length of the data,the unit is word(4 bytes).
 * @param[out] buff - data buff.
 * @return     none
 */
_attribute_ram_code_sec_noinline_ void otp_read_cycle_manual(otp_ptm_type_e ptm_mode, unsigned int addr, unsigned int word_len, unsigned int *buff);

/**
 * @}
 */

#endif /* OTP_H_ */

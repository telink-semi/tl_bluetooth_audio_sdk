/********************************************************************************************************
 * @file    clock.h
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
/** @page CLOCK
 *
 *  Introduction
 *  ===============
 *  tl751x clock setting.
 *
 *  API Reference
 *  ===============
 *  Header File: clock.h
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "lib/include/analog.h"
#include "compiler.h"
#include "reg_include/register.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
/**
 * The following chart lists the maximum operating frequency that each clock source can operate at different voltage levels.
 * (But if it is related to peripherals, this frequency will be related to the whole system, such as MSPI-->
 *  If it is built-in flash, the maximum speed of mspi is 64M,If it is an external flash, the maximum speed
 *  of mspi needs to be based on the board test.)
 * :-------------------------------- Voltage versus frequency(default is 0.8v)  table clock_h_1 --------------------------------------------------------------------------
 * |voltage|  pll_bb | pll_audio | cclk_d25f_dsp | hclk | pclk | clk_n22 | mspi | lspi | gspi | sspi | sdio  | i2s | audio | spdif | anc | sar  | wt_clk      |   alg      |
 * |  0.9V |    312  |   200     |    300        | 150  | 75   |  48     |  64  | 48   | 48   |  60  | 48    | 13  |  37   |  185  | 185 | 24   | 32k_12.288M |    24      |
 * |  0.8V |    312  |   200     |    132        |  66  |  66  |  48     |  64  | 48   | 48   |  60  | 48    | 13  |  37   |  185  | 185 | 24   | 32k_12.288M |    20      |
 */
/*
 * ALG_MODULE_MAX_CLK is configured to 20:
 * - The alg clock source is pclk, with a default division of 1/2.
 *   - 1.0v, pclk maximum 66m, alg maximum 20m.
 *   - 1.1v, pclk maximum 96m, alg maximum 24m.
 * It is necessary to modify the frequency division based on the current size of the pclk. For the convenience of software processing, it is uniformly processed as 20m.
 */
#define ALG_MODULE_MAX_CLK                20

#define INTERNAL_PLL_CLK_TEST             0
/*
 * note:
 * 1.Clock source pll, there are two kinds of clock source,pll_bb and pll_audio, you can flexibly choose which kind of clock pll source,
 *   here is just to provide pll_bb for the clock source of the various frequency divider configurations.
 * 2.If you use pll_audio as the clock source, refer to the pll_bb configuration here.
 */

//0.9v (note: the maximum pclk is 75m )
#define PLL_192M_D25F_DSP_192M_HCLK_96M_PCLK_48M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV4);
//0.8v
#define PLL_264M_D25F_DSP_132M_HCLK_66M_PCLK_66M_MSPI_44M_WT_11M \
    clock_init(CLK_BASEBAND_PLL_264M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, PLL_DIV6_TO_MSPI_CLK, CLK_DIV6);
#define PLL_192M_D25F_DSP_96M_HCLK_48M_PCLK_48M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV4);
#define PLL_192M_D25F_DSP_96M_HCLK_48M_PCLK_24M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M,CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV2);
#define PLL_192M_D25F_DSP_48M_HCLK_48M_PCLK_48M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV4, CCLK_DIV1_TO_HCLK_DIV1_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV4);
#define PLL_192M_D25F_DSP_48M_HCLK_48M_PCLK_24M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV4, CCLK_DIV1_TO_HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV2);
#define PLL_192M_D25F_DSP_48M_HCLK_48M_PCLK_12M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M,CLK_DIV4, CCLK_DIV1_TO_HCLK_DIV4_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV1);
#define PLL_192M_D25F_DSP_48M_HCLK_24M_PCLK_24M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M,CLK_DIV4, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV2);
#define PLL_192M_D25F_DSP_48M_HCLK_24M_PCLK_12M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV4, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV1);
#define PLL_192M_D25F_DSP_24M_HCLK_12M_PCLK_12M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV8, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV1);
#define PLL_192M_D25F_DSP_24M_HCLK_24M_PCLK_12M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV8, CCLK_DIV1_TO_HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV1);
#define PLL_192M_D25F_DSP_24M_HCLK_24M_PCLK_24M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_192M, CLK_DIV8, CCLK_DIV1_TO_HCLK_DIV1_TO_PCLK, PLL_DIV4_TO_MSPI_CLK, CLK_DIV2);

#define XTAL_48M_D25F_DSP_48M_HCLK_48M_PCLK_48M_MSPI_48M clock_init(CLK_XTAL_48M,CLK_DIV1, CCLK_DIV1_TO_HCLK_DIV1_TO_PCLK, PLL_DIV1_TO_MSPI_CLK, CLK_DIV4);
#define RC_24M_D25F_DSP_24M_HCLK_24M_PCLK_24M_MSPI_24M   clock_init(CLK_RC_24M,CLK_DIV1, CCLK_DIV1_TO_HCLK_DIV1_TO_PCLK, PLL_DIV1_TO_MSPI_CLK, CLK_DIV2);

/**********************************************************************************************************************
 *                                          INTERNAL_PLL_CLK_TEST                                                     *
 *********************************************************************************************************************/
#if (INTERNAL_PLL_CLK_TEST)
#define PLL_264M_D25F_DSP_132M_HCLK_66M_PCLK_66M_MSPI_44M_WT_11M \
    clock_init(CLK_BASEBAND_PLL_264M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, PLL_DIV6_TO_MSPI_CLK, CLK_DIV6);
#define PLL_276M_D25F_DSP_138M_HCLK_69M_PCLK_69M_MSPI_46M_WT_11M \
    clock_init(CLK_BASEBAND_PLL_276M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, PLL_DIV6_TO_MSPI_CLK, CLK_DIV6);
#define PLL_288M_D25F_DSP_144M_HCLK_72M_PCLK_72M_MSPI_48M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_288M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, PLL_DIV6_TO_MSPI_CLK, CLK_DIV6);
#define PLL_300M_D25F_DSP_150M_HCLK_75M_PCLK_75M_MSPI_42M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_300M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK, PLL_DIV7_TO_MSPI_CLK, CLK_DIV6);
#define PLL_312M_D25F_DSP_156M_HCLK_78M_PCLK_39M_MSPI_44M_WT_9M \
    clock_init(CLK_BASEBAND_PLL_312M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV7_TO_MSPI_CLK, CLK_DIV4);
#define PLL_324M_D25F_DSP_162M_HCLK_81M_PCLK_40M_MSPI_46M_WT_10M \
    clock_init(CLK_BASEBAND_PLL_324M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV7_TO_MSPI_CLK, CLK_DIV4);
#define PLL_336M_D25F_DSP_168M_HCLK_84M_PCLK_42M_MSPI_48M_WT_10M \
    clock_init(CLK_BASEBAND_PLL_336M, CLK_DIV2, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV7_TO_MSPI_CLK, CLK_DIV4);

#define PLL_204M_D25F_DSP_204M_HCLK_102M_PCLK_51M_MSPI_40M_WT_12M \
    clock_init(CLK_BASEBAND_PLL_204M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV5_TO_MSPI_CLK, CLK_DIV4);
#define PLL_216M_D25F_DSP_216M_HCLK_108M_PCLK_54M_MSPI_43M_WT_10M \
    clock_init(CLK_BASEBAND_PLL_216M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV5_TO_MSPI_CLK, CLK_DIV5);
#define PLL_228M_D25F_DSP_228M_HCLK_114M_PCLK_57M_MSPI_45M_WT_11M \
    clock_init(CLK_BASEBAND_PLL_228M, CLK_DIV1, CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK, PLL_DIV5_TO_MSPI_CLK, CLK_DIV5);
#endif

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 *  @brief  Define sys_clk struct.
 */
typedef struct
{
    unsigned short pll_bb_clk;    /**<pll_bb_clk*/
    float          pll_audio_clk; /**<pll_audio_clk*/
    float          cclk_d25f_dsp; /**<cpu d25f_dsp clk */
    float          hclk;          /**<hclk  */
    float          pclk;          /**< pclk */
    float          mspi_clk;      /**< mspi_clk */
    float          n22_clk;       /**< n22 clk */
    float          wt_clk;        /**< wt clk */
} sys_clk_t;

/**
 *  @brief  Define sys_clk_config_t struct.
 */
typedef struct
{
    unsigned char cclk_d25f_dsp_cfg; /**< cpu d25f_dsp clk cfg */
    unsigned char hclk_pclk_cfg;     /**< hclk_pclk cfg */
    unsigned char mspi_clk_cfg;      /**< mspi_clk cfg */
    unsigned char n22_clk_cfg;       /**< n22 clk cfg */
    unsigned char wt_clk_cfg;        /**< wt clk cfg */
    unsigned char rc_24m_is_used;
    unsigned char bbpll_is_used;
    unsigned char audio_pll_is_used;
} sys_clk_config_t;

extern sys_clk_config_t sys_clk_config;

typedef enum
{
    PLL_192M = 0,
    PLL_264M,
    PLL_300M,
#if (INTERNAL_PLL_CLK_TEST)
    PLL_204M,
    PLL_216M,
    PLL_228M,
    PLL_276M,
    PLL_288M,
    PLL_312M,
    PLL_324M,
    PLL_336M,
#endif
} pll_bb_clk_e;

typedef struct
{
    unsigned short     clk;
    unsigned long long clk_config;
} sys_pll_bb_config_t;

typedef struct
{
    pll_bb_clk_e index;
    sys_pll_bb_config_t cfg;
}sys_pll_bb_clk_t;

typedef enum
{
    PLL_AUDIO_CLK_33P8688M = 0,
    PLL_AUDIO_CLK_36P864M,
    PLL_AUDIO_CLK_147P456M,
    PLL_AUDIO_CLK_169P344M,
    PLL_AUDIO_CLK_184P32M,
} pll_audio_clk_e;

typedef struct
{
    float              clk;
    unsigned long long clk_confog;
} sys_pll_audio_config_t;

typedef struct
{
    pll_audio_clk_e  index;
    sys_pll_audio_config_t cfg;
} sys_pll_audio_clk_t;

/**
 * @brief system clock type.
 */
typedef enum
{
    RC_24M       = 0x00,
    XTAL_48M     = 0x10,
    BASEBAND_PLL = 0x20,
    AUDIO_PLL    = 0x30,
} sys_clock_src_e;


typedef enum
{
    CLK_RC_24M             = RC_24M ,
    CLK_XTAL_48M           = XTAL_48M,
    CLK_BASEBAND_PLL_192M  = BASEBAND_PLL | (PLL_192M << 8),
    CLK_BASEBAND_PLL_264M  = BASEBAND_PLL | (PLL_264M << 8),
    CLK_BASEBAND_PLL_300M  = BASEBAND_PLL | (PLL_300M << 8),
#if (INTERNAL_PLL_CLK_TEST)
    CLK_BASEBAND_PLL_204M  = BASEBAND_PLL | (PLL_204M << 8),
    CLK_BASEBAND_PLL_216M  = BASEBAND_PLL | (PLL_216M << 8),
    CLK_BASEBAND_PLL_228M  = BASEBAND_PLL | (PLL_228M << 8),
    CLK_BASEBAND_PLL_276M  = BASEBAND_PLL | (PLL_276M << 8),
    CLK_BASEBAND_PLL_288M  = BASEBAND_PLL | (PLL_288M << 8),
    CLK_BASEBAND_PLL_312M  = BASEBAND_PLL | (PLL_312M << 8),
    CLK_BASEBAND_PLL_324M  = BASEBAND_PLL | (PLL_324M << 8),
    CLK_BASEBAND_PLL_336M  = BASEBAND_PLL | (PLL_336M << 8),
#endif
    CLK_AUDIO_PLL_33P8688M = AUDIO_PLL | (PLL_AUDIO_CLK_33P8688M << 8),
    CLK_AUDIO_PLL_36P864M  = AUDIO_PLL | (PLL_AUDIO_CLK_36P864M << 8),
    CLK_AUDIO_PLL_147P456M = AUDIO_PLL | (PLL_AUDIO_CLK_147P456M << 8),
    CLK_AUDIO_PLL_169P344M = AUDIO_PLL | (PLL_AUDIO_CLK_169P344M << 8),
    CLK_AUDIO_PLL_184P32M  = AUDIO_PLL | (PLL_AUDIO_CLK_184P32M << 8),
} sys_clk_src_config_e;

/*
 *            1.spi clock source selection,if you select SRC_CLK_BASEBAND_PLL and SRC_CLK_AUDIO_PLL,the frequency should be consistent with the clk initialization function.
 *            2.for master  module
 *            Due to the divider register bit width limitation,The limits between src_clk and spi_clock are as follows.
              src_clk                LSPI(spi_clk minimum)=src_clk/15     GSPI(spi_clk  minimum)=src_clk/255
                SRC_CLK_RC_24M              1600000                          94117.64706

                SRC_CLK_XTAL_48M            3200000                          188235.2941

                SRC_CLK_BASEBAND_PLL_192M   12800000                         752941.1765
                SRC_CLK_BASEBAND_PLL_264M   17600000                         1035294.118
                SRC_CLK_BASEBAND_PLL_300M   20000000                         1176470.588

                SRC_CLK_AUDIO_PLL_33P8688M  2257920                          132818.8235
                SRC_CLK_AUDIO_PLL_36P864M   2457600                          144564.7059
                SRC_CLK_AUDIO_PLL_147P456M  9830400                          578258.8235
                SRC_CLK_AUDIO_PLL_169P344M  11289600                         664094.1176
                SRC_CLK_AUDIO_PLL_184P32M   12288000                         722823.5294

               3.for slave module
               since the maximum clock of the slave spi module is 48M, according to the formula(spi_clock(output from master clk_pin ) <=  slave module clk/4),
               The limits between src_clk and spi_clock  are as follows.
                src_clk                 LSPI/GPSI(spi_clk maximum )=src_clk/4
                SRC_CLK_RC_24M               24000000/4=6000000

                SRC_CLK_XTAL_48M             48000000/4=1200000

                SRC_CLK_BASEBAND_PLL_192M    48000000/4=12000000
                SRC_CLK_BASEBAND_PLL_264M    44000000/4=11000000
                SRC_CLK_BASEBAND_PLL_300M    42857142.86/4=10714285.7

                SRC_CLK_AUDIO_PLL_33P8688M   33868800/4= 8467200
                SRC_CLK_AUDIO_PLL_36P864M    36864000/4= 9216000
                SRC_CLK_AUDIO_PLL_147P456M   36864000/4= 9216000
                SRC_CLK_AUDIO_PLL_169P344M   42336000/4=10584000
                SRC_CLK_AUDIO_PLL_184P32M    46080000/4=11520000
*/
typedef enum
{
    SRC_CLK_RC_24M             = 24 * 1000000 | (RC_24M >> 4),
    SRC_CLK_XTAL_48M           = 48 * 1000000 | (XTAL_48M >> 4),
    SRC_CLK_BASEBAND_PLL_192M  = 192 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_264M  = 264 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_300M  = 300 * 1000000 | (BASEBAND_PLL >> 4),
#if (INTERNAL_PLL_CLK_TEST)
    SRC_CLK_BASEBAND_PLL_204M  = 204 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_216M  = 216 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_228M  = 228 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_276M  = 276 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_288M  = 288 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_312M  = 312 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_324M  = 324 * 1000000 | (BASEBAND_PLL >> 4),
    SRC_CLK_BASEBAND_PLL_336M  = 336 * 1000000 | (BASEBAND_PLL >> 4),
#endif
    SRC_CLK_AUDIO_PLL_33P8688M = 33868800 | (AUDIO_PLL >> 4),
    SRC_CLK_AUDIO_PLL_36P864M  = 36864000 | (AUDIO_PLL >> 4),
    SRC_CLK_AUDIO_PLL_147P456M = 147456000 | (AUDIO_PLL >> 4),
    SRC_CLK_AUDIO_PLL_169P344M = 169344000 | (AUDIO_PLL >> 4),
    SRC_CLK_AUDIO_PLL_184P32M  = 184320000 | (AUDIO_PLL >> 4),
} sys_src_clk_e;

/**
 * @brief system clock power up status.
 */
typedef enum
{
    MODULE_CPU  = BIT(0),
    MODULE_MSPI = BIT(1),
    MODULE_GSPI = BIT(2), //The driver does not provide an interface for selecting 24M RC,
                          //so no relevant processing is done here.
    MODULE_ADC = BIT(3),  //The driver does not provide an interface for selecting 24M RC,
                          //so no relevant processing is done here.
} clk_src_24m_rc_use_modules_e;

/**
 * @brief 32K clock type.
 */
typedef enum
{
    CLK_32K_RC   = 0,
    CLK_32K_XTAL = 1,
} clk_32k_type_e;

/**
 * @brief clock division type. clock division to cclk. cclk division to hclk_pclk,clock division to clk_mspi. clock division to clk_lspi. pclk division to clk_wt. clock division to DSP. XTAL48M division to clk_n22.
 */
typedef enum
{
    CLK_DIV1 = 1,
    CLK_DIV2,
    CLK_DIV3,
    CLK_DIV4,
    CLK_DIV5,
    CLK_DIV6,
    CLK_DIV7,
    CLK_DIV8,
    CLK_DIV9,
    CLK_DIV10,
    CLK_DIV11,
    CLK_DIV12,
    CLK_DIV13,
    CLK_DIV14,
    CLK_DIV15,
} sys_clock_div_e;

/**
 * @brief cclk div to hclk. cclk div to pclk.
 */
typedef enum
{
    CCLK_DIV1_TO_HCLK_DIV1_TO_PCLK = 0, //cclk:hclk:pclk = 1:1:1;
    CCLK_DIV1_TO_HCLK_DIV2_TO_PCLK = 1, //cclk:hclk:pclk = 1:1:2;
    CCLK_DIV1_TO_HCLK_DIV4_TO_PCLK = 2, //cclk:hclk:pclk = 1:1:4;
    CCLK_DIV2_TO_HCLK_DIV2_TO_PCLK = 4, //cclk:hclk:pclk = 1:2:2;
    CCLK_DIV2_TO_HCLK_DIV4_TO_PCLK = 5, //cclk:hclk:pclk = 1:2:4;
} sys_cclk_div_to_hclk_pclk_e;

/**
 * @brief pll_div to mspi clk,the mspi can only be configured up to 64m.
 */
typedef enum
{
    PLL_DIV1_TO_MSPI_CLK  = 1,
    PLL_DIV2_TO_MSPI_CLK  = 2,
    PLL_DIV3_TO_MSPI_CLK  = 3,
    PLL_DIV4_TO_MSPI_CLK  = 4,
    PLL_DIV5_TO_MSPI_CLK  = 5,
    PLL_DIV6_TO_MSPI_CLK  = 6,
    PLL_DIV7_TO_MSPI_CLK  = 7,
    PLL_DIV8_TO_MSPI_CLK  = 8,
    PLL_DIV9_TO_MSPI_CLK  = 9,
    PLL_DIV10_TO_MSPI_CLK = 10,
    PLL_DIV11_TO_MSPI_CLK = 11,
    PLL_DIV12_TO_MSPI_CLK = 12,
    PLL_DIV13_TO_MSPI_CLK = 13,
    PLL_DIV14_TO_MSPI_CLK = 14,
    PLL_DIV15_TO_MSPI_CLK = 15,
} sys_pll_div_to_mspi_clk_e;

/**
 *  @brief  Define rc_24M_cal enable/disable.
 */
typedef enum
{
    RC_24M_CAL_DISABLE = 0,
    RC_24M_CAL_ENABLE,
} rc_24M_cal_e;

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/
extern sys_clk_t      sys_clk;
extern clk_32k_type_e g_clk_32k_src;
extern unsigned char  g_24m_rc_is_used;
extern unsigned char  g_bbpll_is_used;
extern unsigned char  g_audio_pll_is_used;
/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief       This function serves to set 32k clock source.
 * @param[in]   src - variable of 32k type.
 * @return      none.
 */
void clock_32k_init(clk_32k_type_e src);

/**
 * @brief       This function serves to kick 32k xtal.
 * @param[in]   xtal_times - kick times.
 * @return      1 success, 0 error.
 */
unsigned char clock_kick_32k_xtal(unsigned char xtal_times);

/**
 * @brief     This function serves to calibrate 24M RC.
 *            24M RC is inaccurate, and it is greatly affected by temperature, if need use it so real-time calibration is required
 *            The 24M RC needs to be calibrated before the pm_sleep_wakeup function,
 *            because this clock will be used to kick 24m xtal start after wake up,
 *            The more accurate this time, the faster the crystal will start.Calibration cycle depends on usage
 * @return    none.
 */
_attribute_ram_code_sec_noinline_ void clock_cal_24m_rc(void);

/**
 * @brief     This function serves to calibrate 32K RC.
 * @return    none.
 */
void clock_cal_32k_rc(void);

/**
 * @brief  This function serves to get the 32k tick currently.
 * @return the current 32k tick.
 */
_attribute_ram_code_sec_noinline_ unsigned int clock_get_32k_tick(void);

/**
 * @brief  This function serves to set the 32k tick for sleep.
 * @param  tick - the value of to be set to 32k.
 * @return none.
 */
_attribute_ram_code_sec_noinline_ void clock_set_32k_tick(unsigned int tick);

/**
 * @brief       This function use to enable pll audio,Voltage versus frequency refer to table clock_h_1.
 * @param[in]   clk - the pll audio clk
 * @return      none.
 * @note        1. If pll_audio is used as the clock source, when switching the size:
 *                 clock_init(AUDIO_PLL);
 *              2. if pll_audio is not used as the clock source,only call the interface.
 */
_attribute_ram_code_sec_noinline_ void clock_pll_audio_init(pll_audio_clk_e clk);

/**
 * @brief       This function use to select the system clock source,voltage versus frequency refer to table clock_h_1.
 * @param[in]   src                   - cclk source.
 * @param[in]   cclk_d25f_dsp_div     - the cclk divide from src
 * @param[in]   hclk_div              - the hclk divide from cclk.
 * @param[in]   pclk_div              - the pclk divide from hclk.
 * @param[in]   mspi_clk_div - mspi_clk can be divided from pll, rc and xtal.
 *                             When selecting pll as the clock source, in order to not exceed the maximum frequency.
 *                             If it is built-in flash, the maximum speed of mspi is 64M.
 *                             If it is an external flash, the maximum speed of mspi needs to be based on the board test.
 *                             Because the maximum speed is related to the wiring of the board, and is also affected by temperature and GPIO voltage,
 *                             the maximum speed needs to be tested at the highest and lowest voltage of the board,
 *                             and the high and low temperature long-term stability test speed is no problem.
 * @return      none
 * @note        1.Do not switch the clock during the DMA sending and receiving process;
 *              because during the clock switching process, the system clock will be
 *              suspended for a period of time, which may cause data loss
 *              2.This interface is to handle mspi_clk and cclk_clk according to a unified clock source;
 *              3. When switching to pll via clock_d25fclk_hclk_pclk_wtclk_config/clock_mspi_clk_config, it is necessary to place a guarantee
 *              that the interface will be called as well as selecting the parameter src to the corresponding pll source.
 */
void clock_init(sys_clk_src_config_e        src,
                sys_clock_div_e             cclk_div,
                sys_cclk_div_to_hclk_pclk_e hclk_pclk_div,
                sys_pll_div_to_mspi_clk_e   mspi_clk_div,
                sys_clock_div_e             wt_div);


/**
 * @brief       This function used to configure the frequency of CCLK/HCLK/PCLK,voltage versus frequency refer to table clock_h_1.
 *              You need to wait until all the peripherals that use these clocks are idle before you can switch frequencies.
 * @param[in]   src - clock source.
 * @param[in]   cclk_div - divider of CCLK.
 * @param[in]   hclk_div - divider of HCLK.
 * @param[in]   pclk_div - divider of PCLK.
 * @param[in]   wt_div - the wt divider
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void clock_d25fclk_hclk_pclk_wtclk_config(sys_clock_src_e src, sys_clock_div_e cclk_div, sys_cclk_div_to_hclk_pclk_e hclk_pclk_div, sys_clock_div_e wt_div);

/**
 * @brief       This function use to configure the mspi clock source,Voltage versus frequency refer to table clock_h_1.
 * @param[in]   src - the mspi clk source
 * @param[in]   div - mspi_clk can be divided from pll, rc and xtal.
 *                    When selecting pll as the clock source, in order to not exceed the maximum frequency, if it is built-in flash, the maximum speed of mspi is 64M.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void clock_mspi_clk_config(sys_clock_src_e src, sys_clock_div_e div);

/**
 * @brief       This function use to configure the n22 clock source,Voltage versus frequency refer to table clock_h_1.
 * @param[in]   div - the n22 clk source divider. The maximum clock value of N22 is 48M Hz.
 * @return      none.
 */
_attribute_ram_code_sec_noinline_ void clock_n22_clk_config(sys_clock_div_e div);

/**
  * @brief       This function use to set all clock to default.
  * @return      none.
  * @note        After call this, the following clock will set to default source and value:
  *              -----------------------------------------------------------------------
  *              clock source |          clock
  *              -----------------------------------------------------------------------
  *              RC 24M       | HCLK 24M, PCLK 24M, WTCLK 4M, CCLK CLK 24M(dsp and cclk are bound together), MSPI CLK 24M.
  *              XTAL 48M     | N22 CLK 48M
  *              -----------------------------------------------------------------------
  */
_attribute_ram_code_sec_noinline_ void clock_set_all_clock_to_default(void);

/**
 * @brief       This function use to set all clock to XTAL.
 * @return      none.
 * @note        After call this, the following clock will set to XTAL source:
 *              -----------------------------------------------------------------------
 *              clock source |          clock
 *              -----------------------------------------------------------------------
 *              XTAL 48M     | HCLK 24M, PCLK 24M, WTCLK 4M, CCLK_D25F_DSP CLK 24M, MSPI CLK 48M, N22 CLK 48M.
 *              -----------------------------------------------------------------------
 */
_attribute_ram_code_sec_optimize_o2_noinline_ void clock_set_all_clock_to_xtal(void);

/**
 * @brief       This function use to save all clock configuration for the follow-up restore.
 * @return      none.
 * @note        This function needs to be used in conjunction with clock_restore_clock_config().
 */
_attribute_ram_code_sec_noinline_ void clock_save_clock_config(void);

/**
 * @brief       This function use to restore all previously saved clock configurations.
 * @return      none.
 * @note        This function needs to be used in conjunction with clock_save_clock_config().
 */
_attribute_ram_code_sec_noinline_ void clock_restore_clock_config(void);
#endif

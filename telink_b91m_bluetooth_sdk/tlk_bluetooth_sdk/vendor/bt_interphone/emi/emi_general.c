/********************************************************************************************************
 * @file    emi_general.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#include "drivers.h"
#include "tlkmw/tlkmw.h"
#include "emi_general.h"
#include "tlkmw/sys_dev/serial/tlkdrv_serial.h"
#include "ext_driver/bredr_internal/emi/bredr_emi.h"
void emi_init(void);
void emicarrieronly(RF_MODE_BT_e rf_mode, unsigned char pwr_index, signed char rf_chn);
void emi_tx_continue(RF_MODE_BT_e rf_mode, unsigned char pwr_index, signed char rf_chn);
void emirx(RF_MODE_BT_e rf_mode, unsigned char pwr_index, signed char rf_chn);
void emi_tx_burst(RF_MODE_BT_e rf_mode, unsigned char pwr_index, signed char rf_chn);
#ifndef BB_PDZB
#define BB_PDZB (0xd4170300)
#endif
/**
 * @brief   Init the structure of the emi test command and function
 */
test_list_t ate_list[] = {{0x01, emicarrieronly}, {0x02, emi_tx_continue}, {0x03, emirx}, {0x04, emi_tx_burst}};

emi_cmd_t emi_cmd_now = {.g_rssi_val             = 0,
                         .g_tx_packet_cnt        = EMI_TX_PACKET_CNT,
                         .g_run_state            = EMI_RUN_STATE,
                         .g_test_cmd_now         = EMI_TEST_CMD_NOW,
                         .g_power_level          = EMI_POWER_LEVEL,
                         .g_chn                  = EMI_RF_CHN,
                         .g_rf_mode              = EMI_RF_MODE,
                         .g_hop_en               = EMI_HOPPING_EN,
                         .g_pkt_tone_en          = EMI_PKT_TONE_EN,
                         .g_power_scale_slice_en = POWER_SLICE_EN,
                         .g_pkt_type             = EMI_PKT_TYPE};


#if (MCU_CORE_TYPE == MCU_CORE_TL751X)

#define MAX_NOISE_VALUE        -70
#define READ_RSSI_TIMES        100
#define RSSI_COPENSATION_VALUE 0
static signed char    rssi_noise            = -110;
static unsigned short rssi_cnt              = 0;
static unsigned char  g_tlk_ble_rxpara_flag = 1;
extern signed int     s_emi_rssi;
volatile signed char  rssi_temp;

/**
 * @brief      This function is used to get the RSSI value in the ambient noise
 * @return     RSSI values in ambient noise
 */
signed char get_noise_value(void)
{
    signed char   rtn;
    unsigned char ss = reg_rf_dec_err & 0x03;
    if (((reg_rf_ll_cmd & BIT(0)) == 0) && ((ss == 0) || (ss == 1))) {
        if (rssi_cnt >= READ_RSSI_TIMES) {
            rssi_noise = -110;
            rssi_cnt   = 0;
        }
        //        signed char rssi_temp;
        for (int i = 0; i < 50; i++) {
            delay_us(5);
            rssi_temp = rf_get_real_time_rssi();
            if (rssi_temp > rssi_noise) {
                rssi_noise = rssi_temp;
            }
        }

        rssi_cnt++;
    } else if (reg_rf_ll_cmd & BIT(0)) {
        rf_clr_irq_status(FLD_RF_IRQ_RX);
    }

    rtn = rssi_noise;
    return rtn + RSSI_COPENSATION_VALUE;
}

static unsigned char emi_rx_packet[280] __attribute__((aligned(4)));

/**
 * @brief      This function serves to set rx mode and channel
 * @param[in]  none
 * @return     none
 * @note The V1 version can modify RF related configurations through the emi_tx_burst_cfg_t structure
 */
void bt_emi_rx_setup_v1(void)
{
    rf_set_pri_1M_mode();
    rf_set_rx_maxlen(255 - 2); //Rx mode in EMI is manual mode, and only one DMA fifo is used in manual mode.
                               //If multiple DMA fifo are used, it should be noted that rx packet length cannot be greater than the depth of DMA fifo
    rf_set_rx_dma(emi_rx_packet, 0, 253);
    rf_pn_disable();
    rf_set_chn(emi_cmd_now.g_chn); //set freq
    rf_set_tx_rx_off();
    rf_set_rxmode();
    delay_us(150);
    if (g_tlk_ble_rxpara_flag == 1) {
#if (0)
        rf_set_rxpara();
#endif
        g_tlk_ble_rxpara_flag = 0;
    }

    if (emi_cmd_now.g_chn == 24 || emi_cmd_now.g_chn == 48 || emi_cmd_now.g_chn == 72) {
#if (0)
        rf_ldot_ldo_rxtxlf_bypass_en();
#endif
    } else {
#if (0)
        rf_ldot_ldo_rxtxlf_bypass_dis();
#endif
    }
}

unsigned char bt_pwr_list[50] = {
    //BR
    39, //       EXT_BTRF_POWER_INDEX_P10dBm
    34, //       EXT_BTRF_POWER_INDEX_P8dBm,
    30, //       EXT_BTRF_POWER_INDEX_P6dBm,
    25, //       EXT_BTRF_POWER_INDEX_P4dBm,
    22, //       EXT_BTRF_POWER_INDEX_P2dBm,
    18, //       EXT_BTRF_POWER_INDEX_00dBm,
    14, //       EXT_BTRF_POWER_INDEX_N2dBm,
    8,  //        EXT_BTRF_POWER_INDEX_N5dBm,
    6,  //        EXT_BTRF_POWER_INDEX_N10dBm,
    0,  //        EXT_BTRF_POWER_INDEX_N25dBm,
    //EDR
    39, //       EXT_BTRF_POWER_INDEX_P8dBm
    34, //       EXT_BTRF_POWER_INDEX_P5dBm,
    30, //       EXT_BTRF_POWER_INDEX_P3dBm,
    25, //       EXT_BTRF_POWER_INDEX_P1dBm,
    22, //       EXT_BTRF_POWER_INDEX_N1dBm,
    18, //       EXT_BTRF_POWER_INDEX_N4dBm,
    14, //       EXT_BTRF_POWER_INDEX_N6dBm,
    8,  //        EXT_BTRF_POWER_INDEX_N8dBm,
    6,  //        EXT_BTRF_POWER_INDEX_N14dBm,
    0,  //        EXT_BTRF_POWER_INDEX_N29dBm,
};
#elif (MCU_CORE_TYPE == MCU_CORE_B92)

unsigned short bt_pwr_list[BT_PWR_LIST_MAX_LENGTH] = {
    //scale << 8 | slice
    //BR
    63, //       EXT_BTRF_TP_POWER_INDEX_MAX_P11dBm
    40, //       EXT_BTRF_TP_POWER_INDEX_P8dBm
    31, //       EXT_BTRF_TP_POWER_INDEX_P6dBm,
    24, //       EXT_BTRF_TP_POWER_INDEX_P4dBm,
    19, //       EXT_BTRF_TP_POWER_INDEX_P2dBm,
    15, //       EXT_BTRF_TP_POWER_INDEX_P0dBm,
    10, //       EXT_BTRF_TP_POWER_INDEX_N3dBm,
    8,  //        EXT_BTRF_TP_POWER_INDEX_N5dBm,
    2,  //        EXT_BTRF_TP_POWER_INDEX_N17dBm,
    1,  //        EXT_BTRF_TP_POWER_INDEX_N22dBm,

    29 << 8 | 49, //         RF_IQ_POWER_INDEX_P6dBm,
    25 << 8 | 49, //       RF_IQ_POWER_INDEX_P5dBm,
    23 << 8 | 49, //       RF_IQ_POWER_INDEX_P4dBm,
    20 << 8 | 49, //       RF_IQ_POWER_INDEX_P3dBm,
    18 << 8 | 49, //       RF_IQ_POWER_INDEX_P2dBm,
    16 << 8 | 49, //       RF_IQ_POWER_INDEX_P1dBm,
    14 << 8 | 49, //       RF_IQ_POWER_INDEX_P0dBm,
    11 << 8 | 49, //       RF_IQ_POWER_INDEX_N2dBm,
    10 << 8 | 33, //       RF_IQ_POWER_INDEX_N5dBm,
    10 << 8 | 29, //       RF_IQ_POWER_INDEX_N6dBm,

    //1.8V
    //BR
    63, //       EXT_BTRF_TP_POWER_INDEX_MAX_P11dBm
    40, //       EXT_BTRF_TP_POWER_INDEX_P8dBm,
    31, //       EXT_BTRF_TP_POWER_INDEX_P6dBm,
    24, //       EXT_BTRF_TP_POWER_INDEX_P4dBm,
    19, //       EXT_BTRF_TP_POWER_INDEX_P2dBm,
    15, //       EXT_BTRF_TP_POWER_INDEX_P0dBm,
    10, //       EXT_BTRF_TP_POWER_INDEX_N3dBm,
    8,  //        EXT_BTRF_TP_POWER_INDEX_N5dBm,
    2,  //        EXT_BTRF_TP_POWER_INDEX_N17dBm,
    1,  //        EXT_BTRF_TP_POWER_INDEX_N22dBm,

    29 << 8 | 49, //         RF_IQ_POWER_INDEX_P6dBm,
    25 << 8 | 49, //       RF_IQ_POWER_INDEX_P5dBm,
    23 << 8 | 49, //       RF_IQ_POWER_INDEX_P4dBm,
    20 << 8 | 49, //       RF_IQ_POWER_INDEX_P3dBm,
    18 << 8 | 49, //       RF_IQ_POWER_INDEX_P2dBm,
    16 << 8 | 49, //       RF_IQ_POWER_INDEX_P1dBm,
    14 << 8 | 49, //       RF_IQ_POWER_INDEX_P0dBm,
    11 << 8 | 49, //       RF_IQ_POWER_INDEX_N2dBm,
    10 << 8 | 33, //       RF_IQ_POWER_INDEX_N5dBm,
    10 << 8 | 29, //       RF_IQ_POWER_INDEX_N6dBm,
};

unsigned short bt_tone_pwr_list[BT_PWR_LIST_MAX_LENGTH] = {
    //scale << 8 | slice
    //BR
    63, //       EXT_BTRF_TP_POWER_INDEX_MAX_P11dBm
    40, //       EXT_BTRF_TP_POWER_INDEX_P8dBm,
    31, //       EXT_BTRF_TP_POWER_INDEX_P6dBm,
    24, //       EXT_BTRF_TP_POWER_INDEX_P4dBm,
    19, //       EXT_BTRF_TP_POWER_INDEX_P2dBm,
    15, //       EXT_BTRF_TP_POWER_INDEX_P0dBm,
    10, //       EXT_BTRF_TP_POWER_INDEX_N3dBm,
    8,  //        EXT_BTRF_TP_POWER_INDEX_N5dBm,
    2,  //        EXT_BTRF_TP_POWER_INDEX_N17dBm,
    1,  //        EXT_BTRF_TP_POWER_INDEX_N22dBm,

    36,          //                RF_IQ_POWER_INDEX_P6dBm,
    32,          //                RF_IQ_POWER_INDEX_P5dBm,
    BIT(7) | 55, //       RF_IQ_POWER_INDEX_P4dBm,
    BIT(7) | 45, //        RF_IQ_POWER_INDEX_P3dBm,
    BIT(7) | 38, //       RF_IQ_POWER_INDEX_P2dBm,
    BIT(7) | 33, //       RF_IQ_POWER_INDEX_P1dBm,
    BIT(7) | 28, //       RF_IQ_POWER_INDEX_P0dBm,
    BIT(7) | 23, //       RF_IQ_POWER_INDEX_N2dBm,
    BIT(7) | 17, //       RF_IQ_POWER_INDEX_N5dBm,
    BIT(7) | 13, //       RF_IQ_POWER_INDEX_N6dBm,

    //1.8V
    //BR
    63, //       EXT_BTRF_TP_POWER_INDEX_MAX_P11dBm
    40, //       EXT_BTRF_TP_POWER_INDEX_P8dBm,
    31, //       EXT_BTRF_TP_POWER_INDEX_P6dBm,
    24, //       EXT_BTRF_TP_POWER_INDEX_P4dBm,
    19, //       EXT_BTRF_TP_POWER_INDEX_P2dBm,
    15, //       EXT_BTRF_TP_POWER_INDEX_P0dBm,
    10, //       EXT_BTRF_TP_POWER_INDEX_N3dBm,
    8,  //        EXT_BTRF_TP_POWER_INDEX_N5dBm,
    2,  //        EXT_BTRF_TP_POWER_INDEX_N17dBm,
    1,  //        EXT_BTRF_TP_POWER_INDEX_N22dBm,

    36,          //                RF_IQ_POWER_INDEX_P6dBm,
    32,          //                RF_IQ_POWER_INDEX_P5dBm,
    BIT(7) | 55, //       RF_IQ_POWER_INDEX_P4dBm,
    BIT(7) | 45, //        RF_IQ_POWER_INDEX_P3dBm,
    BIT(7) | 38, //       RF_IQ_POWER_INDEX_P2dBm,
    BIT(7) | 33, //       RF_IQ_POWER_INDEX_P1dBm,
    BIT(7) | 28, //       RF_IQ_POWER_INDEX_P0dBm,
    BIT(7) | 23, //       RF_IQ_POWER_INDEX_N2dBm,
    BIT(7) | 17, //       RF_IQ_POWER_INDEX_N5dBm,
    BIT(7) | 13, //       RF_IQ_POWER_INDEX_N6dBm,
};
#endif

void emi_start()
{
#if SWIRE_THROUGH_USB_EN
#if defined(MCU_CORE_TL751X)
    usbhw_init();
#endif
    usb_set_pin_en();
#endif
    emi_init();
    bt_emi_init();
    /* read flash and configure parameter automatically*/
    tlkhal_flash_init(0);

    extern void calibration_func(void);
    calibration_func();
}

/////////////////////////////////////////////////////

/**
 * @brief		This function serves to EMI Init
 * @return 		none
 */

void emi_init(void)
{
    memcpy((unsigned char *)RSSI_ADDR, &emi_cmd_now, sizeof(emi_cmd_t));
}

extern void rf_emi_reset_baseband(void);
/**
 * @brief		This function serves to EMI CarryOnly
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
volatile u32 AA_test_cnt2 = 0;

void emicarrieronly(RF_MODE_BT_e rf_mode, unsigned char pwr_index, signed char rf_chn)
{
    //    unsigned int pending_tick = clock_time();
    //	pa_operation(PA_SETTING_STATE_TX);
    /* core_140c20 default value 0xc4, BIT(2) is 1, BIT(4) is 0, same as tbl_rf_1m
	 * for BLE: 0xc4, for BT: 0xd0
	 * core_140c21 default value 0x00 */
#if (MCU_CORE_TYPE == MCU_CORE_B92)
    REG_ADDR16(MODEM_MODE_CFG_RX1_0) |= BIT(2);
#endif
    /* default value 0x00
	 * for BLE: 0x00, for BT: 0xff */
    REG_ADDR32(BB_PDZB) = 0;

    rf_emi_stop();
#if (MCU_CORE_TYPE == MCU_CORE_B92)
    dma_reset();
#else
    rf_dma_reset();
#endif
    rf_emi_reset_baseband();
    (void)(rf_mode);
    if (emi_cmd_now.g_hop_en) {
        bt_emi_single_tone(rf_chn, emi_cmd_now.g_slice);
    } else {
        bt_emi_single_tone(rf_chn, bt_pwr_list[pwr_index] & 0xff);
    }
    ////    while( ((read_sram8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state ) || (clock_time_exceed(pending_tick,200)));
    while (((read_sram8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state)) {
        AA_test_cnt2++;
#if (TLK_DEV_SERIAL_ENABLE)
        tlkdrv_serial_handler(UART0);
#endif
    }
    rf_emi_stop();
    bt_emi_stop();
}

/**
 * @brief		This function serves to EMI ConPrbs9
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr 	- power level of RF.
 * @param[in]   rf_chn 	- channel of RF.
 * @return 		none
 */
volatile unsigned int AA_mian_loop;
volatile int          BT_error_0 = 0;

void emi_tx_continue(RF_MODE_BT_e rf_mode, unsigned char pwr_index, signed char rf_chn)
{
    //    pa_operation(PA_SETTING_STATE_TX);

    if (emi_cmd_now.g_hop_en) {
        unsigned int tx_cnt = 0;
#if (CHIP_TYPE == MCU_CORE_B92)
        if (emi_cmd_now.g_power_scale_slice_en) {
            bt_emi_tx_burst_mode_init(rf_mode, rf_chn, emi_cmd_now.g_scale, emi_cmd_now.g_slice, emi_cmd_now.g_pkt_type);
        } else {
            bt_emi_tx_burst_mode_init(rf_mode, rf_chn, bt_pwr_list[pwr_index] >> 8 & 0xff, bt_pwr_list[pwr_index] & 0xff, emi_cmd_now.g_pkt_type);
        }
#else
        if (emi_cmd_now.g_power_scale_slice_en) {
            bt_emi_tx_burst_mode_init(rf_mode, rf_chn, emi_cmd_now.g_slice, emi_cmd_now.g_pkt_type);
        } else {
            bt_emi_tx_burst_mode_init(rf_mode, rf_chn, bt_pwr_list[pwr_index] & 0xff, emi_cmd_now.g_pkt_type);
        }
#endif
        while ((read_sram8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state) {
            bt_emi_tx_burst_mode_loop_hoop(&tx_cnt);
        }
    } else {
#if (CHIP_TYPE == MCU_CORE_B92)
        if (emi_cmd_now.g_power_scale_slice_en) {
            bt_emi_tx_continue_mode_run(rf_mode, rf_chn, emi_cmd_now.g_scale, emi_cmd_now.g_slice, emi_cmd_now.g_pkt_type);
        } else {
            bt_emi_tx_continue_mode_run(rf_mode, rf_chn, bt_pwr_list[pwr_index] >> 8 & 0xff, bt_pwr_list[pwr_index] & 0xff, emi_cmd_now.g_pkt_type);
        }
#else
        if (emi_cmd_now.g_power_scale_slice_en) {
            bt_emi_tx_continue_mode_run(rf_mode, rf_chn, emi_cmd_now.g_slice, emi_cmd_now.g_pkt_type);
        } else {
            bt_emi_tx_continue_mode_run(rf_mode, rf_chn, bt_pwr_list[pwr_index], emi_cmd_now.g_pkt_type);
        }
#endif
        while (((read_sram8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state)) {
#if (TLK_DEV_SERIAL_ENABLE)
            tlkdrv_serial_handler(UART0);
#endif
        }
    }

#if (CHIP_TYPE == MCU_CORE_B92)
    rf_emi_stop();
    dma_reset();
    rf_emi_reset_baseband();
#endif
    bt_emi_stop();
    //    pa_operation(PA_SETTING_STATE_TX);
    //    bt_emi_tx_continue_mode_run(rf_mode,rf_chn,pwr,emi_cmd_now.g_pkt_type);
    //    while( ((read_sram8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state ));
    //    bt_emi_stop();
}

/**
 * @brief		This function serves to EMI Rx
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr	    - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
void emirx(RF_MODE_BT_e rf_mode, unsigned char pwr_index, signed char rf_chn)
{
    //	pa_operation(PA_SETTING_STATE_RX);
    (void)(pwr_index);
    unsigned int rx_cnt = 0;
    bt_emi_rx_mode_init(rf_mode, rf_chn);
    while (((read_sram8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state)) {
        bt_emi_rx_mode_loop(&rx_cnt);
        if (rx_cnt != read_sram32(RX_PACKET_NUM_ADDR)) {
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
            write_sram8(RSSI_ADDR, s_emi_rssi);
#else
            write_sram8(RSSI_ADDR, (unsigned char)bt_get_rssi(0));
#endif
            write_sram32(RX_PACKET_NUM_ADDR, rx_cnt);
        }

#if (TLK_DEV_SERIAL_ENABLE)
        tlkdrv_serial_handler(UART0);
#endif
    }
    bt_emi_stop();
}

/**
 * @brief		This function serves to EMI Tx Burst
 * @param[in]   rf_mode - mode of RF.
 * @param[in]   pwr     - power level of RF.
 * @param[in]   rf_chn  - channel of RF.
 * @return 		none
 */
extern unsigned char emi_tx_flag;

void emi_tx_burst(RF_MODE_BT_e rf_mode, unsigned char pwr_index, signed char rf_chn)
{
    //	pa_operation(PA_SETTING_STATE_TX);
    unsigned int tx_cnt = 0;
#if (CHIP_TYPE == MCU_CORE_B92)
    if (emi_cmd_now.g_power_scale_slice_en) {
        bt_emi_tx_burst_mode_init(rf_mode, rf_chn, emi_cmd_now.g_scale, emi_cmd_now.g_slice, emi_cmd_now.g_pkt_type);
    } else {
        bt_emi_tx_burst_mode_init(rf_mode, rf_chn, bt_pwr_list[pwr_index] >> 8 & 0xff, bt_pwr_list[pwr_index] & 0xff, emi_cmd_now.g_pkt_type);
    }
#else
    if (emi_cmd_now.g_adaptive_en) {
        bt_emi_rx_setup_v1();
    } else {
        if (emi_cmd_now.g_power_scale_slice_en) {
            bt_emi_tx_burst_mode_init(rf_mode, rf_chn, emi_cmd_now.g_slice, emi_cmd_now.g_pkt_type);
        } else {
            bt_emi_tx_burst_mode_init(rf_mode, rf_chn, bt_pwr_list[pwr_index] & 0xff, emi_cmd_now.g_pkt_type);
        }
    }
#endif
    while (((read_sram8(RUN_STATUE_ADDR)) == emi_cmd_now.g_run_state)) {
#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
        if (emi_cmd_now.g_adaptive_en) {
            if (get_noise_value() < MAX_NOISE_VALUE) {
                rf_emi_stop();
                if (emi_cmd_now.g_power_scale_slice_en) {
                    bt_emi_tx_burst_mode_init(rf_mode, rf_chn, emi_cmd_now.g_slice, emi_cmd_now.g_pkt_type);
                } else {
                    bt_emi_tx_burst_mode_init(rf_mode, rf_chn, bt_pwr_list[pwr_index] & 0xff, emi_cmd_now.g_pkt_type);
                }
                while (!emi_tx_flag) {
                    bt_emi_tx_burst_mode_loop(&tx_cnt);
                }
                emi_tx_flag = 0;
                bt_emi_stop();
                rf_set_tx_rx_off_auto_mode();
                REG_ADDR32(BB_PDZB) = 0;
                rf_emi_stop();
                bt_emi_rx_setup_v1();
            }
        } else {
        }
        bt_emi_tx_burst_mode_loop(&tx_cnt);
#else
        bt_emi_tx_burst_mode_loop(&tx_cnt);
#endif

        if (emi_cmd_now.g_tx_packet_cnt) {
            if (tx_cnt >= 1000) {
                break;
            }
        }

#if (TLK_DEV_SERIAL_ENABLE)
        tlkdrv_serial_handler(UART0);
#endif
    }
    bt_emi_stop();
}

void emi_loop_server()
{
    emi_cmd_now.g_run_state = read_sram8(RUN_STATUE_ADDR); // get the run state!
    if (emi_cmd_now.g_run_state != 0) {
        AA_mian_loop++;
        emi_cmd_now.g_power_level          = read_sram8(POWER_ADDR);
        emi_cmd_now.g_chn                  = read_sram8(CHANNEL_ADDR);
        emi_cmd_now.g_rf_mode              = read_sram8(RF_MODE_ADDR);
        emi_cmd_now.g_test_cmd_now         = read_sram8(TEST_COMMAND_ADDR); // get the command!
        emi_cmd_now.g_tx_packet_cnt        = read_sram8(TX_PACKET_MODE_ADDR);
        emi_cmd_now.g_hop_en               = read_sram8(EMI_CONFIG_FLAG_ADDR) & 0x01;
        emi_cmd_now.g_adaptive_en          = (read_sram8(EMI_CONFIG_FLAG_ADDR) & 0x02) >> 1;
        emi_cmd_now.g_pkt_type             = read_sram8(RX_PACKET_TYPE_ADDR);
        emi_cmd_now.g_scale                = read_sram8(TX_POWER_SCALE);
        emi_cmd_now.g_slice                = read_sram8(TX_POWER_SLICE);
        emi_cmd_now.g_power_scale_slice_en = (read_sram8(EMI_CONFIG_FLAG_ADDR) & 0x08) >> 3;

        for (int i = 0; i < 4; i++) {
            if (emi_cmd_now.g_test_cmd_now == ate_list[i].cmd_id) {
                switch (emi_cmd_now.g_rf_mode) {
                case BR_DH1:
                    ate_list[i].func(BT_MODE_BR_DH1, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;
                case EDR2_DH1:
                    ate_list[i].func(BT_MODE_EDR2_DH1, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;
                case EDR3_DH1:
                    ate_list[i].func(BT_MODE_EDR3_DH1, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;
                case BR_DH3:
                    ate_list[i].func(BT_MODE_BR_DH3, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;
                case EDR2_DH3:
                    ate_list[i].func(BT_MODE_EDR2_DH3, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;
                case EDR3_DH3:
                    ate_list[i].func(BT_MODE_EDR3_DH3, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;
                case BR_DH5:
                    ate_list[i].func(BT_MODE_BR_DH5, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;
                case EDR2_DH5:
                    ate_list[i].func(BT_MODE_EDR2_DH5, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;
                case EDR3_DH5:
                    ate_list[i].func(BT_MODE_EDR3_DH5, emi_cmd_now.g_power_level, emi_cmd_now.g_chn);
                    break;

                default:
                    break;
                }
            }
        }
        emi_cmd_now.g_run_state = 0;
        write_sram8(RUN_STATUE_ADDR, emi_cmd_now.g_run_state);
    }
}

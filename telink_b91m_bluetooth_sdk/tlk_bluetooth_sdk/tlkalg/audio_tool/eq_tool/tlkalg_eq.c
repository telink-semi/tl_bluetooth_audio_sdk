/********************************************************************************************************
 * @file    tlkalg_eq.c
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
/*
 * tlkalg_eq.c
 *
 *  Created on: 2024年12月31日
 *      Author: Admin
 */
#include "tl_common.h"
#include "tlkalg/audio_tool/eq_tool/tlkalg_eq.h"

#ifndef TLKALG_EQ_TOOL_ENABLE
#define TLKALG_EQ_TOOL_ENABLE 0
#endif

#if TLKALG_EQ_TOOL_ENABLE && TLKALG_EQ_ENABLE
#include "tlklib/dbg/tlkdbg.h"
#include "tlklib/usb/udb/tlkusb_udb.h"

#define EQ_FRAME_LEN        16
#define EQ_FRAME_HEADER_LEN 7

eq_mode_t              eq_tool_para;
e_eq_cmd_type_e        eq_cmd_type;
volatile unsigned char eq_sync_buff[84];
volatile uint8_t       eq_sync_cmd;

uint8_t eq_package_num = 0;
int     eq_len_header  = 0;
int     eq_len_cmd     = 0;
int     eq_cmd_index   = 0;
uint8_t eq_sync_flag   = 0;
uint8_t eq_enable_flag = 0;
uint8_t eq_param_data[500];

extern void tlkalg_tool_para(eq_mode_t *para);

/**
 * @brief   Callback function to handle USB commands received from the application.
 * @param[in]   p       - Pointer to the command data buffer.
 * @param[in]   len     - Length of the command data.
 * @return      0       - Always returns 0.
 */
int app_udb_cmd_cb(uint8_t *p, uint16_t len)
{
    tlk_printf("len: %d\n", len);
    for (uint8_t i = 0; i < len; i++) {
        tlk_printf("data: %x\n", *(p + i));
    }
    return 0;
}

/**
 * @brief   Synchronize equalizer parameters from a buffer.
 * @param[in]   buff    - Pointer to the buffer containing equalizer parameters.
 */
void tlkalg_eq_sync_para(uint8_t *buff)
{
    unsigned char p_eq_sync_res[16] = {0xc7, 0xc7, 0xc7};

    for (unsigned int i = 0; i < sizeof(eq_tool_para); i++) {
        eq_sync_buff[i] = buff[i];
    }

    if (eq_sync_cmd == 0xcc) { //usb_cmd
        p_eq_sync_res[6] = eq_sync_buff[2];
        p_eq_sync_res[7] = eq_sync_buff[3];
        tlkdbg_send_status(0xc7, 0x07, p_eq_sync_res, 14);
    }
}

/**
 * @brief   Load equalizer parameters from a buffer.
 * @param[in]   para    - Pointer to the buffer containing equalizer parameters.
 */
void tlkalg_eq_load_para(uint8_t *para)
{
    tlkalg_tool_para((eq_mode_t *)para);
}

volatile uint8_t p_eq_actual_stages = 0;

/**
 * @brief   Allocate and set equalizer parameters based on the command data.
 * @param[in]   ps      - Pointer to the command data buffer.
 * @param[in]   len     - Length of the command data.
 */
void tlkalg_eq_allocate_parameter(unsigned char *ps, int len)
{
    (void)len;

    uint8_t eq_actual_stages_index = 0;
    uint8_t p_eq_index             = 0;
    uint8_t eq_stages              = 0;
    uint8_t p_eq_stages            = 0;
    //	uint8_t p_eq_actual_stages = 0;
    uint8_t p_eq_is_first_config = 0;
    uint8_t p_eq_mode            = 0;
    uint8_t cmd_type             = 0;
    uint8_t p_group_index        = 0;
    uint8_t p_group_is_enable    = 0;

    unsigned char eq_sync_res[16] = {0xc7, 0x0b, 0x00, 0x02, 0x33};
    //	unsigned char eq_sync_uart_res[16] = {0xc7, 0xc7, 0xc7, 0x0b, 0x00, 0x02, 0x33};

    cmd_type    = ps[6];
    eq_cmd_type = (cmd_type == 0x30) ? EQ_CMD_DISABLE_EQ : ((cmd_type == 0x31) ? EQ_CMD_ENABLE_EQ : ((cmd_type == 0x32) ? EQ_CMD_DOWNLOAD : EQ_CMD_SYNC));

    //	eq_tool_para.CRC = ps[0] << 24 | ps[1] << 16 | ps[2] << 8 | eq_cmd_type;

    switch (eq_cmd_type) {
    case EQ_CMD_DISABLE_EQ:
    {
        p_group_index    = ps[7];
        eq_tool_para.CRC = ps[0] << 24 | ps[1] << 16 | p_group_index << 8 | eq_cmd_type;

        tlkalg_eq_load_para((uint8_t *)&eq_tool_para);
        tlk_printf("EQ_CMD_DISABLE_EQ");
        break;
    }
    case EQ_CMD_ENABLE_EQ:
    {
        p_group_index    = ps[7];
        eq_tool_para.CRC = ps[0] << 24 | ps[1] << 16 | p_group_index << 8 | eq_cmd_type;

        tlkalg_eq_load_para((uint8_t *)&eq_tool_para);
        tlk_printf("EQ_CMD_ENABLE_EQ");
        break;
    }
    case EQ_CMD_DOWNLOAD:
    {
        p_eq_stages          = ps[6 + EQ_FRAME_LEN] & 0x0f;
        p_eq_actual_stages   = ps[8 + EQ_FRAME_LEN];
        p_eq_mode            = ps[9 + EQ_FRAME_LEN];
        p_group_index        = ps[EQ_FRAME_LEN - 1];
        p_eq_is_first_config = ps[EQ_FRAME_LEN - 1];
        p_group_index        = ps[7];
        eq_tool_para.CRC     = ps[0] << 24 | ps[1] << 16 | p_group_index << 8 | eq_cmd_type;

        eq_tool_para.eq_header.eq_mode      = (p_eq_mode == 1) ? EQ_HANDLE_MUSIC_L : ((p_eq_mode == 2) ? EQ_HANDLE_MIC : EQ_HANDLE_SPK);
        eq_tool_para.eq_header.filter_sum   = p_eq_stages;
        eq_tool_para.eq_header.all_gain     = ps[10 + EQ_FRAME_LEN];
        eq_tool_para.eq_header.max_gain_num = (eq_tool_para.eq_header.eq_mode == EQ_HANDLE_MUSIC_L) ? TLKALG_EQ_MAX_FILTER_STAGE : 4;

        eq_stages = (p_eq_is_first_config == 0x30) ? p_eq_stages : p_eq_actual_stages;

        if (eq_stages > 0) {
            p_eq_index = EQ_FRAME_LEN * 2 + EQ_FRAME_HEADER_LEN;

            for (int i = 0; i < eq_stages; i++) {
                p_group_is_enable                                              = ps[p_eq_index + 8];
                eq_actual_stages_index                                         = ps[p_eq_index];
                eq_tool_para.eq_para[eq_actual_stages_index - 1].filter_enable = p_group_is_enable;
                eq_tool_para.eq_para[eq_actual_stages_index - 1].freq_c        = ps[p_eq_index + 4] << 8 | ps[p_eq_index + 3];
                eq_tool_para.eq_para[eq_actual_stages_index - 1].filter_q      = ps[p_eq_index + 6] << 8 | ps[p_eq_index + 5];
                eq_tool_para.eq_para[eq_actual_stages_index - 1].filter_type   = ps[p_eq_index + 7];
                if (p_group_is_enable == 1) {
                    eq_tool_para.eq_para[eq_actual_stages_index - 1].gain = ps[p_eq_index + 2] << 8 | ps[p_eq_index + 1];
                } else {
                    eq_tool_para.eq_para[eq_actual_stages_index - 1].gain = 0;
                }
                p_eq_index += EQ_FRAME_LEN;

                //				tlk_printf("eq_tool_para: %02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x",
                //						eq_tool_para.eq_para[eq_actual_stages_index - 1].gain & 0xFF,
                //						(eq_tool_para.eq_para[eq_actual_stages_index - 1].gain >> 8) & 0xFF,
                //						eq_tool_para.eq_para[eq_actual_stages_index - 1].freq_c & 0xFF,
                //						(eq_tool_para.eq_para[eq_actual_stages_index - 1].freq_c >> 8)& 0xFF,
                //						eq_tool_para.eq_para[eq_actual_stages_index - 1].filter_q & 0xFF,
                //						(eq_tool_para.eq_para[eq_actual_stages_index - 1].filter_q >> 8)& 0xFF,
                //						eq_tool_para.eq_para[eq_actual_stages_index - 1].filter_type & 0xFF,
                //						(eq_tool_para.eq_para[eq_actual_stages_index - 1].filter_type >> 8) & 0xFF);
            }
        }
        tlkalg_eq_load_para((uint8_t *)&eq_tool_para);
        tlk_printf("EQ_CMD_DOWNLOAD");
        break;
    }
    case EQ_CMD_SYNC:
    {
        p_eq_stages = ps[8];
        eq_sync_cmd = ps[EQ_FRAME_LEN - 1];

        p_group_index    = ps[9];
        eq_tool_para.CRC = ps[0] << 24 | ps[1] << 16 | p_group_index << 8 | eq_cmd_type;

        if (p_eq_stages == 0x00) { //Get EQ Stages
            tlkalg_eq_load_para((uint8_t *)&eq_tool_para);
        } else {
            if (eq_sync_cmd == 0xcc) { //usb_cmd
                eq_sync_res[3] = p_eq_stages;
                for (int i = 0; i < 8; i++) {
                    eq_sync_res[4 + i] = eq_sync_buff[i + 8 * p_eq_stages];
                }
                tlkdbg_send_status(0xc7, 0xc7, eq_sync_res, 14);
            }
        }

        tlk_printf("EQ_CMD_SYNC");
        break;
    }
    }
}

/**
 * @brief   Handle equalizer parameter debug commands.
 * @param[in]   p_data  - Pointer to the command data buffer.
 * @param[in]   data_len- Length of the command data.
 */
void app_eq_para_debug_handler(uint8_t *p_data, uint16_t data_len)
{
    tlk_printf("app_eq_para_debug_handler");
    tlkalg_eq_allocate_parameter(p_data, data_len);
}

/**
 * @brief   Hook function to process equalizer configuration commands received via USB.
 * @param[in]   pData   - Pointer to the command data buffer.
 * @param[in]   dataLen - Length of the command data.
 */
void tlkusb_eq_cfg_hook(uint8_t *pData, uint16_t dataLen)
{
    tlk_printf("pData[0-8]:%x, %x, %x, %x, %x, %x, %x, %x, %x", pData[0], pData[1], pData[2], pData[3], pData[4], pData[5], pData[6], pData[7], pData[8]);
    unsigned char p_eq_res[14] = {0};
    eq_package_num++;

    if (pData[6] == 0x33) {
        eq_sync_flag = 1;
        pData[15]    = 0xcc; //usb cmd
        if (eq_enable_flag == 1) {
            p_eq_res[0] = 0xc7;
            tlkdbg_send_status(0xc7, 0xc7, p_eq_res, 14);
        }
    } else {
        p_eq_res[0] = 0xc7;
        if (pData[6] == 0x32) {
            if (eq_cmd_index == 0) {
                p_eq_res[3] = 0x30;
            } else {
                p_eq_res[3] = 0x31;
            }
            pData[15] = p_eq_res[3]; //here send the information abort whether is the first config eq para to app layer
        } else {
            eq_enable_flag = 1;
        }

        tlkdbg_send_status(0xc7, 0xc7, p_eq_res, 14);

        if (eq_package_num == 2) {
            eq_len_header = pData[8] & 0x0f; //filter stage
        }
    }
    tmemcpy(&eq_param_data[eq_len_cmd], &pData[0], dataLen);
    eq_len_cmd += dataLen;

    if ((eq_package_num == eq_len_header + 2) || eq_sync_flag == 1) {
        app_eq_para_debug_handler(eq_param_data, eq_len_cmd);
        eq_package_num = 0;
        eq_len_cmd     = 0;
        eq_len_header  = 0;
        if (eq_param_data[6] == 0x32) {
            eq_cmd_index++;
        }
        eq_sync_flag   = 0;
        eq_enable_flag = 0;
        tmemset(&eq_param_data, 0, sizeof(eq_param_data));
    }
}
#endif

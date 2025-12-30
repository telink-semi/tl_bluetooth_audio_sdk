/********************************************************************************************************
 * @file    tlka_vad_api.h
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
#ifndef TLKA_VAD_API_H
#define TLKA_VAD_API_H

#ifndef MY_VAD_RAMCODE
#define MY_VAD_RAMCODE
#endif

#define VAD_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define VAD_VERSION VAD_VERSION_INT(0, 1, 4)

#define VAD_DEBUG 0  //only for debug
#define VAD_MAXWINSIZE 240

#if __riscv
#define VAD_RAM_CODE __attribute__((section(".ram_code")))
#endif

//typedef short short;
//typedef long long int int64_t;

/* data struct */
typedef struct TLKA_VAD_PARAM
{
	short vad_mod;
	short frm_ms;
    int frm_len;
    int pwr_longterm;
    int win_idx;
    int smp_idx;

    short pwr_diffthd_linear;
    int pwr_shortterm_thd;
    short pwr_longterm_alpha;
    short pwr_winsize;

    int vad_pwrwin[VAD_MAXWINSIZE];

    short vad_fast_en;
#if VAD_DEBUG
    short *dbg_ptr;
#endif
} tlka_vad_param;

int tlka_vad_get_version(void);
int tlka_vad_get_size(void);
int tlka_vad_init(void *state, int fs, int VAD_FAST_VER);

int tlka_vad_process_frame(void *state, short *ps, int vad_en);

#endif

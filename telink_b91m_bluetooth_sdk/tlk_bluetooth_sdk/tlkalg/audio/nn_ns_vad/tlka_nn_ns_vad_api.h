/********************************************************************************************************
 * @file    tlka_nn_ns_vad_api.h
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
#ifndef TLKA_NN_NS_API_H
#define TLKA_NN_NS_API_H

// #define RNN_FLOAT

/*! Version number to ensure header and binary are matching. */
#define NN_NS_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define NN_NS_VERSION                          NN_NS_VERSION_INT(1, 2, 0)

/* error code */
#define NN_NS_OK                (0)
#define NN_NS_INIT_ERR          (-1)
#define NN_NS_INIT_FFT_SIZE     (-2)
#define NN_NS_INIT_TARGET_LEVEL (-3)
#define NN_NS_INIT_VAD_LEVEL    (-4)
#define NN_NS_ERR_ENC1          (-5)
#define NN_NS_ERR_INT_BIT       (-6)
#define NN_NS_ERR_INVALID_PARA  (-7)
#define NN_NS_ERR_TARGET_LEVEL  (-8)

/* parameters setting */
#define NN_NS_SET_TARGET_LEVEL (1000)

typedef struct nn_ns_para
{
    int n_fft;
    int n_framesize;
    int n_target_level;
    int n_vad_level;
} NN_NS_PARA_STRU;

enum nn_target_level
{
    target_10dB,
    target_11dB,
    target_12dB,
    target_13dB,
    target_14dB,
    target_15dB,
    target_16dB,
    target_17dB,
    target_18dB,
    target_19dB,
    target_20dB,
    target_21dB,
    target_22dB,
    target_25dB,
    target_30dB,
    target_unlimited,
    target_end
};

/**
 * The VAD detection provides three levels: sensitive, balanced, and selective.
 * From sensitive to balanced to selective, the detection threshold becomes stricter,
 * resulting in increased sensitivity to speech activity.
 */
enum nn_vad_level
{
    //	sensitive,
    //	balanced,
    //	selective,
    vad_056,
    vad_058,
    vad_060,
    vad_062,
    vad_064,
    vad_066,
    vad_068,
    vad_070,
    vad_072,
    vad_074,
    vad_076,
    vad_078,
    vad_080,
    vad_082,
    vad_084,
    vad_086,
    vad_level_end
};

typedef struct nn_ns NN_NS_STRU;

/**
 * @brief return the version of NN_NS module, it should be match with NN_NS_VERSION defined in this header file
 *
 */
int tlka_nn_ns_get_version(void);

/**
 * @brief return the size of NN_NS_STRU
 *
 */
int tlka_nn_ns_get_size();

/**
 * @brief return the size of scratch buffer size
 *
 * */
int tlka_nn_ns_get_scratch_buf_size();

/**
 * @brief NN_NS init function
 *
 * @param st        [m] NN_NS handle, it should be alloced before calling tlka_nn_ns_init.
 *                      The required memory size is returned by calling tlka_nn_ns_get_size
 * @param para      [i] NN_NS config structure, see NN_NS_PARA_STRU for detail
 * @return int      NN_NS_OK: successful
 *                  others: failed
 */
int tlka_nn_ns_init(NN_NS_STRU *st, NN_NS_PARA_STRU *para);

/**
 * @brief process a frame of data for 16bit input
 *
 * @param st        [m] NN_NS handle
 * @param buffer    [m] input & output buffer
 * @param vad_out   [m] vad value, 0 or 1
 * @return int      NN_NS_OK: successful
 *                  others: failed
 */
int tlka_nn_ns_process_frame(NN_NS_STRU *st, short *buffer, int *vad_out, void *scratch_buf);

/**
 * @brief process a frame of data for 24bit input
 *
 * @param st        [m] NN_NS handle
 * @param buffer    [m] input & output buffer
 * @param vad_out   [m] vad value, 0 or 1
 * @return int      NN_NS_OK: successful
 *                  others: failed
 */
int tlka_nn_ns_process_frame_24(NN_NS_STRU *st, int *buffer, int *vad_out, void *scratch_buf);

/**
 * @brief this function is only for debug perpose
 *
 */
int tlka_nn_ns_free(NN_NS_STRU *st);

/**
 * @brief set real time properties
 *
 * @param st        [m] NN_NS handle
 * @param request   [m] property to be modified, support NN_NS_SET_TARGET_LEVEL currently
 * @param val       [i] pointer to a parameter value
 * @return int      NN_NS_OK: successful
 *                  others: failed
 */
int tlka_nn_ns_set_property(NN_NS_STRU *st, int request, void *val);

#endif

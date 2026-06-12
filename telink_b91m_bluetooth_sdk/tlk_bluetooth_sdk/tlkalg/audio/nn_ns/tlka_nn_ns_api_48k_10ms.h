/********************************************************************************************************
 * @file    tlka_nn_ns_api_48k_10ms.h
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
#if (TLKALG_NN_NS_TYPE == TLKALG_NN_NS_48K_10MS)

#ifndef TLKA_NN_NS_API_H
#define TLKA_NN_NS_API_H

// #define RNN_FLOAT

/*! Version number to ensure header and binary are matching. */
#define NN_NS_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define NN_NS_VERSION                          NN_NS_VERSION_INT(3, 1, 0)

/* error code */
#define NN_NS_OK                (0)
#define NN_NS_INIT_ERR          (-1)
#define NN_NS_INIT_FFT_SIZE     (-2)
#define NN_NS_INIT_TARGET_LEVEL (-3)
#define NN_NS_ERR_ENC1          (-4)
#define NN_NS_ERR_ENC2          (-5)
#define NN_NS_ERR_ENC3          (-6)
#define NN_NS_ERR_ENC4          (-7)
#define NN_NS_ERR_INT_BIT       (-8)
#define NN_NS_ERR_INVALID_PARA  (-9)
#define NN_NS_ERR_TARGET_LEVEL  (-10)

/* parameters setting */
#define NN_NS_SET_TARGET_LEVEL (1000)

typedef struct nn_ns_para
{
    int n_fft;
    int n_framesize;
    int n_target_level;
} NN_NS_PARA_STRU;

enum nn_target_level
{
    target_0dB,
    target_1dB,
    target_2dB,
    target_3dB,
    target_4dB,
    target_5dB,
    target_6dB,
    target_7dB,
    target_8dB,
    target_9dB,
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
    target_23dB,
    target_24dB,
    target_25dB,
    target_26dB,
    target_27dB,
    target_28dB,
    target_29dB,
    target_30dB,
    target_unlimited,
    target_end
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
 * @return int      NN_NS_OK: successful
 *                  others: failed
 */
int tlka_nn_ns_process_frame(NN_NS_STRU *st, short *buffer);

/**
 * @brief process a frame of data for 24bit input
 *
 * @param st        [m] NN_NS handle
 * @param buffer    [m] input & output buffer
 * @return int      NN_NS_OK: successful
 *                  others: failed
 */
int tlka_nn_ns_process_frame_24(NN_NS_STRU *st, int *buffer);

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
#endif
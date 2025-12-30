/********************************************************************************************************
 * @file    tlkalg_asrc_interface.c
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
#include "tlkapi/tlkapi.h"
#include "tlka_asrc_api.h"
#include "tlkalg_asrc_interface.h"

#if (TLKALG_ASRC_48TO16_ENABLE | TLKALG_ASRC_16TO48_ENABLE | TLKALG_ASRC_441TO48_ENABLE | TLKALG_ASRC_441TO16_ENABLE)

uint8_t *g_asrc_48to16_buff_ptr  = NULL;
uint8_t *g_asrc_16to48_buff_ptr  = NULL;
uint8_t *g_asrc_48to441_buff_ptr = NULL;
uint8_t *g_asrc_441to48_buff_ptr = NULL;
uint8_t *g_asrc_441to16_buff_ptr = NULL;

/**
 * @brief       This function changes the input channel to the appropriate channel format for ASRC.
 * @param[in]   channel_in    - the input channel number.
 * @return      The output channel format or -1 if the input channel is invalid.
 */
static int8_t tlkalg_asrc_channel_change(uint8_t channel_in)
{
    int8_t chnl_out;
    switch (channel_in) {
    case 1:
    case 2:
        chnl_out = TLKA_ASRC_SINGLE;
        break;
    case 3:
        chnl_out = TLKA_ASRC_STEREO;
        break;

    default:
        chnl_out = -1;
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc error channel %d", channel_in);
        break;
    }

    return chnl_out;
}

/**
 * @brief       This function calculates the required buffer size for ASRC 48kHz to 16kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_48to16_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_get_size(TLKA_ASRC_48_TO_16, chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 size %d", size);
    return size;
}

/**
 * @brief       This function initializes ASRC for 48kHz to 16kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t tlkalg_asrc_48to16_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_48to16_buff_ptr = p_buff;
    int ret                = tlka_asrc_init(g_asrc_48to16_buff_ptr, TLKA_ASRC_48_TO_16, chnl);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 init error");
    }

    return ret;
}

/**
 * @brief       This function deinitializes ASRC for 48kHz to 16kHz conversion.
 * @return      Always returns 0.
 */
int8_t tlkalg_asrc_48to16_deinit(void)
{
    g_asrc_48to16_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_48to16_deinit");

    return 0;
}

/**
 * @brief       This function processes audio data for ASRC 48kHz to 16kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int tlkalg_asrc_48to16_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to16 input buff point null");
        return 0;
    }

    if (g_asrc_48to16_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to16 struct point null");
        return 0;
    }

    int len_out = tlka_asrc_process_frame(g_asrc_48to16_buff_ptr, (short *)ps, len, (short *)pd);

    return len_out;
}

/**
 * @brief       This function calculates the required buffer size for ASRC 16kHz to 48kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_16to48_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_get_size(TLKA_ASRC_16_TO_48, chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 size %d", size);
    return size;
}

/**
 * @brief       This function initializes ASRC for 16kHz to 48kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t tlkalg_asrc_16to48_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_16to48_buff_ptr = p_buff;
    int ret                = tlka_asrc_init(g_asrc_16to48_buff_ptr, TLKA_ASRC_16_TO_48, chnl);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 init error");
    }

    return ret;
}

/**
 * @brief       This function deinitializes ASRC for 16kHz to 48kHz conversion.
 * @return      Always returns 0.
 */
int8_t tlkalg_asrc_16to48_deinit(void)
{
    g_asrc_16to48_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_16to48_deinit");

    return 0;
}

/**
 * @brief       This function processes audio data for ASRC 16kHz to 48kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int tlkalg_asrc_16to48_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to48 input buff point null");
        return 0;
    }

    if (g_asrc_16to48_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to48 struct point null");
        return 0;
    }

    int len_out = tlka_asrc_process_frame(g_asrc_16to48_buff_ptr, (short *)ps, len, (short *)pd);

    return len_out;
}

/**
 * @brief       This function calculates the required buffer size for ASRC 48kHz to 44.1kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_48to441_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_get_size(TLKA_ASRC_48_TO_44, chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to441 size %d", size);
    return size;
}

/**
 * @brief       This function initializes ASRC for 48kHz to 44.1kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t tlkalg_asrc_48to441_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to441 error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_48to441_buff_ptr = p_buff;
    int ret                 = tlka_asrc_init(g_asrc_48to441_buff_ptr, TLKA_ASRC_48_TO_44, chnl);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to441 init error");
    }

    return ret;
}

/**
 * @brief       This function deinitializes ASRC for 48kHz to 44.1kHz conversion.
 * @return      Always returns 0.
 */
int8_t tlkalg_asrc_48to441_deinit(void)
{
    g_asrc_48to441_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_48to441_deinit");

    return 0;
}

/**
 * @brief       This function processes audio data for ASRC 48kHz to 44.1kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int tlkalg_asrc_48to441_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to441 input buff point null");
        return 0;
    }

    if (g_asrc_48to441_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to441 struct point null");
        return 0;
    }

    int len_out = tlka_asrc_process_frame(g_asrc_48to441_buff_ptr, (short *)ps, len, (short *)pd);

    return len_out;
}

/**
 * @brief       This function calculates the required buffer size for ASRC 44.1kHz to 48kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_441to48_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_get_size(TLKA_ASRC_44_TO_48, chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 size %d", size);
    return size;
}

/**
 * @brief       This function initializes ASRC for 44.1kHz to 48kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t tlkalg_asrc_441to48_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_441to48_buff_ptr = p_buff;
    int ret                 = tlka_asrc_init(g_asrc_441to48_buff_ptr, TLKA_ASRC_44_TO_48, chnl);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 init error");
    }

    return ret;
}

/**
 * @brief       This function deinitializes ASRC for 44.1kHz to 48kHz conversion.
 * @return      Always returns 0.
 */
int8_t tlkalg_asrc_441to48_deinit(void)
{
    g_asrc_441to48_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_441to48_deinit");

    return 0;
}

/**
 * @brief       This function processes audio data for ASRC 44.1kHz to 48kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int tlkalg_asrc_441to48_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to48 input buff point null");
        return 0;
    }

    if (g_asrc_441to48_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to48 struct point null");
        return 0;
    }

    int len_out = tlka_asrc_process_frame(g_asrc_441to48_buff_ptr, (short *)ps, len, (short *)pd);

    return len_out;
}

/**
 * @brief       This function calculates the required buffer size for ASRC 44.1kHz to 16kHz conversion.
 * @param[in]   channel    - the number of channels.
 * @return      The required buffer size or 0 if the channel is invalid.
 */
uint16_t tlkalg_asrc_441to16_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_get_size(TLKA_ASRC_44_TO_16, chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to16 size %d", size);
    return size;
}

/**
 * @brief       This function initializes ASRC for 44.1kHz to 16kHz conversion.
 * @param[in]   p_buff    - the buffer pointer.
 * @param[in]   channel   - the number of channels.
 * @return      The result of the initialization, 0 on failure.
 */
int8_t tlkalg_asrc_441to16_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to16 error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_441to16_buff_ptr = p_buff;
    int ret                 = tlka_asrc_init(g_asrc_441to16_buff_ptr, TLKA_ASRC_44_TO_16, chnl);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to16 init error");
    }

    return ret;
}

/**
 * @brief       This function deinitializes ASRC for 44.1kHz to 16kHz conversion.
 * @return      Always returns 0.
 */
int8_t tlkalg_asrc_441to16_deinit(void)
{
    g_asrc_441to16_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_441to16_deinit");

    return 0;
}

/**
 * @brief       This function processes audio data for ASRC 44.1kHz to 16kHz conversion.
 * @param[in]   ps        - the source buffer pointer.
 * @param[out]  pd        - the destination buffer pointer.
 * @param[in]   len       - the length of the audio data in samples.
 * @param[in]   width     - the width of each audio sample (unused).
 * @param[in]   channel   - the number of channels (unused).
 * @return      The length of the processed audio data in samples.
 */
int tlkalg_asrc_441to16_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to16 input buff point null");
        return 0;
    }

    if (g_asrc_441to16_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to16 struct point null");
        return 0;
    }

    int len_out = tlka_asrc_process_frame(g_asrc_441to16_buff_ptr, (short *)ps, len, (short *)pd);

    return len_out;
}

#endif

/********************************************************************************************************
 * @file    tlkalg_asrc_24bit_interface.c
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
#include "tlkalg_asrc_24bit_interface.h"

#if 1//(TLKALG_ASRC_16TO48_24BIT_ENABLE)

tlka_asrc_24_bit_hp_param *g_asrc_16to48_24bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_16to48_16bit_buff_ptr  = NULL;
tlka_asrc_24_bit_hp_param *g_asrc_48to16_24bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_48to16_16bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_441to48_16bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_441to48_16bit_buff_ptr_backup  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_441to16_16bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_16to441_16bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_48to32_16bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_32to48_16bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_asrc_32to16_16bit_buff_ptr  = NULL;

tlka_asrc_16_bit_hp_param *g_ppm_spk_16bit_buff_ptr  = NULL;
tlka_asrc_16_bit_hp_param *g_ppm_mic_16bit_buff_ptr  = NULL;
tlka_asrc_24_bit_hp_param *g_ppm_spk_24bit_buff_ptr  = NULL;
tlka_asrc_24_bit_hp_param *g_ppm_mic_24bit_buff_ptr  = NULL;

uint8_t g_tlkalg_ppm_spk_16bit_chn = 0;
uint8_t g_tlkalg_ppm_mic_16bit_chn = 0;
uint8_t g_tlkalg_ppm_spk_24bit_chn = 0;
uint8_t g_tlkalg_ppm_mic_24bit_chn = 0;

/**
 * @brief       Change ASRC channel configuration
 * @param[in]   channel_in - Input channel configuration
 * @return      Output channel configuration
 */
static int8_t tlkalg_asrc_24bit_channel_change(uint8_t channel_in)
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


/***************************************16to48 24bit*********************************************/
/**
 * @brief       Get the size required for 16to48 24bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_16to48_24bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_24_bit_hp_get_size(chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 24bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 16to48 24bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to48_24bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 24bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_16to48_24bit_buff_ptr = (tlka_asrc_24_bit_hp_param *)p_buff;
    g_asrc_16to48_24bit_buff_ptr->config_rst = 1;
    g_asrc_16to48_24bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_24_bit_hp_init(g_asrc_16to48_24bit_buff_ptr, TLKA_ASRC_16_TO_48, chnl, 0);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 24bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 16to48 24bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to48_24bit_deinit(void)
{
    g_asrc_16to48_24bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_16to48_24bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 16to48 24bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_16to48_24bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to48 24bit input buff point null");
        return 0;
    }

    if (g_asrc_16to48_24bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to48 24bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to48 24bit data len over");
        return 0;
    }

    int len_out = tlka_asrc_24_bit_process_frame_hp(g_asrc_16to48_24bit_buff_ptr, (int *)ps, len, (int *)pd);
    // len_out = 480;
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "return len %d", len_out);

    return len_out;
}

/***************************************16to48 16bit*********************************************/
/**
 * @brief       Get the size required for 16to48 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_16to48_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }
    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_16_TO_48, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 16to48 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to48_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_16to48_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_16to48_16bit_buff_ptr->config_rst = 1;
    g_asrc_16to48_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_16to48_16bit_buff_ptr, TLKA_ASRC_16_TO_48, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init(g_asrc_16to48_16bit_buff_ptr, TLKA_ASRC_16_TO_48, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to48 16bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 16to48 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to48_16bit_deinit(void)
{
    g_asrc_16to48_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_16to48_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 16to48 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_16to48_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to48 16bit input buff point null");
        return 0;
    }

    if (g_asrc_16to48_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to48 16bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to48 16bit data len over");
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_16to48_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame(g_asrc_16to48_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #endif

    return len_out;
}


/***************************************48to16 24bit*********************************************/
/**
 * @brief       Get the size required for 48to16 24bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_48to16_24bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_24_bit_hp_get_size(chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 24bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 48to16 24bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to16_24bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 24bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_48to16_24bit_buff_ptr = (tlka_asrc_24_bit_hp_param *)p_buff;
    g_asrc_48to16_24bit_buff_ptr->config_rst = 1;
    g_asrc_48to16_24bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_24_bit_hp_init(g_asrc_48to16_24bit_buff_ptr, TLKA_ASRC_48_TO_16, chnl, 0);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 24bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 48to16 24bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to16_24bit_deinit(void)
{
    g_asrc_48to16_24bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_48to16_24bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 48to16 24bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_48to16_24bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to16 24bit input buff point null");
        return 0;
    }

    if (g_asrc_48to16_24bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to16 24bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to16 24bit data len over");
        return 0;
    }

    int len_out = tlka_asrc_24_bit_process_frame_hp(g_asrc_48to16_24bit_buff_ptr, (int *)ps, len, (int *)pd);
    // len_out = 480;
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "return len %d", len_out);

    return len_out;
}

/***************************************48to16 16bit*********************************************/
/**
 * @brief       Get the size required for 48to16 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_48to16_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_48_TO_16, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 48to16 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to16_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_48to16_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_48to16_16bit_buff_ptr->config_rst = 1;
    g_asrc_48to16_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_48to16_16bit_buff_ptr, TLKA_ASRC_48_TO_16, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init((void *)g_asrc_48to16_16bit_buff_ptr, TLKA_ASRC_48_TO_16, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 16bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 48to16 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to16_16bit_deinit(void)
{
    g_asrc_48to16_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_48to16_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 48to16 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_48to16_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to16 16bit input buff point null");
        return 0;
    }

    if (g_asrc_48to16_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to16 16bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to16 16bit data len over");
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_48to16_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame(g_asrc_48to16_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #endif
    // len_out = 480;
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "return len %d", len_out);

    return len_out;
}

/***************************************441to48 16bit*********************************************/
/**
 * @brief       Get the size required for 441to48 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_441to48_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_44_TO_48, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 441to48 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to48_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_441to48_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_441to48_16bit_buff_ptr->config_rst = 1;
    g_asrc_441to48_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_441to48_16bit_buff_ptr, TLKA_ASRC_44_TO_48, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init((void *)g_asrc_441to48_16bit_buff_ptr, TLKA_ASRC_44_TO_48, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 16bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 441to48 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to48_16bit_deinit(void)
{
    g_asrc_441to48_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_441to48_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 441to48 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_441to48_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to48 16bit input buff point null");
        return 0;
    }

    if (g_asrc_441to48_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to48 16bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to48 16bit data len over");
        return 0;
    }
    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_441to48_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame((void *)g_asrc_441to48_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #endif
    // len_out = 480;
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "return len %d", len_out);

    return len_out;
}

/**
 * @brief       Get the size required for 441to48 16bit backup ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_441to48_16bit_backup_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_44_TO_48, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 16bit backup chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 441to48 16bit backup ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to48_16bit_backup_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 16bit backup error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_441to48_16bit_buff_ptr_backup = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_441to48_16bit_buff_ptr_backup->config_rst = 1;
    g_asrc_441to48_16bit_buff_ptr_backup->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_441to48_16bit_buff_ptr_backup, TLKA_ASRC_44_TO_48, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init((void *)g_asrc_441to48_16bit_buff_ptr_backup, TLKA_ASRC_44_TO_48, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to48 16bit backup init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 441to48 16bit backup ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to48_16bit_backup_deinit(void)
{
    g_asrc_441to48_16bit_buff_ptr_backup = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_441to48_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 441to48 16bit backup ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_441to48_16bit_backup_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to48 16bit backup input buff point null");
        return 0;
    }

    if (g_asrc_441to48_16bit_buff_ptr_backup == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to48 16bit backup struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to48 16bit backup data len over");
        return 0;
    }
    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_441to48_16bit_buff_ptr_backup, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame((void *)g_asrc_441to48_16bit_buff_ptr_backup, (short *)ps, len, (short *)pd);
    #endif
    // len_out = 480;
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "return len %d", len_out);

    return len_out;
}

/***************************************441to16 16bit*********************************************/
/**
 * @brief       Get the size required for 441to16 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_441to16_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_44_TO_16, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to16 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 441to16 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to16_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to16 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_441to16_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_441to16_16bit_buff_ptr->config_rst = 1;
    g_asrc_441to16_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_441to16_16bit_buff_ptr, TLKA_ASRC_44_TO_16, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init(g_asrc_441to16_16bit_buff_ptr, TLKA_ASRC_44_TO_16, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 441to16 16bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 441to16 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_441to16_16bit_deinit(void)
{
    g_asrc_441to16_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_441to16_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 441to16 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_441to16_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to16 16bit input buff point null");
        return 0;
    }

    if (g_asrc_441to16_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to16 16bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 441to16 16bit data len over");
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_441to16_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame(g_asrc_441to16_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #endif
    // len_out = 480;
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "return len %d", len_out);

    return len_out;
}

/***************************************16to441 16bit*********************************************/
/**
 * @brief       Get the size required for 16to441 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_16to441_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_16_TO_44, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to441 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 16to441 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to441_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to441 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_16to441_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_16to441_16bit_buff_ptr->config_rst = 1;
    g_asrc_16to441_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_16to441_16bit_buff_ptr, TLKA_ASRC_16_TO_44, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init(g_asrc_16to441_16bit_buff_ptr, TLKA_ASRC_16_TO_44, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 16to441 16bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 16to441 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_16to441_16bit_deinit(void)
{
    g_asrc_16to441_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_16to441_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 16to441 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_16to441_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to441 16bit input buff point null");
        return 0;
    }

    if (g_asrc_16to441_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to441 16bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 16to441 16bit data len over");
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_16to441_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame(g_asrc_16to441_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #endif
    // len_out = 480;
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "return len %d", len_out);

    return len_out;
}

/***************************************48to32 16bit*********************************************/
/**
 * @brief       Get the size required for 48to32 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_48to32_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }
    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_48_TO_32, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to32 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 48to32 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to32_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to32 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_48to32_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_48to32_16bit_buff_ptr->config_rst = 1;
    g_asrc_48to32_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_48to32_16bit_buff_ptr, TLKA_ASRC_48_TO_32, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init(g_asrc_48to32_16bit_buff_ptr, TLKA_ASRC_48_TO_32, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 48to32 16bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 48to32 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_48to32_16bit_deinit(void)
{
	g_asrc_48to32_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_48to32_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 48to32 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_48to32_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to32 16bit input buff point null");
        return 0;
    }

    if (g_asrc_48to32_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to32 16bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 48to32 16bit data len over");
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_48to32_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame(g_asrc_48to32_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #endif

    return len_out;
}

/***************************************32to48 16bit*********************************************/
/**
 * @brief       Get the size required for 32to48 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_32to48_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }
    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_32_TO_48, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 32to48 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 32to48 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_32to48_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 32to48 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_32to48_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_32to48_16bit_buff_ptr->config_rst = 1;
    g_asrc_32to48_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_32to48_16bit_buff_ptr, TLKA_ASRC_32_TO_48, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init(g_asrc_32to48_16bit_buff_ptr, TLKA_ASRC_32_TO_48, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 32to48 16bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 32to48 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_32to48_16bit_deinit(void)
{
    g_asrc_32to48_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_32to48_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 32to48 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_32to48_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 32to48 16bit input buff point null");
        return 0;
    }

    if (g_asrc_32to48_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 32to48 16bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 32to48 16bit data len over");
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_32to48_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame(g_asrc_32to48_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #endif

    return len_out;
}

/***************************************32to16 16bit*********************************************/
/**
 * @brief       Get the size required for 32to16 16bit ASRC buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_asrc_32to16_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }
    #if TLKALG_HIGH_PERFORMANCE_EN
    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    #else
    uint16_t size = tlka_asrc_16_bit_get_size(TLKA_ASRC_48_TO_24, chnl);
    #endif
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 32to16 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the 32to16 16bit ASRC module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_asrc_32to16_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 32to16 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_asrc_32to16_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    #if TLKALG_HIGH_PERFORMANCE_EN
    g_asrc_32to16_16bit_buff_ptr->config_rst = 1;
    g_asrc_32to16_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_asrc_32to16_16bit_buff_ptr, TLKA_ASRC_48_TO_24, chnl, 0);
    #else
    int ret                      = tlka_asrc_16_bit_init(g_asrc_32to16_16bit_buff_ptr, TLKA_ASRC_48_TO_24, chnl);
    #endif
    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "asrc 32to16 16bit init error");
    }

    return ret;
}

/**
 * @brief       Deinitialize the 32to16 16bit ASRC module
 * @return      0 on success
 */
int8_t tlkalg_asrc_32to16_16bit_deinit(void)
{
    g_asrc_16to48_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_asrc_32to16_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the 32to16 16bit ASRC data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_asrc_32to16_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 32to16 16bit input buff point null");
        return 0;
    }

    if (g_asrc_32to16_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 32to16 16bit struct point null");
        return 0;
    }

    if (len > HP_BUF_LEN) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "asrc 32to16 16bit data len over");
        return 0;
    }

    #if TLKALG_HIGH_PERFORMANCE_EN
    int len_out = tlka_asrc_16_bit_process_frame_hp(g_asrc_32to16_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #else
    int len_out = tlka_asrc_16_bit_process_frame(g_asrc_32to16_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    #endif

    return len_out;
}




/***************************************ppm spk 16bit*********************************************/
/**
 * @brief       Get the size required for speaker PPM 16bit buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_spk_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the speaker PPM 16bit module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_tlkalg_ppm_spk_16bit_chn = chnl;
    g_ppm_spk_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    g_ppm_spk_16bit_buff_ptr->config_rst = 1;
    g_ppm_spk_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_ppm_spk_16bit_buff_ptr, TLKA_ASRC_SFO, chnl, 0);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk 16bit init error");
    }

    return ret;
}

/**
 * @brief       Set the PPM parameter for the speaker 16bit
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_spk_16bit_param_set(uint8_t type, void *param)
{
    (void)type;
    int *val = (int *)param;

    int tlkalg_ppm_spk_value = *val;
    if (g_ppm_spk_16bit_buff_ptr != NULL) {
        g_ppm_spk_16bit_buff_ptr->config_rst = 1;
        int ret = tlka_asrc_16_bit_hp_init(g_ppm_spk_16bit_buff_ptr, TLKA_ASRC_SFO, g_tlkalg_ppm_spk_16bit_chn, tlkalg_ppm_spk_value);
        // tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk val %d", tlkalg_ppm_spk_value);
        if (TLKA_ASRC_OK != ret) {
            tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk 16bit init error");
        }
    }
    
    return 0;
}

/**
 * @brief       Deinitialize the speaker PPM 16bit module
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_16bit_deinit(void)
{
    g_ppm_spk_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_ppm_spk_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the speaker PPM 16bit data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_spk_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm spk 16bit input buff point null");
        return 0;
    }

    if (g_ppm_spk_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm spk 16bit struct point null");
        return 0;
    }

    short *psrc = (short *)ps;
    short *pdes = (short *)pd;
    int length = len;
    int out_num = 0;
    while(length > 0) {
        uint16_t ni = length > HP_BUF_LEN ? HP_BUF_LEN : length;
        length -= ni;
        int out_len = tlka_asrc_16_bit_process_frame_hp(g_ppm_spk_16bit_buff_ptr, (short *)psrc, ni, (short *)pdes);
        if (g_tlkalg_ppm_spk_16bit_chn == TLKA_ASRC_STEREO) {
            psrc += ni * 2;      //stereo
            pdes += out_len * 2; //stereo
        } else {
            psrc += ni;      //mono
            pdes += out_len; //mono
        }
        
        out_num += out_len;
    }
    return out_num;
}
/***************************************ppm spk 24bit*********************************************/
/**
 * @brief       Get the size required for speaker PPM 24bit buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_spk_24bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_24_bit_hp_get_size(chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk 24bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the speaker PPM 24bit module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_24bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk 24bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_tlkalg_ppm_spk_24bit_chn = chnl;
    g_ppm_spk_24bit_buff_ptr = (tlka_asrc_24_bit_hp_param *)p_buff;
    g_ppm_spk_24bit_buff_ptr->config_rst = 1;
    g_ppm_spk_24bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_24_bit_hp_init(g_ppm_spk_24bit_buff_ptr, TLKA_ASRC_SFO, chnl, 0);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk 24bit init error");
    }

    return ret;
}

/**
 * @brief       Set the PPM parameter for the speaker 24bit
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_spk_24bit_param_set(uint8_t type, void *param)
{
    (void)type;
    int *val = (int *)param;

    int tlkalg_ppm_spk_value = *val;
    if (g_ppm_spk_24bit_buff_ptr != NULL) {
        g_ppm_spk_24bit_buff_ptr->config_rst = 1;
        int ret = tlka_asrc_24_bit_hp_init(g_ppm_spk_24bit_buff_ptr, TLKA_ASRC_SFO, g_tlkalg_ppm_spk_24bit_chn, tlkalg_ppm_spk_value);
        // tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk val %d", tlkalg_ppm_spk_value);
        if (TLKA_ASRC_OK != ret) {
            tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm spk 24bit init error");
        }
    }

    return 0;
}

/**
 * @brief       Deinitialize the speaker PPM 24bit module
 * @return      0 on success
 */
int8_t tlkalg_ppm_spk_24bit_deinit(void)
{
    g_ppm_spk_24bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_ppm_spk_24bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the speaker PPM 24bit data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_spk_24bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm spk 24bit input buff point null");
        return 0;
    }

    if (g_ppm_spk_24bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm spk 24bit struct point null");
        return 0;
    }

    int *psrc = (int *)ps;
    int *pdes = (int *)pd;
    int length = len;
    int out_num = 0;
    while(length > 0) {
        uint16_t ni = length > HP_BUF_LEN ? HP_BUF_LEN : length;
        length -= ni;
        int out_len = tlka_asrc_24_bit_process_frame_hp(g_ppm_spk_24bit_buff_ptr, (int *)psrc, ni, (int *)pdes);
        if (g_tlkalg_ppm_spk_24bit_chn == TLKA_ASRC_STEREO) {
            psrc += ni * 2;      //stereo
            pdes += out_len * 2; //stereo
        } else {
            psrc += ni;      //mono
            pdes += out_len; //mono
        }

        out_num += out_len;
    }
    return out_num;
}

/***************************************ppm mic 16bit*********************************************/
/**
 * @brief       Get the size required for microphone PPM 16bit buffer
 * @param[in]   channel - Channel configuration
 * @return      Size of the buffer
 */
uint16_t tlkalg_ppm_mic_16bit_get_size(uint8_t channel)
{
    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    uint16_t size = tlka_asrc_16_bit_hp_get_size(chnl);
    size          = (size + 3) / 4 * 4; //4 Byte align

    tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm mic 16bit chn %d size %d", chnl, size);
    return size;
}

/**
 * @brief       Initialize the microphone PPM 16bit module
 * @param[in]   p_buff - Pointer to buffer
 * @param[in]   channel - Channel configuration
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_16bit_init(uint8_t *p_buff, uint8_t channel)
{
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm mic 16bit error buff ptr");
        return 0;
    }

    int8_t chnl = tlkalg_asrc_24bit_channel_change(channel);
    if (chnl == -1) {
        return 0;
    }

    g_tlkalg_ppm_mic_16bit_chn = chnl;
    g_ppm_mic_16bit_buff_ptr = (tlka_asrc_16_bit_hp_param *)p_buff;
    g_ppm_mic_16bit_buff_ptr->config_rst = 1;
    g_ppm_mic_16bit_buff_ptr->buffer_rst = 1;
    int ret                      = tlka_asrc_16_bit_hp_init(g_ppm_mic_16bit_buff_ptr, TLKA_ASRC_SFO, chnl, 0);

    if (TLKA_ASRC_OK != ret) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm mic 16bit init error");
    }

    return ret;
}

/**
 * @brief       Set the PPM parameter for the microphone 16bit
 * @param[in]   type - Parameter type
 * @param[in]   param - Pointer to parameter value
 * @return      0 on success
 */
uint8_t tlkalg_ppm_mic_16bit_param_set(uint8_t type, void *param)
{
    (void)type;
    int *val = (int *)param;

    int tlkalg_ppm_mic_value = *val;
    if (g_ppm_mic_16bit_buff_ptr != NULL) {
        g_ppm_mic_16bit_buff_ptr->config_rst = 1;
        int ret = tlka_asrc_16_bit_hp_init(g_ppm_mic_16bit_buff_ptr, TLKA_ASRC_SFO, g_tlkalg_ppm_mic_16bit_chn, tlkalg_ppm_mic_value);
        if (TLKA_ASRC_OK != ret) {
            tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm mic 16bit init error");
        }
    }
    
    // tlkapi_trace(0xFFFFFFFF, "[TEST]", "ppm mic val %d", tlkalg_ppm_spk_value);
    return 0;
}

/**
 * @brief       Deinitialize the microphone PPM 16bit module
 * @return      0 on success
 */
int8_t tlkalg_ppm_mic_16bit_deinit(void)
{
    g_ppm_mic_16bit_buff_ptr = NULL;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_ppm_mic_16bit_deinit");

    return 0;
}

//len:
/**
 * @brief       Process the microphone PPM 16bit data
 * @param[in]   ps - Pointer to source data
 * @param[out]  pd - Pointer to destination data
 * @param[in]   len - Data length
 * @param[in]   width - Data width
 * @param[in]   channel - Channel configuration
 * @return      Number of processed frames
 */
int tlkalg_ppm_mic_16bit_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;

    if ((ps == NULL || pd == NULL)) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm mic 16bit input buff point null");
        return 0;
    }

    if (g_ppm_mic_16bit_buff_ptr == NULL) {
        tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm mic 16bit struct point null");
        return 0;
    }

    // if (len > HP_BUF_LEN) {
    //     tlkapi_trace(0xFFFFFFFF, "[ERROR]", "ppm mic 16bit data len over");
    //     return 0;
    // }

    // int len_out = tlka_asrc_16_bit_process_frame_hp(g_ppm_mic_16bit_buff_ptr, (short *)ps, len, (short *)pd);
    // return len_out;

    short *psrc = (short *)ps;
    short *pdes = (short *)pd;
    int length = len;
    int out_num = 0;
    while(length > 0) {
        uint16_t ni = length > HP_BUF_LEN ? HP_BUF_LEN : length;
        length -= ni;
        int out_len = tlka_asrc_16_bit_process_frame_hp(g_ppm_mic_16bit_buff_ptr, (short *)psrc, ni, (short *)pdes);
        if (g_tlkalg_ppm_mic_16bit_chn == TLKA_ASRC_STEREO) {
            psrc += ni * 2;      //stereo
            pdes += out_len * 2; //stereo
        } else {
            psrc += ni;      //mono
            pdes += out_len; //mono
        }
        
        out_num += out_len;
    }
    return out_num;
}
#endif

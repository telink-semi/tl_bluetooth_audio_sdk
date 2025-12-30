/********************************************************************************************************
 * @file    tlkalg_aac_interface.c
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
#include "tlkapi/tlkapi.h"
#include "tlkalg/audio/aac/tlka_aac_dec_api.h"

#define TLKMDI_ALGAAC_DBG_FLAG ((TLK_MAJOR_DBGID_MDI_AUDIO << 24) | (TLK_MINOR_DBGID_MDI_AUD_SNK << 16) | TLK_DEBUG_DBG_FLAG_ALL)
#define TLKMDI_ALGAAC_DBG_SIGN "[MDI]"

#if TLKALG_AAC_DEC_ENABLE
void             *g_aac_dec_env_buf_ptr = NULL;
void             *g_aac_scratch_buf_ptr = NULL;
HANDLE_AACDECODER aacDecoder_handle;

TLKA_AAC_DEC_CFG_PARAS aacParas = {
    .chIdx = AAC_CHANNEL_DEFAULT, /**< the output channels is matched with input bitstream */
};

/**
 * @brief       Calculate the size required for AAC decoding and scratch buffer
 * @param[in]   channel - the number of channels
 * @return      Total size required for AAC decoding
 */
uint16_t tlkalg_aac_dec_get_size(uint8_t channel)
{
    (void)channel;
    int size = tlka_aac_dec_get_size();
    size     = (size + 3) / 4 * 4;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "aac size %d", size);

    int scr_size = tlka_aac_dec_get_scratch_size();
    scr_size     = (scr_size + 3) / 4 * 4;
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "aac scratch size %d", scr_size);

    return (size + scr_size);
}

/**
 * @brief       Initialize AAC decoder with provided buffers
 * @param[in]   p_buff - pointer to the buffer for AAC decoding
 * @param[in]   channel - the number of channels
 * @return      0 on success, non-zero on failure
 */
int8_t tlkalg_aac_dec_init(uint8_t *p_buff, uint8_t channel)
{
    (void)channel;
    if (NULL == p_buff) {
        tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aac_dec_init error");
        return 0;
    }
    g_aac_dec_env_buf_ptr = p_buff;
    g_aac_scratch_buf_ptr = p_buff + ((tlka_aac_dec_get_size() + 3) / 4 * 4);

    aacParas.chIdx    = AAC_CHANNEL_DEFAULT; //AAC_CHANNEL_LEFT	AAC_CHANNEL_DEFAULT
    aacDecoder_handle = tlka_aac_dec_init(&aacParas, g_aac_dec_env_buf_ptr, g_aac_scratch_buf_ptr);

    return 0;
}

/**
 * @brief       Deinitialize AAC decoder
 * @return      0 on success, non-zero on failure
 */
int8_t tlkalg_aac_dec_deinit(void)
{
    tlkapi_trace(0xFFFFFFFF, "[TEST]", "tlkalg_aac_dec_deinit");
    g_aac_dec_env_buf_ptr = NULL;
    g_aac_scratch_buf_ptr = NULL;
    return 0;
}

/**
 * @brief       Process an AAC frame and decode it
 * @param[in]   ps - pointer to the source (undecoded) data
 * @param[in]   pd - pointer to the destination (decoded) data
 * @param[in]   len - length of the source data
 * @param[in]   width - the width of the audio data
 * @param[in]   channel - the number of channels
 * @return      Decoding state
 */
int tlkalg_aac_dec_process(uint8_t *ps, uint8_t *pd, uint16_t len, uint8_t width, uint8_t channel)
{
    (void)width;
    (void)channel;
    uint32_t          ret = -1;
    TLKA_AAC_DEC_INFO aacInfo;
    int               valid = len;

    //	tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "src addr %x", ps);

    if (ps == NULL || pd == NULL) {
        tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "PS or PD null");
        return ret;
    }

    if (g_aac_dec_env_buf_ptr == NULL || g_aac_scratch_buf_ptr == NULL) {
        tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "struct null");
        return ret;
    }

    ret = tlka_aac_dec_process_frame(aacDecoder_handle, (unsigned char *)ps, len, &valid, (short *)pd, 1024 * 2, &aacInfo);

    if (TLKA_AAC_DEC_OK != ret) {
        tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "dec error %d %d %d %x", ret, len, valid, ps);
    }

    return ret;
}

#if 0
/**
 * @brief       Set up the AAC required for the decoding and scratch buffer
 * @param[in]   aac_dec_env_buf_ptr - pointer to the system buffer for AAC decoding
 * @param[in]   aac_scratch_buf_ptr - pointer to the AAC module's scratch buffer
 * @return      Size required for the AAC decoding
 */
int aac_set_dec_buffer_scratch (void *aac_dec_env_buf_ptr, void *aac_scratch_buf_ptr)
{
 int aac_dec_env_size = tlka_aac_dec_get_size();
 
 aac_dec_env_size = (aac_dec_env_size+3)/4*4;
 if(NULL != aac_dec_env_buf_ptr) {
  g_aac_dec_env_buf_ptr = aac_dec_env_buf_ptr;
 } else {
  tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "aac_dec_env_buf_ptr NULL");
  return 0;
 }
 
 if(NULL != aac_scratch_buf_ptr) {
  g_aac_scratch_buf_ptr = aac_scratch_buf_ptr;
 }  else {
  tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "aac_scratch_buf_ptr NULL");
  return 0;
 }
 
 aacParas.chIdx    = AAC_CHANNEL_DEFAULT;//AAC_CHANNEL_LEFT	AAC_CHANNEL_DEFAULT
 aacDecoder_handle = tlka_aac_dec_init(&aacParas, g_aac_dec_env_buf_ptr, g_aac_scratch_buf_ptr);
 tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "dec scr %x", g_aac_dec_env_buf_ptr, g_aac_scratch_buf_ptr);
 return (aac_dec_env_size);
}
 
/**
 * @brief       Decode the left channel of AAC data
 * @param[in]   ps - pointer to the source (undecoded) data
 * @param[in]   len - length of the source data
 * @param[out]  pd - pointer to the destination (decoded) data
 * @return      Decoding state
 */
int tlkalg_aac_dec_chn_left (const unsigned char *ps, int len, unsigned char *pd)
{
 int ret;
 TLKA_AAC_DEC_INFO aacInfo;
 
 ret = tlka_aac_dec_process_frame(aacDecoder_handle,					///handle
                                    (unsigned char*)ps, len,			///input addr and size
                                    &len,								///valid packet length
                                    (short*)pd,1024*2,					///output addr and size
                                    &aacInfo);							///aac dec information
 
 return ret;
}
 
/**
 * @brief       Decode the right channel of AAC data
 * @param[in]   ps - pointer to the source (undecoded) data
 * @param[in]   len - length of the source data
 * @param[out]  pd - pointer to the destination (decoded) data
 * @return      Decoding state
 */
int tlkalg_aac_dec_chn_right (const unsigned char *ps, int len, unsigned char *pd)
{
 uint32_t ret = -1;
 TLKA_AAC_DEC_INFO aacInfo;
 
 if (ps == NULL || pd == NULL) {
  return ret;
 }
 
 ret = tlka_aac_dec_process_frame(aacDecoder_handle,
                                    (unsigned char*)ps, len,
                                    &len,
                                    (short*)pd,1024*2,
                                    &aacInfo);
 
 return ret;
}
 
/**
 * @brief       Decode stereo AAC data
 * @param[in]   ps - pointer to the source (undecoded) data
 * @param[in]   len - length of the source data
 * @param[out]  pd - pointer to the destination (decoded) data
 * @return      Decoding state
 */
int tlkalg_aac_dec_stereo (const unsigned char *ps, int len, unsigned char *pd)
{
 uint32_t ret = -1;
 TLKA_AAC_DEC_INFO aacInfo;
 int valid = len;
 
 if (ps == NULL || pd == NULL) {
  tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "PS or PD null");
  return ret;
 }
 
 ret = tlka_aac_dec_process_frame(aacDecoder_handle,
         (unsigned char*)ps, len,
         &valid,
         (short*)pd,1024*2,
         &aacInfo);
 if (TLKA_AAC_DEC_OK != ret) {
  tlkapi_trace(TLKMDI_ALGAAC_DBG_FLAG, TLKMDI_ALGAAC_DBG_SIGN, "dec error %d %d %d %x", ret, len, valid, ps);
 }
 
 return ret;
}
#endif
#endif

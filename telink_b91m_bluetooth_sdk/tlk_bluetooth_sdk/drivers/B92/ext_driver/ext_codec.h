/********************************************************************************************************
 * @file    ext_codec.h
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
#ifndef DRIVERS_B92_EXT_DRIVER_EXT_CODEC_H_
#define DRIVERS_B92_EXT_DRIVER_EXT_CODEC_H_

#include "bit.h"
#include "types.h"
#include "../driver.h"
#include "compiler.h"
#include "ext_peripherals/codec_0581/codec_0581.h"

#ifndef EXT_CODEC_0581
    #define EXT_CODEC_0581 0
#endif


//codec class
typedef enum
{
    TLK_CODEC_INPUT  = 1,
    TLK_CODEC_OUTPUT = 2,
} tlk_codec_e;

//codec frequency,support 16k,24k,32k,48k
typedef enum
{
    TLK_CODEC_FREQ_8000  = 1,
    TLK_CODEC_FREQ_11025 = 2,
    TLK_CODEC_FREQ_16000 = 3,
    TLK_CODEC_FREQ_22050 = 4,
    TLK_CODEC_FREQ_24000 = 5,
    TLK_CODEC_FREQ_32000 = 6,
    TLK_CODEC_FREQ_44100 = 7,
    TLK_CODEC_FREQ_48000 = 8,
    TLK_CODEC_FREQ_MAX   = 9,
} tlk_codec_frequency_e;

//codec channel,max 2 channel
typedef enum
{
    TLK_CODEC_1_CHANNEL   = 1,
    TLK_CODEC_2_CHANNEL   = 2,
    TLK_CODEC_CHANNEL_MAX = 3,
} tlk_codec_channel_e;

//codec mode,Mic only support input,I2S and Line support input and output
typedef enum
{
    TLK_CODEC_MIC,
    TLK_CODEC_LINE,
    TLK_CODEC_I2S,
} tlk_codec_mode_e;

/**
 * @brief      This function serves to init the hardware codec.
 * @return     none
 */
void codec_base_init(void);

/**
 * @brief      This function serves to close the hardware codec.
 * @return     none
 */
void codec_close(void);
/*****************************************codec Input*********************************************/
/**
 * @brief      This function serves to configure input path of the hardware codec.
 * @param[in]  freq  - codec frequency,search 'tlk_codec_frequency_e' for detail.
 * @param[in]  chanC - codec channel,one or two(max) channel.
 * @param[in]  mode  - codec mode,support Line,Mic,I2S.
 * @return     none
 */
void codec_config_input(tlk_codec_frequency_e freq, tlk_codec_channel_e chanC, tlk_codec_mode_e mode);
/**
 * @brief      This function serves to enable codec input path.
 * @param[in]  pBuffer   - codec input buffer.
 * @param[in]  bufferLen - codec input buffer length.
 * @return     none
 */
void codec_input_enable(u8 *pBuffer, u16 bufferLen);
/**
 * @brief      This function serves to disable codec input path.
 * @return     none
 */
void codec_input_disable(void);
/**
 * @brief      This function serves to get data length in input path buffer.
 * @return     none
 */
int codec_input_getDataLen(void);
/**
 * @brief      This function serves to get input data in input buffer.
 * @param[in]  pData   - data buffer.
 * @param[in]  len     - data length need to get from codec input buffer.
 * @return     none
 */
int codec_input_readData(u8 *pData, u16 len);

/*****************************************codec output*********************************************/
/**
 * @brief      This function serves to configure output path of the hardware codec.
 * @param[in]  freq  - codec frequency,search 'tlk_codec_frequency_e' for detail.
 * @param[in]  chanC - codec channel,one or two(max) channel.
 * @param[in]  mode  - codec mode,support Line,I2S.
 * @return     none
 */
void codec_config_output(tlk_codec_frequency_e freq, tlk_codec_channel_e chanC, tlk_codec_mode_e mode);
/**
 * @brief      This function serves to enable codec output path.
 * @param[in]  pBuffer   - codec output buffer.
 * @param[in]  bufferLen - codec output buffer length.
 * @return     none
 */
void codec_output_enable(u8 *pBuffer, u16 bufferLen);
/**
 * @brief      This function serves to disable codec input path.
 * @return     none
 */
void codec_output_disable(void);


u16  codec_get_InputBuffMaxlen(void);
u32  codec_get_InputWriteOffset(void);
u32  codec_get_InputReadOffset(void);
void codec_set_InputReadOffset(u32 rptr);
u16  codec_get_OutputBufferLen(void);
/**
 * @brief      This function serves to get the hardware read offset of the codec input path.
 * @return     none
 */
int codec_output_getReadOffset(void);
/**
 * @brief      This function serves to get the software write offset of the codec input path.
 * @return     none
 */
int codec_output_getWriteOffset(void);
/**
 * @brief      This function serves to set the software write offset of the codec input path.
 * @param[in]  offset - New software write offset need to set.
 * @return     none
 */
void codec_output_setWriteOffset(u32 offset);
/**
 * @brief      This function serves to write data to the buffer of the codec input path.
 * @param[in]  pData    - data need to write.
 * @param[in]  pDataLen - data length.
 * @return     none
 */
int codec_output_writeData(u8 *pData, u16 pDataLen);


#endif /* DRIVERS_B92_EXT_DRIVER_EXT_CODEC_H_ */

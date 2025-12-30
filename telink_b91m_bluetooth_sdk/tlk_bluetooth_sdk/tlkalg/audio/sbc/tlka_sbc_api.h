/********************************************************************************************************
 * @file    tlka_sbc_api.h
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
#ifndef SBC_API_H
#define SBC_API_H

#include <stdint.h>

/*! Construct version number from major/minor/micro values. */
#define SBC_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))

/*! Version number to ensure header and binary are matching. */
#define SBC_VERSION           SBC_VERSION_INT(0, 5, 4)

#define SBC_MAX_CHANNELS      2
#define SBC_X_BUFFER_SIZE     328
#define SBC_MAX_SUBBANDS      8
#define SBC_MAX_BLOCKS        16

#define SBC_MODE_MONO         0x00
#define SBC_MODE_DUAL_CHANNEL 0x01
#define SBC_MODE_STEREO       0x02
#define SBC_MODE_JOINT_STEREO 0x03

/* support 4 subbans float process */
//#define F_FLOAT

/* support 4 subbands odd DCT process  */
//#define ENC_DCT


#ifdef F_FLOAT
    #define OUT_24BIT 1
    #define OUT_32BIT 0
#else
    #define OUT_16BIT 1
#endif

#ifdef __riscv
    #ifdef F_FLOAT
        #define SBC_8SUBBANDS_ASM 0
    #else
        #define SBC_8SUBBANDS_ASM 1 //default setting of 1 in case of 8subbands FIXED
    #endif
#else
    #define SBC_8SUBBANDS_ASM 0
#endif

typedef unsigned char u8;

/*! Return library version number. It should match SBC_VERSION. */
int tlka_sbc_get_version(void);

typedef struct sbc_enc_para
{
#ifdef F_FLOAT
    float X[SBC_MAX_CHANNELS][SBC_X_BUFFER_SIZE];
#else
    int16_t X[SBC_MAX_CHANNELS][SBC_X_BUFFER_SIZE];
#endif
    int32_t  sb_sample_f[SBC_MAX_BLOCKS][SBC_MAX_CHANNELS][SBC_MAX_SUBBANDS];
    uint32_t scale_factor[SBC_MAX_CHANNELS][SBC_MAX_SUBBANDS];


    int32_t  position;
    uint32_t sbc_analyze_even;

    uint16_t sbc_samplerate;
    u8       sbc_blocks;
    u8       sbc_bitpool;
    u8       sbc_allocation;
    u8       sbc_channel;
    u8       sbc_subbands;

} sbc_enc_para_t;

typedef struct sbc_dec_para
{
#ifdef F_FLOAT
    float V[2][160];
#elif SBC_8SUBBANDS_ASM
    int16_t V[2][160]; //DATA_TYPE, 8subbands fixed use int16_t
#else
    int32_t V[2][160]; //DATA_TYPE, 8subbands fixed use int16_t
#endif

    u8 sbc_blocks;
    u8 sbc_bitpool;
    u8 sbc_allocation;
    u8 sbc_subbands;

} sbc_dec_para_t;

/*------------------------------------------------------*
* name:  _SBC_CFG_Param                                 *
* sbc param struct                                      *
*-------------------------------------------------------*/
typedef struct _SBC_CFG_Param
{
    uint16_t sbc_samplerate;
    u8       sbc_blocks;
    u8       sbc_bitpool;
    u8       sbc_allocation;
    u8       sbc_channel;
    u8       msbc;
    u8       sbc_subbands;
} SBC_CFG_Param;

#ifndef SBC_SYNCWORD
    #define SBC_SYNCWORD 0x9C
#endif

#ifndef MSBC_SYNCWORD
    #define MSBC_SYNCWORD 0xAD
#endif

#ifdef __XTENSA__
int tlka_sbc_enc_get_size(u8 msbc);
int tlka_sbc_dec_get_size(u8 msbc);
#else
int tlka_sbc_enc_get_size(void);
int tlka_sbc_dec_get_size(void);
#endif

/*init sbc enc/dec*/
int tlka_sbc_enc_init(sbc_enc_para_t *encoder_p, SBC_CFG_Param *sbc_param);
int tlka_sbc_dec_init(sbc_dec_para_t *decoder_p, SBC_CFG_Param *sbc_param);

/*init msbc enc/dec*/
int tlka_msbc_enc_init(sbc_enc_para_t *encoder_p);
int tlka_msbc_dec_init(sbc_dec_para_t *decoder_p);

/*set bitpool*/
void tlka_sbc_set_enc_blocks_bitpool(sbc_enc_para_t *encoder_p, u8 blocks, u8 bitpool);
void tlka_sbc_set_dec_blocks_bitpool(sbc_dec_para_t *decoder_p, u8 blocks, u8 bitpool);

/*----------------------------------------------------------*
* tlka_sbc_dec_process    			                       	*
*															*
* decoder 	: decoder struct                              	*
* buf 		: input buffer                                  *
* len 		: codesize = subbands * blocks * channels * 2   *
*           : codesize = input bytes						*
* outbuf 	: output buffer                                	*
* out_len 	: output buffer len                           	*
* msbc 		: 1->msbc   0->sbc                        		*
* sbc_out_chn_mask : 1                                  	*
* 															*
* return 	:  encoder output frame size(bytes)				*
* 			0: frame header error							*
*-----------------------------------------------------------*/
#ifdef F_FLOAT
uint32_t tlka_sbc_dec_process(sbc_dec_para_t *decoder, const uint8_t *buf, uint32_t len, uint32_t *outbuf, uint32_t *out_len, int msbc, uint8_t sbc_out_chn_mask);
#else
uint32_t tlka_sbc_dec_process(sbc_dec_para_t *decoder, const uint8_t *buf, uint32_t len, uint16_t *outbuf, uint32_t *out_len, int msbc, uint8_t sbc_out_chn_mask);
#endif

/*----------------------------------------------------------*
* tlka_sbc_enc_process             			              	*
* 														  	*
* encoder 	: encoder struct                              	*
* buf 		: input buffer                                	*
* len 		: codesize = subbands * blocks * channels * 2 	*
* 			: codesize = input bytes
* outbuf 	: output buffer                               	*
* out_len 	: encoder output frmae len                    	*
* msbc 		: 1->msbc   0->sbc              	          	*
* 														  	*
* return 	: codesize									  	*
* 		   	  0 -> encoder error						  	*
*-----------------------------------------------------------*/

#ifdef F_FLOAT
uint32_t tlka_sbc_enc_process(sbc_enc_para_t *encoder, float *buf, uint16_t len, uint8_t *outbuf, uint32_t *out_len, int msbc);
#else
uint32_t tlka_sbc_enc_process(sbc_enc_para_t *encoder, int16_t *buf, uint16_t len, uint8_t *outbuf, uint32_t *out_len, int msbc);
#endif

/* only use for hifi5 */
#ifdef __XTENSA__
int tlka_enc_hifi5_init(int16_t *buf, uint8_t *outbuf, SBC_CFG_Param *sbc_param);
int tlka_dec_hifi5_init(uint8_t *buf, int16_t *outbuf, SBC_CFG_Param *sbc_param);
/*----------------------------------------------------------*
* only support mSBC PLC
* plc = 0: normal packet
* plc = 1: loss packet
*-----------------------------------------------------------*/
uint32_t tlka_sbc_plc(int plc);
#endif

#endif

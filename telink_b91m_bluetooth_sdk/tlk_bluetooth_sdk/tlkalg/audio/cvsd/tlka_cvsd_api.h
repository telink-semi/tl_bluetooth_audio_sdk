/********************************************************************************************************
 * @file    tlka_cvsd_api.h
 *
 * @brief   This is the header file for TLSR/TL
 *
 * @author  Bluetooth Group
 * @date    2024
 *
 * @par     Copyright (c) 2024, Telink Semiconductor (Shanghai) Co., Ltd.
 *          All rights reserved.
 *
 *          The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or
 *          alter) any information contained herein in whole or in part except as expressly authorized
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRANTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef TLKA_CVSD_API_H
#define TLKA_CVSD_API_H

#define CVSD_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define CVSD_VERSION                          CVSD_VERSION_INT(0, 8, 0)
#define BL                                    (77) //(116) //(108) //(54)


#include <stdint.h>
#define SCRATCH_BUFFER_ALIGNMENT_BITS 2
#define scratchAlign(ptr, offset)     (void *)(((uintptr_t)(ptr) + (offset) + 0x3) & (uintptr_t)~0x3)

#define CVSD_RAM_CODE_1_EN            1
#define CVSD_RAM_CODE_2_EN            1

#define CVSD_RAM_DATA_EN              1

//for cvsd
#if CVSD_RAM_CODE_1_EN
#define CVSD_RAM_CODE1 __attribute__((section(".cvsd_ram_code1"))) __attribute__((noinline))
#else
#define CVSD_RAM_CODE1
#endif


//for plc
#if CVSD_RAM_CODE_2_EN
#define CVSD_RAM_CODE2 __attribute__((section(".cvsd_ram_code2"))) __attribute__((noinline))
#else
#define CVSD_RAM_CODE2
#endif


#if CVSD_RAM_DATA_EN
#define CVSD_RAM_DATA __attribute__((section(".cvsd_ram_data"))) __attribute__((noinline))
#else
#define CVSD_RAM_DATA
#endif


#ifdef USEDOUBLES
typedef double Float; /* likely to be bit-exact between machines */
#else
#define Float float
//  typedef float Float;
#endif
#if __riscv
#define CVSD_FIXED_POINT 0
#else
#define CVSD_FIXED_POINT 0
#endif

#define PLC_SAMPLE 16000

#if PLC_SAMPLE == 48000
#define PITCH_MIN_HZ      200
#define PITCH_MAX_HZ      66
#define PITCH_MIN         40 * (PLC_SAMPLE / 8000) /* minimum allowed pitch, 200 Hz */
#define PITCH_MAX         80 * (PLC_SAMPLE / 8000) /* maximum allowed pitch, 66 Hz */
#define PITCHDIFF         (PITCH_MAX - PITCH_MIN)
#define POVERLAPMAX       (PITCH_MAX >> 2)                     /* maximum pitch OLA window */
#define HISTORYLEN        (PITCH_MAX * 3 + POVERLAPMAX)        /* history buffer length */
#define NDEC              PLC_SAMPLE / 4000                    /* decimation */
#define CORRLEN_TIME      5                                    /* 20msec */
#define CORRLEN           PLC_SAMPLE *CORRLEN_TIME / 1000      /* 20 msec correlation length */
#define CORRBUFLEN        (CORRLEN + PITCH_MAX)                /* correlation buffer length */
#define CORRLEN_SHIFT     ((short)__nds__clz16(CORRLEN) + 2)   //((int)ceil(log2(CORRLEN)))
#define CORRMINPOWER      (250)                                /* minimum power */
#define EOVERLAPINCR_TIME 0.1                                  /* end OLA increment per frame, 4ms */
#define EOVERLAPINCR      PLC_SAMPLE *EOVERLAPINCR_TIME / 1000 /* end OLA increment per frame, 4ms */
#define FRAMESZ_TIME      25                                   /* 10 msec  */
#define FRAMESZ           PLC_SAMPLE *FRAMESZ_TIME / 10000
#define ATTENFAC          ((Float).2)          /* attenuation factor per 10ms frame */
#define ATTENINCR         (ATTENFAC / FRAMESZ) /* attenuation per sample */
#else
#define PITCH_MIN_HZ 200
#define PITCH_MAX_HZ 66
#define PITCH_MIN    (40 * (PLC_SAMPLE / 8000))  /* minimum allowed pitch, 200 Hz */
#define PITCH_MAX    (120 * (PLC_SAMPLE / 8000)) /* maximum allowed pitch, 66 Hz */
#define PITCHDIFF    (PITCH_MAX - PITCH_MIN)
#define POVERLAPMAX  (PITCH_MAX >> 2)                   /* maximum pitch OLA window */
#define HISTORYLEN   (PITCH_MAX * 3 + POVERLAPMAX)      /* history buffer length */
#define NDEC         (PLC_SAMPLE / 4000)                /* decimation */
#define CORRLEN_TIME 20                                 /* 20msec */
#define CORRLEN      (PLC_SAMPLE * CORRLEN_TIME / 1000) /* 20 msec correlation length */
#define CORRBUFLEN   (CORRLEN + PITCH_MAX)              /* correlation buffer length */
#if __riscv
#define CORRLEN_SHIFT ((short)__nds__clz16(CORRLEN) + 2) //((int)ceil(log2(CORRLEN)))
#else
#define CORRLEN_SHIFT ((short)ceil(log2(CORRLEN)))
#endif
#define CORRMINPOWER      (250)       /* minimum power */
#define EOVERLAPINCR_TIME 5           /* end OLA increment per frame, 4ms */
#define EOVERLAPINCR      120         // PLC_SAMPLE*EOVERLAPINCR_TIME/1000 //30    /* end OLA increment per frame, 4ms */
#define ATTENFAC          ((Float).2) /* attenuation factor per 10ms frame */
#define ATTENMIN          0.001f
#endif

/* channel type */
typedef enum
{
    TLKA_CVSD_SINGLE = 1,
    TLKA_CVSD_STEREO

} TLKA_CVSD_CHANNEL;

/* frame length selection */
typedef enum
{
    TLKA_CVSD_5MS   = 1,
    TLKA_CVSD_7_5MS = 2,
    TLKA_CVSD_10MS

} TLKA_CVSD_FRAME;

typedef enum
{
    TLKA_CVSD_OK            = 0,
    TLKA_CVSD_INVALID_FRAME = -1,
} TLKA_CVSD_ERROR;

typedef struct fir_down
{
    int          factor;
    short        state[BL];
    const short *coeff;
} fir_down_t;

typedef struct dcmfir_short
{
    int    M;
    int    coeff_size;
    short *coeff;
    short *state;
} dcmfir_short_t;

typedef struct tlka_cvsd_enc_para
{
    //    int accumulator;
    //    int stepsize;
    //    int counter;
    //    int histBits;

    int    accumulator_left;
    int    accumulator_right;
    int    stepsize_left;
    int    stepsize_right;
    int    histBits_left;
    int    histBits_right;
    short *pcm2_right;
    short *pcm1_right;
    short *pcm_right;

    TLKA_CVSD_CHANNEL channel_mode;
    TLKA_CVSD_FRAME   frames;
} TLKA_CVSD_ENC_PARA;

typedef struct tlka_cvsd_dec_para
{
    //    int accumulator;
    //    int stepsize;
    //    int counter;
    //    int histBits;

    int accumulator_left;
    int accumulator_right;
    int stepsize_left;
    int stepsize_right;
    int histBits_left;
    int histBits_right;

    short *pcm2_right;
    short *pcm1_right;
    short *pcm64k_right;

    TLKA_CVSD_CHANNEL channel_mode;
    TLKA_CVSD_FRAME   frames;

    fir_down_t    *pDown;
    dcmfir_short_t decStage1_left;
    dcmfir_short_t decStage1_right;
    dcmfir_short_t decStage2_left;
    dcmfir_short_t decStage2_right;
} TLKA_CVSD_DEC_PARA;

typedef struct cvsd_plc
{
    int erasecnt;  /* consecutive erased frames */
    int poverlap;  /* overlap based on pitch */
    int poffset;   /* offset into pitch period */
    int pitch;     /* pitch estimate */
    int pitchblen; /* current pitch buffer length */
#if CVSD_FIXED_POINT
    short *pitchbufend;          /* end of pitch buffer */
    short *pitchbufstart;        /* start of pitch buffer */
    short  pitchbuf[HISTORYLEN]; /* buffer for cycles of speech */
    short  lastq[POVERLAPMAX];   /* saved last quarter wavelength */
#else
    Float *pitchbufend;          /* end of pitch buffer */
    Float *pitchbufstart;        /* start of pitch buffer */
    Float  pitchbuf[HISTORYLEN]; /* buffer for cycles of speech */
    Float  lastq[POVERLAPMAX];   /* saved last quarter wavelength */
    Float  gain;
#endif
    short history[HISTORYLEN]; /* history buffer */
    short a;

    short *buf_tmp; /* length FRAMESZ */

    TLKA_CVSD_CHANNEL channel_mode;
    TLKA_CVSD_FRAME   frames;

} CVSD_PLC;

typedef struct TLKA_CVSD_PLC_
{
    CVSD_PLC plc_left;
    CVSD_PLC plc_right;

} TLKA_CVSD_PLC;

/* public functions */
TLKA_CVSD_ERROR tlka_cvsd_g711plc_get_size(TLKA_CVSD_CHANNEL channel, TLKA_CVSD_FRAME frames);
void            tlka_cvsd_g711plc_init(void *state, TLKA_CVSD_CHANNEL channel, TLKA_CVSD_FRAME frames);
void            tlka_cvsd_g711plc_dofe(void *state, short *s);         /* synthesize speech for erasure */
void            tlka_cvsd_g711plc_addtohistory(void *state, short *s); /* add a good frame to history buffer */

int tlka_cvsd_get_version(void);

TLKA_CVSD_ERROR tlka_cvsd_enc_get_size(TLKA_CVSD_CHANNEL channel, TLKA_CVSD_FRAME frames);
TLKA_CVSD_ERROR tlka_cvsd_dec_get_size(TLKA_CVSD_CHANNEL channel, TLKA_CVSD_FRAME frames);

int tlka_cvsd_enc_get_scratch_size();
int tlka_cvsd_dec_get_scratch_size();

int tlka_cvsd_enc_init(void *state, TLKA_CVSD_CHANNEL channel, TLKA_CVSD_FRAME frames);

int tlka_cvsd_dec_init(void *state, TLKA_CVSD_CHANNEL channel, TLKA_CVSD_FRAME frames);

/*----------------------------------------------------------------*
 * name: tlka_cvsd_enc_process                                    *
 * pcm: input pcm data                                            *
 * samples: processing length (is equal to the frame length)      *
 * bs: output bits                                                *
 *----------------------------------------------------------------*/
void tlka_cvsd_enc_process(void *state, short *pcm, int samples, unsigned char *b, void *scratch);

/*----------------------------------------------------------------*
 * name: tlka_cvsd_dec_process                                    *
 * bs: output bits                                                *
 * bytes: the length of the current input byte                    *
 * pcm: output pcm data                                           *
 *----------------------------------------------------------------*/
void cvsd_dec_mute(void *state, short *pcm, int samples);
void tlka_cvsd_dec_process(void *state, unsigned char *bs, int bytes, short *pcm, void *scratch);

#endif

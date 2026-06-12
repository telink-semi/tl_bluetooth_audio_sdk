/* Copyright (C) 2003 Jean-Marc Valin */
/**
   @file arch.h
   @brief Various architecture definitions Speex
*/
/*
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of the Xiph.org Foundation nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ARCH_H
#define ARCH_H

#if __riscv
#include <nds_math_types.h>
#include <nds_intrinsic.h>
#include <nds_utils_math.h>
#include <nds_basic_math.h>
#include <nds_statistics_math.h>
#include <nds_complex_math.h>
#define ANDES_LOG_ENABLE 0
#if ANDES_LOG_ENABLE
extern void usb_send_str_data(char *str, unsigned char *ph, int n);
#define my_dump_str_data(en, s, p, n)                  \
    if (en) {                                          \
        usb_send_str_data(s, (unsigned char *)(p), n); \
    }
#endif
#endif

//#include <string.h>
//#include <hifi2/basic_op_xtensa.h>
//#include <hifi2/oper_32b_xtensa.h>
#include <string.h>

#ifndef COMMON_LIB_RAMCODE

#define COMMON_LIB_RAMCODE

#endif


#ifndef M_PI
#define M_PI 3.14159263
#endif

#define USE_BLE_WIN256 1

#define ABS(x)         ((x) < 0 ? (-(x)) : (x)) /**< Absolute integer value. */
#define ABS16(x)       ((x) < 0 ? (-(x)) : (x)) /**< Absolute 16-bit value.  */
#define MIN16(a, b)    ((a) < (b) ? (a) : (b))  /**< Maximum 16-bit value.   */
#define MAX16(a, b)    ((a) > (b) ? (a) : (b))  /**< Maximum 16-bit value.   */
#define ABS32(x)       ((x) < 0 ? (-(x)) : (x)) /**< Absolute 32-bit value.  */
#define MIN32(a, b)    ((a) < (b) ? (a) : (b))  /**< Maximum 32-bit value.   */
#define MAX32(a, b)    ((a) > (b) ? (a) : (b))  /**< Maximum 32-bit value.   */
#define MIN(a, b)      ((a) < (b) ? (a) : (b))
#define MAX(a, b)      ((a) > (b) ? (a) : (b))

#if __riscv
typedef short          spx_int16_t;
typedef unsigned short spx_uint16_t;
typedef int            spx_int32_t;
typedef unsigned int   spx_uint32_t;
#else
typedef int            spx_int32_t;
typedef unsigned int   spx_uint32_t;
typedef short          spx_int16_t;
typedef unsigned short spx_uint16_t;
#endif


#if XCHAL_HAVE_HIFI5

typedef float spx_mem_t;
typedef float spx_coef_t;
typedef float spx_lsp_t;
typedef float spx_sig_t;
typedef short spx_word16_t;
typedef int   spx_word32_t;

#else

typedef float spx_mem_t;
typedef float spx_coef_t;
typedef float spx_lsp_t;
typedef float spx_sig_t;
typedef float spx_word16_t;
typedef float spx_word32_t;

#endif

typedef struct complexF_
{
    spx_word16_t real;
    spx_word16_t imag;
} complexF;

typedef struct complexFloat_
{
    float real;
    float imag;
} complexFloat;

typedef struct complexF32_
{
    spx_word32_t real;
    spx_word32_t imag;
} complexF32;

#define Q15ONE                    1.0f
#define LPC_SCALING               1.f
#define SIG_SCALING               1.f
#define LSP_SCALING               1.f
#define GAMMA_SCALING             1.f
#define GAIN_SCALING              1.f
#define GAIN_SCALING_1            1.f


#define VERY_SMALL                1e-15f
#define VERY_LARGE32              1e15f
#define VERY_LARGE16              1e15f
#define Q15_ONE                   ((spx_word16_t)1.f)

#define QCONST16(x, bits)         (x)
#define QCONST32(x, bits)         (x)

#define NEG16(x)                  (-(x))
#define NEG32(x)                  (-(x))
#define EXTRACT16(x)              (x)
#define EXTEND32(x)               (x)
#define SHR16(a, shift)           (a)
#define SHL16(a, shift)           (a)
#define VSHR16(a, shift)          (a)
#define SHR32(a, shift)           (a)
#define SHL32(a, shift)           (a)
#define PSHR16(a, shift)          (a)
#define PSHR32(a, shift)          (a)
#define VSHR32(a, shift)          (a)
#define SATURATE16(x, a)          (x)
#define SATURATE32(x, a)          (x)

#define PSHR(a, shift)            (a)
#define SHR(a, shift)             (a)
#define SHL(a, shift)             (a)
#define VSHL(a, shift)            (a)
#define SATURATE(x, a)            (((x) > (a) ? (a) : (x) < -(a) ? -(a) : (x)))
#define VSHR(a, shift)            (a)

#define ADD16(a, b)               ((a) + (b))
#define SUB16(a, b)               ((a) - (b))
#define ADD32(a, b)               ((a) + (b))
#define SUB32(a, b)               ((a) - (b))
#define MULT16_16_16(a, b)        ((a) * (b))
#define MULT16_16(a, b)           ((spx_word32_t)(a) * (spx_word32_t)(b))
#define MAC16_16(c, a, b)         ((c) + (spx_word32_t)(a) * (spx_word32_t)(b))
#define MULT_float(a, b)          ((a) * (b))

#define MULT16_32_Q11(a, b)       ((a) * (b))
#define MULT16_32_Q13(a, b)       ((a) * (b))
#define MULT16_32_Q14(a, b)       ((a) * (b))
#define MULT16_32_Q15(a, b)       ((a) * (b))
#define MULT16_32_P15(a, b)       ((a) * (b))

#define MAC16_32_Q11(c, a, b)     ((c) + (a) * (b))
#define MAC16_32_Q15(c, a, b)     ((c) + (a) * (b))

#define MAC16_16_Q11(c, a, b)     ((c) + (a) * (b))
#define MAC16_16_Q13(c, a, b)     ((c) + (a) * (b))
#define MAC16_16_P13(c, a, b)     ((c) + (a) * (b))
#define MULT16_16_Q11_32(a, b)    ((a) * (b))
#define MULT16_16_Q13(a, b)       ((a) * (b))
#define MULT16_16_QC(a, b, c)     ((a) * (b))
#define MULT16_16_Q14(a, b)       ((a) * (b))
#define MULT16_16_Q15(a, b)       ((a) * (b))
#define MULT16_16_P15(a, b)       ((a) * (b))
#define MULT16_16_P13(a, b)       ((a) * (b))
#define MULT16_16_P14(a, b)       ((a) * (b))

#define DIV32_16(a, b)            (((spx_word32_t)(a)) / (spx_word16_t)(b))
#define PDIV32_16(a, b)           (((spx_word32_t)(a)) / (spx_word16_t)(b))
#define DIV32(a, b)               (((spx_word32_t)(a)) / (spx_word32_t)(b))
#define PDIV32(a, b)              (((spx_word32_t)(a)) / (spx_word32_t)(b))
#define DIV_float(a, b)           (((float)(a)) / (float)(b))

#define SQR16_Q15(x)              (MULT16_16_Q15((x), (x)))
#define scratchAlign(ptr, offset) (void *)((char *)(ptr) + (offset))

#define BYTES_PER_CHAR            1
#define BITS_PER_CHAR             8
#define LOG2_BITS_PER_CHAR        3

//static complexF constrain_G(spx_word32_t realPart, spx_word32_t imagPart, float maxVal);

static inline complexF complexInit(complexF c1)
{
    return c1;
};

static inline complexF complexAdd(complexF c1, complexF c2)
{
    c1.real = ADD16(c1.real, c2.real);
    c1.imag = ADD16(c1.imag, c2.imag);
    return c1;
};

static inline complexFloat complexAddFloat(complexFloat c1, complexFloat c2)
{
    c1.real = ADD16(c1.real, c2.real);
    c1.imag = ADD16(c1.imag, c2.imag);
    return c1;
};

static inline complexF complexSub(complexF c1, complexF c2)
{
    c1.real = SUB16(c1.real, c2.real);
    c1.imag = SUB16(c1.imag, c2.imag);
    return c1;
};

static inline complexFloat complexSubFloat(complexFloat c1, complexFloat c2)
{
    c1.real = SUB16(c1.real, c2.real);
    c1.imag = SUB16(c1.imag, c2.imag);
    return c1;
};

static inline complexF complexMultConj(complexF c1, complexF c2, spx_word16_t bitShift)
{
    complexF c3;
    c3.real = ADD16(MULT16_16_QC(c1.real, c2.real, bitShift), MULT16_16_QC(c1.imag, c2.imag, bitShift));
    c3.imag = SUB16(MULT16_16_QC(c1.imag, c2.real, bitShift), MULT16_16_QC(c1.real, c2.imag, bitShift));
    return c3;
};

static inline complexFloat complexMultConjFloat(complexFloat c1, complexFloat c2, spx_word16_t bitShift)
{
    complexFloat c3;
    c3.real = ADD16(MULT16_16_QC(c1.real, c2.real, bitShift), MULT16_16_QC(c1.imag, c2.imag, bitShift));
    c3.imag = SUB16(MULT16_16_QC(c1.imag, c2.real, bitShift), MULT16_16_QC(c1.real, c2.imag, bitShift));
    return c3;
};

static inline complexF32 complexMultConj32(complexF c1, complexF c2, spx_word16_t bitShift)
{
    complexF32 c3;
    c3.real = ADD32(MULT16_16_QC(c1.real, c2.real, bitShift), MULT16_16_QC(c1.imag, c2.imag, bitShift));
    c3.imag = SUB32(MULT16_16_QC(c1.imag, c2.real, bitShift), MULT16_16_QC(c1.real, c2.imag, bitShift));
    return c3;
};

static inline complexF complexMult(complexF c1, complexF c2, spx_word16_t bitShift)
{
    complexF c3;
    c3.real = SUB16(MULT16_16_QC(c1.real, c2.real, bitShift), MULT16_16_QC(c1.imag, c2.imag, bitShift));
    c3.imag = ADD16(MULT16_16_QC(c1.real, c2.imag, bitShift), MULT16_16_QC(c1.imag, c2.real, bitShift));
    return c3;
};

static inline complexFloat complexMultFloat(complexFloat c1, complexFloat c2, spx_word16_t bitShift)
{
    complexFloat c3;
    c3.real = SUB16(MULT16_16_QC(c1.real, c2.real, bitShift), MULT16_16_QC(c1.imag, c2.imag, bitShift));
    c3.imag = ADD16(MULT16_16_QC(c1.real, c2.imag, bitShift), MULT16_16_QC(c1.imag, c2.real, bitShift));
    return c3;
};

static inline complexF complexMultReal(complexF c1, spx_word16_t f1, spx_word16_t bitShift)
{
    c1.real = MULT16_16_QC(c1.real, f1, bitShift);
    c1.imag = MULT16_16_QC(c1.imag, f1, bitShift);
    return c1;
};

static inline complexFloat complexMultRealFloat(complexFloat c1, float f1, spx_word16_t bitShift)
{
    c1.real = MULT16_16_QC(c1.real, f1, bitShift);
    c1.imag = MULT16_16_QC(c1.imag, f1, bitShift);
    return c1;
};

static inline spx_word16_t complexPwr(complexF c1, spx_word16_t bitShift)
{
    spx_word16_t f1;
    f1 = ADD16(MULT16_16_QC(c1.real, c1.real, bitShift), MULT16_16_QC(c1.imag, c1.imag, bitShift));
    return f1;
}

static inline spx_word32_t complexPwr32(complexF c1, spx_word16_t bitShift)
{
    spx_word32_t f1;
    f1 = ADD32(MULT16_16_QC(c1.real, c1.real, bitShift), MULT16_16_QC(c1.imag, c1.imag, bitShift));
    return f1;
}

static inline float complexPwrFloat(complexFloat c1, spx_word16_t bitShift)
{
    float f1;
    f1 = ADD32(MULT16_16_QC(c1.real, c1.real, bitShift), MULT16_16_QC(c1.imag, c1.imag, bitShift));
    return f1;
}

static inline complexF complexFilter(complexF y, complexF x, spx_word16_t coef)
{
    y.real = ADD16(x.real, MULT16_16_QC(SUB16(y.real, x.real), coef, BITCOEFF));
    y.imag = ADD16(x.imag, MULT16_16_QC(SUB16(y.imag, x.imag), coef, BITCOEFF));
    return y;
}

static inline void matClear_spx16(spx_word16_t *c1, int size)
{
#ifdef __riscv
    nds_set_q7(0, (q7_t *)c1, size * sizeof(spx_word16_t));
#else
    memset(c1, 0, size * sizeof(spx_word16_t));
#endif
};

static inline void matClear_spx32(spx_word32_t *c1, int size)
{
#ifdef __riscv
    nds_set_q7(0, (q7_t *)c1, size * sizeof(spx_word32_t));
#else
    memset(c1, 0, size * sizeof(spx_word32_t));
#endif
};

static inline void matClear_complexF(complexF *c1, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        c1[i].real = .0f;
        c1[i].imag = .0f;
    }
}

static inline void matClear_complexF32(complexF32 *c1, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        c1[i].real = .0f;
        c1[i].imag = .0f;
    }
}

static inline void matClear_float(float *c1, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        c1[i] = .0f;
    }
};

static inline void matSet_float(float *c1, float val, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        c1[i] = val;
    }
};

static inline void matSet_int(int *c1, int val, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        c1[i] = val;
    }
};

static inline void matClear_short(short *c1, int size)
{
#ifdef __riscv
    nds_set_q7(0, (q7_t *)c1, size * sizeof(short));
#else
    memset(c1, 0, size * sizeof(short));
#endif
};

#endif

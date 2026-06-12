#ifndef TLKA_BBF_ALG_API_H
#define TLKA_BBF_ALG_API_H

//#define EN_PRINT_DEBUG

/*! Version number to ensure header and binary are matching. */
#define BBF_ALG_VERSION_INT(major, minor, micro) (((major) << 16) | ((minor) << 8) | (micro))
#define BBF_ALG_VERSION_                         (int)BBF_ALG_VERSION_INT(1, 2, 2)

#ifndef BBF_USE_STACK_SCRATCH
#define BBF_USE_STACK_SCRATCH 1
#endif

#define BBF_IO_STREAM_INTERFACE 1

#ifndef BBF_ALG_RAMCODE

#if __riscv | XCHAL_HAVE_HIFI5

#define BBF_ALG_RAMCODE __attribute__((section(".bbf_ram_code")))

#else
#define BBF_ALG_RAMCODE
#endif

#endif

#if BBF_IO_STREAM_INTERFACE
#include "../common_inc/io_stream.h"
#endif

typedef struct
{
    /* support 7.5ms/5ms
	 * 7.5ms: 120
	 * 5ms: 80 */
    unsigned int frame_size;

    /* only support 16K */
    unsigned int sample_rate;

    //array mic number
    unsigned int mic_num;

    //ref mic index
    unsigned int ref_mic_idx;

} BBF_ALG_Param;

int tlka_bbf_alg_get_version(void);
int tlka_bbf_alg_get_size(BBF_ALG_Param *pst_para);
#if BBF_USE_STACK_SCRATCH == 1
int tlka_bbf_alg_init(void *st_in, BBF_ALG_Param *pst_para, unsigned int mem_size);
#else
int tlka_bbf_alg_init(void *st_in, BBF_ALG_Param *pst_para, void *ScratchBuffer, unsigned int mem_size);
#endif

int tlka_bbf_alg_get_scratch_size(void);

#if BBF_USE_STACK_SCRATCH == 1
/*--------------------------------------------------------------------------------------*
* name : tlka_bbf_alg_process_frame                                                  *
* st_in   : input struct pointer                                                           *
* x_in : 	input mic signals buffer                                                       *
* ScratchBuffer : scratch buffer                                                      *
* x_out : output signal buffer                                                          *
*---------------------------------------------------------------------------------------*/
int tlka_bbf_alg_process_frame_float(void *st_in, void *pScratch, float *x_in, float *x_out);
int tlka_bbf_alg_process_frame_16(void *st_in, void *pScratch, short *x_in, short *x_out);
int tlka_bbf_alg_process_frame_24(void *st_in, void *pScratch, int *x_in, int *x_out);

#if BBF_IO_STREAM_INTERFACE
int tlka_alg_chain_bbf_process_frame_float(void *st, void *pScratch, ioStream *mic_stream, ioStream *out_stream);
int tlka_alg_chain_bbf_process_frame_24(void *st, void *pScratch, ioStream *mic_stream, ioStream *out_stream);
int tlka_alg_chain_bbf_process_frame_16(void *st, void *pScratch, ioStream *mic_stream, ioStream *out_stream);
#endif

#else
/*--------------------------------------------------------------------------------------*
* name : tlka_bbf_alg_process_frame                                                  	*
* st_in   : input struct pointer                                                        *
* x_in : 	input mic signals buffer                                                    *
* x_out : output signal buffer                                                          *
*---------------------------------------------------------------------------------------*/

int tlka_bbf_alg_process_frame(void *st_in, short *x_in, short *x_out);
#endif

#endif
#ifdef __cplusplus
}
#endif

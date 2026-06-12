
#ifndef SYNC_WIN_H
#define SYNC_WIN_H

//#include "arch.h"

#define WIN_FRAME_SIZE_80  80
#define WIN_FRAME_SIZE_120 120
#define WIN_FRAME_SIZE_160 160

#define WIN_FFT_SIZE_80    128
#define WIN_FFT_SIZE_120   256
#define WIN_FFT_SIZE_160   320

#define WIN_OVLAP_SIZE_80  48
#define WIN_OVLAP_SIZE_120 136
#define WIN_OVLAP_SIZE_160 160

//Applies the filterbank window to a buffer.
//support 128 and 256 window size
void ApplyFilterBankWindow_16(short *x, short frame_size, short fftsize);

//Applies the filterbank window to a buffer(short data)
//support 128 and 256 window size
void ApplyFilterBankWindow_32(int *y, short *x, short frame_size, short fftsize);

//Applies the filterbank window to a buffer(float data).
//only support 320 window size
void ApplyFilterBankWindow_float(float *x, short frame_size, short fftsize);

void FormExtendedFrame_16(short *frame, short *old_data, short *extended_frame, short frame_size, short fftsize);

void FormExtendedFrame_float(float *frame, float *old_data, float *extended_frame, short frame_size, short fftsize);

void FormExtendedFrame_float2(short *frame, float *old_data, float *extended_frame, size_t frame_size, size_t fftsize);

// Uses overlap-and-add(short data) to produce an output frame(short data).
void OverlapAndAdd_16(short *extended_frame, short *overlap_memory, short *output_frame, short frame_size);

// Uses overlap-and-add(float data) to produce an output frame(short data).
void OverlapAndAdd_float(float *extended_frame, float *overlap_memory, short *output_frame, short frame_size);

// Uses overlap-and-add(float data) to produce an output frame(float data).
void OverlapAndAdd_float2(float *extended_frame, float *overlap_memory, float *output_frame, short frame_size);

#endif

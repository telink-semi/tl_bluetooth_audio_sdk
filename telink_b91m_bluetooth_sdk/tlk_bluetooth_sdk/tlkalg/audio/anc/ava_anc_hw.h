/*******************************************************************************
 * @file    ava_anc_hw.h
 * @version TL-7519.5.2
 *
 * @brief   This is the header file for AVAtronics Adaptive ANC algorithm developed solely for Telink TL-7519
 *
 * 
 * @author  Xi Chen, AVAtronics SA
 * @date    05 Dec 2025
 *
 * @par     Copyright (c) 2025, AVAtronics SA
 *
 *          Licensed under the Partnership Agreement between AVAtronics ("Licensor") and TELINK Semiconductor (Shanghai) ("Licensee")
 *          This SW is provided to be integrated by the Licensee in its SDK and can be shared with its customers for development
 *          and test of Adaptive ANC developed for Licensee.
 *			Licensee has full irrevocable license globally to use this SW in its SDK for TL-7519.
 *          This SW cannot be used on any other device else than TL-7519.
 *
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 *
 *          3rd Party Licenses:
 *          The algorithms and their implementations are done exclusively by the Licensor team without using any third party
 *          algorithm, license or SW; using NatureDSP Library for HiFi5 DSP cores.
 *			The information on the license for NatureDSP Library:
 *			Name: NatureDSP Library version 5.0.0
 *			Type: Proprietary License, for use only with Cadence processor cores
 *			Copyright information: Copyright (c) 2020-2025 Cadence Design Systems, Inc.; Copyright (c) 2009-2020 IntegIT, Limited.
 *			Source Code Download URL: https://github.com/foss-xtensa/ndsplib-hifi5
 *
 *
 ******************************************************************************/

/*******************************************************************************
 * This file contains the WZ/CZ IIR/FIR coefficients for AVA HW ANC.
 * The filters work in the HB mode at 768kHz sampling rate.
 * The filters should be configured as follows:
 * 
 * audio_anc_set_wz_iir_taps(AUDIO_ANC_CHANNEL, 5);
 * audio_anc_update_wz_iir_coef(AUDIO_ANC_CHANNEL, ava_wz_iir, 5);
 * audio_anc_set_wz_fir_taps(AUDIO_ANC_CHANNEL, 390);
 * audio_anc_update_wz_fir_coef(AUDIO_ANC_CHANNEL, ava_wz_fir, 390);
 * 
 * audio_anc_set_cz_iir_taps(AUDIO_ANC_CHANNEL, 0, 5);
 * audio_anc_update_cz_iir_coef(AUDIO_ANC_CHANNEL, 0, ava_cz_iir, 5);
 * audio_anc_set_cz_fir_taps(AUDIO_ANC_CHANNEL, 0, 150);
 * audio_anc_update_cz_fir_coef(AUDIO_ANC_CHANNEL, 0, ava_cz_fir, 150);
 * 
 * audio_anc_set_ref_mic_gain(AUDIO_ANC_CHANNEL, 0x4000, 14);
 * audio_anc_set_wz_fir_gain(AUDIO_ANC_CHANNEL, 0x4000, 14);
 ******************************************************************************/

#ifndef AVA_ANC_HW_H_
#define AVA_ANC_HW_H_

extern int ava_wz_iir[24][5];

extern signed short ava_wz_fir[390];

extern signed short ava_wz_fir_trans[390];

extern int ava_cz_iir[24][5];

extern signed short ava_cz_fir[150];

#endif /* AVA_ANC_HW_H_ */

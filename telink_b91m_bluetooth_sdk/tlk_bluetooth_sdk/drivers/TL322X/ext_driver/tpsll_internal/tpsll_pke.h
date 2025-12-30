/********************************************************************************************************
 * @file    tpsll_pke.h
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
#ifndef TLKDRV_TL322X_EXT_DRIVER_TPSLL_PKE_H_
#define TLKDRV_TL322X_EXT_DRIVER_TPSLL_PKE_H_

#define pke_clr_irq_status    pke_clear_interrupt
#define pke_get_irq_status    pke_wait_till_done
#define pke_opr_start         pke_start
#define pke_mod_add           pke_modadd
#define pke_mod_sub           pke_modsub
#define pke_mod_mul           pke_modmul
#define pke_mod_inv           pke_modinv
#define div2n_u32             Big_Div2n
#define sub_u32               pke_sub
#define pke_eccp_point_mul    eccp_pointMul
#define pke_eccp_point_add    eccp_pointAdd
#define pke_eccp_point_verify eccp_pointVerify
#define pke_x25519_point_mul  x25519_pointMul
#define pke_ed25519_point_mul ed25519_pointMul
#define pke_ed25519_point_add ed25519_pointAdd


#endif /* TLKDRV_TL322X_EXT_DRIVER_TPSLL_PKE_H_ */

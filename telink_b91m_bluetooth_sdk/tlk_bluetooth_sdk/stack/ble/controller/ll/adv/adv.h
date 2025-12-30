/********************************************************************************************************
 * @file    adv.h
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
#ifndef ADV_H_
#define ADV_H_

/**
 *  @brief  Definition for ADV maximum random delay time
 */
typedef enum
{
    MAX_DELAY_10MS  = 128, //SCHEDULER_USE_STIMER_TICK_REPLACE_BIG_SMALL_SLOT
    MAX_DELAY_5MS   = 64,
    MAX_DELAY_2P5MS = 32,
    MAX_DELAY_0MS   = 0,
} adv_max_delay_t;

/**
 * @brief   Set Advertising Interval maximum random delay time. If not set, default value is 10mS according to BLE SPEC.
 *          This API is mainly used for debug, to control adv_interval, this value will recover to 10mS if stack timing is limited
 *          attention: If this API is used, must be called before API "blc ll_setAdvParam" when initialization !!!
 * @param[in]   max_delay - select one from enumeration variable "adv_max_delay_t", other value invalid
 *
 * @return  None
 */
void blc_ll_setMaxAdvDelay_for_AdvEvent(adv_max_delay_t max_delay);


#endif /* ADV_H_ */

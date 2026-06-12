/********************************************************************************************************
 * @file    btp_adapt.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#if (TLK_STK_BT_ENABLE)

int btp_timer_create(TlkApiTimer_t *pTimer, uint32_t periodUs, uint32_t autoReload, TlkApiTimerCB_t timerCB, void *userArg)
{
    return tlksys_timer_createStatic(TLKSYS_TASKID_HOST, pTimer, periodUs, autoReload, timerCB, userArg);
}

int btp_timer_start(TlkApiTimerHandle_t pTimer)
{
    return tlksys_timer_reStart(TLKSYS_TASKID_HOST, pTimer);
}

int btp_timer_stop(TlkApiTimerHandle_t pTimer)
{
    return tlksys_timer_stop(TLKSYS_TASKID_HOST, pTimer);
}


#endif // #if (TLK_STK_BT_ENABLE)

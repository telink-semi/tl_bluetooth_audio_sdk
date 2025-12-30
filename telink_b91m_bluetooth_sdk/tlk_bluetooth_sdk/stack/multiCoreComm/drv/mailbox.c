/********************************************************************************************************
 * @file    mailbox.c
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
#include "drivers.h"
#include "platform/tlkipc_hal.h"
#include "platform/tlkipc_port.h"
#include "mailbox.h"

#if (MCU_DUAL_CORE_ENABLE)

void tlkipc_mailbox_init(void)
{
	tlkipc_hal_mailbox_init();
}

_attribute_ram_code_sec_ void tlkipc_mailbox_send_data(uint8_t* data, uint32_t maxBlockTimeUs)
{
	if(maxBlockTimeUs > MAILBOX_TX_MAX_BLOCK_TIME_US){
		maxBlockTimeUs = MAILBOX_TX_MAX_BLOCK_TIME_US;
	}
	uint32_t clockTime = clock_time();
	do{
		if(tlkipc_hal_is_mailbox_busy()){
			continue;
		}
		uint32_t critical = tlkipc_enterCritical();
		if(tlkipc_hal_is_mailbox_busy()){
			tlkipc_leaveCritical(critical);
			continue;
		}
		tlkipc_hal_mailbox_send(data,8);
		tlkipc_leaveCritical(critical);
		break;
	}while(!clock_time_exceed(clockTime, maxBlockTimeUs));
}



#endif

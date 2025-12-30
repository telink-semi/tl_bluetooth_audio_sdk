/********************************************************************************************************
 * @file    tlkdrv_rtc.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "tl_common.h"
#if TLK_DEV_RTC_ENABLE	
#include "drivers.h"
#include <time.h>

typedef struct 
{
	uint32_t lastTick;
	uint32_t sumTick;
	TlkApiTimer_t timer;
}tlkdrv_rtc_ctrl_t;

#if (MCU_CORE_TYPE == MCU_CORE_TL751X)
#define RTC_AREG1 0x36
#define RTC_AREG2 0x37
#define RTC_AREG3 0x38
#define RTC_AREG4 0x3b
#define POWER_STA_REG  0x3a

#define TS_AREG1 0x3c  //gpio group D&E
#define TS_AREG2 0x58
#define TS_AREG3 0x59
#define TS_AREG4 0x5a

#elif (MCU_CORE_TYPE == MCU_CORE_TL721X)
#define RTC_AREG1 54
#define RTC_AREG2 55
#define RTC_AREG3 56
#define RTC_AREG4 57


#define TS_AREG1 59
#define TS_AREG2 60
#define TS_AREG3 0x7a
#define TS_AREG4 0x7b
#endif


static tlkdrv_rtc_ctrl_t stlkdrvRtcCtrl = {0}; 

static inline void tlkdrv_rtc_timestamp_write(uint32_t ts)
{
	unsigned int r  = core_interrupt_disable();
	analog_write_reg8(RTC_AREG1, ts);
	analog_write_reg8(RTC_AREG2, ts >> 8);
	analog_write_reg8(RTC_AREG3, ts >> 16);
	analog_write_reg8(RTC_AREG4, ts >> 24);
	core_restore_interrupt(r);
}

static inline uint32_t tlkdrv_rtc_timestamp_read(void)
{
	unsigned int r  = core_interrupt_disable();
	uint32_t ret = analog_read_reg8(RTC_AREG1) |(analog_read_reg8(RTC_AREG2)<<8) |(analog_read_reg8(RTC_AREG3)<<16) |(analog_read_reg8(RTC_AREG4) << 24);
	core_restore_interrupt(r);
	return ret;
}

static void tlkdrv_rtc_last32kTimerWrite(uint32_t ts)
{
	analog_write_reg8(TS_AREG1, ts);
	analog_write_reg8(TS_AREG2, ts >> 8);
	analog_write_reg8(TS_AREG3, ts >> 16);
	analog_write_reg8(TS_AREG4, ts >> 24);
}

static uint32_t tlkdrv_rtc_updateTime(void)
{
	unsigned int r  = core_interrupt_disable();
	uint32_t nowTick = stimer_get_tick();
	stlkdrvRtcCtrl.sumTick += nowTick - stlkdrvRtcCtrl.lastTick;
	stlkdrvRtcCtrl.lastTick = nowTick;
	uint32_t pastTimeS = stlkdrvRtcCtrl.sumTick / SYSTEM_TIMER_TICK_1S;
	stlkdrvRtcCtrl.sumTick -= pastTimeS * SYSTEM_TIMER_TICK_1S;
	uint32_t rtcTime = tlkdrv_rtc_timestamp_read();
	rtcTime += pastTimeS;
	tlkdrv_rtc_timestamp_write(rtcTime);
	core_restore_interrupt(r);
	return rtcTime;
}

void tlkdrv_rtc_timerTask(TlkApiTimerHandle_t handle, void* userArg)
{
	(void) handle;
	(void) userArg;
	tlkdrv_rtc_updateTime();
}

void tlkdrv_rtc_init(void)
{
	stlkdrvRtcCtrl.lastTick = stimer_get_tick();
	tlksys_timer_createStatic(TLKSYS_TASKID_SYSTEM, &stlkdrvRtcCtrl.timer, 10 * 1000 * 1000, true, tlkdrv_rtc_timerTask, NULL);
	tlksys_timer_start(TLKSYS_TASKID_SYSTEM, &stlkdrvRtcCtrl.timer);
#if MCU_CORE_TYPE == MCU_CORE_TL751X
	uint8_t first_poweron = !analog_read_reg8(POWER_STA_REG);
	analog_write_reg8(POWER_STA_REG, 0x01);
#elif MCU_CORE_TYPE == MCU_CORE_TL721X
	uint8_t first_poweron = g_pm_status_info.mcu_status == MCU_POWER_ON ? 1 : 0;
#endif
	if(!first_poweron){
		return;
	}
#if MCU_CORE_TYPE == MCU_CORE_TL751X
	analog_write_reg8(0x3a, 0x01);	
#endif
	
	struct tm tm={
		.tm_hour= 0,
		.tm_mday= 1,
		.tm_mon= 7,
		.tm_year= 2025-1900,
	};
	uint32_t timstamp = (uint32_t)mktime(&tm);
	tlkdrv_rtc_timestamp_write(timstamp);	
}

const struct tm *tlkdrv_rtc_getTime(int32_t timeZone)
{
	uint32_t timstamp = tlkdrv_rtc_updateTime() + timeZone * 3600;
	struct tm *ret = gmtime((time_t *)&timstamp);
	return ret;
}

uint32_t tlkdrv_rtc_getTimeStamp(int32_t timeZone)
{
	return tlkdrv_rtc_updateTime() + timeZone * 3600;
}

void tlkdrv_rtc_setTimeStamp(uint32_t time)
{
	unsigned int r = core_interrupt_disable();
	tlkdrv_rtc_timestamp_write(time);
	stlkdrvRtcCtrl.lastTick = stimer_get_tick();
	stlkdrvRtcCtrl.sumTick = 0;
	core_restore_interrupt(r);
}

void tlkdrv_rtc_setTime(struct tm *time)
{
	uint32_t timstamp = (uint32_t)mktime(time);
	tlkdrv_rtc_setTimeStamp(timstamp);
}

void tlkdrv_rtc_powerOffSave(void)
{
	unsigned int r = core_interrupt_disable();
	tlkdrv_rtc_updateTime();
#if MCU_CORE_TYPE == MCU_CORE_TL751X
	uint32_t us = (1 * SYSTEM_TIMER_TICK_1S - stlkdrvRtcCtrl.sumTick) / SYSTEM_TIMER_TICK_1US;
	us = min(us,1 * 1000 * 1000);
	delay_us(us);//delay until 1s   
	//example1: sumTick(remain_time):300ms->delay700ms  
	//example2: sumTick(remain_time):450ms->delay550ms  
	tlkdrv_rtc_timestamp_write( tlkdrv_rtc_timestamp_read() + 1);
#endif
	tlkdrv_rtc_last32kTimerWrite(clock_get_32k_tick());
	core_restore_interrupt(r);
}

const struct tm *tlkfs_diskio_getTime(void)
{
	return tlkdrv_rtc_getTime(+8);
}

#endif






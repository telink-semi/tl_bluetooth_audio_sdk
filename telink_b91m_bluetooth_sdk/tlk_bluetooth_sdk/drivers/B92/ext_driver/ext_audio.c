/********************************************************************************************************
 * @file    ext_audio.c
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
#include "ext_audio.h"
#include "stdlib.h"
#if 0
    #include "application/usbstd/usb.h"
    #include "application/app/usbaud.h"
/////////////////////////////////////
s16     last_speaker_vol=0x7fff;
u8      last_speaker_step=0xff;
////////////////////////////////////
int     usb_vol_music = 0;
int     gradual_vol = 0;
const s16 volume_relative_value_table[] =
{
        0xa000,
        0xbea8,0xc825,0xcdf1,0xd221,0xd568,0xd8a1,0xda64,0xdc61,0xde23,0xdfb7,
        0xe124,0xe273,0xe3a6,0xe4c3,0xe5cd,0xe6c6,0xe7af,0xe88c,0xe95d,0xea23,
        0xeadf,0xeb93,0xec3f,0xece3,0xed81,0xee19,0xeeab,0xef37,0xefbf,0xf043,
        0xf0c1,0xf13c,0xf1b4,0xf227,0xf298,0xf305,0xf36f,0xf3d6,0xf43b,0xf49d,
        0xf4fd,0xf55a,0xf5b5,0xf60f,0xf666,0xf6bb,0xf70e,0xf760,0xf7b0,0xf7fe,
        0xf84b,0xf897,0xf8e0,0xf929,0xf970,0xf9b6,0xf9fb,0xfa3e,0xfa81,0xfac2,
        0xfb02,0xfb41,0xfb7f,0xfbbc,0xfbf9,0xfc34,0xfc6e,0xfca8,0xfce0,0xfd18,
        0xfd4f,0xfd86,0xfdbb,0xfdf0,0xfe24,0xfe58,0xfe8a,0xfebc,0xfeee,0xff1f,
        0xff4f,0xff7f,0xffae,0xffdc,0x0009,0x0037,0x0066,0x0096,0x00c5,0x00f6,
        0x0127,0x0159,0x018b,0x01be,0x01f2,0x0226,0x025c,0x0291,0x02c8,0x0300
};

const u16 volume_scale_table[] =
{
        0,
        55,    58,    62,    65,    69,    73,    78,    82,    87,    92,
        98,    103,   110,   116,   123,   130,   138,   146,   155,   164,
        174,   184,   195,   206,   218,   231,   245,   260,   275,   291,
        309,   327,   346,   367,   389,   412,   436,   462,   489,   518,
        549,   581,   615,   652,   691,   732,   775,   821,   870,   921,
        976,   1034,  1095,  1160,  1229,  1301,  1379,  1460,  1547,  1638,
        1735,  1838,  1947,  2063,  2184,  2314,  2451,  2597,  2751,  2913,
        3086,  3269,  3463,  3668,  3885,  4115,  4359,  4618,  4891,  5181,
        5488,  5813,  6158,  6523,  6909,  7318,  7752,  8211,  8698,  9213,
        9759,  10338, 10951, 11599, 12286, 13014, 13785, 14062, 15467, 16383
};

s16 volume_relative_table_iphone[] =
{
        /*
        0xa000,0xa600,0xac00,0xb200,0xb980,0xbf80,0xc580,0xcb80,0xd180,0xd780,
        0xdd80,0xe380,0xeb00,0xf100,0xf700,0xfd00,0x300
        */
        0xa000,0xa180,0xa300,0xa480,0xa600,0xa780,0xa900,
        0xaa80,0xac00,0xad80,0xaf00,0xb080,0xb200,0xb380,0xb500,0xb680,0xb800,
        0xb980,0xbb00,0xbc80,0xbe00,0xbf80,0xc100,0xc280,0xc400,0xc580,0xc700,
        0xc880,0xca00,0xcb80,0xcd00,0xce80,0xd000,0xd180,0xd300,0xd480,0xd600,
        0xd780,0xd900,0xda80,0xdc00,0xdd80,0xdf00,0xe080,0xe200,0xe380,0xe500,
        0xe680,0xe800,0xe980,0xeb00,0xec80,0xee00,0xef80,0xf100,0xf280,0xf400,
        0xf580,0xf700,0xf880,0xfa00,0xfb80,0xfd00,0xfe80,0x0000,0x0180,0x3000
};

u16 volume_scale_iphone_table[] =
{
        /*
        0,     52,    77,    113,   168,   248,   367,   543,   802,   1187,
        1756,  2597,  3841,  5681,  8403,  12428, 16384
        */
        0,
        55,       58,    62,      69,      78,     87,
        98,      110,    123,    138,    155,   174, 195,    218,    245,    260,
        275,  291, 309,   346,  389,  436,   489,   549,  581,    615,
        652,   732, 691, 775,   821,   870, 921,976,   1034,  1095,
        1160,  1229,  1301,  1379,  1460,  1547,  1638,1756,  2597,   2751,
        2913,3086,  3463,  3841,  4115,  4559,  4918, 5681,  6523,  7318,
        7752,8403,  9213,10338,11599,12428, 13785, 14062, 15467,16384
};

/////////////////////////////////////////////////////////////////////////////////////////////////
u8 blc_get_iphone_volume_scale(s16 vol)
{
    u8 temp = 0xff;
    int i;
    for(i=0;i<67;i++)
    {
        if(vol == volume_relative_table_iphone[i])
        {
            temp = i;
            break;
        }
    }
    return temp;
}

void blc_audio_usb_init(void)
{
    reg_usb_ep6_buf_addr = 0xC0;        // 192 max
    reg_usb_ep7_buf_addr = 0x00;        // 32

    reg_usb_ep_max_size = (192 >> 3);

    usb_set_pin_en();
    usb_init_interrupt();
    core_interrupt_enable();
    plic_interrupt_enable(IRQ_USB_ENDPOINT);        // enable usb endpoint interrupt
    usbhw_set_eps_irq_mask(FLD_USB_EDP7_IRQ);
    usbhw_set_eps_irq_mask(FLD_USB_EDP6_IRQ);

}

blc_audio_ctr_t     audio_ctr = {0};
void blc_audio_clock_calib_init(void)
{
    audio_ctr.cap0                  = analog_read(0x8a);
    audio_ctr.dft_cap               = audio_ctr.cap0 & 0x3f;
    audio_ctr.calib_diff_t          = 0;
    audio_ctr.calib_last_pdu_num    = 0;
    audio_ctr.calib_last_timestamp  = 0;
    audio_ctr.calib_reset_cnt       = 1;
    audio_ctr.interval              = 10*1000*SYSTEM_TIMER_TICK_1US;
}

_attribute_ram_code_ void ble_audio_reset(void){
}

_attribute_ram_code_ void blc_audio_clock_calib2(u32 timeStamp,u32 baseTick,u32 offset)
{
    (void)offset;
    #define DRIFT_TH2 24

    if(audio_ctr.calib_reset_cnt){
        audio_ctr.calib_reset_cnt--;
        audio_ctr.calib_last_timestamp = timeStamp;
        audio_ctr.calib_last_pdu_num =  baseTick;
    //  my_dump_str_data(USER_DUMP_EN, "calib_rest", 0,0);
        return;
    }

    u16 interval_cnt = (u16)((u16)baseTick - audio_ctr.calib_last_pdu_num);
    if(interval_cnt >=10){
        int diff_r =  (int)(timeStamp - audio_ctr.calib_last_timestamp - ((u32)interval_cnt)*audio_ctr.interval);
        int change = diff_r||audio_ctr.calib_diff_t;

        int span = 0, ppm = 0;
        if( change){
            audio_ctr.calib_diff_t += diff_r;
            if(diff_r){
                if(diff_r){
                    span = (int)(interval_cnt * audio_ctr.interval) / diff_r;
                    ppm = ((int)( 1000000) + abs(span) / 2)  / abs(span);
                }

                if (ppm <= 4)
                {
                    ppm >>= 1;
                }
                else
                {
                    ppm = ppm * 3 / 2;
                }
            }
            else{
                span = audio_ctr.calib_diff_t;
                ppm = 1;
            }



            if (audio_ctr.dft_cap < 0x18)
            {
                ppm = ppm >> 1;
            }

            if (ppm < 2 && abs (audio_ctr.calib_diff_t) >= DRIFT_TH2 * 2)
            {
                ppm = 2;
            }
            else if (ppm < 1)
            {
                ppm = 1;
            }
            else if (ppm > 4)
            {
                ppm = 4;
            }
            if((ppm>2) && (abs(audio_ctr.calib_diff_t) > DRIFT_TH2*2) && ((audio_ctr.calib_diff_t * span) <0)){
                ppm >>=1;
            }


            if (span < 0)
            {
                ppm = -ppm;
            }

            int cap  = (int)audio_ctr.dft_cap + ppm;
            if (cap > 0x3f)
            {
                audio_ctr.dft_cap = 0x3f;
            }
            else if (cap < 0x02)
            {
                audio_ctr.dft_cap = 0x02;
            }
            else
            {
                audio_ctr.dft_cap = cap;
            }

            analog_write_reg8(0x8a, audio_ctr.dft_cap | (audio_ctr.cap0 & 0xc0));
//          my_dump_str_u32s (1, "cap adjust", offset , audio_ctr.dft_cap,audio_ctr.calib_diff_t,diff_r);
            audio_ctr.calib_last_pdu_num = baseTick;
            audio_ctr.calib_last_timestamp = timeStamp;
            diff_r = 0;
        }
    }
}

_attribute_ram_code_ void blc_audio_clock_calib(u32 timeStamp,u32 baseTick)
{
    static u32 timeBase = 0;
    static int tickTraceTotal = 0; //total
    static int tickTraceTemp = 0;  //temporary
    static u32 adjustCnt = 0;
    if(timeBase == 0)
    {
//      tlk_printf("cap start");
        timeBase = timeStamp;
        return;
    }
    timeBase = timeBase + baseTick;
    if(abs((int)(timeStamp-timeBase))>500*16)//500us
    {
//       tlk_printf("packet loss,return");
         timeBase = timeStamp;
         tickTraceTotal = 0;
         tickTraceTemp = 0;
         adjustCnt = 0;
         return;
    }
    tickTraceTotal += (int)(timeStamp - (u32)timeBase);//timeTrace means time variation
    tickTraceTemp  += (int)(timeStamp - (u32)timeBase);
    adjustCnt++;
    timeBase = timeStamp;
    if(adjustCnt < 10)
    {
        return;
    }
    else
    {

//      tlk_printf("time trace total %d",tickTraceTotal);
//      tlk_printf("time trace temp %d",tickTraceTemp);
        if(tickTraceTotal < -32 && tickTraceTemp < 0)//
        {
            u8 capChip = analog_read_reg8(0x8a)&0x3f;
            tickTraceTemp = 0;
            if(capChip <0x0a)
            {
                return;
            }
//          tlk_printf("capChip0 -- %x",capChip);
            u8 set_cap = (--capChip)|0x40;
            analog_write_reg8(0x8a, set_cap);
        }
        else if(tickTraceTotal > 32 && tickTraceTemp > 0)
        {
            u8 capChip = analog_read_reg8(0x8a)&0x3f;
            tickTraceTemp = 0;
            if(capChip > 0x35)
            {
                return;
            }
//          tlk_printf("capChip1 ++ %x",capChip);
            u8 set_cap = (++capChip)|0x40;
            analog_write_reg8(0x8a, set_cap);
        }
        adjustCnt = 0;
        tickTraceTemp = 0;
    }
}

void blc_usb_adjust_volume(u16 * pcmData,u16 len)
{
    u32 index;
    u32 i;

    // noise cancel for USB voluem changing.
    if(gradual_vol != usb_vol_music)
    {
        if(gradual_vol > usb_vol_music)
        {
            usb_vol_music += 100;
            if(usb_vol_music > gradual_vol)
            {
                usb_vol_music = gradual_vol;
            }
        }
        else
        {
            usb_vol_music -= 100;
            if(usb_vol_music < gradual_vol)
            {
                usb_vol_music = gradual_vol;
            }
        }
    }

    s16 vol_temp = (s16)usbaud_speaker_curvol_get();
    u8 step_temp = usbaud_speaker_vol_get();

    if((last_speaker_vol!=vol_temp) || (last_speaker_step!=step_temp))
    {
        if(step_temp&0x80)
        {
            usb_vol_music = 0;
            gradual_vol=0;
        }
        else
        {
            index = blc_get_iphone_volume_scale(vol_temp);
            if(index >=67)
            {
                for(i=0;i<sizeof(volume_relative_value_table)/2;i++)
                {
                    if(vol_temp <=volume_relative_value_table[i])
                    {
                        break;
                    }
                }
                if(i>100)
                {
                    i = 100;
                }
                gradual_vol = volume_scale_table[i];
            }
            else
            {
                gradual_vol = volume_scale_iphone_table[index];
            }
        }
        last_speaker_vol = vol_temp;
        last_speaker_step = step_temp;
    }

    for (i=0; i<len; i++)
    {
        short int temp1;
        temp1 = (s16)pcmData[i];
        temp1 = ((temp1 * usb_vol_music) + 0x2000) >> 14;
        pcmData[i] = (unsigned short)temp1;
    }
}

void audio_set_codec_dac_gain(u8 state)
{
    u8 aGain = 12 - 12*state/255;
    (void)aGain; //unused, remove warning
//  audio_set_codec_dac_a_gain(aGain);
//  audio_set_codec_dac_d_gain(audio_i2s_codec_config.out_digital_gain);
//  audio_set_codec_dac_a_gain(audio_i2s_codec_config.out_analog_gain);
}


void ble_audio_timer_init(timer_type_e timer)
{
    timer_type_e timer_type = (timer?IRQ_TIMER1:IRQ_TIMER0);
    plic_interrupt_enable(timer_type);
    plic_set_priority(timer_type, 1);
}

_attribute_ram_code_ void ble_audio_timer_set_capture(timer_type_e timer,unsigned int init_tick,unsigned int cap_tick)
{
    timer_set_init_tick(timer,init_tick);
    timer_set_cap_tick(timer,cap_tick);
    timer_set_mode(timer, TIMER_MODE_SYSCLK);
    timer_start(timer);
}
#endif

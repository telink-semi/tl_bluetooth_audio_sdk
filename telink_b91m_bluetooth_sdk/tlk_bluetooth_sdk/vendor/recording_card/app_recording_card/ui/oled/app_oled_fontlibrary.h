/********************************************************************************************************
 * @file    app_oled_fontlibrary.h
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
#ifndef TLK_OLED_FONTLIBRARY_H
#define TLK_OLED_FONTLIBRARY_H


/* Character Set Definition */
/* Only one of the following two macro definitions should be uncommented */
//#define OLED_CHARSET_UTF8     // Define character set as UTF8
#define OLED_CHARSET_GB2312 // Define character set as GB2312

/* Font Basic Unit */
typedef struct
{
#ifdef OLED_CHARSET_UTF8 // Define character set as UTF8
    char Index[5];       // Chinese character index, 5 bytes space
#endif

#ifdef OLED_CHARSET_GB2312 // Define character set as GB2312
    char Index[3];         // Chinese character index, 3 bytes space
#endif

    uint8_t Data[32]; // Font data
} ChineseCell_t;

/* ASCII Font Data Declaration */
extern const uint8_t OLED_F8x16[][16];
extern const uint8_t OLED_F6x8[][6];

/* Chinese Character Font Data Declaration */
extern const ChineseCell_t OLED_CF16x16[];


#endif
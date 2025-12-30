/********************************************************************************************************
 * @file    hci_cmd.h
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
#ifndef HCI_CMD_H_
#define HCI_CMD_H_

/**
 *  @brief  Command Parameters for "7.1.1 Inquiry command"
 */

/* Inquiry_Length, Maximum amount of time specified before the Inquiry is halted,
 * Range: 0x01 to 0x30, Time = N * 1.28 s, Range: 1.28 to 61.44 s
 * Notice that these are just part of but not all Inquiry_Length value */
typedef enum
{
    INQUIRY_LENGTH_1S28  = 1,
    INQUIRY_LENGTH_2S56  = 2,
    INQUIRY_LENGTH_3S84  = 3,
    INQUIRY_LENGTH_5S12  = 4,
    INQUIRY_LENGTH_6S4   = 5,
    INQUIRY_LENGTH_7S68  = 6,
    INQUIRY_LENGTH_8S96  = 7,
    INQUIRY_LENGTH_10S24 = 8,
    INQUIRY_LENGTH_11S52 = 9,
    INQUIRY_LENGTH_12S8  = 10,
    INQUIRY_LENGTH_20S48 = 16,
    INQUIRY_LENGTH_25S6  = 20,
    INQUIRY_LENGTH_30S72 = 24,
    INQUIRY_LENGTH_40S96 = 32,
    INQUIRY_LENGTH_51S2  = 40,
    INQUIRY_LENGTH_61S44 = 48,
} inq_len_t;

/**
 *  @brief  Command Parameters for "7.3.16 Write Page Timeout command"
 */

/* Page_Timeout, Page Timeout measured in number of Baseband slots.
 * Interval Length = N * 0.625 ms (1 Baseband slot),Range: 0x0001 to 0xFFFF, Time Range: 0.625 ms to 40.9 s
 * Notice that these are just part of but not all Page_Timeout value */
typedef enum
{
    PAGE_TIMEOUT_0_625_MS = 1,
    PAGE_TIMEOUT_1_25_MS  = 2,
    PAGE_TIMEOUT_1_875_MS = 3,
    PAGE_TIMEOUT_2MS5     = 4,
    PAGE_TIMEOUT_3_125MS  = 5,
    PAGE_TIMEOUT_3_75MS   = 6,
    PAGE_TIMEOUT_10MS     = 16,
    PAGE_TIMEOUT_15MS     = 24,
    PAGE_TIMEOUT_20MS     = 32,
    PAGE_TIMEOUT_25MS     = 40,
    PAGE_TIMEOUT_30MS     = 48,
    PAGE_TIMEOUT_35MS     = 56,
    PAGE_TIMEOUT_40MS     = 64,
    PAGE_TIMEOUT_45MS     = 72,
    PAGE_TIMEOUT_50MS     = 80,
    PAGE_TIMEOUT_60MS     = 96,
    PAGE_TIMEOUT_70MS     = 112,
    PAGE_TIMEOUT_80MS     = 128,
    PAGE_TIMEOUT_90MS     = 144,
    PAGE_TIMEOUT_100MS    = 160,
    PAGE_TIMEOUT_150MS    = 240,
    PAGE_TIMEOUT_200MS    = 320,
    PAGE_TIMEOUT_250MS    = 400,
    PAGE_TIMEOUT_300MS    = 480,
    PAGE_TIMEOUT_350MS    = 560,
    PAGE_TIMEOUT_400MS    = 640,
    PAGE_TIMEOUT_450MS    = 720,
    PAGE_TIMEOUT_500MS    = 800,
    PAGE_TIMEOUT_600MS    = 960,
    PAGE_TIMEOUT_700MS    = 1120,
    PAGE_TIMEOUT_800MS    = 1280,
    PAGE_TIMEOUT_900MS    = 1440,
    PAGE_TIMEOUT_1S       = 1600,
    PAGE_TIMEOUT_1S28     = 2048,
    PAGE_TIMEOUT_2S       = 3200,
    PAGE_TIMEOUT_2S56     = 4096,
    PAGE_TIMEOUT_3S       = 4800, //0x12C0
    PAGE_TIMEOUT_4S       = 6400, //0x1900
    PAGE_TIMEOUT_5S       = 8000,
    PAGE_TIMEOUT_5S12     = 8192,
    PAGE_TIMEOUT_6S       = 9600,
    PAGE_TIMEOUT_6S40     = 10624,
    PAGE_TIMEOUT_7S       = 11200,
    PAGE_TIMEOUT_8S       = 12800,
    PAGE_TIMEOUT_9S       = 14400,
    PAGE_TIMEOUT_10S      = 16000,
    PAGE_TIMEOUT_10S68    = 17088, //0x42C0
    PAGE_TIMEOUT_15S      = 24000,
    PAGE_TIMEOUT_20S      = 32000,
    PAGE_TIMEOUT_25S      = 40000,
    PAGE_TIMEOUT_30S      = 48000,
    PAGE_TIMEOUT_35S      = 56000,
    PAGE_TIMEOUT_40S      = 64000,
} page_tm_t;

/**
 *  @brief  Command Parameters for "7.3.20 Write Page Scan Activity command"
 */

/* Page_Scan_Interval, Time = N * 0.625 ms, Time Range: 11.25 to 2560 ms,
 * Notice that these are just part of but not all Page_Scan_Interval value */
typedef enum
{
    PAGESCAN_INR_11MS25 = 18,
    PAGESCAN_INR_12MS5  = 20,
    PAGESCAN_INR_15M    = 24,
    PAGESCAN_INR_17MS5  = 28,
    PAGESCAN_INR_20MS   = 32,
    PAGESCAN_INR_25MS   = 40,
    PAGESCAN_INR_30MS   = 48,
    PAGESCAN_INR_35MS   = 56,
    PAGESCAN_INR_40MS   = 64,
    PAGESCAN_INR_45MS   = 72,
    PAGESCAN_INR_50MS   = 80,
    PAGESCAN_INR_60MS   = 96,
    PAGESCAN_INR_62MS5  = 100,
    PAGESCAN_INR_70MS   = 112,
    PAGESCAN_INR_75MS   = 120,
    PAGESCAN_INR_80MS   = 128,
    PAGESCAN_INR_90MS   = 144,
    PAGESCAN_INR_100MS  = 160,
    PAGESCAN_INR_150MS  = 240,
    PAGESCAN_INR_200MS  = 320,
    PAGESCAN_INR_225MS  = 360,
    PAGESCAN_INR_250MS  = 400,
    PAGESCAN_INR_300MS  = 480,
    PAGESCAN_INR_350MS  = 560,
    PAGESCAN_INR_400MS  = 640,
    PAGESCAN_INR_450MS  = 720,
    PAGESCAN_INR_500MS  = 800,
    PAGESCAN_INR_600MS  = 960,
    PAGESCAN_INR_700MS  = 1120,
    PAGESCAN_INR_750MS  = 1200,
    PAGESCAN_INR_800MS  = 1280,
    PAGESCAN_INR_900MS  = 1440,
    PAGESCAN_INR_1000MS = 1600,
    PAGESCAN_INR_1280MS = 2048,
    PAGESCAN_INR_1500MS = 2400,
    PAGESCAN_INR_2000MS = 3200,
    PAGESCAN_INR_2560MS = 4096,
} pagescan_inr_t;

/* Page_Scan_Window, Time = N * 0.625 ms, Time Range: 10.625 to 2560 ms,
 * Notice that these are just part of but not all Page_Scan_Window value */
typedef enum
{
    PAGESCAN_WIN_10MS625  = 17,
    PAGESCAN_WIN_11MS25   = 18,
    PAGESCAN_WIN_12MS5    = 20,
    PAGESCAN_WIN_15M      = 24,
    PAGESCAN_WIN_17MS5    = 28,
    PAGESCAN_WIN_20MS     = 32,
    PAGESCAN_WIN_25MS     = 40,
    PAGESCAN_WIN_30MS     = 48,
    PAGESCAN_WIN_35MS     = 56,
    PAGESCAN_WIN_40MS     = 64,
    PAGESCAN_WIN_45MS     = 72,
    PAGESCAN_WIN_50MS     = 80,
    PAGESCAN_WIN_60MS     = 96,
    PAGESCAN_WIN_62MS5    = 100,
    PAGESCAN_WIN_70MS     = 112,
    PAGESCAN_WIN_75MS     = 120,
    PAGESCAN_WIN_80MS     = 128,
    PAGESCAN_WIN_90MS     = 144,
    PAGESCAN_WIN_100MS    = 160,
    PAGESCAN_INR_143MS125 = 229,
    PAGESCAN_WIN_150MS    = 240,
    PAGESCAN_WIN_200MS    = 320,
    PAGESCAN_WIN_250MS    = 400,
    PAGESCAN_WIN_300MS    = 480,
    PAGESCAN_WIN_350MS    = 560,
    PAGESCAN_WIN_400MS    = 640,
    PAGESCAN_WIN_450MS    = 720,
    PAGESCAN_WIN_500MS    = 800,
    PAGESCAN_WIN_600MS    = 960,
    PAGESCAN_WIN_700MS    = 1120,
    PAGESCAN_WIN_750MS    = 1200,
    PAGESCAN_WIN_800MS    = 1280,
    PAGESCAN_WIN_900MS    = 1440,
    PAGESCAN_WIN_1000MS   = 1600,
    PAGESCAN_WIN_1280MS   = 2048,
    PAGESCAN_WIN_1500MS   = 2400,
    PAGESCAN_WIN_2000MS   = 3200,
    PAGESCAN_WIN_2560MS   = 4096,
} pagescan_win_t;

/**
 *  @brief  Command Parameters for "7.3.22 Write Inquiry Scan Activity command"
 */

/* Inquiry_Scan_Interval, Time = N * 0.625 ms, Time Range: 11.25 to 2560 ms,
 * Notice that these are just part of but not all Inquiry_Scan_Interval value */
typedef enum
{
    INQSCAN_INR_11MS25   = 18,
    INQSCAN_INR_12MS5    = 20,
    INQSCAN_INR_15M      = 24,
    INQSCAN_INR_17MS5    = 28,
    INQSCAN_INR_20MS     = 32,
    INQSCAN_INR_25MS     = 40,
    INQSCAN_INR_30MS     = 48,
    INQSCAN_INR_35MS     = 56,
    INQSCAN_INR_40MS     = 64,
    INQSCAN_INR_45MS     = 72,
    INQSCAN_INR_50MS     = 80,
    INQSCAN_INR_60MS     = 96,
    INQSCAN_INR_62MS5    = 100,
    INQSCAN_INR_70MS     = 112,
    INQSCAN_INR_75MS     = 120,
    INQSCAN_INR_80MS     = 128,
    INQSCAN_INR_90MS     = 144,
    INQSCAN_INR_100MS    = 160,
    INQSCAN_INR_107MS5   = 172,
    INQSCAN_INR_143MS125 = 229,
    INQSCAN_INR_150MS    = 240,
    INQSCAN_INR_200MS    = 320,
    INQSCAN_INR_225MS    = 360,
    INQSCAN_INR_250MS    = 400,
    INQSCAN_INR_300MS    = 480,
    INQSCAN_INR_350MS    = 560,
    INQSCAN_INR_400MS    = 640,
    INQSCAN_INR_450MS    = 720,
    INQSCAN_INR_500MS    = 800,
    INQSCAN_INR_600MS    = 960,
    INQSCAN_INR_700MS    = 1120,
    INQSCAN_INR_750MS    = 1200,
    INQSCAN_INR_800MS    = 1280,
    INQSCAN_INR_900MS    = 1440,
    INQSCAN_INR_1000MS   = 1600,
    INQSCAN_INR_1280MS   = 2048,
    INQSCAN_INR_1500MS   = 2400,
    INQSCAN_INR_2000MS   = 3200,
    INQSCAN_INR_2560MS   = 4096,
} inqscan_inr_t;

/* Inquiry_Scan_Window, Time = N * 0.625 ms, Time Range: 10.625 to 2560 ms,
 * Notice that these are just part of but not all Inquiry_Scan_Window value */
typedef enum
{
    INQSCAN_WIN_10MS625 = 17,
    INQSCAN_WIN_11MS25  = 18,
    INQSCAN_WIN_12MS5   = 20,
    INQSCAN_WIN_15M     = 24,
    INQSCAN_WIN_17MS5   = 28,
    INQSCAN_WIN_20MS    = 32,
    INQSCAN_WIN_25MS    = 40,
    INQSCAN_WIN_30MS    = 48,
    INQSCAN_WIN_35MS    = 56,
    INQSCAN_WIN_40MS    = 64,
    INQSCAN_WIN_45MS    = 72,
    INQSCAN_WIN_50MS    = 80,
    INQSCAN_WIN_60MS    = 96,
    INQSCAN_WIN_62MS5   = 100,
    INQSCAN_WIN_70MS    = 112,
    INQSCAN_WIN_75MS    = 120,
    INQSCAN_WIN_80MS    = 128,
    INQSCAN_WIN_90MS    = 144,
    INQSCAN_WIN_100MS   = 160,
    INQSCAN_WIN_150MS   = 240,
    INQSCAN_WIN_200MS   = 320,
    INQSCAN_WIN_250MS   = 400,
    INQSCAN_WIN_300MS   = 480,
    INQSCAN_WIN_350MS   = 560,
    INQSCAN_WIN_400MS   = 640,
    INQSCAN_WIN_450MS   = 720,
    INQSCAN_WIN_500MS   = 800,
    INQSCAN_WIN_600MS   = 960,
    INQSCAN_WIN_700MS   = 1120,
    INQSCAN_WIN_750MS   = 1200,
    INQSCAN_WIN_800MS   = 1280,
    INQSCAN_WIN_900MS   = 1440,
    INQSCAN_WIN_1000MS  = 1600,
    INQSCAN_WIN_1280MS  = 2048,
    INQSCAN_WIN_1500MS  = 2400,
    INQSCAN_WIN_2000MS  = 3200,
    INQSCAN_WIN_2560MS  = 4096,
} inqscan_win_t;


#endif /* HCI_CMD_H_ */

/********************************************************************************************************
 * @file    em_map.h
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


#ifndef EM_MAP_H_
#define EM_MAP_H_

/**
 ****************************************************************************************
 * @addtogroup EM EM
 * @ingroup IP
 * @brief Mapping of the different common area in the exchange memory
 *
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rwip_config.h"
#include "rwbt_config.h"
#if(CHIP_TYPE==CHIP_TYPE_TL752X)

#include "../../common/co_math.h"


/// Retrieve Exchange memory address to set into HW interface
#define REG_EM_ADDR_GET(elem, idx) (REG_EM_##elem##_ADDR_GET(idx) >> 2)
/// Retrieve Exchange index from address load from  HW interface
#define REG_EM_IDX_GET(elem, addr) ((((addr)<<2) - (EM_##elem##_OFFSET))/(REG_EM_##elem##_SIZE))

/// Exchange memory base address
#define EM_BASE_ADDR         REG_EM_ET_BASE_ADDR

/// Null pointer in EM offset space
#define EM_PTR_NULL              (0x0000)
#endif

/*
 ****************************************************************************************
 **********************              Common EM part                **********************
 ****************************************************************************************
 */
/// Start of the common EM part
#define EM_COMMON_OFFSET (0)

/*
 * EXCHANGE TABLE
 ****************************************************************************************
 */

/// Exchange table area definition
#define REG_EM_ET_SIZE_LCL 16
#define EM_EXCH_TABLE_LEN  16
#define EM_ET_OFFSET       (EM_COMMON_OFFSET)
#define EM_ET_END          (EM_ET_OFFSET + EM_EXCH_TABLE_LEN * REG_EM_ET_SIZE_LCL)

/**
 * MODEn[3:0]
 *
 *  0x0: No mode selected, nothing to be performed
 *  0x1: BR/EDR Mode
 *  0x2: BLE Mode
 *  0x3-0xF: Reserved for future use           -
 */
#define EM_ET_MODE_NONE  0x00
#define EM_ET_MODE_BREDR 0x01
#define EM_ET_MODE_BLE   0x02

/// exchange table entry status
enum em_et_status
{
    /// 000: Exchange Table entry associated event is ready for processing.
    EM_ET_STATUS_READY = 0x00, //!< EM_ET_STATUS_READY
    /// 001: Exchange Table entry is waiting for start (already read by Event Scheduler)
    EM_ET_STATUS_UNDER_PROCESS = 0x01, //!< EM_ET_STATUS_UNDER_PROCESS
    /// 010: Exchange Table entry associated event is started
    EM_ET_STATUS_STARTED = 0x02, //!< EM_ET_STATUS_STARTED
    /// 011: Exchange Table entry associated event is terminated (normal termination)
    EM_ET_STATUS_TERM_NORMAL = 0x03, //!< EM_ET_STATUS_TERM_NORMAL
    /// 100: Exchange Table entry associated event is terminated (abort termination under prio bandwidth)
    EM_ET_STATUS_TERM_ABORT_IN_PRIO_BW = 0x04, //!< EM_ET_STATUS_TERM_ABORT_IN_PRIO_BW
    /// 101: Exchange Table entry associated event is terminated (abort termination after prio bandwidth)
    EM_ET_STATUS_TERM_ABORT = 0x05, //!< EM_ET_STATUS_TERM_ABORT
    /// 110: Exchange Table entry associated event is skipped
    EM_ET_STATUS_SKIPPED = 0x06, //!< EM_ET_STATUS_SKIPPED
    /// 111: Reserved for future use
};

/*
 * FREQUENCY TABLE
 ****************************************************************************************
 */

/// Frequency table area definition
#define EM_FT_OFFSET (EM_ET_END)

/// number of frequencies / Depends on RF target
#if defined(CFG_RF_ATLAS)
    /// IcyTRx requires 40 x 32-bit words for Frequency table + 40 byte for VCO sub-band table
    #define EM_RF_FREQ_TABLE_LEN 160
    #define EM_RF_VCO_TABLE_LEN  40
#else
    #if BT_EMB_PRESENT
        /// Ripple/ExtRC requires 80 x 8-bit words for Frequency table / No VCO sub-band table
#if(CHIP_TYPE==CHIP_TYPE_TL752X)
	#define EM_RF_FREQ_TABLE_LEN  80
	#define EM_RF_VCO_TABLE_LEN 0
#else
	#define EM_RF_FREQ_TABLE_LEN 160
	#define EM_RF_VCO_TABLE_LEN  40
#endif
    #endif // BT_EMB_PRESENT
#endif

#define EM_FT_END (EM_FT_OFFSET + (EM_RF_VCO_TABLE_LEN + EM_RF_FREQ_TABLE_LEN) * sizeof(uint8_t))

/*
 * RF SW SPI
 ****************************************************************************************
 */
/// RF SW-Driven SPI transfers area definition
#if defined(CFG_RF_ATLAS)
    /// IcyTYRx (Former Atlas) SW Driven SPI space
    #define EM_RF_SW_SPI_OFFSET   (EM_FT_END)
#if(CHIP_TYPE==CHIP_TYPE_TL752X)

	#define EM_RF_SW_SPI_SIZE_MAX   136
#else
    #define EM_RF_SW_SPI_SIZE_MAX 120 // max reg_burst_wr 0x70+5
#endif
    #define EM_RF_SW_SPI_END      (EM_RF_SW_SPI_OFFSET + EM_RF_SW_SPI_SIZE_MAX)

#elif defined(CFG_RF_BTIPT)
    /// BTIPT (Former Atlas) SW Driven SPI space
    #define EM_RF_SW_SPI_OFFSET   (EM_FT_END)
    #define EM_RF_SW_SPI_SIZE_MAX 100
    #define EM_RF_SW_SPI_END      (EM_RF_SW_SPI_OFFSET + EM_RF_SW_SPI_SIZE_MAX)
#else
    /// Ripple SW Driven SPI space
    /// RF-SW-SPI transfers area definition
    #define EM_RF_SW_SPI_OFFSET   (EM_FT_END)
    #define EM_RF_SW_SPI_SIZE_MAX 8 //6
    #define EM_RF_SW_SPI_END      (EM_RF_SW_SPI_OFFSET + EM_RF_SW_SPI_SIZE_MAX)
#endif


/*
 * RF HW SPI
 ****************************************************************************************
 */
/// RF HW-Driven SPI transfers area definition
#if defined(CFG_RF_ATLAS)
    /// IcyTYRx (Former Atlas) SW Driven SPI space
    #define EM_RF_HW_SPI_OFFSET   (EM_RF_SW_SPI_END)
    #define EM_RF_HW_SPI_SIZE_MAX 100
    #define EM_RF_HW_SPI_END      (EM_RF_HW_SPI_OFFSET + EM_RF_HW_SPI_SIZE_MAX)

#elif defined(CFG_RF_BTIPT)
    /// BTIPT SW Driven SPI space
    #define EM_RF_HW_SPI_OFFSET   (EM_RF_SW_SPI_END)
#if(CHIP_TYPE==CHIP_TYPE_TL752X)
	#define EM_RF_HW_SPI_SIZE_MAX 100
#else
    #define EM_RF_HW_SPI_SIZE_MAX 256
#endif
    #define EM_RF_HW_SPI_END      (EM_RF_HW_SPI_OFFSET + EM_RF_HW_SPI_SIZE_MAX)
#elif defined(CFG_RF_CALYPSO)
  /// CALYPSO RF SW Driven SPI space
  #define EM_RF_HW_SPI_OFFSET     (EM_RF_SW_SPI_END)
  #define EM_RF_HW_SPI_SIZE_MAX   0x100
  #define EM_RF_HW_SPI_END        (EM_RF_HW_SPI_OFFSET + EM_RF_HW_SPI_SIZE_MAX)
#else
    /// Ripple SW Driven SPI space
    /// RF-SW-SPI transfers area definition
    #define EM_RF_HW_SPI_OFFSET   (EM_RF_SW_SPI_END)
    #define EM_RF_HW_SPI_SIZE_MAX 0
    #define EM_RF_HW_SPI_END      (EM_RF_HW_SPI_OFFSET + EM_RF_HW_SPI_SIZE_MAX)
#endif


/// End of the common EM part
#define EM_COMMON_END (EM_RF_HW_SPI_END)

#if (BT_EMB_PRESENT)
    /*
 * ENCRYPTION
 ****************************************************************************************
 */

    /// Encryption area definition
#if(CHIP_TYPE==CHIP_TYPE_TL752X)

	#define EM_ENC_OFFSET     CO_ALIGN4_HI(EM_COMMON_END)
#else
	#define EM_ENC_OFFSET     ((EM_COMMON_END + 1) & (~0x01))
#endif
    #define EM_ENC_IN_OFFSET  (EM_ENC_OFFSET)
    #define EM_ENC_IN_SIZE    (16)
    #define EM_ENC_OUT_OFFSET (EM_ENC_IN_OFFSET + EM_ENC_IN_SIZE)
    #define EM_ENC_OUT_SIZE   (16)
    #define EM_ENC_END        (EM_ENC_OFFSET + EM_ENC_IN_SIZE + EM_ENC_OUT_SIZE)

#endif // (BT_EMB_PRESENT)


/*
 ****************************************************************************************
 **********************                BT EM part                  **********************
 ****************************************************************************************
 */
#if(CHIP_TYPE==CHIP_TYPE_TL752X)
#define EM_BT_OFFSET           CO_ALIGN4_HI(EM_ENC_END)
#else
#define EM_BT_OFFSET (EM_ENC_END)
#endif

/*
 * RF SPI part
 ****************************************************************************************
 */


/// @} IPDEXMEM

#endif // EM_MAP_H_

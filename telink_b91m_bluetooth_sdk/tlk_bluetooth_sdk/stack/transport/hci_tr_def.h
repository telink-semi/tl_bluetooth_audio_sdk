/********************************************************************************************************
 * @file    hci_tr_def.h
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
#ifndef HCI_TR_DEF_H_
#define HCI_TR_DEF_H_

#include "tl_common.h"

/*!  HCI Transmit Type. */
#define HCI_TR_TYPE_NONE  0x00
#define HCI_TR_TYPE_CMD   0x01
#define HCI_TR_TYPE_ACL   0x02
#define HCI_TR_TYPE_SCO   0x03
#define HCI_TR_TYPE_EVENT 0x04
#define HCI_TR_TYPE_ISO   0x05

/*!  HCI Head length define. */
#define HCI_CMD_HEAD_LEN 3
#define HCI_ACL_HEAD_LEN 4
#define HCI_ISO_HEAD_LEN 4
#define HCI_EVT_HEAD_LEN 2
#define HCI_SCO_HEAD_LEN 3


/*! H5 packet length macro. */
#define HCI_H5_HEAD_LEN 4
#define HCI_H5_CRC_LEN  2


/*! Common macro. */
#define BYTES_TO_UINT8(n, p) \
    {                        \
        n = (u8)(p)[0];      \
    }

#define BYTES_TO_UINT16(n, p)                   \
    {                                           \
        n = ((u16)(p)[0] + ((u16)(p)[1] << 8)); \
    }

#define BYTES_TO_UINT32(n, p)                                                               \
    {                                                                                       \
        n = ((u32)(p)[0] + ((u32)(p)[1] << 8) + ((u32)(p)[2] << 16) + ((u32)(p)[3] << 24)); \
    }


#define BSTREAM_TO_UINT8(n, p) \
    {                          \
        BYTES_TO_UINT8(n, p);  \
        p += 1;                \
    }
#define BSTREAM_TO_UINT16(n, p) \
    {                           \
        BYTES_TO_UINT16(n, p);  \
        p += 2;                 \
    }
#define BSTREAM_TO_UINT32(n, p) \
    {                           \
        BYTES_TO_UINT32(n, p);  \
        p += 4;                 \
    }

#define UINT8_TO_BSTREAM(p, n) \
    {                          \
        *(p)++ = (u8)(n);      \
    }
#define UINT16_TO_BSTREAM(p, n)  \
    {                            \
        *(p)++ = (u8)(n);        \
        *(p)++ = (u8)((n) >> 8); \
    }
#define UINT32_TO_BSTREAM(p, n)   \
    {                             \
        *(p)++ = (u8)(n);         \
        *(p)++ = (u8)((n) >> 8);  \
        *(p)++ = (u8)((n) >> 16); \
        *(p)++ = (u8)((n) >> 24); \
    }

#define COUNTOF(x)          (sizeof((x)) / sizeof((x)[0]))

#define MEM_SIZE_ALIGN4(x)  (((x) + 3) & ~3)
#define MEM_SIZE_ALIGN16(x) (((x) + 15) & ~15)

/*! HCI fifo size define. */
#define HCI_TX_FIFO_SIZE 760
#define HCI_TX_FIFO_NUM  8

#define HCI_RX_FIFO_SIZE 760 //HCI_IN_FIFO_MAX_SIZE(max2(ACL_PERIPHR_MAX_TX_OCTETS,ACL_CENTRAL_MAX_TX_OCTETS), HCI_ISO_IN_OCTETS_MAX)
#define HCI_RX_FIFO_NUM  8


/*! HCI ACL data packet max size define. */
#ifndef HCI_TR_RX_BUF_SIZE
#define HCI_TR_RX_BUF_SIZE (760) // must be larger than IAL length
#endif
#ifndef HCI_TR_TX_BUF_SIZE
#define HCI_TR_TX_BUF_SIZE (760)
#endif


#ifndef HCI_UART_SoftwareRxDone_EN //If the HCI_TR_BAUDRATE is greater than 1m(1000000), you are advised to enable it
#define HCI_UART_SoftwareRxDone_EN 0
#endif

#if (HCI_UART_SoftwareRxDone_EN)
#ifndef HCI_TR_RTS_PIN
#error "please define UART RTS Pin for HCI."
#endif

#ifndef HCI_TR_CTS_PIN
#error "please define UART CTS Pin for HCI."
#endif
#endif
/*! HCI transport typedef. */
typedef enum
{
    HCI_TR_SOC,
    HCI_TR_H4,
    HCI_TR_H5,
    HCI_TR_USB,
    HCI_TR_MEM_SHARE
} HCI_TR_MODE;

typedef struct
{
    unsigned int baudrate;  //This member configures the baud rate for UART communication.
    unsigned int tx_Pin;    //
    unsigned int rx_Pin;    //
    unsigned int HwFlowCtl; //Specifies whether the hardware flow control mode is enabled or disabled
    unsigned int cts_Pin;   //uart_parity_e
    unsigned int rts_Pin;   //uart_parity_e

} HCI_TR_UART;

//typedef struct{
//	HCI_UART hci_uart;
//	u8 *tx_buf;
//	u8 *rx_buf;
//}HCI_TR;


/*! Debug system -- Author: GaoQiu. */
//#define HCI_TR_DEBUG

#ifdef HCI_TR_DEBUG
#define ASSERT(exp, errCode)               \
    do {                                   \
        if (!(exp)) {                      \
            REG_ADDR32(0x40000) = errCode; \
            while (1);                     \
        }                                  \
    } while (0)

#else
#define ASSERT(exp, errCode)
#endif

/*! Common error */
#define HCI_TR_ERR_INVALID_PARAM 0x00010000

/*! HCI H5 Protocol error */
#define HCI_TR_ERR_H5_HEAD_LEN        0x01010000
#define HCI_TR_ERR_H5_TX_BUF_OVFL     0x01020000
#define HCI_TR_ERR_H5_HCI_RX_BUF_OVFL 0x01030000

/*! HCI Slip Protocol error */
#define HCI_TR_ERR_SLIP_ESCAPE     0x02010000
#define HCI_TR_ERR_SLIP_ENCODE_BUF 0x02020000
#define HCI_TR_ERR_SLIP_DECODE_BUF 0x02030000

/*! HCI Transport error */
#define HCI_TR_ERR_TR_BACKUP_BUF 0x03010000
#define HCI_TR_ERR_TR_TX_BUF     0x03020000


#ifdef HCI_TR_DEBUG
#define HCI_TRACK_DATA(pData, len)         \
    do {                                   \
        printf("{ ");                      \
        for (int i = 0; i < (len); i++) {  \
            printf("0x%02X ", (pData)[i]); \
        }                                  \
        printf("}\n");                     \
    } while (0);

#define H5_TRACK(format, ...)        printf(format, ##__VA_ARGS__)
#define H5_TRACK_ERR(format, ...)    printf("[ERR -H5  ] " format, ##__VA_ARGS__)
#define H5_TRACK_INFO(format, ...)   printf("[INFO-H5  ] " format, ##__VA_ARGS__)
#define H5_TRACK_WRN(format, ...)    printf("[WARN-H5  ] " format, ##__VA_ARGS__)

#define SLIP_TRACK(format, ...)      printf(format, ##__VA_ARGS__)
#define SLIP_TRACK_ERR(format, ...)  printf("[ERR -SLIP] " format, ##__VA_ARGS__)
#define SLIP_TRACK_INFO(format, ...) printf("[INFO-SLIP] " format, ##__VA_ARGS__)
#define SLIP_TRACK_WRN(format, ...)  printf("[WARN-SLIP] " format, ##__VA_ARGS__)

#define TR_TRACK(format, ...)        printf(format, ##__VA_ARGS__)
#define TR_TRACK_ERR(format, ...)    printf("[ERR -TR  ] " format, ##__VA_ARGS__)
#define TR_TRACK_INFO(format, ...)   printf("[INFO-TR  ] " format, ##__VA_ARGS__)
#define TR_TRACK_WRN(format, ...)    printf("[WARN-TR  ] " format, ##__VA_ARGS__)

#else
#define HCI_TRACK_DATA(pData, len)

#define H5_TRACK(format, ...)
#define H5_TRACK_ERR(format, ...)
#define H5_TRACK_INFO(format, ...)
#define H5_TRACK_WRN(format, ...)

#define SLIP_TRACK(format, ...)
#define SLIP_TRACK_ERR(format, ...)
#define SLIP_TRACK_INFO(format, ...)
#define SLIP_TRACK_WRN(format, ...)

#define TR_TRACK(format, ...)
#define TR_TRACK_ERR(format, ...)
#define TR_TRACK_INFO(format, ...)
#define TR_TRACK_WRN(format, ...)
#endif

#endif /* HCI_TR_DEF_H_ */

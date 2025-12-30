/********************************************************************************************************
 * @file    hci_h5.h
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
#ifndef HCI_H5_H_
#define HCI_H5_H_

#include "hci_tr.h"
#include "common/types.h"
//#include "hci_tr_api.h"
#include "common/fifo.h"
#if (HCI_TR_MODE == HCI_TR_H5)

    /*!  H5 transport buffer config macro. */
    #define HCI_H5_TX_BUF_SIZE MEM_SIZE_ALIGN4(HCI_H5_HEAD_LEN + HCI_TR_TX_BUF_SIZE + HCI_H5_CRC_LEN)


    /*! HCI H5 Packet Type. */
    #define HCI_H5_PKT_TYPE_ACK       0
    #define HCI_H5_PKT_TYPE_CMD       1
    #define HCI_H5_PKT_TYPE_ACL       2
    #define HCI_H5_PKT_TYPE_SCO       3
    #define HCI_H5_PKT_TYPE_EVT       4
    #define HCI_H5_PKT_TYPE_VENDOR    14
    #define HCI_H5_PKT_TYPE_LINK_CTRL 15

    /*! HCI H5 Link Control message. */
    #define HCI_H5_MSG_SYNC       0x7E01
    #define HCI_H5_MSG_SYNC_RSP   0x7D02
    #define HCI_H5_MSG_CONFIG     0xFC03
    #define HCI_H5_MSG_CONFIG_RSP 0x7B04

/*! HCI H5 Transport Link State. */
enum
{
    HCI_H5_LINK_STATE_IDLE,
    HCI_H5_LINK_STATE_INIT,
    HCI_H5_LINK_STATE_ACTIVE,
};

/*! HCI H5 Link Config Info. */
typedef struct
{
    u8 slidWinSize;
    u8 oofFlowCtrl;
    u8 dataIntgrtChkType;
    u8 version;
} HciH5Config_t;

/*! HCI H5 Protocol Packet Header.*/
typedef struct
{
    u8  seqNum;       /*!< Sequence Number.              */
    u8  ackNum;       /*!< Acknowledgment Number.        */
    u8  crc;          /*!< Data Integrity Check Present. */
    u8  reliable;     /*!< Reliable Packet.              */
    u8  pktType;      /*!< Packet Type.                  */
    u8  headCheckSum; /*!< Header Checksum.              */
    u16 payloadLen;   /*!< Payload length.               */
} HciH5Head_t;

    /*! H5 head handle macro. */
    #define H5_HDR_SEQ(head)       ((u32)(head) & 0x07)
    #define H5_HDR_ACK(head)       (((u32)(head) >> 3) & 0x07)
    #define H5_HDR_CRC(head)       (((u32)(head) >> 6) & 0x01)
    #define H5_HDR_RELIABLE(head)  (((u32)(head) >> 7) & 0x01)
    #define H5_HDR_PKT_TYPE(head)  (((u32)(head) >> 8) & 0x0F)
    #define H5_HDR_LEN(head)       (((u32)(head) >> 12) & 0x0FFF)
    #define H5_HDR_CHECKSUM(head)  (((u32)(head) >> 24) & 0xFF)

    #define H5_SET_SEQ(hdr, seq)   ((hdr)[0] |= (seq))
    #define H5_SET_ACK(hdr, ack)   ((hdr)[0] |= (ack) << 3)
    #define H5_SET_RELIABLE(hdr)   ((hdr)[0] |= 1 << 7)
    #define H5_SET_TYPE(hdr, type) ((hdr)[1] |= type)
    #define H5_SET_LEN(hdr, len)   (((hdr)[1] |= ((len) & 0x0f) << 4), ((hdr)[2] |= (len) >> 4))


    /*! OOF Flow Control. */
    #define HCI_H5_OOF_FLW_CTRL_NONE 0
    #define HCI_H5_OOF_FLW_CTRL_EN   1

    /*! Level of Data Integrity Check. */
    #define HCI_H5_DATA_INTEGRITY_LEVEL_NONE  0
    #define HCI_H5_DATA_INTEGRITY_LEVEL_CRC16 1

    /*! Level of Data Integrity Check. */
    #define HCI_H5_VERSION_V1_0 0 /*!< HCI H5 version v1.0. */


    /*! HCI H5 Link Config selection. */
    #define HCI_H5_SLIDING_WIN_SIZE     1 /*!< Sliding Window Size. */
    #define HCI_H5_OOF_FLW_CTRL         HCI_H5_OOF_FLW_CTRL_NONE
    #define HCI_H5_DATA_INTEGRITY_LEVEL HCI_H5_DATA_INTEGRITY_LEVEL_NONE
    #define HCI_H5_VERSION              HCI_H5_VERSION_V1_0

/**
 * @brief : H5 protocol initialization.
 * @param : pHciRxFifo    Pointer point to hci rx FIFO.
 * @param : pHciTxFifo    Pointer point to hci tx FIFO.
 * @return: none.
 */
void HCI_H5_Init(tlk_fifo_t *pHciRxFifo, tlk_fifo_t *pHciTxFifo);

/**
 * @brief : H5 protocol loop.
 * @param : none.
 * @return: none.
 */
void HCI_H5_Poll(void);


#endif /* HCI_H5_H_ */
#endif

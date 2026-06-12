/********************************************************************************************************
 * @file    tlkusb_mscScsi.h
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
#pragma once
#define TLKUSB_MSC_REC_BUFFER_LEN 256

//SCSI - Small Computer System Interface
//BOT - Bulk Only Transport
//CBW - Command Block Wrapper
//CSW - Command Status Wrapper

#define TLKUSB_MSC_CBW_SIGN_OFFSET    0
#define TLKUSB_MSC_CBW_SIGN_LENGTH    4
#define TLKUSB_MSC_CBW_DTAG_OFFSET    4
#define TLKUSB_MSC_CBW_DTAG_LENGTH    4
#define TLKUSB_MSC_CBW_DLEN_OFFSET    8
#define TLKUSB_MSC_CBW_DLEN_LENGTH    4
#define TLKUSB_MSC_CBW_FLAG_OFFSET    12
#define TLKUSB_MSC_CBW_FLAG_LENGTH    1
#define TLKUSB_MSC_CBW_BLUN_OFFSET    13
#define TLKUSB_MSC_CBW_BLUN_LENGTH    1 //BIT[0~3]
#define TLKUSB_MSC_CBW_BLEN_OFFSET    14
#define TLKUSB_MSC_CBW_BLEN_LENGTH    1
#define TLKUSB_MSC_CBW_CB_OFFSET      15


#define TLKUSB_MSC_CSW_SIGN_OFFSET    0
#define TLKUSB_MSC_CSW_SIGN_LENGTH    4
#define TLKUSB_MSC_CSW_DTAG_OFFSET    4
#define TLKUSB_MSC_CSW_DTAG_LENGTH    4
#define TLKUSB_MSC_CSW_RESIDUE_OFFSET 8
#define TLKUSB_MSC_CSW_RESIDUE_LENGTH 4
#define TLKUSB_MSC_CSW_STATUS_OFFSET  12
#define TLKUSB_MSC_CSW_STATUS_LENGTH  1


//SCSI command
//#define TLKUSB_MSC_SCSI_FORMAT_UNIT                   0x04 //Not Deal
#define TLKUSB_MSC_SCSI_TEST_UNIT_READY        0x00
#define TLKUSB_MSC_SCSI_REQUEST_SENSE          0x03
#define TLKUSB_MSC_SCSI_INQUIRY                0x12
#define TLKUSB_MSC_SCSI_MODE_SENSE6            0x1A
#define TLKUSB_MSC_SCSI_START_STOP_UNIT        0x1B
#define TLKUSB_MSC_SCSI_ALLOW_MEDIUM_REMOVAL   0x1E
#define TLKUSB_MSC_SCSI_READ_FORMAT_CAPACITIES 0x23
#define TLKUSB_MSC_SCSI_READ_CAPACITY10        0x25
#define TLKUSB_MSC_SCSI_READ10                 0x28
#define TLKUSB_MSC_SCSI_WRITE10                0x2A
#define TLKUSB_MSC_SCSI_VERIFY10               0x2F
#define TLKUSB_MSC_SCSI_MODE_SENSE10           0x5A

//CSW Status Definitions
#define TLKUSB_MSC_CSW_STATUS_CMD_PASSED  0x00
#define TLKUSB_MSC_CSW_STATUS_CMD_FAILED  0x01
#define TLKUSB_MSC_CSW_STATUS_PHASE_ERROR 0x00


#define TLKUSB_MSC_CBW_SIGNATURE          0x43425355
#define TLKUSB_MSC_CSW_SIGNATURE          0x53425355
#define TLKUSB_MSC_CBW_LENGTH             31
#define TLKUSB_MSC_CSW_LENGTH             13

//SCSI Flag
typedef enum
{
    TLKUSB_MSC_SCSI_FLAG_NONE      = 0x00,
    TLKUSB_MSC_SCSI_FLAG_SEND_DATA = 0x01,
    TLKUSB_MSC_SCSI_FLAG_READ_DATA = 0x02,
    TLKUSB_MSC_SCSI_FLAG_RECV_DATA = 0x04,
} TLKUSB_MSC_SCSI_FLAG_ENUM;

//SCSI Stage
typedef enum
{
    TLKUSB_MSC_SCSI_STAGE_IDLE = 0,
    TLKUSB_MSC_SCSI_STAGE_DATA,
    TLKUSB_MSC_SCSI_STAGE_STATUS,
    TLKUSB_MSC_SCSI_STAGE_COMPLETE,
} TLKUSB_MSC_SCSI_STAGE_ENUM;

typedef struct
{
    uint32_t dSignature;  /*CBW_SIGNATURE*/
    uint32_t dTag;        /*A command block identifier sent by the Host. The device needs the original dTag (part of CSW) and then sends it to the host.*/
    uint32_t dDataLength; /*The CBW command requires the number of bytes transferred between the command and the response. If the value is 0, no data is transmitted*/
    uint8_t  bmFlags;     /*Data transmission direction.0x00:Host->Device 0x80:Device->Host*/
    uint8_t  bLUN;        /*For a device with multiple LUN logical units, select a target. If there are no multiple LUN, write 0.*/
    uint8_t  bCBLength;   /*Length of command,0~16.*/
    uint8_t  CB[16];      /*Command*/
} tlkusb_mscScsiCBW_t;

typedef struct
{
    uint32_t dSignature;   /*CSW_SIGNATURE*/
    uint32_t dTag;         /*The same to dTag in CBW.*/
    uint32_t dDataResidue; /*Data to be transferred.*/
    uint8_t  bStatus;      /*Indicates the execution status of a command*/
} tlkusb_mscScsiCSW_t;

typedef struct
{
    uint8_t  stage; //SCSI stage
    uint8_t  flags; //Recv or Send data mask
    uint8_t  enable;
    uint8_t  resv00;
    uint8_t  curLun; //current LUN
    uint8_t  resv01;
    uint16_t rcvLen; //Recv length
    uint16_t optLen; //optLen in buffer
    uint16_t datLen; //dataLen in buffer
    uint32_t blkOff; //Block Offset
    uint16_t blkCnt; //Block Count
    uint16_t blkNum; //Block Number
    uint8_t  rcvBuff[TLKUSB_MSC_REC_BUFFER_LEN];
    uint8_t  cswBuff[16];
    uint8_t  buffer[TLK_CFG_FS_SECTOR_SIZE];
} tlkusb_msc_scsi_t;

/**
 * @brief       Initialize the MSC SCSI module.
 * @param[in]   none.
 * @return      Operation result. TLK_ENONE means success, others means failure.
 */
int tlkusb_msc_scsiInit(void);

/**
 * @brief       Reset the MSC SCSI module.
 * @param[in]   none.
 * @return      none.
 */
void tlkusb_msc_scsiReset(void);

/**
 * @brief       Handler for the MSC SCSI module.
 * @param[in]   none.
 * @return      none.
 */
void tlkusb_msc_scsiHandler(void);

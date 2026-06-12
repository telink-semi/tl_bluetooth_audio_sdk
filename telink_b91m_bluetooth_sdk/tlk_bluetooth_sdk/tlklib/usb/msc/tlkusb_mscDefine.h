/********************************************************************************************************
 * @file    tlkusb_mscDefine.h
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

#define TLKUSB_MSC_UNIT_COUNT         1

#define TLKUSB_MSC_VENDOR_STRING_DEF  "TLK-DISK"         // length<=8
#define TLKUSB_MSC_PRODUCE_STRING_DEF "Telink Disk Demo" // length<=16
#define TLKUSB_MSC_VERSION_STRING_DEF "1.01"             // length<=4

typedef enum
{
    TLKUSB_MSC_INF_MSC,
    TLKUSB_MSC_INF_MAX,
} TLKUSB_MSC_INF_ENUM;

typedef enum
{
    TLKUSB_MSC_EDP_IN  = 1,
    TLKUSB_MSC_EDP_OUT = 5,
} TLKUSB_MSC_EDP_ENUM;

#define MS_CBW_SIGNATURE        0x43425355UL
#define MS_CSW_SIGNATURE        0x53425355UL
#define MS_COMMAND_DIR_DATA_OUT (0 << 7)
#define MS_COMMAND_DIR_DATA_IN  (1 << 7)

//SCSI Command
#define MS_SCSI_CMD_INQUIRY                      0x12
#define MS_SCSI_CMD_REQUEST_SENSE                0x03
#define MS_SCSI_CMD_TEST_UNIT_READY              0x00
#define MS_SCSI_CMD_READ_CAPACITY_10             0x25
#define MS_SCSI_CMD_SEND_DIAGNOSTIC              0x1D
#define MS_SCSI_CMD_PREVENT_ALLOW_MEDIUM_REMOVAL 0x1E
#define MS_SCSI_CMD_WRITE_10                     0x2A
#define MS_SCSI_CMD_READ_10                      0x28
#define MS_SCSI_CMD_WRITE_6                      0x0A
#define MS_SCSI_CMD_READ_6                       0x08
#define MS_SCSI_CMD_VERIFY_10                    0x2F
#define MS_SCSI_CMD_MODE_SENSE_6                 0x1A
#define MS_SCSI_CMD_MODE_SENSE_10                0x5A

//Sense key
#define MS_SCSI_SENSE_KEY_GOOD                            0x00
#define MS_SCSI_SENSE_KEY_RECOVERED_ERROR                 0x01
#define MS_SCSI_SENSE_KEY_NOT_READY                       0x02
#define MS_SCSI_SENSE_KEY_MEDIUM_ERROR                    0x03
#define MS_SCSI_SENSE_KEY_HARDWARE_ERROR                  0x04
#define MS_SCSI_SENSE_KEY_ILLEGAL_REQUEST                 0x05

#define MS_SCSI_SENSE_KEY_UNIT_ATTENTION                  0x06
#define MS_SCSI_SENSE_KEY_DATA_PROTECT                    0x07
#define MS_SCSI_SENSE_KEY_BLANK_CHECK                     0x08
#define MS_SCSI_SENSE_KEY_VENDOR_SPECIFIC                 0x09
#define MS_SCSI_SENSE_KEY_COPY_ABORTED                    0x0A
#define MS_SCSI_SENSE_KEY_ABORTED_COMMAND                 0x0B
#define MS_SCSI_SENSE_KEY_VOLUME_OVERFLOW                 0x0D
#define MS_SCSI_SENSE_KEY_MISCOMPARE                      0x0E

#define MS_SCSI_ASENSE_NO_ADDITIONAL_INFORMATION          0x00
#define MS_SCSI_ASENSE_LOGICAL_UNIT_NOT_READY             0x04
#define MS_SCSI_ASENSE_INVALID_FIELD_IN_CDB               0x24

#define MS_SCSI_ASENSE_NOT_READY_TO_READY_CHANGE          0x28
#define MS_SCSI_ASENSE_WRITE_PROTECTED                    0x27
#define MS_SCSI_ASENSE_FORMAT_ERROR                       0x31
#define MS_SCSI_ASENSE_INVALID_COMMAND                    0x20
#define MS_SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE 0x21
#define MS_SCSI_ASENSE_MEDIUM_NOT_PRESENT                 0x3A

#define MS_SCSI_ASENSEQ_NO_QUALIFIER                      0x00
#define MS_SCSI_ASENSEQ_FORMAT_COMMAND_FAILED             0x01
#define MS_SCSI_ASENSEQ_INITIALIZING_COMMAND_REQUIRED     0x02
#define MS_SCSI_ASENSEQ_OPERATION_IN_PROGRESS             0x07

enum MS_Descriptor_ClassSubclassProtocol_t
{
    MS_CSCP_MassStorageClass          = 0x08,
    MS_CSCP_SCSITransparentSubclass   = 0x06,
    MS_CSCP_BulkOnlyTransportProtocol = 0x50,
};

enum MS_ClassRequests_t
{
    MS_REQ_GetMaxLUN = 0xFE,
    MS_REQ_MassStorageReset,
};

enum MS_CommandStatusCodes_t
{
    MS_MS_SCSI_COMMAND_Pass,
    MS_MS_SCSI_COMMAND_Fail,
    MS_MS_SCSI_COMMAND_PhaseError,
};

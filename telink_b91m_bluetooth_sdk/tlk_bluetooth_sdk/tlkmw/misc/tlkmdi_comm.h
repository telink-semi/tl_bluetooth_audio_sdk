/********************************************************************************************************
 * @file    tlkmdi_comm.h
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
#ifndef TLKMDI_COMM_H
#define TLKMDI_COMM_H

#include "common/types.h"

#if (!MCU_CORE_TL752X_TEMP)
#define TLKDEV_SERIAL0_PORT UART0
#define TLKDEV_SERIAL1_PORT UART1
#else
#define TLKDEV_SERIAL0_PORT 0
#define TLKDEV_SERIAL1_PORT 1
#endif

#define TLKMDI_COMM_SERIAL_BAUDRATE   1500000 // 921600//115200
#define TLKMDI_COMM_SERIAL_RBUFF_NUMB 2
#define TLKMDI_COMM_SERIAL_RBUFF_SIZE 540 // 280 //The configuration should be larger than the maximum frame of data in the system
#define TLKMDI_COMM_SERIAL_SBUFF_NUMB 4
#define TLKMDI_COMM_SERIAL_SBUFF_SIZE 128


/**
 * @brief       This file defines constants related to communication data handling.
 * @note        The configuration values should be adjusted based on the system's maximum frame length requirements.
 */
#define TLKMDI_COMM_DATA_CHANNEL_MAX 4

/**
 * @brief       This macro defines the maximum length of a receive frame in the communication system.
 * @note        The configuration should be greater than or equal to the length of the largest frame of data in the system.
 */
#define TLKMDI_COMM_RECV_FRAME_MAXLEN (256 + 128)

/**
 * @brief       This macro defines the maximum length of a send frame in the communication system.
 */
#define TLKMDI_COMM_SEND_FRAME_MAXLEN 128

/**
 * @brief       This macro defines the size of the cache used for receiving data in the communication system.
 */
#define TLKMDI_COMM_RECV_CACHE_SIZE (16 + TLKMDI_COMM_RECV_FRAME_MAXLEN + 32)

/**
 * @brief       This macro defines the size of the cache used for sending data in the communication system.
 */
#define TLKMDI_COMM_SEND_CACHE_SIZE (16 + TLKMDI_COMM_SEND_FRAME_MAXLEN + 16)


typedef int (*TlkMdiCommResetCB)(void);
typedef void (*TlkMdiCommDatCB)(uint8_t datID, uint32_t number, uint8_t *pData, uint16_t dataLen); // number:Range[0,0xFFFFF]

/**
 * @brief       This enum defines the possible authentication status values for communication.
 */
typedef enum
{
    TLKMDI_COMM_AUTH_STATUS_NONE    = 0,
    TLKMDI_COMM_AUTH_STATUS_SUCCESS = 1,
    TLKMDI_COMM_AUTH_STATUS_FAILURE = 2,
    TLKMDI_COMM_AUTH_STATUS_RUNNING = 3,
} TLKMDI_COMM_AUTH_STATUS_ENUM;

/**
 * @brief       Initializes the communication module.
 * @return      TLK_ENONE if successful, otherwise a negative error code.    
 */
int tlkmdi_comm_init(void);

/**
 * @brief       This function resets communication by closing, opening, and resetting the serial port if enabled, and resets the service protocol timeout.
 * @return      none.
 */
void tlkmdi_comm_reset(void);

/**
 * @brief       This function handles communication by processing serial handlers if the serial device is enabled.
 * @return      none.
 */
void tlkmdi_comm_handler(void);
/**
 * @brief       This function returns the buffer size for communication.
 * @return      The buffer size.   
 */
int tlkmdi_comm_getBufferSize(void);

/**
 * @brief       This function returns the data buffer size for communication.
 * @return      The data buffer size.    
 */
int tlkmdi_comm_getDataBufferSize(void);

/**
 * @brief       This function checks if the serial FIFO has more than 60% bytes available.
 * @param[in]   dataLen - the length of data to check against.
 * @return      true if more than 60% bytes are available, false otherwise.  
 */
bool tlkmdi_comm_sfifoIsMore60(uint16_t dataLen);

/**
 * @brief       This function checks if the serial FIFO has more than 80% bytes available.
 * @param[in]   dataLen - the length of data to check against.
 * @return      true if more than 80% bytes are available, false otherwise.  
 */
bool tlkmdi_comm_sfifoIsMore80(uint16_t dataLen);


/**
 * @brief       This function finds the next available data channel ID.
 * @param[out]  pDatID - the pointer to store the available data channel ID.
 * @return      TLK_ENONE if an available ID is found, -TLK_EQUOTA if no ID is available.   
 */
int tlkmdi_comm_getValidDatID(uint8_t *pDatID);

/**
 * @brief       This function registers a callback for a specific data channel.
 * @param[in]   datID - the data channel ID.
 * @param[in]   datCB - the callback function to register.
 * @param[in]   isForce - if true, force registration even if a callback is already set.
 * @return      TLK_ENONE on success, -TLK_EQUOTA if the datID exceeds the maximum, -TLK_EREPEAT if a callback is already registered.  
 */
int tlkmdi_comm_regDatCB(uint8_t datID, TlkMdiCommDatCB datCB, bool isForce);

/**
 * @brief       This function sends data over the serial port.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      TLK_ENONE on success, -TLK_ENOSUPPORT if the serial device is not enabled.
 */
int tlkmdi_comm_send(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function sends a command over the communication interface.
 * @param[in]   mType - the type of the message.
 * @param[in]   cmdID - the ID of the command.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      The result of the send operation.    
 */
int tlkmdi_comm_sendCmd(uint8_t mType, uint16_t cmdID, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       This function sends a response over the communication interface.
 * @param[in]   mType - the type of the message.
 * @param[in]   cmdID - the ID of the command.
 * @param[in]   status - the status of the response.
 * @param[in]   reason - the reason for the response.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      The result of the send operation.
 */
int tlkmdi_comm_sendRsp(uint8_t mType, uint16_t cmdID, uint8_t status, uint8_t reason, uint8_t *pData, uint8_t dataLen);

/**
 * @brief       This function sends an event over the communication interface.
 * @param[in]   mType - the type of the message.
 * @param[in]   evtID - the ID of the event.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      The result of the send operation.  
 */
int tlkmdi_comm_sendEvt(uint8_t mType, uint16_t evtID, uint8_t *pData, uint8_t dataLen);


/**
 * @brief       This function sends data packets over the communication interface.
 * @param[in]   datID - the ID of the data.
 * @param[in]   numb - the number associated with the data.
 * @param[in]   pData - the pointer to the data to send.
 * @param[in]   dataLen - the length of the data to send.
 * @return      The result of the send operation, -TLK_EPARAM if input parameters are invalid. 
 */
int tlkmdi_comm_sendDat(uint8_t datID, uint32_t numb, uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function sends log data over the communication interface.
 * @param[in]   pData - the pointer to the log data to send.
 * @param[in]   dataLen - the length of the log data to send.
 * @return      The result of the send operation, -TLK_EBUSY if the serial FIFO is busy.  
 */
int tlkmdi_comm_sendLogDat(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function sends VCD data over the communication interface.
 * @param[in]   pData - the pointer to the VCD data to send.
 * @param[in]   dataLen - the length of the VCD data to send.
 * @return      The result of the send operation, -TLK_EBUSY if the serial FIFO is busy.  
 */
int tlkmdi_comm_sendVcdDat(uint8_t *pData, uint16_t dataLen);

/**
 * @brief       This function sends OTA data over the communication interface.
 * @param[in]   taskID - the ID of the task.
 * @param[in]   pData - the pointer to the OTA data to send.
 * @param[in]   dataLen - the length of the OTA data to send.
 * @param[in]   UserArg - the user argument.
 * @return      The result of the send operation, -TLK_EBUSY if the serial FIFO is busy.
 */
int tlkmdi_comm_sendOTADat(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg);

/**
 * @brief       This function sends cover data over the communication interface.
 * @param[in]   pData - the pointer to the cover data to send.
 * @param[in]   dataLen - the length of the cover data to send.
 * @return      The result of the send operation, -TLK_EBUSY if the serial FIFO is busy. 
 */
int tlkmdi_comm_sendCoverDat(uint8_t *pData, uint16_t dataLen);


#endif // TLKMDI_COMM_H

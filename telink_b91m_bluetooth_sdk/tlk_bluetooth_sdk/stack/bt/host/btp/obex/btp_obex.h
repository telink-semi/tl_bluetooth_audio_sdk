/********************************************************************************************************
 * @file    btp_obex.h
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
#ifndef BTP_OBEX_H
#define BTP_OBEX_H

#if (TLKBTP_CFG_OBEX_ENABLE)


#define BTP_OBEX_MAP_CONNID_START      0x10010000
#define BTP_OBEX_PBAP_CONNID_START     0x20010000
#define BTP_OBEX_COVERART_CONNID_START 0x30010000


#define BTP_OBEX_RFC_VERSION           0x10 // v1.0
#define BTP_OBEX_L2C_VERSION           0x12

typedef enum
{
    BTP_OBEX_CHTYPE_RFCOMM = 0,
    BTP_OBEX_CHTYPE_L2CAP,
} BTP_OBEX_CHTYPE_ENUM;

/*
  Operation Opcodes
 */
typedef enum
{
    BTP_OBEX_OPCODE_CONNECT        = 0x80,
    BTP_OBEX_OPCODE_DISCONN        = 0x81,
    BTP_OBEX_OPCODE_PUT_FINAL      = 0x82,
    BTP_OBEX_OPCODE_PUT            = 0X02,
    BTP_OBEX_OPCODE_GET            = 0x03,
    BTP_OBEX_OPCODE_GET_FINAL      = 0x83,
    BTP_OBEX_OPCODE_SETPATH        = 0x85,
    BTP_OBEX_OPCODE_RESERVED       = 0x04,
    BTP_OBEX_OPCODE_RESERVED_FINAL = 0x84,
    BTP_OBEX_OPCODE_ABORT          = 0xff,
    BTP_OBEX_OPCODE_RESERVED_LOWER = 0x06,
    BTP_OBEX_OPCODE_RESERVER_UPPER = 0x0f,
    BTP_OBEX_OPCODE_USER_LOWER     = 0x10,
    BTP_OBEX_OPCODE_USER_UPPER     = 0x1f,
} BTP_OBEX_OPCODE_ENUM;

typedef enum
{
    BTP_OBEX_RSP_CONTINUE               = 0x90,
    BTP_OBEX_RSP_OK                     = 0xA0,
    BTP_OBEX_RSP_CREATED                = 0xA1,
    BTP_OBEX_RSP_ACCEPTED               = 0xA2,
    BTP_OBEX_RSP_NON_AUTH_INFO          = 0xA3,
    BTP_OBEX_RSP_NO_CONTENT             = 0xA4,
    BTP_OBEX_RSP_RESET_CONTENT          = 0xA5,
    BTP_OBEX_RSP_PARTIAL_RESET          = 0xA6,
    BTP_OBEX_RSP_MULTIPLE_CHOICE        = 0xB0,
    BTP_OBEX_RSP_MOVED_PERM             = 0xB1,
    BTP_OBEX_RSP_MOVED_TEMP             = 0xB2,
    BTP_OBEX_RSP_SEE_OTHER              = 0xB3,
    BTP_OBEX_RSP_NOT_MODIFIED           = 0xB4,
    BTP_OBEX_RSP_USE_PROXY              = 0xB5,
    BTP_OBEX_RSP_BAD_REQUEST            = 0xC0,
    BTP_OBEX_RSP_UNAUTHORIZED           = 0xC1,
    BTP_OBEX_RSP_PAYMENT_REQUIRED       = 0xC2,
    BTP_OBEX_RSP_FORBIDDEN              = 0xC3,
    BTP_OBEX_RSP_NOT_FOUND              = 0xC4,
    BTP_OBEX_RSP_METHOD_NOT_ALLOWED     = 0xC5,
    BTP_OBEX_RSP_NOT_ACCEPTABLE         = 0xC6,
    BTP_OBEX_RSP_PROXY_AUTH_REQUIRED    = 0xC7,
    BTP_OBEX_RSP_REQUEST_TIMEOUT        = 0xC8,
    BTP_OBEX_RSP_CONFLICT               = 0xC9,
    BTP_OBEX_RSP_GONE                   = 0xCA,
    BTP_OBEX_RSP_LENGTH_REQUIRED        = 0xCB,
    BTP_OBEX_RSP_PRECONDITION_FAIL      = 0xCC,
    BTP_OBEX_RSP_REQ_ENTITY_TOO_LARGE   = 0xCD,
    BTP_OBEX_RSP_REQ_URL_TOO_LARGE      = 0xCE,
    BTP_OBEX_RSP_UNSUPPORTED_MEDIA_TYPE = 0xCF,
    BTP_OBEX_RSP_SERVER_ERROR           = 0xD0,
    BTP_OBEX_RSP_NOT_IMPLEMENTED        = 0xD1,
    BTP_OBEX_RSP_BAD_GATEWAY            = 0xD2,
    BTP_OBEX_RSP_SERVICE_UNAVAILABLE    = 0xD3,
    BTP_OBEX_RSP_GATEWAY_TIMEOUT        = 0xD4,
    BTP_OBEX_RSP_HTTP_VER_NOT_SUPPORTED = 0xD5,
    BTP_OBEX_RSP_DATABASE_FULL          = 0xE0,
    BTP_OBEX_RSP_DATABASE_LOCKED        = 0xE1,
} BTP_OBEX_RSP_ENUM;

typedef enum
{
    BTP_OBEX_HEADID_COUNT          = 0xC0, // Number of objects (used by Connect)
    BTP_OBEX_HEADID_NAME           = 0x01, // name of the object (often a file name)
    BTP_OBEX_HEADID_TYPE           = 0x42, // type of object - e.g. text, html, binary, manufacturer specific
    BTP_OBEX_HEADID_LENGTH         = 0xC3, // the length of the object in bytes
    BTP_OBEX_HEADID_TIME0          = 0x44, // date/time stamp ISO 8601 version - preferred
    BTP_OBEX_HEADID_TIME1          = 0xC4, // date/time stamp 4 byte version (for compatibility only)
    BTP_OBEX_HEADID_DESC           = 0x05, // text description of the object
    BTP_OBEX_HEADID_TARGET         = 0x46, // name of service that operation is targeted to
    BTP_OBEX_HEADID_HTTP           = 0x47, // an HTTP 1.x header
    BTP_OBEX_HEADID_BODY           = 0x48, // a chunk of the object body
    BTP_OBEX_HEADID_END_BODY       = 0x49, // the final chunk of the object body
    BTP_OBEX_HEADID_WHO            = 0x4A, // identifies the OBEX application, used to tell if talking to a peer
    BTP_OBEX_HEADID_CONNECT_ID     = 0xCB, // an identifier used for OBEX connection multiplexing
    BTP_OBEX_HEADID_APP_PARAM      = 0x4C, // extended application request & response information
    BTP_OBEX_HEADID_AUTH_CHALL     = 0x4D, // authentication digest-challenge
    BTP_OBEX_HEADID_AUTH_RSP       = 0x4E, // authentication digest-response
    BTP_OBEX_HEADID_CLASS          = 0x4F,
    BTP_OBEX_HEADID_CREATOR_ID     = 0xCF, // indicates the creator of an object
    BTP_OBEX_HEADID_WAN_UUID       = 0x50, // uniquely identifies the network client (OBEX server)
    BTP_OBEX_HEADID_OBJECT_CLASS   = 0x51, // OBEX Object class of object
    BTP_OBEX_HEADID_SESSION_PARAM  = 0x52, // Parameters used in session commands/responses
    BTP_OBEX_HEADID_SESSION_SEQ    = 0x93, // Sequence number used in each OBEX packet for reliability
    BTP_OBEX_HEADID_SINGLE_RSPMODE = 0x97, // Single Response Mode
    BTP_OBEX_HEADID_SRM_PARAM      = 0x98, // Single Response Mode Parameter
    BTP_OBEX_HEADID_RESERVED_LOWER = 0x10,
    BTP_OBEX_HEADID_RESERVER_UPPER = 0x2F,
    BTP_OBEX_HEADID_USER_LOWER     = 0x30,
    BTP_OBEX_HEADID_USER_UPPER     = 0x3F,
} BTP_OBEX_HEADID_ENUM;

typedef struct
{
    uint16_t arg_count;
    uint16_t arg_name;
    uint16_t arg_type;
    uint16_t arg_len;
    uint16_t arg_time_iso;
    uint16_t arg_time_32;
    uint16_t arg_des;
    uint16_t arg_tar;
    uint16_t arg_http;
    uint16_t arg_body;
    uint16_t arg_eob;
    uint16_t arg_who;
    uint16_t arg_cid;
    uint16_t arg_app;
    uint16_t arg_au_cal;
    uint16_t arg_au_rsp;
    uint16_t arg_class;
    uint16_t arg_srm;
    uint16_t arg_srmParam;
} btp_obex_headarg_t;

typedef struct
{
    uint8_t  ctype; // Refer to BTP_OBEX_CHTYPE_ENUM.  Channel Type
    uint8_t  usrOrSrv;
    uint8_t  rtnMode;
    uint8_t  fcsIsEn;
    uint16_t ehnCtrl;
    uint16_t channel;
    uint16_t mtuSize;
    uint16_t ifrmCtrl;
    uint16_t psmOrChn;
    uint16_t aclHandle;
    uint32_t connectID;
} btp_obex_t;

/******************************************************************************
 * Function: Obex Extract Data interface
 * Descript: This interface be used by pbap to extract the Obex data from
 *           peer entity.
 * Params:
 * Return: Returning true/false.
 *******************************************************************************/
bool btp_obex_extractArgHeaders(btp_obex_headarg_t *header, uint8_t *pData, uint16_t dataLen);

/******************************************************************************
 * Function: The Obex Pack Connect Header interface
 * Descript: Defines pack the Obex Connect Header data.
 * Params:
 *        @mtuSize[IN--The Channel's MTU size.
 *        @ctype[IN]--which transport will used(rfcomm/l2cap).
 *        @payloadLen[IN]--The data length.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The Connect header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendConnectReq(btp_obex_t *pObex, uint8_t *pUsrData, uint16_t dataLen);

/******************************************************************************
 * Function: The Obex Pack Connect Rsp Header interface
 * Descript: Defines pack the Obex Connect Rsp Header data.
 * Params:
 *        @connectID[IN]--The connection ID.
 *        @mtuSize[IN]--The Channel's MTU size.
 *        @ctype[IN]--which transport will used(rfcomm/l2cap).
 *        @dataLen[IN]--The data length.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The Connect header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendConnectRsp(btp_obex_t *pObex, uint8_t *pUsrData, uint16_t dataLen);

/******************************************************************************
 * Function: The Obex Pack DisConnect Cmd Header interface
 * Descript: Defines pack the Obex DisConnect cmd Header data.
 * Params:
 *        @connectID[IN]--The connection ID.
 *        @dataLen[IN]--The data length.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The Connect header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendDisconnReq(btp_obex_t *pObex, uint8_t *pUsrData, uint16_t dataLen);

/******************************************************************************
 * Function: The Obex Pack DisConnect Rsp Header interface
 * Descript: Defines pack the Obex DisConnect Rsp Header data.
 * Params:
 *        @connectID[IN]--The connection ID.
 *        @dataLen[IN]--The data length.
 *        @isSucc[IN]--The action is success
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The Connect header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendDisconnRsp(btp_obex_t *pObex, uint8_t *pUsrData, uint16_t dataLen, bool isSucc);

/******************************************************************************
 * Function: The Obex Pack SetPath Cmd Header interface
 * Descript: Defines pack the Obex setpath cmd Header data.
 * Params:
 *        @connectID[IN]--The connection ID.
 *        @pathLen[IN]--The path length.
 *        @flags[IN]--The flag.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The Connect header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendSetPathReq(btp_obex_t *pObex, uint8_t *pPath, uint16_t pathLen, uint8_t flags);

/******************************************************************************
 * Function: The Obex Pack Put Cmd Header interface
 * Descript: Defines pack the Obex Put cmd Header data.
 * Params:
 *        @dataLen[IN]--The data length.
 *        @isFinal[IN]--The final cmd.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The Connect header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendPutReq(btp_obex_t *pObex, uint8_t *pUsrData, uint16_t dataLen, bool isFinal);

/******************************************************************************
 * Function: The Obex Get Command Pack interface
 * Descript: Defines Pack Obex send to Get Command header.
 * Params:
 *        @connectID[IN--The connection id.
 *        @isEnSRM[IN]--The single response mode.
 *        @isWait[IN]--The need to wait.
 *        @pName[IN]--The record name.
 *        @nameLen[IN]--The name length.
 *        @pType[IN]--The type of content need to get.
 *        @typeLen[IN]--The length.
 *        @dataLen[IN]--The data length.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The Header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendGetReq(btp_obex_t *pObex, bool isEnSRM, bool isWait, uint8_t *pName, uint8_t nameLen, uint8_t *pType, uint8_t typeLen, uint8_t *pAppData, uint8_t dataLen);
int btp_obex_sendGetReq1(btp_obex_t *pObex, bool isEnSRM, bool isWait, uint8_t *pName, uint8_t nameLen, uint8_t *pType, uint8_t typeLen, uint8_t *pAppData, uint8_t dataLen);

/******************************************************************************
 * Function: The Obex Get Continue Command Pack interface
 * Descript: Defines pack the Obex Get Continue Command header.
 * Params:
 *        @isWait[IN]--The need to wait.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The headerLen.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendGetContinueReq(btp_obex_t *pObex, bool isWait);

/******************************************************************************
 * Function: The Obex abort Command Pack interface
 * Descript: Defines pack the Obex Get Continue Command header.
 * Params:
 *        @isWait[IN]--The need to wait.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The headerLen.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendAbort(btp_obex_t *pObex, bool isWait);

/******************************************************************************
 * Function: The Obex Send Error Response header interface
 * Descript: Defines pack the Obex send to send Error response header.
 * Params:
 *        @ctype[IN--The transport layer type.
 *        @rspCode[IN]--The response code.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendErrorRsp(btp_obex_t *pObex, uint8_t rspCode);

/******************************************************************************
 * Function: The Obex Pack Common Rsp Header interface
 * Descript: Defines pack the Obex Common Rsp Header data.
 * Params:
 *        @rspCode[IN]--The response code to be send.
 *        @dataLen[IN]--The data length.
 *        @buffer[IN]--The header.
 *        @headerLen[OUT]--The Connect header length.
 * Return: Returning header content.
 *******************************************************************************/
int btp_obex_sendCommRsp(btp_obex_t *pObex, uint8_t rspCode, uint8_t *pUsrData, uint16_t dataLen);

int btp_obex_sendData(btp_obex_t *pObex, uint8_t *pHead, uint16_t headLen, uint8_t *pBody, uint16_t bodyLen);


#endif // #if (BTP_OBEX_H)

#endif

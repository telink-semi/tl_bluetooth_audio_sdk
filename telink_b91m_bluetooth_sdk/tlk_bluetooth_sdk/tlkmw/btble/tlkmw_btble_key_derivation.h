/********************************************************************************************************
 * @file    tlkmw_btble_key_derivation.h
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
#include <string.h>
#include "tl_common.h"
#include "common/types.h"
#include "common/utility.h"

#define BTBLE_KEY_DERIVATION_LOG_EN   1
#define TLKMDI_SMP_OVER_BREDR_TIMEOUT 30000000 //30s

typedef enum
{

    // general states
    SM_GENERAL_IDLE,
    SM_GENERAL_SEND_PAIRING_FAILED,
    SM_GENERAL_TIMEOUT, // no other security messages are exchanged
    SM_GENERAL_REENCRYPTION_FAILED,

    // Classic
    SM_BR_EDR_W4_ENCRYPTION_COMPLETE,
    SM_BR_EDR_ENCRYPTION_COMPLETE,
    SM_BR_EDR_GENERATE_LTK_FROM_LINKKEY,

    SM_BR_EDR_INITIATOR_SEND_PAIRING_REQUEST,
    SM_BR_EDR_INITIATOR_W4_PAIRING_RESPONSE,
    SM_BR_EDR_INITIATOR_GENERATE_LTK_FROM_LINKKEY,
    SM_BR_EDR_INITIATOR_W4_IDENTITY_INFORMATION,
    SM_BR_EDR_INITIATOR_W4_IDENTITY_ADDRESS_INFORMATION,
    SM_BR_EDR_INITIATOR_SEND_IDENTITY_INFORMATION,
    SM_BR_EDR_INITIATOR_SEND_IDENTITY_ADDRESS_INFORMATION,

    SM_BR_EDR_RESPONDER_W4_PAIRING_REQUEST,
    SM_BR_EDR_RESPONDER_SEND_PAIRING_RESPONSE,
    SM_BR_EDR_RESPONDER_GENERATE_LTK_FROM_LINKKEY,
    SM_BR_EDR_RESPONDER_SEND_IDENTITY_INFORMATION,
    SM_BR_EDR_RESPONDER_SEND_IDENTITY_ADDRESS_INFORMATION,
    SM_BR_EDR_RESPONDER_W4_IDENTITY_INFORMATION,
    SM_BR_EDR_RESPONDER_W4_IDENTITY_ADDRESS_INFORMATION,
} security_manager_state_t;

typedef struct
{
    uint8_t role;
    /** 
     * 0x00 Link Level Encryption is OFF.
     * 0x01 Link Level Encryption is ON with E0 for BR/EDR.Link Level Encryption is ON with AES-CCM for LE.
     * 0x02 Link Level Encryption is ON with AES-CCM for BR/EDR.
     * All other values Reserved for future use.
     */
    uint8_t  encryption_enabled      : 2;
    uint8_t  simple_pairing_complete : 1;
    uint8_t  use_ct2_flag            : 1;
    uint8_t  recvbit                 : 4;
    uint16_t acl_handle;

    uint8_t                  link_key_type;
    uint8_t                  resvbyte;
    uint8_t                  peer_addr[6]; //peer address
    security_manager_state_t state;
    uint8_t                  link_key[16];
    uint8_t                  ltk[16];
    uint8_t                  peer_irk[16];
    TlkApiTimer_t            smp_timer;
} btble_key_derivation_t;

struct btble_smp_key_distribution
{
    uint8_t encKey  : 1; /** < EDIV and rand */
    uint8_t idKey   : 1; /** < the device shall distribute IRK and public device or static random address using Identity Address Information */
    uint8_t signKey : 1; /** < the device shall distribute CSRK using the Signing Information command */
    uint8_t linkKey : 1; /** < the device would like to derive the Link Key from the LTK */
    uint8_t rtu     : 4; /** < Reserved for future use */
};

struct btble_smp_auth_req
{
    uint8_t bonding_flags : 2; /** < refer to enum ble_host_smp_bonding_flags */
    uint8_t MITM          : 1; /** < man-in-the-middle */
    uint8_t sc            : 1; /** < LE secure connections */
    uint8_t keypress      : 1; /** < the Passkey Entry protocol */
    uint8_t ct2           : 1; /** < 1 upon transmission to indicate support for the h7 function.*/
    uint8_t rfu           : 2; /** < Reserved for future use */
};

struct btble_smp_pairing
{
    uint8_t code;          /** < 0x01 SMP_OPCODE_PAIRING_REQUEST */
    uint8_t io_capability; /** < IO Capability */
    uint8_t oob_data_flag; /** < OOB data flag */

    union
    { /** < authentication requirement */
        struct btble_smp_auth_req auth_req;
        uint8_t                   auth_req_raw;
    };

    uint8_t max_key_size; /** < Maximum Encryption Key Size */

    union
    { /** < Initiator Key Distribution */
        struct btble_smp_key_distribution initiator_key;
        uint8_t                           initiator_key_raw;
    };

    union
    { /** < Responder Key Distribution */
        struct btble_smp_key_distribution responder_key;
        uint8_t                           responder_key_raw;
    };
};

struct btble_smp_identity_information
{
    uint8_t code;    /** < 0x08 SMP_OPCODE_IDENTITY_INFORMATION */
    uint8_t irk[16]; /** < Identity Resolving Key */
};

struct btble_smp_identity_address_information
{
    uint8_t code;     /** < 0x09 SMP_OPCODE_IDENTITY_ADDRESS_INFORMATION */
    uint8_t addrType; /** < BD_ADDR type */
    uint8_t addr[6];  /** < device’s public device address or static random address.*/
};

struct btble_smp_pairing_failed
{
    uint8_t code;   /** < 0x05 SMP_OPCODE_PAIRING_FAILED */
    uint8_t reason; /** < pairing failed reason */
};

void tlkmw_btble_key_derivation_init(bool enable);
void tlkmw_btble_key_derivation_store_link_key(uint8_t bd_addr[6], uint8_t link_key[16], uint8_t key_type);
void tlkmw_btble_bredr_set_encryption_enabled(uint16_t acl_handle, uint8_t encryption_enabled);
void tlkmw_btble_bredr_set_simple_pairing_complete(uint8_t bd_addr[6]);
void tlkmw_btble_bredr_set_connect_complete(uint16_t aclHandle, uint8_t peer_addr[6]);
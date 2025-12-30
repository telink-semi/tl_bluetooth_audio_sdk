/********************************************************************************************************
 * @file    ble_error_code.h
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
// This file is define Bluetooth Controller Error Code Description.

enum bluetooth_controller_error_code
{
    BT_ERR_SUCCESS                                                  = 0,
    BT_ERR_UNKNOWN_HCI_COMMAND                                      = 0x01,
    BT_ERR_UNKNOWN_CONNECTION_IDENTIFIER                            = 0x02,
    BT_ERR_HARDWARE_FAILURE                                         = 0x03,
    BT_ERR_PAGE_TIMEOUT                                             = 0x04,
    BT_ERR_AUTHENTICATION_FAILURE                                   = 0x05,
    BT_ERR_PIN_OR_KEY_MISSING                                       = 0x06,
    BT_ERR_MEMORY_CAPACITY_EXCEEDED                                 = 0x07,
    BT_ERR_CONNECTION_TIMEOUT                                       = 0x08,
    BT_ERR_CONNECTION_LIMIT_EXCEEDED                                = 0x09,
    BT_ERR_SYNCHRONOUS_CONNECTION_LIMIT_TO_A_DEVICE_EXCEEDED        = 0x0A,
    BT_ERR_CONNECTION_ALREADY_EXISTS                                = 0x0B,
    BT_ERR_COMMAND_DISALLOWED                                       = 0x0C,
    BT_ERR_CONNECTION_REJECTED_DUE_TO_LIMITED_RESOURCES             = 0x0D,
    BT_ERR_CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS              = 0x0E,
    BT_ERR_CONNECTION_REJECTED_DUE_TO_UNACCEPTABLE_BD_ADDR          = 0x0F,
    BT_ERR_CONNECTION_ACCEPT_TIMEOUT_EXCEEDED                       = 0x10,
    BT_ERR_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE                   = 0x11,
    BT_ERR_INVALID_HCI_COMMAND_PARAMETERS                           = 0x12,
    BT_ERR_REMOTE_USER_TERMINATED_CONNECTION                        = 0x13,
    BT_ERR_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES = 0x14,
    BT_ERR_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF     = 0x15,
    BT_ERR_CONNECTION_TERMINATED_BY_LOCAL_HOST                      = 0x16,
    BT_ERR_REPEATED_ATTEMPTS                                        = 0x17,
    BT_ERR_PAIRING_NOT_ALLOWED                                      = 0x18,
    BT_ERR_UNKNOWN_LMP_PDU                                          = 0x19,
    BT_ERR_UNSUPPORTED_REMOTE_FEATURE                               = 0x1A,
    BT_ERR_SCO_OFFSET_REJECTED                                      = 0x1B,
    BT_ERR_SCO_INTERVAL_REJECTED                                    = 0x1C,
    BT_ERR_SCO_AIR_MODE_REJECTED                                    = 0x1D,
    BT_ERR_INVALID_LMP_PARAMETERS                                   = 0x1E,
    BT_ERR_UNSPECIFIED_ERROR                                        = 0x1F,
    BT_ERR_UNSUPPORTED_LMP_PARAMETER_VALUE                          = 0x20,
    BT_ERR_UNSUPPORTED_LL_PARAMETER_VALUE                           = 0x20,
    BT_ERR_ROLE_CHANGE_NOT_ALLOWED                                  = 0x21,
    BT_ERR_LMP_RESPONSE_TIMEOUT                                     = 0x22,
    BT_ERR_LL_RESPONSE_TIMEOUT                                      = 0x22,
    BT_ERR_LMP_ERROR_TRANSACTION_COLLISION                          = 0x23,
    BT_ERR_LL_PROCEDURE_COLLISION                                   = 0x23,
    BT_ERR_LMP_PDU_NOT_ALLOWED                                      = 0x24,
    BT_ERR_ENCRYPTION_MODE_NOT_ACCEPTABLE                           = 0x25,
    BT_ERR_LINK_KEY_CANNOT_BE_CHANGED                               = 0x26,
    BT_ERR_REQUESTED_QOS_NOT_SUPPORTED                              = 0x27,
    BT_ERR_INSTANT_PASSED                                           = 0x28,
    BT_ERR_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                      = 0x29,
    BT_ERR_DIFFERENT_TRANSACTION_COLLISION                          = 0x2A,
    BT_ERR_QOS_UNACCEPTABLE_PARAMETER                               = 0x2C,
    BT_ERR_QOS_REJECTED                                             = 0x2D,
    BT_ERR_CLASSIFICATION_NOT_SUPPORTED                             = 0x2E,
    BT_ERR_INSUFFICIENT_SECURITY                                    = 0x2F,
    BT_ERR_PARAMETER_OUT_OF_MANDATORY_RANGE                         = 0x30,
    BT_ERR_ROLE_SWITCH_PENDING                                      = 0x32,
    BT_ERR_RESERVED_SLOT_VIOLATION                                  = 0x34,
    BT_ERR_ROLE_SWITCH_FAILED                                       = 0x35,
    BT_ERR_EXTENDED_INQUIRY_RESPONSE_TOO_LARGE                      = 0x36,
    BT_ERR_SECURE_SIMPLE_PAIRING_NOT_SUPPORTED_BY_HOST              = 0x37,
    BT_ERR_HOST_BUSY_PAIRING                                        = 0x38,
    BT_ERR_CONNECTION_REJECTED_DUE_TO_NO_SUITABLE_CHANNEL_FOUND     = 0x39,
    BT_ERR_CONTROLLER_BUSY                                          = 0x3A,
    BT_ERR_UNACCEPTABLE_CONNECTION_PARAMETERS                       = 0x3B,
    BT_ERR_ADVERTISING_TIMEOUT                                      = 0x3C,
    BT_ERR_CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE                 = 0x3D,
    BT_ERR_CONNECTION_FAILED_TO_BE_ESTABLISHED                      = 0x3E,
    BT_ERR_CONNECTION_FAILED_TO_SYNCHRONIZATION_TIMEOUT             = 0x3E,
    // BT_ERR_MAC_CONNECTION_FAILED = 0x3F,     // This section is no longer used. core v6.0
    BT_ERR_COARSE_CLOCK_ADJUSTMENT_REJECTED_BUT_WILL_TRY_TO_ADJUST_USING_CLOCK_DRAGGING = 0x40,
    BT_ERR_TYPE0_SUBMAP_NOT_DEFINED                                                     = 0x41,
    BT_ERR_UNKNOWN_ADVERTISING_IDENTIFIER                                               = 0x42,
    BT_ERR_LIMIT_REACHED                                                                = 0x43,
    BT_ERR_OPERATION_CANCELLED_BY_HOST                                                  = 0x44,
    BT_ERR_PACKET_TOO_LONG                                                              = 0x45,
    BT_ERR_TOO_LATE                                                                     = 0x46,
    BT_ERR_TOO_EARLY                                                                    = 0x47,
    BT_ERR_INSUFFICIENT_CHANNELS                                                        = 0x48,
};

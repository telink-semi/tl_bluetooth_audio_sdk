/********************************************************************************************************
 * @file    vcs.h
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

#define VCS_MIN_VOLUME_SETTING   0
#define VCS_MAX_VOLUME_SETTING   255

#define VCS_MUTE_STATE_NOT_MUTED 0
#define VCS_MUTE_STATE_MUTED     1

/* Application error codes(VCS) */
enum vcs_error_code
{
    VCS_ERROR_CODE_INVALID_CHANGE_COUNTER = 0x80,
    VCS_ERROR_CODE_OPCODE_NOT_SUPPORTED   = 0x81,
};

/* Volume Control Point  request opcodes */
enum vcs_volume_control_opcode
{
    VCS_OPCODE_RELATIVE_VOLUME_DOWN        = 0x00,
    VCS_OPCODE_RELATIVE_VOLUME_UP          = 0x01,
    VCS_OPCODE_UNMUTE_RELATIVE_VOLUME_DOWN = 0x02,
    VCS_OPCODE_UNMUTE_RELATIVE_VOLUME_UP   = 0x03,
    VCS_OPCODE_SET_ABSOLUTE_VOLUME         = 0x04,
    VCS_OPCODE_UNMUTE                      = 0x05,
    VCS_OPCODE_MUTE                        = 0x06,
    VCS_OPCODE_MAX,
};

struct vcs_volume_state_format
{
    uint8_t volume_setting; /** < Volume setting, range 0-255. */
    uint8_t mute;           /** < Mute state, 0=unmuted, 1=muted. */
    uint8_t change_counter; /** < Counter to keep track of changes. */
};

struct vcs_volume_flags
{
    uint8_t persisted : 1; /** < 0x00=Reset Volume Setting, 0x01=User Set Volume Setting. */
    uint8_t rfu       : 7;
};

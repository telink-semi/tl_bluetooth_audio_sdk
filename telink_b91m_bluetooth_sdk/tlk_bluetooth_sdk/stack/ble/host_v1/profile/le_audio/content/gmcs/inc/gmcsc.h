/********************************************************************************************************
 * @file    gmcsc.h
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

enum gmcsc_error_code
{
    BLE_GMCSC_ERR_START = 0x80,
    BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE,
    BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY,
    BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS,
    BLE_GMCSC_ERR_INVALID_PARAMETER,
};

enum ble_gmcsc_event_id
{
    GMCSC_EVT_ID_,
};

typedef void (*ble_gmcsc_event_callback)(uint16_t conn_handle, enum ble_gmcsc_event_id event_id, const void *event_msg);

struct ble_gmcsc_register_param
{
};

/**
 *   @brief Register the LE Audio general media control service client control.
 *
 *   @param[in] param Pointer to the structure containing the initial values for the service.
 *
 *   @note This function should be called before any other function in the GMCS module.
 *
 *   @return None.
 */
void ble_lea_register_GMCS_control_client(const struct ble_gmcsc_register_param *param);

/**
 *   @brief Register the LE Audio general media control service client event callback.
 *
 *   @param[in] event_callback Pointer to the callback function to be called when an event occurs.
 *
 *   @return None.
 */
void ble_lea_register_GMCS_client_event_callback(ble_gmcsc_event_callback event_callback);

/**
 *   @brief Write a media control opcode play.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Play) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.1
 *         If the Media State characteristic value is Paused or Seeking, the media player shall start playing
 *            the current track and the Media State characteristic value shall be set to Playing.
 *         If the Media State characteristic value is Playing, the Media State characteristic value remains set to playing.
 */
int ble_gmcsc_write_play(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode pause.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Pause) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.2
 *          If the Media State characteristic value is Playing, the media player shall pause playing
 *            the current track and the Media State characteristic value shall be set to Paused.
 *          If the Media State characteristic value is Seeking, the media player shall stop seeking, set the current
 *            track and track position as a result of seeking, and the Media State characteristic value shall be set to Paused.
 *          If the Media State characteristic value is Paused, the Media State characteristic value remains set to Paused.
 */
int ble_gmcsc_write_pause(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode fast rewind.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Fast Rewind) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.3
 *         If the Media State characteristic is Paused or Playing, the media player shall move the current track
 *            position backwards. The media player should stop playing the current track, set the Media State characteristic
 *            to Seeking, and move the current track position backwards at a speed determined by the implementation.
 *         If the Media State characteristic value is Seeking, the seeking speed should be adjusted in the negative direction.
 *            The method by which the media player handles multiple Fast Rewind opcode writes is left up to the implementation.
 */
int ble_gmcsc_write_fast_rewind(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode fast forward.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Fast Forward) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.4
 *         If the Media State characteristic value is Paused or Playing, the media player shall move the current track
 *            position forward.The media player should stop playing the current track, set the Media State characteristic
 *            to Seeking, and move the current track position forward at a speed determined by the implementation.
 *         If the Media State characteristic is Seeking, the seeking speed should be adjusted in the positive direction.
 *            The method by which the media player handles multiple Fast Forward opcode writes is left up to the implementation.
 */
int ble_gmcsc_write_fast_forward(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode stop.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Stop) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.5
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the media player shall stop
 *            any activity and the Media State characteristic shall be set to Paused.
 *            The track position shall be set to the beginning of the current track.
 */
int ble_gmcsc_write_stop(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode move relative.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] offset The offset value to move the current track position.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Move Relative) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.6
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the track position of the current
 *            track shall be set to the current track position in addition to the offset.
 *         The current track position shall not move before the start of the track and shall not move past the end of
 *            the track. The current track position shall not wrap.
 *         If the addition of the offset to the current track position results in a negative track position,
 *            the Track Position characteristic shall be set to 0.
 *         If the addition of the offset to the current track position results in a track position past the end of
 *            the current track, the Track Position characteristic shall be set to the end of the track.
 */
int ble_gmcsc_write_move_relative(uint16_t conn_handle, int32_t offset);

/**
 *   @brief Write a media control opcode previous segment.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Previous Segment) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.7
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track position shall be
 *            set to either the starting position of the previous segment of this track or the starting position of
 *            the current segment, as determined by the implementation.
 *         The previous segment is defined as the segment with a segment position less than the current track position.
 *         The comparison should be to a reasonable user margin of error such that if the Previous Segment opcode is
 *            used within a few seconds of the start of a segment, then the previous segment is the segment before the
 *            current segment, but after a few seconds, the previous segment is the start of the current segment.
 *         The exact time used in the margin of error is an implementation detail.
 *         If the opcode is received while in the first segment, the current track position shall be set to the
 *            starting position of the first segment.
 *         The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_previous_segment(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode next segment.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Next Segment) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.8
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track position shall be
 *           set to the starting position of the next segment of this track. The next segment is defined as the segment
 *           with a segment position greater than the current position within the current track. If the opcode is received
 *           while in the last segment, the current track position shall be set to the position of the end of the segment.
 *         The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_next_segment(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode first segment.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(First Segment) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.9
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track position shall be
 *           set to the starting position of the first segment of this track.
 *         The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_first_segment(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode last segment.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Last Segment) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.10
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track position shall be
 *           set to the starting position of the last segment of this track.
 *         The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_last_segment(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode goto segment.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] n The segment number to go to.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Goto Segment) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.11
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track position shall be
 *           set to the starting position of the nth segment within the current track. If the value n is a positive number,
 *           setting the current track position is equivalent to sending the First Segment opcode once, followed by
 *           the Next Segment opcode n-1 times. If the value n is a negative number setting, the current track position is
 *           equivalent to sending the Last Segment opcode once, followed by the Previous Segment opcode n-1 times.
 *         If the value of n is zero, the current track position shall not change.
 *         The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_goto_segment(uint16_t conn_handle, int32_t n);

/**
 *   @brief Write a media control opcode previous track.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Previous Track) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.12
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track shall be
 *          set to either the previous track based on the playing order or remain on the current track, as determined by
 *          the implementation. The Track Position characteristic shall be set to 0. The resulting Media State characteristic
 *          value is left up to the implementation.
 */
int ble_gmcsc_write_previous_track(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode next track.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Next Track) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.13
 *          If the Media State characteristic value is Playing, Paused, or Seeking, the current track shall be
 *            set to the next track based on the playing order and the Track Position shall be set to 0.
 *          The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_next_track(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode first track.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(First Track) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.14
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track shall be
 *            set to the first track based on the playing order and the Track Position shall be set to 0.
 *          The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_first_track(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode last track.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Last Track) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.15
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track shall be
 *            set to the last track based on the playing order and the Track Position shall be set to 0.
 *          The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_last_track(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode goto track.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] n The track number to go to.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Goto Track) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.16
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current track shall be
 *            set to the nth track based on the playing order. If the value n is a positive number, setting the
 *            current track is equivalent to sending the First Track opcode once, followed by the Next Track opcode n-1 times.
 *          If the value n is a negative number, then setting the current track is equivalent to sending the Last Track
 *            opcode once, followed by the Previous Track opcode n-1 times. If the value n is zero, the current track
 *            shall not be changed. In all cases, the Track Position shall be set to 0.
 *          The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_goto_track(uint16_t conn_handle, int32_t n);

/**
 *   @brief Write a media control opcode previous group.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Previous Group) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.17
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current group shall be
 *           set to the previous group in the current parent group. The current track shall be set to the first track
 *           of the current group resulting from the Previous Group command and the Track Position shall be set to 0.
 *          The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_previous_group(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode next group.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Next Group) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.18
 *          If the Media State characteristic value is Playing, Paused, or Seeking, the current group shall be
 *            set to the next group in the current parent group. The current track shall be set to the first track of
 *            the current group resulting from the Next Group command and the Track Position shall be set to 0.
 *           The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_next_group(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode first group.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(First Group) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.19
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current group shall be
 *           set to the first group in the current parent group. If the current group is already the first group
 *           within the current parent group, the current group shall not be changed. The current track shall be
 *           set to the first track of the current group resulting from the First Group command and the Track Position
 *           shall be set to 0. The resulting Media State characteristic value is left up to the implementation.
 */
int ble_gmcsc_write_first_group(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode last group.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Last Group) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.20
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current group shall be
 *           set to the last group in the current parent group. If the current group is already the last group within
 *           the current parent group, the current group shall not be changed. The current track shall be set to the
 *           first track of the current group resulting from the Last Group command and the Track Position shall be set to 0.
 *           The resulting Media State characteristic value is left up to the implementation.
 *
 */
int ble_gmcsc_write_last_group(uint16_t conn_handle);

/**
 *   @brief Write a media control opcode goto group.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] n The group number to go to.
 *
 *   @return BLE_HOST_ERR_SUCC if the Media Control(Goto Group) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDOPCODE if remote device does not support the opcode.
 *      - BLE_GMCSC_ERR_MEDIA_CONTROL_BUSY if the Media Control is already in progress(not receive notification).
 *
 *   @note This opcode is define in MCS_v1.0, section 3.18.1.21
 *         If the Media State characteristic value is Playing, Paused, or Seeking, the current group shall be
 *           set to the nth group of the current parent group. If the value n is a positive number, setting the
 *           current group is equivalent to sending the First Group opcode once, followed by the Next Group opcode n-1 times.
 *         If the value n is a negative number, setting the current group is equivalent to sending the Last Group opcode once,
 *           followed by the Previous Group opcode n-1 times. The current track shall be set to the first track of the
 *           current group resulting from the Goto Group command and the Track Position shall be set to 0.
 *         If the value n is zero, the current group shall not be changed.
 *         The resulting Media State characteristic value is left up to the implementation.
 *
 */
int ble_gmcsc_write_goto_group(uint16_t conn_handle, int32_t n);

/**
 *   @brief Write a search control opcode track name.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] track_name The track name to search, max length is 62 bytes.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(Track Name) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 *      - BLE_GMCSC_ERR_INVALID_PARAMETER if the track name is NULL.
 */
int ble_gmcsc_write_search_track_name(uint16_t conn_handle, char *track_name);

/**
 *   @brief Write a search control opcode artist name.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] artist_name The artist name to search, max length is 62 bytes.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(Artist Name) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 *      - BLE_GMCSC_ERR_INVALID_PARAMETER if the artist name is NULL.
 */
int ble_gmcsc_write_search_artist_name(uint16_t conn_handle, char *artist_name);

/**
 *   @brief Write a search control opcode album name.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] album_name The album name to search, max length is 62 bytes.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(Album Name) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 *      - BLE_GMCSC_ERR_INVALID_PARAMETER if the album name is NULL.
 */
int ble_gmcsc_write_search_album_name(uint16_t conn_handle, char *album_name);

/**
 *   @brief Write a search control opcode group name.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] group_name The group name to search, max length is 62 bytes.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(Group Name) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 *      - BLE_GMCSC_ERR_INVALID_PARAMETER if the group name is NULL.
 */
int ble_gmcsc_write_search_group_name(uint16_t conn_handle, char *group_name);

/**
 *   @brief Write a search control opcode earliest year.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] earliest_year The earliest year to search, max length is 4 bytes.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(Earliest Year) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 *      - BLE_GMCSC_ERR_INVALID_PARAMETER if the earliest year is NULL.
 */
int ble_gmcsc_write_search_earliest_year(uint16_t conn_handle, char *earliest_year);

/**
 *   @brief Write a search control opcode latest year.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] latest_year The latest year to search, max length is 4 bytes.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(Latest Year) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 *      - BLE_GMCSC_ERR_INVALID_PARAMETER if the latest year is NULL.
 */
int ble_gmcsc_write_search_latest_year(uint16_t conn_handle, char *latest_year);

/**
 *   @brief Write a search control opcode genre.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *   @param[in] genre The genre to search, max length is 62 bytes.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(Genre) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 *      - BLE_GMCSC_ERR_INVALID_PARAMETER if the genre is NULL.
 */
int ble_gmcsc_write_search_genre(uint16_t conn_handle, char *genre);

/**
 *   @brief Write a search control opcode only tracks.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(only tracks) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 */
int ble_gmcsc_write_search_only_tracks(uint16_t conn_handle);

/**
 *   @brief Write a search control opcode only groups.
 *
 *   @param[in] conn_handle ACL Connection handle.
 *
 *   @return BLE_HOST_ERR_SUCC if the Search Control(only groups) is sent successfully.
 *      - BLE_PRF_ERR_INVALID_CONN_HANDLE if the connection handle is invalid.
 *      - BLE_PRF_ERR_REMOTE_NOT_FOUND_ATTRIBUTE if remote attribute is not found.
 *      - BLE_GMCSC_ERR_REMOTE_UNSUPPORTEDSEARCH_RESULTS if remote device does not support search results.
 */
int ble_gmcsc_write_search_only_groups(uint16_t conn_handle);

/********************************************************************************************************
 * @file    tlkdbg_sync_stream_inner.h
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

typedef struct 
{
    void (*init)(void);
    void (*send)(uint8_t type, void *pData, uint16_t dataLen);
}tlkdbg_sync_stream_t;

/**
 * @brief       Send log message from N22 core to shared memory
 * @param[in]   type    - log type
 * @param[in]   pData   - pointer to the data to send
 * @param[in]   dataLen - length of the data to send
 * @return      none.
 */
void tlkdbg_n22core_write(uint8_t type, void *pData, uint16_t dataLen);



/********************************************************************************************************
 * @file    tlkmdi_tinySql_disk_audio.c
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
#include "tl_common.h"
#include "tlkapi/tlkapi.h"
#include "tlkmw/tinysql/tlkmdi_tinySql.h"
#include "tlkmw/tinysql/tlkmdi_tinySql_inner.h"

#if (TLK_MW_TINYSQL_ENABLE)
/**
 * @brief   Audio disk data structure for TinySQL.
 * @note    Contains equalizer data and its length.
 */
typedef struct
{
    uint16_t resv;                               //!< Reserved field
    uint16_t eqDataLen;                          //!< Length of equalizer data
    uint8_t  eq[TLKMDI_TINYSQL_EQ_DATA_MAX_LEN]; //!< Equalizer data buffer
} sql_audio_disk_format_t;

/******************************************************************************
                           private code begin
******************************************************************************/
static sql_audio_disk_format_t sSqlAudDiskData   = {0}; //!< Global instance of audio disk data
static tlkapi_save_ctrl_t      sSqlDiskAudioCtrl = {0}; //!< Save control structure for audio data

/**
 * @brief   Operates the mutex for audio disk operations.
 * @param[in] isLock - true to lock the mutex, false to unlock.
 * @return  none.
 */
static void tlkmdi_tinySql_audioDisk_mutexOperate(uint8_t isLock)
{
    tlkmdi_tinySql_mutex_operate(isLock);
}

/**
 * @brief   Resets the audio disk data to initial state.
 * @return  none.
 * @note    Clears all audio disk data by setting it to zero.
 */
static void tlkmdi_tinySql_audioDiskReset(void)
{
    memset(&sSqlAudDiskData, 0, sizeof(sSqlAudDiskData));
}

/**
 * @brief   Initializes the audio disk module.
 * @return  none.
 * @note    Loads audio data from flash storage using tlkapi_save3 mechanism.
 *          If loading fails, resets the audio disk data.
 */
static void tlkmdi_tinySql_audioDiskInit(void)
{
    unsigned int saveAddress = tlkmdi_tinySql_getSaveAddr(TLKMDI_TINYSQL_DISK5_ADDR);
    tlkapi_save3_init(&sSqlDiskAudioCtrl, TLKMDI_TINYSQL_SAVE_SIGN, TLKMDI_TINYSQL_VER, sizeof(sSqlAudDiskData), saveAddress + 4096 * 0, saveAddress + 4096 * 1);
    int ret = tlkapi_save3_load(&sSqlDiskAudioCtrl, (uint8_t *)&sSqlAudDiskData, sizeof(sSqlAudDiskData));
    if (ret < (int)sizeof(sSqlAudDiskData)) {
        tlkmdi_tinySql_audioDiskReset();
    }
}

/**
 * @brief   Saves the audio disk data to flash storage.
 * @return  none.
 * @note    Uses smart save mechanism which automatically decides whether to save directly 
 *          or migrate based on available space.
 */
static void tlkmdi_tinySql_audioDiskSave(void)
{
    tlkapi_save3_smartSave(&sSqlDiskAudioCtrl, (uint8_t *)&sSqlAudDiskData, sizeof(sSqlAudDiskData));
}

/**
 * @brief   Restores the audio disk data to factory defaults.
 * @return  none.
 * @note    Cleans the save sector, resets audio disk data and requests a new save operation.
 */
static void tlkmdi_tinySql_audioDiskRestore(void)
{
    tlkapi_save3_clean(&sSqlDiskAudioCtrl);
    tlkmdi_tinySql_audioDiskReset();
    tlkmdi_tinySql_requestSave(tinySql_audioSaveIndex);
}

/**
 * @brief   Audio disk operations structure.
 * @note    Implements the tinySqlDisk_t interface for audio data storage.
 */
const tinySqlDisk_t tinySql_audio_disk = {
    .init           = tlkmdi_tinySql_audioDiskInit,
    .restoreFactory = tlkmdi_tinySql_audioDiskRestore,
    .save           = tlkmdi_tinySql_audioDiskSave,
};

/******************************************************************************
                           private code end
******************************************************************************/

/**
 * @brief Retrieves the pointer to the equalizer parameters.
 * 
 * @param[out] len If not NULL, the actual length of the equalizer data will be returned through this parameter.
 * @returns   Returns the pointer to the equalizer parameters; if the length of the equalizer data is 0, NULL is returned.
 */
const void *tlkmdi_tinySql_getEqParamPointer(uint16_t *len)
{
    void *ptr = NULL;
    if (sSqlAudDiskData.eqDataLen != 0) {
        ptr = (void *)sSqlAudDiskData.eq;
    }
    if (len) {
        *len = sSqlAudDiskData.eqDataLen;
    }
    return ptr;
}

/**
 * @brief Retrieves the equalizer parameter data.
 * 
 * @param[out] recBuffer Buffer to store the retrieved equalizer parameter data.
 * @param[in] bufferLen  Size of the buffer.
 * @returns   Returns the length of the retrieved data; if an error occurs, the corresponding error code is returned.
 */
int tlkmdi_tinySql_getEqParam(void *recBuffer, uint16_t bufferLen)
{
    if (sSqlAudDiskData.eqDataLen == 0) {
        return -TLK_ENODATA;
    }
    if (recBuffer == NULL) {
        return -TLK_EPARAM;
    }
    if (bufferLen < sSqlAudDiskData.eqDataLen) {
        return -TLK_EOVERFLOW;
    }
    tlkmdi_tinySql_audioDisk_mutexOperate(true);
    memcpy(recBuffer, sSqlAudDiskData.eq, sSqlAudDiskData.eqDataLen);
    tlkmdi_tinySql_audioDisk_mutexOperate(false);
    return sSqlAudDiskData.eqDataLen;
}

/**
 * @brief Saves the equalizer parameter data.
 * 
 * @param[in] pEqParam Pointer to the equalizer parameter data to be saved.
 * @param[in] dataLen  Length of the equalizer parameter data to be saved.
 * @returns   Returns the length of the saved data; if an error occurs, the corresponding error code is returned.
 */
int tlkmdi_tinySql_saveEqParam(void *pEqParam, uint16_t dataLen)
{
    if (pEqParam == NULL) {
        return -TLK_EPARAM;
    }
    if (dataLen > TLKMDI_TINYSQL_EQ_DATA_MAX_LEN) {
        return -TLK_EOVERFLOW;
    }
    tlkmdi_tinySql_audioDisk_mutexOperate(true);
    memmove(sSqlAudDiskData.eq, pEqParam, dataLen);
    sSqlAudDiskData.eqDataLen = dataLen;
    tlkmdi_tinySql_requestSave(tinySql_audioSaveIndex);
    tlkmdi_tinySql_audioDisk_mutexOperate(false);
    return dataLen;
}

#endif

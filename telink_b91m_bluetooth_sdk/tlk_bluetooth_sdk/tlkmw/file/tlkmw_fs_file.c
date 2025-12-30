/********************************************************************************************************
 * @file    tlkmw_fs_file.c
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
#if TLK_CFG_FS_ENABLE
#include "tlkapi/tlkapi.h"
#include "tlklib/fatfs/ff/source/ff.h"
#include <stdio.h>
#include <stdarg.h>

#define TLKMW_FS_DEBUG_LOG_ENABLE 1
#if TLKMW_FS_DEBUG_LOG_ENABLE
#define tlkmw_fs_printf(fmt, ...) tlkapi_trace(0xFFFFFFFF, "[FS]", fmt, ##__VA_ARGS__)
#else
#define tlkmw_fs_printf(fmt, ...) ;
#endif

/**
 * @brief       This function is the core open-file routine (internal).
 * @param[in]   file      File control block handle obtained from tlkmw_fs_mallocFileCtrl().
 * @param[in]   filePath  Null-terminated path string in FATFS format.
 * @param[in]   flag      FATFS open flags, e.g. FA_WRITE | FA_CREATE_ALWAYS.
 * @return      TLK_ENONE on success, -TLK_EFAIL on any FATFS error.
 */
static int tlkmw_fs_open_core(TlkOsFileCtrlHandle_t file, char *filePath, uint8_t flag)
{
    FIL    *fCtrl = (FIL *)file;
    FRESULT res   = f_open(fCtrl, (const TCHAR *)filePath, flag);

    if (res != FR_OK) {
        tlkmw_fs_printf("open file %s failed, %d", (char *)filePath, res);
    } else {
        tlkmw_fs_printf("open file %s success", (char *)filePath);
    }
    return (res == FR_OK) ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief       This function allocates memory for a file control block.
 * @param[in]   none.
 * @return      Pointer to the allocated FIL structure (NULL if out of memory).
 */
TlkOsFileCtrlHandle_t tlkmw_fs_mallocFileCtrl(void)
{
    return tlkos_calloc(sizeof(FIL));
}

/**
 * @brief       This function releases memory previously allocated by tlkmw_fs_mallocFileCtrl().
 * @param[in]   file  File control block handle to free.
 * @return      none.
 */
void tlkmw_fs_freeFileCtrl(TlkOsFileCtrlHandle_t file)
{
    tlkos_free(file);
}

/**
 * @brief       This function checks if a file exists in the given path.
 * @param[in]   filePath  Path of the file.
 * @return      True means existed,false means not existed.
 */
bool tlkmw_fs_isFileExisted(char *filePath)
{
    FIL     fCtrl = {0};
    FRESULT res   = f_open(&fCtrl, (const TCHAR *)filePath, FA_OPEN_EXISTING);
    if (res != FR_OK) {
        return false;
    }
    f_close(&fCtrl);
    return true;
}

/**
 * @brief       This function creates a new file or truncates an existing one.
 * @param[in]   file      File control block handle.
 * @param[in]   filePath  Path of the file to create.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_createNewFile(TlkOsFileCtrlHandle_t file, char *filePath)
{
    return tlkmw_fs_open_core(file, filePath, FA_WRITE | FA_CREATE_ALWAYS);
}

/**
 * @brief       This function opens an existing file for append.
 * @param[in]   file      File control block handle.
 * @param[in]   filePath  Path of the file to open.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_openExist(TlkOsFileCtrlHandle_t file, char *filePath)
{
    return tlkmw_fs_open_core(file, filePath, FA_OPEN_EXISTING | FA_WRITE | FA_READ);
}

/**
 * @brief       This function opens a file for append (creates it if it does not exist).
 * @param[in]   file      File control block handle.
 * @param[in]   filePath  Path of the file to open.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_openForAppend(TlkOsFileCtrlHandle_t file, char *filePath)
{
    return tlkmw_fs_open_core(file, filePath, FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND);
}

/**
 * @brief       This function opens an existing file for read (creates it if it does not exist).
 * @param[in]   file      File control block handle.
 * @param[in]   filePath  Path of the file to open.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_openForRead(TlkOsFileCtrlHandle_t file, char *filePath)
{
    return tlkmw_fs_open_core(file, filePath, FA_READ | FA_OPEN_ALWAYS);
}

/**
 * @brief       This function writes data to an open file.
 * @param[in]   file      File control block handle.
 * @param[in]   pdata     Pointer to the data buffer to write.
 * @param[in]   writeLen  Number of bytes to write.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_write(TlkOsFileCtrlHandle_t file, uint8_t *pdata, uint32_t writeLen)
{
    FIL *fCtrl = (FIL *)file;
    UINT bw;
    while (writeLen) {
        uint32_t packageLen = writeLen > 512 ? 512 : writeLen;
        FRESULT  res        = f_write(fCtrl, pdata, packageLen, &bw);
        if (res != FR_OK) {
            tlkmw_fs_printf("write file failed, %d", res);
            return -TLK_EFAIL;
        }
        writeLen -= packageLen;
        pdata += packageLen;
    }
    return TLK_ENONE;
}

/**
 * @brief       This function reads data from an open file.
 * @param[in]   file            File control block handle.
 * @param[out]  pBuffer         Buffer to store the read data.
 * @param[in]   readReqLen      Number of bytes requested to read.
 * @param[out]  readResultLen   Actual number of bytes read.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_read(TlkOsFileCtrlHandle_t file, uint8_t *pBuffer, uint32_t readReqLen, uint32_t *readResultLen)
{
    UINT br    = 0;
    FIL *fCtrl = (FIL *)file;

    uint32_t readLen = 0;
    while (readReqLen) {
        uint32_t len = readReqLen > 512 ? 512 : readReqLen;
        FRESULT  res = f_read(fCtrl, pBuffer, len, &br);
        if (res != FR_OK) {
            tlkmw_fs_printf("read file failed, %d", res);
            return -TLK_EFAIL;
        }
        readReqLen -= len;
        readLen += br;
        pBuffer += br;
        if (br == 0) {
            break;
        }
    }
    if (readResultLen) {
        *readResultLen = readLen;
    }
    return TLK_ENONE;
}

/**
 * @brief       This function moves the file read/write pointer to a specified byte offset.
 * @param[in]   file      File control block handle.
 * @param[in]   offset    Byte offset from the beginning of the file.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_seek(TlkOsFileCtrlHandle_t file, uint32_t offset)
{
    FIL    *fCtrl = (FIL *)file;
    FRESULT res   = f_lseek(fCtrl, offset);

    if (res != FR_OK) {
        tlkmw_fs_printf("seek file failed, offset=%lu, res=%d", offset, res);
        return -TLK_EFAIL;
    }

    tlkmw_fs_printf("seek file success, offset=%lu", offset);
    return TLK_ENONE;
}

/**
 * @brief       This function closes an open file.
 * @param[in]   file  File control block handle.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_close(TlkOsFileCtrlHandle_t file)
{
    FIL    *fCtrl = (FIL *)file;
    FRESULT res   = f_close(fCtrl);

    if (res != FR_OK) {
        tlkmw_fs_printf("File close failed %d", res);
    } else {
        tlkmw_fs_printf("File close success !");
    }

    return (res == FR_OK) ? TLK_ENONE : -TLK_EFAIL;
}

/**
 * @brief       This function deletes a file.
 * @param[in]   filePath  Null-terminated file path in FATFS format.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_deleteFile(const char *filePath)
{
    FRESULT res = f_unlink((const TCHAR *)filePath);
    if (res != FR_OK) {
        tlkmw_fs_printf("delete file %s failed, %d", filePath, res);
        return -TLK_EFAIL;
    }
    tlkmw_fs_printf("delete file %s success", filePath);
    return TLK_ENONE;
}

/**
 * @brief       This function renames/moves a file.
 * @param[in]   oldPath  Original file path.
 * @param[in]   newPath  New file path.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_renameFile(const char *oldPath, const char *newPath)
{
    FRESULT res = f_rename((const TCHAR *)oldPath, (const TCHAR *)newPath);
    if (res != FR_OK) {
        tlkmw_fs_printf("rename file %s -> %s failed, %d", oldPath, newPath, res);
        return -TLK_EFAIL;
    }
    tlkmw_fs_printf("rename file %s -> %s success", oldPath, newPath);
    return TLK_ENONE;
}

/**
 * @brief       This function gets the size of a file based on its file path.
 * @param[in]   filePath  Null-terminated path string of the file in FATFS format.
 * @param[out]  fileSize  Pointer to a variable where the file size will be stored.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_getFileSize(const char *filePath, uint32_t *fileSize)
{
    if (filePath == NULL || fileSize == NULL) {
        return -TLK_EFAIL;
    }

    FILINFO info = {0};
    FRESULT res  = f_stat((const TCHAR *)filePath, &info);
    if (res != FR_OK) {
        return -TLK_EFAIL;
    }

    *fileSize = info.fsize;
    return TLK_ENONE;
}

#endif
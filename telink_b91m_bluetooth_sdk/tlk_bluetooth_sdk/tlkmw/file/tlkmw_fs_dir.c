/********************************************************************************************************
 * @file    tlkmw_fs_dir.c
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
#include "tlkmw/tlkmw.h"
#include "tlklib/fatfs/ff/source/ff.h"

typedef struct
{
    DIR  dir;
    char ext[16];
} TlkMwFsDirIterCore_t;

/**
 * @brief       This function converts a string to lowercase.
 * @param[out]  dst    - destination buffer.
 * @param[in]   src    - source string.
 * @param[in]   max    - maximum length.
 * @return      none.
 */
static void tlkmw_fs_dir_str2Lower(char *dst, const char *src, uint32_t max)
{
    if (dst == NULL || max == 0) {
        return;
    }
    if (src == NULL) {
        src = "";
    }
    uint32_t i = 0;
    while (i < max - 1 && src[i]) {
        uint8_t c = (uint8_t)src[i];
        if (c >= 'A' && c <= 'Z') {
            c += 32;
        }
        dst[i] = (char)c;
        ++i;
    }
    dst[i] = '\0';
}

/**
 * @brief       This function checks if the file extension matches.
 * @param[in]   fname    - file name.
 * @param[in]   ext      - extension to match.
 * @return      Returns true if matched, otherwise false.
 */
static bool tlkmw_fs_dir_extIsMatch(const char *fname, const char *ext)
{
    if (!fname) {
        return false;
    }
    if (!ext || !*ext) {
        return true;
    }

    const char *dot = strrchr(fname, '.');
    if (!dot || dot == fname) {
        return false;
    }

    const char *fext = dot + 1;

    char fext_lower[16] = {0};
    char ext_lower[16]  = {0};

    tlkmw_fs_dir_str2Lower(fext_lower, fext, sizeof(fext_lower));
    tlkmw_fs_dir_str2Lower(ext_lower, ext, sizeof(ext_lower));

    return strcmp(fext_lower, ext_lower) == 0;
}

/**
 * @brief       This function creates a directory iterator for traversing files with optional extension filter.
 * @param[in]   dirPath  Null-terminated directory path, e.g. "0:/log".
 * @param[in]   ext      File extension to match (case-insensitive, must include dot, e.g. ".txt").
 *                       Pass NULL or "" to disable filtering.
 * @return      Valid iterator handle on success; NULL on any error (out of memory or directory open failed).
 */
TlkOsFsIter_t tlkmw_fs_dir_fileIterCreate(const char *dirPath, const char *ext)
{
    TlkMwFsDirIterCore_t *it = tlkos_calloc(sizeof(TlkMwFsDirIterCore_t));
    if (it == NULL) {
        return NULL;
    }

    tlkmw_fs_dir_str2Lower(it->ext, ext, sizeof(it->ext));

    if (f_opendir(&it->dir, dirPath) != FR_OK) {
        tlkos_free(it);
        return NULL;
    }
    return (TlkOsFsIter_t)it;
}

/**
 * @brief       This function gets the next file (non-directory) from the iterator.
 * @param[in]   it       Iterator handle obtained from tlkmw_fs_dir_fileIterCreate().
 * @param[out]  nameBuf  Buffer to receive the null-terminated file name.
 * @param[in]   bufLen   Size of nameBuf in bytes (must ≥ 1).
 * @return      TLK_ENONE     another file found,
 *              -TLK_EEMPTY   no more files,
 *              -TLK_EFAIL    FatFS error.
 */
int tlkmw_fs_dir_fileIterNext(TlkOsFsIter_t it, char *nameBuf, uint32_t bufLen)
{
    if (!it || !nameBuf || bufLen == 0) {
        return -TLK_EFAIL;
    }
    TlkMwFsDirIterCore_t *iter = (TlkMwFsDirIterCore_t *)it;
    FILINFO               fno  = {0};

    while (1) {
        fno.fname[0] = 0;
        if (f_readdir(&iter->dir, &fno) != FR_OK) {
            return -TLK_EFAIL;
        }
        if (fno.fname[0] == '.') {
            continue;
        }
        if (fno.fname[0] == 0) {
            return -TLK_EEMPTY; /* end of dir */
        }

        if (fno.fattrib & AM_DIR) {
            continue;
        }
        if (!tlkmw_fs_dir_extIsMatch(fno.fname, iter->ext)) {
            continue;
        }

        strncpy(nameBuf, fno.fname, bufLen);
        nameBuf[bufLen - 1] = '\0';
        return TLK_ENONE;
    }
}

/**
 * @brief       This function destroys the iterator and releases all resources.
 * @param[in]   it  Iterator handle to destroy. 
 * @return      none.
 */
void tlkmw_fs_dir_fileIterDestroy(TlkOsFsIter_t it)
{
    if (it == NULL) {
        return;
    }
    TlkMwFsDirIterCore_t *iter = (TlkMwFsDirIterCore_t *)it;
    f_closedir(&iter->dir);
    tlkos_free(iter);
}

#endif
/********************************************************************************************************
 * @file    tlkmw_fs.h
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

/**
 * @brief       This function initializes the file system.
 * @param[in]   none.
 * @return      none.
 */
void tlkmw_fs_init(void);

/**
 * @brief  Allocates memory for a file control block.
 * @return Pointer to the allocated FIL structure (NULL if out of memory).
 */
/**
 * @brief       This function allocates memory for a file control block.
 * @param[in]   none.
 * @return      Pointer to the allocated FIL structure (NULL if out of memory).
 */
TlkOsFileCtrlHandle_t tlkmw_fs_mallocFileCtrl(void);

/**
 * @brief  Releases memory previously allocated by tlkmw_fs_mallocFileCtrl().
 * @param[in] file  File control block handle to free.
 */
/**
 * @brief       This function releases memory previously allocated by tlkmw_fs_mallocFileCtrl().
 * @param[in]   file  File control block handle to free.
 * @return      none.
 */
void tlkmw_fs_freeFileCtrl(TlkOsFileCtrlHandle_t file);

/**
 * @brief      Is a file has existed in path.
 * @param[in]  filePath  Path of the file.
 * @return     True means existed,false means not existed.
 */
/**
 * @brief       This function checks if a file exists in the given path.
 * @param[in]   filePath  Path of the file.
 * @return      True means existed,false means not existed.
 */
bool tlkmw_fs_isFileExisted(char *filePath);

/**
 * @brief       This function creates a new file or truncates an existing one.
 * @param[in]   file      File control block handle.
 * @param[in]   filePath  Path of the file to create.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_createNewFile(TlkOsFileCtrlHandle_t file, char *filePath);

/**
 * @brief       This function opens an existing file for append.
 * @param[in]   file      File control block handle.
 * @param[in]   filePath  Path of the file to open.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_openExist(TlkOsFileCtrlHandle_t file, char *filePath);

/**
 * @brief       This function opens a file for append (creates it if it does not exist).
 * @param[in]   file      File control block handle.
 * @param[in]   filePath  Path of the file to open.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_openForAppend(TlkOsFileCtrlHandle_t file, char *filePath);

/**
 * @brief       This function opens an existing file for read (creates it if it does not exist).
 * @param[in]   file      File control block handle.
 * @param[in]   filePath  Path of the file to open.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_openForRead(TlkOsFileCtrlHandle_t file, char *filePath);

/**
 * @brief       This function writes data to an open file.
 * @param[in]   file      File control block handle.
 * @param[in]   pdata     Pointer to the data buffer to write.
 * @param[in]   writeLen  Number of bytes to write.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_write(TlkOsFileCtrlHandle_t file, uint8_t *pdata, uint32_t writeLen);

/**
 * @brief       This function reads data from an open file.
 * @param[in]   file            File control block handle.
 * @param[out]  pBuffer         Buffer to store the read data.
 * @param[in]   readReqLen      Number of bytes requested to read.
 * @param[out]  readResultLen   Actual number of bytes read.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_read(TlkOsFileCtrlHandle_t file, uint8_t *pBuffer, uint32_t readReqLen, uint32_t *readResultLen);

/**
 * @brief       This function moves the file read/write pointer to a specified byte offset.
 * @param[in]   file      File control block handle.
 * @param[in]   offset    Byte offset from the beginning of the file.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_seek(TlkOsFileCtrlHandle_t file, uint32_t offset);

/**
 * @brief       This function closes an open file.
 * @param[in]   file  File control block handle.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_close(TlkOsFileCtrlHandle_t file);

/**
 * @brief       This function deletes a file.
 * @param[in]   filePath  Null-terminated file path in FATFS format.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_deleteFile(const char *filePath);

/**
 * @brief       This function renames/moves a file.
 * @param[in]   oldPath  Original file path.
 * @param[in]   newPath  New file path.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_renameFile(const char *oldPath, const char *newPath);

/**
 * @brief       This function gets the size of a file based on its file path.
 * @param[in]   filePath  Null-terminated path string of the file in FATFS format.
 * @param[out]  fileSize  Pointer to a variable where the file size will be stored.
 * @return      TLK_ENONE on success, -TLK_EFAIL otherwise.
 */
int tlkmw_fs_getFileSize(const char *filePath, uint32_t *fileSize);

/*
 * Demo: Create / Write / Read a simple text file
 * ------------------------------------------------
 * File name : "0:/hello.txt"
 * Contents  : "Hello world\r\n"
 * Steps:
 *   1. Allocate file control block with tlkmw_fs_mallocFileCtrl().
 *   2. Open file with tlkmw_fs_createNewFile()  -> creates/overwrites file.
 *   3. Write "Hello world\r\n" into file using tlkmw_fs_write().
 *   4. Close file with tlkmw_fs_close().
 *   5. Re-open file with tlkmw_fs_openForRead().
 *   6. Read back contents via tlkmw_fs_read().
 *   7. Print read buffer to debug port.
 *   8. Close and free resources.
 *
 * Expected result:
 *   open file 0:/hello.txt success
 *   File close success !
 *   open file 0:/hello.txt success
 *   read: Hello world
 *   File close success !
 */

/*
void tlkmw_fs_demo(void)
{
    TlkOsFileCtrlHandle_t file = tlkmw_fs_mallocFileCtrl();
    if (!file) return;
    if (tlkmw_fs_createNewFile(file, "0:/hello.txt") == TLK_ENONE) {
        const char *msg = "Hello world\r\n";
        tlkmw_fs_write(file, (uint8_t *)msg, strlen(msg));
    }
    tlkmw_fs_close(file);

    uint8_t buf[32] = {0};
    uint32_t got = 0;
    if (tlkmw_fs_openForRead(file, "0:/hello.txt") == TLK_ENONE) {
        tlkmw_fs_read(file, buf, sizeof(buf) - 1, &got);
        buf[got] = '\0';
        tlk_printf("read: %s", buf);
        tlkmw_fs_close(file);
    }

    tlkmw_fs_freeFileCtrl(file);
}
*/


/**
 * @brief       This function creates a directory iterator for traversing files with optional extension filter.
 * @param[in]   dirPath  Null-terminated directory path, e.g. "0:/log".
 * @param[in]   ext      File extension to match (case-insensitive, must include dot, e.g. ".txt").
 *                       Pass NULL or "" to disable filtering.
 * @return      Valid iterator handle on success; NULL on any error (out of memory or directory open failed).
 */
TlkOsFsIter_t tlkmw_fs_dir_fileIterCreate(const char *dirPath, const char *ext);

/**
 * @brief       This function gets the next file (non-directory) from the iterator.
 * @param[in]   it       Iterator handle obtained from tlkmw_fs_dir_fileIterCreate().
 * @param[out]  nameBuf  Buffer to receive the null-terminated file name.
 * @param[in]   bufLen   Size of nameBuf in bytes (must ≥ 1).
 * @return      TLK_ENONE     another file found,
 *              -TLK_EEMPTY   no more files,
 *              -TLK_EFAIL    FatFS error.
 */
int tlkmw_fs_dir_fileIterNext(TlkOsFsIter_t it, char *nameBuf, uint32_t bufLen);

/**
 * @brief  Destroy the iterator and release all resources.
 *
 * @param[in]  it  Iterator handle to destroy. 
 */
/**
 * @brief       This function destroys the iterator and releases all resources.
 * @param[in]   it  Iterator handle to destroy. 
 * @return      none.
 */
void tlkmw_fs_dir_fileIterDestroy(TlkOsFsIter_t it);


/*
 * Demo: Iterate over all “.txt” files in the root directory
 * ----------------------------------------------------------
 * Directory : "0:/"
 * Extension : ".txt"
 * Steps:
 *   1. Create iterator with tlkmw_fs_dir_fileIterCreate("0:/", ".txt").
 *   2. Loop tlkmw_fs_dir_fileIterNext() until -TLK_EEMPTY.
 *   3. Print each file name via tlk_printf().
 *   4. Destroy iterator with tlkmw_fs_dir_fileIterDestroy().
 *
 * Expected result (example):
 *   TXT: xxx.txt
 *   TXT: xxx.txt
 */

/*
void tlkmw_fs_dir_txt_demo(void)
{
    TlkOsFsIter_t it = tlkmw_fs_dir_fileIterCreate("0:/", "txt");
    if (!it) {
        tlk_printf("iterator create fail");
        return;
    }

    char name[256 + 1];
    while (tlkmw_fs_dir_fileIterNext(it, name, sizeof(name)) == TLK_ENONE) {
        tlk_printf("TXT: %s", name);
    }

    tlkmw_fs_dir_fileIterDestroy(it);
}
 */
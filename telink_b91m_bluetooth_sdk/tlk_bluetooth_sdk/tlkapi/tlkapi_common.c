/********************************************************************************************************
 * @file    tlkapi_common.c
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
#include "common/types.h"
#include "drivers.h"
#include "tlkapi/tlkapi_common.h"

/**
 * @brief       This function converts UTF-8 encoded string to Unicode.
 * @param[out]  pDst     - the destination buffer for storing Unicode characters.
 * @param[in]   dstLen   - the length of destination buffer.
 * @param[in]   pSrc     - the source buffer containing UTF-8 encoded string.
 * @param[in]   srcLen   - the length of source buffer.
 * @returns     The number of Unicode characters converted. 
 */
int tlkapi_utf8ToUnicode(uint16_t *pDst, uint16_t dstLen, uint8_t *pSrc, uint16_t srcLen)
{
    int      ii, iii;
    uint16_t unicode = 0;
    uint16_t codeLen = 0;

    uint8_t count = 0;

    if (pDst == NULL || dstLen == 0 || pSrc == NULL || srcLen == 0) {
        return 0;
    }

    while (srcLen != 0) {
        // 1. UTF-8 ---> Unicode
        if (0 == (pSrc[0] & 0x80)) {
            // one byte
            codeLen = 1;
            unicode = pSrc[0];
        } else if (0xC0 == (pSrc[0] & 0xE0) && 0x80 == (pSrc[1] & 0xC0)) { // two bytes
            codeLen = 2;
            if (srcLen < codeLen) {
                break;
            }
            unicode = (int)((((int)pSrc[0] & 0x001F) << 6) | ((int)pSrc[1] & 0x003F));
        } else if (0xE0 == (pSrc[0] & 0xF0) && 0x80 == (pSrc[1] & 0xC0) && 0x80 == (pSrc[2] & 0xC0)) { // three  bytes
            codeLen = 3;
            if (srcLen < codeLen) {
                break;
            }
            ii      = (((int)pSrc[0] & 0x000F) << 12);
            iii     = (((int)pSrc[1] & 0x003F) << 6);
            unicode = ii | iii | ((int)pSrc[2] & 0x003F);
            unicode = (int)((((int)pSrc[0] & 0x000F) << 12) | (((int)pSrc[1] & 0x003F) << 6) | ((int)pSrc[2] & 0x003F));
        } else if (0xF0 == (pSrc[0] & 0xF0) && 0x80 == (pSrc[1] & 0xC0) && 0x80 == (pSrc[2] & 0xC0) && 0x80 == (pSrc[3] & 0xC0)) { // four bytes
            codeLen = 4;
            if (srcLen < codeLen) {
                break;
            }
            unicode = (((int)(pSrc[0] & 0x07)) << 18) | (((int)(pSrc[1] & 0x3F)) << 12) | (((int)(pSrc[2] & 0x3F)) << 6) | (pSrc[3] & 0x3F);
        } else {
            break;
        }

        srcLen -= codeLen;
        pSrc += codeLen;
        if (unicode < 0x80) {
            if (count == 0 && unicode == 0x20) {
                continue;
            }
        }
        pDst[count++] = unicode;
        if (count == dstLen) {
            break;
        }
    } // end while

    return count;
}

/**
 * @brief       This function generates random numbers and fills the buffer.
 * @param[out]  pBuff    - the buffer to store random numbers.
 * @param[in]   buffLen  - the length of the buffer.
 * @returns     none. 
 */
void tlkapi_random(uint8_t *pBuff, uint16_t buffLen)
{
    int          i;
    unsigned int randNums = 0;
    /* if len is odd */
    for (i = 0; i < buffLen; i++) {
        if ((i & 3) == 0) {
            randNums = trng_rand();
        }
        pBuff[i] = randNums & 0xff;
        randNums >>= 8;
    }
}

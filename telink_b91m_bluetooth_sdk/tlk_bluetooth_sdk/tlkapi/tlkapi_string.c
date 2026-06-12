/********************************************************************************************************
 * @file    tlkapi_string.c
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
#include "tlkapi/tlkapi_common.h"
#include "tlkapi/tlkapi_string.h"

/**
 * @brief       Calculate the length of a wide character string
 * @param[in]   pStr - Pointer to the wide character string
 * @param[in]   maxLen - Maximum length to check
 * @returns     Length of the wide character string
*/
int tlkapi_wcharStrlen(uint8_t *pStr, uint16_t maxLen)
{
    uint16_t  index;
    uint16_t  count = maxLen / 2;
    uint16_t *pTemp = (uint16_t *)pStr;
    for (index = 0; index < count; index++) {
        if (pTemp[index] == 0) {
            break;
        }
    }
    return (index * 2);
}

/**
 * @brief       Convert ASCII character to its numeric value
 * @param[in]   c[IN]--ASCII character to convert('0'-'9', 'a'-'f', 'A'-'F')
 * @returns:    Numeric value of the character, -1 if invalid
*/
char tlkapi_ascii_char2val(char c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
        return (c & 0x7) + 9;
    }

    return (char)(-1);
}

/**
 * @brief       Convert ASCII string to numeric value according to specified base
 * @param[in]   str - ASCII string to convert
 * @param[in]   base - Base of the numeric system (e.g., 10 for decimal, 16 for hexadecimal)
 * @returns     Numeric value of the string, 0 if conversion fails
*/
int tlkapi_ascii_str2val(const char str[], char base)
{
    int         val = 0;
    signed char v;
    while (*str != 0) {
        v = tlkapi_ascii_char2val(*str);
        if (v == -1 || v >= base) {
            if (val == 0) // to filter abormal beginning and ending
            {
                str++;
                continue;
            } else {
                break;
            }
        }
        val = val * base + v;
        str++;
    }
    return val;
}

const uint32_t sTlkApiPow10Array[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

/**
 * @brief       Converts a numeric value to a string in decimal format.
 * @param[out]  str - Buffer to store the converted string
 * @param[in]   dec - Decimal number to convert
 * @param[in]   num - Number of digits to process (maximum 10)
 * @param[in]   isDropZero - Whether to discard zero or not.
 * @returns     The length of the converted string.Negative means failure.
 */
int tlkapi_decToStr(char *str, uint32_t dec, uint32_t num, bool isDropZero)
{
    uint8_t  temp;
    uint32_t value;
    char    *pTemp;

    if (str == NULL || num == 0) {
        return 0;
    }
    if (num > 10) {
        num = 10;
    }

    pTemp = str;
    while (num--) {
        value = dec / sTlkApiPow10Array[num];
        temp  = value % 10;
        if (isDropZero && temp == 0) {
            continue;
        }
        if (isDropZero) {
            isDropZero = false;
        }
        *pTemp++ = '0' + temp;
    }
    if (pTemp == str) {
        *pTemp++ = '0';
    }

    return (pTemp - str);
}

/**
 * @brief       Converts a hexadecimal value to its string representation
 * @param[out]  str - Buffer to store the converted hexadecimal string
 * @param[in]   hex - Hexadecimal value to convert
 * @param[in]   num - Number of hex digits to process (maximum 8)
 * @param[in]   isDropZero - Flag indicating whether leading zeros should be dropped
 * @returns     Length of the resulting string
 */
int tlkapi_hexToStr(char *str, uint32_t hex, uint32_t num, bool isDropZero)
{
    char   *pTemp;
    uint8_t temp;
    uint8_t offset;

    if (str == NULL || num == 0) {
        return 0;
    }

    pTemp = str;
    if (num > 8) {
        num = 8;
    }
    while (num--) {
        offset = num << 2;
        temp   = (hex & (0xFUL << offset)) >> offset;
        if (isDropZero && temp == 0) {
            continue;
        }
        if (isDropZero) {
            isDropZero = false;
        }
        if (temp < 10) {
            *pTemp++ = '0' + temp;
        } else {
            *pTemp++ = 'A' + (temp - 10);
        }
    }
    if (pTemp == str) {
        *pTemp++ = '0';
        *pTemp++ = '0';
    }

    return (pTemp - str);
}

/**
 * @brief       Convert a string to a 32-bit integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted integer value
 * @returns     TLK_ENONE if success. Negative means failure
 */
int tlkapi_strToInt32(char *str, int strLen, int32_t *pValue)
{
    int32_t value;
    uint8_t isNeg;

    if (str == NULL || strLen == 0 || pValue == NULL) {
        return -TLK_EPARAM;
    }

    while (strLen != 0 && str[0] == ' ') {
        str++;
        strLen--;
    }
    if (strLen == 0 || str[0] < '0' || str[0] > '9') {
        return -TLK_EFAIL;
    }

    value = 0;
    isNeg = false;
    if (str[0] == '-' || str[0] == '+') {
        if (str[0] == '-') {
            isNeg = true;
        }
        strLen--;
        str++;
    }
    if (strLen == 0 || strLen > 10) {
        return -TLK_EPARAM;
    }

    while (strLen != 0) {
        if (str[0] < '0' || str[0] > '9') {
            break;
        }
        value *= 10;
        value += (str[0] - '0');
        str++;
        strLen--;
    }

    if (isNeg) {
        value *= -1;
    }
    *pValue = value;

    return TLK_ENONE;
}

/**
 * @brief       Convert a string to a 8-bit integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted integer value
 * @returns     TLK_ENONE if success. Negative means failure
 */
int tlkapi_strToInt08(char *str, int strLen, int8_t *pValue)
{
    int     ret;
    int32_t value;

    ret = tlkapi_strToInt32(str, strLen, &value);
    if (ret != TLK_ENONE) {
        return ret;
    }

    if (value < -255 || value > 255) {
        return -TLK_EOVERFLOW;
    }
    *pValue = value;

    return TLK_ENONE;
}

/**
 * @brief       Convert a string to a 16-bit integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted integer value
 * @returns     TLK_ENONE if success. Negative means failure
 */
int tlkapi_strToInt16(char *str, int strLen, int16_t *pValue)
{
    int     ret;
    int32_t value;

    ret = tlkapi_strToInt32(str, strLen, &value);
    if (ret != TLK_ENONE) {
        return ret;
    }

    if (value < -32767 || value > 32767) {
        return -TLK_EOVERFLOW;
    }
    *pValue = value;

    return TLK_ENONE;
}

/**
 * @brief       Convert a string to a 32-bit unsigned integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted unsigned integer value
 * @returns     TLK_ENONE if success. Negative means failure
 */
int tlkapi_strToUint32(char *str, int strLen, uint32_t *pValue)
{
    uint32_t value;

    if (str == NULL || strLen == 0 || strLen > 10 || pValue == NULL) {
        return -TLK_EPARAM;
    }

    while (strLen != 0 && str[0] == ' ') {
        str++;
        strLen--;
    }
    if (strLen == 0 || str[0] < '0' || str[0] > '9') {
        return -TLK_EFORMAT;
    }

    value = 0;
    while (strLen != 0) {
        if (str[0] < '0' || str[0] > '9') {
            break;
        }
        value *= 10;
        value += (str[0] - '0');
        str++;
        strLen--;
    }

    *pValue = value;

    return TLK_ENONE;
}

/**
 * @brief       Convert a string to a 8-bit unsigned integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted unsigned integer value
 * @returns     TLK_ENONE if success. Negative means failure
*/
int tlkapi_strToUint08(char *str, int strLen, uint8_t *pValue)
{
    int      ret;
    uint32_t value;

    ret = tlkapi_strToUint32(str, strLen, &value);
    if (ret != TLK_ENONE) {
        return ret;
    }

    if (value > 0xFF) {
        return -TLK_EOVERFLOW;
    }
    *pValue = value;

    return TLK_ENONE;
}

/**
 * @brief       Convert a string to a 16-bit unsigned integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted unsigned integer value
 * @returns     TLK_ENONE if success. Negative means failure
*/
int tlkapi_strToUint16(char *str, int strLen, uint16_t *pValue)
{
    int      ret;
    uint32_t value;

    ret = tlkapi_strToUint32(str, strLen, &value);
    if (ret != TLK_ENONE) {
        return ret;
    }

    if (value > 0xFFFF) {
        return -TLK_EOVERFLOW;
    }
    *pValue = value;

    return TLK_ENONE;
}

/**
 * @brief       Convert a string to an array of bytes
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pHex - Pointer to store the converted array of bytes
 * @param[in]   hexLen - Length of the array of bytes
 * @returns     TLK_ENONE if success. Negative means failure
*/
int tlkapi_strToArray(char *str, int strLen, uint8_t *pHex, int hexLen)
{
    uint8_t temp;
    uint8_t flag0;
    uint8_t flag1;
    uint8_t length;

    if (str == NULL || strLen == 0 || pHex == NULL || hexLen == 0) {
        return -TLK_EPARAM;
    }

    temp   = 0;
    flag0  = 0;
    flag1  = 0;
    length = 0;
    while (strLen != 0) {
        if (str[0] >= 'A' && str[0] <= 'F') {
            temp |= (str[0] - 'A') + 10;
        } else if (str[0] >= 'a' && str[0] <= 'f') {
            temp |= (str[0] - 'a') + 10;
        } else if (str[0] >= '0' && str[0] <= '9') {
            temp |= (str[0] - '0');
        } else if (str[0] == ' ') {
            flag0 = 1;
        } else {
            return -TLK_EFORMAT;
        }

        if (flag0 == 1) {
            if (flag1 == 1) {
                flag1 = 2;
                temp >>= 4;
            }
            flag0 = 0;
        } else {
            flag1++;
            temp <<= 4;
        }

        if (flag1 == 2) {
            pHex[length++] = temp;
            temp           = 0;
            if (length == hexLen) {
                break;
            }
        }

        str++;
        strLen--;
    }

    return length;
}

/**
 * @brief       Convert an array of bytes to a string
 * @param[in]   pHex - Pointer to the array of bytes to convert
 * @param[in]   hexLen - Length of the array of bytes
 * @param[out]  str - Pointer to store the converted string
 * @param[in]   strLen - Length of the string
 * @param[in]   split - Character to use as a separator between bytes
 * @returns     Length of the converted string
*/
int tlkapi_arrayToStr(uint8_t *pHex, int hexLen, char *str, int strLen, char split)
{
    char   *pStr;
    uint8_t temp;
    uint8_t temp0;
    int     length;

    if (str == NULL || strLen == 0 || pHex == NULL || hexLen == 0) {
        return -TLK_EPARAM;
    }

    temp   = 0;
    length = 0;
    pStr   = str;
    while (hexLen--) {
        if (length + 2 > strLen) {
            break;
        }
        length += 2;
        temp  = *pHex;
        temp0 = (temp & 0xF0) >> 4;
        temp &= 0x0F;
        if (temp0 < 10) {
            *pStr++ = '0' + temp0;
        } else {
            *pStr++ = 'A' + (temp0 - 10);
        }
        if (temp < 10) {
            *pStr++ = '0' + temp;
        } else {
            *pStr++ = 'A' + (temp - 10);
        }
        if (split != 0 && hexLen != 0) {
            if (length + 1 > strLen) {
                break;
            }
            *pStr++ = split;
            length += 1;
        }
        pHex++;
    }

    return (pStr - str);
}

/**
 * @brief       Find a character in a string
 * @param[in]   pSrc - Pointer to the source string
 * @param[in]   srcLen - Length of the source string
 * @param[in]   dst - Character to find
 * @param[out]  pOffset - Pointer to store the offset of the found character
 * @returns     Pointer to the found character, or NULL if not found
*/
char *tlkapi_str_findChar(char *pSrc, uint16_t srcLen, char dst, uint16_t *pOffset)
{
    uint16_t offset;
    if (pSrc == NULL || srcLen == 0) {
        return NULL;
    }
    offset = 0;
    while (offset < srcLen) {
        if (pSrc[offset] == dst) {
            break;
        }
        offset++;
    }
    if (offset == srcLen) {
        return NULL;
    }
    if (pOffset != NULL) {
        *pOffset = offset;
    }
    return &pSrc[offset];
}

/**
 * @brief       Find a string in a string
 * @param[in]   pSrc - Pointer to the source string
 * @param[in]   srcLen - Length of the source string
 * @param[in]   pDst - Pointer to the destination string
 * @param[in]   dstLen - Length of the destination string
 * @param[out]  pOffset - Pointer to store the offset of the found string
 * @returns     Pointer to the found string, or NULL if not
*/
char *tlkapi_str_findStr(char *pSrc, uint16_t srcLen, char *pDst, uint16_t dstLen, uint16_t *pOffset)
{
    if (pSrc == NULL || srcLen < dstLen || pDst == NULL || dstLen == 0) {
        return NULL;
    }
    if (dstLen == 1) {
        return tlkapi_str_findChar(pSrc, srcLen, pDst[0], pOffset);
    } else {
        uint16_t index  = 0;
        uint16_t offset = 0;
        while (index < srcLen) {
            if (index + dstLen > srcLen) {
                break;
            }

            offset = 0;
            while (offset < dstLen && pSrc[index + offset] == pDst[offset]) {
                offset++;
            }
            if (offset == dstLen) {
                break;
            }

            index++;
        }
        if (index + dstLen > srcLen) {
            return NULL;
        }
        if (pOffset != NULL) {
            *pOffset = index;
        }
        return &pSrc[index];
    }
}

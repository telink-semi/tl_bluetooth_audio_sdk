/********************************************************************************************************
 * @file    tlkapi_string.h
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

#define tlkapi_uint08ToStr(str, value, isDropZero) tlkapi_hexToStr(str, value, 2, isDropZero)
#define tlkapi_uint16ToStr(str, value, isDropZero) tlkapi_hexToStr(str, value, 4, isDropZero)
#define tlkapi_uint32ToStr(str, value, isDropZero) tlkapi_hexToStr(str, value, 8, isDropZero)

/**
 * @brief       Calculate the length of a wide character string
 * @param[in]   pStr - Pointer to the wide character string
 * @param[in]   maxLen - Maximum length to check
 * @returns     Length of the wide character string
*/
int tlkapi_wcharStrlen(uint8_t *pStr, uint16_t maxLen);

/**
 * @brief       Convert ASCII character to its numeric value
 * @param[in]   c[IN]--ASCII character to convert('0'-'9', 'a'-'f', 'A'-'F')
 * @returns:    Numeric value of the character, -1 if invalid
*/
char tlkapi_ascii_char2val(const char c);

/**
 * @brief       Convert ASCII string to numeric value according to specified base
 * @param[in]   str - ASCII string to convert
 * @param[in]   base - Base of the numeric system (e.g., 10 for decimal, 16 for hexadecimal)
 * @returns     Numeric value of the string, 0 if conversion fails
*/
int tlkapi_ascii_str2val(const char str[], char base);

/**
 * @brief       Converts a numeric value to a string in decimal format.
 * @param[out]  str - Buffer to store the converted string
 * @param[in]   dec - Decimal number to convert
 * @param[in]   num - Number of digits to process (maximum 10)
 * @param[in]   isDropZero - Whether to discard zero or not.
 * @returns     The length of the converted string.Negative means failure.
 */
int tlkapi_decToStr(char *str, uint32_t dec, uint32_t num, bool isDropZero);

/**
 * @brief       Converts a hexadecimal value to its string representation
 * @param[out]  str - Buffer to store the converted hexadecimal string
 * @param[in]   hex - Hexadecimal value to convert
 * @param[in]   num - Number of hex digits to process (maximum 8)
 * @param[in]   isDropZero - Flag indicating whether leading zeros should be dropped
 * @returns     Length of the resulting string
 */
int tlkapi_hexToStr(char *str, uint32_t hex, uint32_t num, bool isDropZero);

/**
 * @brief       Convert a string to a 32-bit integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted integer value
 * @returns     TLK_ENONE if success. Negative means failure
 */
int tlkapi_strToInt32(char *str, int strLen, int32_t *pValue);

/**
 * @brief       Convert a string to a 8-bit integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted integer value
 * @returns     TLK_ENONE if success. Negative means failure
 */
int tlkapi_strToInt08(char *str, int strLen, int8_t *pValue);

/**
 * @brief       Convert a string to a 16-bit integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted integer value
 * @returns     TLK_ENONE if success. Negative means failure
 */
int tlkapi_strToInt16(char *str, int strLen, int16_t *pValue);

/**
 * @brief       Convert a string to a 32-bit unsigned integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted unsigned integer value
 * @returns     TLK_ENONE if success. Negative means failure
 */
int tlkapi_strToUint32(char *str, int strLen, uint32_t *pValue);

/**
 * @brief       Convert a string to a 8-bit unsigned integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted unsigned integer value
 * @returns     TLK_ENONE if success. Negative means failure
*/
int tlkapi_strToUint08(char *str, int strLen, uint8_t *pValue);

/**
 * @brief       Convert a string to a 16-bit unsigned integer value
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pValue - Pointer to store the converted unsigned integer value
 * @returns     TLK_ENONE if success. Negative means failure
*/
int tlkapi_strToUint16(char *str, int strLen, uint16_t *pValue);

/**
 * @brief       Convert a string to an array of bytes
 * @param[in]   str - The input string to convert
 * @param[in]   strLen - Length of the input string
 * @param[out]  pHex - Pointer to store the converted array of bytes
 * @param[in]   hexLen - Length of the array of bytes
 * @returns     TLK_ENONE if success. Negative means failure
*/
int tlkapi_strToArray(char *str, int strLen, uint8_t *pHex, int hexLen);

/**
 * @brief       Convert an array of bytes to a string
 * @param[in]   pHex - Pointer to the array of bytes to convert
 * @param[in]   hexLen - Length of the array of bytes
 * @param[out]  str - Pointer to store the converted string
 * @param[in]   strLen - Length of the string
 * @param[in]   split - Character to use as a separator between bytes
 * @returns     Length of the converted string
*/
int tlkapi_arrayToStr(uint8_t *pHex, int hexLen, char *str, int strLen, char split);

/**
 * @brief       Find a character in a string
 * @param[in]   pSrc - Pointer to the source string
 * @param[in]   srcLen - Length of the source string
 * @param[in]   dst - Character to find
 * @param[out]  pOffset - Pointer to store the offset of the found character
 * @returns     Pointer to the found character, or NULL if not found
*/
char *tlkapi_str_findChar(char *pSrc, uint16_t srcLen, char dst, uint16_t *pOffset);

/**
 * @brief       Find a string in a string
 * @param[in]   pSrc - Pointer to the source string
 * @param[in]   srcLen - Length of the source string
 * @param[in]   pDst - Pointer to the destination string
 * @param[in]   dstLen - Length of the destination string
 * @param[out]  pOffset - Pointer to store the offset of the found string
 * @returns     Pointer to the found string, or NULL if not
*/
char *tlkapi_str_findStr(char *pSrc, uint16_t srcLen, char *pDst, uint16_t dstLen, uint16_t *pOffset);

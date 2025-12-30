/********************************************************************************************************
 * @file    co_assert.h
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


#ifndef _CO_ASSERT_H_
#define _CO_ASSERT_H_

#define PLF_DEBUG 0

#ifndef assert
    #define assert(ignore) ((void)0)
#endif

#if (PLF_DEBUG)
/**
 ****************************************************************************************
 * @brief Print the assertion error reason and loop forever.
 *
 * @param condition C string containing the condition.
 * @param file C string containing file where the assertion is located.
 * @param line Line number in the file where the assertion is located.
 ****************************************************************************************
 */
void assert_err(const char *condition, const char *file, int line);

/**
 ****************************************************************************************
 * @brief Print the assertion error reason and loop forever.
 * The parameter value that is causing the assertion will also be disclosed.
 *
 * @param param0 parameter value 0.
 * @param param1 parameter value 1.
 * @param file C string containing file where the assertion is located.
 * @param line Line number in the file where the assertion is located.
 ****************************************************************************************
 */
void assert_param(int param0, int param1, const char *file, int line);

/**
 ****************************************************************************************
 * @brief Print the assertion warning reason.
 *
 * @param param0 parameter value 0.
 * @param param1 parameter value 1.
 * @param file C string containing file where the assertion is located.
 * @param line Line number in the file where the assertion is located.
 ****************************************************************************************
 */
void assert_warn(int param0, int param1, const char *file, int line);
#endif //PLF_DEBUG


/*
 * ASSERTION CHECK
 ****************************************************************************************
 */
#if (PLF_DEBUG)
    /// Assertions showing a critical error that could require a full system reset
    #define ASSERT_ERR(cond)                             \
        do {                                             \
            if (!(cond)) {                               \
                assert_err(#cond, __MODULE__, __LINE__); \
            }                                            \
        } while (0)

    /// Assertions showing a critical error that could require a full system reset
    #define ASSERT_INFO(cond, param0, param1)                                 \
        do {                                                                  \
            if (!(cond)) {                                                    \
                assert_param((int)param0, (int)param1, __MODULE__, __LINE__); \
            }                                                                 \
        } while (0)

    /// Assertions showing a non-critical problem that has to be fixed by the SW
    #define ASSERT_WARN(cond, param0, param1)                                \
        do {                                                                 \
            if (!(cond)) {                                                   \
                assert_warn((int)param0, (int)param1, __MODULE__, __LINE__); \
            }                                                                \
        } while (0)

#else
    /// Assertions showing a critical error that could require a full system reset

    #ifndef ASSERT_ERR
        #define ASSERT_ERR(cond) while (!(cond))
    #endif

    /// Assertions showing a critical error that could require a full system reset
    #define ASSERT_INFO(cond, param0, param1)

    /// Assertions showing a non-critical problem that has to be fixed by the SW
    #define ASSERT_WARN(cond, param0, param1)

#endif //PLF_DEBUG


/// @} CO_BT
#endif // _CO_ASSERT_H_

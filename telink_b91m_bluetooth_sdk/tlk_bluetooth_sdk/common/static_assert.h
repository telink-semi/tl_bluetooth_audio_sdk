/********************************************************************************************************
 * @file    static_assert.h
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
#ifndef COMMON_STATIC_ASSERT_H_
#define COMMON_STATIC_ASSERT_H_

#define STATIC_ASSERT_M(COND, MSG) __attribute__((unused)) typedef char static_assertion_##MSG[(!!(COND)) * 2 - 1]
// token pasting madness:
#define STATIC_ASSERT3(X, L)                    STATIC_ASSERT_M(X, static_assertion_at_line_##L)
#define STATIC_ASSERT2(X, L)                    STATIC_ASSERT3(X, L)

#define STATIC_ASSERT(X)                        STATIC_ASSERT2(X, __LINE__)

#define STATIC_ASSERT_POW2(expr)                STATIC_ASSERT(!((expr) & ((expr) - 1))) //  assert  expr  is  2**N
#define STATIC_ASSERT_EVEN(expr)                STATIC_ASSERT(!((expr) & 1))
#define STATIC_ASSERT_ODD(expr)                 STATIC_ASSERT(((expr) & 1))
#define STATIC_ASSERT_INT_DIV(a, b)             STATIC_ASSERT((a) / (b) * (b) == (a))


#define STATIC_ASSERT_MSG(cond, msg)            __attribute__((unused)) typedef char STATIC_ASSERT_MSG_(msg, __LINE__, __COUNTER__)[1 - 2 * !(cond)]

#define STATIC_ASSERT_MSG_(MSG, LINE, COUNTER)  STATIC_ASSERT_MSG__(MSG, LINE, COUNTER)
#define STATIC_ASSERT_MSG__(MSG, LINE, COUNTER) static_assert##COUNTER##__FileOf_##MSG##__LineOf_##LINE


#define STATIC_ASSERT_FILE(exp, file_name)      STATIC_ASSERT_MSG(exp, file_name)


#define STRINGIFY(x)                            STRINGIFY_HELPER(x)
#define STRINGIFY_HELPER(x)                     #x

#define STATIC_ASSERT_THIS_FILE(cond)           _Static_assert(cond, "Assertion failed in " __FILE__ ", line " STRINGIFY(__LINE__))

#endif /* COMMON_STATIC_ASSERT_H_ */

/********************************************************************************************************
 * @file    bit.h
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
#ifndef BIT_H_
#define BIT_H_


#ifndef BIT
#define BIT(n) (1 << (n))
#endif
#define BIT_MASK_LEN(len)      (BIT(len) - 1)
#define BIT_RNG(s, e)          (BIT_MASK_LEN((e) - (s) + 1) << (s))

#define BM_CLR_MASK_V(x, mask) ((x) & ~(mask))

#ifndef BM_SET
#define BM_SET(x, mask) ((x) |= (mask))
#endif
#ifndef BM_CLR
#define BM_CLR(x, mask) ((x) &= ~(mask))
#endif
#ifndef BM_IS_SET
#define BM_IS_SET(x, mask) ((x) & (mask))
#endif
#ifndef BM_IS_CLR
#define BM_IS_CLR(x, mask) ((~x) & (mask))
#endif
#ifndef BM_FLIP
#define BM_FLIP(x, mask) ((x) ^= (mask))
#endif

#define ONES(x) BIT_MASK_LEN(x)
#define ONES_32 0xffffffff
#define ALL_SET 0xffffffff


// BITSx  are internal used macro, please use BITS instead
#define BITS1(a)                      BIT(a)
#define BITS2(a, b)                   (BIT(a) | BIT(b))
#define BITS3(a, b, c)                (BIT(a) | BIT(b) | BIT(c))
#define BITS4(a, b, c, d)             (BIT(a) | BIT(b) | BIT(c) | BIT(d))
#define BITS5(a, b, c, d, e)          (BIT(a) | BIT(b) | BIT(c) | BIT(d) | BIT(e))
#define BITS6(a, b, c, d, e, f)       (BIT(a) | BIT(b) | BIT(c) | BIT(d) | BIT(e) | BIT(f))
#define BITS7(a, b, c, d, e, f, g)    (BIT(a) | BIT(b) | BIT(c) | BIT(d) | BIT(e) | BIT(f) | BIT(g))
#define BITS8(a, b, c, d, e, f, g, h) (BIT(a) | BIT(b) | BIT(c) | BIT(d) | BIT(e) | BIT(f) | BIT(g) | BIT(h))

#define BITS(...)                     VARARG(BITS, __VA_ARGS__)

// !!!!   v is already a masked value,  no need to shift
#define BM_MASK_VAL(x, mask, v)     (((x) & ~(mask)) | (v))
#define BM_SET_MASK_VAL(x, mask, v) ((x) = BM_MASK_VAL(x, mask, v))

#define BIT_SET(x, n)               ((x) |= BIT(n))
#define BIT_CLR(x, n)               ((x) &= ~BIT(n))
#define BIT_IS_SET(x, n)            ((x) & BIT(n))
#define BIT_FLIP(x, n)              ((x) ^= BIT(n))
#define BIT_SET_HIGH(x)             ((x) |= BIT((sizeof((x)) * 8 - 1)))  // set the highest bit
#define BIT_CLR_HIGH(x)             ((x) &= ~BIT((sizeof((x)) * 8 - 1))) // clr the highest bit
#define BIT_IS_SET_HIGH(x)          ((x) & BIT((sizeof((x)) * 8 - 1)))   // check the highest bit


#define BIT_MASK(start, len)        (BIT_MASK_LEN(len) << (start))


#define BIT_IS_EVEN(x)              (((x) & 1) == 0)
#define BIT_IS_ODD(x)               (!BIT_IS_EVEN((x)))
#define BIT_IS_POW2(x)              (!((x) & ((x) - 1)))
#define BIT_TURNOFF_1(x)            ((x) &= ((x) - 1))
#define BIT_ISOLATE_1(x)            ((x) &= (-(x)))
#define BIT_PROPAGATE_1(x)          ((x) |= ((x) - 1))
#define BIT_ISOLATE_0(x)            ((x) = ~(x) & ((x) + 1))
#define BIT_TURNON_0(x)             ((x) |= ((x) + 1))
#define CLAMP_TO_ONE(x)             (!!(x)) // compiler defined, not standard.    0 --> 0,   1 --> 0xffffffff

/* Return the bit index of the lowest 1 in y.   ex:  0b00110111000  --> 3 */
#define BIT_LOW_BIT(y)                                                                                                                                                            \
    (((y) & BIT(0)) ?                                                                                                                                                             \
         0 :                                                                                                                                                                      \
         (((y) & BIT(1)) ?                                                                                                                                                        \
              1 :                                                                                                                                                                 \
              (((y) & BIT(2)) ?                                                                                                                                                   \
                   2 :                                                                                                                                                            \
                   (((y) & BIT(3)) ?                                                                                                                                              \
                        3 :                                                                                                                                                       \
                        (((y) & BIT(4)) ?                                                                                                                                         \
                             4 :                                                                                                                                                  \
                             (((y) & BIT(5)) ?                                                                                                                                    \
                                  5 :                                                                                                                                             \
                                  (((y) & BIT(6)) ?                                                                                                                               \
                                       6 :                                                                                                                                        \
                                       (((y) & BIT(7)) ?                                                                                                                          \
                                            7 :                                                                                                                                   \
                                            (((y) & BIT(8)) ?                                                                                                                     \
                                                 8 :                                                                                                                              \
                                                 (((y) & BIT(9)) ?                                                                                                                \
                                                      9 :                                                                                                                         \
                                                      (((y) & BIT(10)) ?                                                                                                          \
                                                           10 :                                                                                                                   \
                                                           (((y) & BIT(11)) ?                                                                                                     \
                                                                11 :                                                                                                              \
                                                                (((y) & BIT(12)) ?                                                                                                \
                                                                     12 :                                                                                                         \
                                                                     (((y) & BIT(13)) ?                                                                                           \
                                                                          13 :                                                                                                    \
                                                                          (((y) & BIT(14)) ?                                                                                      \
                                                                               14 :                                                                                               \
                                                                               (((y) & BIT(15)) ?                                                                                 \
                                                                                    15 :                                                                                          \
                                                                                    (((y) & BIT(16)) ?                                                                            \
                                                                                         16 :                                                                                     \
                                                                                         (((y) & BIT(17)) ?                                                                       \
                                                                                              17 :                                                                                \
                                                                                              (((y) & BIT(18)) ?                                                                  \
                                                                                                   18 :                                                                           \
                                                                                                   (((y) & BIT(19)) ?                                                             \
                                                                                                        19 :                                                                      \
                                                                                                        (((y) & BIT(20)) ?                                                        \
                                                                                                             20 :                                                                 \
                                                                                                             (((y) & BIT(21)) ?                                                   \
                                                                                                                  21 :                                                            \
                                                                                                                  (((y) & BIT(22)) ?                                              \
                                                                                                                       22 :                                                       \
                                                                                                                       (((y) & BIT(23)) ?                                         \
                                                                                                                            23 :                                                  \
                                                                                                                            (((y) & BIT(24)) ?                                    \
                                                                                                                                 24 :                                             \
                                                                                                                                 (((y) & BIT(25)) ?                               \
                                                                                                                                      25 :                                        \
                                                                                                                                      (((y) & BIT(26)) ?                          \
                                                                                                                                           26 :                                   \
                                                                                                                                           (((y) & BIT(27)) ?                     \
                                                                                                                                                27 :                              \
                                                                                                                                                (((y) & BIT(28)) ?                \
                                                                                                                                                     28 :                         \
                                                                                                                                                     (((y) & BIT(29)) ?           \
                                                                                                                                                          29 :                    \
                                                                                                                                                          (((y) & BIT(30)) ?      \
                                                                                                                                                               30 :               \
                                                                                                                                                               (((y) & BIT(31)) ? \
                                                                                                                                                                    31 :          \
                                                                                                                                                                    32))))))))))))))))))))))))))))))))

/* Return the bit index of the highest 1 in (y).   ex:  0b00110111000  --> 8 */
#define BIT_HIGH_BIT(y)                                                                                                                                                          \
    (((y) & BIT(31)) ?                                                                                                                                                           \
         31 :                                                                                                                                                                    \
         (((y) & BIT(30)) ?                                                                                                                                                      \
              30 :                                                                                                                                                               \
              (((y) & BIT(29)) ?                                                                                                                                                 \
                   29 :                                                                                                                                                          \
                   (((y) & BIT(28)) ?                                                                                                                                            \
                        28 :                                                                                                                                                     \
                        (((y) & BIT(27)) ?                                                                                                                                       \
                             27 :                                                                                                                                                \
                             (((y) & BIT(26)) ?                                                                                                                                  \
                                  26 :                                                                                                                                           \
                                  (((y) & BIT(25)) ?                                                                                                                             \
                                       25 :                                                                                                                                      \
                                       (((y) & BIT(24)) ?                                                                                                                        \
                                            24 :                                                                                                                                 \
                                            (((y) & BIT(23)) ?                                                                                                                   \
                                                 23 :                                                                                                                            \
                                                 (((y) & BIT(22)) ?                                                                                                              \
                                                      22 :                                                                                                                       \
                                                      (((y) & BIT(21)) ?                                                                                                         \
                                                           21 :                                                                                                                  \
                                                           (((y) & BIT(20)) ?                                                                                                    \
                                                                20 :                                                                                                             \
                                                                (((y) & BIT(19)) ?                                                                                               \
                                                                     19 :                                                                                                        \
                                                                     (((y) & BIT(18)) ?                                                                                          \
                                                                          18 :                                                                                                   \
                                                                          (((y) & BIT(17)) ?                                                                                     \
                                                                               17 :                                                                                              \
                                                                               (((y) & BIT(16)) ?                                                                                \
                                                                                    16 :                                                                                         \
                                                                                    (((y) & BIT(15)) ?                                                                           \
                                                                                         15 :                                                                                    \
                                                                                         (((y) & BIT(14)) ?                                                                      \
                                                                                              14 :                                                                               \
                                                                                              (((y) & BIT(13)) ?                                                                 \
                                                                                                   13 :                                                                          \
                                                                                                   (((y) & BIT(12)) ?                                                            \
                                                                                                        12 :                                                                     \
                                                                                                        (((y) & BIT(11)) ?                                                       \
                                                                                                             11 :                                                                \
                                                                                                             (((y) & BIT(10)) ?                                                  \
                                                                                                                  10 :                                                           \
                                                                                                                  (((y) & BIT(9)) ?                                              \
                                                                                                                       9 :                                                       \
                                                                                                                       (((y) & BIT(8)) ?                                         \
                                                                                                                            8 :                                                  \
                                                                                                                            (((y) & BIT(7)) ?                                    \
                                                                                                                                 7 :                                             \
                                                                                                                                 (((y) & BIT(6)) ?                               \
                                                                                                                                      6 :                                        \
                                                                                                                                      (((y) & BIT(5)) ?                          \
                                                                                                                                           5 :                                   \
                                                                                                                                           (((y) & BIT(4)) ?                     \
                                                                                                                                                4 :                              \
                                                                                                                                                (((y) & BIT(3)) ?                \
                                                                                                                                                     3 :                         \
                                                                                                                                                     (((y) & BIT(2)) ?           \
                                                                                                                                                          2 :                    \
                                                                                                                                                          (((y) & BIT(1)) ?      \
                                                                                                                                                               1 :               \
                                                                                                                                                               (((y) & BIT(0)) ? \
                                                                                                                                                                    0 :          \
                                                                                                                                                                    32))))))))))))))))))))))))))))))))

#define COUNT_ARGS_IMPL2(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N, ...) N
#define COUNT_ARGS_IMPL(args)                                                                                               COUNT_ARGS_IMPL2 args
#define COUNT_ARGS(...)                                                                                                     COUNT_ARGS_IMPL((__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define MACRO_CHOOSE_HELPER2(base, count)                                                                                   base##count
#define MACRO_CHOOSE_HELPER1(base, count)                                                                                   MACRO_CHOOSE_HELPER2(base, count)
#define MACRO_CHOOSE_HELPER(base, count)                                                                                    MACRO_CHOOSE_HELPER1(base, count)

#define MACRO_GLUE(x, y)                                                                                                    x y
#define VARARG(base, ...)                                                                                                   MACRO_GLUE(MACRO_CHOOSE_HELPER(base, COUNT_ARGS(__VA_ARGS__)), (__VA_ARGS__))

#define MV(m, v)                                                                                                            (((v) << BIT_LOW_BIT(m)) & (m))

/* warning MASK_VALn  are internal used macro, please use MASK_VAL instead */
#define MASK_VAL2(m, v)                                                    (MV(m, v))
#define MASK_VAL4(m1, v1, m2, v2)                                          (MV(m1, v1) | MV(m2, v2))
#define MASK_VAL6(m1, v1, m2, v2, m3, v3)                                  (MV(m1, v1) | MV(m2, v2) | MV(m3, v3))
#define MASK_VAL8(m1, v1, m2, v2, m3, v3, m4, v4)                          (MV(m1, v1) | MV(m2, v2) | MV(m3, v3) | MV(m4, v4))
#define MASK_VAL10(m1, v1, m2, v2, m3, v3, m4, v4, m5, v5)                 (MV(m1, v1) | MV(m2, v2) | MV(m3, v3) | MV(m4, v4) | MV(m5, v5))
#define MASK_VAL12(m1, v1, m2, v2, m3, v3, m4, v4, m5, v5, m6, v6)         (MV(m1, v1) | MV(m2, v2) | MV(m3, v3) | MV(m4, v4) | MV(m5, v5) | MV(m6, v6))
#define MASK_VAL14(m1, v1, m2, v2, m3, v3, m4, v4, m5, v5, m6, v6, m7, v7) (MV(m1, v1) | MV(m2, v2) | MV(m3, v3) | MV(m4, v4) | MV(m5, v5) | MV(m6, v6) | MV(m7, v7))
#define MASK_VAL16(m1, v1, m2, v2, m3, v3, m4, v4, m5, v5, m6, v6, m7, v7, m8, v8) \
    (MV(m1, v1) | MV(m2, v2) | MV(m3, v3) | MV(m4, v4) | MV(m5, v5) | MV(m6, v6) | MV(m7, v7) | MV(m8, v8))

#define MASK_VAL(...) VARARG(MASK_VAL, __VA_ARGS__)


#endif

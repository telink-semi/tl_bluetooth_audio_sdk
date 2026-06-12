/********************************************************************************************************
 * @file    tlklib_vsprintf.c
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
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define TLKLIB_VSPRINTF_OUTC(c) \
    do {                        \
        if (p < e)              \
            *p++ = (c);         \
    } while (0)

#define TLKLIB_VSPRINTF_PAD(n, c)                          \
    do {                                                   \
        int _n = (n);                                      \
        while (_n-- > 0 && p < e) TLKLIB_VSPRINTF_OUTC(c); \
    } while (0)

static inline void tlklib_vsprintf_outs(const char *s, char **pp, char *e)
{
    char *p = *pp;
    while (p < e && *s != '\0') {
        *p++ = *s++;
    }
    *pp = p;
}

/* ---------- Unsigned integer to string conversion ---------- */
static inline int tlklib_snprintf_utoa(uint32_t v, unsigned base, int upper, int width, int prec, int alt, int left, int zero, char **pp, char *end)
{
    char *p = *pp;
    char *e = end;

    char        tmp[32]; // Buffer for reversed digits
    int         i = 0, digits = 0;
    const char *dig = upper ? "0123456789ABCDEF" : "0123456789abcdef";
    uint32_t    t   = v;

    // Handle zero case
    if (t == 0) {
        tmp[i++] = '0';
        digits   = 1;
    } else {
        do {
            if (i < (int)sizeof(tmp) - 1) {
                tmp[i++] = dig[t % base];
            }
            t /= base;
        } while (t && i < (int)sizeof(tmp) - 1);
        digits = i;
    }

    // Apply precision requirement - minimum number of digits
    if (prec > digits) {
        digits = prec;
    }

    // Calculate prefix length (for 0x, 0, etc.)
    int prefix = 0;
    if (alt && base == 16 && v != 0) {
        prefix = 2; // 0x
    }
    if (alt && base == 8 && v != 0) {
        prefix = 1; // 0
    }

    int body    = digits + prefix; // Total characters needed for content
    int pad_cnt = width - body;    // Padding characters needed
    if (pad_cnt < 0) {
        pad_cnt = 0;
    }

    // Determine padding character
    char pad_char = ' ';
    if (zero && !left && prec < 0) {
        pad_char = '0';
    }

    // Left alignment: content first, then padding
    if (left) {
        // Output prefix (0x, 0, etc.)
        if (prefix) {
            if (base == 16) {
                TLKLIB_VSPRINTF_OUTC('0');
                TLKLIB_VSPRINTF_OUTC(upper ? 'X' : 'x');
            } else if (base == 8) {
                TLKLIB_VSPRINTF_OUTC('0');
            }
        }
        // Output leading zeros for precision
        for (int j = digits - i; j > 0 && p < e; j--) {
            TLKLIB_VSPRINTF_OUTC('0');
        }
        // Output digits in reverse order (they were stored reversed)
        while (i-- > 0 && p < e) {
            TLKLIB_VSPRINTF_OUTC(tmp[i]);
        }
        // Right padding for left alignment
        TLKLIB_VSPRINTF_PAD(pad_cnt, ' ');
    }
    // Right alignment: padding first, then content
    else {
        // Special handling for zero padding with prefix
        if (pad_char == '0' && prefix) {
            if (base == 16) {
                TLKLIB_VSPRINTF_OUTC('0');
                TLKLIB_VSPRINTF_OUTC(upper ? 'X' : 'x');
                prefix = 0; // Prefix already output
            } else if (base == 8) {
                TLKLIB_VSPRINTF_OUTC('0');
                prefix = 0; // Prefix already output
            }
        }

        // Left padding
        TLKLIB_VSPRINTF_PAD(pad_cnt, pad_char);

        // Output prefix (if not already done)
        if (prefix) {
            if (base == 16) {
                TLKLIB_VSPRINTF_OUTC('0');
                TLKLIB_VSPRINTF_OUTC(upper ? 'X' : 'x');
            } else if (base == 8) {
                TLKLIB_VSPRINTF_OUTC('0');
            }
        }

        // Output leading zeros for precision
        for (int j = digits - i; j > 0 && p < e; j--) {
            TLKLIB_VSPRINTF_OUTC('0');
        }

        // Output digits in reverse order
        while (i-- > 0 && p < e) {
            TLKLIB_VSPRINTF_OUTC(tmp[i]);
        }
    }

    *pp = p;
    return body > width ? body : width;
}

/* ---------- Simple NaN check without 64-bit operations ---------- */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"

static inline int tlklib_is_nan(float v)
{
    // NaN is the only value that doesn't equal itself
    return (v != v);
}

#pragma GCC diagnostic pop

/* ---------- Simple infinity check ---------- */
static inline int tlklib_is_inf(float v)
{
    // Check for very large numbers (using float limits)
    return (v > 1e10f || v < -1e10f);
}

/* ---------- Floating point to string conversion with fixed rounding ---------- */
static inline void tlklib_snprintf_ftoa(float v, int width, int prec, int left, int zero, char **pp, char *end)
{
    char *p = *pp;
    char *e = end;

    // Set precision limits
    if (prec < 0) {
        prec = 6;
    }
    if (prec > 9) {
        prec = 9; // Reasonable limit for 32-bit floats
    }

    // Check for special values
    if (tlklib_is_nan(v)) {
        // Handle NaN with width
        int len = 3;
        int pad = width - len;
        if (pad < 0) {
            pad = 0;
        }

        if (!left) {
            TLKLIB_VSPRINTF_PAD(pad, ' ');
        }
        tlklib_vsprintf_outs("nan", &p, e);
        if (left) {
            TLKLIB_VSPRINTF_PAD(pad, ' ');
        }

        *pp = p;
        return;
    }

    // Check for numbers too large
    if (tlklib_is_inf(v)) {
        int len = (v < 0) ? 4 : 3; // "-inf" or "inf"
        int pad = width - len;
        if (pad < 0) {
            pad = 0;
        }

        if (!left) {
            TLKLIB_VSPRINTF_PAD(pad, ' ');
        }
        if (v < 0) {
            TLKLIB_VSPRINTF_OUTC('-');
        }
        tlklib_vsprintf_outs("inf", &p, e);
        if (left) {
            TLKLIB_VSPRINTF_PAD(pad, ' ');
        }

        *pp = p;
        return;
    }

    // Handle negative numbers and negative zero
    int is_negative = 0;
    if (v < 0) {
        is_negative = 1;
        v           = -v;
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    // Handle negative zero specifically
    if (v == 0.0f && is_negative) {
        is_negative = 0; // Treat negative zero as regular zero
    }
#pragma GCC diagnostic pop

    // Calculate integer part
    uint32_t integer_part = (uint32_t)v;
    float    fractional   = v - (float)integer_part;

    // Apply rounding to fractional part
    if (prec > 0) {
        // Calculate rounding factor for the desired precision
        float round_factor = 0.5f;
        for (int i = 0; i < prec; i++) {
            round_factor *= 0.1f;
        }

        // Apply rounding to fractional part
        fractional += round_factor;

        // Check if rounding caused fractional part to overflow to 1.0
        if (fractional >= 1.0f) {
            integer_part++;
            fractional -= 1.0f;
        }

        // Ensure fractional part is in valid range [0, 1)
        if (fractional < 0) {
            fractional = 0;
        }
        if (fractional >= 1.0f) {
            fractional = 0.9999999f;
        }
    }

    // Calculate the total length for width padding
    int      integer_len = 0;
    uint32_t temp        = integer_part;
    do {
        integer_len++;
        temp /= 10;
    } while (temp > 0);
    if (integer_part == 0) {
        integer_len = 1;
    }

    int total_len = integer_len + (prec > 0 ? prec + 1 : 0) + (is_negative ? 1 : 0);
    int pad_cnt   = width - total_len;
    if (pad_cnt < 0) {
        pad_cnt = 0;
    }

    char pad_char = zero && !left ? '0' : ' ';

    // Handle padding for right alignment with zero fill
    if (!left && pad_char == '0') {
        if (is_negative) {
            TLKLIB_VSPRINTF_OUTC('-');
            is_negative = 0; // Already printed the sign
        }
        TLKLIB_VSPRINTF_PAD(pad_cnt, '0');
    }
    // Handle padding for right alignment with space fill
    else if (!left) {
        TLKLIB_VSPRINTF_PAD(pad_cnt, ' ');
    }

    // Output sign if not already done
    if (is_negative) {
        TLKLIB_VSPRINTF_OUTC('-');
    }

    // Handle padding for left alignment with zero fill (unusual case)
    if (left && pad_char == '0') {
        TLKLIB_VSPRINTF_PAD(pad_cnt, '0');
    }

    // Output integer part
    tlklib_snprintf_utoa(integer_part, 10, 0, 0, 0, 0, 0, 0, &p, e);

    // Output fractional part if precision > 0
    if (prec > 0 && p < e) {
        TLKLIB_VSPRINTF_OUTC('.');

        // Convert fractional part to integer with exact precision
        uint32_t fractional_int = 0;
        float    frac_temp      = fractional;

        // Multiply by 10^prec to get the fractional digits as integer
        for (int i = 0; i < prec; i++) {
            frac_temp *= 10.0f;
            fractional_int = fractional_int * 10 + (uint32_t)frac_temp;
            frac_temp -= (uint32_t)frac_temp;
        }

        // Output fractional digits with leading zeros
        char     frac_buf[10];
        int      frac_idx  = 0;
        uint32_t temp_frac = fractional_int;

        // Handle case where fractional part is zero
        if (temp_frac == 0) {
            for (int i = 0; i < prec && p < e; i++) {
                TLKLIB_VSPRINTF_OUTC('0');
            }
        } else {
            // Store digits in reverse order
            do {
                if (frac_idx < (int)sizeof(frac_buf) - 1) {
                    frac_buf[frac_idx++] = '0' + (temp_frac % 10);
                    temp_frac /= 10;
                }
            } while (temp_frac > 0 && frac_idx < (int)sizeof(frac_buf) - 1);

            // Add leading zeros if needed
            for (int i = frac_idx; i < prec && p < e; i++) {
                TLKLIB_VSPRINTF_OUTC('0');
            }

            // Output digits in correct order
            while (frac_idx-- > 0 && p < e) {
                TLKLIB_VSPRINTF_OUTC(frac_buf[frac_idx]);
            }
        }
    }

    // Handle padding for left alignment
    if (left) {
        TLKLIB_VSPRINTF_PAD(pad_cnt, ' ');
    }

    *pp = p;
}

/* ---------- Variable arguments snprintf implementation ---------- */
int tlklib_vsnprintf(char *buf, size_t size, const char *fmt, va_list ap)
{
    // Validate input parameters
    if (!buf || !fmt || size == 0) {
        return -1;
    }

    char *p = buf;
    char *e = buf + size - 1; // Reserve space for null terminator

    // Process format string
    while (*fmt && p < e) {
        if (*fmt != '%') {
            TLKLIB_VSPRINTF_OUTC(*fmt++);
            continue;
        }

        const char *percent_fmt = fmt; // Remember start of format specifier
        ++fmt;

        // Handle escaped percent sign
        if (*fmt == '%') {
            TLKLIB_VSPRINTF_OUTC('%');
            ++fmt;
            continue;
        }

        // Parse format flags
        int left = 0, zero = 0, alt = 0, width = 0, prec = -1;

        /* Parse flags: -, +, space, #, 0 */
        while (*fmt == '-' || *fmt == '+' || *fmt == ' ' || *fmt == '#' || *fmt == '0') {
            if (*fmt == '-') {
                left = 1;
            }
            if (*fmt == '#') {
                alt = 1;
            }
            if (*fmt == '0') {
                zero = 1;
            }
            ++fmt;
        }

        /* Parse width field */
        if (*fmt >= '0' && *fmt <= '9') {
            width = 0;
            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + (*fmt++ - '0');
            }
        }

        /* Parse precision field */
        if (*fmt == '.') {
            ++fmt;
            prec = 0;
            while (*fmt >= '0' && *fmt <= '9') {
                prec = prec * 10 + (*fmt++ - '0');
            }
        }

        // Check for premature end of format string
        if (!*fmt) {
            fmt = percent_fmt;
            TLKLIB_VSPRINTF_OUTC(*fmt++);
            continue;
        }

        // Process format specifier
        char spec = *fmt++;
        switch (spec) {
        case 'd':
        { // Signed decimal integer
            int      v    = va_arg(ap, int);
            uint32_t uv   = (v < 0) ? (uint32_t)(-v) : (uint32_t)v;
            int      sign = (v < 0);

            // Calculate actual width accounting for sign
            int actual_width = width;
            if (sign) {
                actual_width--;
            }

            if (!left) {
                if (zero) {
                    if (sign) {
                        TLKLIB_VSPRINTF_OUTC('-');
                    }
                    TLKLIB_VSPRINTF_PAD(actual_width, '0');
                } else {
                    TLKLIB_VSPRINTF_PAD(actual_width, ' ');
                    if (sign) {
                        TLKLIB_VSPRINTF_OUTC('-');
                    }
                }
            } else {
                if (sign) {
                    TLKLIB_VSPRINTF_OUTC('-');
                }
            }

            tlklib_snprintf_utoa(uv, 10, 0, 0, prec, 0, 0, 0, &p, e);

            if (left) {
                TLKLIB_VSPRINTF_PAD(actual_width, ' ');
            }
            break;
        }
        case 'u':
        { // Unsigned decimal integer
            unsigned int v = va_arg(ap, unsigned int);
            tlklib_snprintf_utoa(v, 10, 0, width, prec, 0, left, zero, &p, e);
            break;
        }
        case 'x':
        case 'X':
        { // Hexadecimal integer
            unsigned int v = va_arg(ap, unsigned int);
            tlklib_snprintf_utoa(v, 16, (spec == 'X'), width, prec, alt, left, zero, &p, e);
            break;
        }
        case 'o':
        { // Octal integer
            unsigned int v = va_arg(ap, unsigned int);
            tlklib_snprintf_utoa(v, 8, 0, width, prec, alt, left, zero, &p, e);
            break;
        }
        case 'p':
        {
            uintptr_t ptr_val = (uintptr_t)va_arg(ap, void *);
            tlklib_snprintf_utoa((uint32_t)ptr_val, 16, 0, width, prec, 1, left, zero, &p, e);
            break;
        }
        case 'f':
        case 'F':
        { // Floating point
            double v = va_arg(ap, double);
            // Convert double to float to avoid 64-bit operations
            tlklib_snprintf_ftoa((float)v, width, prec, left, zero, &p, e);
            break;
        }
        case 'c':
        {
            int ch  = va_arg(ap, int);
            int pad = width - 1;
            if (pad < 0) {
                pad = 0;
            }

            if (!left) {
                TLKLIB_VSPRINTF_PAD(pad, ' ');
            }
            TLKLIB_VSPRINTF_OUTC(ch);
            if (left) {
                TLKLIB_VSPRINTF_PAD(pad, ' ');
            }
            break;
        }
        case 's':
        {
            const char *str = va_arg(ap, char *);
            if (!str) {
                str = "(null)";
            }

            // Calculate string length for width handling
            int         len = 0;
            const char *s   = str;
            while (*s && len < 256) { // Limit to prevent infinite loop
                len++;
                s++;
            }

            int pad = width - len;
            if (pad < 0) {
                pad = 0;
            }

            if (!left) {
                TLKLIB_VSPRINTF_PAD(pad, ' ');
            }
            tlklib_vsprintf_outs(str, &p, e);
            if (left) {
                TLKLIB_VSPRINTF_PAD(pad, ' ');
            }
            break;
        }
        default:
            fmt = percent_fmt;
            TLKLIB_VSPRINTF_OUTC(*fmt++);
            break;
        }
    }

    // Null terminate
    if (p < buf + size) {
        *p = '\0';
    } else if (size > 0) {
        buf[size - 1] = '\0';
    }

    return (int)(p - buf);
}

int tlklib_snprintf(char *buf, size_t size, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = tlklib_vsnprintf(buf, size, fmt, ap);
    va_end(ap);
    return n;
}

int tlklib_vsprintf(char *buf, const char *fmt, va_list ap)
{
    return tlklib_vsnprintf(buf, 0x7FFFFFFF, fmt, ap);
}

int tlklib_sprintf(char *buf, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = tlklib_vsnprintf(buf, 0x7FFFFFFF, fmt, ap);
    va_end(ap);
    return n;
}

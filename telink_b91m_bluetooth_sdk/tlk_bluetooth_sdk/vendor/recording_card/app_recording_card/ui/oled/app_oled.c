/********************************************************************************************************
 * @file    app_oled.c
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
#include "drivers.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "app_oled_fontlibrary.h"
#include "app_oled.h"

/**
  * Data Storage Format:
  * Vertical 8 pixels, MSB at bottom, left-to-right then top-to-bottom
  * Each bit corresponds to one pixel
  * 
  *      B0 B0                  B0 B0
  *      B1 B1                  B1 B1
  *      B2 B2                  B2 B2
  *      B3 B3  ------------->  B3 B3 --
  *      B4 B4                  B4 B4  |
  *      B5 B5                  B5 B5  |
  *      B6 B6                  B6 B6  |
  *      B7 B7                  B7 B7  |
  *                                    |
  *  -----------------------------------
  *  |   
  *  |   B0 B0                  B0 B0
  *  |   B1 B1                  B1 B1
  *  |   B2 B2                  B2 B2
  *  --> B3 B3  ------------->  B3 B3
  *      B4 B4                  B4 B4
  *      B5 B5                  B5 B5
  *      B6 B6                  B6 B6
  *      B7 B7                  B7 B7
  * 
  * Coordinate System Definition:
  * Top-left corner is (0, 0)
  * X-axis extends rightward, range: 0~127
  * Y-axis extends downward, range: 0~63
  * 
  *       0             X-axis           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y-axis |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */
#if (TLK_OLED_UI_EN)
static uint16_t last_crc = 0;
uint8_t         OLED_DisplayBuf[OLED_WIDTH][OLED_LENGTH]; // Modified to 72×40 pixel screen frame buffer size

/**
 * @brief Calculate CRC16 checksum for display buffer
 * @param data Pointer to data buffer
 * @param length Data length in bytes
 * @return CRC16 checksum value
 */
static uint16_t OLED_crc16(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFF;
    while (length--) {
        crc ^= *data++;
        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

/**
 * @brief Initialize CRC checksum for display buffer
 */
void OLED_crc_Init(void)
{
    memset(OLED_DisplayBuf, 0, sizeof(OLED_DisplayBuf));
    last_crc = OLED_crc16((const uint8_t *)OLED_DisplayBuf, sizeof(OLED_DisplayBuf));
}

/**
 * @brief Check if display buffer has changed
 * @return 1 if changed, 0 if unchanged
 */
int OLED_HasChanged(void)
{
    uint16_t current_crc = OLED_crc16((const uint8_t *)OLED_DisplayBuf, sizeof(OLED_DisplayBuf));

    if (current_crc != last_crc) {
        last_crc = current_crc;
        return 1;
    }

    return 0;
}

/**
 * @brief Initialize OLED hardware interface (I2C and GPIO)
 */
void OLED_hardware_init(void)
{
    i2c_set_pin(IIC_OLED_SDA_PIN, IIC_OLED_SCL_PIN);
    i2c_set_tx_dma_config(IIC_OLED_TX_DMA_CHN);
    i2c_set_rx_dma_config(IIC_OLED_RX_DMA_CHN);
    i2c_master_init();
    i2c_set_master_clk((unsigned char)(sys_clk.pclk * 1000 * 1000 / (4 * IIC_OLED_CLK_SPEED)));

    gpio_function_en(IIC_OLED_RESET);
    gpio_output_en(IIC_OLED_RESET);
    gpio_input_dis(IIC_OLED_RESET);
    gpio_set_up_down_res(IIC_OLED_RESET, GPIO_PIN_PULLUP_10K);
    gpio_set_level(IIC_OLED_RESET, 0);
}

/**
 * @brief Write command to OLED controller
 * @param Command Command byte to write (range: 0x00~0xFF)
 */
void OLED_WriteCommand(uint8_t Command)
{
    uint8_t write_buf[4];
    int     ref = 0;

    write_buf[0] = 0x00;
    write_buf[1] = Command;

    i2c_master_write_dma(IIC_SLAVE_ADDR, (unsigned char *)write_buf, 2);
    ref = (stimer_get_tick() - 2) | 1;
    while (i2c_master_busy() && !clock_time_exceed(ref, 50 * 1000));
}

/**
 * @brief Write data to OLED controller
 * @param Data Pointer to data buffer
 * @param len Number of bytes to write
 */
void OLED_WriteData(uint8_t *Data, uint8_t len)
{
    uint8_t write_buf[OLED_WIDTH + 4];
    int     ref = 0;

    write_buf[0] = 0x40;

    memcpy(&write_buf[1], Data, len);

    i2c_master_write_dma(IIC_SLAVE_ADDR, (unsigned char *)write_buf, len + 1);
    ref = (stimer_get_tick() - 2) | 1;
    while (i2c_master_busy() && !clock_time_exceed(ref, 50 * 1000));
}

/**
 * @brief Initialize OLED display controller
 * @note Must be called before using other display functions
 */
void OLED_Init(void)
{
    OLED_hardware_init(); // Initialize hardware interface first

    gpio_set_level(IIC_OLED_RESET, 1);
    delay_ms(1);
    gpio_set_level(IIC_OLED_RESET, 0);
    delay_ms(1);
    gpio_set_level(IIC_OLED_RESET, 1);
    delay_ms(1);

    OLED_WriteCommand(0xAE); /* Display off */

    OLED_WriteCommand(0x0C); /* Set lower column address */
    OLED_WriteCommand(0x11); /* Set higher column address */

    OLED_WriteCommand(0x40); /* Set display start line */

    OLED_WriteCommand(0xA4); /* Set entire display on/off */

    OLED_WriteCommand(0xA6); /* Normal / reverse */

    OLED_WriteCommand(0xD5); /* Set osc division */
    OLED_WriteCommand(0xF0);

    OLED_WriteCommand(0xA8); /* Multiplex ratio */
    OLED_WriteCommand(0x27); /* Duty = 1/40 */

    OLED_WriteCommand(0xD3); /* Set display offset */
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x20); /* Set page address mode */
    OLED_WriteCommand(0x02);

    OLED_WriteCommand(0xA1); /* Set segment remap */

    OLED_WriteCommand(0xC8); /* COM scan direction */

    OLED_WriteCommand(0xDA); /* Set COM pins */
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0xAD); /* Internal IREF setting */
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0x81); /* Contrast control */
    OLED_WriteCommand(0x1F); /* 128 */

    OLED_WriteCommand(0xD9); /* Set pre-charge period */
    OLED_WriteCommand(0x1F);

    OLED_WriteCommand(0xdb); /* Set VCOMH */
    OLED_WriteCommand(0x20);

    OLED_WriteCommand(0x8d); /* Set charge pump enable */
    OLED_WriteCommand(0x10); /* VCC: 0x14=7.5V; 0x15=6V; 0x94=8.5V; 0x95=9V */

    OLED_Clear(); // Clear frame buffer
    OLED_ShowString(0, 0, "welcome", OLED_6X8);
    OLED_ShowString(0, 8, "telink", OLED_6X8);
    OLED_Update(); // Update display to prevent garbage on startup

    // Enable display
    OLED_WriteCommand(0xAF); // Turn on OLED display

    OLED_crc_Init();
}

/**
 * @brief Set cursor position for OLED controller
 * @param Page Page address (range: 0~7)
 * @param X Column address (range: 0~127)
 * @note OLED Y-axis is written in 8-bit groups (1 page = 8 Y coordinates)
 */
void OLED_SetCursor(uint8_t Page, uint8_t X)
{
    /* Set page and column address via commands */
    OLED_WriteCommand(0xB0 | Page);              // Set page address
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4)); // Set X high nibble
    OLED_WriteCommand(0x00 | (X & 0x0F));        // Set X low nibble
}

/* Utility functions for internal use only */

/**
 * @brief Power function (X^Y)
 * @param X Base number
 * @param Y Exponent
 * @return X raised to power Y
 */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1; // Default result is 1
    while (Y--)          // Multiply Y times
    {
        Result *= X; // Accumulate multiplication
    }
    return Result;
}

/**
 * @brief Point-in-polygon test (PNPoly algorithm)
 * @param nvert Number of polygon vertices
 * @param vertex Array of X coordinates
 * @param verty Array of Y coordinates
 * @param testx Test point X coordinate
 * @param testy Test point Y coordinate
 * @return 1 if point is inside polygon, 0 otherwise
 * @note Algorithm by W. Randolph Franklin
 * @see https://wrfranklin.org/Research/Short_Notes/pnpoly.html
 */
uint8_t OLED_pnpoly(uint8_t nvert, int16_t *vertex, int16_t *verty, int16_t testx, int16_t testy)
{
    int16_t i, j, c = 0;

    for (i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((verty[i] > testy) != (verty[j] > testy)) && (testx < (vertex[j] - vertex[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertex[i])) {
            c = !c;
        }
    }
    return c;
}

/**
 * @brief Check if point is within specified angle range
 * @param X Point X coordinate
 * @param Y Point Y coordinate
 * @param StartAngle Start angle (range: -180~180)
 * @param EndAngle End angle (range: -180~180)
 * @return 1 if inside angle range, 0 otherwise
 * @note 0 degrees is right horizontal, 180/-180 is left horizontal
 *       Positive is downward, negative is upward, clockwise rotation
 */
uint8_t OLED_IsInAngle(int16_t X, int16_t Y, int16_t StartAngle, int16_t EndAngle)
{
    int16_t PointAngle;
    PointAngle = atan2(Y, X) / 3.14 * 180; // Calculate angle in degrees
    if (StartAngle < EndAngle)             // Start angle less than end angle
    {
        if (PointAngle >= StartAngle && PointAngle <= EndAngle) {
            return 1;
        }
    } else // Start angle greater than end angle
    {
        if (PointAngle >= StartAngle || PointAngle <= EndAngle) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Update entire display buffer to OLED screen
 * @note All display functions only modify the frame buffer.
 *       Call OLED_Update() or OLED_UpdateArea() to actually send data to hardware.
 */
void OLED_Update(void)
{
    uint8_t j, n;
    uint8_t data[OLED_LENGTH] = {0};
    /* Iterate through each page */
    for (j = 0; j < OLED_WIDTH; j++) {
        /* Set cursor to first column of each page */
        OLED_SetCursor(j, 0);

        for (n = 0; n < OLED_LENGTH; n++) {
            data[n] = OLED_DisplayBuf[j][n];
        }
        /* Write 128 bytes of frame buffer data to OLED hardware */
        OLED_WriteData(data, OLED_LENGTH);
    }
}

/**
 * @brief Update specified area of display buffer to OLED screen
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Width Area width (range: 0~128)
 * @param Height Area height (range: 0~64)
 * @note Updates at least the specified area. If Y range partially covers pages,
 *       the entire affected pages will be updated.
 */
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t j;
    int16_t Page, Page1;

    /* Negative coordinates need offset when calculating page address */
    /* (Y + Height - 1) / 8 + 1 is equivalent to ceiling((Y + Height) / 8) */
    Page  = Y / 8;
    Page1 = (Y + Height - 1) / 8 + 1;
    if (Y < 0) {
        Page -= 1;
        Page1 -= 1;
    }

    /* Iterate through affected pages */
    for (j = Page; j < Page1; j++) {
        if (X >= 0 && X <= OLED_LENGTH && j >= 0 && j <= OLED_WIDTH) // Clip to screen
        {
            /* Set cursor to specified column of current page */
            OLED_SetCursor(j, X);
            /* Write Width bytes of frame buffer data */
            OLED_WriteData(&OLED_DisplayBuf[j][X], Width);
        }
    }
}

/**
 * @brief Clear entire frame buffer
 * @note Call update function to actually clear the screen
 */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < OLED_WIDTH; j++) // Iterate 8 pages
    {
        for (i = 0; i < OLED_LENGTH; i++) // Iterate 128 columns
        {
            OLED_DisplayBuf[j][i] = 0x00; // Clear all frame buffer data
        }
    }
}

/**
 * @brief Clear specified area of frame buffer
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Width Area width (range: 0~128)
 * @param Height Area height (range: 0~64)
 * @note Call update function to actually clear the screen area
 */
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t i, j;

    for (j = Y; j < Y + Height; j++) // Iterate specified rows
    {
        for (i = X; i < X + Width; i++) // Iterate specified columns
        {
            if (i >= 0 && i <= OLED_LENGTH && j >= 0 && j <= OLED_WIDTH) // Clip to screen
            {
                OLED_DisplayBuf[j / 8][i] &= ~(0x01 << (j % 8)); // Clear specific bit
            }
        }
    }
}

/**
 * @brief Reverse (invert) entire frame buffer
 * @note Call update function to actually invert the screen
 */
void OLED_Reverse(void)
{
    uint8_t i, j;
    for (j = 0; j < OLED_WIDTH; j++) // Iterate 8 pages
    {
        for (i = 0; i < OLED_LENGTH; i++) // Iterate 128 columns
        {
            OLED_DisplayBuf[j][i] ^= 0xFF; // XOR all data with 0xFF
        }
    }
}

/**
 * @brief Reverse (invert) specified area of frame buffer
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Width Area width (range: 0~128)
 * @param Height Area height (range: 0~64)
 * @note Call update function to actually invert the screen area
 */
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height)
{
    int16_t i, j;

    for (j = Y; j < Y + Height; j++) // Iterate specified rows
    {
        for (i = X; i < X + Width; i++) // Iterate specified columns
        {
            if (i >= 0 && i <= OLED_LENGTH && j >= 0 && j <= OLED_WIDTH) // Clip to screen
            {
                OLED_DisplayBuf[j / 8][i] ^= 0x01 << (j % 8); // Toggle specific bit
            }
        }
    }
}

/**
 * @brief Display single ASCII character
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Char Character to display (ASCII printable character)
 * @param FontSize Font size: OLED_8X16 (8x16 pixels) or OLED_6X8 (6x8 pixels)
 * @note Call update function to actually display on screen
 */
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize)
{
    if (FontSize == OLED_8X16) // 8x16 pixel font
    {
        /* Display from OLED_F8x16 font library as 8x16 image */
        OLED_ShowImage(X, Y, 8, 16, OLED_F8x16[Char - ' ']);
    } else if (FontSize == OLED_6X8) // 6x8 pixel font
    {
        /* Display from OLED_F6x8 font library as 6x8 image */
        OLED_ShowImage(X, Y, 6, 8, OLED_F6x8[Char - ' ']);
    }
}

/**
 * @brief Display string (supports ASCII and Chinese mixed text)
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param String Null-terminated string to display
 * @param FontSize Font size: OLED_8X16 (8x16 pixels) or OLED_6X8 (6x8 pixels)
 * @note Chinese characters must be defined in OLED_CF16x16 array.
 *       If character not found, displays default graphic (box with question mark).
 *       At OLED_8X16: Chinese characters display at 16x16 pixels.
 *       At OLED_6X8: Chinese characters display as '?'.
 * @note Call update function to actually display on screen
 */
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize)
{
    uint16_t i = 0;
    char     SingleChar[5];
    uint8_t  CharLength = 0;
    uint16_t XOffset    = 0;
    uint16_t pIndex;

    while (String[i] != '\0') // Iterate through string
    {
#ifdef OLED_CHARSET_UTF8 // UTF-8 character set
        /* Extract one UTF-8 character into SingleChar buffer */
        /* Check first byte flag bits */
        if ((String[i] & 0x80) == 0x00) // 0xxxxxxx: 1-byte ASCII
        {
            CharLength    = 1;
            SingleChar[0] = String[i++];
            SingleChar[1] = '\0';
        } else if ((String[i] & 0xE0) == 0xC0) // 110xxxxx: 2-byte sequence
        {
            CharLength    = 2;
            SingleChar[0] = String[i++];
            if (String[i] == '\0') {
                break;
            }
            SingleChar[1] = String[i++];
            SingleChar[2] = '\0';
        } else if ((String[i] & 0xF0) == 0xE0) // 1110xxxx: 3-byte sequence
        {
            CharLength    = 3;
            SingleChar[0] = String[i++];
            if (String[i] == '\0') {
                break;
            }
            SingleChar[1] = String[i++];
            if (String[i] == '\0') {
                break;
            }
            SingleChar[2] = String[i++];
            SingleChar[3] = '\0';
        } else if ((String[i] & 0xF8) == 0xF0) // 11110xxx: 4-byte sequence
        {
            CharLength    = 4;
            SingleChar[0] = String[i++];
            if (String[i] == '\0') {
                break;
            }
            SingleChar[1] = String[i++];
            if (String[i] == '\0') {
                break;
            }
            SingleChar[2] = String[i++];
            if (String[i] == '\0') {
                break;
            }
            SingleChar[3] = String[i++];
            SingleChar[4] = '\0';
        } else {
            i++; // Unexpected byte, skip and continue
            continue;
        }
#endif

#ifdef OLED_CHARSET_GB2312 // GB2312 character set
        /* Extract one GB2312 character into SingleChar buffer */
        /* Check high bit of first byte */
        if ((String[i] & 0x80) == 0x00) // High bit 0: 1-byte ASCII
        {
            CharLength    = 1;
            SingleChar[0] = String[i++];
            SingleChar[1] = '\0';
        } else // High bit 1: 2-byte Chinese
        {
            CharLength    = 2;
            SingleChar[0] = String[i++];
            if (String[i] == '\0') {
                break;
            }
            SingleChar[1] = String[i++];
            SingleChar[2] = '\0';
        }
#endif

        /* Display the extracted character */
        if (CharLength == 1) // Single-byte character (ASCII)
        {
            OLED_ShowChar(X + XOffset, Y, SingleChar[0], FontSize);
            XOffset += FontSize;
        } else // Multi-byte character (Chinese)
        {
            /* Search font library for matching character */
            /* Last entry (empty string) is default/unmatched character */
            for (pIndex = 0; strcmp(OLED_CF16x16[pIndex].Index, "") != 0; pIndex++) {
                if (strcmp(OLED_CF16x16[pIndex].Index, SingleChar) == 0) {
                    break; // Found matching character index
                }
            }
            if (FontSize == OLED_8X16) // 8x16 font mode
            {
                /* Display as 16x16 image from Chinese font library */
                OLED_ShowImage(X + XOffset, Y, 16, 16, OLED_CF16x16[pIndex].Data);
                XOffset += 16;
            } else if (FontSize == OLED_6X8) // 6x8 font mode
            {
                /* Insufficient space, display '?' instead */
                OLED_ShowChar(X + XOffset, Y, '?', OLED_6X8);
                XOffset += OLED_6X8;
            }
        }
    }
}

/**
 * @brief Display unsigned decimal number
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Number Value to display (range: 0~4294967295)
 * @param Length Number of digits to display (range: 0~10)
 * @param FontSize Font size: OLED_8X16 or OLED_6X8
 * @note Call update function to actually display on screen
 */
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; i < Length; i++) // Iterate each digit
    {
        /* Extract decimal digit and convert to ASCII */
        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
    }
}

/**
 * @brief Display signed decimal number
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Number Value to display (range: -2147483648~2147483647)
 * @param Length Number of digits (excluding sign) to display (range: 0~10)
 * @param FontSize Font size: OLED_8X16 or OLED_6X8
 * @note Call update function to actually display on screen
 */
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t  i;
    uint32_t Number1;
    if (Number >= 0) // Non-negative number
    {
        OLED_ShowChar(X, Y, '+', FontSize); // Display '+' sign
        Number1 = Number;
    } else // Negative number
    {
        OLED_ShowChar(X, Y, '-', FontSize); // Display '-' sign
        Number1 = -Number;
    }

    for (i = 0; i < Length; i++) // Iterate each digit
    {
        OLED_ShowChar(X + (i + 1) * FontSize, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', FontSize);
    }
}

/**
 * @brief Display hexadecimal number
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Number Value to display (range: 0x00000000~0xFFFFFFFF)
 * @param Length Number of hex digits to display (range: 0~8)
 * @param FontSize Font size: OLED_8X16 or OLED_6X8
 * @note Call update function to actually display on screen
 */
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++) // Iterate each hex digit
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;

        if (SingleNumber < 10) {
            OLED_ShowChar(X + i * FontSize, Y, SingleNumber + '0', FontSize);
        } else {
            OLED_ShowChar(X + i * FontSize, Y, SingleNumber - 10 + 'A', FontSize);
        }
    }
}

/**
 * @brief Display binary number
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Number Value to display (range: 0x00000000~0xFFFFFFFF)
 * @param Length Number of binary digits to display (range: 0~16)
 * @param FontSize Font size: OLED_8X16 or OLED_6X8
 * @note Call update function to actually display on screen
 */
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize)
{
    uint8_t i;
    for (i = 0; i < Length; i++) // Iterate each bit
    {
        OLED_ShowChar(X + i * FontSize, Y, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', FontSize);
    }
}

/**
 * @brief Display floating-point number
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Number Value to display (range: -4294967295.0~4294967295.0)
 * @param IntLength Number of integer digits to display (range: 0~10)
 * @param FraLength Number of fractional digits to display (range: 0~9), rounded
 * @param FontSize Font size: OLED_8X16 or OLED_6X8
 * @note Call update function to actually display on screen
 */
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize)
{
    uint32_t PowNum, IntNum, FraNum;

    if (Number >= 0) // Non-negative number
    {
        OLED_ShowChar(X, Y, '+', FontSize); // Display '+' sign
    } else                                  // Negative number
    {
        OLED_ShowChar(X, Y, '-', FontSize); // Display '-' sign
        Number = -Number;
    }

    /* Extract integer and fractional parts */
    IntNum = Number;
    Number -= IntNum;
    PowNum = OLED_Pow(10, FraLength);
    FraNum = round(Number * PowNum);
    IntNum += FraNum / PowNum; // Handle carry from rounding

    /* Display integer part */
    OLED_ShowNum(X + FontSize, Y, IntNum, IntLength, FontSize);

    /* Display decimal point */
    OLED_ShowChar(X + (IntLength + 1) * FontSize, Y, '.', FontSize);

    /* Display fractional part */
    OLED_ShowNum(X + (IntLength + 2) * FontSize, Y, FraNum, FraLength, FontSize);
}

/**
 * @brief Display bitmap image
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Width Image width in pixels (range: 0~128)
 * @param Height Image height in pixels (range: 0~64)
 * @param Image Pointer to bitmap data (vertical byte format)
 * @note Call update function to actually display on screen
 */
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image)
{
    uint8_t i = 0, j = 0;
    int16_t Page, Shift;

    X += X_offset;

    /* Clear target area first */
    OLED_ClearArea(X, Y, Width, Height);

    /* Iterate through affected pages */
    /* (Height - 1) / 8 + 1 is ceiling(Height / 8) */
    for (j = 0; j < (Height - 1) / 8 + 1; j++) {
        for (i = 0; i < Width; i++) {
            if (X + i >= 0 && X + i <= OLED_LENGTH) // Clip to screen
            {
                /* Handle negative coordinate offset */
                Page  = Y / 8;
                Shift = Y % 8;
                if (Y < 0) {
                    Page -= 1;
                    Shift += 8;
                }

                if (Page + j >= 0 && Page + j <= OLED_WIDTH) // Clip to screen
                {
                    /* Display image data in current page */
                    OLED_DisplayBuf[Page + j][X + i] |= Image[j * Width + i] << (Shift);
                }

                if (Page + j + 1 >= 0 && Page + j + 1 <= OLED_WIDTH) // Clip to screen
                {
                    /* Display image data spilled to next page */
                    OLED_DisplayBuf[Page + j + 1][X + i] |= Image[j * Width + i] >> (8 - Shift);
                }
            }
        }
    }
}

/**
 * @brief Display formatted string using printf syntax
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param FontSize Font size: OLED_8X16 or OLED_6X8
 * @param format printf-style format string
 * @param ... Variable arguments for format string
 * @note Supports ASCII and Chinese mixed text. Chinese characters must be defined
 *       in OLED_CF16x16 array. Call update function to actually display on screen.
 */
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...)
{
    char    String[256];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    OLED_ShowString(X, Y, String, FontSize);
}

/**
 * @brief Draw single pixel point
 * @param X X coordinate (screen range: 0~127)
 * @param Y Y coordinate (screen range: 0~63)
 * @note Call update function to actually display on screen
 */
void OLED_DrawPoint(int16_t X, int16_t Y)
{
    if (X >= 0 && X <= 71 && Y >= 0 && Y <= 39) // Clip to screen bounds
    {
        /* Set corresponding bit in frame buffer */
        OLED_DisplayBuf[Y / 8][X] |= 0x01 << (Y % 8);
    }
}

/**
 * @brief Get pixel state at specified position
 * @param X X coordinate (screen range: 0~127)
 * @param Y Y coordinate (screen range: 0~63)
 * @return 1 if pixel is set (lit), 0 if cleared (dark)
 */
uint8_t OLED_GetPoint(int16_t X, int16_t Y)
{
    if (X >= 0 && X <= 71 && Y >= 0 && Y <= 39) // Clip to screen bounds
    {
        if (OLED_DisplayBuf[Y / 8][X] & 0x01 << (Y % 8)) {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Draw line between two points
 * @param X0 First endpoint X coordinate (screen range: 0~127)
 * @param Y0 First endpoint Y coordinate (screen range: 0~63)
 * @param X1 Second endpoint X coordinate (screen range: 0~127)
 * @param Y1 Second endpoint Y coordinate (screen range: 0~63)
 * @note Uses Bresenham's line algorithm for efficiency (no floating-point).
 * @see https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf
 * @note Call update function to actually display on screen
 */
void OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1)
{
    int16_t x, y, dx, dy, d, incrE, incrNE, temp;
    int16_t x0 = X0, y0 = Y0, x1 = X1, y1 = Y1;
    uint8_t yflag = 0, xyflag = 0;

    if (y0 == y1) // Horizontal line
    {
        if (x0 > x1) {
            temp = x0;
            x0   = x1;
            x1   = temp;
        }

        for (x = x0; x <= x1; x++) {
            OLED_DrawPoint(x, y0);
        }
    } else if (x0 == x1) // Vertical line
    {
        if (y0 > y1) {
            temp = y0;
            y0   = y1;
            y1   = temp;
        }

        for (y = y0; y <= y1; y++) {
            OLED_DrawPoint(x0, y);
        }
    } else // Diagonal line
    {
        /* Bresenham's line algorithm */
        /* Reference: https://www.bilibili.com/video/BV1364y1d7Lo */

        if (x0 > x1) {
            temp = x0;
            x0   = x1;
            x1   = temp;
            temp = y0;
            y0   = y1;
            y1   = temp;
        }

        if (y0 > y1) {
            y0    = -y0;
            y1    = -y1;
            yflag = 1;
        }

        if (y1 - y0 > x1 - x0) // Slope > 1
        {
            temp   = x0;
            x0     = y0;
            y0     = temp;
            temp   = x1;
            x1     = y1;
            y1     = temp;
            xyflag = 1;
        }

        dx     = x1 - x0;
        dy     = y1 - y0;
        incrE  = 2 * dy;
        incrNE = 2 * (dy - dx);
        d      = 2 * dy - dx;
        x      = x0;
        y      = y0;

        /* Draw start point with coordinate transformation */
        if (yflag && xyflag) {
            OLED_DrawPoint(y, -x);
        } else if (yflag) {
            OLED_DrawPoint(x, -y);
        } else if (xyflag) {
            OLED_DrawPoint(y, x);
        } else {
            OLED_DrawPoint(x, y);
        }

        while (x < x1) {
            x++;
            if (d < 0) {
                d += incrE;
            } else {
                y++;
                d += incrNE;
            }

            if (yflag && xyflag) {
                OLED_DrawPoint(y, -x);
            } else if (yflag) {
                OLED_DrawPoint(x, -y);
            } else if (xyflag) {
                OLED_DrawPoint(y, x);
            } else {
                OLED_DrawPoint(x, y);
            }
        }
    }
}

/**
 * @brief Draw rectangle
 * @param X Top-left X coordinate (screen range: 0~127)
 * @param Y Top-left Y coordinate (screen range: 0~63)
 * @param Width Rectangle width (range: 0~128)
 * @param Height Rectangle height (range: 0~64)
 * @param IsFilled Fill mode: OLED_UNFILLED (outline only) or OLED_FILLED (solid)
 * @note Call update function to actually display on screen
 */
void OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled)
{
    int16_t i, j;
    if (!IsFilled) // Outline only
    {
        for (i = X; i < X + Width; i++) {
            OLED_DrawPoint(i, Y);
            OLED_DrawPoint(i, Y + Height - 1);
        }
        for (i = Y; i < Y + Height; i++) {
            OLED_DrawPoint(X, i);
            OLED_DrawPoint(X + Width - 1, i);
        }
    } else // Filled
    {
        for (i = X; i < X + Width; i++) {
            for (j = Y; j < Y + Height; j++) {
                OLED_DrawPoint(i, j);
            }
        }
    }
}

/**
 * @brief Draw triangle
 * @param X0 First vertex X coordinate (screen range: 0~127)
 * @param Y0 First vertex Y coordinate (screen range: 0~63)
 * @param X1 Second vertex X coordinate (screen range: 0~127)
 * @param Y1 Second vertex Y coordinate (screen range: 0~63)
 * @param X2 Third vertex X coordinate (screen range: 0~127)
 * @param Y2 Third vertex Y coordinate (screen range: 0~63)
 * @param IsFilled Fill mode: OLED_UNFILLED (outline only) or OLED_FILLED (solid)
 * @note Call update function to actually display on screen
 */
void OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled)
{
    int16_t minx = X0, miny = Y0, maxx = X0, maxy = Y0;
    int16_t i, j;
    int16_t vx[] = {X0, X1, X2};
    int16_t vy[] = {Y0, Y1, Y2};

    if (!IsFilled) // Outline only
    {
        OLED_DrawLine(X0, Y0, X1, Y1);
        OLED_DrawLine(X0, Y0, X2, Y2);
        OLED_DrawLine(X1, Y1, X2, Y2);
    } else // Filled
    {
        if (X1 < minx) {
            minx = X1;
        }
        if (X2 < minx) {
            minx = X2;
        }
        if (Y1 < miny) {
            miny = Y1;
        }
        if (Y2 < miny) {
            miny = Y2;
        }

        if (X1 > maxx) {
            maxx = X1;
        }
        if (X2 > maxx) {
            maxx = X2;
        }
        if (Y1 > maxy) {
            maxy = Y1;
        }
        if (Y2 > maxy) {
            maxy = Y2;
        }

        /* Iterate bounding box and fill points inside triangle */
        for (i = minx; i <= maxx; i++) {
            for (j = miny; j <= maxy; j++) {
                if (OLED_pnpoly(3, vx, vy, i, j)) {
                    OLED_DrawPoint(i, j);
                }
            }
        }
    }
}

/**
 * @brief Draw circle
 * @param X Center X coordinate (screen range: 0~127)
 * @param Y Center Y coordinate (screen range: 0~63)
 * @param Radius Circle radius (range: 0~255)
 * @param IsFilled Fill mode: OLED_UNFILLED (outline only) or OLED_FILLED (solid)
 * @note Uses Bresenham's circle algorithm for efficiency.
 * @see https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf
 * @note Call update function to actually display on screen
 */
void OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled)
{
    int16_t x, y, d, j;

    d = 1 - Radius;
    x = 0;
    y = Radius;

    /* Draw starting points of 8 octants */
    OLED_DrawPoint(X + x, Y + y);
    OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X + y, Y + x);
    OLED_DrawPoint(X - y, Y - x);

    if (IsFilled) {
        for (j = -y; j < y; j++) {
            OLED_DrawPoint(X, Y + j);
        }
    }

    while (x < y) {
        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }

        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X + y, Y + x);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - y, Y - x);
        OLED_DrawPoint(X + x, Y - y);
        OLED_DrawPoint(X + y, Y - x);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X - y, Y + x);

        if (IsFilled) {
            for (j = -y; j < y; j++) {
                OLED_DrawPoint(X + x, Y + j);
                OLED_DrawPoint(X - x, Y + j);
            }

            for (j = -x; j < x; j++) {
                OLED_DrawPoint(X - y, Y + j);
                OLED_DrawPoint(X + y, Y + j);
            }
        }
    }
}

/**
 * @brief Draw ellipse
 * @param X Center X coordinate (screen range: 0~127)
 * @param Y Center Y coordinate (screen range: 0~63)
 * @param A Horizontal semi-axis length (range: 0~255)
 * @param B Vertical semi-axis length (range: 0~255)
 * @param IsFilled Fill mode: OLED_UNFILLED (outline only) or OLED_FILLED (solid)
 * @note Uses Bresenham's ellipse algorithm.
 * @see https://blog.csdn.net/myf_666/article/details/128167392
 * @note Call update function to actually display on screen
 */
void OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled)
{
    int16_t x, y, j;
    int16_t a = A, b = B;
    float   d1, d2;

    x  = 0;
    y  = b;
    d1 = b * b + a * a * (-b + 0.5);

    if (IsFilled) {
        for (j = -y; j < y; j++) {
            OLED_DrawPoint(X, Y + j);
            OLED_DrawPoint(X, Y + j);
        }
    }

    OLED_DrawPoint(X + x, Y + y);
    OLED_DrawPoint(X - x, Y - y);
    OLED_DrawPoint(X - x, Y + y);
    OLED_DrawPoint(X + x, Y - y);

    /* Draw middle section */
    while (b * b * (x + 1) < a * a * (y - 0.5)) {
        if (d1 <= 0) {
            d1 += b * b * (2 * x + 3);
        } else {
            d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
            y--;
        }
        x++;

        if (IsFilled) {
            for (j = -y; j < y; j++) {
                OLED_DrawPoint(X + x, Y + j);
                OLED_DrawPoint(X - x, Y + j);
            }
        }

        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X + x, Y - y);
    }

    /* Draw side sections */
    d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;

    while (y > 0) {
        if (d2 <= 0) {
            d2 += b * b * (2 * x + 2) + a * a * (-2 * y + 3);
            x++;
        } else {
            d2 += a * a * (-2 * y + 3);
        }
        y--;

        if (IsFilled) {
            for (j = -y; j < y; j++) {
                OLED_DrawPoint(X + x, Y + j);
                OLED_DrawPoint(X - x, Y + j);
            }
        }

        OLED_DrawPoint(X + x, Y + y);
        OLED_DrawPoint(X - x, Y - y);
        OLED_DrawPoint(X - x, Y + y);
        OLED_DrawPoint(X + x, Y - y);
    }
}

/**
 * @brief Draw arc (circular arc)
 * @param X Center X coordinate (screen range: 0~127)
 * @param Y Center Y coordinate (screen range: 0~63)
 * @param Radius Arc radius (range: 0~255)
 * @param StartAngle Start angle (range: -180~180)
 * @param EndAngle End angle (range: -180~180)
 * @param IsFilled Fill mode: OLED_UNFILLED (arc only) or OLED_FILLED (pie sector)
 * @note 0 degrees is right horizontal, 180/-180 is left horizontal.
 *       Positive is downward, negative is upward, clockwise rotation.
 * @note Call update function to actually display on screen
 */
void OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled)
{
    int16_t x, y, d, j;

    /* Uses modified Bresenham circle algorithm */

    d = 1 - Radius;
    x = 0;
    y = Radius;

    /* Draw points only if within angle range */
    if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) {
        OLED_DrawPoint(X + x, Y + y);
    }
    if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {
        OLED_DrawPoint(X - x, Y - y);
    }
    if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {
        OLED_DrawPoint(X + y, Y + x);
    }
    if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {
        OLED_DrawPoint(X - y, Y - x);
    }

    if (IsFilled) {
        for (j = -y; j < y; j++) {
            if (OLED_IsInAngle(0, j, StartAngle, EndAngle)) {
                OLED_DrawPoint(X, Y + j);
            }
        }
    }

    while (x < y) {
        x++;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            y--;
            d += 2 * (x - y) + 1;
        }

        if (OLED_IsInAngle(x, y, StartAngle, EndAngle)) {
            OLED_DrawPoint(X + x, Y + y);
        }
        if (OLED_IsInAngle(y, x, StartAngle, EndAngle)) {
            OLED_DrawPoint(X + y, Y + x);
        }
        if (OLED_IsInAngle(-x, -y, StartAngle, EndAngle)) {
            OLED_DrawPoint(X - x, Y - y);
        }
        if (OLED_IsInAngle(-y, -x, StartAngle, EndAngle)) {
            OLED_DrawPoint(X - y, Y - x);
        }
        if (OLED_IsInAngle(x, -y, StartAngle, EndAngle)) {
            OLED_DrawPoint(X + x, Y - y);
        }
        if (OLED_IsInAngle(y, -x, StartAngle, EndAngle)) {
            OLED_DrawPoint(X + y, Y - x);
        }
        if (OLED_IsInAngle(-x, y, StartAngle, EndAngle)) {
            OLED_DrawPoint(X - x, Y + y);
        }
        if (OLED_IsInAngle(-y, x, StartAngle, EndAngle)) {
            OLED_DrawPoint(X - y, Y + x);
        }

        if (IsFilled) {
            for (j = -y; j < y; j++) {
                if (OLED_IsInAngle(x, j, StartAngle, EndAngle)) {
                    OLED_DrawPoint(X + x, Y + j);
                }
                if (OLED_IsInAngle(-x, j, StartAngle, EndAngle)) {
                    OLED_DrawPoint(X - x, Y + j);
                }
            }

            for (j = -x; j < x; j++) {
                if (OLED_IsInAngle(-y, j, StartAngle, EndAngle)) {
                    OLED_DrawPoint(X - y, Y + j);
                }
                if (OLED_IsInAngle(y, j, StartAngle, EndAngle)) {
                    OLED_DrawPoint(X + y, Y + j);
                }
            }
        }
    }
}

#endif
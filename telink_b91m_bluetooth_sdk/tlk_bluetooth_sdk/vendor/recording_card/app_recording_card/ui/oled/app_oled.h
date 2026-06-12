/********************************************************************************************************
 * @file    app_oled.h
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
#ifndef TLK_OLED_HARDWARE_H
#define TLK_OLED_HARDWARE_H

/* FontSize parameter values */
/* This parameter is used not only for judgment but also for calculating horizontal character offset, default value is font pixel width */
#define OLED_8X16 8
#define OLED_6X8  6

/* IsFilled parameter values */
#define OLED_UNFILLED       0
#define OLED_FILLED         1

#define OLED_WIDTH          8
#define OLED_LENGTH         128

#define X_offset            28
#define Y_offset            0

#define IIC_OLED_SDA_PIN    I2C_GPIO_SDA_C0
#define IIC_OLED_SCL_PIN    I2C_GPIO_SCL_C1
#define IIC_OLED_RESET      GPIO_PA2
#define IIC_OLED_TX_DMA_CHN (tlkhal_dma_malloc())
#define IIC_OLED_RX_DMA_CHN (tlkhal_dma_malloc())
#define IIC_OLED_CLK_SPEED  400000

#define IIC_SLAVE_ADDR      0x78

/* Initialization Functions */
void OLED_Init(void);

/* Update Functions */
void OLED_Update(void);
void OLED_UpdateArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
int  OLED_HasChanged(void);

/* Framebuffer Control Functions */
void OLED_Clear(void);
void OLED_ClearArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);
void OLED_Reverse(void);
void OLED_ReverseArea(int16_t X, int16_t Y, uint8_t Width, uint8_t Height);

/* Display Functions */
void OLED_ShowChar(int16_t X, int16_t Y, char Char, uint8_t FontSize);
void OLED_ShowString(int16_t X, int16_t Y, char *String, uint8_t FontSize);
void OLED_ShowNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowSignedNum(int16_t X, int16_t Y, int32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowHexNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowBinNum(int16_t X, int16_t Y, uint32_t Number, uint8_t Length, uint8_t FontSize);
void OLED_ShowFloatNum(int16_t X, int16_t Y, double Number, uint8_t IntLength, uint8_t FraLength, uint8_t FontSize);
void OLED_ShowImage(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, const uint8_t *Image);
void OLED_Printf(int16_t X, int16_t Y, uint8_t FontSize, char *format, ...);

/* Drawing Functions */
void    OLED_DrawPoint(int16_t X, int16_t Y);
uint8_t OLED_GetPoint(int16_t X, int16_t Y);
void    OLED_DrawLine(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1);
void    OLED_DrawRectangle(int16_t X, int16_t Y, uint8_t Width, uint8_t Height, uint8_t IsFilled);
void    OLED_DrawTriangle(int16_t X0, int16_t Y0, int16_t X1, int16_t Y1, int16_t X2, int16_t Y2, uint8_t IsFilled);
void    OLED_DrawCircle(int16_t X, int16_t Y, uint8_t Radius, uint8_t IsFilled);
void    OLED_DrawEllipse(int16_t X, int16_t Y, uint8_t A, uint8_t B, uint8_t IsFilled);
void    OLED_DrawArc(int16_t X, int16_t Y, uint8_t Radius, int16_t StartAngle, int16_t EndAngle, uint8_t IsFilled);

#endif
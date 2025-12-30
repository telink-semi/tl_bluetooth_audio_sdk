/********************************************************************************************************
 * @file    tlkdrv_sy5500_i2c.c
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
#include "tlkmw/tlkmw.h"
#include "tlkapi/tlkapi.h"
#include "inner/tlkdrv_sy5500_cfg.h"
#include "inner/tlkdrv_sy5500_i2c_reg.h"
#include "tlkdrv_sy5500_i2c.h"
#if TLK_DEV_SY5500_ENABLE

/**
 * @brief I2C register write control structure
 */
static regs_write_t sTlkdrvSy5500RegWriteCtrl = {0};

/**
 * @brief I2C register cache structure
 */
static regs_cache_t sTlkdrvSy5500RegCache;

/**
 * @brief I2C event callback function pointer
 */
static TlkDrvSy5500I2cEvtCB sTlkDrvSy5500I2cEvtCB;

/**
 * @brief Delay milliseconds function
 * @param[in] ms Delay milliseconds
 */
static void tlkdrv_sy5500_i2c_delayMs(uint32_t ms)
{
    if (tlkos_task_delayMs(ms) != TLK_ENONE) {
        delay_ms(ms);
    }
}

/**
 * @brief Delay microseconds function
 * @param[in] us Delay microseconds
 */
static void tlkdrv_sy5500_i2c_delayUs(uint32_t us)
{
    delay_us(us);
}

/**
 * @brief Write data to SY5500 via I2C
 * @param[in] addr Register address
 * @param[in] data Data to be written
 */
static void tlkdrv_sy5500_i2c_write(uint8_t addr, uint8_t data)
{
    uint8_t buf[2] = {
        [0] = addr,
        [1] = data,
    };

    i2c_master_send_stop(1);
    i2c_master_write_dma(TLKDEV_SY5500_I2C_SLAVE_ADDR << 1, buf, 2);
    tlkdrv_sy5500_i2c_delayUs(150);
    uint8_t cnt1ms = 0;
    while (i2c_master_busy() && cnt1ms <= 5) {
        tlkdrv_sy5500_i2c_delayMs(1);
        cnt1ms++;
    }
}

/**
 * @brief Read data from SY5500 via I2C
 * @param[in] addr Register address
 * @param[out] data Buffer to store read data
 * @param[in] len Length of data to read
 */
static void tlkdrv_sy5500_i2c_read(uint8_t addr, uint8_t *data, uint8_t len)
{
    uint8_t cnt1ms = 0;
    i2c_master_send_stop(0);
    i2c_master_write_dma(TLKDEV_SY5500_I2C_SLAVE_ADDR << 1, &addr, 1);
    tlkdrv_sy5500_i2c_delayUs(150);
    while (i2c_master_busy() && cnt1ms <= 5) {
        tlkdrv_sy5500_i2c_delayMs(1);
        cnt1ms++;
    }

    i2c_master_send_stop(1);
    i2c_master_read_dma(TLKDEV_SY5500_I2C_SLAVE_ADDR << 1, data, len);
    tlkdrv_sy5500_i2c_delayUs(150);
    cnt1ms = 0;
    while (i2c_master_busy() && cnt1ms <= 5) {
        tlkdrv_sy5500_i2c_delayMs(1);
        cnt1ms++;
    }
}

/**
 * @brief Write a single register value
 */
static void tlkdrv_sy5500_i2c_write_single(void)
{
    switch (sTlkdrvSy5500RegWriteCtrl.reg) {
    case _ADD_SY5500_VER_WP_50:
    case _ADD_SY5500_VER_WP_60:
    case _ADD_SY5500_VER_WP_61:
    case _ADD_SY5500_VER_WP_62:
        // Clear write protection before writing, finally restore write protection
        tlkdrv_sy5500_i2c_write(_ADD_SY5500_WPEN_73, _KEY_CLR_SY5500_WPEN_73);
        tlkdrv_sy5500_i2c_write(_ADD_SY5500_WPEN_72, _KEY_CLR_SY5500_WPEN_72);
        tlkdrv_sy5500_i2c_write(sTlkdrvSy5500RegWriteCtrl.reg, sTlkdrvSy5500RegWriteCtrl.val);
        tlkdrv_sy5500_i2c_write(_ADD_SY5500_WPEN_73, _KEY_SET_SY5500_WPEN_73);
        tlkdrv_sy5500_i2c_write(_ADD_SY5500_WPEN_72, _KEY_SET_SY5500_WPEN_72);
        break;
    case _ADD_SY5500_ST_20:
    case _ADD_SY5500_I2C_CMD_44:
        // Directly write to status and command registers
        tlkdrv_sy5500_i2c_write(sTlkdrvSy5500RegWriteCtrl.reg, sTlkdrvSy5500RegWriteCtrl.val);
        break;
    default:
        // Default direct write
        tlkdrv_sy5500_i2c_write(sTlkdrvSy5500RegWriteCtrl.reg, sTlkdrvSy5500RegWriteCtrl.val);
        break;
    }
}

/**
 * @brief Verify if register write was successful
 * @return Returns true if write succeeded, false if failed
 */
static bool tlkdrv_sy5500_i2c_reg_read_ack(void)
{
    uint8_t dat = 0;
    if (sTlkdrvSy5500RegWriteCtrl.reg == _ADD_SY5500_ST_20 || sTlkdrvSy5500RegWriteCtrl.reg == _ADD_SY5500_I2C_CMD_44) {
        return true;
    }

    tlkdrv_sy5500_i2c_read(sTlkdrvSy5500RegWriteCtrl.reg, &dat, 1);
    if (dat != sTlkdrvSy5500RegWriteCtrl.val) {
        return false;
    }
    return true;
}

/**
 * @brief Read multiple register values
 * @param[in] addr Starting register address
 * @param[out] data Buffer to store read data
 * @param[in] len Number of registers to read
 */
static void tlkdrv_sy5500_i2c_readReg(uint8_t addr, uint8_t *data, uint8_t len)
{
    tlkdrv_sy5500_i2c_read(addr, data, len);
}

/**
 * @brief Write register value with retry mechanism
 * @param[in] reg Register address
 * @param[in] val Value to be written
 */
static void tlkdrv_sy5500_i2c_writeReg(uint8_t reg, uint8_t val)
{
    sTlkdrvSy5500RegWriteCtrl.reg = reg;
    sTlkdrvSy5500RegWriteCtrl.val = val;
    for (uint8_t cnt = 0; cnt < sTlkdrvSy5500RegWriteCtrl.retryTimes + 1; cnt++) {
        tlkdrv_sy5500_i2c_write_single();
        if (tlkdrv_sy5500_i2c_reg_read_ack() == true) {
            break;
        } //fail need retry
    }
}

/**
 * @brief Determine if currently in RTX mode
 * @return Returns true if in RTX mode, otherwise false
 */
static inline bool tlkdrv_sy5500_i2c_isRTXmode(void)
{
    return (_SY5500_ST_CHIP_TRX == (sTlkdrvSy5500RegCache.state[0] & 0x07));
}

/**
 * @brief Determine if earphones are in charging box
 * @return Returns true if in charging box, otherwise false
 */
static inline bool tlkdrv_sy5500_i2c_isInBox(void)
{
    return !((sTlkdrvSy5500RegCache.state[0] & (1 << 3)) == 0);
}

/**
 * @brief Determine if 5V power is connected
 * @return Returns true if 5V power is connected, otherwise false
 */
static inline bool tlkdrv_sy5500_i2c_is5vOn(void)
{
    return (sTlkdrvSy5500RegCache.state[0] & 0x07) >= _SY5500_ST_CHIP_ChargeOFF;
}

/**
 * @brief Determine if power is abnormal
 * @return Returns true if power is abnormal, otherwise false
 */
static inline bool tlkdrv_sy5500_i2c_isPowerError(void)
{
    return sTlkdrvSy5500RegCache.state[2] != 0;
}

/**
 * @brief Send I2C event to upper layer application
 * @param[in] evtID Event ID
 */
static inline void tlkdrv_sy5500_i2c_sendEvt(uint16_t evtID)
{
    if (sTlkDrvSy5500I2cEvtCB == NULL) {
        return;
    }
    sTlkDrvSy5500I2cEvtCB(evtID);
}

/**
 * @brief Exit RTX mode
 */
void tlkdrv_sy5500_i2c_exitRTXMode(void)
{
    for (int retry = 0; retry < 3; retry++) {
        if (!tlkdrv_sy5500_i2c_isRTXmode()) {
            break;
        }
        tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_I2C_CMD_44, _SY5500_I2C_CMD_ExitTRXmode);
    }
    tlkdrv_sy5500_i2c_readReg(_ADD_SY5500_ST_10, sTlkdrvSy5500RegCache.state, 2);
}

/**
 * @brief Check and handle events
 */
static void tlkdrv_sy5500_i2c_evtCheck(void)
{
    if (tlkdrv_sy5500_i2c_is5vOn()) {
        tlkdrv_sy5500_i2c_sendEvt(TLKDRV_SY5500_I2C_EVT_5V_ON);
    } else {
        tlkdrv_sy5500_i2c_sendEvt(TLKDRV_SY5500_I2C_EVT_5V_OFF);
    }
    if (tlkdrv_sy5500_i2c_isInBox()) {
        tlkdrv_sy5500_i2c_sendEvt(TLKDRV_SY5500_I2C_EVT_ENTER_BOX);
    } else {
        tlkdrv_sy5500_i2c_sendEvt(TLKDRV_SY5500_I2C_EVT_LEAVE_BOX);
    }
    if (tlkdrv_sy5500_i2c_isPowerError()) {
        tlkdrv_sy5500_i2c_sendEvt(TLKDRV_SY5500_I2C_EVT_5V_POWER_ERROR);
    }
}

/**
 * @brief Set default state register values
 */
static void tlkdrv_sy5500_setDefaultState(void)
{
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_IO_CaseCheck_40, 0x40);
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_RstDelay_set_41, 0x50 | (1 << 7) | (7 << 1));
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_PWKSet_42, 0x00);
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_VinPullDown_43, 0x01);
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_VER_WP_50, 0xa5);
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_VER_WP_60, 0x3a);
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_VER_WP_62, 0x01);
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_IO_CaseCheck_40, 0x50);

    if (gpio_get_level(TLKDEV_SY5500_I2C_PIN_IRQ) == 0) {
        tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_I2C_CMD_44, _SY5500_I2C_CMD_ClrIrq);
    }
    sTlkdrvSy5500RegWriteCtrl.haveChanged = 1;
}

/**
 * @brief Initialize SY5500 I2C interface
 * @param[in] writeRetryTimes Write retry times
 * @param[in] evtCB Event callback function
 */
void tlkdrv_sy5500_i2c_init(uint8_t writeRetryTimes, TlkDrvSy5500I2cEvtCB evtCB)
{
    // GPIO IRQ pin initialization
    sTlkDrvSy5500I2cEvtCB                = evtCB;
    sTlkdrvSy5500RegWriteCtrl.retryTimes = writeRetryTimes;
    gpio_function_en(TLKDEV_SY5500_I2C_PIN_IRQ);
    gpio_output_dis(TLKDEV_SY5500_I2C_PIN_IRQ);
    gpio_input_en(TLKDEV_SY5500_I2C_PIN_IRQ);
    gpio_set_up_down_res(TLKDEV_SY5500_I2C_PIN_IRQ, GPIO_PIN_PULLUP_1M);

    // I2C interface initialization
    i2c_set_pin((i2c_sda_pin_e)TLKDEV_SY5500_I2C_PIN_SDA, (i2c_scl_pin_e)TLKDEV_SY5500_I2C_PIN_SCL);
    i2c_set_rx_dma_config(TLKDEV_SY5500_I2C_RX_DMA_CHN);
    i2c_set_tx_dma_config(TLKDEV_SY5500_I2C_TX_DMA_CHN);

    i2c_master_init();
    i2c_set_master_clk((unsigned char)(sys_clk.pclk * 1000 * 1000 / (4 * TLKDEV_SY5500_I2C_SPEED)));

    // Read initial state register values
    tlkdrv_sy5500_i2c_readReg(_ADD_SY5500_ST_10, sTlkdrvSy5500RegCache.state, 2);
    tlkdrv_sy5500_i2c_readReg(_ADD_SY5500_ST_20, &sTlkdrvSy5500RegCache.state[2], 1);

    tlkapi_printf(TLKDEV_SY5500_DEBUG_LOG_EN, "tlkdrv_sy5500_i2c_start %x %x %x", sTlkdrvSy5500RegCache.state[0], sTlkdrvSy5500RegCache.state[1], sTlkdrvSy5500RegCache.state[2]);
    tlkdrv_sy5500_setDefaultState();
}

/**
 * @brief Handle I2C state changes
 */
void tlkdrv_sy5500_i2c_stateChgHandler(void)
{
    if (gpio_get_level(TLKDEV_SY5500_I2C_PIN_IRQ) != 0 && sTlkdrvSy5500RegWriteCtrl.haveChanged == 0) {
        return;
    }
    sTlkdrvSy5500RegWriteCtrl.haveChanged = 0;
    tlkdrv_sy5500_i2c_readReg(_ADD_SY5500_ST_10, sTlkdrvSy5500RegCache.state, 2);
    tlkdrv_sy5500_i2c_readReg(_ADD_SY5500_ST_20, &sTlkdrvSy5500RegCache.state[2], 1);
    tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_I2C_CMD_44, _SY5500_I2C_CMD_ClrIrq);

    tlkapi_printf(TLKDEV_SY5500_DEBUG_LOG_EN, "tlkdrv_sy5500_i2c_stateChgHandler %x %x %x", sTlkdrvSy5500RegCache.state[0], sTlkdrvSy5500RegCache.state[1],
                  sTlkdrvSy5500RegCache.state[2]);
    tlkdrv_sy5500_i2c_evtCheck();

    if (sTlkdrvSy5500RegCache.state[2]) {
        tlkdrv_sy5500_i2c_writeReg(_ADD_SY5500_ST_20, sTlkdrvSy5500RegCache.state[2]);
        sTlkdrvSy5500RegCache.state[2] = 0;
    }
}
#endif
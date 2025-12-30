/********************************************************************************************************
 * @file    sd_nand.c
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
#if TLK_CFG_FS_ENABLE && MCU_CORE_TYPE == CHIP_TYPE_TL721X
#include "drivers.h"
#include "sd_nand.h"
#include "hal_spi.h"
#include "tlkapi/tlkapi.h"
#include "../tlkmw_fs_diskio.h"

#define SD_NAND_POWER_ON_PIN GPIO_PA2

#define SD_LOG(fmt, ...)     tlkapi_trace(0xFFFFFFFF, "[FS]", fmt, ##__VA_ARGS__)

// #define SD_LOG(fmt, ...) ;

sd_nand_t fs_sd_nand;

/**
 * @brief       This function calculates CRC7 for SD commands.
 * @param[in]   data    - pointer to data buffer.
 * @param[in]   len     - length of data.
 * @return      Returns the calculated CRC7 value.
 */
uint8_t sd_crc7_block(const uint8_t *data, uint32_t len)
{
    // crc7 table, poly is 0x09
    static uint8_t sd_crc7_table[256] = {
        0x00, 0x09, 0x12, 0x1B, 0x24, 0x2D, 0x36, 0x3F, 0x48, 0x41, 0x5A, 0x53, 0x6C, 0x65, 0x7E, 0x77, 0x19, 0x10, 0x0B, 0x02, 0x3D, 0x34, 0x2F, 0x26, 0x51, 0x58,
        0x43, 0x4A, 0x75, 0x7C, 0x67, 0x6E, 0x32, 0x3B, 0x20, 0x29, 0x16, 0x1F, 0x04, 0x0D, 0x7A, 0x73, 0x68, 0x61, 0x5E, 0x57, 0x4C, 0x45, 0x2B, 0x22, 0x39, 0x30,
        0x0F, 0x06, 0x1D, 0x14, 0x63, 0x6A, 0x71, 0x78, 0x47, 0x4E, 0x55, 0x5C, 0x64, 0x6D, 0x76, 0x7F, 0x40, 0x49, 0x52, 0x5B, 0x2C, 0x25, 0x3E, 0x37, 0x08, 0x01,
        0x1A, 0x13, 0x7D, 0x74, 0x6F, 0x66, 0x59, 0x50, 0x4B, 0x42, 0x35, 0x3C, 0x27, 0x2E, 0x11, 0x18, 0x03, 0x0A, 0x56, 0x5F, 0x44, 0x4D, 0x72, 0x7B, 0x60, 0x69,
        0x1E, 0x17, 0x0C, 0x05, 0x3A, 0x33, 0x28, 0x21, 0x4F, 0x46, 0x5D, 0x54, 0x6B, 0x62, 0x79, 0x70, 0x07, 0x0E, 0x15, 0x1C, 0x23, 0x2A, 0x31, 0x38, 0x41, 0x48,
        0x53, 0x5A, 0x65, 0x6C, 0x77, 0x7E, 0x09, 0x00, 0x1B, 0x12, 0x2D, 0x24, 0x3F, 0x36, 0x58, 0x51, 0x4A, 0x43, 0x7C, 0x75, 0x6E, 0x67, 0x10, 0x19, 0x02, 0x0B,
        0x34, 0x3D, 0x26, 0x2F, 0x73, 0x7A, 0x61, 0x68, 0x57, 0x5E, 0x45, 0x4C, 0x3B, 0x32, 0x29, 0x20, 0x1F, 0x16, 0x0D, 0x04, 0x6A, 0x63, 0x78, 0x71, 0x4E, 0x47,
        0x5C, 0x55, 0x22, 0x2B, 0x30, 0x39, 0x06, 0x0F, 0x14, 0x1D, 0x25, 0x2C, 0x37, 0x3E, 0x01, 0x08, 0x13, 0x1A, 0x6D, 0x64, 0x7F, 0x76, 0x49, 0x40, 0x5B, 0x52,
        0x3C, 0x35, 0x2E, 0x27, 0x18, 0x11, 0x0A, 0x03, 0x74, 0x7D, 0x66, 0x6F, 0x50, 0x59, 0x42, 0x4B, 0x17, 0x1E, 0x05, 0x0C, 0x33, 0x3A, 0x21, 0x28, 0x5F, 0x56,
        0x4D, 0x44, 0x7B, 0x72, 0x69, 0x60, 0x0E, 0x07, 0x1C, 0x15, 0x2A, 0x23, 0x38, 0x31, 0x46, 0x4F, 0x54, 0x5D, 0x62, 0x6B, 0x70, 0x79};

    uint8_t crc = 0x00;

    while (len--) {
        crc = sd_crc7_table[(crc << 1) ^ (*data++)];
    }

    return (crc << 1) | 0x01;
}

/**
 * @brief       This function ignores bytes during SD communication.
 * @param[in]   neg_dat    - negative data value to ignore.
 * @param[in]   n          - maximum number of bytes to ignore.
 * @return      Returns the first non-matching byte or the last byte checked.
 */
uint8_t sd_nand_byte_ignor(uint8_t neg_dat, int n)
{
    uint8_t tmp;

    do {
        hal_spi_master_byte_trans(0xff, &tmp);

        if (tmp != neg_dat) {
            break;
        }
        n--;
    } while (n);

    if (!n) {
        return neg_dat;
    }

    return tmp;
}

/**
 * @brief       This function waits for a specific byte during SD communication.
 * @param[in]   dat    - data byte to wait for.
 * @param[in]   n      - maximum number of attempts.
 * @return      Returns 1 if byte is found, 0 otherwise.
 */
uint8_t sd_nand_byte_wait_until(uint8_t dat, int n)
{
    uint8_t tmp;

    do {
        hal_spi_master_byte_trans(0xff, &tmp);

        if (tmp == dat) {
            break;
        }
        n--;
    } while (n);

    if (!n) {
        return 0;
    }

    return 1;
}

/**
 * @brief       This function sends a command to the SD card.
 * @param[in]   wd      - pointer to command data.
 * @param[in]   wlen    - length of command data.
 * @param[out]  r1      - pointer to store R1 response.
 * @param[out]  rd      - pointer to store response data.
 * @param[in]   rlen    - length of response data.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_cmd_send(uint8_t *wd, int wlen, uint8_t *r1, uint8_t *rd, int rlen)
{
    /* Physical Layer Specification Version 3.01 
     * 7.5.1 Command/Response 
     */
    uint8_t tmp, cmd;
    cmd = wd[0] & 0x3f;

    // Select first
    hal_spi_master_sel(0);

    if (cmd == _SD_CMD_GO_IDLE_STATE) {
        goto L_TX_CMD;
    }

    // N-CS, wait MISO high, which the read back data is non-zero
    if (sd_nand_byte_ignor(0x00, 10) == 0x00) {
        hal_spi_master_sel(1);
        return 0;
    }

L_TX_CMD:

    // 6 Bytes Command
    hal_spi_master_write(wd, wlen);

    // N-CR, Max is 8, Read R1
    *r1 = sd_nand_byte_ignor(0xff, 10);
    if (*r1 == 0xff) {
        hal_spi_master_sel(1);
        return 0;
    }

    // Parameter request
    if (!rd || !rlen) {
        goto L_END;
    }

    // Wait 0xFE data token
    if (cmd == _SD_CMD_SEND_CSD) {
        // The 0xFE is data token
        if (!sd_nand_byte_wait_until(0xfe, 10)) {
            hal_spi_master_sel(1);
            return 0;
        }
    }

    // Get data
    hal_spi_master_read(rd, rlen);

L_END:

    hal_spi_master_byte_trans(0xff, &tmp);
    hal_spi_master_sel(1);
    return 1;
}

/**
 * @brief       This function waits for R1 response from SD card.
 * @param[in]   cmd         - command index.
 * @param[in]   parameter   - command parameter.
 * @return      Returns R1 response byte or 0xff on error.
 */
uint8_t sd_nand_cmd_wait_r1(uint8_t cmd, uint32_t parameter)
{
    uint8_t buf[8];
    uint8_t r1;

    // Generate command
    buf[0] = cmd | 0x40;
    buf[1] = parameter >> 24;
    buf[2] = parameter >> 16;
    buf[3] = parameter >> 8;
    buf[4] = parameter;
    buf[5] = sd_crc7_block(buf, 5);

    // N-CS, wait MISO high, which the read back data is non-zero
    if (cmd != _SD_CMD_STOP_TRANSMISSION && sd_nand_byte_ignor(0x00, 10) == 0x00) {
        goto L_ERR;
    }

    // 6 Bytes Command
    hal_spi_master_write(buf, 6);

    // N-CR, Max is 8, Read R1
    r1 = sd_nand_byte_ignor(0xff, 10);

    if (r1 == 0xff) {
        goto L_ERR;
    }

    // Error
    if (r1 > 1) {
        goto L_ERR;
    }

    return r1;

L_ERR:
    return 0xff;
}

/**
 * @brief       This function waits for the start token during data transfer.
 * @param[in]   n    - maximum number of attempts.
 * @return      Returns 1 on success, -1 on data error token, 0 on timeout.
 */
int sd_nand_wait_start_token(int n)
{
    uint8_t tmp;

    do {
        hal_spi_master_byte_trans(0xff, &tmp);

        if (tmp == TOKEN_START_READ) {
            return 1;
        } else if (((tmp & 0xf0) == 0x00) && (tmp & 0x0f)) {
            /*
             * If a read operation fails and the card cannot provide the required data, 
             * it will send a data error token instead. 
             */
            return -1;
        }
    } while (--n);

    return 0;
}

/**
 * @brief       This function receives data from the SD card.
 * @param[out]  dat          - pointer to buffer to store received data.
 * @param[in]   block_size   - size of data block.
 * @param[in]   is_last_block - flag indicating if this is the last block.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_receive_dat(uint8_t *dat, int block_size, int is_last_block)
{
    // CMD12 buffer left move 2bits
    // 0x4c, 0x00, 0x00, 0x00, 0x00, 0x61
    uint8_t cmd12x[] = {0xA6, 0x00, 0x00, 0x00, 0x00, 0x30, 0xFF, 0xFF};
    uint8_t crc[2];
    uint8_t r1;
    int     i;

    if (sd_nand_wait_start_token(10000) != 1) {
        return 0;
    }

    // Read buffer
    if (!hal_spi_master_read(dat, block_size)) {
        return 0;
    }

    // Read CRC
    if (!hal_spi_master_read(crc, 2)) {
        return 0;
    }

    // Check CRC

    // Send command after one clock
    if (is_last_block) {
        if (sd_nand_wait_start_token(10000) != 1) {
            return 0;
        }

        if (!hal_spi_master_write(cmd12x, 7)) {
            return 0;
        }

        // wait 0xF0
        for (i = 0; i < 100; i++) {
            hal_spi_master_byte_trans(0xff, &crc[0]);
            if (crc[0] == 0x00 || crc[0] == 0XFF) {
                continue;
            }
            uint8_t index = 0x80;
            uint8_t j     = 0;
            for (j = 0; j < 8; j++) {
                if ((crc[0] & index) == 0x00) {
                    break;
                }
                index = index >> 1;
            }
            if (j < 8) {
                index = 0xff;
                hal_spi_master_byte_trans(0xff, &crc[1]);
                r1 = (crc[0] & (index >> j)) << j | (crc[1] & (index << (8 - j))) >> (8 - j);
                break;
            }
        }

        if (i >= 100 || r1 != 0x00) {
            return 0;
        }
    }

    return 1;
}

/**
 * @brief       This function sends data to the SD card.
 * @param[in]   token       - data token to send.
 * @param[in]   dat         - pointer to data to send.
 * @param[in]   block_size  - size of data block.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_send_data(uint8_t token, uint8_t *dat, int block_size)
{
    uint8_t crc[4];
    uint8_t tmp;

    // Wait ready
    if (sd_nand_byte_ignor(0x00, 100000) == 0x00) {
        return 0;
    }

    // TX token
    hal_spi_master_byte_trans(token, &tmp);

    // TX data
    if (!hal_spi_master_write(dat, block_size)) {
        return 0;
    }

    // TX CRC
    crc[0] = 0xff;
    crc[1] = 0xff;
    if (!hal_spi_master_write(crc, 2)) {
        return 0;
    }

    // Get data response
    tmp = 0xff;
    hal_spi_master_byte_trans(0xff, &tmp);

    if ((tmp & DRT_MASK) != DRT_ACCEPT) {
        return 0;
    }

    return 1;
}

/**
 * @brief       This function reads blocks from the SD NAND device.
 * @param[in]   sd_nand      - pointer to SD NAND device structure.
 * @param[in]   lba          - logical block address to read from.
 * @param[out]  dat          - pointer to buffer to store read data.
 * @param[in]   block_count  - number of blocks to read.
 * @return      Returns 0 on success, 1 on failure.
 */
int sd_nand_block_read(sd_nand_t *sd_nand, uint32_t lba, uint8_t *dat, int block_count)
{
    /* Physical Layer Specification Version 3.01
     * 7.2.3 Data Read
     */
    int     n;
    int     ret = 1;
    uint8_t r1;

    if (sd_nand->state != SD_NAND_ST_INIT_DONE || !block_count) {
        return 1;
    }

    // Select first
    hal_spi_master_sel(0);

    // Send command
    r1 = sd_nand_cmd_wait_r1(block_count > 1 ? _SD_CMD_READ_MULT_BLOCK : _SD_CMD_READ_SINGLE_BLOCK, lba);

    // Read block
    for (n = 0; n < block_count; n++) {
        // Data Token start
        if (!sd_nand_receive_dat(dat + n * sd_nand->block_size, sd_nand->block_size, block_count > 1 && n == block_count - 1)) {
            goto L_ERR;
        }
    }

    // Release, data read done
    ret = 0;

L_ERR:
    if (ret) {
        SD_LOG("sd read error");
    }
    hal_spi_master_byte_trans(0xff, &r1);
    hal_spi_master_sel(1);
    return ret;
}

/**
 * @brief       This function writes blocks to the SD NAND device.
 * @param[in]   sd_nand      - pointer to SD NAND device structure.
 * @param[in]   lba          - logical block address to write to.
 * @param[in]   dat          - pointer to data to write.
 * @param[in]   block_count  - number of blocks to write.
 * @return      Returns 0 on success, 1 on failure.
 */
int sd_nand_block_write(sd_nand_t *sd_nand, uint32_t lba, uint8_t *dat, int block_count)
{
    /* Physical Layer Specification Version 3.01
     * 7.5.3 Data Write
     */
    int     i, ret;
    uint8_t r1;

    ret = 1;

    if (sd_nand->state != SD_NAND_ST_INIT_DONE || !block_count) {
        return 1;
    }

    // Select first
    hal_spi_master_sel(0);

    // Erase advance
    if (block_count > 1) {
        // TX CMD55 First
        if (sd_nand_cmd_wait_r1(_SD_CMD_APP_CMD, 0) > 1) {
            goto L_ERR;
        }

        // TX CMD23
        if (sd_nand_cmd_wait_r1(_SD_CMD_SET_BLOCK_COUNT, block_count) > 1) {
            goto L_ERR;
        }

        // Start write command
        r1 = sd_nand_cmd_wait_r1(_SD_CMD_WRITE_MULT_BLOCK, lba);
    } else {
        // Start write command
        r1 = sd_nand_cmd_wait_r1(_SD_CMD_WRITE_SINGLE_BLOCK, lba);
    }

    if (r1 > 1) {
        goto L_ERR;
    }

    // Data transfer
    for (i = 0; i < block_count; i++) {
        if (!sd_nand_send_data(block_count > 1 ? TOKEN_START_WRITE : TOKEN_START_READ, dat + sd_nand->block_size * i, sd_nand->block_size)) {
            goto L_ERR;
        }
    }

    // TX stop token
    if (block_count > 1) {
        // Must wait busy finished
        sd_nand_byte_wait_until(0xff, 1000);
        // Must wait done
        hal_spi_master_byte_trans(TOKEN_STOP_TRANS, &r1);
    }

    // May be 10mS or more !
    sd_nand_byte_wait_until(0xff, 100000);

    ret = 0;

L_ERR:
    if (ret) {
        SD_LOG("sd write error");
    }
    hal_spi_master_byte_trans(0xff, &r1);
    hal_spi_master_sel(1);
    return ret;
}

/**
 * @brief       This function swaps bytes in place.
 * @param[in]   buf    - pointer to buffer.
 * @param[in]   len    - length of buffer.
 * @return      none.
 */
void sd_nand_swap_in_place(void *buf, int len)
{
    uint8_t *u8ptr;
    uint8_t  tmp;
    int      i;
    int      j;

    u8ptr = buf;

    for (i = 0, j = len - 1; i < j; i++, j--) {
        tmp = u8ptr[i];

        u8ptr[i] = u8ptr[j];
        u8ptr[j] = tmp;
    }
}

/**
 * @brief       This function performs dummy write for SD NAND initialization.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @return      Returns 1 on success.
 */
int sd_nand_st_dummy_write(sd_nand_t *sd_nand)
{
    hal_spi_master_sel(1);
    hal_spi_master_dummy(10);

    // Change state to CMD0
    sd_nand->cmd_retry = 3;
    sd_nand->state     = SD_NAND_ST_CMD0;
    return 1;
}

/**
 * @brief       This function executes CMD0 (GO_IDLE_STATE) for SD NAND initialization.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_st_cmd0(sd_nand_t *sd_nand)
{
    uint8_t cmd0[] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95};

    uint8_t r1;

    // Decrease
    if (sd_nand->cmd_retry) {
        sd_nand->cmd_retry--;
    } else {
        return 0;
    }

    if (!sd_nand_cmd_send(cmd0, sizeof(cmd0), &r1, NULL, 0)) {
        return 0;
    }

    // 0x01 is idle
    if (r1 != SD_ERR_IDLE) {
        return 0;
    }

    SD_LOG("Go idle state ACK -> %02x\r\n", r1);

    // Change state to CMD0
    sd_nand->cmd_retry = 3;
    sd_nand->state     = SD_NAND_ST_CMD8;

    return 1;
}

/**
 * @brief       This function executes CMD8 (SEND_IF_COND) for SD NAND initialization.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_st_cmd8(sd_nand_t *sd_nand)
{
    uint8_t     cmd8[] = {0x48, 0x00, 0x00, 0x01, 0xaa, 0x87};
    uint8_t     r1;
    sd_spi_r7_t r7;
    int         len = sizeof(sd_spi_r7_t);

    // Decrease
    if (sd_nand->cmd_retry) {
        sd_nand->cmd_retry--;
    } else {
        return 0;
    }

    if (!sd_nand_cmd_send(cmd8, sizeof(cmd8), &r1, (uint8_t *)&r7, len)) {
        return 0;
    }

    if (r1 != SD_ERR_IDLE) {
        return 0;
    }

    // Not equal with CMD8
    if (r7.echo_back != 0xaa) {
        SD_LOG("R7 echo not eque -> %02x\r\n", r7.echo_back);
        return 0;
    }

    // Voltage not support
    if (r7.va != 0x01) {
        SD_LOG("R7 voltage not support -> %d\r\n", r7.va);
        return 0;
    }

    // Get Card version
    sd_nand->card_type = r7.cmd_ver;

    SD_LOG("Send interface condition ACK, support 2.7~3.6V, cmd ver is %d\r\n", r7.cmd_ver);

    // Change state to CMD0
    sd_nand->cmd_retry = 3;
    sd_nand->state     = SD_NAND_ST_CMD58_READ_OCR;

    return 1;
}

/**
 * @brief       This function executes CMD58 (READ_OCR) for SD NAND initialization.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_st_cmd58_read_ocr(sd_nand_t *sd_nand)
{
    uint8_t  cmd58[] = {0x7a, 0x00, 0x00, 0x00, 0x00, 0xfd};
    uint8_t  r1;
    uint32_t ocr;

    // Decrease
    if (sd_nand->cmd_retry) {
        sd_nand->cmd_retry--;
    } else {
        return 0;
    }

    if (!sd_nand_cmd_send(cmd58, sizeof(cmd58), &r1, (uint8_t *)&ocr, 4)) {
        return 0;
    }

    // Swap OCR
    sd_nand_swap_in_place(&ocr, 4);

    if (r1 != SD_ERR_IDLE) {
        return 0;
    }

    SD_LOG("OCR is %08x \r\n", ocr);

    sd_nand->card_type = (ocr & 0x40000000) ? SDC_V2 : SDC_V2_SDHC;


    // Change state to ACMD41
    sd_nand->cmd_retry = 30;
    sd_nand->tick_ref  = stimer_get_tick();
    sd_nand->state     = SD_NAND_ST_ACMD41;

    return 1;
}

/**
 * @brief       This function decodes CSD register data.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @param[in]   csd        - pointer to CSD data.
 * @param[in]   len        - length of CSD data.
 * @return      Returns 1 on success, 0 on failure.
 */
static int sd_nand_csd_decode(sd_nand_t *sd_nand, uint8_t *csd, int len)
{
    (void)len;
    //SD_STR_HEX("CSD RB:", csd, len);

    // CSD data
    sd_nand->csd_ver = csd[0] >> 6;

    // Only support SD ver2.0
    if (sd_nand->csd_ver != 0x01) {
        SD_LOG("CSD Version not support %d\r\n", sd_nand->csd_ver);
        return 0;
    }

    // Get block size
    sd_nand->block_size = 1;
    sd_nand->block_size <<= csd[5] & 0x0f;

    // Block counter
    sd_nand->c_size = ((csd[7] & 0x3f) << 16) | (csd[8] << 8) | (csd[9]);

    SD_LOG("CSD STR %d, READ BLK LEN %d, CAP SIZE %d\r\n", sd_nand->csd_ver, sd_nand->block_size, sd_nand->c_size);

    sd_nand->state = SD_NAND_ST_INIT_DONE;
    return 1;
}

/**
 * @brief       This function executes CMD9 (SEND_CSD) for SD NAND initialization.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_st_cmd9(sd_nand_t *sd_nand)
{
    float capacity;
    // Get sector number and sector size
    uint8_t cmd9[] = {0x49, 0x00, 0x00, 0x00, 0x00, 0xaf};
    uint8_t r1;
    uint8_t csd[32];

    // Decrease
    if (sd_nand->cmd_retry) {
        sd_nand->cmd_retry--;
    } else {
        return 0;
    }

    if (!sd_nand_cmd_send(cmd9, sizeof(cmd9), &r1, csd, 16)) {
        return 0;
    }

    // error occurred
    if (r1 > 1) {
        return 0;
    }

    if (!sd_nand_csd_decode(sd_nand, csd, 16)) {
        return 0;
    }

    // memory capacity=(C_SIZE+1)*512KByte
    capacity = (sd_nand->c_size + 1) * 512.0 / 1024.0 / 1024.0;
    (void)capacity;
    //SD_STR_HEX("CMD9 RSP ", csd, 16);
    SD_LOG("SD nand total capacity %d.%dGB\r\n", (int)capacity, ((int)(capacity * 10)) % 10);

    sd_nand->state = SD_NAND_ST_INIT_DONE;

    // Change clock to high speed
    hal_spi_master_clk_set(SPI_CLK_20MHZ);
    return 1;
}

/**
 * @brief       This function executes ACMD41 for SD NAND initialization.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_st_acmd41(sd_nand_t *sd_nand)
{
    uint8_t cmd55[]  = {0x77, 0x00, 0x00, 0x00, 0x00, 0x65};
    uint8_t acmd41[] = {0x69, 0x40, 0x00, 0x00, 0x00, 0x77};
    uint8_t r1;

    // Check every 100mS
    if (!clock_time_exceed(sd_nand->tick_ref, 100 * 1000)) {
        return 0;
    }

    // Decrease
    if (sd_nand->cmd_retry) {
        sd_nand->cmd_retry--;
    } else {
        return 0;
    }

    sd_nand->tick_ref = stimer_get_tick();

    // TX CMD55 first
    if (!sd_nand_cmd_send(cmd55, sizeof(cmd55), &r1, NULL, 0)) {
        return 0;
    }

    // TX CMD41 next
    if (!sd_nand_cmd_send(acmd41, sizeof(acmd41), &r1, NULL, 0)) {
        return 0;
    }

    if (r1 != SD_ERR_NO_ERR) {
        return 0;
    }

    SD_LOG("ACMD41 R1 = %02x, Retry remain %d\r\n", r1, sd_nand->cmd_retry);

    sd_nand->state     = SD_NAND_ST_CMD58_GET_CCS;
    sd_nand->cmd_retry = 3;
    return 1;
}

/**
 * @brief       This function executes CMD58 (READ_OCR) to get CCS for SD NAND initialization.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @return      Returns 1 on success, 0 on failure.
 */
int sd_nand_st_cmd58_read_ccs(sd_nand_t *sd_nand)
{
    uint8_t  cmd58[] = {0x7a, 0x00, 0x00, 0x00, 0x00, 0xfd};
    uint8_t  r1;
    uint32_t ocr;

    // Decrease
    if (sd_nand->cmd_retry) {
        sd_nand->cmd_retry--;
    } else {
        return 0;
    }

    if (!sd_nand_cmd_send(cmd58, sizeof(cmd58), &r1, (uint8_t *)&ocr, 4)) {
        SD_LOG("Read failed !\r\n");
        return 0;
    }

    if (r1 != SD_ERR_NO_ERR) {
        return 0;
    }

    // Swap OCR
    sd_nand_swap_in_place(&ocr, 4);

    SD_LOG("OCR is %08x, init done !\r\n", ocr);

    // Init finished !
    sd_nand->state     = SD_NAND_ST_CMD9;
    sd_nand->cmd_retry = 3;

    return 1;
}

/**
 * @brief       This function powers on the SD NAND device.
 * @param[in]   none.
 * @return      none.
 */
static void sd_nand_power_on(void)
{
    gpio_write(SD_NAND_POWER_ON_PIN, 1);
    gpio_set_output_en(SD_NAND_POWER_ON_PIN, 1);
    gpio_set_gpio_en(SD_NAND_POWER_ON_PIN);
    gpio_set_up_down_res(SD_NAND_POWER_ON_PIN, GPIO_PIN_PULLUP_1M);
    if (-TLK_ENOSUPPORT == tlkos_task_delayMs(1)) {
        delay_ms(1);
    }
}

/**
 * @brief       This function powers off the SD NAND device.
 * @param[in]   none.
 * @return      none.
 */
static void sd_nand_power_off(void)
{
    gpio_write(SD_NAND_POWER_ON_PIN, 0);
    gpio_set_output_en(SD_NAND_POWER_ON_PIN, 1);
    gpio_set_gpio_en(SD_NAND_POWER_ON_PIN);
    gpio_set_up_down_res(SD_NAND_POWER_ON_PIN, GPIO_PIN_PULLDOWN_100K);
}

/**
 * @brief       This function initializes the SD NAND device.
 * @param[in]   sd_nand    - pointer to SD NAND device structure.
 * @return      none.
 */
void sd_nand_init(sd_nand_t *sd_nand)
{
    sd_nand_power_on();
    hal_spi_master_init();

    sd_nand->state     = SD_NAND_ST_DUMMY_WRITE;
    sd_nand->cmd_retry = 3;
    hal_spi_master_clk_set(SPI_CLK_250KHZ);
    while (1) {
        switch (sd_nand->state) {
        case SD_NAND_ST_DUMMY_WRITE:
            sd_nand_st_dummy_write(sd_nand);
            break;

        case SD_NAND_ST_CMD0:
            sd_nand_st_cmd0(sd_nand);
            break;

        case SD_NAND_ST_CMD8:
            sd_nand_st_cmd8(sd_nand);
            break;

        case SD_NAND_ST_CMD58_READ_OCR:
            sd_nand_st_cmd58_read_ocr(sd_nand);
            break;

        case SD_NAND_ST_ACMD41:
            sd_nand_st_acmd41(sd_nand);
            break;

        case SD_NAND_ST_CMD58_GET_CCS:
            sd_nand_st_cmd58_read_ccs(sd_nand);
            break;

        case SD_NAND_ST_CMD9:
            sd_nand_st_cmd9(sd_nand);
            break;

        case SD_NAND_ST_INIT_DONE:
            return;
        case SD_NAND_ST_IDLE:
        default:
            break;
        }

        if (sd_nand->cmd_retry == 0 && (sd_nand->state != SD_NAND_ST_IDLE && sd_nand->state != SD_NAND_ST_INIT_DONE)) {
            SD_LOG("Command timeout, enter idle state %d \r\n", sd_nand->state);

            sd_nand->state = SD_NAND_ST_IDLE;

            // Set clock to low speed
            hal_spi_master_clk_set(SPI_CLK_250KHZ);
            return;
        }
    }
    if (sd_nand->state != SD_NAND_ST_INIT_DONE) {
        SD_LOG("sd_nand init fail %d", sd_nand->state);
    }
}

/**
 * @brief       This function initializes the SPI SD disk IO.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_spisd_init(void)
{
    sd_nand_init(&fs_sd_nand);
}

/**
 * @brief       This function writes data to the SPI SD disk IO.
 * @param[in]   buff    - pointer to data buffer to write.
 * @param[in]   lba     - logical block address to write to.
 * @param[in]   cnt     - number of blocks to write.
 * @return      Returns 0 on success, otherwise failure.
 */
static int tlkmw_fs_drv_spisd_write(uint8_t *buff, uint32_t lba, uint32_t cnt)
{
    return sd_nand_block_write(&fs_sd_nand, lba, buff, cnt);
}

/**
 * @brief       This function reads data from the SPI SD disk IO.
 * @param[out]  buff    - pointer to buffer to store read data.
 * @param[in]   lba     - logical block address to read from.
 * @param[in]   cnt     - number of blocks to read.
 * @return      Returns 0 on success, otherwise failure.
 */
static int tlkmw_fs_drv_spisd_read(uint8_t *buff, uint32_t lba, uint32_t cnt)
{
    return sd_nand_block_read(&fs_sd_nand, lba, buff, cnt);
}

/**
 * @brief       This function gets the sector size of the SPI SD disk IO.
 * @param[in]   none.
 * @return      Returns the sector size (512 bytes).
 */
static uint32_t tlkmw_fs_drv_spisd_get_sector_size(void)
{
    return 512;
}

/**
 * @brief       This function gets the sector number of the SPI SD disk IO.
 * @param[in]   none.
 * @return      Returns the sector number.
 */
static uint32_t tlkmw_fs_drv_spisd_get_sector_num(void)
{
    return fs_sd_nand.c_size * 1024;
}

/**
 * @brief       This function puts the SPI SD disk IO to sleep.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_spisd_sleep(void)
{
    sd_nand_power_off();
}

/**
 * @brief       This function wakes up the SPI SD disk IO.
 * @param[in]   none.
 * @return      none.
 */
static void tlkmw_fs_drv_spisd_awake(void)
{
    sd_nand_init(&fs_sd_nand);
}

const tlkmw_fs_diskio_t gTlkmwFsDiskIoSpisd = {
    .init          = tlkmw_fs_drv_spisd_init,
    .sleep         = tlkmw_fs_drv_spisd_sleep,
    .awake         = tlkmw_fs_drv_spisd_awake,
    .write         = tlkmw_fs_drv_spisd_write,
    .read          = tlkmw_fs_drv_spisd_read,
    .getSectorSize = tlkmw_fs_drv_spisd_get_sector_size,
    .getSectorNum  = tlkmw_fs_drv_spisd_get_sector_num,
};
#endif
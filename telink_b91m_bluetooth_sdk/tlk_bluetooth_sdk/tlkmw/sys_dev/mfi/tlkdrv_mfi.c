/********************************************************************************************************
 * @file    tlkdrv_mfi.c
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
#include "tlkdrv_mfi.h"
#include "tlkapi/tlkapi.h"

#if (TLK_DEV_MFI_ENABLE)

/**
 * @brief Initialize MFI device
 * @return 0: success, <0: failure
 */
int tlkdev_mfi_init(void)
{
    int ret;

    ret = tlkdrv_mfi_init();

    return ret;
}

/**
 * @brief Open MFI device
 * @return 0: success, <0: failure
 */
int tlkdev_mfi_open(void)
{
    return tlkdrv_mfi_open();
}

/**
 * @brief Close MFI device
 * @return 0: success, <0: failure
 */
int tlkdev_mfi_close(void)
{
    return tlkdrv_mfi_close();
}

/**
 * @brief Load certificate data from MFI device
 * @param[in] pBuffer - buffer to store certificate data
 * @param[in] buffLen - buffer length
 * @return 0: success, <0: failure
 */
//<Accessory Interface Specification R36.pdf> P665
int tlkdev_mfi_loadCertificateData(uint8_t *pBuffer, uint16_t buffLen)
{
    uint16_t index;
    uint8_t  bytes[2];
    uint8_t  protocolVersionMajor;
    uint16_t accessoryCertificateDataLength, numCertificateBytes;

    if (!tlkdrv_mfi_isOpen()) {
        tlkapi_error(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "loadCertificateData: Device is not open!");
        return -TLK_ENOREADY;
    }

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_ErrorCode, bytes, 1);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "ErrorCode=%d", bytes[0]);

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_DeviceVersion, &protocolVersionMajor, 1);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "device_version=%d", protocolVersionMajor);

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_AuthenticationRevision, &protocolVersionMajor, 1);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "authen_version=%d", protocolVersionMajor);

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_AuthenticationProtocolMajorVersion, &protocolVersionMajor, 1);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "major_version=%d", protocolVersionMajor);

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_AccessoryCertificateDataLength, bytes, sizeof(bytes));
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "data len=%x %x", bytes[0], bytes[1]);

    accessoryCertificateDataLength = ((uint16_t)bytes[0] << 0x08U) | bytes[1];
    if ((protocolVersionMajor == 3 && (accessoryCertificateDataLength < 607 || accessoryCertificateDataLength > 609)) ||
        (protocolVersionMajor == 2 && accessoryCertificateDataLength > 1280)) {
        tlkapi_error(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "tlkdev_mfi_loadCertificateData fail");
        return -TLK_EFAIL;
    }

    numCertificateBytes = 0;
    for (index = 0; accessoryCertificateDataLength != 0; index++) {
        uint16_t numBytes = tlkapi_min(accessoryCertificateDataLength, (uint16_t)128);
        if (numCertificateBytes + numBytes > buffLen) {
            tlkapi_fatal(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "loadCertificateData: overflow ! [%d-%d]", numCertificateBytes, numBytes);
            break;
        }
        tlkdrv_mfi_read(kHAPMFiHWAuthRegister_AccessoryCertificateData1 + index, &((uint8_t *)pBuffer)[numCertificateBytes], numBytes);
        accessoryCertificateDataLength -= numBytes;
        numCertificateBytes += numBytes;
    }
    tlkapi_array(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "Certificate data=", pBuffer, numCertificateBytes);
    //    tlkapi_array(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "Certificate data2=", &pBuffer[256],256);
    //    tlkapi_array(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "Certificate data3=", &pBuffer[512], 96);
    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_ErrorCode, bytes, 1);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "ErrorCode=%d", bytes[0]);

    return TLK_ENONE;
}

/**
 * @brief Load challenge data for authentication
 * @param[in] pData - challenge data to load
 * @param[in] dataLen - challenge data length
 * @param[in] pBuff - buffer to store response data
 * @param[in] buffLen - response buffer length
 * @return 0: success, <0: failure
 */
int tlkdev_mfi_loadChallengeData(uint8_t *pData, uint16_t dataLen, uint8_t *pBuff, uint16_t buffLen)
{
    uint16_t nc;
    uint8_t  length;

    if (!tlkdrv_mfi_isOpen()) {
        tlkapi_error(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "loadCertificateData: Device is not open!");
        return -TLK_ENOREADY;
    }

    tlkapi_array(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "tlkdev_mfi_loadChallengeData=", pData, dataLen);
    tlkdrv_mfi_write(kHAPMFiHWAuthRegister_ChallengeData, pData, dataLen);

    nc = 1;
    tlkdrv_mfi_write(kHAPMFiHWAuthRegister_AuthenticationControlAndStatus, (uint8_t *)&nc, 1);

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_AuthenticationControlAndStatus, (uint8_t *)&nc, 1);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "kHAPMFiHWAuthRegister_AuthenticationControlAndStatus=%d", nc);

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_ChallengeResponseDataLength, (uint8_t *)&nc, 2);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "kHAPMFiHWAuthRegister_ChallengeResponseDataLength=%d", nc);

    length = nc >> 8;
    if (length > buffLen) {
        length = buffLen;
    }

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_ChallengeResponseData, pBuff, length);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "length = %d", length);
    tlkapi_array(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "ChallengeResponseData=", pBuff, length);

    nc = 0;
    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_ErrorCode, (uint8_t *)&nc, 1);
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, ">>> ErrorCode=%d", nc);

    return TLK_ENONE;
}

#define TRY_TIMES (1100)
static tlkdrv_mfi_t sTlkdrvMfiCtrl;

/**
 * @brief Initialize MFI driver
 * @return 0: success, <0: failure
 */
int tlkdrv_mfi_init(void)
{
    tmemset(&sTlkdrvMfiCtrl, 0, sizeof(tlkdrv_mfi_t));

    sTlkdrvMfiCtrl.isInit   = true;
    tlkhal_i2c_cfg_t i2cCfg = {
        .sda_pin = TLKDRV_MFI_I2C_SDA_PIN,
        .scl_pin = TLKDRV_MFI_I2C_SCL_PIN,
    };
    tlkhal_i2c_set_pin(&i2cCfg);
    i2c_master_init();
    i2c_set_master_clk((unsigned char)(sys_clk.pclk * 1000 * 1000 / (4 * TLKDRV_MFI_I2C_CLK_SPEED)));

    return TLK_ENONE;
}

/**
 * @brief Check if MFI device is open
 * @return true: open, false: closed
 */
bool tlkdrv_mfi_isOpen(void)
{
    return sTlkdrvMfiCtrl.isOpen;
}

/**
 * @brief Open MFI driver
 * @return 0: success, <0: failure
 */
int tlkdrv_mfi_open(void)
{
    if (!sTlkdrvMfiCtrl.isInit) {
        return -TLK_ENOREADY;
    }
    if (sTlkdrvMfiCtrl.isOpen) {
        return -TLK_EREPEAT;
    }
    sTlkdrvMfiCtrl.isOpen = true;

    uint32_t chipID;
    uint8_t  buffer[4];

    tmemset(buffer, 0, sizeof(uint8_t) * 4);

    tlkdrv_mfi_read(kHAPMFiHWAuthRegister_DeviceID, buffer, 4);
    ARRAY_TO_UINT32H(buffer, 0, chipID);

    if (chipID != TLKDRV_MFI_CHIP_ID) {
        tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "tlkdrv_mfi_open check failure: %d - %d", TLKDRV_MFI_CHIP_ID, chipID);
        return -TLK_EFAIL;
    }
    tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "tlkdrv_mfi_open check ok: %d - %d", TLKDRV_MFI_CHIP_ID, chipID);
    return TLK_ENONE;
}

/**
 * @brief Close MFI driver
 * @return 0: success, <0: failure
 */
int tlkdrv_mfi_close(void)
{
    if (!sTlkdrvMfiCtrl.isOpen) {
        return TLK_ENONE;
    }
    sTlkdrvMfiCtrl.isOpen = false;
    return TLK_ENONE;
}

/**
 * @brief Read data from MFI register
 * @param[in] reg - register address
 * @param[out] pBuff - buffer to store read data
 * @param[in] readLen - number of bytes to read
 * @return number of bytes read
 */
int tlkdrv_mfi_read(uint8_t reg, uint8_t *pBuff, uint8_t readLen)
{
    uint16_t try_times = TRY_TIMES;
    uint8_t  ret;

    i2c_master_send_stop(1); //0 ro 1

    if (!readLen) {
        tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "tlkdrv_mfi_read len error");
        return 0;
    }
    do {
        ret = i2c_master_write(TLKDRV_MFI_I2C_ADDR, (unsigned char *)&reg, 1);
        if (!ret) {
            delay_us(500);
        }
        try_times--;
    } while (try_times && !ret);

    if (try_times == 0) {
        tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "iic wait ack timeout");
    }

    i2c_master_send_stop(1);
    try_times = TRY_TIMES;
    do {
        ret = i2c_master_read(TLKDRV_MFI_I2C_ADDR, pBuff, readLen);
        if (!ret) {
            delay_us(500);
        }
        try_times--;

    } while (try_times && !ret);

    if (try_times == 0) {
        tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "iic wait ack timeout1");
    }
    return readLen;
}

/**
 * @brief Write data to MFI register
 * @param[in] reg - register address
 * @param[in] pData - data to write
 * @param[in] dataLen - number of bytes to write
 * @return number of bytes written
 */
int tlkdrv_mfi_write(uint8_t reg, uint8_t *pData, uint8_t dataLen)
{
    uint16_t try_times = TRY_TIMES;
    uint8_t  ret;


    if (!sTlkdrvMfiCtrl.isOpen) {
        return -TLK_ENOREADY;
    }

    if (dataLen == 0) {
        tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "TLK_ENOREADY len error");
        return -TLK_ENOREADY;
    }
    uint8_t buf[1024];
    buf[0] = reg;
    tmemcpy(&buf[1], pData, dataLen);

    try_times = TRY_TIMES;

    i2c_master_send_stop(1);

    do {
        ret = i2c_master_write(TLKDRV_MFI_I2C_ADDR, buf, dataLen + 1);
        if (!ret) {
            delay_us(500);
        }
        try_times--;

    } while (try_times && !ret);

    if (try_times == 0) {
        tlkapi_trace(TLKDRV_MFI_DBG_FLAG, TLKDRV_MFI_DBG_SIGN, "iic wait ack timeout4");
    }

    return dataLen;
}

#endif //TLK_DEV_MFI_ENABLE

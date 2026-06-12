/********************************************************************************************************
 * @file    svc_format.c
 *
 * @brief   This is the source file for TLSR/TL
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
#include "common/types.h"

#include "../gatt/inc/gatt.h"

#include "inc/svc_format.h"

const uint16_t gattIncludeValueLen = 6;

const uint16_t characteristicPropertiesLen = CHARACTERISTIC_PROPERTIES_LENGTH;

const uint8_t charPropRead                        = CHAR_PROP_READ;
const uint8_t charPropReadWrite                   = CHAR_PROP_READ | CHAR_PROP_WRITE;
const uint8_t charPropReadWriteWithout            = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP;
const uint8_t charPropReadWriteWriteWithout       = CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP;
const uint8_t charPropReadWriteWriteWithoutNotify = CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY;
const uint8_t charPropReadWriteNotify             = CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_NOTIFY;
const uint8_t charPropReadWriteWithoutNotify      = CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY;
const uint8_t charPropReadNotify                  = CHAR_PROP_READ | CHAR_PROP_NOTIFY;


const uint8_t charPropWrite                   = CHAR_PROP_WRITE;
const uint8_t charPropWriteWithout            = CHAR_PROP_WRITE_WITHOUT_RSP;
const uint8_t charPropWriteWriteWithout       = CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP;
const uint8_t charPropWriteIndicate           = CHAR_PROP_WRITE | CHAR_PROP_INDICATE;
const uint8_t charPropWriteNotifyIndicate     = CHAR_PROP_WRITE | CHAR_PROP_NOTIFY | CHAR_PROP_INDICATE;
const uint8_t charPropWriteWriteWithoutNotify = CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP | CHAR_PROP_NOTIFY;

const uint8_t charPropNotify   = CHAR_PROP_NOTIFY;
const uint8_t charPropIndicate = CHAR_PROP_INDICATE;

const uint8_t  clientCharacteristicConfiguration[2] = {0x00, 0x00};
const uint16_t clientCharacteristicConfigurationLen = sizeof(clientCharacteristicConfiguration);

uint8_t  clientCharacteristicConfigurationWrite[2] = {0x00, 0x00};
uint16_t clientCharacteristicConfigurationWriteLen = sizeof(clientCharacteristicConfigurationWrite);

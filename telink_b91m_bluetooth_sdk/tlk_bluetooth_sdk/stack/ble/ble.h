/********************************************************************************************************
 * @file    ble.h
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
#ifndef BLE_H_
#define BLE_H_

#include "stack/stack.h"

//#include "vendor/common/ble_common/ble_common.h"
#include "ble_common.h"
#include "ble_format.h"

#include "controller/ble_controller.h"
#include "host_v1/inc/host.h"
#include "host_v1/inc/profile.h"

#include "hci/ble_hci.h"
#include "hci/hci_const.h"
#include "hci/hci_cmd.h"
#include "hci/hci_event.h"


#include "service/device_information.h"
#include "service/hids.h"
#include "service/uuid.h"

#include "device/multi_device.h"

#include "stack/ble/debug/ble_debug.h"



#endif /* BLE_H_ */

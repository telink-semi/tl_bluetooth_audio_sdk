/********************************************************************************************************
 * @file    tpsll_int.h
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
#ifndef DRIVERS_TL721X_EXT_DRIVER_EXT_INT_H_
#define DRIVERS_TL721X_EXT_DRIVER_EXT_INT_H_
#include "common/types.h"
#include "drivers.h"


#define GLOBAL_INT_DISABLE() u32 rie = core_interrupt_disable()

#define GLOBAL_INT_RESTORE() core_restore_interrupt(rie)

#endif /* DRIVERS_TL721X_EXT_DRIVER_EXT_INT_H_ */

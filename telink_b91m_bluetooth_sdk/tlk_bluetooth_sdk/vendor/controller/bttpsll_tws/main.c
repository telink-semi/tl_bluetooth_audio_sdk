/********************************************************************************************************
 * @file    main.c
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
#include "tlkapi/tlkapi.h"

#if (CONTROLLER_MODE == BTTPSLL_TWS)

/**
 * @brief       Initialize user application and hardware modules
 * @param[in]   none
 * @return      none
 */
extern void user_init(void);

/**
 * @brief       Main loop function
 *              This function contains the main processing loop for the application,
 *              which includes SDK main loop processing, multi-core communication
 *              handling, and power management processing.
 * @param[in]   none
 * @return      none
 */
extern void main_loop(void);

/**
 * @brief       This is main function
 * @param[in]	none
 * @return      none
 */
int main(void)
{
    user_init();

    while (1) {
        main_loop();
    }

    return 0;
}
#endif

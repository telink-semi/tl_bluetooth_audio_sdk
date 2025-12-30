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

#if (CONTROLLER_MODE == BTBLE_TPSLL_HEADSET)
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
 * @brief       Main function - entry point of the program
 * @param[in]	void - no input parameters
 * @return      int - returns 0 on successful execution (though this is never reached due to infinite loop)
 * @note        The function initializes the system and enters an infinite loop calling main_loop()
 *              Actual program logic runs in main_loop() which is called continuously
 */
int main(void)
{
    user_init();

    // gpio_function_en(GPIO_PF4);
    // gpio_input_dis(GPIO_PF4);
    // gpio_output_en(GPIO_PF4);

    // gpio_function_en(GPIO_PF5);
    // gpio_input_dis(GPIO_PF5);
    // gpio_output_en(GPIO_PF5);    // ZEWEN: temp debug for 3mode switch

    while (1) {
        main_loop();
    }

    return 0;
}
#endif

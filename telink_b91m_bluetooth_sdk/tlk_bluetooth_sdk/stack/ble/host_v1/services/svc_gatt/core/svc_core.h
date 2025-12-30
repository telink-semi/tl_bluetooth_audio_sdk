/********************************************************************************************************
 * @file    svc_core.h
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


/**
 * @brief      for user add default GATT and GAP service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_addCoreGroup(void);

/**
 * @brief      for user remove default GATT and GAP service in all GAP server.
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_removeCoreGroup(void);

/**
 * @brief      for user calculate database hash value(core version >= 5.1).
 * @param[in]  none.
 * @return     none.
 */
void blc_svc_calculateDatabaseHash(void);

/**
 * @brief     for user set device name.
 * @param[in] name: user device name.
 * @return    none.
 */
void blc_svc_setDeviceName(const char *name);

/**
 * @brie      for user set appearance.
 * @param[in] appearance: user appearance.
 * @return    none.
 */
void blc_svc_setAppearance(uint16_t appearance);

/**
 * @brief     for user set peripheral preferred parameters.
 * @param[in] minInterval: user minInterval, unit is 1.25ms
 * @param[in] maxInterval: user maxInterval, unit is 1.25ms
 * @param[in] latency: user latency, unit is connection events.
 * @param[in] timeout: user timeout, unit is 10ms.
 * @return    none.
 */
void blc_svc_setPeripheralPreferredParameters(uint16_t minInterval, uint16_t maxInterval, uint16_t latency, uint16_t timeout);

/********************************************************************************************************
 * @file    tlk_ota_protocol_common.c
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
#include "drivers.h"
#include "tlkapi/tlkapi.h"

#include "tlk_ota_protocol_common.h"
#include "general_protocol/tlk_ota_general_protocol.h"
#include "ble_previous_protocol/tlk_ota_ble_previous_protocol.h"

#if (TLK_MW_USER_CTRL_ENABLE)

int TLK_MW_USER_CTRL_ENABLE_VALUE = 1;

/*Here is the user area saving format. Applicable only to the current SDK.*/
static tlkapi_save_ctrl_t sTlk_boot_ota_cfg_ctrl;
sTlk_boot_and_ota_cfg_t   sTlk_boot_ota_cfg;

/**
 * @brief      Load user configuration from NVDS OTA
 * @param[in]  pBuffer - pointer to buffer
 * @param[in]  buffLen - buffer length
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_nvds_ota_usercfg_load(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    (void)UserArg;
    if (pBuffer == NULL || buffLen < sizeof(sTlk_boot_and_ota_cfg_t)) {
        return -OTA_LOADERR;
    }
    /*If there is no need for dynamic calculation of the backup address, there is no need to pay attention to it.*/
    /*Load Boot and OTA Configuration Zone, save User Info Zone Addr.*/
    tlkapi_save3_init(&sTlk_boot_ota_cfg_ctrl, TLKMW_OTA_COMMON_SAVE_SIGN, TLKMW_OTA_COMMON_SAVE_VER, sizeof(sTlk_boot_and_ota_cfg_t), TLKMW_TINYSQL_OTA_INFO_ADDR0,
                      TLKMW_TINYSQL_OTA_INFO_ADDR1);

    int ret = tlkapi_save3_load(&sTlk_boot_ota_cfg_ctrl, pBuffer, buffLen);
    if (ret < (int)buffLen) {
        memset(pBuffer, 0XFFFFFFFF, buffLen);
    }
    if (tlkapi_save3_migrate(&sTlk_boot_ota_cfg_ctrl, pBuffer, buffLen) == TLK_ENONE) {
        return OTA_NONE;
    } else {
        return -OTA_LOADERR;
    }
}

/**
 * @brief      Save user configuration to NVDS OTA
 * @param[in]  pBuffer - pointer to buffer
 * @param[in]  buffLen - buffer length
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_nvds_ota_usercfg_save(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    (void)UserArg;
    if (pBuffer == NULL || buffLen < sizeof(sTlk_boot_and_ota_cfg_t)) {
        return -OTA_SAVEERR;
    }

    if (tlkapi_save3_smartSave(&sTlk_boot_ota_cfg_ctrl, pBuffer, buffLen) == TLK_ENONE) {
        return OTA_NONE;
    } else {
        return -OTA_SAVEERR;
    }
}

static nvds_ota_Interface_t sTlk_ota_interface;
static sTlkMwOtaCommon_t    sTlkMwCommonCtrl;

/*Redefine ota_interface_hal functions.*/
/**
 * @brief      Allocate memory for OTA interface
 * @param[in]  dataLen - length of data to allocate
 * @param[out] none
 * @return     void* - pointer to allocated memory
 */
void *nvds_ota_malloc(uint32_t dataLen)
{
    return tlkos_malloc(dataLen);
}

/**
 * @brief      Free memory for OTA interface
 * @param[in]  pBuffer - pointer to buffer to free
 * @param[out] none
 * @return     none
 */
void nvds_ota_free(void *pBuffer)
{
    tlkos_free(pBuffer);
}

/**
 * @brief      Save OTA status to NVDS
 * @param[in]  pBuffer - pointer to buffer
 * @param[in]  buffLen - buffer length
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - result of save operation
 */
int tlk_nvds_ota_status_save(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    return tlk_nvds_ota_usercfg_save(pBuffer, buffLen, UserArg);
}

/**
 * @brief      Save backup address to NVDS OTA
 * @param[in]  pBuffer - pointer to buffer
 * @param[in]  buffLen - buffer length
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - result of save operation
 */
int tlk_nvds_ota_backup_addr_save(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    return tlk_nvds_ota_usercfg_save(pBuffer, buffLen, UserArg);
}

/**
 * @brief      Save user area address to NVDS OTA
 * @param[in]  pBuffer - pointer to buffer
 * @param[in]  buffLen - buffer length
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - result of save operation
 */
int tlk_nvds_ota_userarea_addr_save(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    return tlk_nvds_ota_usercfg_save(pBuffer, buffLen, UserArg);
}

/**
 * @brief      Initialize OTA common functionality
 * @param[in]  none
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlkmw_ota_common_init(void)
{
    OTA_MEMSET(&sTlkMwCommonCtrl, 0, sizeof(sTlkMwOtaCommon_t));
    for (uint8_t i = 0; i < TLKMW_OTA_NOTIFY_ARRAY_NUM; i++) {
        sTlkMwCommonCtrl.notifyCB[i].threadID = 0xFFFF;
    }

    tlk_nvds_ota_interface_init(&sTlk_ota_interface);
    /*If there is no need for dynamic calculation of the backup address, there is no need to pay attention to it.*/
    if (sTlk_ota_interface.nvds_ota_user_load != NULL && sTlk_ota_interface.nvds_ota_user_load((uint8_t *)&sTlk_boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL) == OTA_NONE) {
        unsigned int address             = tlk_nvds_get_full_size() + TLK_CFG_FLASH_PBAP_LIST_ADDR - 0x100000; //The corresponding offset for the last 1M.
        sTlk_boot_ota_cfg.user_area_addr = address;

        tlk_nvds_ota_userarea_addr_save((uint8_t *)&sTlk_boot_ota_cfg, sizeof(sTlk_boot_and_ota_cfg_t), NULL);
    }

    if (tlk_ota_general_protocol_init(&sTlk_ota_interface) != OTA_NONE) {
        return -OTA_INITERR;
    }
    if (tlk_ota_ble_previous_protocol_init(&sTlk_ota_interface) != OTA_NONE) {
        return -OTA_INITERR;
    }

    return OTA_NONE;
}

/**
 * @brief      Register channel send interface for OTA
 * @param[in]  channel - communication channel
 * @param[in]  send    - send function pointer
 * @param[out] none
 * @return     none
 */
void tlkmw_ota_register_chn_send_interface(uint8_t channel, int (*send)(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg))
{
    if (channel >= TLKMW_OTA_TRANS_CHN_MAX || send == NULL) {
        return;
    }
    sTlkMwCommonCtrl.intf[channel].send = send;
}

/**
 * @brief      Register channel receive interface for OTA
 * @param[in]  channel - communication channel
 * @param[in]  recv    - receive function pointer
 * @param[out] none
 * @return     none
 */
void tlkmw_ota_register_chn_recv_interface(uint8_t channel, int (*recv)(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg))
{
    if (channel >= TLKMW_OTA_TRANS_CHN_MAX || recv == NULL) {
        return;
    }
    sTlkMwCommonCtrl.intf[channel].recv = recv;
}

/**
 * @brief      Register OTA notification callback
 * @param[in]  threadID - thread identifier
 * @param[in]  notify   - notification function pointer
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlkmw_ota_register_notify_callback(uint16_t threadID, void (*notify)(void *pData, uint8_t dataLen))
{
    uint8_t index = 0;

    for (index = 0; index < TLKMW_OTA_NOTIFY_ARRAY_NUM; index++) {
        if (sTlkMwCommonCtrl.notifyCB[index].threadID == 0xFFFF) {
            sTlkMwCommonCtrl.notifyCB[index].threadID = threadID;
            sTlkMwCommonCtrl.notifyCB[index].notify   = notify;
            return OTA_NONE;
        }
    }
    return -OTA_MEMORYERR;
}

/**
 * @brief      Notify OTA status
 * @param[in]  pBuffer - pointer to buffer
 * @param[in]  buffLen - buffer length
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlk_ota_notify_status(uint8_t *pBuffer, uint32_t buffLen, void *UserArg)
{
    (void)UserArg;
    sTlkMwNotifyEvent_t *notify_event = (sTlkMwNotifyEvent_t *)pBuffer;
    notify_event->taskID              = sTlkMwCommonCtrl.taskID;

    for (uint8_t i = 0; i < TLKMW_OTA_NOTIFY_ARRAY_NUM; i++) {
        if (sTlkMwCommonCtrl.notifyCB[i].threadID != 0xFFFF) {
            tlksys_runFuncInTaskWithArg(sTlkMwCommonCtrl.notifyCB[i].threadID, sTlkMwCommonCtrl.notifyCB[i].notify, pBuffer, buffLen);
        }
    }
    return OTA_NONE;
}

/**
 * @brief      Get busy task ID for OTA
 * @param[in]  none
 * @param[out] none
 * @return     uint32_t - busy task ID
 */
uint32_t tlkmw_ota_common_get_busy_taskid(void)
{
    return sTlkMwCommonCtrl.taskID;
}

/**
 * @brief      Send data via OTA common interface
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  UserArg - user argument
 * @param[out] none
 * @return     none
 */
void tlkmw_ota_common_send_data(uint8_t *pData, uint16_t dataLen, void *UserArg)
{
    if (sTlkMwCommonCtrl.intf[sTlkMwCommonCtrl.optChn].send == NULL) {
        return;
    }
    sTlkMwCommonCtrl.intf[sTlkMwCommonCtrl.optChn].send(sTlkMwCommonCtrl.taskID, pData, dataLen, UserArg);
}

/**
 * @brief      Receive data via OTA common interface
 * @param[in]  taskID  - task identifier
 * @param[in]  channel - communication channel
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[out] none
 * @return     none
 */
void tlkmw_ota_common_recv_data(uint32_t taskID, uint8_t channel, uint8_t *pData, uint16_t dataLen)
{
    if (pData == NULL || dataLen == 0) {
        return;
    }

    /*Only supporting multiple OTA channels, should attention taskID and channel.*/
    if (sTlkMwCommonCtrl.taskID == 0) {
        sTlkMwCommonCtrl.taskID = taskID;
    }

    if (taskID != sTlkMwCommonCtrl.taskID) {
        return;
    }

    if (sTlkMwCommonCtrl.optChn == TLKMW_OTA_TRANS_CHN_NONE) {
        sTlkMwCommonCtrl.optChn = channel;
    }
    if (channel != sTlkMwCommonCtrl.optChn) {
        return;
    }

    if (sTlkMwCommonCtrl.intf[sTlkMwCommonCtrl.optChn].recv == NULL) {
        return;
    }

    sTlkMwCommonCtrl.intf[sTlkMwCommonCtrl.optChn].recv(sTlkMwCommonCtrl.taskID, pData, dataLen, NULL);
}

/**
 * @brief      Reset OTA common functionality
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void tlkmw_ota_common_reset(void)
{
    uint16_t remain_size = sizeof(sTlkMwUnitIntf_t) * TLKMW_OTA_TRANS_CHN_MAX + sizeof(sTlkMwNotifyUnit_t) * TLKMW_OTA_NOTIFY_ARRAY_NUM;
    OTA_MEMSET(&sTlkMwCommonCtrl, 0, sizeof(sTlkMwOtaCommon_t) - remain_size);
}

#else
/**
 * @brief      Register channel send interface for OTA
 * @param[in]  channel - communication channel
 * @param[in]  send    - send function pointer
 * @param[out] none
 * @return     none
 */
void tlkmw_ota_register_chn_send_interface(uint8_t channel, int (*send)(uint32_t taskID, uint8_t *pData, uint16_t dataLen, void *UserArg))
{
    (void)channel;
    (void)send;
}

/**
 * @brief      Register OTA notification callback
 * @param[in]  threadID - thread identifier
 * @param[in]  notify   - notification function pointer
 * @param[out] none
 * @return     int - OTA_NONE if success, error code otherwise
 */
int tlkmw_ota_register_notify_callback(uint16_t threadID, void (*notify)(void *pData, uint8_t dataLen))
{
    (void)threadID;
    (void)notify;
    return -OTA_MEMORYERR;
}

#endif //TLK_MW_USER_CTRL_ENABLE


/**
 * @brief      Get full flash size
 * @param[in]  none
 * @param[out] none
 * @return     int - flash size in bytes
 */
int tlk_nvds_get_full_size(void)
{
    /*
		Flash Type	uid CMD		MID		    Company
		P25Q80U		0x4b		0x146085	PUYA
		P25Q16SU    0x4b        0x156085    PUYA
		P25Q32SU    0x4b        0x166085    PUYA
		P25Q128L    0x4b        0x186085    PUYA
		P25Q64SL    0x4b        0x176085    PUYA
	    GD25LQ16E   0x4b        0x1560c8    GD
	 */

    unsigned int  flash_mid = 0;
    unsigned char flash_capacity;

#if (MCU_CORE_TYPE == MCU_CORE_TL751X || MCU_CORE_TYPE == MCU_CORE_TL721X || MCU_CORE_TYPE == MCU_CORE_TL753X || MCU_CORE_TYPE == MCU_CORE_TL322X)
    flash_mid = flash_read_mid_with_device_num(SLAVE0);
#else
    flash_mid = flash_read_mid();
#endif

    flash_capacity = ((flash_mid & 0x00ff0000) >> 16);

    if (flash_capacity == FLASH_SIZE_1M) {
        return 0x100000;
    } else if (flash_capacity == FLASH_SIZE_2M) {
        return 0x200000;
    } else if (flash_capacity == FLASH_SIZE_4M) {
        return 0x400000;
    } else if (flash_capacity == FLASH_SIZE_8M) {
        return 0x800000;
    } else if (flash_capacity == FLASH_SIZE_16M) {
        return 0x1000000;
    } else {
        return 0x200000;
    }
}

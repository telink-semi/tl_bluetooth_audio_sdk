/********************************************************************************************************
 * @file    ble_host_sal.h
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


// #ifdef __cplusplus
// extern "C" {
// #endif

/*************** list, queue, stack, or other data structure ***********/
// ble all data structure use sys/queue.h defined in kernel.
// #include <sys/queue.h>

/*************** timer sal interface ***********/

/**
 *   @brief BLE host timer handle type_id.
 */
typedef void *ble_host_sal_timer_handle_t;

/**
 *   @brief BLE host timer callback function type_id.
 *
 *   @param[in]  arg         timer callback function argument.
 *
 *   @return none.
 */
typedef void (*ble_host_sal_timer_callback)(ble_host_sal_timer_handle_t handle, void *arg);

/**
 *   @brief BLE host timer initialization.
 *
 *   @return none.
 */
void ble_host_sal_timer_init(void);

/**
 *   @brief BLE host timer create a timer.
 *
 *   @param[in]  cb          timer callback function.
 *   @param[in]  arg         timer callback function argument.
 *   @param[in]  timeout_ms  timer timeout in millisecond.
 *   @param[out] timer_hdl   timer handle.
 *
 *   @return true if success, false if failed.
 */
bool ble_host_sal_timer_create(ble_host_sal_timer_callback cb, void *arg, uint32_t timeout_ms, ble_host_sal_timer_handle_t *timer_hdl);

/**
 *   @brief ble host timer update timeout value.
 *
 *   @param[in]  timer_hdl   timer handle.
 *   @param[in]  timeout_ms  timer timeout in millisecond.
 *
 *   @return true if success, false if failed.
 */
bool ble_host_sal_timer_update_timeout(ble_host_sal_timer_handle_t timer_hdl, uint32_t timeout_ms);

/**
 *   @brief ble host timer start a timer.
 *
 *   @param[in]  timer_hdl   timer handle.
 *
 *   @return true if success, false if failed.
 */
bool ble_host_sal_timer_start(ble_host_sal_timer_handle_t timer_hdl);

/**
 *   @brief ble host timer stop a timer.
 *
 *   @param[in]  timer_hdl   timer handle.
 *
 *   @return true if success, false if failed.
 */
bool ble_host_sal_timer_stop(ble_host_sal_timer_handle_t timer_hdl);

/**
 *   @brief ble host timer delete a timer.
 *
 *   @param[in]  timer_hdl   timer handle.
 *
 *   @return true if success, false if failed.
 */
bool ble_host_sal_timer_delete(ble_host_sal_timer_handle_t timer_hdl);

/**************** memory pool(malloc/free) sal interface ***********/
/**
 *   @brief BLE host memory pool initialization.
 *
 *   @return none.
 */
void ble_host_sal_memory_pool_init(void);

/**
 *   @brief BLE host memory malloc.
 *
 *   @param[in]  size        malloc size.
 *   @param[in]  type_id     high layer type id(range 256-65535).
 *
 *   @return malloc pointer.
 */
void *ble_host_sal_memory_malloc(uint32_t size, uint16_t type_id);

/**
 *   @brief BLE host memory free.
 *
 *   @param[in]  ptr          malloc pointer.
 *
 *   @return none.
 */
void ble_host_sal_memory_free(void *ptr);

/****************** log sal interface ****************/
enum ble_host_log_level
{
    BLE_HOST_LOG_LEVEL_NONE = 0,
    BLE_HOST_LOG_LEVEL_ERROR,
    BLE_HOST_LOG_LEVEL_WARN,
    BLE_HOST_LOG_LEVEL_INFO,
    BLE_HOST_LOG_LEVEL_DEBUG,
};

/**
 *   @brief BLE host log initialization.
 *
 *   @return none.
 */
void ble_host_sal_log_init(enum ble_host_log_level level);

/**
 *   @brief BLE host log output.
 *
 *   @param[in]  level   log level.
 *   @param[in]  fmt     log format string.
 *
 *   @return none.
 */
void ble_host_sal_log_output(enum ble_host_log_level level, const char *fmt, ...);

/**
 *   @brief BLE host log set output level.
 *
 *   @param[in]  level   log level.
 *
 *   @return none.
 */
void ble_host_sal_log_set_output_level(enum ble_host_log_level level);

#define BLE_HOST_SAL_LOG_ERROR(fmt, ...) ble_host_sal_log_output(BLE_HOST_LOG_LEVEL_ERROR, "[E]" fmt, ##__VA_ARGS__)
#define BLE_HOST_SAL_LOG_WARN(fmt, ...)  ble_host_sal_log_output(BLE_HOST_LOG_LEVEL_WARN, "[W]" fmt, ##__VA_ARGS__)
#define BLE_HOST_SAL_LOG_INFO(fmt, ...)  ble_host_sal_log_output(BLE_HOST_LOG_LEVEL_INFO, "[I]" fmt, ##__VA_ARGS__)
#define BLE_HOST_SAL_LOG_DEBUG(fmt, ...) ble_host_sal_log_output(BLE_HOST_LOG_LEVEL_DEBUG, "[D]" fmt, ##__VA_ARGS__)

/****************** hci sal interface ****************/
/**
 *   @brief BLE host hci initialization.
 *
 *   @return none.
 */
void ble_host_sal_hci_init(void);

/**
 *   @brief BLE host hci send value to controller.
 *
 *   @param[in]  data    hci data pointer.
 *   @param[in]  len     hci data length.
 *
 *   @return none.
 */
void ble_host_sal_hci_send_packet(const uint8_t *data, uint16_t len);

/**
 *   @brief BLE host hci receive value from controller ack.
 *
 *   @param[in]  timeout_ms  timeout in millisecond.
 *
 *   @note BLE Host send HCI command to controller, and wait for response.
 */
void ble_host_sal_hci_wait_ack(uint16_t timeout_ms);

/**
 *   @brief BLE host hci receive value from controller complete.
 *
 *   @note BLE Host send HCI command to controller, and wait for response.
 */
void ble_host_sal_hci_receive_complete(void);

/**
 *   @brief BLE host hci receive value from controller.
 *
 *   @note It called by Application Layer Controller Driver.
 */
// void ble_host_hci_rx_packet(uint8_t *data, uint32_t len);

/**
 *   @brief BLE host hci receive value from controller.
 *
 *   @note It called by Application Layer Controller Driver, only receive complete packet.
 */
// void ble_host_hci_rx_complete_packet(uint8_t *data, uint32_t len);

/********************* assert interface ****************/

#define BLE_HOST_SAL_ASSERT(expr)                                                          \
    do {                                                                                   \
        if (!(expr)) {                                                                     \
            BLE_HOST_SAL_LOG_ERROR("assert failed: %s:%d: %s", __func__, __LINE__, #expr); \
            while (1) {                                                                    \
                ;                                                                          \
            }                                                                              \
        }                                                                                  \
    } while (0)

/********************* RTOS or bare metal register interface ****************/
/**
 *   @brief BLE host task entry function.
 *
 *   @return none.
 */
typedef void (*ble_host_task)(void);

/**
 *   @brief BLE host os/bare metal initialization.
 *
 *   @return none.
 */
void ble_host_sal_os_init(void);

/**
 *   @brief BLE host os/bare metal register task.
 *
 *   @param[in]  stack_size  task stack size(not used now).
 *   @param[in]  task_entry  task entry function, bare metal enters in loop.
 *
 *   @return none.
 */
void ble_host_sal_os_register(uint32_t stack_size, void (*task_entry)(void));

/**
 *   @brief BLE host os/bare metal get ble host task.
 *
 *   @return ble host task pointer.
 */
ble_host_task ble_host_sal_get_task(void);

/**
 *   @brief BLE host os/bare metal get ble host task stack size (Not used now).
 *
 *   @return ble host task stack size.
 */
uint32_t ble_host_sal_get_task_stack_size(void);

/**
 *   @brief BLE host os/bare metal enter critical section.
 *
 *   @return none.
 */
void ble_host_sal_os_enter_critical_section(void);

/**
 *   @brief BLE host os/bare metal exit critical section.
 *
 *   @return none.
 */
void ble_host_sal_os_exit_critical_section(void);

/**
 *   @brief Wakeup ble host task.
 *
 *   @return none.
 *
 *   @note It only used in os.
 */
void ble_host_sal_os_wakeup(void);

/**
 *   @brief BLE host task enter to sleep.
 *
 *   @return none.
 *
 *   @note It only used in os, platform need add critical section.
 */
void ble_host_sal_os_sleep(void);

/********************* NVS sal interface ****************/
/**
 *   @brief BLE host NVS handle type_id.
 */
typedef void *ble_host_sal_nvs_handle_t;

enum ble_host_sal_nvs_result
{
    BLE_HOST_SAL_NVS_RESULT_OK = 0,
    BLE_HOST_SAL_NVS_RESULT_VERSION_NOT_MATCH,
    BLE_HOST_SAL_NVS_RESULT_PARTITION_NOT_EXIST,
    BLE_HOST_SAL_NVS_RESULT_INVALID_HANDLE,
    BLE_HOST_SAL_NVS_RESULT_NOT_SUPPORT,
    BLE_HOST_SAL_NVS_RESULT_KEY_NOT_EXIST,
    BLE_HOST_SAL_NVS_RESULT_INVALID_LENGTH,
};

/**
 *   @brief BLE host NVS initialization.
 *
 *   @param[in]  partition_name  partition name.
 *   @param[in]  version         partition version.
 *
 *   @return 0 if success, -1 means version is not match.
 *
 *   @note If host enable SMP bounding function, partition name should be "tlk_smp".
 *         If profile support nvs, partition name should be "tlk_prf".
 */
int ble_host_sal_nvs_init(const char *partition_name, uint32_t version);

/**
 *   @brief BLE host NVS erase partition data.
 *
 *   @param[in]  partition_name  partition name.
 *
 *   @return 0 if success.
 */
int ble_host_sal_nvs_erase(const char *partition_name);

/**
 *   @brief BLE host NVS open partition space.
 *
 *   @param[in]  partition_name  partition name.
 *   @param[out] handle          nvs operate handle.
 *
 *   @return 0 if success, -2 means partition is not exist.
 */
int ble_host_sal_nvs_open(const char *partition_name, ble_host_sal_nvs_handle_t *handle);

/**
 *   @brief BLE host NVS close partition space.
 *
 *   @param[in]  handle  nvs operate handle.
 *
 *   @return 0 if success, -3 means handle is invalid.
 *
 *   @note If current handle had not commit value, it will commit value and close handle.
 */
int ble_host_sal_nvs_close(ble_host_sal_nvs_handle_t handle);

/**
 *   @brief BLE host NVS commit current all cache value.
 *
 *   @param[in]  handle  nvs operate handle.
 *
 *   @return 0 if success, -3 means handle is invalid.
 *
 *   @note If current handle had not commit value, it will commit value and close handle.
 */
int ble_host_sal_nvs_commit(ble_host_sal_nvs_handle_t handle);

/**
 *   @brief BLE host NVS erase key value.
 *
 *   @param[in]  handle  nvs operate handle.
 *
 *   @return 0 if success, -3 means handle is invalid.
 */
int ble_host_sal_nvs_erase_key(ble_host_sal_nvs_handle_t handle, const char *key);

/**
 *   @brief BLE host NVS write uint32 value.
 *
 *   @param[in]  handle  nvs operate handle.
 *   @param[in]  key     value key.
 *   @param[in]  value   value.
 *
 *   @return 0 if success, -3 means handle is invalid.
 */
int ble_host_sal_nvs_write_uint32(ble_host_sal_nvs_handle_t handle, const char *key, uint32_t value);

/**
 *   @brief BLE host NVS read uint32 value.
 *
 *   @param[in]  handle  nvs operate handle.
 *   @param[in]  key     value key.
 *   @param[out] value   value pointer.
 *
 *   @return 0 if success, -3 means handle is invalid.
 */
int ble_host_sal_nvs_read_uint32(ble_host_sal_nvs_handle_t handle, const char *key, uint32_t *value);

/**
 *   @brief BLE host NVS write variable length value.
 *
 *   @param[in]  handle  nvs operate handle.
 *   @param[in]  key     value key.
 *   @param[in]  value   value pointer.
 *   @param[in]  length  value length.
 *
 *   @return 0 if success, -3 means handle is invalid.
 */
int ble_host_sal_nvs_write_blob(ble_host_sal_nvs_handle_t handle, const char *key, const void *value, uint16_t length);

/**
 *   @brief BLE host NVS read variable length value.
 *
 *   @param[in]  handle  nvs operate handle.
 *   @param[in]  key     value key.
 *   @param[out] value   value pointer.
 *   @param[out] length  input length is value length, output length is actual value length, if read failed, output length is 0.
 *
 *   @return 0 if success, -3 means handle is invalid.
 *
 *   @note If value is null, it will return length of value.
 */
int ble_host_sal_nvs_read_blob(ble_host_sal_nvs_handle_t handle, const char *key, void *value, uint16_t *length);

// #ifdef __cplusplus
// }
// #endif

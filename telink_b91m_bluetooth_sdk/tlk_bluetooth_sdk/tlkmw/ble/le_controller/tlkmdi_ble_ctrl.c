/********************************************************************************************************
 * @file    tlkmdi_ble_ctrl.c
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

#include "stack/btble.h"
#include "tlkapi/tlkapi.h"
#include "tlklib/usb/tlkusb_stdio.h"
#include "tlklib/usb/udb/tlkusb_udb.h"
#include "tlkmw/tlkmw.h"

#include "stack/multiCoreComm/service/service_shareMemory.h"

#include "vendor/common/ble_common/tlkble_hal_mode.h"
#include "tlkmdi_config_ctrl.h"
#include "tlkmdi_buffer_ctrl.h"
#include "tlkmdi_ble_ctrl.h"

#include "stack/ble/host_v1/hci/inc/ble_hci_log.h"
#include "stack/bt/common/rwip.h"

#if (TLK_CFG_RTOS_ENABLE && !MCU_DUAL_CORE_ENABLE)

#define LE_OS_LOG_EN 0

_attribute_data_retention_ TlkOsTaskHandle_t   btTaskHandle             = NULL;
_attribute_data_retention_ TlkOsSemphrHandle_t btSemphrHandle           = NULL;
_attribute_data_retention_ TlkOsMutexHandle_t  btSendDataMutex          = NULL;
_attribute_data_retention_ TlkOsSemphrHandle_t hciCmdSyncSemaphore      = NULL;
static uint32_t                                bleThreadLoopOnceTimerMs = TLKOS_WAIT_FOREVER;

void tlkmdi_ble_set_thread_loop_once_period(uint32_t ms)
{
    bleThreadLoopOnceTimerMs = ms;
    if (btTaskHandle == NULL) {
        return;
    }
    tlkos_semphr_give(btSemphrHandle);
}

/**
 * @brief        This function is used to send a semaphore and can be called inside an interrupt.
 * @param[in]    none.
 * @return        none.
 */
_attribute_ram_code_ static void app_os_give_sem_from_isr(void)
{
    if (btTaskHandle == NULL) {
        return;
    }
    if (tlkos_semphr_giveFromISR(btSemphrHandle) != TLK_ENONE) {
        tlkapi_printf(LE_OS_LOG_EN, "BLE CTRL Semaphore give in IRQ FALSE\r\n");
    }
}

/**
 * @brief        This function is to send a semaphore
 * @param[in]    none.
 * @return        none.
 */
_attribute_ram_code_ static void app_os_give_sem(void)
{
    if (btTaskHandle == NULL) {
        return;
    }
    if (tlkos_semphr_give(btSemphrHandle) != TLK_ENONE) {
        tlkapi_printf(LE_OS_LOG_EN, "BLE CTRL Semaphore give FALSE\r\n");
    }
}

/**
 * @brief        Acquires a mutex semaphore.
 * @param[in]    none
 * @return      none
 */
_attribute_ram_code_ static void app_os_take_mutex(void)
{
    if (tlkos_mutex_lock(btSendDataMutex) != TLK_ENONE) {
        tlkapi_printf(LE_OS_LOG_EN, "Mutex Lock FALSE\r\n");
    }
}

/**
 * @brief        Releases a mutex semaphore.
 * @param[in]    none
 * @return      none
 */
_attribute_ram_code_ static void app_os_give_mutex(void)
{
    if (tlkos_mutex_unlock(btSendDataMutex) != TLK_ENONE) {
        tlkapi_printf(LE_OS_LOG_EN, "Mutex unlock FALSE\r\n");
    }
}

static void tlk_ble_controller_thread(void *pUsrArg)
{
    (void)pUsrArg;
    while (1) {
        tlkos_semphr_take(btSemphrHandle, bleThreadLoopOnceTimerMs);
#if (BLE_CONTROLLER_INITIAL_EN)
        tlksdk_main_loop();
#endif
    }
}

void ble_host_sal_hci_wait_ack(uint16_t timeout_ms)
{
    /* Block for ack, loop until timeout or ack received */
    if (tlkos_semphr_take(hciCmdSyncSemaphore, timeout_ms) == TLK_ENONE) {
        tlkapi_printf(LE_OS_LOG_EN, "HCI CMD SYNC Semaphore take SUCC");
    } else {
        tlkapi_printf(LE_OS_LOG_EN, "ERROR: HCI CMD opcode timeout");
    }
}

void ble_host_sal_hci_receive_complete(void)
{
    /* If the command was synchronous wake up ble_host_hci_cmd_tx() */
    if (tlkos_semphr_give(hciCmdSyncSemaphore) == TLK_ENONE) {
        tlkapi_printf(LE_OS_LOG_EN, "HCI CMD SYNC Semaphore give\r\n");
    }
}

#else //bare metal

__attribute__((section(".retention_data"))) static volatile bool hci_receive_complete = false;

void ble_host_sal_hci_wait_ack(uint16_t timeout_ms)
{
    uint32_t t = clock_time();
    /* Block for ack, loop until timeout or ack received */
    while (!clock_time_exceed(t, timeout_ms * 1000)) {
#if (LE_HOST_SEND_HCI_MODE == LE_HOST_SEND_HCI_MODE_MESSAGE)
        tlk_share_memory_service_hci_handler();
#elif (LE_HOST_SEND_HCI_MODE == LE_HOST_SEND_HCI_MODE_VHCI)
#endif
        if (hci_receive_complete == true) {
            break;
        }
    }
    hci_receive_complete = false;
}

void ble_host_sal_hci_receive_complete(void)
{
    hci_receive_complete = true;
}

#endif //#if (TLK_CFG_RTOS_ENABLE && !MCU_DUAL_CORE_ENABLE)


#if (LE_HOST_SEND_HCI_MODE == LE_HOST_SEND_HCI_MODE_VHCI)

static int app_ble_controller_hci_send_data(u32 h, u8 *para, int n)
{
    u8  send_buf[HCI_TX_FIFO_SIZE];
    u8 *p   = &send_buf[0];
    u16 len = 0;

    if (!p || n >= HCI_TX_FIFO_SIZE) {
        return -1;
    }

    if (h & HCI_FLAG_EVENT_BT_STD) {
        len  = n + 3;          //3: 1(HCI Type) + 1(Event Code) + 1(Parameter Total Length)
        *p++ = HCI_TYPE_EVENT; //HCI Type
        *p++ = h;              //Event Code
        *p++ = n;              //Parameter Total Length
        memcpy(p, para, n);
        p += n;
    } else if (h & HCI_FLAG_ACL_BT_STD) { //ACL data
                                          /*+------+-------+---------+-----------+-----------------+
        | 2    | 1     | 2       | 2         | n               |
        +------+-------+---------+-----------+-----------------+
        | len  | type  | handle  | data_len  | data_total_len  |
        +------+-------+---------+-----------+-----------------+*/
        n    = para[1];                   // para[1]: rf_len
        len  = n + 5;                     // handle| PB | BC :2byte    data total length: 2byte  : ACL date Type(0x02) 1byte
        *p++ = HCI_TYPE_ACL_DATA;
        *p++ = h;                                                         // para[1]: llid       //Handle:12|PB flag:2|BC flag:2 (2B)
        *p++ = ((h >> 8) & 0x0F) | ((para[0] & 3) == 0x01 ? 0x10 : 0x20); //start llid 2 ->0x20 ;  continue llid 1 ->0x10
        *p++ = n;                                                         //HCI ACL Data Total Length (2B)
        *p++ = n >> 8;
        memcpy(p, para + 2, n);
        p += n;
    } else if (h & HCI_FLAG_ISO_DATE_STD) {
        len  = 1 + n; //type(1)
        *p++ = HCI_TYPE_ISO_DATA;
        memcpy(p, para, n);
    }

    if (len != 0) {
        ble_host_hci_rx_complete_packet(send_buf, len);
    }

    return 0;
}
#endif // LE_HOST_SEND_HCI_MODE == LE_HOST_SEND_HCI_MODE_VHCI

#if (BLE_CONTROLLER_INITIAL_EN)

#if (TLKMDI_LE_PERIODIC_ADV_EN && PDA_SCH_CALLBACK_EN)
_attribute_ram_code_ static void tlk_periodic_adv_start_cb(void)
{
    gpio_function_en(GPIO_PF2);
    gpio_output_en(GPIO_PF2);
    gpio_input_dis(GPIO_PF2);
    gpio_set_high_level(GPIO_PF2);
}

_attribute_ram_code_ static void tlk_periodic_adv_end_cb(void)
{
    gpio_set_low_level(GPIO_PF2);
}
#endif

void user_ble_controller_init(uint8_t mac_public[6])
{
    tlkapi_printf(APP_LOG_EN, "app_ble_controller_init begin");

    //////////// LinkLayer Initialization  Begin /////////////////////////
    if (mac_public) {
        blc_ll_initStandby_module(mac_public);
    } else {
        u8 mac[] = {0x97, 0x97, 0x97, 0x97, 0x97, 0x97};
        blc_ll_initStandby_module(mac);
    }

#if !MCU_CORE_TL752X_TEMP
    void ext_pke_init(void);
    ext_pke_init();
#endif

    blc_ll_init2MPhyCodedPhy_feature();

#if (TLKMDI_ACL_CENTRAL_EN || TLKMDI_ACL_PERIPHR_EN)
    blc_ll_initAclConnection_module();

#if TLKMDI_ACL_CENTRAL_EN
    blc_ll_initAclCentralRole_module();
    /* ACL Central TX FIFO */
    blc_ll_initAclCentralTxFifo(app_acl_cen_tx_fifo, ACL_CENTRAL_TX_FIFO_SIZE, ACL_CENTRAL_TX_FIFO_NUM, TLKMW_ACL_CENTRAL_MAX_NUM);
#endif
#if TLKMDI_ACL_PERIPHR_EN
    blc_ll_initAclPeriphrRole_module();
    /* ACL Peripheral TX FIFO */
    blc_ll_initAclPeriphrTxFifo(app_acl_per_tx_fifo, ACL_PERIPHR_TX_FIFO_SIZE, ACL_PERIPHR_TX_FIFO_NUM, TLKMW_ACL_PERIPHR_MAX_NUM);
#endif

    blc_ll_setMaxConnectionNumber(TLKMW_ACL_CENTRAL_MAX_NUM, TLKMW_ACL_PERIPHR_MAX_NUM);

    blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CENTRAL_MAX_TX_OCTETS, ACL_PERIPHR_MAX_TX_OCTETS);

    /* all ACL connection share same RX FIFO */
    blc_ll_initAclConnRxFifo(app_acl_rx_fifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);

    //blc_ll_setAclCentralBaseConnectionInterval(CONN_INTERVAL_31P25MS);
    tlksdk_sch_set_base_interval(PLAN_INTERVAL_20MS);

    /* HCI RX ACL FIFO (host to controller)*/
    if (blc_ll_initHciAclDataFifo(tlkmdi_app_hci_rxAclfifo, HCI_RX_ACL_FIFO_SIZE, HCI_RX_ACL_FIFO_NUM) != BLE_SUCCESS) {
        while (1);
    }
#endif

    //////////// HCI Initialization  Begin /////////////////////////
#if (LE_HOST_SEND_HCI_MODE == LE_HOST_SEND_HCI_MODE_VHCI)

    /* HCI Event */
    blc_hci_registerControllerEventHandler(app_ble_controller_hci_send_data); //controller hci event to host all processed in this func
#endif

    /* HCI ACL DATA */
#if (TLKMDI_ACL_CENTRAL_EN || TLKMDI_ACL_PERIPHR_EN)
    blc_hci_registerControllerDataHandler(blc_hci_sendACLData2Host);
#endif
    /* HCI ISO DATA */
#if (TLKMDI_LE_CIS_CENTRAL_EN || TLKMDI_LE_CIS_PERIPHR_EN || TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN)
    blc_hci_registerControllerIsoDataHandler(blc_hci_sendIsoData2Host);
    blc_setHciInBufferMaxOctets(HCI_ISO_IN_OCTETS_MAX, HCI_RX_FIFO_NUM);
#endif
    //////////////HCI EVENT & DATA SEND C2H///////////////////////


    ////////// HCI Initialization  End /////////////////////////

#if TLKMDI_LE_LEGACY_ADV_EN
    blc_ll_initLegacyAdvertising_module();
    blc_ll_configLegacyAdvEnableStrategy(LEG_ADV_EN_STRATEGY_3); //standard BLE HCI command strategy
#endif

#if TLKMDI_LE_EXTENDED_ADV_EN
    /* Extended ADV module and ADV Set Parameters buffer initialization */
    blc_ll_initExtendedAdvModule_initExtendedAdvSetParamBuffer(app_extAdvSetParam_buf, EXT_ADV_SETS_NUMBER);
    blc_ll_initExtendedAdvDataBuffer(app_extAdvData_buf, EXT_ADV_DATA_LENGTH);
#if TLKMDI_LE_EXTENDED_SCAN_RSP_EN
    blc_ll_initExtendedScanRspDataBuffer(app_extScanRspData_buf, EXT_SCANRSP_DATA_LENGTH);
#endif
    blc_ll_setMaxAdvDelay_for_AdvEvent(MAX_DELAY_10MS); //must use 10ms delay
#endif

#if TLKMDI_LE_PERIODIC_ADV_EN
    blc_ll_initPeriodicAdvModule_initPeriodicdAdvSetParamBuffer(app_peridAdvSet_buffer, APP_PERID_ADV_SETS_NUMBER);
    blc_ll_initPeriodicAdvDataBuffer(app_peridAdvData_buffer, APP_PERID_ADV_DATA_LENGTH);
#if (PDA_SCH_CALLBACK_EN)
    blc_ll_periodic_adv_start_cb(tlk_periodic_adv_start_cb);
    blc_ll_periodic_adv_end_cb(tlk_periodic_adv_end_cb);
#endif
#endif

#if TLKMDI_LE_LEGACY_SCAN_EN
    blc_ll_initLegacyScanning_module();
#endif

#if TLKMDI_LE_LEGACY_INIT_EN
    blc_ll_initLegacyInitiating_module();
#endif

#if TLKMDI_LE_EXTENDED_SCAN_EN
    blc_ll_initExtendedScanning_module();
#endif

#if (TLKMDI_LE_EXTENDED_INIT_EN)
    blc_ll_initExtendedInitiating_module();
#endif


#if (TLKMDI_SYNCHRONIZED_RECEIVER_EN || TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN)
    blc_ll_initPeriodicAdvertisingSynchronization_module();
#endif

#if (TLKMDI_PAST_EN)
    blc_ll_initPAST_module();
#endif

#if (TLKMDI_POWER_CONTROL)
    blc_ll_initPCL_module();
#endif

#if (TLKMDI_ISOCHRONOUS_BROADCASTER_EN || TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN)
    blc_ll_InitBisParametersBuffer(app_bisToatlParam, BIS_NUM_IN_ALL_BIG_BCST, BIS_NUM_IN_ALL_BIG_SYNC);
#endif

#if (TLKMDI_ISOCHRONOUS_BROADCASTER_EN)
    blc_ll_initBigBcstModule_initBigBcstParametersBuffer(app_bigBcstParam, BIG_BCST_NUM);
    /* BIS TX buffer init */
    if (blc_ll_initBisTxFifo(app_bisBcstTxfifo, BIS_TX_PDU_FIFO_SIZE, BIS_TX_PDU_FIFO_NUM) != BLE_SUCCESS) {
        while (1);
    }
    blc_ll_initBisBcstSduInBuffer(app_bis_sdu_in_fifo, BIS_SDU_IN_FIFO_SIZE, BIS_SDU_IN_FIFO_NUM);
#endif

#if (TLKMDI_ISOCHRONOUS_BROADCASTER_SYNC_EN)
    blc_ll_initBigSyncModule_initBigSyncParametersBuffer(app_bigSyncParam, BIG_SYNC_NUM);
    /* BIS RX buffer init */
    blc_ll_initBisRxFifo(app_bisSyncRxfifo, BIS_RX_PDU_FIFO_SIZE, BIS_RX_PDU_FIFO_NUM, BIS_NUM_IN_ALL_BIG_SYNC);
    blc_ll_initBisSyncSduOutBuffer(app_bis_sdu_out_fifo, BIS_SDU_OUT_FIFO_SIZE, BIS_SDU_OUT_FIFO_NUM);
#endif


#if (TLKMDI_LE_CIS_CENTRAL_EN || TLKMDI_LE_CIS_PERIPHR_EN)

#if (TLKMDI_LE_CIS_CENTRAL_EN)
    blc_ll_initCisCentralModule_initCigParametersBuffer(app_cig_param, CIG_NUMBER);
#endif
#if (TLKMDI_LE_CIS_PERIPHR_EN)
    blc_ll_initCisPeriphrModule_initCisPeriphrParametersBuffer(app_cis_per_param, CIS_PERIPHR_NUMBER);
#endif
    blc_ll_initCisConnModule_initCisConnParametersBuffer(app_cis_conn_param, CIS_CENTRAL_NUMBER, CIS_PERIPHR_NUMBER);

    /* CIS RX PDU buffer initialization */
    blc_ll_initCisRxFifo(app_cis_rxPduFifo, CIS_RX_PDU_FIFO_SIZE, CIS_RX_PDU_FIFO_NUM);
    /* CIS TX PDU buffer initialization */
    blc_ll_initCisTxFifo(app_cis_txPduFifo, CIS_TX_PDU_FIFO_SIZE, CIS_TX_PDU_FIFO_NUM);

    /* CIS SDU in & out buffer initialization */
    blc_ll_initCisSduBuffer(app_cis_sdu_in_fifo, CIS_SDU_IN_FIFO_SIZE, app_cis_sdu_out_fifo, CIS_SDU_OUT_FIFO_SIZE);

#endif

#if PROJ_BLUETOOTH_CONTROLLER
    //    blc_ll_setAutoExchangeDataLengthEnable(0);

    //    blc_ll_addBLEFeature();

#if (BLUETOOTH_CONTROLLER_MODE == BLE_CONTROLLER_ONLY_MODE || BLUETOOTH_CONTROLLER_MODE == BT_BLE_CONTROLLER_MODE)
#if SDK_RELEASE
    extern void btc_hci_regBleCmdCallback(le_hci_cmd_callback_t cb);
    extern int  blc_hci_cmd_handler_cb(u8 * cmdPara, uint16_t opcode);
#endif
    btc_hci_regBleCmdCallback(blc_hci_cmd_handler_cb);
#endif
#endif
    //////////// LinkLayer Initialization  End /////////////////////////

#if (TLK_CFG_RTOS_ENABLE && !MCU_DUAL_CORE_ENABLE)
    int stuats = tlkos_task_create(tlk_ble_controller_thread, "bt_task", 4 * 1024, TLKSYS_TASK_CONTROLLER_PRIORITY, NULL, NULL, &btTaskHandle);
    if (stuats != TLK_ENONE) {
        tlkapi_printf(APP_LOG_EN, "OS BLE CTRL THREAD create FALSE\r\n");
        TLKSTK_ERROR_DEBUG(LE_OS_LOG_EN, 0xdeaddead);
    }
    stuats = tlkos_semphr_createCounting(&btSemphrHandle, 50, 1); //init semaphore value: 1
    if (stuats != TLK_ENONE) {
        tlkapi_printf(APP_LOG_EN, "OS BLE CTRL TASK semaphore create FALSE\r\n");
        TLKSTK_ERROR_DEBUG(LE_OS_LOG_EN, 0xdeaddead);
    }
    stuats = tlkos_mutex_create(&btSendDataMutex);
    if (stuats != TLK_ENONE) {
        tlkapi_printf(APP_LOG_EN, "OS BLE Send Data mutex create FALSE\r\n");
        TLKSTK_ERROR_DEBUG(LE_OS_LOG_EN, 0xdeaddead);
    }
    if (tlkos_semphr_createCounting(&hciCmdSyncSemaphore, 1, 0) != TLK_ENONE) {
        tlkapi_printf(APP_LOG_EN, "OS HCI CMD SYNC semaphore create FALSE\r\n");
        TLKSTK_ERROR_DEBUG(LE_OS_LOG_EN, 0xdeaddead);
    }
    tlksdk_os_registerGiveSemCb(app_os_give_sem_from_isr, app_os_give_sem);
    tlksdk_os_registerMutexSemCb(app_os_take_mutex, app_os_give_mutex);
    tlksdk_os_setOsSupEnable(1);
#endif

    tlkapi_printf(APP_LOG_EN, "app_ble_controller_init end");
}

#define BLE_BT_AUDIO_SDK_LE_VENDOR_CMD_OCF_START (0x50)
#define BLE_BT_AUDIO_SDK_LE_VENDOR_CMD_OCF_END   (0x5F)

#define BLE_VENDOR_CMD_START                     (0x50)

enum BLE_HCI_VENDOR_SUB_CMD
{
    BLE_HCI_SUB_CMD_SET_BD_ADDR = 0x01,
    BLE_HCI_SUB_CMD_SET_ACL_LATENCY,
};

/* Get the OGF and OCF from the opcode in the command */
struct ble_hci_vendor_cmd
{
    uint8_t sub_cmd;
    uint8_t data[0];
} __attribute__((packed));

uint8_t blc_hci_vendor_command_handler_hook(uint8_t *p, uint8_t *returnPara)
{
    uint8_t *p_ret = returnPara;
    U8_TO_STREAM(p_ret, BLE_SUCCESS);

    u8 opcode = p[1];

    if (opcode == BLE_VENDOR_CMD_START) {
        struct ble_hci_vendor_cmd *cmd = (struct ble_hci_vendor_cmd *)(p + 4);
        if (cmd->sub_cmd == BLE_HCI_SUB_CMD_SET_BD_ADDR) {
            blc_ll_writeBDAddr(cmd->data);
        } else if (cmd->sub_cmd == BLE_HCI_SUB_CMD_SET_ACL_LATENCY) {
            u16 latency = cmd->data[0] | cmd->data[1] << 8;
            tlksdk_pm_setManualLatency(latency);
        }
    }

    return (p_ret - returnPara);
}

#else

void user_ble_controller_init(uint8_t mac_public[6])
{
    (void)mac_public;
}

#endif

/* Host Data to Controller */
void ble_host_sal_hci_send_packet(const uint8_t *data, uint16_t len)
{
#if (LE_HOST_SEND_HCI_MODE == LE_HOST_SEND_HCI_MODE_MESSAGE)
    // tlk_printf("D25_TX %s", hex_to_str(data, len));
    while (tlk_d25f_hci_send_message(TLK_SHARE_MEMORY_MESSAGE_TYPE_BLE, (u8 *)data, len) != 0);
#elif (LE_HOST_SEND_HCI_MODE == LE_HOST_SEND_HCI_MODE_VHCI)
    blc_hci_handler((uint8_t *)data, len);
#if (TLK_CFG_RTOS_ENABLE)
    tlkapi_printf(LE_OS_LOG_EN, "BLE CTRL TASK semaphore give:%d\n", __LINE__);
    if (tlkos_semphr_give(btSemphrHandle) != TLK_ENONE) {
        tlkapi_printf(LE_OS_LOG_EN, "BLE CTRL TASK Semaphore give FALSE\r\n");
    }
#endif
#else
    (void)data;
    (void)len;
#endif
}

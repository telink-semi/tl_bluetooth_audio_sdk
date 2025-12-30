/********************************************************************************************************
 * @file    past.h
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
#ifndef PAST_H_
#define PAST_H_

#include "stack/ble/hci/hci_cmd.h"

/*! \brief	PAST source. */
enum
{
    PAST_SYNC_SRC_SCAN, //Periodic sync info from ext_scanner
    PAST_SYNC_SRC_BCST, //Periodic sync info from ext_broadcaster
    PAST_SYNC_SRC_TOTAL
};

/*! \brief	PAST parameter receive mode. */
enum
{
    PAST_MODE_OFF          = 0, /*!< LE_Periodic_Advertising_Sync_Transfer_Received event is disabled. */
    PAST_MODE_RPT_DISABLED = 1, /*!< LE_Periodic_Advertising_Sync_Transfer_Received event is enabled, LE_Periodic_Advertising_Report events is disabled. */
    PAST_MODE_RPT_ENABLED_DUP_FILTER_DIS =
        2, /*!< LE_Periodic_Advertising_Sync_Transfer_Received event is enabled, LE_Periodic_Advertising_Report events is enabled with duplicate filtering disabled. */
    PAST_MODE_RPT_ENABLED_DUP_FILTER_EN =
        3, /*!< LE_Periodic_Advertising_Sync_Transfer_Received event is enabled, LE_Periodic_Advertising_Report events is enabled with duplicate filtering enabled. */
    PAST_MODE_TOTAL = PAST_MODE_RPT_ENABLED_DUP_FILTER_EN
};

/*! \brief	PAST parameter CTE type. */
enum
{
    PAST_CTE_TYPE_NOT_SYNC_TO_AOA     = BIT(0), /*!< Do not sync to packets with an AoA Constant Tone Extension. */
    PAST_CTE_TYPE_NOT_SYNC_TO_AOD_1US = BIT(1), /*!< Do not sync to packets with an AoD Constant Tone Extension with 1 s slots. */
    PAST_CTE_TYPE_NOT_SYNC_TO_AOD_2US = BIT(2), /*!< Do not sync to packets with an AoD Constant Tone Extension with 2 s slots. */
    PAST_CTE_TYPE_ONLY_SYNC_TO_CTE    = BIT(4), /*!< Do not sync to packets without a Constant Tone Extension. */

    PAST_CTE_TYPE_SYNC_TO_WITHOUT_CTE = PAST_CTE_TYPE_NOT_SYNC_TO_AOA | PAST_CTE_TYPE_NOT_SYNC_TO_AOD_1US | PAST_CTE_TYPE_NOT_SYNC_TO_AOD_2US,
};

#define PAST_SYNC_MIN_TIMEOUT 0x000A /*!< Minimum synchronization timeout. */
#define PAST_SYNC_MAX_TIMEOUT 0x4000 /*!< Maximum synchronization timeout. */
#define PAST_MAX_SKIP         0x01F3 /*!< Maximum synchronization skip. */

typedef struct
{
    u8  pastMode;
    u8  cteType;
    u16 pastSkip;
    u16 pastSyncTimeout;
} ll_past_mng_t;

_attribute_aligned_(4) typedef struct
{
    u8  perSyncSrc; /*!< Periodic sync source. */
    u8  pastMode;
    u16 pastRcvdCEt; //Mark CEt

    u8   pastSyncCteType; //AOA/AOD concerned
    u8   past_occpied;
    bool pastRcvdSucc;
    u8   rsvd[1];

    u16 perServiceData; /*!< ID for periodic sync indication. */
    u16 perSyncHandle;  /*!< Periodic sync handle.(src:ext_scan, syncHandle /src:own_prd_bcst, advHandle) */
    u16 pastSkip;
    u16 pastSyncTimeout;

    u32 pastSendPending;
    u32 pastCreateSync; /*!< Create PeriodicAdv sync by receiving LL_PERIODIC_SYNC_IND packet method. */
    u32 pastRcvdTick;   //Special use

#if (1) /* dec special */
    u32 pastRcvdNo;
    u8 *pastDecPending;
    u8  pastTemBuf[48]; /* 39payload +2rf_header +4dma_header */
#endif

} ll_past_cb_t;

extern ll_past_mng_t blt_PastMng;


/**
 * @brief      This function is used to initialize the PAST module
 * @param[in]  none
 * @return     none
 */
void blc_ll_initPAST_module(void);


/**
 * @brief      This function is used to instruct the Controller to send synchronization information
 * 			   about the periodic advertising train identified by the Sync_Handle parameter to a connected
 * 			   device.
 * @param[in]  connHandle - Connection_Handle Range: 0x0000 to 0x0EFF
 * @param[in]  serviceData - A value provided by the Host
 * @param[in]  sync_handle - Sync_Handle identifying the periodic advertising train
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_periodicAdvSyncTransfer(u16 connHandle, u16 serviceData, u16 syncHandle);


/**
 * @brief      This function is used to to instruct the Controller to send synchronization information
 * 			   about the periodic advertising in an advertising set to a connected device.
 * @param[in]  connHandle - Connection_Handle Range: 0x0000 to 0x0EFF
 * @param[in]  serviceData - A value provided by the Host
 * @param[in]  advHandle - Used to identify an advertising set: 0x00C0xEF
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_periodicAdvSetInfoTransfer(u16 connHandle, u16 serviceData, u8 advHandle);


/**
 * @brief      This function is used to specify how the Controller will process periodic advertising
 * 			   synchronization information received from the device identified by the Connection_Handle
 * 			   parameter (the "transfer mode")
 * @param[in]  connHandle - Connection_Handle Range: 0x0000 to 0x0EFF
 * @param[in]  mode -
 * @param[in]  skip - The number of periodic advertising packets that can be skipped after a successful receive
 * 				      Range: 0x0000 to 0x01F3
 * @param[in]  syncTimeout - Synchronization timeout for the periodic advertising train Range: 0x000A to 0x4000
 * 			                 Time = N*10 ms: Time Range: 100 ms to 163.84 s
 * @param[in]  cteType -
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setPeriodicAdvSyncTransferParams(u16 connHandle, u8 mode, u16 skip, u16 syncTimeout, u8 cteType);


/**
 * @brief      This function is used to specify the initial value for the mode, skip, timeout, and
 * 			   Constant Tone Extension type (set by the HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters command;
 * 			   see Section 7.8.91) to be used for all subsequent connections over the LE transport.
 * @param[in]  mode -
 * @param[in]  skip - The number of periodic advertising packets that can be skipped after a successful receive
 * 				      Range: 0x0000 to 0x01F3
 * @param[in]  syncTimeout - Synchronization timeout for the periodic advertising train Range: 0x000A to 0x4000
 * 			                 Time = N*10 ms: Time Range: 100 ms to 163.84 s
 * @param[in]  cteType -
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setDftPeriodicAdvSyncTransferParams(u8 mode, u16 skip, u16 syncTimeout, u8 cteType);


/**
 * @brief      This function is used to enable or disable reports for the periodic advertising train
 *             identified by the Sync_Handle parameter
 * @param[in]  sync_handle - Sync_Handle identifying the periodic advertising train
 * @param[in]  enable
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_periodicAdvertisingReceiveEn(hci_le_setPeriodicAdvReceiveEnCmdParams_t *cmdPara);


/**
 * @brief      This function is used to instruct the Controller to send synchronization information
 * 			   about the periodic advertising train identified by the Sync_Handle parameter to a connected
 * 			   device.
 * @param[in]  *cmdPara - refer to 'hci_le_pastCmdParams_t'
 * @param[out] *retPara - refer to 'hci_le_pastRetParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_periodicAdvSyncTransfer(hci_le_pastCmdParams_t *cmdPara, hci_le_pastRetParams_t *retPara);


/**
 * @brief      This function is used to to instruct the Controller to send synchronization information
 * 			   about the periodic advertising in an advertising set to a connected device.
 * @param[in]  *cmdPara - refer to 'hci_le_paSetInfoTransferCmdParams_t'
 * @param[out] *retPara - refer to 'hci_le_paSetInfoTransferRetParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_periodicAdvSetInfoTransfer(hci_le_paSetInfoTransferCmdParams_t *cmdPara, hci_le_paSetInfoTransferRetParams_t *retPara);

/**
 * @brief      This function is used to specify how the Controller will process periodic advertising
 * 			   synchronization information received from the device identified by the Connection_Handle
 * 			   parameter (the "transfer mode")
 * @param[in]  *cmdPara - refer to 'hci_le_pastParamsCmdParams_t'
 * @param[out] *retPara - refer to 'hci_le_pastParamsRetParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_setPeriodicAdvSyncTransferParams(hci_le_pastParamsCmdParams_t *cmdPara, hci_le_pastParamsRetParams_t *retPara);

/**
 * @brief      This function is used to specify the initial value for the mode, skip, timeout, and
 * 			   Constant Tone Extension type (set by the HCI_LE_Set_Periodic_Advertising_Sync_Transfer_Parameters command;
 * 			   see Section 7.8.91) to be used for all subsequent connections over the LE transport.
 * @param[in]  *cmdPara - refer to 'hci_le_dftPastParamsCmdParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_setDftPeriodicAdvSyncTransferParams(hci_le_dftPastParamsCmdParams_t *cmdPara);


#endif /* LL_PC_H_ */

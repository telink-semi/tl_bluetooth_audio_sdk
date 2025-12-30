/********************************************************************************************************
 * @file    pcl.h
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
#ifndef PWRCTRL_H_
#define PWRCTRL_H_


/*! \brief	The RSSI metric shall be set to 127 */
#define LL_RSSI_METRIC_VALUE 127

/*! \brief	Power control txPower field */
#define LL_PWR_CTRL_TXPWR_MAX     127 //maximum tx power
#define LL_PWR_CTRL_TXPWR_MIN     126 //minimum tx power
#define LL_PWR_CTRL_TXPWR_UNAVA   127 //unavailable tx power
#define LL_PWR_CTRL_TXPWR_UNMNGED 126 //unmanaged tx power

/*! \brief	Power control Min or Max field. */
#define LL_PWR_CTRL_LIMIT_MIN_BIT BIT(0) //sender is at the minimum supported power
#define LL_PWR_CTRL_LIMIT_MAX_BIT BIT(1) //sender is at the maximum supported power

/*! \brief	Path_Loss_Reporting_Parameters: High Threshold unused. */
#define LL_PWR_CTRL_PATHLOSS_UNUSED_HIGH_THRESHOLD 0xFF

/*! \brief	Default Low threshold for auto request. */
#define LL_PWR_CTRL_AUTO_RSSI_LOW_THRESHOLD -60 //unit: dBm

/*! \brief	Default High threshold for auto request. */
#define LL_PWR_CTRL_AUTO_RSSI_HIGH_THRESHOLD -38 //unit: dBm

/*! \brief  Default auto Minimum time spent until request. */
#define LL_PWR_CTRL_AUTO_MIN_TIME 20

/*! \brief	Default auto request of 'delta' value. */
#define LL_PWR_CTRL_AUTO_REQUEST_VAL 5 //unit: dBm

/* Refer to <<DS_TLSR9518-E5_Datasheet for Telink Multi-Standard Wireless SoC TLSR9518.pdf>>, Page 38.
 * BLE 1 Mbps RF_RX: Sensitivity 1 Mbps - 		-95.5dBm
 * BLE 2 Mbps RF_RX: Sensitivity 2 Mbps - 		-92.5dBm
 * BLE 500 kbps RF_RX: Sensitivity 500 kbps - 	-98.5dBm
 * BLE 125 kbps RF_RX: Sensitivity 500 kbps - 	-99.5dBm
 */
#define LL_PWR_CTRL_RSSI_MIN_VALUE (-90) //dBm

/**
 * @brief Command/Return Parameters for "7.8.117 LE Enhanced Read Transmit Power Level command"
 */
typedef enum
{
    BLE_PC_PHY_NONE     = 0x00,
    BLE_PC_PHY_1M       = 0x01,
    BLE_PC_PHY_2M       = 0x02,
    BLE_PC_PHY_CODED_S8 = 0x03,
    BLE_PC_PHY_CODED_S2 = 0x04,
    BLE_PC_PHY_TOTAL    = BLE_PC_PHY_CODED_S2,
    BLE_PC_PHY_INVALID  = 0xFF,
} pc_phy_t;

typedef enum
{
    BLE_PC_1M_BIT       = (1 << 0), //LE 1M PHY bit
    BLE_PC_2M_BIT       = (1 << 1), //LE 2M PHY bit
    BLE_PC_CODED_S8_BIT = (1 << 2), //LE Coded S8 PHY bit
    BLE_PC_CODED_S2_BIT = (1 << 3), //LE Coded S2 PHY bit
    BLE_PC_MAX_BIT      = BLE_PC_CODED_S2_BIT,

    BLE_PC_ALL_BITS = BLE_PC_1M_BIT | BLE_PC_2M_BIT | BLE_PC_CODED_S8_BIT | BLE_PC_CODED_S2_BIT
} pc_phy_bits_t;

/*! brief	Tx Power Reporting Reason */
enum
{
    LL_PWR_CTRL_RPTING_REASON_LOCAL = 0x00, /*!< Local transmit power changed */
    LL_PWR_CTRL_RPTING_REASON_REMOTE,       /*!< Remote transmit power changed */
    LL_PWR_CTRL_RPTING_READ_REMOTE          /*!< HCI_LE_Read_Remote_Transmit_Power_Level command completed */
};

/*! \brief	Power control scheme states. */
enum
{
    LL_PWR_CTRL_PATHLOSS_RPTING_DISABLED = 0x00, /*!< Path loss reporting disabled */
    LL_PWR_CTRL_PATHLOSS_RPTING_ENABLED,         /*!< Path loss reporting enabled */
    LL_PWR_CTRL_PATHLOSS_RPTING_READY,           /*!< Path loss reporting ready for enable */
    /* Auto monitoring power: controller auto Initiate LL_PC_REQ enabled */
    LL_PWR_CTRL_AUTO_MONITORING_ENABLED  = 0x01,
    LL_PWR_CTRL_AUTO_MONITORING_DISABLED = 0x00,
};

/*! \brief	Path loss monitoring zones. */
enum
{
    LL_PWR_CTRL_PATH_LOSS_ZONE_LOW = 0x00, /*!< Low path loss zone. */
    LL_PWR_CTRL_PATH_LOSS_ZONE_MID,        /*!< Medium path loss zone. */
    LL_PWR_CTRL_PATH_LOSS_ZONE_HIGH        /*!< High path loss zone. */
};

typedef struct __attribute__((packed))
{
    u8 llid;
    u8 rf_len;
    u8 opcode;
    u8 phy;
    s8 delta;
    s8 txPwr;
} rf_pkt_ll_pwr_ctrl_req_t; //LL_POWER_CONTROL_REQ

typedef struct __attribute__((packed))
{
    u8 llid;
    u8 rf_len;
    u8 opcode;
    u8 limitInfo; //bit0:min, bit1:max, bit2~7:rfu
    s8 delta;
    s8 txPwr;
    u8 APR;
} rf_pkt_ll_pwr_ctrl_rsp_t; //LL_POWER_CONTROL_RSP

typedef struct __attribute__((packed))
{
    u8 llid;
    u8 rf_len;
    u8 opcode;
    u8 phy;
    u8 limitInfo;          //bit0:min, bit1:max, bit2~7:rfu
    s8 delta;
    s8 txPwr;
} rf_pkt_ll_pwr_chg_ind_t; //LL_POWER_CHANGE_IND

typedef struct
{
    //power control concerned
    u8 pcl_cis_curTimeSpent;
    u8 pcl_occpied;
    u8 pcl_rsvd[2];

    s8 phyTxPwrLvl[BLE_PC_PHY_TOTAL];
    u8 peerApr[BLE_PC_PHY_TOTAL];

    s8   usedPhyTxPwr; // pcl used, current used tx pwr
    bool rdRemoteTxPwr;
    u8   pathLossRptState;
    u8   autoMinitorState;

    u8   peerTxPwrLvl;
    bool pc_peerReqRcvd;
    u8   pc_sendReq;
    u8   pc_delta;

    u8   pc_reqPhy;    //refer to 'pc_phy_t'
    u8   peerLimitInfo;
    bool pwrRptLocal;  //current report local power changes
    bool pwrRptRemote; //current report remote power changes

    u8 pcl_chg_ind_irq_pending;
    s8 pcl_chg_tx_pwr;
    u8 pcl_chg_lephy;
    u8 pcl_chg_cis_use;

    struct
    {
        s8 highThreshold;       /*!< High extreme RSSI threshold. */
        s8 lowThreshold;        /*!< LOw extreme RSSI threshold. */
        u8 minTimeSpent;        /*!< Minimum time spent to trigger an LL_PC_REQ. */
        u8 curTimeSpent;        /*!< Current time spent in an extreme RSSI zone. */
        u8 reqDelta;            /*!< Value of 'delta' in LL_PC_REQ. */
        u8 sendPcReqIrqPending; /*!< IRQ send LL_PC_REQ pending flag. */
        s8 sendDeltaMark;
        u8 sendPcReqTxPwrPhy;
    } autoMonitor;

    struct
    {
        u8 highThreshold;             /*!< Path loss high threshold. */
        u8 highHysteresis;            /*!< Path loss high hysteresis. */
        u8 lowThreshold;              /*!< Path loss low threshold. */
        u8 lowHysteresis;             /*!< Path loss low hysteresis. */

        u8 minTimeSpent;              /*!< Minimum time spent to trigger an event. */
        u8 curTimeSpent;              /*!< Current time spent in a new path loss zone. */
        u8 curZone;                   /*!< Current path loss zone. */
        u8 newZone;                   /*!< New zone. */

        u8 curPathLoss;               /*!< Current path loss. */
        u8 sendReq2StartMonitoring;   /*!< A power control request is required to start path loss monitoring. */
        u8 pathLossRptEvtIrqPending;  /*!< IRQ Path loss reporting event pending flag. */
        u8 pathLossRptEvtloopPending; /*!< Loop Path loss reporting event pending flag. */
    } pathLoss;                       /*!< Path loss monitoring parameters. */

} ll_pcl_cb_t;

/**
 * @brief      This function is used to initialize the LL Power control module
 * @param[in]  none
 * @return     none
 */
void blc_ll_initPCL_module(void);


/**
 * @brief      This function is used to read the minimum and maximum transmit powers supported by the Controller
 *             across all supported PHYs
 * @param[in]  connHandle - Connection_Handle Range: 0x0000 to 0x0EFF
 * @param[in]  *pOutCurTxPwrLvl - Current transmit power level, -127~20, unit:dBm
 * @param[in]  *pOutMaxTxPwrLvl - Maximum transmit power level, -127~20, unit:dBm
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */

ble_sts_t blc_ll_readEnhancedTxPower(u16 connHandle, u8 phy, s8 *pOutCurTxPwrLvl, s8 *pOutMaxTxPwrLvl);

/**
 * @brief      This function is used to to read the transmit power level used by the remote Controller on the ACL
 *             connection that is identified by the Connection_Handle parameter and the PHY indicated by the PHY parameter
 * @param[in]  connHandle - Connection_Handle Range: 0x0000 to 0x0EFF
 * @param[in]  delta - shall be set to the requested change in the recipient's transmit power level, in dB, for
 *                     the PHY indicated. The value is a signed integer: a positive value indicates a request to
 *                     increase the transmit power level, a negative value indicates a request to decrease it, and zero
 *                     indicates that no change is being requested.
 * @param[in]  phy - refer to 'pc_phy_t': range: 1~4
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_readRemoteTxPwrLvl(u16 connHandle, u8 delta, u8 phy);

/**
 * @brief      This function is used to to set the path loss threshold reporting parameters for the ACL connection
 *             identified by the Connection_Handle parameter.
 * @param[in]  connHandle - Connection_Handle Range: 0x0000 to 0x0EFF
 * @param[in]  highThresh - High threshold for the path loss Units: dB, 0xFF unused
 * @param[in]  highHyst - Hysteresis value for the high threshold Units: dB
 * @param[in]  lowThresh - Low threshold for the path loss Units: dB, 0xFF unused
 * @param[in]  lowHyst - Hysteresis value for the low threshold Units: dB
 * @param[in]  minTime - Minimum time in number of connection events to be observed once the path crosses the threshold before an event is generated.
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setPathLossRptingParams(u16 connHandle, u8 highThresh, u8 highHyst, u8 lowThresh, u8 lowHyst, u16 minTime);

/**
 * @brief      This function is used to to enable or disable path loss reporting for the ACL connection identified
 *             by the Connection_Handle parameter.
 * @param[in]  connHandle - Connection_Handle Range: 0x0000 to 0x0EFF
 * @param[in]  enable - 0: Reporting disabled; 1: Reporting enabled
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setPathLossRptingEnable(u16 connHandle, u8 enable);

/**
 * @brief      This function is used to to enable or disable the reporting to the local Host of transmit power level
 *             changes in the local and remote Controllers for the ACL connection identified by the Connection_Handle parameter.
 * @param[in]  connHandle - Connection_Handle Range: 0x0000 to 0x0EFF
 * @param[in]  localEn - 0: Disable local transmit power reports; 1: Disable local transmit power reports
 * @param[in]  remoteEn - 0: Disable remote transmit power reports; 1: Disable remote transmit power reports
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_ll_setTxPwrRptingEnable(u16 connHandle, u8 localEn, u8 remoteEn);


/**
 * @brief      This function is used to to reads the Received Signal Strength Indication (RSSI) value from a Controller..
 * @param[in]  cmdPara - refer to 'hci_readRssi_cmdParam_t'
 * @param[in]  retPara - refer to 'hci_readRssi_retParam_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_readRSSI(hci_readRssi_cmdParam_t *cmdPara, hci_readRssi_retParam_t *retPara);

/**
 * @brief      This function is used to read the minimum and maximum transmit powers supported by the Controller
 *             across all supported PHYs
 * @param[in]  cmdPara - refer to 'hci_le_rdTxPwrLvlCmdParams_t'
 * @param[in]  retPara - refer to 'hci_le_enRdTxPwrLvlRetParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_readEnhancedTxPower(hci_le_rdTxPwrLvlCmdParams_t *cmdPara, hci_le_enRdTxPwrLvlRetParams_t *retPara);

/**
 * @brief      This function is used to to read the transmit power level used by the remote Controller on the ACL
 *             connection that is identified by the Connection_Handle parameter and the PHY indicated by the PHY parameter
 * @param[in]  cmdPara - refer to 'hci_le_rdTxPwrLvlCmdParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_readRemoteTxPwrLvl(hci_le_rdTxPwrLvlCmdParams_t *cmdPara);

/**
 * @brief      This function is used to to set the path loss threshold reporting parameters for the ACL connection
 *             identified by the Connection_Handle parameter.
 * @param[in]  cmdPara - refer to 'hci_le_setPathLossRptingCmdParams_t'
 * @param[in]  retPara - refer to 'hci_le_setPathLossRptingRetParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_setPathLossRptingParams(hci_le_setPathLossRptingCmdParams_t *cmdPara, hci_le_setPathLossRptingRetParams_t *retPara);

/**
 * @brief      This function is used to to enable or disable path loss reporting for the ACL connection identified
 *             by the Connection_Handle parameter.
 * @param[in]  cmdPara - refer to 'hci_le_setPathLossRptingEnCmdParams_t'
 * @param[in]  retPara - refer to 'hci_le_setPathLossRptingEnRetParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_setPathLossRptingEnable(hci_le_setPathLossRptingEnCmdParams_t *cmdPara, hci_le_setPathLossRptingEnRetParams_t *retPara);

/**
 * @brief      This function is used to to enable or disable the reporting to the local Host of transmit power level
 *             changes in the local and remote Controllers for the ACL connection identified by the Connection_Handle parameter.
 * @param[in]  cmdPara - refer to 'hci_le_setTxPwrRptingEnCmdParams_t'
 * @param[in]  retPara - refer to 'hci_le_setTxPwrRptingEnRetParams_t'
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t blc_hci_le_setTxPwrRptingEnable(hci_le_setTxPwrRptingEnCmdParams_t *cmdPara, hci_le_setTxPwrRptingEnRetParams_t *retPara);

#endif /* PWRCTRL_H_ */

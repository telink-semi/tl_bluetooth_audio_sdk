/********************************************************************************************************
 * @file    bth_handle.h
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
#ifndef BTH_HANDLE_H
#define BTH_HANDLE_H

typedef struct
{
    uint8_t state;
    uint8_t attrs;
    uint16_t busys;
    uint16_t flags;
    uint16_t timeout;
    uint32_t connTime;
    uint16_t aclHandle;
    uint16_t pageTime;
    uint16_t rswTimer;      // negetive rsw
    uint16_t authSecrTimer; // negetive auth
    uint16_t exitTimer;     // Exit Sniffer wait timer in disconnect acl.
    uint16_t connTimer;

    uint8_t sniffBusys;    // Refer BTH_ACL_SNIFF_BUSYS_ENUM.
    uint8_t sniffReqCount;
    uint16_t sniffReqTimer;
    uint16_t unsniffReqTimer;
    uint16_t leaveSniffTimer;

    uint8_t reason;
    uint8_t othBusys; // Other Busys
    uint8_t btaddr[6];

    uint32_t devClass;
    uint8_t curRole;
    uint8_t airMode;
    uint8_t aclPktNumb;
    uint8_t positive;   // 1:positive connect to the remote or 0: negative be connected
    uint8_t switchCnt;  // role_switch_req_cnt
    uint8_t scanMode;   // page_scan_repetition_mode
    uint16_t clkOffs;
    uint32_t extFeature; // Peer

    uint8_t curMode;
    uint8_t initRole;
    uint8_t curPolicy;
    uint8_t setPolicy;

    uint8_t pinCode[4];
    uint8_t linkKey[16];
    uint8_t infoRsp[16];
    uint8_t devName[32];

    TlkApiTimer_t timer;
} bth_handle_t, bth_acl_handle_t;

typedef struct
{
    uint8_t         state;
    uint8_t         busys;
    uint8_t         reason;
    uint8_t         airMode;
    uint8_t         timeout;
    uint8_t         positive;
    uint8_t         linkType; // BTH_LINK_TYPE_ESCO or BTH_LINK_TYPE_SCO
    uint8_t         resv01;
    uint16_t         aclHandle;
    uint16_t         scoHandle;
    TlkApiTimer_t  timer;
} bth_sco_handle_t;

typedef uint8_t (*bth_acl_get_tws_role_cb_t)(void);
typedef uint8_t (*bth_acl_get_tws_handover_cb_t)(void);

void bth_handle_acl_register_tws_role_cb(bth_acl_get_tws_role_cb_t cb);
void bth_handle_acl_register_tws_handover_cb(bth_acl_get_tws_handover_cb_t cb);

/******************************************************************************
 * Function: bth_handle_init
 * Descript: Initial the acl handle control block and sco handle control block.
 * Params:
 * 		@aclNumb[IN]--refer to TLK_STK_BTACL_NUMB
 * 		@scoNumb[IN]--refer to TLK_STK_BTSCO_NUMB
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *         If others value is returned means the initial process fail.
 *******************************************************************************/
int bth_handle_init(uint8_t aclNumb, uint8_t scoNumb);

/******************************************************************************
 * Function: bth_handle_deinit
 * Descript: deinitial the acl handle control block and sco handle control block.
 * Params:
 * Return: Returning TLK_ENONE(0x00) means the initial process success.
 *******************************************************************************/
int bth_handle_deinit(void);

/******************************************************************************
 * Function: bth_handle_getMemLen
 * Descript: get the size of the required number of nodes interface.
 * Params:
 * 		@aclNumb[IN]--refer to TLK_STK_BTACL_NUMB
 * 		@scoNumb[IN]--refer to TLK_STK_BTSCO_NUMB
 * Return: success: node size.
 *         -TLK_EPARAM means the get memory length fail.
 *******************************************************************************/
int bth_handle_getMemLen(uint8_t aclNumb, uint8_t scoNumb);


/******************************************************************************
 * Function: bth_handle_getIdleAclCount
 * Descript: Get the number of idle acl handle.
 * Params: None.
 * Return: The number of idle acl handle.
 *******************************************************************************/
uint8_t bth_handle_getIdleAclCount(void);

/******************************************************************************
 * Function: bth_handle_getUsedAclCount
 * Descript: Get the number of used acl handle.
 * Params: None.
 * Return: The number of used acl handle.
 *******************************************************************************/
uint8_t bth_handle_getUsedAclCount(void);

/******************************************************************************
 * Function: bth_handle_getConnAclCount
 * Descript: Get the number of Connected acl handle.
 * Params: None.
 * Return: The number of connected acl handle.
 *******************************************************************************/
uint8_t bth_handle_getConnAclCount(void);

/******************************************************************************
 * Function: bth_handle_getDiscAclCount
 * Descript: Get the number of DisConnected acl handle.
 * Params: None.
 * Return: The number of connected acl handle.
 *******************************************************************************/
uint8_t bth_handle_getDiscAclCount(void);

/******************************************************************************
 * Function: bth_handle_getIdleScoCount
 * Descript: Get the number of idle sco handle.
 * Params: None.
 * Return: The number of idle sco handle.
 *******************************************************************************/
uint8_t bth_handle_getIdleScoCount(void);

/******************************************************************************
 * Function: bth_handle_getUsedScoCount
 * Descript: Get the number of used sco handle.
 * Params: None.
 * Return: The number of used sco handle.
 *******************************************************************************/
uint8_t bth_handle_getUsedScoCount(void);

/******************************************************************************
 * Function: bth_handle_getConnScoCount
 * Descript: Get the number of connected sco handle.
 * Params: None.
 * Return: The number of connected sco handle.
 *******************************************************************************/
uint8_t bth_handle_getConnScoCount(void);

/******************************************************************************
 * Function: bth_handle_getDiscScoCount
 * Descript: Get the number of disconnected sco handle.
 * Params: None.
 * Return: The number of disconnected sco handle.
 *******************************************************************************/
uint8_t bth_handle_getDiscScoCount(void);

/******************************************************************************
 * Function: bth_handle_getAclHandle
 * Descript: Get the acl handle via bt address.
 * Params: None.
 * Return: The acl handle.
 *******************************************************************************/
uint16_t bth_handle_getAclHandle(uint8_t *pBtAddr);

/******************************************************************************
 * Function: bth_handle_getBtAddr
 * Descript: Get the Bt address.
 * Params: @aclHandle[IN]--The acl link handle.
 * Return: The bt address.
 *******************************************************************************/
uint8_t *bth_handle_getBtAddr(uint16_t aclHandle);

/******************************************************************************
 * Function: bth_handle_resetAcl
 * Descript: reset the acl handle and release the resource.
 * Params:
 *        @pHandle[IN]--The acl link handle.
 * Return: None.
 *******************************************************************************/
void bth_handle_resetAcl(bth_acl_handle_t *pHandle);

/******************************************************************************
 * Function: bth_handle_resetSco
 * Descript: reset the sco handle and release the resource.
 * Params:
 *        @pHandle[IN]--The acl link handle.
 * Return: None.
 *******************************************************************************/
void bth_handle_resetSco(bth_sco_handle_t *pHandle);


/******************************************************************************
 * Function: Get the idle acl handle item or get used or connected item
 *           via acl handle or bt address
 * Descript: get the acl handle item.
 * Params:
 * Return: acl Handle item.
 *******************************************************************************/
bth_acl_handle_t *bth_handle_getIdleAcl(void);
bth_acl_handle_t *bth_handle_getFirstConnAcl(void);
bth_acl_handle_t *bth_handle_getUsedAcl(uint16_t aclHandle);
bth_acl_handle_t *bth_handle_getConnAcl(uint16_t aclHandle);
bth_acl_handle_t *bth_handle_getDiscAcl(uint16_t aclHandle);
bth_acl_handle_t *bth_device_getConnAclByType(uint8_t devType);
bth_acl_handle_t *bth_handle_searchUsedAcl(uint8_t *pBtAddr);
bth_acl_handle_t *bth_handle_searchConnAcl(uint8_t *pBtAddr);

int    bth_handle_aclPktNumbUpdate(uint16_t aclHandle, bool isInc, uint8_t pktNumb);
uint8_t bth_handle_aclGetTotalUsedPktNumb(void);
void bth_handle_set_all_acl_max_slot(uint8_t slot);

/******************************************************************************
 * Function: Get the idle sco handle item or get used or connected item
 *           via sco handle or acl handle.
 * Descript: get the sco handle item.
 * Params:
 * Return: sco Handle item.
 *******************************************************************************/
bth_sco_handle_t *bth_handle_getIdleSco(void);
bth_sco_handle_t *bth_handle_getUsedSco(uint16_t scoHandle);
bth_sco_handle_t *bth_handle_getConnSco(uint16_t scoHandle);
bth_sco_handle_t *bth_handle_getDiscSco(uint16_t scoHandle);
bth_sco_handle_t *bth_handle_searchUsedSco(uint16_t aclHandle);
bth_sco_handle_t *bth_handle_searchConnSco(uint16_t aclHandle);
bth_sco_handle_t *bth_handle_searchBusySco(uint16_t aclHandle);
bth_sco_handle_t *bth_handle_findUsedSco(uint16_t aclHandle, uint8_t offset);
bth_sco_handle_t *bth_handle_findConnSco(uint16_t aclHandle, uint8_t offset);
uint16_t            bth_handle_getConnScoHandle(uint16_t aclHandle);


#endif // BTH_HANDLE_H

/********************************************************************************************************
 * @file    bth.h
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
#ifndef BTH_H
#define BTH_H


/******************************************************************************
 * Function: BTH Init interface
 * Descript: This interface be used to initial bth layer.
 * Params:
 * Return: Returning TLK_ENONE(0x00) means the send process success.
 *         If others value is returned means the send process fail.
 *******************************************************************************/
int  bth_init(void);
void bth_deinit(void);

int bth_needMemLen(void);

bool bth_isBusy(void);

bool bth_pmIsBusy(void);
uint bth_getAclCount(void);

int bth_sendEnterSleepCmd(void);
int bth_sendLeaveSleepCmd(void);

/******************************************************************************
 * Function: BTH Destroy interface
 * Descript: This interface be used to release the resource which allocate.
 * Params: @aclhandle[IN]--The acl link handle.
 * Return: None.
 *******************************************************************************/
void bth_destroy(uint16_t aclHandle);

/******************************************************************************
 * Function: bth_isHeadset
 * Descript: This interface be used to verify peer device is headset or
 *           others device.
 * Params: @aclhandle[IN]--The acl link handle.
 * Return: Return true means peer device type headset, false means others .
 *******************************************************************************/
bool bth_isHeadset(uint16_t aclHandle);

/******************************************************************************
 * Function: bth_devClassToDevType
 * Descript: This interface be used to reflect from devClass to device type.
 * Params: @devClass[IN]--The device class.
 * Return: The Device type.
 *******************************************************************************/
uint8_t bth_devClassToDevType(uint32_t devClass);

/******************************************************************************
 * Function: bth_acl_setInitRole
 * Descript: This interface be used to set the role of acl link.
 * Params: @btaddr[IN]--The device bt address.
 *         @initRole[IN]--The role of acl link.
 * Return: TLK_ENONE is set success, others means failure.
 *******************************************************************************/
extern int bth_acl_setInitRole(uint8_t btaddr[6], uint8_t initRole);

/******************************************************************************
 * Function: bth_acl_connect
 * Descript: This interface be used to start the acl link set up.
 * Params: @btaddr[IN]--The device bt address.
 *         @devClass[IN]--The device class.
 *         @initRole[IN]--The role of acl link.
 *         @timeout[IN]--The acl setup timeout value. Range:3000~3600000.
 * Return: TLK_ENONE is set success, others means failure.
 *******************************************************************************/
extern int bth_acl_connect(uint8_t btaddr[6], uint32_t devClass, uint8_t initRole, uint32_t timeout);

/******************************************************************************
 * Function: bth_acl_disconn
 * Descript: This interface be used to tear up the acl link which specify
 *           by aclhandle.
 * Params: @aclHandle[IN]--The acl link handle.
 * Return: TLK_ENONE is set success, others means failure.
 *******************************************************************************/
extern int bth_acl_disconn(uint16_t aclHandle, uint8_t reason);

/******************************************************************************
 * Function: bth_acl_destroy
 * Descript: This interface be used to release the acl link resource which
 *           specify by aclhandle.
 * Params: @aclHandle[IN]--The acl link handle.
 * Return: None.
 *******************************************************************************/
extern void bth_acl_destroy(uint16_t aclHandle);

/******************************************************************************
 * Function: bth_acl_connectCancel
 * Descript: This interface be used to cancel the acl link connect which
 *           specify by bt address.
 * Params: @btaddr[IN]--The peer device address.
 * Return: TLK_ENONE is set success, others means failure.
 *******************************************************************************/
extern int bth_acl_connectCancel(uint8_t btaddr[6]);


/******************************************************************************
 * Function: bth_sco_connect
 * Descript: This interface be used to connect the SCO link which
 *           specify by aclHandle.
 * Params: @aclHandle[IN]--The acl link handle.
 *         @linkType[IN]--it is a scl or sco link type.
 *         @airMode[IN]--The air mode(cvsd or else).
 * Return: TLK_ENONE is set success, others means failure.
 *******************************************************************************/
extern int bth_sco_connect(uint16_t aclHandle, uint16_t linkType, uint8_t airMode);

/******************************************************************************
 * Function: bth_sco_disconn
 * Descript: This interface be used to disconn the SCO link which
 *           specify by scoHandle.
 * Params: @scoHandle[IN]--The sco link handle.
 *         @reason[IN]--disconnect reason.
 * Return: TLK_ENONE is set success, others means failure.
 *******************************************************************************/
extern int bth_sco_disconn(uint16_t scoHandle, uint8_t reason);

/******************************************************************************
 * Function: bth_sco_destroy
 * Descript: This interface be used to destroy the SCO link resource.
 * Params: @scoHandle[IN]--The sco link handle.
 * Return: None.
 *******************************************************************************/
extern void bth_sco_destroy(uint16_t scoHandle);

/******************************************************************************
 * Function: bth_sco_destroyByAclHandle
 * Descript: This interface be used to destroy the SCO link resource.
 * Params: @scoHandle[IN]--The sco link handle.
 * Return: None.
 *******************************************************************************/
extern void bth_sco_destroyByAclHandle(uint16_t aclHandle);

/******************************************************************************
 * Function: bth_sco_sendMute
 * Descript: This interface be used to set the phone mute.
 * Params: @scoHandle[IN]--The sco link handle.
 *         @micSpk[IN]--is mic or speaker.
 *         @enable[IN]--enable or disable.
 * Return: TLK_ENONE is set success, others means failure.
 *******************************************************************************/
extern int bth_sco_sendMute(uint16_t scoHandle, uint8_t micSpk, uint8_t enable);


#endif // BTH_H

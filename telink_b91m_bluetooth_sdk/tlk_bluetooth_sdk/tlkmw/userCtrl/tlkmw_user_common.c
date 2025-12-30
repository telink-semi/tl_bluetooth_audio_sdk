/********************************************************************************************************
 * @file    tlkmw_user_common.c
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
#include "tlkmw/tlkmw.h"
#include "tlkmw_user_common.h"
#include "ota/tlk_ota_protocol_common.h"
#include "ota/general_protocol/tlk_ota_general_desc_parse.h"
#include "ota/general_protocol/tlk_ota_general_protocol_example.h"

#define TLKAPP_TINYSQL_TOTAL_IMAGE_ADDR 0x12000
dfuOutImgCtrl_t                    dfuOutImgCtrl;
extern sTlk_total_fw_descriptors_t sTlkMwCurImgHeader;
extern int                         tlkmw_ota_load_cur_img_header(void);
extern sTlk_boot_and_ota_cfg_t     sTlk_boot_ota_cfg;

/**
 * @brief      Get the startup address by bin type from flash
 * @param[in]  bin_type - bin type
 * @param[out] none
 * @return     uint32_t - bin startup address
 */
static uint32_t tlkmw_getBinStartUpAddrFromFlash(uint8_t bin_type, uint8_t isSclectVersion)
{
#if (TLK_MW_USER_CTRL_ENABLE)
    uint32_t bin_version            = 0x00;
    uint8_t  mode_select            = 0;
    uint32_t flash_start_addr_binx  = 0;
    uint32_t binx_start_addr_offset = 0;
    tlkmw_ota_load_cur_img_header();
    //TODO
    sTlk_boot_ota_cfg.switch_app_mode  = 0xff;
    sTlk_boot_ota_cfg.default_app_mode = 0x00;

    if ((sTlk_boot_ota_cfg.switch_app_mode != 0xff) && (sTlk_boot_ota_cfg.switch_app_mode != 0x00)) {
        mode_select = sTlk_boot_ota_cfg.switch_app_mode;
    } else {
        mode_select = sTlk_boot_ota_cfg.default_app_mode;
    }
    struct sTlk_fw_group_list_t *gTlk_fw_group_list;
    gTlk_fw_group_list = tlkmw_get_fw_group_node_by_index(sTlkMwCurImgHeader.fw_group_list, mode_select);

    for (uint8_t index = 0; index < gTlk_fw_group_list->bin_count; index++) {
        struct sTlk_fw_info_list_t *gTlk_fw_info_list_t;
        gTlk_fw_info_list_t = tlkmw_get_fw_info_node_by_index(sTlkMwCurImgHeader.fw_group_list->fw_info_list, index);

        sTlk_fw_descriptors_node_t *gTlk_fw_descriptors_node_t;
        for (uint32_t i = 0; i < sTlkMwCurImgHeader.fw_number; i++) {
            gTlk_fw_descriptors_node_t = tlkmw_get_fw_descpts_node_by_index(sTlkMwCurImgHeader.fw_descpts_list, i);

            if ((gTlk_fw_info_list_t->bin_type == gTlk_fw_descriptors_node_t->fw_type) && (gTlk_fw_info_list_t->bin_version == gTlk_fw_descriptors_node_t->fw_version) &&
                (gTlk_fw_descriptors_node_t->fw_type == bin_type)) {
                if (isSclectVersion) {
                    bin_version = gTlk_fw_descriptors_node_t->fw_version;
                } else {
                    binx_start_addr_offset = gTlk_fw_descriptors_node_t->start_addr;
                }

                break;
            }
        }
    }
    if (isSclectVersion) {
        return bin_version;
    } else {
        flash_start_addr_binx = (binx_start_addr_offset + sizeof(sTlk_cur_fw_descriptor_header_t) + TLKAPP_TINYSQL_TOTAL_IMAGE_ADDR) + FLASH_R_BASE_ADDR;
        return flash_start_addr_binx;
    }
#else
    (void)bin_type;
    (void)isSclectVersion;
    return 0;
#endif
}

/**
 * @brief      Get the startup address of N22 core from flash
 * @param[in]  none
 * @param[out] none
 * @return     uint32_t - N22 core startup address
 */
uint32_t tlkmw_getN22StartUpAddrFromFlash(void)
{
#if (MCU_DUAL_CORE_ENABLE)
    return tlkmw_getBinStartUpAddrFromFlash(BINX_N22, 0);
#else
    return 0;
#endif
}

/**
 * @brief      Get the startup address of DSP core from flash
 * @param[in]  none
 * @param[out] none
 * @return     uint32_t - DSP core startup address
 */
uint32_t tlkmw_getDSPStartUpAddrFromFlash(void)
{
#if (MCU_DUAL_CORE_ENABLE)
    return tlkmw_getBinStartUpAddrFromFlash(BINX_DSP, 0);
#else
    return 0;
#endif
}

uint32_t tlkmw_getBinVersion_d25f(void)
{
    return tlkmw_getBinStartUpAddrFromFlash(BINX_D25F, 1);
}


#if (TLKMW_RECORDING_CARD_EN && TLK_MW_USER_CTRL_ENABLE)

extern void         tlkapp_spi_io_power_on(void);
extern unsigned int calculate_CRC32(unsigned int crc, unsigned char *pStart, unsigned int uSize);

/**
 * @brief      Initialize customer OTA process
 * @param[in]  none
 * @param[out] none
 * @return     int - TLK_ENONE if success
 */
int tlkmw_ota_start_customer_init(void)
{
    tlkapp_spi_io_power_on();
    return TLK_ENONE;
}

/**
 * @brief      Handle special operations after OTA end acknowledgment
 * @param[in]  pData   - pointer to data buffer
 * @param[in]  dataLen - length of data
 * @param[in]  userArg - user argument
 * @param[out] none
 * @return     int - TLK_ENONE if success
 */
int tlkmw_ota_end_ack_deal_SpecialOperationsr(uint8_t *pData, uint16_t dataLen, void *userArg)
{
    (void)pData;
    (void)dataLen;
    (void)userArg;
    sTlk_total_fw_descriptors_t        *sTlk_total_fw_descriptors_s = NULL;
    extern sTlk_total_fw_descriptors_t *tlkmw_get_recvImgHeaderAddr(void);
    sTlk_total_fw_descriptors_s  = tlkmw_get_recvImgHeaderAddr();
    sTlkMwOta_t *sTlkMwOtaCtrl_t = NULL;
    sTlkMwOtaCtrl_t              = tlkmw_get_otaCtrl();
    if (sTlk_total_fw_descriptors_s->fw_descpts_list == NULL) {
        return TLK_EHANDLE;
    }
    if (sTlk_total_fw_descriptors_s->fw_number == 0) {
        return TLK_ENONE;
    }
    for (uint8_t index = 0; index < sTlk_total_fw_descriptors_s->fw_number; index++) {
        sTlk_fw_descriptors_node_t *fw_info = tlkmw_get_fw_descpts_node_by_index(sTlk_total_fw_descriptors_s->fw_descpts_list, index);
        if (fw_info == NULL) {
            return false;
        }
        if (fw_info->fw_type == BINX_CUSTOMER) //TODO
        {
            dfuOutImgCtrl.dfu_bin_addr      = fw_info->start_addr + sTlkMwOtaCtrl_t->backAddr;
            dfuOutImgCtrl.dfu_bin_crc.crc32 = 0;
            dfuOutImgCtrl.dfu_bin_size      = fw_info->fw_size - 32;
            dfuOutImgCtrl.dfu_bin_offset    = 0;
            dfuOutImgCtrl.dfu_bin_trs_end   = 0;
            dfuOutImgCtrl.dfu_wait_reboot   = 1;

            uint8_t pBuffer[4] = {0};
            uint8_t buffLen    = 0;
            pBuffer[buffLen++] = TLK_FIRMWARE_OTA_SUCCESS;
            pBuffer[buffLen++] = 0;
            tlkmw_ota_update_ota_status(pBuffer, buffLen, NULL);

            uint32_t crc_start_addr = dfuOutImgCtrl.dfu_bin_addr + dfuOutImgCtrl.dfu_bin_size;
            flash_dread(crc_start_addr, 32, (unsigned char *)&dfuOutImgCtrl.dfu_bin_crc);

            uint32_t img_crc                = 0xFFFFFFFF;
            img_crc                         = calculate_CRC32(img_crc, (uint8_t *)((dfuOutImgCtrl.dfu_bin_addr) + FLASH_R_BASE_ADDR), dfuOutImgCtrl.dfu_bin_size);
            dfuOutImgCtrl.dfu_bin_crc.crc32 = img_crc;
            extern void tlkapp_set_wifi_startDfu(void);
            tlkapp_set_wifi_startDfu();
            return 1;
        }
    }
    return TLK_ENONE;
}

/**
 * @brief      Test SPI OTA functionality
 * @param[in]  none
 * @param[out] none
 * @return     none
 */
void spi_ota_test()
{
    dfuOutImgCtrl.dfu_bin_addr      = 0x400000;
    dfuOutImgCtrl.dfu_bin_crc.crc32 = 0xffffff;
    dfuOutImgCtrl.dfu_bin_size      = 756129; //756000;
    dfuOutImgCtrl.dfu_bin_offset    = 0;
    dfuOutImgCtrl.dfu_bin_trs_end   = 0;
    dfuOutImgCtrl.dfu_wait_reboot   = 1;
    //	tlk_printf("fw_info->start_addr = %08x,backaddr = %08x",fw_info->start_addr,sTlkMwOtaCtrl_t->backAddr);

    uint8_t pBuffer[4] = {0};
    uint8_t buffLen    = 0;
    pBuffer[buffLen++] = TLK_FIRMWARE_OTA_SUCCESS;
    pBuffer[buffLen++] = 0;
    tlkmw_ota_update_ota_status(pBuffer, buffLen, NULL);

    // uint32_t crc_start_addr = dfuOutImgCtrl.dfu_bin_addr + dfuOutImgCtrl.dfu_bin_size -32;
    // flash_dread(crc_start_addr, 32, (unsigned char*)&dfuOutImgCtrl.dfu_bin_crc);
    tlk_printf("bin_addr = %08x  crc = %08x  size = %08x", dfuOutImgCtrl.dfu_bin_addr, dfuOutImgCtrl.dfu_bin_crc.crc32, dfuOutImgCtrl.dfu_bin_size);
    extern void tlkapp_set_wifi_startDfu(void);
    tlkapp_set_wifi_startDfu();
}


#endif

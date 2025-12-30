/********************************************************************************************************
 * @file    sdmmc.c
 *
 * @brief   This is the source file for tl751x
 *
 * @author  Driver Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
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
#include "sdmmc.h"
#include "driver.h"
#include "lib/include/clock.h"
#include "error_handler/error_handler.h"
#include <stdbool.h>

static volatile unsigned char cmd_done = 0;
static volatile unsigned char transport_done = 0;
static volatile unsigned int timeout_status = 0;

/**
 * @brief      This function multiplexed sdmmc pins.
 * @param[in]  bus_clk - sdmmc bus clock initialization pointer.
 * @return     none
 */
void sdmmc_set_bus_clk_init(sdmmc_bus_clk_t *bus_clk)
{
    if (bus_clk == NULL)
    {
        return;
    }
    reg_sdio_clk_set &= ~(FLD_SDIO_CLK_MOD | FLD_SDIO_DIV_IN_SEL);
    reg_sdio_clk_set |= MASK_VAL(FLD_SDIO_DIV_IN_SEL, bus_clk->clock_src >> 4, FLD_SDIO_CLK_MOD, bus_clk->clock_div);
}

/**
 * @brief      This function multiplexed sdmmc pins.
 * @param[in]  pin       - the selected pin.
 * @param[in]  function  - functions of multiplexed pins.
 * @return     none
 */
void sdmmc_set_pin_mux(gpio_func_pin_e pin, gpio_func_e function)
{
    if ((gpio_func_pin_e)pin != GPIO_NONE_PIN)
    {
        gpio_input_en((gpio_pin_e)pin);
        reg_gpio_func_mux(pin) = function;
        gpio_set_up_down_res((gpio_pin_e)pin, GPIO_PIN_UP_DOWN_FLOAT);
        gpio_function_dis((gpio_pin_e)pin);
    }
}

/**
 * @brief      This function servers to set sdmmc pin.
 * @param[in]  sdmmc_pin_config  - the pointer of pin config struct.
 * @return     none
 */
void sdmmc_set_pin(sdmmc_pin_config_t *sdmmc_pin_config)
{
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_clk_pin, SDMMC_CK_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_cmd_pin, SDMMC_CMD_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_ds_pin, SDMMC_DS_I);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_dat0_pin, SDMMC_DAT0_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_dat1_pin, SDMMC_DAT1_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_dat2_pin, SDMMC_DAT2_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_dat3_pin, SDMMC_DAT3_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_dat4_pin, SDMMC_DAT4_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_dat5_pin, SDMMC_DAT5_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_dat6_pin, SDMMC_DAT6_IO);
    sdmmc_set_pin_mux(sdmmc_pin_config->sdmmc_dat7_pin, SDMMC_DAT7_IO);
}

void sdmmc_set_reset_pin(gpio_pin_e pin, unsigned int delay_num)
{
    gpio_input_dis(pin);
    gpio_output_en(pin);
    gpio_set_low_level(pin);
    delay_us(delay_num);
    gpio_set_high_level(pin);
    delay_us(delay_num);
}

__attribute__((weak))
void sdmmc_wait_cmd_done(void)
{
    unsigned int timeout = 0;
    while (!cmd_done) {
        timeout++;
        if (timeout > 0xFFFFFFF) {
            break;
        }
    }
    cmd_done = 0;
}

__attribute__((weak))
void sdmmc_wait_transport_done(void)
{
    unsigned int timeout = 0;
    while (!transport_done) {
        if (timeout_status) {
            timeout_status = 0;
            drv_timeout_handler(DRV_API_ERROR_TIMEOUT_SDMMC_CLK_WAIT);
            break;
        }
        timeout++;
        if (timeout > 0xFFFFFFF) {
            break;
        }
    }
    transport_done = 0;
}

/**
 * @brief      This function servers to cyclic detection of sdmmc card status.
 * @return     none
 */
void sdmmc_wait_card_detection(void)
{
    bool status = 0;
    volatile unsigned int timeout = 0;

    while (!status)
    {
        timeout++;
        status = sdmmc_get_card_insertion_status();
        if (timeout > 0xFFFFFFF)
        {
            break;
        }
    }
    sdmmc_set_card_insertion_status();
}

/**
 * @brief      This function servers to setting the direction of data transfer for sdmmc.
 * @param[in]  data_dir  - sdmmc data transfer direction.
 * @return     none
 */
void sdmmc_set_data_xfr_dir(sdmmc_data_dir_e data_dir)
{
    volatile unsigned char reg_val;

    reg_val = reg_sdmmc_cmd_transfer_mode_byte;

    if (data_dir == HOST_TO_CARD)
    {
        BM_CLR(reg_val, FLD_DATA_XFR_DIR_SELECT);
    }
    else if (data_dir == CARD_TO_HOST)
    {
        BM_SET(reg_val, FLD_DATA_XFR_DIR_SELECT);
    }
    reg_sdmmc_cmd_transfer_mode_byte = reg_val;
}

/**
 * @brief      This function servers to sets the sdmmc bounding size of buf.
 * @param[in]  size  - bounding size.
 * @return     none
 */
void sdmmc_set_buf_boundary(sdmmc_buf_boundary_size_e size)
{
    volatile unsigned int reg_val = 0;

    reg_val |= reg_blk_ctrl;
    reg_val &= ~FLD_BUUFER_BOUNDARY;
    reg_val |= MASK_VAL(FLD_BUUFER_BOUNDARY, size);
    reg_blk_ctrl = reg_val;
}

/**
 * @brief      This function servers to sets the bounding size of buf.
 * @param[in]  size  - bounding size.
 * @return     none
 */
void sdmmc_set_data_block_size(unsigned short num_byte)
{
    volatile unsigned int reg_val = 0;

    reg_val |= reg_blk_ctrl;
    reg_val &= ~FLD_BLOCK_SIZE_REGISTER;
    reg_val |= MASK_VAL(FLD_BLOCK_SIZE_REGISTER, num_byte);
    reg_blk_ctrl = reg_val;
}

/**
 * @brief      This function servers to setting the sdmmc auto command enable.
 * @param[in]  size  - bounding size.
 * @return     none
 */
void sdmmc_auto_cmd_en(sdmmc_auto_cmd_e auto_cmd)
{
    volatile unsigned int reg_val;

    reg_val = reg_sdmmc_cmd_transfer_mode;
    reg_val &= ~FLD_AUTO_CMD_EN;
    reg_val |= MASK_VAL(FLD_AUTO_CMD_EN, auto_cmd);
    reg_sdmmc_cmd_transfer_mode = reg_val;
}

bool sdmmc_clk_ready(void)
{
    return !(reg_sdmmc_irq_status_1 & FLD_CLOCK_READY_STS);
}

bool sdmmc_internal_clk_ready(void)
{
    return !(reg_sdmmc_clock_timeout_control & FLD_INTERNAL_CLK_STABLE);
}

/**
 * @brief      This function servers to setting block count enable for sdma.
 * @param[in]  num  - numerical reg_val.
 * @return     none
 */
void sdmmc_set_block_count(unsigned short num)
{
    volatile unsigned int reg_val = 0;

    reg_val |= reg_blk_ctrl;
    reg_val &= ~FLD_BLOCK_COUNTREGISTER;
    reg_val |= MASK_VAL(FLD_BLOCK_COUNTREGISTER, num);
    reg_blk_ctrl = reg_val;
}

/**
 * @brief      This function servers to setting the clock division factor for sdmmc.
 * @param[in]  div  - clock division factor.
 * @return     none
 */
void sdmmc_set_clk_div(sdmmc_module_clock_div_e div)
{
    volatile unsigned int reg_val;

    reg_val = reg_sdmmc_clock_timeout_control;
    reg_val &= ~FLD_SDCLK_FREQ_SELECT0;
    reg_val &= ~FLD_SDCLK_FREQ_SELECT1;
    reg_val |= MASK_VAL(FLD_SDCLK_FREQ_SELECT0, (div >> 8) & 0x03);
    reg_val |= MASK_VAL(FLD_SDCLK_FREQ_SELECT1, div & 0xFF);
    reg_sdmmc_clock_timeout_control = reg_val;
}

/**
 * @brief      This function servers to setting the communication data bit width of sdmmc.
 * @param[in]  data_width  - sdmmc communication data bit width.
 * @return     none
 */
void sdmmc_set_data_width(sdmmc_data_width_e data_width)
{
    switch (data_width)
    {
    case DATA_WIDTH_1BIT:
        BM_CLR(reg_sdmmc_host_ctrl_b0, FLD_DATA_TRANSFER_WIDTH);
        BM_CLR(reg_sdmmc_host_ctrl_b0, FLD_EXTENDED_DATA_TRANSFER_WIDTH);
        break;
    case DATA_WIDTH_4BIT:
        BM_SET(reg_sdmmc_host_ctrl_b0, FLD_DATA_TRANSFER_WIDTH);
        BM_CLR(reg_sdmmc_host_ctrl_b0, FLD_EXTENDED_DATA_TRANSFER_WIDTH);
        break;
    case DATA_WIDTH_8BIT:
        BM_CLR(reg_sdmmc_host_ctrl_b0, FLD_DATA_TRANSFER_WIDTH);
        BM_SET(reg_sdmmc_host_ctrl_b0, FLD_EXTENDED_DATA_TRANSFER_WIDTH);
        break;
    default:
        break;
    }
}

/**
 * @brief      This function servers to setting up the sdmmc sampling configuration.
 * @return     none
 */
void sdmmc_set_sampling_edge(sdmmc_sampe_edge_t *sampe_edge)
{
    if (sampe_edge == NULL)
    {
        return;
    }
    sdmmc_set_cmd_sampling_edge(sampe_edge->cmd_edge);
    sdmmc_set_read_data_sampling_edge(sampe_edge->read_data_edge);
    sdmmc_set_write_data_sampling_edge(sampe_edge->write_data_edge);
    sdmmc_set_resp_start_sampling_edge(sampe_edge->resp_start_edge);
}

/**
 * @brief      This function servers to setting the configuration information sent by the sdmmc regular command.
 * @param[in]  sdmmc_send_command_info  - configuration information structure sent by the sdmmc regular command.
 * @return     none
 */
void sdmmc_send_cmd(sdmmc_send_cmd_info_t *cmd_info)
{
    volatile unsigned int reg_val;

    sdmmc_set_cmd_arg(cmd_info->argument.arg);

    reg_val = reg_sdmmc_cmd_transfer_mode_b1;

    reg_val &= ~FLD_RSP_TYPE_SELECT_B1;
    reg_val &= ~FLD_SUB_CMD_FLAG_B1;
    reg_val &= ~FLD_CMD_CRC_CHECK_EN_B1;
    reg_val &= ~FLD_CMD_INDEX_CHENK_EN_B1;
    reg_val &= ~FLD_DATA_PRESENT_SELECT_B1;
    reg_val &= ~FLD_CMD_TYPE_B1;
    reg_val &= ~FLD_CMD_INDEX_B1;
    reg_val &= ~FLD_RESERVED_CMD_B1;

    reg_val |= cmd_info->cmd_type << CMD_TYPE_POS;
    reg_val |= cmd_info->cmd_index << CMD_INDEX_POS;
    reg_val |= cmd_info->sub_cmd_flag << SUB_CMD_FLAG_POS;
    reg_val |= cmd_info->crc_check_en << CMD_CRC_CHECK_EN_POS;
    reg_val |= cmd_info->sdmmc_resport << RSP_TYPE_SELECT_POS;
    reg_val |= cmd_info->cmd_index_check_en << CMD_INDEX_CHENK_EN_POS;
    reg_val |= cmd_info->data_present_select_en << DATA_PRESENT_SELECT_POS;
    reg_sdmmc_cmd_transfer_mode_b1 = reg_val;
}

void sdmmc_cmd1_wait_ready(sdmmc_send_cmd_info_t *cmd_info)
{
    unsigned int timeout = 0;
    unsigned char really_state = 0;
    volatile unsigned int reg_val;
    while (!really_state)
    {
        timeout++;
        sdmmc_send_cmd(cmd_info);
        sdmmc_wait_cmd_done();
        reg_val = reg_sdmmc_response_0;
        really_state = (reg_val >> 31) & 0x01;
        if (timeout > 0xFFFFFFFF)
        {
            break;
        }
    }
}

unsigned int sdmmc_write_block_data(unsigned int *buf_addr, unsigned int block_size)
{
    if (buf_addr == NULL) {
        return 0;
    }

    while (!(sdmmc_get_write_read_status() & FLD_WRITE_BUFFER_EN)) /* buffer_en means one block can be write. */
        ;
    for (unsigned int i = 0; i < (block_size + 3) >> 2; i++) {
        reg_sdmmc_data_port = *buf_addr;
        buf_addr++;
    }

    return block_size;
}

unsigned int sdmmc_read_block_data(unsigned int *buf_addr, unsigned int block_size)
{
    if (buf_addr == NULL) {
        return 0;
    }

    while (!(sdmmc_get_write_read_status() & FLD_READ_BUFFER_EN)) /* buffer_en means one block can be read. */
        ;
    for (unsigned int i = 0; i < (block_size + 3) >> 2; i++) {
        *buf_addr = reg_sdmmc_data_port;
        buf_addr++;
    }

    return block_size;
}

////得到卡信息
////cardinfo:卡信息存储区
////返回值:错误状态
// sdmmc_error_e sdmmc_get_sdcardinfo(sd_card_info *cardinfo,unsigned int* CSD_Tab, unsigned int* CID_Tab,unsigned char CardType,unsigned short RCA)
//{
//     sdmmc_error_e errorstatus=SD_OK;
//     unsigned char tmp=0;
//     cardinfo->CardType=(unsigned char)CardType;                 //卡类型
//     cardinfo->RCA=(unsigned short)RCA;                          //卡RCA值
//     tmp=(unsigned char)((CSD_Tab[0]&0xFF000000)>>24);
//     cardinfo->sd_csd.CSDStruct=(tmp&0xC0)>>6;       //CSD结构
//     cardinfo->sd_csd.SysSpecVersion=(tmp&0x3C)>>2;  //2.0协议还没定义这部分(为保留),应该是后续协议定义的
//     cardinfo->sd_csd.Reserved1=tmp&0x03;            //2个保留位
//     tmp=(unsigned char)((CSD_Tab[0]&0x00FF0000)>>16);           //第1个字节
//     cardinfo->sd_csd.TAAC=tmp;                      //数据读时间1
//     tmp=(unsigned char)((CSD_Tab[0]&0x0000FF00)>>8);            //第2个字节
//     cardinfo->sd_csd.NSAC=tmp;                      //数据读时间2
//     tmp=(unsigned char)(CSD_Tab[0]&0x000000FF);             //第3个字节
//     cardinfo->sd_csd.MaxBusClkFrec=tmp;             //传输速度
//     tmp=(unsigned char)((CSD_Tab[1]&0xFF000000)>>24);           //第4个字节
//     cardinfo->sd_csd.CardComdClasses=tmp<<4;        //卡指令类高四位
//     tmp=(unsigned char)((CSD_Tab[1]&0x00FF0000)>>16);           //第5个字节
//     cardinfo->sd_csd.CardComdClasses|=(tmp&0xF0)>>4;//卡指令类低四位
//     cardinfo->sd_csd.RdBlockLen=tmp&0x0F;           //最大读取数据长度
//     tmp=(unsigned char)((CSD_Tab[1]&0x0000FF00)>>8);            //第6个字节
//     cardinfo->sd_csd.PartBlockRead=(tmp&0x80)>>7;   //允许分块读
//     cardinfo->sd_csd.WrBlockMisalign=(tmp&0x40)>>6; //写块错位
//     cardinfo->sd_csd.RdBlockMisalign=(tmp&0x20)>>5; //读块错位
//     cardinfo->sd_csd.DSRImpl=(tmp&0x10)>>4;
//     cardinfo->sd_csd.Reserved2=0;                   //保留
//
//     tmp=(unsigned char)(CSD_Tab[1]&0x000000FF);         //第7个字节
//     cardinfo->sd_csd.DeviceSize=(tmp&0x3F)<<16;//C_SIZE
//     tmp=(unsigned char)((CSD_Tab[2]&0xFF000000)>>24);   //第8个字节
//     cardinfo->sd_csd.DeviceSize|=(tmp<<8);
//     tmp=(unsigned char)((CSD_Tab[2]&0x00FF0000)>>16);   //第9个字节
//     cardinfo->sd_csd.DeviceSize|=(tmp);
//     tmp=(unsigned char)((CSD_Tab[2]&0x0000FF00)>>8);    //第10个字节
//     cardinfo->CardCapacity=(long long)(cardinfo->sd_csd.DeviceSize+1)*512*1024;//计算卡容量
//     cardinfo->CardBlockSize=512;            //块大小固定为512字节
//
//     cardinfo->sd_csd.EraseGrSize=(tmp&0x40)>>6;
//     cardinfo->sd_csd.EraseGrMul=(tmp&0x3F)<<1;
//     tmp=(unsigned char)(CSD_Tab[2]&0x000000FF);         //第11个字节
//     cardinfo->sd_csd.EraseGrMul|=(tmp&0x80)>>7;
//     cardinfo->sd_csd.WrProtectGrSize=(tmp&0x7F);
//     tmp=(unsigned char)((CSD_Tab[3]&0xFF000000)>>24);       //第12个字节
//     cardinfo->sd_csd.WrProtectGrEnable=(tmp&0x80)>>7;
//     cardinfo->sd_csd.ManDeflECC=(tmp&0x60)>>5;
//     cardinfo->sd_csd.WrSpeedFact=(tmp&0x1C)>>2;
//     cardinfo->sd_csd.MaxWrBlockLen=(tmp&0x03)<<2;
//     tmp=(unsigned char)((CSD_Tab[3]&0x00FF0000)>>16);       //第13个字节
//     cardinfo->sd_csd.MaxWrBlockLen|=(tmp&0xC0)>>6;
//     cardinfo->sd_csd.WriteBlockPaPartial=(tmp&0x20)>>5;
//     cardinfo->sd_csd.Reserved3=0;
//     cardinfo->sd_csd.ContentProtectAppli=(tmp&0x01);
//     tmp=(unsigned char)((CSD_Tab[3]&0x0000FF00)>>8);        //第14个字节
//     cardinfo->sd_csd.FileFormatGrouop=(tmp&0x80)>>7;
//     cardinfo->sd_csd.CopyFlag=(tmp&0x40)>>6;
//     cardinfo->sd_csd.PermWrProtect=(tmp&0x20)>>5;
//     cardinfo->sd_csd.TempWrProtect=(tmp&0x10)>>4;
//     cardinfo->sd_csd.FileFormat=(tmp&0x0C)>>2;
//     cardinfo->sd_csd.ECC=(tmp&0x03);
//     tmp=(unsigned char)(CSD_Tab[3]&0x000000FF);         //第15个字节
//     cardinfo->sd_csd.CSD_CRC=(tmp&0xFE)>>1;
//     cardinfo->sd_csd.Reserved4=1;
//     tmp=(unsigned char)((CID_Tab[0]&0xFF000000)>>24);       //第0个字节
//     cardinfo->sd_cid.ManufacturerID=tmp;
//     tmp=(unsigned char)((CID_Tab[0]&0x00FF0000)>>16);       //第1个字节
//     cardinfo->sd_cid.OEM_AppliID=tmp<<8;
//     tmp=(unsigned char)((CID_Tab[0]&0x000000FF00)>>8);      //第2个字节
//     cardinfo->sd_cid.OEM_AppliID|=tmp;
//     tmp=(unsigned char)(CID_Tab[0]&0x000000FF);         //第3个字节
//     cardinfo->sd_cid.ProdName1=tmp<<24;
//     tmp=(unsigned char)((CID_Tab[1]&0xFF000000)>>24);       //第4个字节
//     cardinfo->sd_cid.ProdName1|=tmp<<16;
//     tmp=(unsigned char)((CID_Tab[1]&0x00FF0000)>>16);       //第5个字节
//     cardinfo->sd_cid.ProdName1|=tmp<<8;
//     tmp=(unsigned char)((CID_Tab[1]&0x0000FF00)>>8);        //第6个字节
//     cardinfo->sd_cid.ProdName1|=tmp;
//     tmp=(unsigned char)(CID_Tab[1]&0x000000FF);         //第7个字节
//     cardinfo->sd_cid.ProdName2=tmp;
//     tmp=(unsigned char)((CID_Tab[2]&0xFF000000)>>24);       //第8个字节
//     cardinfo->sd_cid.ProdRev=tmp;
//     tmp=(unsigned char)((CID_Tab[2]&0x00FF0000)>>16);       //第9个字节
//     cardinfo->sd_cid.ProdSN=tmp<<24;
//     tmp=(unsigned char)((CID_Tab[2]&0x0000FF00)>>8);        //第10个字节
//     cardinfo->sd_cid.ProdSN|=tmp<<16;
//     tmp=(unsigned char)(CID_Tab[2]&0x000000FF);             //第11个字节
//     cardinfo->sd_cid.ProdSN|=tmp<<8;
//     tmp=(unsigned char)((CID_Tab[3]&0xFF000000)>>24);       //第12个字节
//     cardinfo->sd_cid.ProdSN|=tmp;
//     tmp=(unsigned char)((CID_Tab[3]&0x00FF0000)>>16);       //第13个字节
//     cardinfo->sd_cid.Reserved1|=(tmp&0xF0)>>4;
//     cardinfo->sd_cid.ManufactDate=(tmp&0x0F)<<8;
//     tmp=(unsigned char)((CID_Tab[3]&0x0000FF00)>>8);        //第14个字节
//     cardinfo->sd_cid.ManufactDate|=tmp;
//     tmp=(unsigned char)(CID_Tab[3]&0x000000FF);         //第15个字节
//     cardinfo->sd_cid.CID_CRC=(tmp&0xFE)>>1;
//     cardinfo->sd_cid.Reserved2=1;
//     return errorstatus;
// }

void sdmmc_timeout_init(sdmmc_timerout_config_t *config)
{
    if (config == NULL)
    {
        return;
    }
    sdmmc_set_timeout_clk_unit(config->base_clk_unit);
    sdmmc_set_timeout_clk_val(config->clk_num);
}

void sdmmc_single_block_trans_init(void)
{
    volatile unsigned char reg_val;

    reg_val = reg_sdmmc_cmd_transfer_mode_byte;
    reg_val &= ~FLD_DMA_BLOCK_COUNT_EN;
    reg_val &= ~FLD_BLOCK_SELECT;
    reg_sdmmc_cmd_transfer_mode_byte = reg_val;
}

void sdmmc_multiple_block_trans_init(void)
{
    volatile unsigned char reg_val;

    reg_val = reg_sdmmc_cmd_transfer_mode_byte;
    reg_val |= FLD_DMA_BLOCK_COUNT_EN;
    reg_val |= FLD_BLOCK_SELECT;
    reg_sdmmc_cmd_transfer_mode_byte = reg_val;
}

void sdmmc_block_trans_init(sdmmc_dma_config_t *config)
{
    bool dma_en;
    sdmmc_dma_select_e dma_selec;
    volatile unsigned char reg_val;

    if (config == NULL)
    {
        return;
    }
    dma_en = config->dma_en;
    dma_selec = config->dma_select;
    reg_val = reg_sdmmc_cmd_transfer_mode_byte;

    if (dma_en)
    {
        reg_val |= FLD_DMA_BLOCK_COUNT_EN;
    }
    else
    {
        reg_val &= ~FLD_DMA_BLOCK_COUNT_EN;
    }
    reg_val |= FLD_REP_ERR_CHECK_EN;
    reg_val &= ~FLD_DMA_BLOCK_COUNT_EN;
    reg_val &= ~FLD_DATA_XFR_DIR_SELECT;
    reg_val &= ~FLD_BLOCK_SELECT;
    reg_val &= ~FLD_REP_TYPE_R1_R15;
    reg_val &= ~FLD_RSP_INTERRUPT_DIS;
    reg_val &= ~FLD_RESERVED_FOR_XFR_MODE;

    switch (dma_selec)
    {
    case SDMA:
        (dma_en == true) ? (reg_val |= FLD_DMA_EN) : (reg_val &= ~FLD_DMA_EN);
        break;
    case ADMA2:
        break;
    case ADMA3:
        break;
    default:
        break;
    }

    reg_sdmmc_cmd_transfer_mode_byte = reg_val;
}

void sdmmc_core_init(sdmmc_config_t *config)
{
    if (config == NULL)
    {
        return;
    }
    sdmmc_set_pin(config->pin_config);
    sdmmc_set_bus_clk_init(config->bus_clk);
    sdmmc_set_sampling_edge(config->sampe_edge);
    sdmmc_set_cap_slot_type(config->slot_type);
    sdmmc_timeout_init(config->timerout_config);
    sdmmc_set_clk_div(60);
    sdmmc_internal_clk_en();
    sdmmc_host_clk_en();
    sdmmc_wait_clk_ready();
    sdmmc_wait_internal_clk_ready();
}

void sdmmc_data_trans_init(sdmmc_trans_config_t *config)
{
    if (config == NULL)
    {
        return;
    }
    sdmmc_auto_cmd_en(config->auto_cmd);
    sdmmc_set_buf_boundary(BUF_SIZE_512KB);
    sdmmc_set_data_width(config->data_width);
    sdmmc_block_trans_init(config->dma_config);
    sdmmc_set_data_block_size(config->block_size);
}

void sdmmc_cmd_transfer(sdmmc_send_cmd_info_t *cmd_info)
{

    if (cmd_info == NULL)
    {
        return;
    }
    if (cmd_info->cmd_index == HOST_SEND_OP_COND)
    {
        sdmmc_cmd1_wait_ready(cmd_info);
    }
    else
    {
        while (!sdmmc_wait_data_ready(DATA0))
            ;
        sdmmc_send_cmd(cmd_info);
        sdmmc_wait_cmd_done();
    }
}

void sdmmc_cmd_data_transfer(data_transfer_t *transfer)
{
    sdmmc_data_dir_e dir = 0x00;
    unsigned int i;
    unsigned int num = 0x00;
    unsigned int block_num = 0x00;
    if (transfer == NULL)
    {
        return;
    }

    while (!sdmmc_wait_data_ready(DATA0))
        ;
    if ((transfer->cmd_info->cmd_index == HOST_WRITE_MULTIPLE_BLOCK) || (transfer->cmd_info->cmd_index == HOST_READ_MULTIPLE_BLOCK))
    {
        if (transfer->block_num > 1)
        {
            num = transfer->block_num;
            block_num = transfer->block_num;
        }
        else
        {
            return;
        }
    }
    else if ((transfer->cmd_info->cmd_index == HOST_WRITE_BLOCK) || (transfer->cmd_info->cmd_index == HOST_READ_SINGLE_BLOCK))
    {
        if (transfer->block_num == 1)
        {
            num = 0;
            block_num = transfer->block_num;
        }
        else
        {
            return;
        }
    }
    else if(transfer->cmd_info->cmd_index == HOST_SEND_EXT_CSD){
        num = 0;
        block_num = transfer->block_num;
    }
    sdmmc_set_block_count(num);
    if ((transfer->cmd_info->cmd_index == HOST_READ_SINGLE_BLOCK) || (transfer->cmd_info->cmd_index == HOST_READ_MULTIPLE_BLOCK)||(transfer->cmd_info->cmd_index == HOST_SEND_EXT_CSD))
    {
        dir = CARD_TO_HOST;
    }
    else if ((transfer->cmd_info->cmd_index == HOST_WRITE_BLOCK) || (transfer->cmd_info->cmd_index == HOST_WRITE_MULTIPLE_BLOCK))
    {
        dir = HOST_TO_CARD;
    }
    if ((transfer->cmd_info->cmd_index == HOST_READ_SINGLE_BLOCK) || (transfer->cmd_info->cmd_index == HOST_WRITE_BLOCK)||(transfer->cmd_info->cmd_index == HOST_SEND_EXT_CSD))

    {
        sdmmc_single_block_trans_init();
    }
    else if ((transfer->cmd_info->cmd_index == HOST_READ_MULTIPLE_BLOCK) || (transfer->cmd_info->cmd_index == HOST_WRITE_MULTIPLE_BLOCK))
    {
        sdmmc_multiple_block_trans_init();
    }

    sdmmc_set_data_xfr_dir(dir);
    if (transfer->dma_config->dma_en == false)
    {
        sdmmc_send_cmd(transfer->cmd_info);
        sdmmc_wait_cmd_done();
        if (dir == CARD_TO_HOST)
        {
            for (i = 0x00; i < block_num; i++)
            {
                sdmmc_read_block_data((unsigned int *)(transfer->buf_addr.src + transfer->block_size * i), transfer->block_size);
            }
        }
        else if (dir == HOST_TO_CARD)
        {
            for (i = 0x00; i < block_num; i++)
            {
                sdmmc_write_block_data((unsigned int *)(transfer->buf_addr.dest + transfer->block_size * i), transfer->block_size);
            }
        }
    }
    else if ((transfer->dma_config->dma_en == true) && (transfer->dma_config->dma_select == SDMA))
    {
        sdmmc_set_sdma_addr((unsigned int)transfer->buf_addr.dest);
        sdmmc_send_cmd(transfer->cmd_info);
        sdmmc_wait_cmd_done();
    }
    sdmmc_wait_transport_done();
}

__attribute__((weak))
void sdio_irq_handler(void)
{
    unsigned int status;
    status = reg_sdmmc_irq_sts & reg_sdmmc_cq_intr_sts_en;

    if (status & SDMMC_INTR_CMD_COMPLETE_STS) {
        sdmmc_set_clear_irq_status(SDMMC_INTR_CMD_COMPLETE_STS);
        cmd_done = 1;
    }

    if (status & SDMMC_INTR_TRANSFER_COMPLETE_STS) {
        sdmmc_set_clear_irq_status(SDMMC_INTR_TRANSFER_COMPLETE_STS);
        transport_done = 1;
    }

    if (status & SDMMC_INTR_DATA_TIMEOUT_ERR_STS) {
        sdmmc_set_clear_irq_status(SDMMC_INTR_DATA_TIMEOUT_ERR_STS);
        timeout_status = 1;
    }
}
// PLIC_ISR_REGISTER(sdio_irq_handler, IRQ_SDIO)

/********************************************************************************************************
 * @file    i3c.c
 *
 * @brief   This is the source file for tl322x
 *
 * @author  Driver Group
 * @date    2025
 *
 * @par     Copyright (c) 2025, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "i3c.h"

volatile unsigned char g_dev_count[2] ={0};
volatile i3c_device_info_t g_dev_list[2][I3C_MAX_DEVCNT]; /*!< I3C slave record list */

static unsigned int g_enabled_ints =0;
static unsigned char buff[1024];

static unsigned char i3c_dma_tx_chn[2];
static unsigned char i3c_dma_rx_chn[2];
dma_config_t i3c_tx_dma_config[2] = {
    {
        .dst_req_sel = DMA_REQ_I3C0_TX, // tx req
        .src_req_sel = 0,
        .dst_addr_ctrl = DMA_ADDR_FIX,
        .src_addr_ctrl = DMA_ADDR_INCREMENT,
        .dstmode = DMA_HANDSHAKE_MODE,
        .srcmode = DMA_NORMAL_MODE,
        .dstwidth = DMA_CTR_WORD_WIDTH,
        .srcwidth = DMA_CTR_WORD_WIDTH,
        .src_burst_size = 0,
        .read_num_en = 0,
        .priority = 0,
        .write_num_en = 0,
        .auto_en = 0, // must be 0
    },
   {
        .dst_req_sel = DMA_REQ_I3C1_TX, // tx req
        .src_req_sel = 0,
        .dst_addr_ctrl = DMA_ADDR_FIX,
        .src_addr_ctrl = DMA_ADDR_INCREMENT,
        .dstmode = DMA_HANDSHAKE_MODE,
        .srcmode = DMA_NORMAL_MODE,
        .dstwidth = DMA_CTR_WORD_WIDTH,
        .srcwidth = DMA_CTR_WORD_WIDTH,
        .src_burst_size = 0,
        .read_num_en = 0,
        .priority = 0,
        .write_num_en = 0,
        .auto_en = 0, // must be 0
    },
};
dma_config_t i3c_rx_dma_config[2] = {
    {
        .dst_req_sel = 0, // tx req
        .src_req_sel = DMA_REQ_I3C0_RX,
        .dst_addr_ctrl = DMA_ADDR_INCREMENT,
        .src_addr_ctrl = DMA_ADDR_FIX,
        .dstmode = DMA_NORMAL_MODE,
        .srcmode = DMA_HANDSHAKE_MODE,
        .dstwidth = DMA_CTR_BYTE_WIDTH,
        .srcwidth = DMA_CTR_BYTE_WIDTH,
        .src_burst_size = 0,
        .read_num_en = 0,
        .priority = 0,
        .write_num_en = 1,
        .auto_en = 0, // must be 0
    },
    {
        .dst_req_sel = 0, // tx req
        .src_req_sel = DMA_REQ_I3C1_RX,
        .dst_addr_ctrl = DMA_ADDR_INCREMENT,
        .src_addr_ctrl = DMA_ADDR_FIX,
        .dstmode = DMA_NORMAL_MODE,
        .srcmode = DMA_HANDSHAKE_MODE,
        .dstwidth = DMA_CTR_BYTE_WIDTH,
        .srcwidth = DMA_CTR_BYTE_WIDTH,
        .src_burst_size = 0,
        .read_num_en = 0,
        .priority = 0,
        .write_num_en = 1,
        .auto_en = 0, // must be 0
    },
};

/**
 * @brief     This function serves to set i3c tx_dma channel.
 * @param[in] chn     - i3c_chn_e.
 * @param[in] dma_chn -  dma channel.
 * @return    none
 */
void i3c_set_tx_dma_config(i3c_chn_e chn,dma_chn_e dma_chn){
    i3c_dma_tx_chn[chn] = dma_chn;
    dma_config(dma_chn, &i3c_tx_dma_config[chn]);
}

/**
 * @brief     This function serves to set i3c rx_dma channel.
 * @param[in] chn     - i3c_chn_e.
 * @param[in] dma_chn -  dma channel.
 * @return    none
 */
void i3c_set_rx_dma_config(i3c_chn_e chn,dma_chn_e dma_chn){
    i3c_dma_rx_chn[chn] = dma_chn;
    dma_config(dma_chn, &i3c_rx_dma_config[chn]);
}

/**********************************************************master ********************************************/
/**
 * @brief      This function selects a pin port for I3C master interface.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  sda_pin        - the pin port selected as I3C sda pin port.
 * @param[in]  scl_pin        - the pin port selected as I3C scl pin port.
 * @param[in]  sda_pullup_pin - the pin port selected as I3C pullup pin port.
 * @return     none
 */
void i3c_master_set_pin(i3c_chn_e chn,gpio_func_pin_e sda_pin,gpio_func_pin_e scl_pin,gpio_func_pin_e sda_pullup_pin)
{
     //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    gpio_input_en((gpio_pin_e)sda_pin); //enable sda input
    gpio_input_en((gpio_pin_e)scl_pin); //enable scl input
    gpio_set_up_down_res((gpio_pin_e)sda_pin, GPIO_PIN_PULLUP_10K);
    gpio_set_up_down_res((gpio_pin_e)scl_pin, GPIO_PIN_PULLUP_10K);
    if(chn == I3C0){
        gpio_set_mux_function(scl_pin,I3C0_SCL_IO);
        gpio_set_mux_function(sda_pin,I3C0_SDA_IO);
        if(sda_pullup_pin != GPIO_NONE_PIN){
            gpio_set_mux_function(sda_pullup_pin,I3C0_SDA_PULLUP_EN);
        }
        //disable sda_pin and scl_pin gpio function.
        gpio_function_dis((gpio_pin_e)scl_pin);
        gpio_function_dis((gpio_pin_e)sda_pin);
        if(sda_pullup_pin != GPIO_NONE_PIN){
           gpio_function_dis((gpio_pin_e)sda_pullup_pin);
        }
    }else if(chn == I3C1){
        gpio_set_mux_function(scl_pin,I3C1_SCL_IO);
        gpio_set_mux_function(sda_pin,I3C1_SDA_IO);
        if(sda_pullup_pin != GPIO_NONE_PIN){
           gpio_set_mux_function(sda_pullup_pin,I3C1_SDA_PULLUP_EN);
        }
        //disable sda_pin and scl_pin gpio function.
        gpio_function_dis((gpio_pin_e)scl_pin);
        gpio_function_dis((gpio_pin_e)sda_pin);
        if(sda_pullup_pin != GPIO_NONE_PIN){
           gpio_function_dis((gpio_pin_e)sda_pullup_pin);
        }
    }

}

static unsigned int i3c_calc_error_ratio(unsigned int cur_freq, unsigned int desired_freq){
    if(cur_freq > desired_freq){
        return (cur_freq - desired_freq) * 100 /desired_freq;
    }else{
         return (desired_freq - cur_freq) * 100 / desired_freq;
    }
}

/**
 * @brief      This function servers to set i3c master baud rate.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  baudrate_hz    - i3c_baudrate_hz_t.
 * @param[in]  clk_src_t      - i3c_clk_src_t.
 * @param[in]  is_odhpp       - whether the high scl for open-drain is the push-pull high .
 * @return     1: fail    0: success
 */
unsigned char i3c_master_set_baudrate(i3c_chn_e chn, i3c_baudrate_hz_t *baudrate_hz, i3c_clk_src_t *clk_src_t,unsigned char is_odhpp){
   unsigned int div, freq;
   unsigned int div_even, div_odd;
   unsigned int pp_baud, od_baud, i2c_baud;
   unsigned int err_rate0, err_rate1;
   unsigned int i3c_ppbaud_hz  = baudrate_hz->i3c_pushpull_baud;
   unsigned int i3c_ppbaud_maxhz = i3c_ppbaud_hz /10 + i3c_ppbaud_hz;
   unsigned int i3c_odbaud_hz  = baudrate_hz->i3c_opendrain_baud;
   unsigned int i3c_odbaud_maxhz = i3c_odbaud_hz /10 + i3c_odbaud_hz;
   unsigned int i2c_baud_hz  = baudrate_hz->i2c_baud;
   unsigned int i3c_pplow_ns , i3c_odlow_ns;
   unsigned int source_clock_hz =0;
   unsigned char i3c_clk =0;
   if(clk_src_t->clk_src == CLK_RC_24M){
       source_clock_hz = 24000000/clk_src_t->clk_src_div;
       i3c_clk =0;
   }else if(clk_src_t->clk_src == CLK_XTAL_24M){
       source_clock_hz = 24000000/clk_src_t->clk_src_div;
       i3c_clk=1;
   } else{
       i3c_clk=2;
       pll_bb_clk_e pll = (clk_src_t->clk_src >> 8)&0xff;
       switch (pll) {
       case PLL_144M:
           source_clock_hz = 144000000/clk_src_t->clk_src_div;
           break;
       case PLL_120M:
           source_clock_hz = 120000000/clk_src_t->clk_src_div;
           break;
       case PLL_180M:
           source_clock_hz = 180000000/clk_src_t->clk_src_div;
           break;
       case PLL_192M:
           source_clock_hz = 192000000/clk_src_t->clk_src_div;
           break;
       default:
           break;
       }
   }
  if(chn == I3C0){
      reg_i3c0_clk_set =  (i3c_clk<<4)| (clk_src_t->clk_src_div);
  }else if(chn==I3C1){
      reg_i3c1_clk_set =  (i3c_clk<<4)| (clk_src_t->clk_src_div);
  }
   /* Find out the div to generate target freq
   scl for push-pull:  high: (ppbaud+1)fclk cycle  low: (ppbaud+1+pplow)fclk cycle
   scl for open-drain:  scl low = (ppbaud+1)*(odbaud+1)
                        odhpp =0  scl high  = scl low
                        odhpp=1   scl high  =  push-pull high
   scl for i2c:  scl high  (PPBAUD + 1) x (ODBAUD + 1) x [(I2CBAUD >> 1) + 1] fclk cycles.
                 i2cbaud is even :  scl low  = scl high
                 i2cbaud is odd  :  scl low  = scl high+1
   */
   //Processing according to pplow=0
   freq = source_clock_hz / 2;
   /* ppFreq = FCLK / 2 / (PPBAUD + 1)), 0 <= PPBAUD <= 15 */
   div = freq /i3c_ppbaud_hz;
   div = (div == 0) ? 1 : div;
   if(freq /div > i3c_ppbaud_maxhz){
        div ++;
   }
   if(div > I3C_PPBAUD_DIV_MAX){
       return 1;
   }
   pp_baud  = div -1;
   freq /= div;

   i3c_pplow_ns = (unsigned int)(1000000000 /(2 * freq));
   //odbaud
   if(is_odhpp){
    //  scl high  =  push-pull high
    /* odFreq = (2*freq) / (ODBAUD + 2), 1 <= ODBAUD <= 255    High is 1 PPBUAD, Low is ODBUAD + 1 PPBUAD*/
    div = (2*freq)/ i3c_odbaud_hz;
    div = div < 2? 2:div;
    if((2*freq /div) > i3c_odbaud_maxhz){
        div++;
    }
    od_baud = div -2;
    freq = (2* freq)/ div;
   }else{
     // odhpp =0  scl high  = scl low
     /* odFreq = ppFreq / (ODBAUD + 1), 1 <= ODBAUD <= 255 */
      div = freq / i3c_odbaud_hz;
      div = div < 1 ? 1:div;
      if(freq / div > i3c_odbaud_maxhz){
          div ++;
      }
      od_baud = div -1;
      freq /=div;
   }
   i3c_odlow_ns = (od_baud + 1) * i3c_pplow_ns;
   /* i2cFreq = odFreq / (I2CBAUD + 1), 0 <= I2CBAUD <= 7 (I2CBAUD need << 1 in register) */
   /* i2cFreq = NSEC_PER_SEC / (I2CBAUD + 1)*i3cOdLow_Ns */
   div_even  = (source_clock_hz / i2c_baud_hz) / (2 * (pp_baud + 1) * (od_baud + 1));
   div_even  = div_even == 0 ? 1 : div_even;
   err_rate0 = i3c_calc_error_ratio((unsigned int)(1000000000 / (2 * div_even * i3c_odlow_ns)), i2c_baud_hz);

   div_odd   = ((source_clock_hz / i2c_baud_hz) / ((pp_baud + 1) * (od_baud + 1) - 1)) / 2;
   div_odd   = div_odd == 0 ? 1 : div_odd;
   err_rate1 = i3c_calc_error_ratio((unsigned int)(1000000000 / ((2 * div_odd + 1) * i3c_odlow_ns)), i2c_baud_hz);

   if (err_rate0 < I3C_ERROR_RATE_MAX || err_rate1 < I3C_ERROR_RATE_MAX)
   {
       /* Use this div */
       i2c_baud = err_rate0 < err_rate1 ? (div_even - 1) * 2 : (div_odd - 1) * 2 + 1;
   }
   else
   {
       /* Use div + 1, unless current freq is already lower than desired. */
       i2c_baud = freq / div_even < i2c_baud_hz ? (div_even - 1) * 2 : div_even * 2;
   }
   reg_i3c_mconfig(chn) =  (reg_i3c_mconfig(chn) & ~(FLD_I3C_M_PPBAUD | FLD_I3C_M_PPLOW |FLD_I3C_M_ODBAUD |FLD_I3C_M_I2CBAUD)) |
                           ((pp_baud << 8) | (od_baud << 16) | (i2c_baud << 28));
   return 0;
}

/**
 * @brief      This function servers to initialize i3c master.
 * @param[in]  chn            - i3c_chn_e.
 * @return     1: fail    0: success
 */
unsigned char  i3c_master_init(i3c_chn_e chn,i3c_master_config_t *m_config){
    if(chn == I3C0){
        reg_rst7 |= FLD_RST7_I3C0;
        reg_clk_en7|=FLD_CLK7_I3C0_EN;
    }else if(chn == I3C1){
        reg_rst7 |= FLD_RST7_I3C1;
        reg_clk_en7|=FLD_CLK7_I3C1_EN;
    }
    reg_i3c_mconfig(chn) = (m_config->master_en)|(m_config->timeout_dis << 3)|(m_config->opendrain_stop_en <<6)|
                           (m_config->opendrain_stop_en << 24);
    return i3c_master_set_baudrate(chn,&m_config->baudrate_hz,&m_config->clk_src,m_config->opendrain_high_en);
}

/**
 * @brief      This function servers to i3c master emit request.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  master_req     - i3c_bus_request_e.
 * @return     none
 */
void i3c_master_emit_request(i3c_chn_e chn,i3c_bus_request_e master_req){
     unsigned int mctrl_val;
     mctrl_val = reg_i3c_mctrl(chn);

    if(master_req ==I3C_REQUEST_PROCESSDAA){
        mctrl_val = mctrl_val &(~FLD_I3C_M_CTRL_TYPE);
    }
    if(master_req == I3C_REQUEST_TARGETREST){
        mctrl_val = (mctrl_val &(~FLD_I3C_M_CTRL_TYPE))|(0x02<<4);
    }

    reg_i3c_mctrl(chn) = (mctrl_val & (~FLD_I3C_M_REQUEST)) | (master_req);
}

/**
 * @brief      This function servers to i3c master daa process.
 * @param[in]  chn              - i3c_chn_e.
 * @param[in]  address_list     - the list of slave-assigned addresses.
 * @param[in]  list_cnt         - the list count.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_process_daa(i3c_chn_e chn,unsigned char *address_list,unsigned char list_cnt){
    i3c_common_return_status_e result = I3C_STATUS_SUCCESS;
    unsigned char rxbuff[8] ={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    unsigned char rx_size =0;
    _Bool mctrl_done   =0;
    unsigned int status =0;
    unsigned int err_status=0;
    unsigned char rx_count=0;
    unsigned char dev_count=0;
    g_enabled_ints =  i3c_master_get_irq_mask(chn);
    i3c_master_clr_irq_mask(chn,g_enabled_ints);
    i3c_master_emit_request(chn,I3C_REQUEST_PROCESSDAA);

    do{
        status = i3c_master_get_irq_status(chn);
        /* Check for error flags. */
        err_status = i3c_master_get_irq_err_status(chn);
        result = i3c_master_check_and_clear_err(chn,err_status);
        if(result != I3C_STATUS_SUCCESS){
            break;
        }
        if((!mctrl_done) || (rx_size < 8)){
            i3c_master_get_fifo_count(chn,&rx_count,NULL);
            if(rx_count !=0){
                rxbuff[rx_size++] =  (unsigned char)(reg_i3c_mrdatab_0(chn)&FLD_I3C_MRDATA_B);
            }
            if(i3c_master_get_irq_status(chn)&I3C_MASTER_CONTROLDONE_FLAG){
                i3c_master_clr_irq_status(chn,I3C_MASTER_CONTROLDONE_FLAG);
                mctrl_done = 1;
            }
        }else if((i3c_master_get_state(chn) == I3C_MASTER_STATE_DAA) &&
                 (0 != (i3c_master_get_irq_status(chn)&I3C_MASTER_BETWEEN_FLAG)) ){
              if(((dev_count + 1) > list_cnt) || (( g_dev_count[chn] + 1) > I3C_MAX_DEVCNT)){
                  result = I3C_STATUS_SLAVE_COUNT_EXCEED;
                  break;
              }
              /* Assign the dynamic address from address list. */
              g_dev_list[chn][dev_count].dynamic_addr = *address_list++;
              reg_i3c_mwdatab(chn) = g_dev_list[chn][dev_count].dynamic_addr;

              /* Emit process DAA again. */
              i3c_master_emit_request(chn,I3C_REQUEST_PROCESSDAA);
              g_dev_list[chn][dev_count].vendor_id   = (((unsigned short)rxbuff[0] << 8 | (unsigned short)rxbuff[1]) & 0xFFFE) >> 1;
              g_dev_list[chn][dev_count].part_number = ((unsigned int)rxbuff[2] << 24 | (unsigned int)rxbuff[3] << 16 |
                                                    (unsigned int)rxbuff[4] << 8 | (unsigned int)rxbuff[5]);
              g_dev_list[chn][dev_count].bcr        = rxbuff[6];
              g_dev_list[chn][dev_count].dcr        = rxbuff[7];
              dev_count++;
              g_dev_count[chn]++;
              /* Ready to handle next device. */
              mctrl_done = 0;
              rx_size    = 0;
        }
    }while ((status & (unsigned int)I3C_MASTER_COMPLETE_FLAG) != (unsigned int)I3C_MASTER_COMPLETE_FLAG);
    /* Master stops DAA if slave device number exceeds the prepared address number. */
    if (result == I3C_STATUS_SLAVE_COUNT_EXCEED)
    {
        /* Send the STOP signal */
        reg_i3c_mctrl(chn) = ((reg_i3c_mctrl(chn) & ~(FLD_I3C_M_REQUEST | FLD_I3C_M_DIR | FLD_I3C_M_RDTERM)) |I3C_REQUEST_EMITSTOP);
    }
    /* Clear all flags. */
    i3c_master_clr_irq_err_status(chn, (unsigned int)I3C_MASTER_ALL_ERROR_FLAGS);
    i3c_master_clr_irq_status(chn, (unsigned int)I3C_MASTER_CLEAR_FLAG);

    /* Enable I3C IRQ sources while we configure stuff. */
    i3c_master_set_irq_mask(chn, g_enabled_ints);

    return result;
}

/**
 * @brief      This function servers to i3c master restart with rxsize.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  type        - i3c_bus_type_e.
 * @param[in]  address     - the slave id.
 * @param[in]  dir         - i3c_direction_e
 * @param[in]  rx_size     - the read size.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_restart_with_rxsize(i3c_chn_e chn, i3c_bus_type_e type, unsigned char address, i3c_direction_e dir, unsigned char rx_size){
    unsigned int mctrl_val;
    /* Clear all flags. */
    i3c_master_clr_irq_status(chn,I3C_MASTER_CLEAR_FLAG);
     /* Issue start command. */

    mctrl_val = reg_i3c_mctrl(chn);
    mctrl_val &= ~(FLD_I3C_M_CTRL_TYPE|FLD_I3C_M_REQUEST|FLD_I3C_M_DIR|FLD_I3C_M_ADDR|FLD_I3C_M_RDTERM);
    mctrl_val |= ((type<<4)|(0)|(dir<<8)|(address<<9)|(rx_size<<16));
    reg_i3c_mctrl(chn) = mctrl_val;
    mctrl_val |= ((type<<4)|(I3C_REQUEST_EMITSTARTADDR)|(dir<<8)|(address<<9)|(rx_size<<16));
    reg_i3c_mctrl(chn) = mctrl_val;
    return I3C_STATUS_SUCCESS;
}

/**
 * @brief      This function servers to i3c master restart.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  type        - i3c_bus_type_e.
 * @param[in]  address     - the slave id.
 * @param[in]  dir         - i3c_direction_e
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_restart(i3c_chn_e chn, i3c_bus_type_e type, unsigned char address, i3c_direction_e dir){
    return i3c_master_restart_with_rxsize(chn, type, address, dir, 0);
}

/**
 * @brief      This function servers to i3c master start with rxsize.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  type        - i3c_bus_type_e.
 * @param[in]  address     - the slave id.
 * @param[in]  dir         - i3c_direction_e
 * @param[in]  rx_size     - the read size.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_start_with_rxsize (i3c_chn_e chn, i3c_bus_type_e type, unsigned char address, i3c_direction_e dir, unsigned char rx_size)
{
    i3c_master_state_e masterState = i3c_master_get_state(chn);
    if ((masterState != I3C_MASTER_STATE_IDLE) && (masterState != I3C_MASTER_STATE_NORMACT))
    {
        return I3C_STATUS_BUSY;
    }

    return i3c_master_restart_with_rxsize(chn, type, address, dir, rx_size);
}

/**
 * @brief      This function servers to i3c master start without rxsize.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  type        - i3c_bus_type_e.
 * @param[in]  address     - the slave id.
 * @param[in]  dir         - i3c_direction_e
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_start(i3c_chn_e chn, i3c_bus_type_e type, unsigned char address, i3c_direction_e dir){
    i3c_master_state_e masterState = i3c_master_get_state(chn);
    if ((masterState != I3C_MASTER_STATE_IDLE) && (masterState != I3C_MASTER_STATE_NORMACT))
    {
        return I3C_STATUS_BUSY;
    }

    return i3c_master_start_with_rxsize(chn, type, address, dir, 0);
}

/**
 * @brief      This function servers to i3c master wait for ctrl done.
 * @param[in]  chn         - i3c_chn_e.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_wait_for_ctrl_done(i3c_chn_e chn,unsigned char wait_idle){
    i3c_common_return_status_e result = I3C_STATUS_SUCCESS;
    unsigned int status, err_status;
    while(result == I3C_STATUS_SUCCESS){
        status = i3c_master_get_irq_status(chn);
        err_status = i3c_master_get_irq_err_status(chn);
        /* Check for error flags. */
        result = i3c_master_check_and_clear_err(chn, err_status);
        /* Check if the control finishes. */
        if (0 != (status & (unsigned int)I3C_MASTER_CONTROLDONE_FLAG))
        {
            i3c_master_clr_irq_status(chn, I3C_MASTER_CONTROLDONE_FLAG);
            if (!wait_idle)
            {
                break;
            }
        }
        /* I3C_MASTER_CONTROLDONE_FLAG only indicates ACK got, need to wait for SDA high. */
        if (wait_idle && i3c_master_get_state(chn) == I3C_MASTER_STATE_IDLE)
        {
            break;
        }
    }

    return result;
}

/**
 * @brief      This function servers to i3c master wait for complete.
 * @param[in]  chn         - i3c_chn_e.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_wait_complete(i3c_chn_e chn, unsigned char wait_idle)
{
    unsigned int status, err_status;
    i3c_common_return_status_e result = I3C_STATUS_SUCCESS;
    do
    {
        status    = i3c_master_get_irq_status(chn);
        err_status = i3c_master_get_irq_err_status(chn);
        result    = i3c_master_check_and_clear_err(chn, err_status);
    } while (((status & (unsigned int)I3C_MASTER_COMPLETE_FLAG) != (unsigned int)I3C_MASTER_COMPLETE_FLAG) &&
             (result == I3C_STATUS_SUCCESS));

    i3c_master_clr_irq_status(chn, I3C_MASTER_COMPLETE_FLAG);

    if (wait_idle)
    {
        while (i3c_master_get_state(chn) != I3C_MASTER_STATE_IDLE);
    }

    return result;
}

/**
 * @brief      This function servers to i3c master emit stop.
 * @param[in]  chn         - i3c_chn_e.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_emitstop(i3c_chn_e chn, unsigned char wait_idle)
{
    i3c_common_return_status_e result = I3C_STATUS_SUCCESS;

    /* Return an error if the bus is not in transaction. */
    if (i3c_master_get_state(chn) != I3C_MASTER_STATE_NORMACT)
    {
        return I3C_STATUS_INVALID_REQ;
    }

    /* Send the STOP signal */
     reg_i3c_mctrl(chn) = ((reg_i3c_mctrl(chn) & ~(FLD_I3C_M_REQUEST | FLD_I3C_M_DIR | FLD_I3C_M_RDTERM)) |I3C_REQUEST_EMITSTOP);

    /* Wait for the stop operation finishes. */
    /* Also check for errors while waiting. */
    result = i3c_master_wait_for_ctrl_done(chn, wait_idle);

    return result;
}

/*!
 * @brief This function was used of check if device request wins arbitration.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return  1: wins arbitration;  0: no wins arbitration;
 */
static unsigned char i3c_master_transfer_nostart_flag(i3c_chn_e chn, i3c_master_transfer_t *transfer)
{
    (void)transfer;
    /* Wait tx fifo empty. */
    unsigned char tx_count = 0xFF;

    while (tx_count != 0)
    {
        i3c_master_get_fifo_count(chn, NULL, &tx_count);
    }

    /* Check if device request wins arbitration. */
    if (0 != (i3c_master_get_irq_status(chn) & (unsigned int)I3C_MASTER_ARBITRATIONWON_FLAG))
    {
        /* Clear all flags. */
        i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
        /* Enable I3C IRQ sources. */
        i3c_master_set_irq_mask(chn, g_enabled_ints);
        return 1;
    }
    return 0;
}

/*!
 * @brief This function was used of waiting to see if data can be filled to send.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  byte_counts    - the fill tx counts.
 * return      i3c_common_return_status_e.
 */
static i3c_common_return_status_e i3c_master_wait_tx_ready(i3c_chn_e chn, unsigned char byte_counts)
{
    unsigned int err_status;
    i3c_common_return_status_e result;
    unsigned char tx_count;
    unsigned char txfifo_size = 2 << ((reg_i3c_capabilities(chn)& FLD_I3C_FIFOTX) >> 26);
    do
    {
        /* Get the number of words in the tx fifo and compute empty slots. */
        i3c_master_get_fifo_count(chn, NULL, &tx_count);
        tx_count = txfifo_size - tx_count;
        /* Check for error flags. */
        err_status = i3c_master_get_irq_err_status(chn);
        result = i3c_master_check_and_clear_err(chn, err_status);
        if (I3C_STATUS_SUCCESS != result)
        {
            return result;
        }
    } while (tx_count < byte_counts);
    return I3C_STATUS_SUCCESS;
}

static i3c_common_return_status_e i3c_master_send(i3c_chn_e chn, void *tx_buff, unsigned char tx_size, i3c_master_transfer_flags_e flags)
{
    unsigned char *txbuff;
    txbuff     = (unsigned char *)(tx_buff);
    i3c_common_return_status_e result = I3C_STATUS_SUCCESS;
    unsigned char enable_word = ((flags & I3C_TRANSFER_WORDS_FLAG) == I3C_TRANSFER_WORDS_FLAG) ? 1 : 0;
    unsigned char byte_counts = enable_word ? 2 : 1;
    /* Send data buffer */
    while (0 != tx_size)
    {
        /* Wait until there is room in the fifo. This also checks for errors. */
        result = i3c_master_wait_tx_ready(chn, byte_counts);
        if (I3C_STATUS_SUCCESS != result)
        {
            return result;
        }

        /* Write byte into I3C master data register. */
        if (tx_size > byte_counts)
        {
            if (enable_word)
            {
                reg_i3c_mwdata_h(chn) = *(unsigned short*)txbuff;
            }
            else
            {
                reg_i3c_mwdatab(chn) = *txbuff;
            }
        }
        else
        {
            if (enable_word)
            {
                reg_i3c_mwdata_he(chn) = *(unsigned short*)txbuff;
            }
            else
            {
                reg_i3c_mwdatabe(chn) = *txbuff;
            }
        }

        txbuff = txbuff + byte_counts;
        tx_size  = tx_size - byte_counts;
    }

    result = i3c_master_wait_complete(chn, 0);
    if ((result == I3C_STATUS_SUCCESS) && ((flags & I3C_TRANSFER_NOSTOP_FLAG) == 0))
    {
        result = i3c_master_emitstop(chn, 1);
    }

    return result;
}

/*!
 * @brief This function was used to i3c master write.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return      i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_write(i3c_chn_e chn,i3c_master_transfer_t* transfer){
    i3c_common_return_status_e result    =  I3C_STATUS_SUCCESS;
    i3c_master_state_e masterState = i3c_master_get_state(chn);
    i3c_direction_e direction;
    i3c_master_flush_txfifo(chn);
    i3c_master_flush_rxfifo(chn);
    if ((masterState != I3C_MASTER_STATE_IDLE) && (masterState != I3C_MASTER_STATE_NORMACT))
    {
        return I3C_STATUS_BUSY;
    }

    /* Clear all flags. */
    i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
    /* Reset fifos. These flags clear automatically. */
    reg_i3c_mdatactrl(chn) |= FLD_I3C_M_FLUSHTB | FLD_I3C_M_FLUSHFB;

    /* Disable I3C IRQ sources while we configure stuff. */
    g_enabled_ints =  i3c_master_get_irq_mask(chn);
    i3c_master_clr_irq_mask(chn,g_enabled_ints);

    if(transfer->subaddress_size != 0){
        direction = I3C_WRITE;
    }
    if (0 != (transfer->flags & I3C_TRANSFER_STARTWITH_BROADCAST_ADDR))
    {
        if (0 != (transfer->flags & I3C_TRANSFER_NOSTART_FLAG))
        {
            return I3C_STATUS_INVALID_ARGUMENT;
        }

        if (0UL != (transfer->flags & I3C_TRANSFER_REPEATED_START_FLAG))
        {
            return I3C_STATUS_INVALID_ARGUMENT;
        }

        /* Issue 0x7E as start. */
        result = i3c_master_start(chn, transfer->bus_type, 0x7e, I3C_WRITE);
        if (result != I3C_STATUS_SUCCESS)
        {
            return result;
        }

        result = i3c_master_wait_for_ctrl_done(chn, 0);
        if (result != I3C_STATUS_SUCCESS)
        {
            return result;
        }
    }

    if (0UL == (transfer->flags & I3C_TRANSFER_NOSTART_FLAG))
    {

       result = i3c_master_start(chn, transfer->bus_type, transfer->slave_address, direction);

        if (result != I3C_STATUS_SUCCESS)
        {
            return result;
        }

        result = i3c_master_wait_for_ctrl_done(chn, 0);
        if (result != I3C_STATUS_SUCCESS)
        {
            return result;
        }

        if (i3c_master_transfer_nostart_flag(chn, transfer))
        {
            return I3C_STATUS_IBIWON;
        }
    }
    /* Subaddress, MSB first. */
    if (0 != transfer->subaddress_size)
    {
        unsigned int subaddress_remaining = transfer->subaddress_size;
        while (0 != subaddress_remaining)
        {
            unsigned char subaddress_byte = *(unsigned char*)(transfer->subaddress +(transfer->subaddress_size - subaddress_remaining));
            result = i3c_master_wait_tx_ready(chn, 1);
            reg_i3c_mwdatab(chn) = subaddress_byte;
            subaddress_remaining--;
        }
    }
    /* Transmit data. */
    if (transfer->data_size > 0)
    {
        /* Send Data. */
        result = i3c_master_send(chn, transfer->data, transfer->data_size, transfer->flags);
    }
    else
    {
        if ((transfer->flags & I3C_TRANSFER_NOSTOP_FLAG) == 0)
        {
            result = i3c_master_emitstop(chn, 1);
        }
    }

    if (result == I3C_STATUS_NAK)
    {
        (void)i3c_master_emitstop(chn, 1);
    }

    /* Clear all flags. */
    i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
    /* Enable I3C IRQ sources while we configure stuff. */
    i3c_master_set_irq_mask(chn, g_enabled_ints);

    return result;
}

static i3c_common_return_status_e i3c_master_receive(i3c_chn_e chn, unsigned char *rx_buff, unsigned char rx_size, i3c_master_transfer_flags_e flags)
{
    i3c_common_return_status_e result   = I3C_STATUS_SUCCESS;
    unsigned char is_rx_autoterm = ((flags & I3C_TRANSFER_RXAUTOTERM_FLAG) != 0);
    unsigned char completed    = 0;
    unsigned int status;
    unsigned char *buf;

    /* Handle empty read. */
    if (rx_size == 0)
    {
        return I3C_STATUS_SUCCESS;
    }

    /* Receive data */
    buf = (unsigned char *)rx_buff;

    while ((rx_size != 0) || !completed)
    {
        /* Check for errors. */
        result = i3c_master_check_and_clear_err(chn, i3c_master_get_irq_err_status(chn));
        if (I3C_STATUS_SUCCESS != result)
        {
            return result;
        }

        /* Check complete flag */
        if (!completed)
        {
            status = i3c_master_get_irq_status(chn) & (unsigned int)I3C_MASTER_COMPLETE_FLAG;
            if (0 != status)
            {
                completed = 1;
                /* Clear complete flag */
                i3c_master_clr_irq_status(chn, I3C_MASTER_COMPLETE_FLAG);
                /* Send stop if needed */
                if ((flags & I3C_TRANSFER_NOSTOP_FLAG) == 0)
                {
                    result = i3c_master_emitstop(chn, 0);

                    if (I3C_STATUS_SUCCESS != result)
                    {
                        return result;
                    }
                }
            }
        }

        /* Check RX data */
        if ((0 != rx_size) && (0 != (reg_i3c_mdatactrl(chn) & FLD_I3C_M_RXCOUNT)))
        {
            *buf++ = (unsigned char)(reg_i3c_mrdatab_0(chn) & FLD_I3C_MRDATA_B);
            rx_size--;
            if ((flags & I3C_TRANSFER_DISABLE_RXTERM_FLAG) == 0)
            {
                if ((!is_rx_autoterm) && (rx_size == 1))
                {
                    reg_i3c_mctrl(chn) = (reg_i3c_mctrl(chn) & ~(FLD_I3C_M_RDTERM))|(1<<16);
                }
            }
        }
    }

    /* Wait idle if stop is sent. */
    if ((flags & I3C_TRANSFER_NOSTOP_FLAG) == 0)
    {
        while (i3c_master_get_state(chn) != I3C_MASTER_STATE_IDLE);
    }
    return result;
}

/*!
 * @brief This function was used to i3c master read.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return      i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_read(i3c_chn_e chn,i3c_master_transfer_t* transfer){
    i3c_common_return_status_e result      = I3C_STATUS_SUCCESS;
    i3c_master_state_e masterState = i3c_master_get_state(chn);
    i3c_rx_term_ops_e rx_term_ops;
    i3c_direction_e direction      = transfer->direction;
    /* Return an error if the bus is already in use not by us. */
    if ((masterState != I3C_MASTER_STATE_IDLE) && (masterState != I3C_MASTER_STATE_NORMACT) )
    {
        return I3C_STATUS_BUSY;
    }
    /* Clear all flags. */
    i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
    /* Reset fifos. These flags clear automatically. */
    reg_i3c_mdatactrl(chn) |= FLD_I3C_M_FLUSHTB | FLD_I3C_M_FLUSHFB;

    /* Disable I3C IRQ sources while we configure stuff. */
    g_enabled_ints =  i3c_master_get_irq_mask(chn);
    i3c_master_clr_irq_mask(chn,g_enabled_ints);

    if(transfer->subaddress_size != 0){
        direction = I3C_WRITE;
    }

    /* True: Set Rx termination bytes at start point, False: Set Rx termination one bytes in advance. */
    if ((transfer->flags & I3C_TRANSFER_DISABLE_RXTERM_FLAG) != 0)
    {
        rx_term_ops = I3C_RX_TERMDISABLE;
    }
    else if (transfer->data_size <= 255)
    {
        rx_term_ops = I3C_RX_AUTOTERM;
    }
    else
    {
        rx_term_ops = I3C_RX_TERMLASTBYTE;
    }

    if (0 != (transfer->flags & I3C_TRANSFER_STARTWITH_BROADCAST_ADDR))
    {
        if (0 != (transfer->flags & I3C_TRANSFER_NOSTART_FLAG))
        {
            return I3C_STATUS_INVALID_ARGUMENT;
        }

        if (0UL != (transfer->flags & I3C_TRANSFER_REPEATED_START_FLAG))
        {
            return I3C_STATUS_INVALID_ARGUMENT;
        }

        /* Issue 0x7E as start. */
        result = i3c_master_start(chn, transfer->bus_type, 0x7E, I3C_WRITE);
        if (result != I3C_STATUS_SUCCESS)
        {
            return result;
        }

        result = i3c_master_wait_for_ctrl_done(chn, 0);
        if (result != I3C_STATUS_SUCCESS)
        {
            return result;
        }
    }

    if (0 == (transfer->flags & I3C_TRANSFER_NOSTART_FLAG))
    {
        if ((direction == I3C_READ) && (rx_term_ops == I3C_RX_AUTOTERM))
        {
            result = i3c_master_start_with_rxsize(chn, transfer->bus_type, transfer->slave_address, direction,
                                               (unsigned char)transfer->data_size);
        }
        else
        {
            result = i3c_master_start(chn, transfer->bus_type, transfer->slave_address, direction);
        }
        if (result != I3C_STATUS_SUCCESS)
        {
            return result;
        }

        result = i3c_master_wait_for_ctrl_done(chn, 0);
        if (result != I3C_STATUS_SUCCESS)
        {
            return result;
        }

        if (i3c_master_transfer_nostart_flag(chn, transfer))
        {
            return I3C_STATUS_IBIWON;
        }
    }
    else
    {
        if ((direction == I3C_READ) && (rx_term_ops != I3C_RX_TERMDISABLE))
        {
            /* Can't set Rx termination more than one bytes in advance without START. */
            rx_term_ops = I3C_RX_TERMLASTBYTE;
        }
    }
    /* Subaddress, MSB first. */
    if (0 != transfer->subaddress_size)
    {
        unsigned int subaddress_remaining = transfer->subaddress_size;
        while (0 != subaddress_remaining)
        {
            unsigned char subaddress_byte =*(unsigned char*)(transfer->subaddress+(transfer->subaddress_size - subaddress_remaining));
            result = i3c_master_wait_tx_ready(chn, 1);

            if(1 == subaddress_remaining)
            {
                reg_i3c_mwdatabe(chn)= subaddress_byte;
                result         = i3c_master_wait_complete(chn, 0);
                if (I3C_STATUS_SUCCESS != result)
                {
                    if (result == I3C_STATUS_NAK)
                    {
                        (void)i3c_master_emitstop(chn, 1);
                    }

                    /* Clear all flags. */
                    i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
                    /* Enable I3C IRQ sources. */
                    i3c_master_set_irq_mask(chn, g_enabled_ints);
                    return result;
                }
            }
            else
            {
                reg_i3c_mwdatab(chn) = subaddress_byte;
            }
            subaddress_remaining--;
        }
        /* Need to send repeated start if switching directions to read. */
        if (0 != transfer->data_size)
        {
            if (rx_term_ops == I3C_RX_AUTOTERM)
            {
                result = i3c_master_restart_with_rxsize(chn, transfer->bus_type, transfer->slave_address, I3C_READ,
                                                           (unsigned char)transfer->data_size);
            }
            else
            {
                result = i3c_master_restart(chn, transfer->bus_type, transfer->slave_address, I3C_READ);
            }

            if (I3C_STATUS_SUCCESS != result)
            {
                /* Clear all flags. */
                i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
                /* Enable I3C IRQ sources. */
                i3c_master_set_irq_mask(chn, g_enabled_ints);
                return result;
            }

            result = i3c_master_wait_for_ctrl_done(chn, 0);
            if (result != I3C_STATUS_SUCCESS)
            {
                return result;
            }
        }
    }

    if (rx_term_ops == I3C_RX_AUTOTERM)
    {
        transfer->flags |= I3C_TRANSFER_RXAUTOTERM_FLAG;
    }
    else
    {
        transfer->flags &= ~I3C_TRANSFER_RXAUTOTERM_FLAG;
    }
    /* Receive Data. */
    if (transfer->data_size > 0)
    {
        result = i3c_master_receive(chn, transfer->data, transfer->data_size, transfer->flags);
    }
    else
    {
        if ((transfer->flags & I3C_TRANSFER_NOSTOP_FLAG) == 0)
        {
            result = i3c_master_emitstop(chn, 1);
        }
    }

    if (result == I3C_STATUS_NAK)
    {
        (void)i3c_master_emitstop(chn, 1);
    }

    /* Clear all flags. */
    i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
    /* Enable I3C IRQ sources. */
    i3c_master_set_irq_mask(chn, g_enabled_ints);

    return result;
}

/*!
 * @brief This function was used to i3c master get ibi type.
 * @param[in]  chn         - i3c_chn_e.
 * return      i3c_ibi_type_e.
 */
i3c_ibi_type_e i3c_master_get_ibi_type(i3c_chn_e chn)
{
   return  (reg_i3c_mstatus(chn)&FLD_I3C_M_IBITYPE) >> 6;
}

/*!
 * @brief This function was used to i3c master write by dma.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return      i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_write_dma(i3c_chn_e chn, i3c_master_transfer_t *transfer){
    //fill data
    for(unsigned char i=0;i<(transfer->data_size);i++){
      if(i == (transfer->data_size-1)){
         buff[4*i]= *((unsigned char*)(transfer->data) +i);
         buff[4*i+1]= 1;
         buff[4*i+2]= 0;
         buff[4*i+3]=0;
      }else{
         buff[4*i]=*((unsigned char*)(transfer->data) +i);
         buff[4*i+1]= 0;
         buff[4*i+2]= 0;
         buff[4*i+3]=0;
      }
    }

    i3c_common_return_status_e result = I3C_STATUS_SUCCESS;
    /* Disable I3C IRQ sources while we configure stuff. */
    g_enabled_ints =  i3c_master_get_irq_mask(chn);
    i3c_master_clr_irq_mask(chn,g_enabled_ints);
    //DMA configure
    dma_chn_dis(i3c_dma_tx_chn[chn]);
    i3c_master_flush_txfifo(I3C0);
    reg_i3c_mdmactrl(chn) = (reg_i3c_mdmactrl(chn)& ~(FLD_I3C_M_DMAWIDTH|FLD_I3C_M_DMATB))|(0x18);//enable txdma: 10mode, dma width: byte
    dma_set_size(i3c_dma_tx_chn[chn],transfer->data_size * 4,DMA_WORD_WIDTH);
    dma_set_address(i3c_dma_tx_chn[chn],(unsigned int)(buff),i3c_base_addr(chn)+0xb0);
    dma_chn_en(i3c_dma_tx_chn[chn]);

    //request
    result = i3c_master_start(chn, transfer->bus_type, transfer->slave_address, I3C_WRITE);

    if (result != I3C_STATUS_SUCCESS)
    {
        return result;
    }

    result = i3c_master_wait_for_ctrl_done(chn, 0);
    if (result != I3C_STATUS_SUCCESS)
    {
        /* Clear all flags. */
        i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
        /* Enable I3C IRQ sources. */
        i3c_master_set_irq_mask(chn, g_enabled_ints);
        return result;
    }
    if ((i3c_master_get_irq_status(chn) & (unsigned int)I3C_MASTER_ARBITRATIONWON_FLAG)){
        /* Clear all flags. */
        i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
        /* Enable I3C IRQ sources. */
        i3c_master_set_irq_mask(chn, g_enabled_ints);
        return I3C_STATUS_IBIWON;
    }
    /* Enable I3C IRQ sources. */
    i3c_master_set_irq_mask(chn, g_enabled_ints);
    return result;
}

/*!
 * @brief This function was used to i3c master read by dma.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return      i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_read_dma(i3c_chn_e chn,i3c_master_transfer_t *transfer){
    i3c_common_return_status_e result = I3C_STATUS_SUCCESS;
    /* Disable I3C IRQ sources while we configure stuff. */
    g_enabled_ints =  i3c_master_get_irq_mask(chn);
    i3c_master_clr_irq_mask(chn,g_enabled_ints);
    //dma i3c configure
    i3c_master_flush_rxfifo(chn);
    reg_i3c_mdmactrl(chn) = (reg_i3c_mdmactrl(chn)& ~(FLD_I3C_M_DMAWIDTH|FLD_I3C_M_DMATB))|(0x12); //enable txdma: 10mode, dma width: half-word

    //dma configure
    dma_set_size(i3c_dma_rx_chn[chn],transfer->data_size,DMA_BYTE_WIDTH);
    dma_set_address(i3c_dma_rx_chn[chn],i3c_base_addr(chn)+0xc0,(unsigned int)(transfer->data));
    dma_chn_en(i3c_dma_rx_chn[chn]);
   //request
    result = i3c_master_start(chn, transfer->bus_type, transfer->slave_address, I3C_READ);

    if (result != I3C_STATUS_SUCCESS)
    {
        return result;
    }

    result = i3c_master_wait_for_ctrl_done(chn, 0);
    if (result != I3C_STATUS_SUCCESS)
    {
        /* Clear all flags. */
        i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
        /* Enable I3C IRQ sources. */
        i3c_master_set_irq_mask(chn, g_enabled_ints);
        return result;
    }
    if ((i3c_master_get_irq_status(chn) & (unsigned int)I3C_MASTER_ARBITRATIONWON_FLAG)){
        /* Clear all flags. */
        i3c_master_clr_irq_status(chn, I3C_MASTER_CLEAR_FLAG);
        /* Enable I3C IRQ sources. */
        i3c_master_set_irq_mask(chn, g_enabled_ints);
        return I3C_STATUS_IBIWON;
    }
    /* Enable I3C IRQ sources. */
    i3c_master_set_irq_mask(chn, g_enabled_ints);
    return result;
}


/*!
 * @brief This function was used to I3C master register IBI rule.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  ibiRule     - i3c_register_ibi_addr_t
 */
void i3c_master_register_ibi(i3c_chn_e chn, i3c_register_ibi_addr_t *ibi_rule)
{
    unsigned int rule_value = reg_i3c_ibirules(chn);

    for (unsigned int count = 0; count < sizeof(ibi_rule->address); count++)
    {
        rule_value |= ((unsigned int)ibi_rule->address[count]) << (count * 6);
    }

    rule_value &= ~FLD_I3C_NOBYTE;

    if (!ibi_rule->ibi_has_payload)
    {
        rule_value |= FLD_I3C_NOBYTE;
    }

    reg_i3c_ibirules(chn)= rule_value;
}

/*!
 * brief This function server to when does the slave capture data event occur on the master's timeline.
 * @param[in]   chn                   - i3c_chn_e.
 * @param[in]   ibi_complete_point    - master ibi completion point
 * @param[in]   ibi_timing_data       - Two points in time from the start of the slave segment ibi to ack.
 */
unsigned int i3c_master_get_sample_point_tick(i3c_chn_e chn,unsigned int ibi_complete_point,unsigned char* ibi_timing_data){
    //sample_point_tick =  c_ref -  (S1*9*(ppbaud+1+pplow+ppbaud+1)*24)/(S2*24);
    unsigned int s1= *(unsigned short *)(ibi_timing_data+1);
    unsigned int s2 = *(unsigned char *)(ibi_timing_data+3);
    unsigned char ppbaud = (reg_i3c_mconfig(chn)&FLD_I3C_M_PPBAUD)>>8;
    unsigned char pplow  = (reg_i3c_mconfig(chn)&FLD_I3C_M_PPLOW)>>12;
    unsigned int sample_point_tick =  ibi_complete_point -  (s1*9*(ppbaud+1+ppbaud+pplow+1)*24)/(s2*24);
    return sample_point_tick;
}
/****************************************************************slave ****************************************/
/**
 * @brief      This function selects a pin port for I3C slave interface.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  sda_pin        - the pin port selected as I3C sda pin port.
 * @param[in]  scl_pin        - the pin port selected as I3C scl pin port.
 * @return     none
 */
void i3c_slave_set_pin(i3c_chn_e chn,gpio_func_pin_e sda_pin,gpio_func_pin_e scl_pin)
{
     //When the pad is configured with mux input and a pull-up resistor is required, gpio_input_en needs to be placed before gpio_function_dis,
    //otherwise first set gpio_input_disable and then call the mux function interface,the mux pad will misread the short low-level timing.confirmed by minghai.20210709.
    gpio_input_en((gpio_pin_e)sda_pin); //enable sda input
    gpio_input_en((gpio_pin_e)scl_pin); //enable scl input
    gpio_set_up_down_res((gpio_pin_e)sda_pin, GPIO_PIN_PULLUP_10K);
    gpio_set_up_down_res((gpio_pin_e)scl_pin, GPIO_PIN_PULLUP_10K);
    if(chn == I3C0){
        gpio_set_mux_function(scl_pin,I3C0_SCL_IO);
        gpio_set_mux_function(sda_pin,I3C0_SDA_IO);
        //disable sda_pin and scl_pin gpio function.
        gpio_function_dis((gpio_pin_e)scl_pin);
        gpio_function_dis((gpio_pin_e)sda_pin);
    }else if(chn == I3C1){
        gpio_set_mux_function(scl_pin,I3C1_SCL_IO);
        gpio_set_mux_function(sda_pin,I3C1_SDA_IO);
        //disable sda_pin and scl_pin gpio function.
        gpio_function_dis((gpio_pin_e)scl_pin);
        gpio_function_dis((gpio_pin_e)sda_pin);
    }

}

/**
 * @brief      This function server to initialize i3c slave.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  slaveConfig    - i3c_slave_config_t.
 * @return     none
 */
void i3c_slave_init(i3c_chn_e chn,const i3c_slave_config_t *slave_config){
    if(chn == I3C0){
        reg_rst7 |= FLD_RST7_I3C0;
        reg_clk_en7|=FLD_CLK7_I3C0_EN;
    }else if(chn == I3C1){
        reg_rst7 |= FLD_RST7_I3C1;
        reg_clk_en7|=FLD_CLK7_I3C1_EN;
    }
   unsigned int config_value;
   unsigned char match_count;
   unsigned int source_clock_hz=0;
   sys_clk_src_config_e clk_src = slave_config->clk_src.clk_src;
   unsigned char div = slave_config->clk_src.clk_src_div;
   unsigned char i3c_clk = 0;
   if(clk_src == CLK_RC_24M){
       source_clock_hz = 24000000/div;
       i3c_clk =0;
   }else if(clk_src== CLK_XTAL_24M){
       source_clock_hz = 24000000/div;
       i3c_clk=1;
   }
   else{
       i3c_clk=2;
       pll_bb_clk_e pll = (clk_src >> 8)&0xff;
       switch (pll) {
       case PLL_144M:
           source_clock_hz = 144000000/div;
           break;
       case PLL_120M:
           source_clock_hz = 120000000/div;
           break;
       case PLL_180M:
           source_clock_hz = 180000000/div;
           break;
       case PLL_192M:
           source_clock_hz = 192000000/div;
           break;
       default:
           break;
       }
   }
   if(chn == I3C0){
      reg_i3c0_clk_set =  (i3c_clk<<4)| (div);
   }else if(chn==I3C1){
      reg_i3c1_clk_set =  (i3c_clk<<4)| (div);
   }
   /* Set as (slowClk(MHz) - 1) to generate 1us clock cycle for IBI request to drive SDA low. Note: Use BAMATCH = 1 to
      generate 1us clock cycle if slow clock is 1MHz. The value of 0 would not give a correct match indication. */
    if(source_clock_hz !=0){
        match_count = ((unsigned char)(source_clock_hz/1000000) -1);
        match_count = (match_count == 0) ? 1:match_count;
   }else{
       /* BAMATCH has default value based on Soc default slow clock after reset, using this default value when slowClock_Hz is 0. */
       match_count = (unsigned char)((reg_i3c_config(chn)& FLD_I3C_S_BAMATCH) >> 16);
   }
    config_value = reg_i3c_config(chn);
    config_value &= ~(FLD_I3C_S_TGTENA|FLD_I3C_S_MATCHSS|FLD_I3C_S_S0IGNORE|FLD_I3C_S_OFFLINE|FLD_I3C_S_BAMATCH|FLD_I3C_S_SADDR);
    config_value |= (slave_config->static_addr <<25 )|(match_count << 16)|(slave_config->offline << 9) |(slave_config->is_ignore_s0s1_error<<3)|
                    (slave_config->is_match_slave_startstop <<2)|(1);
    reg_i3c_config(chn) = config_value;
    if(slave_config->is_master_capable){
        reg_i3c_mconfig(chn) = ((reg_i3c_mconfig(chn) &(~FLD_I3C_M_CTRENA))|I3C_MASTER_CAPABLE);
    }
    if(slave_config ->is_hotjoin){
        i3c_slave_request_event(chn,I3C_SLAVE_EVENT_HOTJOIN_REQ);
    }
}

/*!
 * brief This function server to I3C slave request event.
 * @param[in]  chn    - i3c_chn_e.
 * @param[in]   event  - I3C slave event of type #i3c_slave_event_e
 */
void i3c_slave_request_event(i3c_chn_e chn, i3c_slave_event_e event)
{
    unsigned int ctrl_value = reg_i3c_ctrl(chn);

    ctrl_value &= ~FLD_I3C_EVENT_REQ;
    ctrl_value |= event;

    reg_i3c_ctrl(chn) = ctrl_value;
}


/*!
 * brief This function server to I3C slave request IBI event with data payload(mandatory and extended).
 * @param[in]  chn      - i3c_chn_e.
 * @param[in]  data     - Pointer to IBI data to be sent in the request.
 * @param[in] data_size - IBI data size.
 */
void i3c_slave_request_ibi_with_data(i3c_chn_e chn, unsigned char *data, unsigned char data_size)
{
    unsigned int ctrl_value;

    if (data_size > 1)
    {
        ctrl_value = data[1]<<8;
        if (data_size > 2)
        {
            ctrl_value |= data[2]<<16;
        }
        if (data_size > 3)
        {
            ctrl_value |= (data[3]<<24);
        }
        ctrl_value |= data_size - 1;
        reg_i3c_ibiext1(chn) = ctrl_value;
    }

    if (data_size > 4)
    {
        ctrl_value = data[4];
        if (data_size > 5)
        {
            ctrl_value |= data[5]<<8;
        }
        if (data_size > 6)
        {
            ctrl_value |= data[6]<<16;
        }
        if (data_size > 7)
        {
            ctrl_value |= data[7]<<24;
        }
        reg_i3c_ibiext2(chn) = ctrl_value;
    }


    ctrl_value = reg_i3c_ctrl(chn);

    ctrl_value &= ~(FLD_I3C_EVENT_REQ | FLD_I3C_IBIDATA | FLD_I3C_EXTDATA);
    if(data_size>1){
        ctrl_value |= ((I3C_SLAVE_EVENT_IBI) | data[0] <<8| (1<<3));
    }else{
        ctrl_value |= ((I3C_SLAVE_EVENT_IBI) | data[0] <<8| (0<<3));
    }
    reg_i3c_ctrl(chn)  = ctrl_value;
}

/*!
 * brief This function server to I3C slave tx data by dma.
 * @param[in]  chn      - i3c_chn_e.
 * @param[in]  data     - the tx data.
 * @param[in] data_size - the data size.
 */
void i3c_slave_set_tx_dma(i3c_chn_e chn,unsigned char *data, unsigned int data_size){
    for(unsigned char i=0;i<(data_size);i++){
      if(i == (data_size-1)){
         buff[4*i]= data[i];
         buff[4*i+1]= 1;
         buff[4*i+2]= 0;
         buff[4*i+3]=0;
      }else{
         buff[4*i]= data[i];
         buff[4*i+1]= 0;
         buff[4*i+2]= 0;
         buff[4*i+3]=0;
      }
    }
    dma_chn_dis(i3c_dma_tx_chn[chn]);
    i3c_master_flush_txfifo(I3C0);
    reg_i3c_sdmactrl(chn) = (reg_i3c_sdmactrl(chn)& ~(FLD_I3C_S_DMAWIDTH|FLD_I3C_S_DMATB))|(0x18); //enable txdma: 10mode, dma width: byte
    dma_set_address(i3c_dma_tx_chn[chn],(unsigned int)(buff),i3c_base_addr(chn)+0x30);
    dma_set_size(i3c_dma_tx_chn[chn],data_size*4,DMA_WORD_WIDTH);
    dma_chn_en(i3c_dma_tx_chn[chn]);
}

/*!
 * brief This function server to I3C slave rx data by dma.
 * @param[in]  chn      - i3c_chn_e.
 * @param[out] data     - the tx data.
 * @param[in] data_size - the data size.
 */
void i3c_slave_set_rx_dma(i3c_chn_e chn,unsigned char *data,unsigned int data_size){
    i3c_slave_flush_rxfifo(chn);
    reg_i3c_sdmactrl(chn) = (reg_i3c_sdmactrl(chn)& ~(FLD_I3C_S_DMAWIDTH|FLD_I3C_S_DMATB))|(0x12); //enable txdma: 10mode, dma width: half-word
    dma_set_address(i3c_dma_rx_chn[chn],i3c_base_addr(chn)+0x40,(unsigned int)(data));
    dma_set_size(i3c_dma_rx_chn[chn],data_size,DMA_BYTE_WIDTH);
    dma_chn_en(i3c_dma_rx_chn[chn]);
}

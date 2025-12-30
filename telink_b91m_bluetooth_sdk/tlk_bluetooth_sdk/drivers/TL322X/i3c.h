/********************************************************************************************************
 * @file    i3c.h
 *
 * @brief   This is the header file for tl322x
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
#include "reg_include/register.h"
#include "lib/include/clock.h"
#include "string.h"
#include "dma.h"
#include "gpio.h"

/*************************************************** common ************************************************/
typedef enum{
    I3C0  =0,
    I3C1  =1,
}i3c_chn_e;

/*! @brief Construct a status code value from a group and code number. */
#define MAKE_STATUS(group, code) ((((group)*100) + (code)))
#define I3C_STATUS_GROUP  0
/*! @brief I3C status return codes. */
typedef enum
{
    I3C_STATUS_SUCCESS              = MAKE_STATUS(I3C_STATUS_GROUP, 0),
    I3C_STATUS_BUSY                 = MAKE_STATUS(I3C_STATUS_GROUP, 1), /*!< The master is already performing a transfer. */
    I3C_STATUS_IDLE                 = MAKE_STATUS(I3C_STATUS_GROUP, 2), /*!< The slave driver is idle. */
    I3C_STATUS_NAK                  = MAKE_STATUS(I3C_STATUS_GROUP, 3), /*!< The slave device sent a NAK in response to an address. */
    I3C_STATUS_WRITE_ABORT          = MAKE_STATUS(I3C_STATUS_GROUP, 4), /*!< The slave device sent a NAK in response to a write. */
    I3C_STATUS_TERM                 = MAKE_STATUS(I3C_STATUS_GROUP, 5), /*!< The master terminates slave read. */
    I3C_STATUS_READ_FIFO_ERROR      = MAKE_STATUS(I3C_STATUS_GROUP, 6), /*!< Read from M/SRDATAB register when FIFO empty. */
    I3C_STATUS_WRITE_FIFO_ERROR     = MAKE_STATUS(I3C_STATUS_GROUP, 7), /*!< Write to M/SWDATAB register when FIFO full. */
    I3C_STATUS_MSG_ERROR            = MAKE_STATUS(I3C_STATUS_GROUP,8), /*!< Message SDR/DDR mismatch or read/write message in wrong state */
    I3C_STATUS_INVALID_REQ          = MAKE_STATUS(I3C_STATUS_GROUP, 9), /*!< Invalid use of request. */
    I3C_STATUS_TIMEOUT              = MAKE_STATUS(I3C_STATUS_GROUP, 10), /*!< The module has stalled too long in a frame. */
    I3C_STATUS_SLAVE_COUNT_EXCEED   = MAKE_STATUS(I3C_STATUS_GROUP, 11), /*!< The I3C slave count has exceed the definition in I3C_MAX_DEVCNT. */
    I3C_STATUS_IBIWON               = MAKE_STATUS(I3C_STATUS_GROUP, 12), /*!< The I3C slave event IBI or MR or HJ won the arbitration on a header address. */
    I3C_STATUS_OVERRUN_ERROR        = MAKE_STATUS(I3C_STATUS_GROUP, 13), /*!< Slave internal from-bus buffer/FIFO overrun. */
    I3C_STATUS_UNDERRUN_ERROR       = MAKE_STATUS(I3C_STATUS_GROUP, 14), /*!< Slave internal to-bus buffer/FIFO underrun */
    I3C_STATUS_UNDERRUN_NAK         = MAKE_STATUS(I3C_STATUS_GROUP, 15), /*!< Slave internal from-bus buffer/FIFO underrun and NACK error */
    I3C_STATUS_INVALID_START        = MAKE_STATUS(I3C_STATUS_GROUP, 16), /*!< Slave invalid start flag */
    I3C_STATUS_SDRPARITY_ERROR      = MAKE_STATUS(I3C_STATUS_GROUP, 17), /*!< SDR parity error */
    I3C_STATUS_S0S1_ERROR           = MAKE_STATUS(I3C_STATUS_GROUP, 18), /*!< S0 or S1 error */
    I3C_STATUS_INVALID_ARGUMENT     = MAKE_STATUS(I3C_STATUS_GROUP, 19), /*!<interfece argument invalid */
}i3c_common_return_status_e;

/*! @brief Watermark of TX int/dma trigger level. */
typedef enum
{
    I3C_TX_TRIGGER_ON_NOT_EMPTY                  = 0, /*!< Trigger on empty. */
    I3C_TX_TRIGGER_UNTIL_ONEQUART_OR_LESS    = 1, /*!< Trigger on 1/4 full or less. */
    I3C_TX_TRIGGER_UNTIL_ONE_HALF_OR_LESS    = 2, /*!< Trigger on 1/2 full or less. */
    I3C_TX_TRIGGER_UNTIL_ONE_LESS_THAN_FULL  = 3, /*!< Trigger on 1 less than full or less. */
} i3c_tx_trigger_level_e;

/*! @brief Watermark of RX int/dma trigger level. */
typedef enum _i3c_rx_trigger_level
{
    I3C_RX_TRIGGER_ON_NOT_EMPTY                  = 0, /*!< Trigger on not empty. */
    I3C_RX_TRIGGER_UNTIL_ONEQUART_OR_MORE        = 1, /*!< Trigger on 1/4 full or more. */
    I3C_RX_TRIGGER_UNTIL_ONE_HALF_OR_MORE        = 2, /*!< Trigger on 1/2 full or more. */
    I3C_RX_TRIGGER_UNTIL_THREE_QUART_OR_MORE    = 3, /*!< Trigger on 3/4 full or more. */
} i3c_rx_trigger_level_e;


/**
 *  @brief      This function serves to get the depth of fifo.
 *  @param[in]  chn - i3c_chn_e
 *  @return     the depth of fifo.
 */
static inline unsigned char i3c_get_txfifo_size(i3c_chn_e chn){
    return 2 << ((reg_i3c_capabilities(chn)& FLD_I3C_FIFOTX) >> 26);
}

/**
 * @brief     This function serves to set i3c tx_dma channel.
 * @param[in] chn     - i3c_chn_e.
 * @param[in] dma_chn -  dma channel.
 * @return    none
 */
void i3c_set_tx_dma_config(i3c_chn_e chn,dma_chn_e dma_chn);

/**
 * @brief     This function serves to set i3c rx_dma channel.
 * @param[in] chn     - i3c_chn_e.
 * @param[in] dma_chn -  dma channel.
 * @return    none
 */
void i3c_set_rx_dma_config(i3c_chn_e chn,dma_chn_e dma_chn);

/*************************************************** master ************************************************/
/*! @brief The relevant cccs supported are listed below:(corresponding timing through the i3c_master_write/read interface)
 *ENEC/DISEC:(enable/disable target events byte : bit3:ENHJ/DISHJ  bit1: ENCR/DISCR  bit0: ENINT/DISINT)
 * format 1: direct(0x80/0x81)
 * re/start + 0x7e/w/ ack + direct enec/disec ccc + restart + target addr/w/ack + enable/disable target event byte  + is_stop
 * format 2: broadcast(0x00/0x01)
 * re/start + 0x7e/w/ack + broadcast enec/disec ccc + enable/disable target events byte + is_stop
 *RSTDAA:
 * format: (0x06)
 * re/start + 0x7e/w/ack + broadcast RSTDAA ccc + is_stop
 *DEFTGTS:
 * format:(0x08)
 * re/start + 0x7e/w/ack + broadcast DEFTGTS ccc + count +(define buff: (describes active controller: dynamic addr + dcr + bcr +static addr ) + (describes first target or group: target or group addr + dcr +bcr + static addr) )+ is_stop
 *SETXTIME:
 * format1:  broadcast (0x28)
 * re/start + 0x7e/w/ack + broadcast setxtime ccc + subcommand byte(0xdf:Only Enter Async Mode 0 is supported.) + data optional + is_stop
 * format2: direct(0x98)
 * re/start + 0x7e/w/ack + direct setxtime ccc + restart + target addr/w/ack + subcommand byte(0xdf:Only Enter Async Mode 0 is supported.) + is_stop
 *GETXTIME:
 * format(0x99):
 * re/start + 0x7e/w/ack + direct getxtime ccc + restart + target addr/r/ack + supported modes byte  + state byte + frequency byte + inaccuracy byte + is_stop
 *SETAASA:
 * format(0x29):
 * re/start + 0x7e/w/ack + broadcast setaasa ccc + is_stop
 *RSTACT
 * format1: broadcast(0x2a)
 * re/start + 0x7e/w/ack + broadcast rstact ccc + defining byte (Reset the I3C Peripheral Only)+ is_stop
 * format2: direct(0x9a)
 * write: re/start + 0x7e/w/ack + direct rstact ccc + defining byte +restart + target addr/w/ack + is_stop
 * read:  re/start + 0x7e/w/ack + direct rstact ccc + defining byte + restart + target addr/r/ack + returned data + is_stop
 *SETGRPA:
 * format:(0x9b)
 * re/start + 0x7e/w/ack + direct setgrpa ccc + restart + target addr/w/ack + 7-bit group address<<1 + is_stop
 *RSTGRPA:
 * format: broadcast(0x2c):
 * re/start + 0x7e/w/ack + broadcast rstgrpa ccc + is_stop
 * format: direct(0x9c):
 * 1.re/start + 0x7e/w/ack + direct rstgrpa ccc + restart + group addr/w/ack+ is_stop
 * 2. re/start + 0x7e/w/ack + direct rstgrpa ccc + restart + target addr/w/ack + is_stop
 *DEFGRPA:
 * format(0x2b):
 * re/start + 0x7e/w/ack + broadcast defgrpa ccc + group addr<<1 + group descriptor + count + (one dynamic addr for each target in this group) + is_stop
 *SETDASA:
 * format(0x87):
 * re/start + 0x7e/w/ack + direct setdasa ccc + restart + target addr(static) /w/ack + dynamic address<<1 +is_stop
 *GETPID:
 * format(0x8d):
 * re/start + 0x7e/w/ack + direct getpid ccc +restart + target addr /r/ack + byte 5 + byte 4 + byte 3 + byte 2 + byte 1 + byte 0 + is_stop;
 *GETBCR:
 * format(0x8e):
 * re/start + 0x7e/w/ack + direct getbcr ccc + restart + target addr /r/ack + getbcr byte + is_stop
 *GETDCR:
 * format(0x8f):
 * re/start + 0x7e/w/ack + direct getdcr ccc + restart + target addr /r/ack + getdcr byte +is_stop
 *GETSTATUS:
 * format(0x90):
 * re/start + 0x7e/w/ack + direct getstatus ccc + restart + target addr /r/ack + msb + lsb + is_stop
 *GETACCCR:
 * format(0x91):
 * re/start + 0x7e/w/ack + direct getacccr ccc + restart + target addr(secondary controller)/r/ack + 7-bit dynamic address<<1 + is_stop
 * re/start + 0x7e/w/ack + direct getacccr ccc + restart + target addr/r/nack + restart + 0x7e/w/ack + is+stop
 * re/srart + 0x7e/w/ack + direct getacccr ccc + restart + target addr/r/ack + incorrect 7-bit dynamic address<<1 + start +0x7e/w/ack +is_stop
 *GETCAPS:
 * format1(0x95):
 * re/start + 0x7e/w/ack + direct getcaps ccc + restart + target addr/r/ack + 4 getcap byte + is_stop
 * re/start + 0x7e/w/ack + direct getcaps ccc + defining byte + restart + target addr/r/ack + n byte_n + is_stop
 */
#define I3C_MAX_DEVCNT       10
#define  I3C_PPBAUD_DIV_MAX        15
#define  I3C_ERROR_RATE_MAX        10

/*! @brief Structure with I3C baudrate settings. */
typedef struct
{
    unsigned int i2c_baud;           /*!< Desired I2C baud rate in Hertz. */
    unsigned int i3c_pushpull_baud;  /*!< Desired I3C push-pull baud rate in Hertz. */
    unsigned int i3c_opendrain_baud; /*!< Desired I3C open-drain baud rate in Hertz. */
}i3c_baudrate_hz_t;

/*! @brief I3C master enable configuration. */
typedef enum
{
    I3C_MASTER_OFF     = 0, /*!< Master off. */
    I3C_MASTER_ON      = 1, /*!< Master on. */
    I3C_MASTER_CAPABLE = 2  /*!< Master capable. */
} i3c_master_enable_e;

/*! @brief I3C clock source divide*/
typedef struct
{
   sys_clk_src_config_e clk_src;
   unsigned char clk_src_div;
}i3c_clk_src_t;

/*!
 * @brief Structure with settings to initialize the I3C master module.
 */
typedef struct
{
    i3c_master_enable_e master_en;     /*!< Enable master mode. */
    _Bool timeout_dis;                 /*!< Whether to disable timeout to prevent the ERRWARN. */
    _Bool opendrain_stop_en;           /*!< Whether to emit open-drain speed STOP. */
    _Bool opendrain_high_en;           /*!< Enable Open-Drain High to be 1 PPBAUD count for i3c messages, or 1 ODBAUD. */
    i3c_baudrate_hz_t baudrate_hz;     /*!< Desired baud rate settings. */
    i3c_clk_src_t  clk_src;
} i3c_master_config_t;

/*! @brief Emits the requested operation. */
typedef enum{
    I3C_REQUEST_NONE             = 0, /*!< No request. */
    I3C_REQUEST_EMITSTARTADDR     = 1, /*!< Request to emit start and address on bus. */
    I3C_REQUEST_EMITSTOP          = 2, /*!< Request to emit stop on bus. */
    I3C_REQUEST_IBIACKNACK       = 3, /*!< Manual IBI ACK or NACK. */
    I3C_REQUEST_PROCESSDAA       = 4, /*!< Process DAA. */
    I3C_REQUEST_TARGETREST       = 6, /*!< Request to force exit. */
    I3C_REQUEST_AUTOIBI          = 7, /*!< Hold in stopped state, but Auto-emit START,7E. */
}i3c_bus_request_e;


/*! @brief Bus type with EmitStartAddr. */
typedef enum
{
    I3C_TYPEI3CSDR = 0, /*!< SDR mode of I3C. */
    I3C_TYPEI2C    = 1, /*!< Standard i2c protocol. */
} i3c_bus_type_e;

/*!
 * @brief I3C master peripheral flags.
 *
 * The following status register flags can be cleared:
 * - #I3C_MASTER_SLAVESTART_FLAG
 * - #I3C_MASTER_CONTROLDONE_FLAG
 * - #I3C_MASTER_COMPLETE_FLAG
 * - #I3C_MASTER_ARBITRATIONWON_FLAG
 * - #I3C_MASTER_SLAVE2MASTER_FLAG
 *
 * All flags except #I3C_MASTER_BETWEEN_FLAG and #I3C_MASTER_NACKDETECT_FLAG can be enabled as
 * interrupts.
 *
 * @note These enums are meant to be OR'd together to form a bit mask.
 */
typedef enum
{
    I3C_MASTER_BETWEEN_FLAG        = BIT(4),   /*!< Between messages/DAAs flag */
    I3C_MASTER_NACKDETECT_FLAG     = BIT(5),   /*!< NACK detected flag */
    I3C_MASTER_SLAVESTART_FLAG     = BIT(8),   /*!< Slave request start flag */
    I3C_MASTER_CONTROLDONE_FLAG    = BIT(9),   /*!< Master request complete flag */
    I3C_MASTER_COMPLETE_FLAG       = BIT(10),  /*!< Transfer complete flag */
    I3C_MASTER_RXREADY_FLAG        = BIT(11),  /*!< Rx data ready in Rx buffer flag */
    I3C_MASTER_TXTEADY_FLAG        = BIT(12),  /*!< Tx buffer ready for Tx data flag */
    I3C_MASTER_ARBITRATIONWON_FLAG = BIT(13),  /*!< Header address won arbitration flag */
    I3C_MASTER_ERROR_FLAG          = BIT(15),  /*!< Error occurred flag */
    I3C_MASTER_SLAVE2MASTER_FLAG   = BIT(19),  /*!< Switch from slave to master flag */
    I3C_MASTER_CLEAR_FLAG          = I3C_MASTER_SLAVESTART_FLAG | I3C_MASTER_CONTROLDONE_FLAG | I3C_MASTER_COMPLETE_FLAG |
                                     I3C_MASTER_ARBITRATIONWON_FLAG | I3C_MASTER_SLAVE2MASTER_FLAG | I3C_MASTER_ERROR_FLAG,
}i3c_master_flags;

/*!
 * @brief I3C master error flags to indicate the causes.
 * @note These enums are meant to be OR'd together to form a bit mask.
 */
typedef enum
{
    I3C_MASTER_ERROR_NACK_FLAG           = BIT(2),  /*!< Slave NACKed the last address */
    I3C_MASTER_ERROR_WRITE_ABORT_FLAG    = BIT(3),  /*!< Slave NACKed the write data */
    I3C_MASTER_ERROR_TERM_FLAG           = BIT(4),  /*!< Master terminates slave read */
    I3C_MASTER_ERROR_READ_FLAG           = BIT(16), /*!< Read from MRDATAB register when FIFO empty */
    I3C_MASTER_ERROR_WRITE_FLAG          = BIT(17), /*!< Write to MWDATAB register when FIFO full */
    I3C_MASTER_ERROR_MSG_FLAG            = BIT(18), /*!< Message SDR/DDR mismatch or read/write message in wrong state */
    I3C_MASTER_ERROR_INVALIDREQ_FLAG     = BIT(19), /*!< Invalid use of request */
    I3C_MASTER_ERROR_TIMEOUT_FLAG        = BIT(20), /*!< The module has stalled too long in a frame */
    I3C_MASTER_ALL_ERROR_FLAGS           = I3C_MASTER_ERROR_NACK_FLAG | I3C_MASTER_ERROR_WRITE_ABORT_FLAG |
                                           I3C_MASTER_ERROR_TERM_FLAG | I3C_MASTER_ERROR_READ_FLAG |
                                           I3C_MASTER_ERROR_WRITE_FLAG | I3C_MASTER_ERROR_MSG_FLAG | I3C_MASTER_ERROR_INVALIDREQ_FLAG |
                                           I3C_MASTER_ERROR_TIMEOUT_FLAG, /*!< All error flags */
}i3c_master_error_flags;

/*! @brief I3C working master state. */
typedef enum
{
    I3C_MASTER_STATE_IDLE    = 0, /*!< Bus stopped. */
    I3C_MASTER_STATE_SLVREQ  = 1, /*!< Bus stopped but slave holding SDA low. */
    I3C_MASTER_STATE_MSGSDR  = 2, /*!< In SDR Message mode from using MWMSG_SDR. */
    I3C_MASTER_STATE_NORMACT = 3, /*!< In normal active SDR mode. */
    I3C_MASTER_STATE_DAA     = 5, /*!< In ENTDAA mode. */
    I3C_MASTER_STATE_IBIACK  = 6, /*!< Waiting on IBI ACK/NACK decision. */
    I3C_MASTER_STATE_IBIRCV  = 7, /*!< Receiving IBI. */
} i3c_master_state_e;

/*! @brief I3C device information. */
typedef struct
{
    unsigned char dynamic_addr;     /*!< Device dynamic address. */
    unsigned char static_addr;      /*!< Static address. */
    unsigned char dcr;              /*!< Device characteristics register information. */
    unsigned char bcr;              /*!< Bus characteristics register information. */
    unsigned short vendor_id;       /*!< Device vendor ID(manufacture ID). */
    unsigned int part_number;       /*!< Device part number info */
} i3c_device_info_t;

typedef enum
{
    I3C_TRANSFER_DEFAULT_FLAG             = 0x00, /*!< Transfer starts with a start signal, stops with a stop signal. */
    I3C_TRANSFER_NOSTART_FLAG             = 0x01, /*!< Don't send a start condition, address, and sub address */
    I3C_TRANSFER_REPEATED_START_FLAG      = 0x02, /*!< Send a repeated start condition */
    I3C_TRANSFER_NOSTOP_FLAG              = 0x04, /*!< Don't send a stop condition. */
    I3C_TRANSFER_WORDS_FLAG               = 0x08, /*!< Transfer in words, else transfer in bytes. */
    I3C_TRANSFER_DISABLE_RXTERM_FLAG      = 0x10, /*!< Disable Rx termination. Note: It's for I3C CCC transfer. */
    I3C_TRANSFER_RXAUTOTERM_FLAG          = 0x20, /*!< Set Rx auto-termination. Note: It's adaptive based on Rx size(<=255 bytes) except in I3C_MasterReceive. */
    I3C_TRANSFER_STARTWITH_BROADCAST_ADDR = 0x40, /*!< Start transfer with 0x7E, then read/write data with device address. */
}i3c_master_transfer_flags_e;

/*! @brief IBI response. */
typedef enum
{
    I3C_IBI_RESPACK           = 0, /*!< ACK with no mandatory byte. */
    I3C_IBI_RESPNACK          = 1, /*!< NACK. */
    I3C_IBI_RESPACK_MANDATORY = 2, /*!< ACK with mandatory byte. */
    I3C_IBI_RESPMANUAL        = 3, /*!< Reserved. */
} i3c_ibi_response_e;

/*! @brief Direction of master and slave transfers. */
typedef enum
{
    I3C_WRITE = 0, /*!< Master transmit. */
    I3C_READ  = 1  /*!< Master receive. */
} i3c_direction_e;

/*!
 * This structure is used to pass transaction parameters to the i3c_master_write/i3c_master_read API.
 */
typedef struct
{
    unsigned char slave_address;         /*!< The 7-bit slave address. */
    unsigned char* subaddress;           /*!< Sub address. Transferred MSB first. */
    unsigned char subaddress_size;       /*!< Length of sub address to send in bytes. Maximum size is 4 bytes. */
    unsigned char *data;                 /*!< Pointer to data to transfer. */
    unsigned int data_size;              /*!< Number of bytes to transfer. */
    unsigned char bus_type;              /*!< bus type. */
    unsigned char ibi_response;          /*!< ibi response during transfer. */
    unsigned char direction;             /*!< direction. */
    unsigned char flags;                 /*!< Bit mask of options for the transfer. */
}i3c_master_transfer_t;

/*! @brief I3C master read termination operations. */
typedef enum
{
    I3C_RX_TERMDISABLE   = 0, /*!< Master doesn't terminate read, used for CCC transfer. */
    I3C_RX_AUTOTERM      = 1,  /*!< Master auto terminate read after receiving specified bytes(<=255). */
    I3C_RX_TERMLASTBYTE  = 2,  /*!< Master terminates read at any time after START, no length limitation. */
} i3c_rx_term_ops_e;

/*! @brief IBI type. */
typedef enum
{
    I3C_IBI_NORMAL        = 1, /*!< In-band interrupt. */
    I3C_IBI_HOTJOIN       = 3, /*!< slave hot join. */
    I3C_IBI_MASTERREQUEST = 2, /*!< slave master ship request. */
} i3c_ibi_type_e;


/*! @brief Structure with setting master IBI rules and slave registry. */
typedef struct
{
    unsigned char address[5]; /*!< Address array for registry. */
    _Bool ibi_has_payload; /*!< Whether the address array has mandatory IBI byte. */
} i3c_register_ibi_addr_t;

/**
 * @brief      This function serves to enable i3c master mask function.
 * @param[in]  chn  - i3c_chn_e.
 * @param[in]  mask - to select interrupt type.
 * @return     none
 */
static inline void i3c_master_set_irq_mask(i3c_chn_e chn,i3c_master_flags mask){
    reg_i3c_mintset(chn)  |= mask;
}

/**
 * @brief      This function serves to get i3c master mask function.
 * @param[in]  chn  - i3c_chn_e.
 * @return     enabled mask.
 */
static inline unsigned int i3c_master_get_irq_mask(i3c_chn_e chn){
    return reg_i3c_mintset(chn);
}

/**
 * @brief      This function serves to clear i3c master mask function.
 * @param[in]  chn  - i3c_chn_e.
 * @param[in]  mask - to select Interrupt type.
 * @return     none
 */
static inline void i3c_master_clr_irq_mask(i3c_chn_e chn,i3c_master_flags mask){
    reg_i3c_mintclr(chn)  = mask;
}

/**
 * @brief      This function serves to get i3c master interrupt status.
 * @param[in]  chn  - i3c_chn_e.
 * @return     interrupt status.
 */
static inline unsigned int i3c_master_get_irq_status(i3c_chn_e chn){
    return reg_i3c_mstatus(chn);
}

/**
 * @brief      This function serves to clear i3c master interrupt status.
 * @param[in]  chn  - i3c_chn_e.
 * @param[in]  flag - i3c_master_flags.
 * @return     interrupt status.
 *
 */
static inline void i3c_master_clr_irq_status(i3c_chn_e chn,i3c_master_flags flag){
    reg_i3c_mstatus(chn) =  flag;
}

/**
 * @brief      This function serves to get i3c master error interrupt status.
 * @param[in]  chn  - i3c_chn_e.
 * @return     interrupt error status.
 */
static inline unsigned int i3c_master_get_irq_err_status(i3c_chn_e chn){
    return reg_i3c_merrwarn(chn);
}

/**
 * @brief      This function serves to clear i3c master error interrupt status.
 * @param[in]  chn  - i3c_chn_e.
 * @param[in]  flag - i3c_master_error_flags.
 */
static inline void i3c_master_clr_irq_err_status(i3c_chn_e chn,i3c_master_error_flags flag){
    reg_i3c_merrwarn(chn) = flag;
}

/**
 * @brief      This function serves to check and clear i3c master error interrupt status.
 * @param[in]  chn    - i3c_chn_e.
 * @param[in]  status - error status.
 * @return     i3c_common_return_status_e.
 */
static inline i3c_common_return_status_e i3c_master_check_and_clear_err(i3c_chn_e chn, unsigned int status){
    i3c_common_return_status_e result = I3C_STATUS_SUCCESS;

    /* Check for error. These errors cause a stop to automatically be sent. We must */
    /* clear the errors before a new transfer can start. */
    status &= (unsigned int)I3C_MASTER_ALL_ERROR_FLAGS;

    if (0 != (status))
    {
        /* Select the correct error code. Ordered by severity, with bus issues first. */
        if (0 != (status & (unsigned int)I3C_MASTER_ERROR_TIMEOUT_FLAG))
        {
            result = I3C_STATUS_TIMEOUT;
        }
        else if (0 != (status & (unsigned int)I3C_MASTER_ERROR_NACK_FLAG))
        {
            result = I3C_STATUS_NAK;
        }
        else if (0 != (status & (unsigned int)I3C_MASTER_ERROR_WRITE_ABORT_FLAG))
        {
            result = I3C_STATUS_WRITE_ABORT;
        }
        else if (0 != (status & (unsigned int)I3C_MASTER_ERROR_TERM_FLAG))
        {
            result = I3C_STATUS_TERM;
        }
        else if (0 != (status & (unsigned int)I3C_MASTER_ERROR_MSG_FLAG))
        {
            result = I3C_STATUS_MSG_ERROR;
        }
        else if (0 != (status & (unsigned int)I3C_MASTER_ERROR_READ_FLAG))
        {
            result = I3C_STATUS_READ_FIFO_ERROR;
        }
        else if (0 != (status & (unsigned int)I3C_MASTER_ERROR_WRITE_FLAG))
        {
            result = I3C_STATUS_WRITE_FIFO_ERROR;
        }
        else if (0 != (status & (unsigned int)I3C_MASTER_ERROR_INVALIDREQ_FLAG))
        {
            result = I3C_STATUS_INVALID_REQ;
        }

        /* Clear the flags. */
        i3c_master_clr_irq_err_status(chn, status);

        /* Reset fifos. These flags clear automatically. */
        reg_i3c_mdatactrl(chn) |= FLD_I3C_M_FLUSHTB | FLD_I3C_M_FLUSHFB;
    }

    return result;
}

/**
 * @brief      This function serves to get fifo count.
 * @param[in]  chn      - i3c_chn_e.
 * @param[out] rx_count - rxfifo count.
 * @param[out] rx_count - txfifo count.
 */
static inline void i3c_master_get_fifo_count(i3c_chn_e chn,unsigned char* rx_count,unsigned char* tx_count){
    if(NULL != tx_count ){
        *tx_count = (reg_i3c_mdatactrl(chn) & FLD_I3C_M_TXCOUNT) >> 16;
    }
    if(NULL != rx_count){
        *rx_count = (reg_i3c_mdatactrl(chn) & FLD_I3C_M_RXCOUNT) >> 24;
    }
}

/*!
 * @brief  This function serves to get the I3C master state.
 * @param[in]  chn      - i3c_chn_e.
 * @return I3C master state.
 */
static inline i3c_master_state_e i3c_master_get_state(i3c_chn_e chn)
{
    unsigned char  master_state = reg_i3c_mstatus(chn)&FLD_I3C_M_STATE;
    i3c_master_state_e return_code;

    switch (master_state)
    {
        case (unsigned char)I3C_MASTER_STATE_IDLE:
            return_code = I3C_MASTER_STATE_IDLE;
            break;
        case (unsigned char)I3C_MASTER_STATE_SLVREQ:
            return_code = I3C_MASTER_STATE_SLVREQ;
            break;
        case (unsigned char)I3C_MASTER_STATE_MSGSDR:
            return_code = I3C_MASTER_STATE_MSGSDR;
            break;
        case (unsigned char)I3C_MASTER_STATE_NORMACT:
            return_code = I3C_MASTER_STATE_NORMACT;
            break;
        case (unsigned char)I3C_MASTER_STATE_DAA:
            return_code = I3C_MASTER_STATE_DAA;
            break;
        case (unsigned char)I3C_MASTER_STATE_IBIACK:
            return_code = I3C_MASTER_STATE_IBIACK;
            break;
        case (unsigned char)I3C_MASTER_STATE_IBIRCV:
            return_code = I3C_MASTER_STATE_IBIRCV;
            break;
        default:
            return_code = I3C_MASTER_STATE_IDLE;
            break;
    }

    return return_code;
}

/*!
 * @brief      This function serves to emit ibi response.
 * @param[in]  chn               - i3c_chn_e.
 * @param[in]  ibi_response      - i3c_ibi_response_e.
 */
static inline void i3c_master_emit_ibi_response(i3c_chn_e chn, i3c_ibi_response_e ibi_response){
    unsigned int ctrl_val = reg_i3c_mctrl(chn);
    ctrl_val &= ~(FLD_I3C_M_IBIRESP | FLD_I3C_M_REQUEST);
    ctrl_val |= ((ibi_response <<6) | I3C_REQUEST_IBIACKNACK);
    reg_i3c_mctrl(chn) = ctrl_val;
}

/*!
 * @brief      This function serves to get ibi response setting.
 * @param[in]  chn               - i3c_chn_e.
 * @return     the master ibi response setting
 */
static inline i3c_ibi_response_e i3c_master_get_ibi_response(i3c_chn_e chn){
     return     (reg_i3c_mctrl(chn)&FLD_I3C_M_IBIRESP) >>6;
}

/*!
 * @brief  This function serves to i3c master get IBI Address.
 * @param[in]  chn  - i3c_chn_e.
 * @return The 8-bit IBI address.
 */
static inline unsigned char i3c_get_ibi_address(i3c_chn_e chn)
{
    return (reg_i3c_mstatus(chn)&FLD_I3C_M_IBIADDR) >>24;
}

/*!
 * @brief This function serves to set the watermarks for I3C master FIFOs.
 * @param[in]  chn               - i3c_chn_e.
 * @param[in] tx_lvl Transmit FIFO watermark level.
 *            The #I3C_MASTER_TXTEADY_FLAG flag is set whenever the number of words in the transmit FIFO reaches @a tx_lvl.
 * @param[in] rx_lvl Receive FIFO watermark level.
 *            The #I3C_MASTER_RXREADY_FLAG flag is set whenever the number of words in the receive FIFO reaches @a rx_lvl.
 */
static inline void i3c_master_set_watermarks(i3c_chn_e chn, i3c_tx_trigger_level_e tx_lvl, i3c_rx_trigger_level_e rx_lvl)
{
    reg_i3c_mdatactrl(chn) = ( reg_i3c_mdatactrl(chn) & (~ FLD_I3C_M_TXTRIG |FLD_I3C_M_RXTRIG))|((tx_lvl << 4)| (rx_lvl<<6)) | FLD_I3C_M_UNLOCK;
}

/*!
 * @brief  This function serves to flush i3c master txfifo.
 * @param[in]  chn  - i3c_chn_e.
 */
static inline void i3c_master_flush_txfifo(i3c_chn_e chn){
    reg_i3c_mdatactrl(chn) =   FLD_I3C_M_FLUSHTB;
}

/*!
 * @brief  This function serves to flush i3c master rxfifo.
 * @param[in]  chn  - i3c_chn_e.
 */
static inline void i3c_master_flush_rxfifo(i3c_chn_e chn){
    reg_i3c_mdatactrl(chn)  = FLD_I3C_M_FLUSHFB;
}

/**
 * @brief      This function selects a pin port for I3C master interface.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  sda_pin        - the pin port selected as I3C sda pin port.
 * @param[in]  scl_pin        - the pin port selected as I3C scl pin port.
 * @param[in]  sda_pullup_pin - the pin port selected as I3C pullup pin port.
 * @return     none
 */
void i3c_master_set_pin(i3c_chn_e chn,gpio_func_pin_e sda_pin,gpio_func_pin_e scl_pin,gpio_func_pin_e sda_pullup_pin);

/**
 * @brief      This function servers to set i3c master baud rate.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  baudrate_hz    - i3c_baudrate_hz_t.
 * @param[in]  clk_src_t      - i3c_clk_src_t.
 * @param[in]  is_odhpp       - whether the high scl for open-drain is the push-pull high .
 * @return     none
 */
unsigned char i3c_master_set_baudrate(i3c_chn_e chn, i3c_baudrate_hz_t *baudrate_hz, i3c_clk_src_t *clk_src_t,unsigned char is_odhpp);

/**
 * @brief      This function servers to initialize i3c master.
 * @param[in]  chn            - i3c_chn_e.
 * @return     none
 */
unsigned char  i3c_master_init(i3c_chn_e chn,i3c_master_config_t *m_config);

/**
 * @brief      This function servers to i3c master emit request.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  master_req     - i3c_bus_request_e.
 * @return     none
 */
void i3c_master_emit_request(i3c_chn_e chn,i3c_bus_request_e master_req);

/**
 * @brief      This function servers to i3c master daa process.
 * @param[in]  chn              - i3c_chn_e.
 * @param[in]  address_list     - the list of slave-assigned addresses.
 * @param[in]  list_cnt         - the list count.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_process_daa(i3c_chn_e chn,unsigned char *address_list,unsigned char list_cnt);

/**
 * @brief      This function servers to i3c master restart with rxsize.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  type        - i3c_bus_type_e.
 * @param[in]  address     - the slave id.
 * @param[in]  dir         - i3c_direction_e
 * @param[in]  rx_size     - the read size.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_restart_with_rxsize(i3c_chn_e chn, i3c_bus_type_e type, unsigned char address, i3c_direction_e dir, unsigned char rx_size);

/**
 * @brief      This function servers to i3c master restart without rxsize.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  type        - i3c_bus_type_e.
 * @param[in]  address     - the slave id.
 * @param[in]  dir         - i3c_direction_e
 * @param[in]  rx_size     - the read size.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_restart(i3c_chn_e chn, i3c_bus_type_e type, unsigned char address, i3c_direction_e dir);

/**
 * @brief      This function servers to i3c master start with rxsize.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  type        - i3c_bus_type_e.
 * @param[in]  address     - the slave id.
 * @param[in]  dir         - i3c_direction_e
 * @param[in]  rx_size     - the read size.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_start_with_rxsize (i3c_chn_e chn, i3c_bus_type_e type, unsigned char address, i3c_direction_e dir, unsigned char rx_size);

/**
 * @brief      This function servers to i3c master start without rxsize.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  type        - i3c_bus_type_e.
 * @param[in]  address     - the slave id.
 * @param[in]  dir         - i3c_direction_e
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_start(i3c_chn_e chn, i3c_bus_type_e type, unsigned char address, i3c_direction_e dir);

/**
 * @brief      This function servers to i3c master wait for ctrl done.
 * @param[in]  chn         - i3c_chn_e.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_wait_for_ctrl_done(i3c_chn_e chn,unsigned char wait_idle);

/**
 * @brief      This function servers to i3c master wait for complete.
 * @param[in]  chn         - i3c_chn_e.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_wait_complete(i3c_chn_e chn, unsigned char wait_idle);

/**
 * @brief      This function servers to i3c master emit stop.
 * @param[in]  chn         - i3c_chn_e.
 * @return     i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_emitstop(i3c_chn_e chn, unsigned char wait_idle);

/*!
 * @brief This function was used to i3c master write.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return      i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_write(i3c_chn_e chn,i3c_master_transfer_t* transfer);

/*!
 * @brief This function was used to i3c master read.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return      i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_read(i3c_chn_e chn,i3c_master_transfer_t* transfer);

/*!
 * @brief This function was used to i3c master get ibi type.
 * @param[in]  chn         - i3c_chn_e.
 * return      i3c_ibi_type_e.
 */
i3c_ibi_type_e i3c_master_get_ibi_type(i3c_chn_e chn);

/*!
 * @brief This function was used to i3c master write by dma.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return      i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_write_dma(i3c_chn_e chn, i3c_master_transfer_t *transfer);

/*!
 * @brief This function was used to i3c master read by dma.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  transfer    - i3c_master_transfer_t.
 * return      i3c_common_return_status_e.
 */
i3c_common_return_status_e i3c_master_read_dma(i3c_chn_e chn,i3c_master_transfer_t *transfer);

/*!
 * @brief This function was used to I3C master register IBI rule.
 * @param[in]  chn         - i3c_chn_e.
 * @param[in]  ibiRule     - i3c_register_ibi_addr_t
 */
void i3c_master_register_ibi(i3c_chn_e chn, i3c_register_ibi_addr_t *ibiRule);

/*!
 * brief This function server to when does the slave capture data event occur on the master's timeline.
 * @param[in]   chn                   - i3c_chn_e.
 * @param[in]   ibi_complete_point    - master ibi completion point
 * @param[in]   ibi_timing_data       - Two points in time from the start of the slave segment ibi to ack.
 */
unsigned int i3c_master_get_sample_point_tick(i3c_chn_e chn,unsigned int ibi_complete_point,unsigned char* ibi_timing_data);

/****************************************************************** slave ******************************************/
typedef struct
{
    _Bool is_master_capable;         /*!< Whether to enable slave master capable. */
    _Bool is_hotjoin;                /*!< Whether to enable slave hotjoin before enable slave. */
    unsigned char static_addr;       /*!< Static address. */
    _Bool is_ignore_s0s1_error;      /*!< Whether to ignore S0/S1 error in SDR mode. */
    _Bool offline;                   /*!< Whether to wait 60 us of bus quiet or HDR request to ensure slave track SDR mode safely. */
    _Bool is_match_slave_startstop;  /*!< Whether to assert start/stop status only the time slave is addressed. */
    i3c_clk_src_t  clk_src;
} i3c_slave_config_t;

/*!
 * @brief I3C slave peripheral flags.
 *
 * The following status register flags can be cleared:
 * - #I3C_SLAVE_BUSSTART_FLAG
 * - #I3C_SLAVE_MATCHED_FLAG
 * - #I3C_SLAVE_BUSSTOP_FLAG
 *
 * Only below flags can be enabled as interrupts.
 * - #I3C_SLAVE_BUSSTART_FLAG
 * - #I3C_SLAVE_MATCHED_FLAG
 * - #I3C_SLAVE_BUSSTOP_FLAG
 * - #I3C_SLAVE_RXREADY_FLAG
 * - #I3C_SLAVE_TXREADY_FLAG
 * - #I3C_SLAVE_DYNAMIC_ADDRCHANGED_FLAG
 * - #I3C_SLAVE_RECEIVED_CCC_FLAG
 * - #I3C_SLAVE_ERROR_FLAG
 * - #kI3C_SlaveHDRCommandMatchFlag
 * - #I3C_SLAVE_CCCHANDLED_FLAG
 * - #I3C_SLAVE_EVENTSENT_FLAG

 * @note These enums are meant to be OR'd together to form a bit mask.
 */
typedef enum{
    I3C_SLAVE_NOTSTOP_FLAG                  = BIT(0),     /*!< Slave status not stop flag */
    I3C_SLAVE_MESSAGE_FLAG                  = BIT(1),     /*!< Slave status message, indicating slave is listening to the bus traffic or responding */
    I3C_SLAVE_REQUIRED_READ_FLAG            = BIT(3),     /*!< Slave status required, either is master doing SDR read from slave, or is IBI pushing out. */
    I3C_SLAVE_REQUIRED_WRITE_FLAG           = BIT(4),     /*!< Slave status request write, master is doing SDR                                                             write to slave, except slave in ENTDAA mode */
    I3C_SLAVE_BUSDAA_FLAG                   = BIT(5),     /*!< I3C bus is in ENTDAA mode */
    I3C_SLAVE_BUSSTART_FLAG                 = BIT(8),     /*!< Start/Re-start event is seen since the bus was last cleared */
    I3C_SLAVE_MATCHED_FLAG                  = BIT(9),     /*!< Slave address(dynamic/static) matched since last cleared */
    I3C_SLAVE_BUSSTOP_FLAG                  = BIT(10),    /*!<Stop event is seen since the bus was last cleared */
    I3C_SLAVE_RXREADY_FLAG                  = BIT(11),    /*!< Rx data ready in rx buffer flag */
    I3C_SLAVE_TXREADY_FLAG                  = BIT(12),    /*!< Tx buffer ready for Tx data flag */
    I3C_SLAVE_DYNAMIC_ADDRCHANGED_FLAG      = BIT(13),    /*!< Slave dynamic address has been assigned, re-assigned, or lost */
    I3C_SLAVE_RECEIVED_CCC_FLAG             = BIT(14),    /*!< Slave received Common command code */
    I3C_SLAVE_ERROR_FLAG                    = BIT(15),    /*!< Error occurred flag */
    I3C_SLAVE_CCCHANDLED_FLAG               = BIT(17),    /*!< Slave received Common command code is handled by I3C module */
    I3C_SLAVE_EVENTSENT_FLAG                = BIT(18),    /*!< Slave IBI/P2P/MR/HJ event has been sent */
    I3C_SLAVE_TGTRST_FLAG                   = BIT(19),    /*!< Slave reset */
    I3C_SLAVE_IBI_DISABLE_FLAG              = BIT(24),    /*!< Slave in band interrupt is disabled. */
    I3C_SLAVE_MASTER_REQUSET_DISABLE_FLAG   = BIT(25),    /*!< Slave master request is disabled. */
    I3C_SLAVE_HOTJOIN_DISABLE_FLAG          = BIT(29),    /*!< Slave Hot-Join is disabled. */
    /*! All flags which are cleared by the driver upon starting a transfer. */
    I3C_SLAVE_CLEAR_FLAGS  = I3C_SLAVE_BUSSTART_FLAG | I3C_SLAVE_MATCHED_FLAG | I3C_SLAVE_BUSSTOP_FLAG,

    I3C_SLAVE_ALLIRQ_FLAGS = I3C_SLAVE_BUSSTART_FLAG | I3C_SLAVE_MATCHED_FLAG | I3C_SLAVE_BUSSTOP_FLAG |
                             I3C_SLAVE_RXREADY_FLAG  | I3C_SLAVE_DYNAMIC_ADDRCHANGED_FLAG |
                             I3C_SLAVE_RECEIVED_CCC_FLAG | I3C_SLAVE_ERROR_FLAG  |
                             I3C_SLAVE_CCCHANDLED_FLAG | I3C_SLAVE_EVENTSENT_FLAG|I3C_SLAVE_TGTRST_FLAG,
}i3c_slave_flags;


/*!
 * @brief I3C slave error flags to indicate the causes.
 * @note These enums are meant to be OR'd together to form a bit mask.
 */
typedef enum
{
    I3C_SLAVE_ERROR_OVERRUN_FLAG        = BIT(0), /*!< Slave internal from-bus buffer/FIFO overrun. */
    I3C_SLAVE_ERROR_UNDERRUN_FLAG       = BIT(1), /*!< Slave internal to-bus buffer/FIFO underrun */
    I3C_SLAVE_ERROR_UNDERRUN_NACK_FLAG  = BIT(2), /*!< Slave internal from-bus buffer/FIFO underrun and NACK error */
    I3C_SLAVE_ERROR_TERM_FLAG           = BIT(3), /*!< Terminate error from master */
    I3C_SLAVE_ERROR_INVALIDSTRAT_FLAG   = BIT(4), /*!< Slave invalid start flag */
    I3C_SLAVE_ERROR_SDRPARITY_FLAG      = BIT(8), /*!< SDR parity error */
    I3C_SLAVE_ERROR_S0S1_FLAG           = BIT(11),/*!< S0 or S1 error */
    I3C_SLAVE_ERROR_OVERREAD_FLAG       = BIT(16),/*!< Over-read error */
    I3C_SLAVE_ERROR_OVERWRITE_FLAG      = BIT(17),/*!< Over-write error */
}i3c_slave_error_flags;

/*! @brief I3C slave.event */
typedef enum
{
    I3C_SLAVE_EVENT_NORMAL       = 0, /*!< Normal mode. */
    I3C_SLAVE_EVENT_IBI          = 1, /*!< In band interrupt event. */
    I3C_SLAVE_EVENT_MASTER_REQ   = 2, /*!< Master request event. */
    I3C_SLAVE_EVENT_HOTJOIN_REQ  = 3, /*!< Hot-join event. */
} i3c_slave_event_e;

/*! @brief I3C slave.mapped address setting */
typedef struct{
    unsigned int i3c_map_ena :1;
    unsigned int i3c_map_addr:7;
    unsigned int i3c_map_mapsa:1;
    unsigned int i3c_map_sa10b:3;
    unsigned int i3c_map_nack:1;
    unsigned int i3c_map_auto:1;
    unsigned int i3c_map_pid:10;
    unsigned int i3c_map_dcr:8;
}i3c_slave_mapped_config_t;

/**
 * @brief      This function serves to enable i3c slave mask function.
 * @param[in]  chn  - i3c_chn_e.
 * @param[in]  mask - to select interrupt type.
 * @return     none
 */
static inline void i3c_slave_set_irq_mask(i3c_chn_e chn,i3c_slave_flags mask){
    reg_i3c_intset(chn)  |= mask;
}

/**
 * @brief      This function serves to get i3c slave mask function.
 * @param[in]  chn  - i3c_chn_e.
 * @return     enabled mask.
 */
static inline unsigned int i3c_slave_get_irq_mask(i3c_chn_e chn){
    return reg_i3c_intset(chn);
}

/**
 * @brief      This function serves to clear i3c slave mask function.
 * @param[in]  chn  - i3c_chn_e.
 * @param[in]  mask - to select Interrupt type.
 * @return     none
 */
static inline void i3c_slave_clr_irq_mask(i3c_chn_e chn,i3c_slave_flags mask){
    reg_i3c_intclr(chn)  = mask;
}

/**
 * @brief      This function serves to get i3c slave interrupt status.
 * @param[in]  chn  - i3c_chn_e.
 * @return     interrupt status.
 */
static inline unsigned int i3c_slave_get_irq_status(i3c_chn_e chn){
    return reg_i3c_status(chn);
}

/**
 * @brief      This function serves to clear i3c slave interrupt status.
 * @param[in]  chn  - i3c_chn_e.
 * @return     interrupt status.
 */
static inline void i3c_slave_clr_irq_status(i3c_chn_e chn,i3c_slave_flags flag){
    reg_i3c_status(chn) =  flag;
}

/*!
 * @brief This function serves to sets the watermarks for I3C slave FIFOs.
 * @param base The I3C peripheral base address.
 * @param tx_lvl - Transmit FIFO watermark level.
 *        The #I3C_SLAVE_TXREADY_FLAG flag is set whenever the number of words in the transmit FIFO reaches @a tx_lvl.
 * @param rx_lvl - Receive FIFO watermark level.
 *        The #I3C_SLAVE_RXREADY_FLAG flag is set whenever the number of words in the receive FIFO reaches @a rx_lvl.
 */
static inline void i3c_slave_set_watermarks(i3c_chn_e chn, i3c_tx_trigger_level_e tx_lvl, i3c_rx_trigger_level_e rx_lvl)
{
    reg_i3c_sdatactrl(chn) = (reg_i3c_sdatactrl(chn) & (~ FLD_I3C_S_TXTRIG |FLD_I3C_S_RXTRIG))|((tx_lvl << 4)| (rx_lvl<<6)) | FLD_I3C_S_UNLOCK;
}

/**
 * @brief      This function serves to flush slave txfifo.
 * @param[in]  chn  - i3c_chn_e.
 */
static inline void i3c_slave_flush_txfifo(i3c_chn_e chn){
    reg_i3c_sdatactrl(chn) =   FLD_I3C_S_FLUSHTB;
}

/**
 * @brief      This function serves to flush slave rxfifo.
 * @param[in]  chn  - i3c_chn_e.
 */
static inline void i3c_slave_flush_rxfifo(i3c_chn_e chn){
    reg_i3c_sdatactrl(chn)  = FLD_I3C_S_FLUSHFB;
}

/*!
 * @brief This function serves to gets the current number of bytes in the I3C slave FIFOs.
 * @param[in]  chn  - i3c_chn_e.
 * @param[out] txCount Pointer through which the current number of bytes in the transmit FIFO is returned.
 *             Pass NULL if this value is not required.
 * @param[out] rxCount Pointer through which the current number of bytes in the receive FIFO is returned.
 *             Pass NULL if this value is not required.
 */
static inline void i3c_slave_get_fifo_counts(i3c_chn_e chn, unsigned char *rx_count, unsigned char *tx_count)
{
    if (NULL != tx_count)
    {
        *tx_count = (reg_i3c_sdatactrl(chn) & FLD_I3C_S_TXCOUNT) >> 16;
    }
    if (NULL != rx_count)
    {
        *rx_count = (reg_i3c_sdatactrl(chn) & FLD_I3C_S_RXCOUNT) >> 24;
    }
}

/**
 * @brief      This function serves to slave write byte.
 * @param[in]  chn   - i3c_chn_e.
 * @param[in]  byte  - the write byte.
 */
static inline void i3c_slave_write_byte(i3c_chn_e chn,unsigned char byte){
    reg_i3c_swdatab(chn) = byte;
}

/**
 * @brief      This function serves to slave write the end byte.
 * @param[in]  chn  - i3c_chn_e.
 * @param[in]  byte  - the write end byte.
 */
static inline void i3c_slave_write_end_byte(i3c_chn_e chn,unsigned char byte_end){
    reg_i3c_wdatabe(chn) = byte_end;
}

/**
 * @brief      This function serves to slave read data.
 * @param[in]  chn  - i3c_chn_e.
 * @return     the read data.
 */
static inline unsigned char i3c_slave_read_byte(i3c_chn_e chn){
    return  reg_i3c_s_rdatab(chn);
}

/**
 * @brief      This function serves to set slave map configure.
 * @param[in]  chn          - i3c_chn_e.
 * @param[in]  mapped_flag  - the mapped address flag.
 * @param[in]  mapped_cfg   - the mapped configure.
 */
static inline void i3c_slave_set_mapped(i3c_chn_e chn,unsigned char mapped_flag, i3c_slave_mapped_config_t *mapped_cfg){
    reg_i3c_maptrl(chn,mapped_flag) = *(unsigned int*)mapped_cfg;
}

/**
 * @brief      This function selects a pin port for I3C slave interface.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  sda_pin        - the pin port selected as I3C sda pin port.
 * @param[in]  scl_pin        - the pin port selected as I3C scl pin port.
 * @return     none
 */
void i3c_slave_set_pin(i3c_chn_e chn,gpio_func_pin_e sda_pin,gpio_func_pin_e scl_pin);

/**
 * @brief      This function server to initialize i3c slave.
 * @param[in]  chn            - i3c_chn_e.
 * @param[in]  slaveConfig    - i3c_slave_config_t.
 * @return     none
 */
void i3c_slave_init(i3c_chn_e chn,const i3c_slave_config_t *slave_config);

/*!
 * brief This function server to I3C slave request event.
 * @param[in]  chn    - i3c_chn_e.
 * @param[in]   event  - I3C slave event of type #i3c_slave_event_e
 */
void i3c_slave_request_event(i3c_chn_e chn, i3c_slave_event_e event);

/*!
 * brief This function server to I3C slave request IBI event with data payload(mandatory and extended).
 * @param[in]  chn      - i3c_chn_e.
 * @param[in]  data     - Pointer to IBI data to be sent in the request.
 * @param[in] data_size - IBI data size.
 */
void i3c_slave_request_ibi_with_data(i3c_chn_e chn, unsigned char *data, unsigned char data_size);

/*!
 * brief This function server to I3C slave tx data by dma.
 * @param[in]  chn      - i3c_chn_e.
 * @param[in]  data     - the tx data.
 * @param[in] data_size - the data size.
 */
void i3c_slave_set_tx_dma(i3c_chn_e chn,unsigned char *data, unsigned int data_size);

/*!
 * brief This function server to I3C slave rx data by dma.
 * @param[in]  chn      - i3c_chn_e.
 * @param[out] data     - the tx data.
 * @param[in] data_size - the data size.
 */
void i3c_slave_set_rx_dma(i3c_chn_e chn,unsigned char *data,unsigned int data_size);

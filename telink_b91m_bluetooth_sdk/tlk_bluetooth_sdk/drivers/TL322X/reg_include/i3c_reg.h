/********************************************************************************************************
 * @file    i3c_reg.h
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
#include "soc.h"
/*******************************      i3c0 registers: 0x80141400      ******************************/
/*******************************      i3c1 registers: 0x80241000      ******************************/

#define i3c_base_addr(i)                   ((i)  ?   (0x80241000):(0x80141400)) /* i3c[0-1] */
/******************************************  master  ****************************************************/
#define reg_i3c_mconfig(i)         REG_ADDR32(i3c_base_addr(i)+0x00)
enum{
    FLD_I3C_M_CTRENA            =    BIT_RNG(0,1),
    FLD_I3C_M_DISTO             =    BIT(3),
    FLD_I3C_M_ODSTOP            =    BIT(6),
    FLD_I3C_M_PPBAUD            =    BIT_RNG(8,11),
    FLD_I3C_M_PPLOW             =    BIT_RNG(12,15),
    FLD_I3C_M_ODBAUD            =    BIT_RNG(16,23),
    FLD_I3C_M_ODHPP             =    BIT(24),
    FLD_I3C_M_I2CBAUD           =    BIT_RNG(28,31),
};
#define reg_i3c_capabilities_2(i)   REG_ADDR32(i3c_base_addr(i)+0x5c)
enum{
    FLD_I3C_MAPCNT          = BIT_RNG(0,3),
    FLD_I3C_I2C0B           = BIT(4),
    FLD_I3C_IBIEXT          = BIT(8),
    FLD_I3C_IBIXREG         = BIT(9),
    FLD_I3C_SMLINE          = BIT_RNG(12,13),
    FLD_I3C_V1_1            = BIT(16),
    FLD_I3C_TGTRST          = BIT(17),
    FLD_I3C_GROUP           = BIT_RNG(18,19),
    FLD_I3C_AASA            = BIT(21),

};

#define reg_i3c_capabilities(i)    REG_ADDR32(i3c_base_addr(i)+0x60)
enum{
    FLD_I3C_IDENA          = BIT_RNG(0,1),
    FLD_I3C_IDREG          = BIT_RNG(2,5),
    FLD_I3C_CNTLR          = BIT(9),
    FLD_I3C_CAP_SADDR      = BIT_RNG(10,11),
    FLD_I3C_CCCHANDLE      = BIT_RNG(12,15),
    FLD_I3C_IBI_MR_HJ      = BIT_RNG(16,20),
    FLD_I3C_TIMECTRL       = BIT(21),
    FLD_I3C_FIFOTX         = BIT_RNG(26,27),
    FLD_I3C_FIFORX         = BIT_RNG(28,29),
    FLD_I3C_INT_SUPPORT    = BIT(30),
    FLD_I3C_DMA_SUPPORT    = BIT(31),
};

#define reg_i3c_mctrl(i)          REG_ADDR32(i3c_base_addr(i)+0x84)
enum{
    FLD_I3C_M_REQUEST          =  BIT_RNG(0,2),
    FLD_I3C_M_CTRL_TYPE        =  BIT_RNG(4,5),
    FLD_I3C_M_IBIRESP          =  BIT_RNG(6,7),
    FLD_I3C_M_DIR              =  BIT(8),
    FLD_I3C_M_ADDR             =  BIT_RNG(9,15),
    FLD_I3C_M_RDTERM           =  BIT_RNG(16,23),
};

//status flags
#define reg_i3c_mstatus(i)       REG_ADDR32(i3c_base_addr(i)+0x88)
enum{
    FLD_I3C_M_STATE           =  BIT_RNG(0,2),
    FLD_I3C_M_BETWEEN         =  BIT(4),
    FLD_I3C_M_NACKED          =  BIT(5),
    FLD_I3C_M_IBITYPE         =  BIT_RNG(6,7),
    FLD_I3C_M_TGTSTART        =  BIT(8),
    FLD_I3C_M_MCTRLDONE       =  BIT(9),
    FLD_I3C_M_COMPLETE        =  BIT(10),
    FLD_I3C_M_RXPEND          =  BIT(11),
    FLD_I3C_M_TXNOTFULL       =  BIT(12),
    FLD_I3C_M_IBIWON          =  BIT(13),
    FLD_I3C_M_ERRWARN         =  BIT(15),
    FLD_I3C_M_IBIADDR         =  BIT_RNG(24,30),
};

#define reg_i3c_ibirules(i)   REG_ADDR32(i3c_base_addr(i)+0x8c)
enum{
    FLD_I3C_ADDR0             = BIT_RNG(0,5),
    FLD_I3C_ADDR1_0           = BIT_RNG(6,7),
    FLD_I3C_ADDR1_1           = BIT_RNG(8,11),
    FLD_I3C_ADDR2_0           = BIT_RNG(12,15),
    FLD_I3C_ADDR2_1           = BIT_RNG(16,17),
    FLD_I3C_ADDR3             = BIT_RNG(18,23),
    FLD_I3C_ADDR4             = BIT_RNG(24,29),
    FLD_I3C_MSB0              = BIT(30),
    FLD_I3C_NOBYTE            = BIT(31),
};

//set mask
#define reg_i3c_mintset(i)    REG_ADDR32(i3c_base_addr(i)+0x90)

//clr mask
#define reg_i3c_mintclr(i)    REG_ADDR32(i3c_base_addr(i)+0x94)

//err status flag
#define reg_i3c_merrwarn(i)   REG_ADDR32(i3c_base_addr(i)+0x9c)
enum{
    FLD_I3C_M_NACK       =  BIT(2),//It won't continue to send and receive data. The clk should be gone after Nack.
    FLD_I3C_M_WRABT      =  BIT(3),
/*
 * Why is there a term interrupt on the master and slave side for msg mode but not for mctrl:
 * 1. ctrl is controlled, master side can always terminate by configuring the length to 1, slave side should be able to terminate by end data;
 * 2. msg has no mechanism for ctrl;
 */
    FLD_I3C_M_TERM       =  BIT(4),
    FLD_I3C_M_OREAD      =  BIT(16),
    FLD_I3C_M_OWRITE     =  BIT(17),
    FLD_I3C_M_MSGERR     =  BIT(18),
    FLD_I3C_M_INVREQ     =  BIT(19),
    FLD_I3C_M_TIMEOUT    =  BIT(20),
    FLD_I3C_M_WRONGSIZE  =  BIT(21),
};

#define reg_i3c_mdmactrl(i)     REG_ADDR32(i3c_base_addr(i)+0xa0)
enum{
    FLD_I3C_M_DMAFB     = BIT_RNG(0,1),
    FLD_I3C_M_DMATB     = BIT_RNG(2,3),
    FLD_I3C_M_DMAWIDTH  = BIT_RNG(4,5),
};

#define reg_i3c_mdatactrl(i)    REG_ADDR32(i3c_base_addr(i)+0xac)
enum{
    FLD_I3C_M_FLUSHTB    = BIT(0),
    FLD_I3C_M_FLUSHFB    = BIT(1),
    FLD_I3C_M_UNLOCK     = BIT(3),
    FLD_I3C_M_TXTRIG     = BIT_RNG(4,5),
    FLD_I3C_M_RXTRIG     = BIT_RNG(6,7),
    FLD_I3C_M_TXCOUNT    = BIT_RNG(16,23),
    FLD_I3C_M_RXCOUNT    = BIT_RNG(24,29),
    FLD_I3C_M_TXFULL     = BIT(30),
    FLD_I3C_M_RXEMPTY    = BIT(31),
};

#define reg_i3c_mwdatab(i)        REG_ADDR32(i3c_base_addr(i)+0xb0)
enum{
    FLD_I3C_MWDATAB   = BIT_RNG(0,7),
    FLD_I3C_MWDATAB_1 = BIT(8),
    FLD_I3C_MWDATAB_2 = BIT(16),
};

#define reg_i3c_mwdatabe(i)   REG_ADDR32(i3c_base_addr(i)+0xb4)


#define reg_i3c_mwdata_h(i)   REG_ADDR32(i3c_base_addr(i)+0xb8)
enum{
    FLD_I3C_MWDATA_H0    = BIT_RNG(0,7),
    FLD_I3C_MWDATA_H1    = BIT_RNG(8,15),
    FLD_I3C_MWEND1       = BIT(16),
};

#define reg_i3c_mwdata_he(i)  REG_ADDR32(i3c_base_addr(i)+0xbc)



#define reg_i3c_mrdatab_0(i)    REG_ADDR32(i3c_base_addr(i)+0xc0)
enum{
    FLD_I3C_MRDATA_B   = BIT_RNG(0,7),
};
/**************************************** slave ***************************************************/
#define reg_i3c_config(i)       REG_ADDR32(i3c_base_addr(i)+0x04)
enum{
    FLD_I3C_S_TGTENA    = BIT(0),
    FLD_I3C_S_MATCHSS   = BIT(2),
    FLD_I3C_S_S0IGNORE  = BIT(3),
    FLD_I3C_S_OFFLINE   = BIT(9),
    FLD_I3C_S_BAMATCH   = BIT_RNG(16,23),
    FLD_I3C_S_SADDR     = BIT_RNG(25,31),
};

#define reg_i3c_status(i)        REG_ADDR32(i3c_base_addr(i)+0x08)
enum{
    FLD_I3C_S_STNOTSTOP  = BIT(0),
    FLD_I3C_S_STMSG      = BIT(1),
    FLD_I3C_S_STCCCH     = BIT(2),
    FLD_I3C_S_STREQRD    = BIT(3),
    FLD_I3C_S_STREQWR    = BIT(4),
    FLD_I3C_S_STDAA      = BIT(5),
    FLD_I3C_S_START_DTC  = BIT(8),
    FLD_I3C_S_MATCHED    = BIT(9),
    FLD_I3C_S_STOP_DTC   = BIT(10),
    FLD_I3C_S_RXPEND     = BIT(11),
    FLD_I3C_S_TXNOTFULL  = BIT(12),
    FLD_I3C_S_DACHG      = BIT(13),
    FLD_I3C_S_CCC        = BIT(14), //The I3C_11_AC received a CCC that it does not automatically handle.
    FLD_I3C_S_ERRWARN    = BIT(15),
    FLD_I3C_S_CHANDLED   = BIT(17), //The I3C_11_AC handled a CCC.
                                   //change as a result of the CCC.
    FLD_I3C_S_EVENT_REQ  = BIT(18),
    FLD_I3C_S_TGTRST     = BIT(19),
    FLD_I3C_S_EVDET      = BIT_RNG(20,21),
    FLD_I3C_S_IBIDIS     = BIT(24),
    FLD_I3C_S_MRDIS      = BIT(25),
    FLD_I3C_S_HJDIS      = BIT(27),
    FLD_I3C_S_TIMECTRL   = BIT_RNG(30,31),
};

#define reg_i3c_ctrl(i)          REG_ADDR32(i3c_base_addr(i)+0x0c)
enum{
    FLD_I3C_EVENT_REQ      = BIT_RNG(0,1),
    FLD_I3C_EXTDATA        = BIT(3),
    FLD_I3C_MAPIDX         = BIT_RNG(4,7),
    FLD_I3C_IBIDATA        = BIT_RNG(8,15),
};

#define reg_i3c_intset(i)     REG_ADDR32(i3c_base_addr(i)+0x10)
#define reg_i3c_intclr(i)     REG_ADDR32(i3c_base_addr(i)+0x14)

#define reg_i3c_sdmactrl(i)     REG_ADDR32(i3c_base_addr(i)+0x20)
enum{
    FLD_I3C_S_DMAFB     =   BIT_RNG(0,1),
    FLD_I3C_S_DMATB     =   BIT_RNG(2,3),
    FLD_I3C_S_DMAWIDTH  =   BIT_RNG(4,5),
};

#define reg_i3c_sdatactrl(i)    REG_ADDR32(i3c_base_addr(i)+0x2c)
enum{
    FLD_I3C_S_FLUSHTB     = BIT(0),
    FLD_I3C_S_FLUSHFB     = BIT(1),
    FLD_I3C_S_UNLOCK      = BIT(3),
    FLD_I3C_S_TXTRIG      = BIT_RNG(4,5),
    FLD_I3C_S_RXTRIG      = BIT_RNG(6,7),
    FLD_I3C_S_TXCOUNT     = BIT_RNG(16,21),
    FLD_I3C_S_RXCOUNT     = BIT_RNG(24,29),
};

#define reg_i3c_swdatab(i)      REG_ADDR32(i3c_base_addr(i)+0x30)
enum{
    FLD_I3C_S_WDATA_B    = BIT_RNG(0,7),
    FLD_I3C_S_WEND1      = BIT(8),
    FLD_I3C_S_WEND2      = BIT(16),
};


#define reg_i3c_wdatabe(i)       REG_ADDR32(i3c_base_addr(i)+0x34)
enum{
    FLD_I3C_S_WDATA_BE    = BIT_RNG(0,7),
};

#define reg_i3c_s_rdatab(i)     REG_ADDR32(i3c_base_addr(i)+0x40)

#define reg_i3c_msglast(i)      REG_ADDR32(i3c_base_addr(i)+0x7c)
enum{
    FLD_I3C_MAPLAST      = BIT_RNG(0,3),
    FLD_I3C_LASTSTATIC   = BIT(4),
    FLD_I3C_LASTGROUP    = BIT(5),
    FLD_I3C_MAPLASTM1    = BIT_RNG(8,11),
    FLD_I3C_LASTGROUPM1  = BIT(5),
    FLD_I3C_MAPLASTM2    = BIT_RNG(16,19),
    FLD_I3C_LASTGROUPM2  = BIT(21),
};


#define reg_i3c_groupdef(i)     REG_ADDR32(i3c_base_addr(i)+0x114)
enum{
    FLD_I3C_GRP0ENA      =  BIT(0),
    FLD_I3C_GRP0DA       =  BIT_RNG(1,7),
    FLD_I3C_GRP1ENA      =  BIT(8),
    FLD_I3C_GRP1DA       =  BIT_RNG(9,15),
    FLD_I3C_GRP2ENA      =  BIT(16),
    FLD_I3C_GRP2DA       =  BIT_RNG(17,23),
};

#define reg_i3c_maptrl(i,j)      REG_ADDR32(i3c_base_addr(i)+0x120+4*j)
enum{
    FLD_I3C_ENA      = BIT(0),
    FLD_I3C_ADDR     = BIT_RNG(1,7),
    FLD_I3C_MAPSA    = BIT(8),
    FLD_I3C_SA10B    = BIT_RNG(9,11),
    FLD_I3C_NACK     = BIT(12),
    FLD_I3C_AUTO     = BIT(13),
    FLD_I3C_PID1     = BIT_RNG(14,15),
    FLD_I3C_PID2     = BIT_RNG(16,23),
    FLD_I3C_DCR      = BIT_RNG(24,31),
};

#define reg_i3c_ibiext1(i)      REG_ADDR32(i3c_base_addr(i)+0x140)
enum{
    FLD_I3C_CNT          = BIT_RNG(0,2),
    FLD_I3C_EXT1         = BIT_RNG(8,15),
    FLD_I3C_EXT2         = BIT_RNG(16,23),
    FLD_I3C_EXT3         = BIT_RNG(24,31),
};

#define reg_i3c_ibiext2(i)      REG_ADDR32(i3c_base_addr(i)+0x144)
enum{
    FLD_I3C_EXT4         = BIT_RNG(0,7),
    FLD_I3C_EXT5         = BIT_RNG(8,15),
    FLD_I3C_EXT6         = BIT_RNG(16,23),
    FLD_I3C_EXT7         = BIT_RNG(24,31),
};



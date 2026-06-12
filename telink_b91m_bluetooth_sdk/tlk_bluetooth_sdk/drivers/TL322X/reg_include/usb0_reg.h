/********************************************************************************************************
 * @file    usb0_reg.h
 *
 * @brief   This is the header file for tl322x
 *
 * @author  Driver Group
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
#ifndef __USB0_REG_H__
#define __USB0_REG_H__
#include "soc.h"

/******************************* USB registers: 0x280000 *******************************/
#define REG_USB0_BASE 0x280000

/******************************* Global *******************************/
#define reg_usb_gotgctl REG_ADDR32(REG_USB0_BASE + 0x00)

enum
{
    FLD_USB_GOTGCTL_MULTVALIDBC         = BIT_RNG(22, 26),
    FLD_USB_GOTGCTL_CHIRPEN             = BIT(27),
    FLD_USB_GOTGCTL_EUSB2_PHY_DISC_SUPP = BIT(28),
    FLD_USB_GOTGCTL_TESTMODE_CORR_EUSB2 = BIT(31),
};

#define reg_usb_gotgint REG_ADDR32(REG_USB0_BASE + 0x04)

enum
{
    FLD_USB_GOTGINT_MULTVALIPCHNG = BIT(20),
};

#define reg_usb_gahbcfg REG_ADDR32(REG_USB0_BASE + 0x08)

enum
{
    FLD_USB_GAHBCFG_GLBLINTRMSK             = BIT(0),
    FLD_USB_GAHBCFG_HBSTLEN                 = BIT_RNG(1, 4),
    FLD_USB_GAHBCFG_DMAEN                   = BIT(5),
    FLD_USB_GAHBCFG_REMMEMSUPP              = BIT(21),
    FLD_USB_GAHBCFG_NOTIALLDMAWRIT          = BIT(22),
    FLD_USB_GAHBCFG_AHBSINGLE               = BIT(23),
    FLD_USB_GAHBCFG_INVDESCENDIANESS        = BIT(24),
    FLD_USB_GAHBCFG_LOA_EOP_CHECK_CLKS_BYTE = BIT_RNG(25, 26),
    FLD_USB_GAHBCFG_LOA_EOP_CHECK_CLKS_WORD = BIT_RNG(27, 28),
};

#define reg_usb_gusbcfg REG_ADDR32(REG_USB0_BASE + 0x0c)

enum
{
    FLD_USB_GUSBCFG_TOUTCAL       = BIT_RNG(0, 2),
    FLD_USB_GUSBCFG_DDRSEL        = BIT(7),
    FLD_USB_GUSBCFG_USBTRDTIM     = BIT_RNG(10, 13),
    FLD_USB_GUSBCFG_PHYLPWRCLKSEL = BIT(15),
    FLD_USB_GUSBCFG_ULPIAUTORES   = BIT(18),
    FLD_USB_GUSBCFG_ULPICLKSUSM   = BIT(19),
    FLD_USB_GUSBCFG_TXENDDELAY    = BIT(28),
    FLD_USB_GUSBCFG_CORRUPTTXPKT  = BIT(31),
};

#define reg_usb_grstctl REG_ADDR32(REG_USB0_BASE + 0x10)

enum
{
    FLD_USB_GRSTCTL_CSFTRST     = BIT(0),
    FLS_USB_GRSTCTL_PIUFSSFTRST = BIT(1),
    FLD_USB_GRSTCTL_RXFFLSH     = BIT(4),
    FLD_USB_GRSTCTL_TXFFLSH     = BIT(5),
    FLD_USB_GRSTCTL_TXFNUM      = BIT_RNG(6, 10), /**< 0x10: Flush all the transmit FIFOs. */
    FLD_USB_GRSTCTL_CSFTRSTDONE = BIT(29),
    FLD_USB_GRSTCTL_DMAREQ      = BIT(30),
    FLD_USB_GRSTCTL_AHBIDLE     = BIT(31),
};

#define reg_usb_gintsts REG_ADDR32(REG_USB0_BASE + 0x14)

typedef enum
{
    FLD_USB_GINTSTS_SOF         = BIT(3),
    FLD_USB_GINTSTS_RXFLVL      = BIT(4),
    FLD_USB_GINTSTS_GINNAKEFF   = BIT(6),
    FLD_USB_GINTSTS_GOUTNAKEFF  = BIT(7),
    FLD_USB_GINTSTS_ERLYSUSP    = BIT(10),
    FLD_USB_GINTSTS_USBSUSP     = BIT(11),
    FLD_USB_GINTSTS_USBRST      = BIT(12),
    FLD_USB_GINTSTS_ENUMDONE    = BIT(13),
    FLD_USB_GINTSTS_ISOOUTDROP  = BIT(14),
    FLD_USB_GINTSTS_EOPF        = BIT(15),
    FLD_USB_GINTSTS_RSTRDONEINT = BIT(16),
    FLD_USB_GINTSTS_IEPINT      = BIT(18),
    FLD_USB_GINTSTS_OEPINT      = BIT(19),
    FLD_USB_GINTSTS_FETSUSP     = BIT(22),
    FLD_USB_GINTSTS_RESETDET    = BIT(23),
    FLD_USB_GINTSTS_LPM_INT     = BIT(27),
    FLD_USB_GINTSTS_WKUPINT     = BIT(31),
} usb_gintsts_e;

#define reg_usb_gintmsk REG_ADDR32(REG_USB0_BASE + 0x18)

typedef enum
{
    FLD_USB_GINTMSK_OTGINTMSK      = BIT(2),
    FLD_USB_GINTMSK_SOFMSK         = BIT(3),
    FLD_USB_GINTMSK_RXFLVLMSK      = BIT(4),
    FLD_USB_GINTMSK_GINNAKEFFMSK   = BIT(6),
    FLD_USB_GINTMSK_GOUTNAKEFFMSK  = BIT(7),
    FLD_USB_GINTMSK_ERLYSUSPMSK    = BIT(10),
    FLD_USB_GINTMSK_USBSUSPMSK     = BIT(11),
    FLD_USB_GINTMSK_USBRSTMSK      = BIT(12),
    FLD_USB_GINTMSK_ENUMDONEMSK    = BIT(13),
    FLD_USB_GINTMSK_ISOOUTDROPMSK  = BIT(14),
    FLD_USB_GINTMSK_EOPFMSK        = BIT(15),
    FLD_USB_GINTMSK_RSTRDONEINTMSK = BIT(16),
    FLD_USB_GINTMSK_EPMISMSK       = BIT(17),
    FLD_USB_GINTMSK_IEPINTMSK      = BIT(18),
    FLD_USB_GINTMSK_OEPINTMSK      = BIT(19),
    FLD_USB_GINTMSK_FETSUSPMSK     = BIT(22),
    FLD_USB_GINTMSK_RESETDETMSK    = BIT(23),
    FLD_USB_GINTMSK_LPM_INTMSK     = BIT(27),
    FLD_USB_GINTMSK_WKUPINTMSK     = BIT(31),
} usb_gintmsk_e;

#define reg_usb_grxstsr REG_ADDR32(REG_USB0_BASE + 0x1c)

enum
{
    FLD_USB_GRXSTSR_EPNUM  = BIT_RNG(0, 3),
    FLD_USB_GRXSTSR_BCNT   = BIT_RNG(4, 14),
    FLD_USB_GRXSTSR_DPID   = BIT_RNG(15, 16),
    FLD_USB_GRXSTSR_PKTSTS = BIT_RNG(17, 20),
    FLD_USB_GRXSTSR_FN     = BIT_RNG(21, 24),
};

#define reg_usb_grxstsp REG_ADDR32(REG_USB0_BASE + 0x20)

enum
{
    FLD_USB_GRXSTSP_EPNUM  = BIT_RNG(0, 3),
    FLD_USB_GRXSTSP_BCNT   = BIT_RNG(4, 14),
    FLD_USB_GRXSTSP_DPID   = BIT_RNG(15, 16),
    FLD_USB_GRXSTSP_PKTSTS = BIT_RNG(17, 20),
    FLD_USB_GRXSTSP_FN     = BIT_RNG(21, 24),
};

#define reg_usb_grxfsiz REG_ADDR32(REG_USB0_BASE + 0x24)

enum
{
    FLD_USB_GRXFSIZ_RXFDEP = BIT_RNG(0, 10), /**< Minimum value is 16. Maximum value is 32768. */
};

#define reg_usb_gnptxfsiz REG_ADDR32(REG_USB0_BASE + 0x28)

enum
{
    FLD_USB_GNPTXFSIZ_INEPTXF0STADDR = BIT_RNG(0, 10),  /**< IN Endpoint FIFO0 Transmit RAM Start. */
    FLD_USB_GNPTXFSIZ_INEPTXF0DEP    = BIT_RNG(16, 24), /**< IN Endpoint TxFIFO 0 Depth, Minimum value is 16. Maximum value is 32768. */
};

#define reg_usb_gpvndctl REG_ADDR32(REG_USB0_BASE + 0x34)

enum
{
    FLD_USB_GPVNDCTL_REGDATA     = BIT_RNG(0, 7),
    FLD_USB_GPVNDCTL_VCTRL       = BIT_RNG(8, 15),
    FLD_USB_GPVNDCTL_REGADDR     = BIT_RNG(16, 21),
    FLD_USB_GPVNDCTL_REGWR       = BIT(22),
    FLD_USB_GPVNDCTL_NEWREGREQ   = BIT(25),
    FLD_USB_GPVNDCTL_VSTSBSY     = BIT(26),
    FLD_USB_GPVNDCTL_VSTSDONE    = BIT(27),
    FLD_USB_GPVNDCTL_DISULPIDRVR = BIT(31),
};

#define reg_usb_gsnpsid REG_ADDR32(REG_USB0_BASE + 0x40)
#define reg_usb_ghwcfg1 REG_ADDR32(REG_USB0_BASE + 0x44)

#define reg_usb_ghwcfg2 REG_ADDR32(REG_USB0_BASE + 0x48)

enum
{
    FLD_USB_GHWCFG2_NUMDEVEPS     = BIT_RNG(10, 13),
    FLD_USB_GHWCFG2_DYNFIFOSIZING = BIT(19),
};

#define reg_usb_ghwcfg3 REG_ADDR32(REG_USB0_BASE + 0x4c)

enum
{
    FLD_USB_GHWCFG3_XFERSIZEWIDTH = BIT_RNG(0, 3),
    FLD_USB_GHWCFG3_PKTSIZEWIDTH  = BIT_RNG(4, 6),
    FLD_USB_GHWCFG3_RSTTYPE       = BIT(11),
    FLD_USB_GHWCFG3_BCSUPPORT     = BIT(14),
    FLD_USB_GHWCFG3_LPMMODE       = BIT(15),
    FLD_USB_GHWCFG3_DFIFODEPTH    = BIT_RNG(16, 31), /**< Minimum value is 16. Maximum value is 32768. */
};

#define reg_usb_ghwcfg4 REG_ADDR32(REG_USB0_BASE + 0x50)

enum
{
    FLD_USB_GHWCFG4_NUMDEVPERIOEPS   = BIT_RNG(0, 3),
    FLD_USB_GHWCFG4_PARTIALPWRDN     = BIT(4),
    FLD_USB_GHWCFG4_AHBFREQ          = BIT(5),
    FLD_USB_GHWCFG4_HIBERNATION      = BIT(6),
    FLD_USB_GHWCFG4_ENHANCEDLPMSUPT1 = BIT(9),
    FLD_USB_GHWCFG4_SERVINTFLOW      = BIT(10),
    FLD_USB_GHWCFG4_IPGISOCSUPT      = BIT(11),
    FLD_USB_GHWCFG4_ACGSUPT          = BIT(12),
    FLD_USB_GHWCFG4_ENHANCEDLPMSUPT  = BIT(13),
    FLD_USB_GHWCFG4_NUMCTLEPS        = BIT_RNG(16, 19),
    FLD_USB_GHWCFG4_INEPS            = BIT_RNG(26, 29),
    FLD_USB_GHWCFG4_DESCDMAENABLED   = BIT(30),
    FLD_USB_GHWCFG4_DESCDMA          = BIT(31),
};

#define reg_usb_glpmcfg REG_ADDR32(REG_USB0_BASE + 0x54)

enum
{
    FLD_USB_GLPMCFG_LPMCAP            = BIT(0),
    FLD_USB_GLPMCFG_APPL1RES          = BIT(1),
    FLD_USB_GLPMCFG_HIRD              = BIT_RNG(2, 5),
    FLD_USB_GLPMCFG_BREMOTEWAKE       = BIT(6),
    FLD_USB_GLPMCFG_ENBLSLPM          = BIT(7),
    FLD_USB_GLPMCFG_HIRD_THRES        = BIT_RNG(8, 12),
    FLD_USB_GLPMCFG_COREL1RES         = BIT_RNG(13, 14),
    FLD_USB_GLPMCFG_SLPSTS            = BIT(15),
    FLD_USB_GLPMCFG_L1RESUMEOK        = BIT(16),
    FLD_USB_GLPMCFG_LPM_ACCEPT_CTRL   = BIT_RNG(20, 23),
    FLD_USB_GLPMCFG_LPM_ENBESL        = BIT(28),
    FLD_USB_GLPMCFG_LPM_RESTORESLPSTS = BIT(29),
};

#define reg_usb_gpwrdn REG_ADDR32(REG_USB0_BASE + 0x58)

enum
{
    FLD_USB_GPWRDN_PMUINTSEL                          = BIT(0),
    FLD_USB_GPWRDN_PMUACTV                            = BIT(1),
    FLD_USB_GPWRDN_RESTORE                            = BIT(2),
    FLD_USB_GPWRDN_PWRDNCLMP                          = BIT(3),
    FLD_USB_GPWRDN_PWRDNRST_N                         = BIT(4),
    FLD_USB_GPWRDN_PWRDNSWTCH                         = BIT(5),
    FLD_USB_GPWRDN_LNSTSCHNG                          = BIT(7),
    FLD_USB_GPWRDN_LINESTAGECHANGEMSK                 = BIT(8),
    FLD_USB_GPWRDN_RESETDETECTED                      = BIT(9),
    FLD_USB_GPWRDN_RESETDETMSK                        = BIT(10),
    FLD_USB_GPWRDN_DISCONNECTDETECT                   = BIT(11),
    FLD_USB_GPWRDN_CONNECTDET                         = BIT(13),
    FLD_USB_GPWRDN_CONNDETMSK                         = BIT(14),
    FLD_USB_GPWRDN_STSCHNGINT                         = BIT(17),
    FLD_USB_GPWRDN_STSCHNGINTMSK                      = BIT(18),
    FLD_USB_GPWRDN_LINESTATE                          = BIT_RNG(19, 20),
    FLD_USB_GPWRDN_MULTVALIDBC                        = BIT_RNG(24, 28),
    FLD_USB_GPWRDN_ULPI_LATCH_ENABLE_DURING_HIB_ENTRY = BIT(29),
};

#define reg_usb_gdfifocfg REG_ADDR32(REG_USB0_BASE + 0x5c)

enum
{
    FLD_USB_GDFIFOCFG_GDFIFOCFG      = BIT_RNG(0, 15),
    FLD_USB_GDFIFOCFG_EPINFOBASEADDR = BIT_RNG(16, 31),
};

#define reg_usb_grefclk REG_ADDR32(REG_USB0_BASE + 0x64)

enum
{
    FLD_USB_GREFCLK_SOF_CNT_WKUP_ALERT = BIT_RNG(0, 9),
    FLD_USB_GREFCLK_REFCLKMODE         = BIT(14),
    FLD_USB_GREFCLK_REFCLKPER          = BIT_RNG(15, 31),
};

#define reg_usb_gintmsk2 REG_ADDR32(REG_USB0_BASE + 0x68)

enum
{
    FLD_USB_GINTMSK2_WKUPALERTINTMSK = BIT(0),
};

#define reg_usb_gintsts2 REG_ADDR32(REG_USB0_BASE + 0x6c)

enum
{
    FLD_USB_GINTSTS2_WKUPALERTINT = BIT(0),
};

/******************************* Device *******************************/
#define reg_usb_dieptxf(i) REG_ADDR32(REG_USB0_BASE + (((i) == 0) ? 0x28 : (0x104 + (i - 1) * 4))) /**< i[0-8]. */

enum
{
    FLD_USB_DIEPTXF_INEPNTXFSTADDR = BIT_RNG(0, 10),
    FLD_USB_DIEPTXF_INEPNTXFDEP    = BIT_RNG(16, 24), /**< Minimum value is 16. Maximum value is 32768. */
};

#define reg_usb_dcfg REG_ADDR32(REG_USB0_BASE + 0x800)

enum
{
    FLD_USB_DCFG_DEVSPD        = BIT_RNG(0, 1),
    FLD_USB_DCFG_NZSTSOUTHSHK  = BIT(2),
    FLD_USB_DCFG_DEVADDR       = BIT_RNG(4, 10),
    FLD_USB_DCFG_PERFRINT      = BIT_RNG(11, 12),
    FLD_USB_DCFG_ENDEVOUTNAK   = BIT(13),
    FLD_USB_DCFG_XCVRDLY       = BIT(14),
    FLD_USB_DCFG_ERRATICINTMSK = BIT(15),
    FLD_USB_DCFG_IPGISOCSUPT   = BIT(17),
    FLD_USB_DCFG_DESCDMA       = BIT(23),
    FLD_USB_DCFG_PERSCHINTVL   = BIT_RNG(24, 25),
    FLD_USB_DCFG_RESVALID      = BIT_RNG(26, 31),
};

#define reg_usb_dctl REG_ADDR32(REG_USB0_BASE + 0x804)

enum
{
    FLD_USB_DCTL_RMTWKUPSIG          = BIT(0),
    FLD_USB_DCTL_SFTDISCON           = BIT(1), /**< 1: disconnect, 0: connect. */
    FLD_USB_DCTL_GNPINNAKSTS         = BIT(2),
    FLD_USB_DCTL_GOUTNAKSTS          = BIT(3),
    FLD_USB_DCTL_TSTCTL              = BIT_RNG(4, 6),
    FLD_USB_DCTL_SGNPINNAK           = BIT(7),
    FLD_USB_DCTL_CGNPINNAK           = BIT(8),
    FLD_USB_DCTL_SGOUTNAK            = BIT(9),
    FLD_USB_DCTL_CGOUTNAK            = BIT(10),
    FLD_USB_DCTL_PWRONPRGDONE        = BIT(11),
    FLD_USB_DCTL_GMC                 = BIT_RNG(13, 14),
    FLD_USB_DCTL_IGNRFRMNUM          = BIT(15),
    FLD_USB_DCTL_NAKONBBLE           = BIT(16),
    FLD_USB_DCTL_ENCONTONBNA         = BIT(17),
    FLD_USB_DCTL_DEEPSLEEPBESLREJECT = BIT(18),
    FLD_USB_DCTL_SERVINT             = BIT(19),
};

#define reg_usb_dsts REG_ADDR32(REG_USB0_BASE + 0x808)

enum
{
    FLD_USB_DSTS_SUSPSTS   = BIT(0),
    FLD_USB_DSTS_ENUMSPD   = BIT_RNG(1, 2), /**< 0: high speed, 1: full speed. */
    FLD_USB_DSTS_ERRTICERR = BIT(3),
    FLD_USB_DSTS_SOFFN     = BIT_RNG(8, 21),
    FLD_USB_DSTS_DEVLNSTS  = BIT_RNG(22, 23),
};

#define reg_usb_diepmsk REG_ADDR32(REG_USB0_BASE + 0x810)

typedef enum
{
    FLD_USB_DIEPMSK_XFERCOMPLMSK   = BIT(0),
    FLD_USB_DIEPMSK_EPDISBLDMSK    = BIT(1),
    FLD_USB_DIEPMSK_AHBERRMSK      = BIT(2),
    FLD_USB_DIEPMSK_TIMEOUTMSK     = BIT(3),
    FLD_USB_DIEPMSK_INTKNTXFEMPMSK = BIT(4),
    FLD_USB_DIEPMSK_INTKNEPMISMSK  = BIT(5),
    FLD_USB_DIEPMSK_INEPNAKEFFMSK  = BIT(6),
    FLD_USB_DIEPMSK_TXFIFOUNDRNMSK = BIT(8),
    FLD_USB_DIEPMSK_BNAININTRMSK   = BIT(9),
    FLD_USB_DIEPMSK_NAKMSK         = BIT(13),
} usb_diepmsk_e;

#define reg_usb_doepmsk REG_ADDR32(REG_USB0_BASE + 0x814)

typedef enum
{
    FLD_USB_DOEPMSK_XFERCOMPLMSK   = BIT(0),
    FLD_USB_DOEPMSK_EPDISBLDMSK    = BIT(1),
    FLD_USB_DOEPMSK_AHBERRMSK      = BIT(2),
    FLD_USB_DOEPMSK_SETUPMSK       = BIT(3),
    FLD_USB_DOEPMSK_OUTTKNEPDISMSK = BIT(4),
    FLD_USB_DOEPMSK_STSPHSERCVDMSK = BIT(5),
    FLD_USB_DOEPMSK_BACK2BACKSETUP = BIT(6),
    FLD_USB_DOEPMSK_OUTPKTERRMSK   = BIT(8),
    FLD_USB_DOEPMSK_BNAOUTINTRMSK  = BIT(9),
    FLD_USB_DOEPMSK_BBLEERRMSK     = BIT(12),
    FLD_USB_DOEPMSK_NAKMSK         = BIT(13),
    FLD_USB_DOEPMSK_NYETMSK        = BIT(14),
} usb_doepmsk_e;

#define reg_usb_daint REG_ADDR32(REG_USB0_BASE + 0x818)

enum
{
    FLD_USB_DAINT_INEPINT0  = BIT(0),
    FLD_USB_DAINT_INEPINT1  = BIT(1),
    FLD_USB_DAINT_INEPINT2  = BIT(2),
    FLD_USB_DAINT_INEPINT3  = BIT(3),
    FLD_USB_DAINT_INEPINT4  = BIT(4),
    FLD_USB_DAINT_INEPINT5  = BIT(5),
    FLD_USB_DAINT_INEPINT6  = BIT(6),
    FLD_USB_DAINT_INEPINT7  = BIT(7),
    FLD_USB_DAINT_INEPINT8  = BIT(8),
    FLD_USB_DAINT_OUTEPINT0 = BIT(16),
    FLD_USB_DAINT_OUTEPINT1 = BIT(17),
    FLD_USB_DAINT_OUTEPINT2 = BIT(18),
    FLD_USB_DAINT_OUTEPINT3 = BIT(19),
    FLD_USB_DAINT_OUTEPINT4 = BIT(20),
    FLD_USB_DAINT_OUTEPINT5 = BIT(21),
    FLD_USB_DAINT_OUTEPINT6 = BIT(22),
    FLD_USB_DAINT_OUTEPINT7 = BIT(23),
    FLD_USB_DAINT_OUTEPINT8 = BIT(24),
};

#define reg_usb_daintmsk REG_ADDR32(REG_USB0_BASE + 0x81c)

typedef enum
{
    FLD_USB_DAINTMSK_INEPMSK0  = BIT(0),
    FLD_USB_DAINTMSK_INEPMSK1  = BIT(1),
    FLD_USB_DAINTMSK_INEPMSK2  = BIT(2),
    FLD_USB_DAINTMSK_INEPMSK3  = BIT(3),
    FLD_USB_DAINTMSK_INEPMSK4  = BIT(4),
    FLD_USB_DAINTMSK_INEPMSK5  = BIT(5),
    FLD_USB_DAINTMSK_INEPMSK6  = BIT(6),
    FLD_USB_DAINTMSK_INEPMSK7  = BIT(7),
    FLD_USB_DAINTMSK_INEPMSK8  = BIT(8),
    FLD_USB_DAINTMSK_OUTEPMSK0 = BIT(16),
    FLD_USB_DAINTMSK_OUTEPMSK1 = BIT(17),
    FLD_USB_DAINTMSK_OUTEPMSK2 = BIT(18),
    FLD_USB_DAINTMSK_OUTEPMSK3 = BIT(19),
    FLD_USB_DAINTMSK_OUTEPMSK4 = BIT(20),
    FLD_USB_DAINTMSK_OUTEPMSK5 = BIT(21),
    FLD_USB_DAINTMSK_OUTEPMSK6 = BIT(22),
    FLD_USB_DAINTMSK_OUTEPMSK7 = BIT(23),
    FLD_USB_DAINTMSK_OUTEPMSK8 = BIT(24),
} usb_daintmsk_e;

#define reg_usb_dthrctl REG_ADDR32(REG_USB0_BASE + 0x830)

enum
{
    FLD_USB_DTHRCTL_NONISOTHREN = BIT(0),
    FLD_USB_DTHRCTL_ISOTHREN    = BIT(1),
    FLD_USB_DTHRCTL_TXTHRLEN    = BIT_RNG(2, 10),
    FLD_USB_DTHRCTL_AHBTHRRATIO = BIT_RNG(11, 12),
    FLD_USB_DTHRCTL_RXTHREN     = BIT(16),
    FLD_USB_DTHRCTL_RXTHRLEN    = BIT_RNG(17, 25),
    FLD_USB_DTHRCTL_ARBPRKEN    = BIT(27),
};

#define reg_usb_diepempmsk REG_ADDR32(REG_USB0_BASE + 0x834)

/******************************* Device IN Endpoint *******************************/
#define reg_usb_diepctl(i) REG_ADDR32(REG_USB0_BASE + 0x900 + (i) * 0x20) /**< i[0-8]. */

enum
{
    FLD_USB_DIEPCTL0_MPS     = BIT_RNG(0, 1), /**< only for control endpoint, 0b00: 64 bytes, 0b01: 32 bytes, 0b10: 16 bytes, 0b11: 8 bytes. */
    FLD_USB_DIEPCTL_MPS      = BIT_RNG(0, 10),
    FLD_USB_DIEPCTL_USBACTEP = BIT(15),
    FLD_USB_DIEPCTL_DPID     = BIT(16),
    FLD_USB_DIEPCTL_NAKSTS   = BIT(17),
    FLD_USB_DIEPCTL_EPTYPE   = BIT_RNG(18, 19),
    FLD_USB_DIEPCTL_STALL    = BIT(21),
    FLD_USB_DIEPCTL_TXFNUM   = BIT_RNG(22, 25),
    FLD_USB_DIEPCTL_CNAK     = BIT(26),
    FLD_USB_DIEPCTL_SNAK     = BIT(27),
    FLD_USB_DIEPCTL_SETD0PID = BIT(28),
    FLD_USB_DIEPCTL_SETD1PID = BIT(29),
    FLD_USB_DIEPCTL_EPDIS    = BIT(30),
    FLD_USB_DIEPCTL_EPENA    = BIT(31),
};

#define reg_usb_diepint(i) REG_ADDR32(REG_USB0_BASE + 0x908 + (i) * 0x20) /**< i[0-8]. */

typedef enum
{
    FLD_USB_DIEPINT_XFERCOMPL   = BIT(0),
    FLD_USB_DIEPINT_EPDISBLD    = BIT(1),
    FLD_USB_DIEPINT_AHBERR      = BIT(2),
    FLD_USB_DIEPINT_INTKNTXFEMP = BIT(4),
    FLD_USB_DIEPINT_INTKNEPMIS  = BIT(5),
    FLD_USB_DIEPINT_INEPNAKEFF  = BIT(6),
    FLD_USB_DIEPINT_TXFEMP      = BIT(7),
    FLD_USB_DIEPINT_TXFIFOUNDRN = BIT(8),
    FLD_USB_DIEPINT_BNAINTR     = BIT(9),
    FLD_USB_DIEPINT_BBLEERR     = BIT(12),
    FLD_USB_DIEPINT_NAKINTRPT   = BIT(13),
    FLD_USB_DIEPINT_NYETINTRPT  = BIT(14),
} usb_diepint_e;

#define reg_usb_diepdma(i) REG_ADDR32(REG_USB0_BASE + 0x914 + (i) * 0x20) /**< i[0-8]. */
#define reg_usb_dtxfsts(i) REG_ADDR32(REG_USB0_BASE + 0x918 + (i) * 0x20) /**< i[0-8]. */

enum
{
    FLD_USB_DTXFSTS_INEPTXFSPCAVAIL = BIT_RNG(0, 15),
};

#define reg_usb_diepdmab(i) REG_ADDR32(REG_USB0_BASE + 0x91c + (i) * 0x20) /**< i[0-8]. */

/******************************* Device OUT Endpoint *******************************/
#define reg_usb_doepctl(i) REG_ADDR32(REG_USB0_BASE + 0xb00 + (i) * 0x20) /**< i[0-8]. */

enum
{
    FLD_USB_DOEPCTL0_MPS     = BIT_RNG(0, 1), /**< only for control endpoint, 0b00: 64 bytes, 0b01: 32 bytes, 0b10: 16 bytes, 0b11: 8 bytes. */
    FLD_USB_DOEPCTL_MPS      = BIT_RNG(0, 10),
    FLD_USB_DOEPCTL_USBACTEP = BIT(15),
    FLD_USB_DOEPCTL_DPID     = BIT(16),
    FLD_USB_DOEPCTL_NAKSTS   = BIT(17),
    FLD_USB_DOEPCTL_EPTYPE   = BIT_RNG(18, 19),
    FLD_USB_DOEPCTL_SNP      = BIT(20),
    FLD_USB_DOEPCTL_STALL    = BIT(21),
    FLD_USB_DOEPCTL_CNAK     = BIT(26),
    FLD_USB_DOEPCTL_SNAK     = BIT(27),
    FLD_USB_DOEPCTL_SETD0PID = BIT(28),
    FLD_USB_DOEPCTL_SETD1PID = BIT(29),
    FLD_USB_DOEPCTL_EPDIS    = BIT(30),
    FLD_USB_DOEPCTL_EPENA    = BIT(31),
};

#define reg_usb_doepint(i) REG_ADDR32(REG_USB0_BASE + 0xb08 + (i) * 0x20) /**< i[0-8]. */

typedef enum
{
    FLD_USB_DOEPINT_XFERCOMPL      = BIT(0),
    FLD_USB_DOEPINT_EPDISBLD       = BIT(1),
    FLD_USB_DOEPINT_AHBERR         = BIT(2),
    FLD_USB_DOEPINT_SETUP          = BIT(3),
    FLD_USB_DOEPINT_OUTTKNEPDIS    = BIT(4),
    FLD_USB_DOEPINT_STSPHSERCVD    = BIT(5),
    FLD_USB_DOEPINT_BACK2BACKSETUP = BIT(6),
    FLD_USB_DOEPINT_OUTPKTERR      = BIT(8),
    FLD_USB_DOEPINT_BNAINTR        = BIT(9),
    FLD_USB_DOEPINT_PKTDRPSTS      = BIT(11),
    FLD_USB_DOEPINT_BBLEERR        = BIT(12),
    FLD_USB_DOEPINT_NAKINTRPT      = BIT(13),
    FLD_USB_DOEPINT_NYETINTRPT     = BIT(14),
} usb_doepint_e;

#define reg_usb_doeptsiz(i) REG_ADDR32(REG_USB0_BASE + 0xb10 + (i) * 0x20) /**< i[1-8]. */

enum
{
    FLD_USB_DOEPTSIZ_RXDPID = BIT_RNG(29, 30),                             /**< 0b00: DATA0, 0b01: DATA2, 0b10: DATA1, 0b11: MDATA. */
};

#define reg_usb_doepdma(i)  REG_ADDR32(REG_USB0_BASE + 0xb14 + (i) * 0x20) /**< i[0-8]. */
#define reg_usb_doepdmab(i) REG_ADDR32(REG_USB0_BASE + 0xb1c + (i) * 0x20) /**< i[0-8]. */

/******************************* Power Clock *******************************/
#define reg_usb_pcgcctl REG_ADDR32(REG_USB0_BASE + 0xe00)

enum
{
    FLD_USB_PCGCCTL_STOPPCLK       = BIT(0),
    FLD_USB_PCGCCTL_GATEHCLK       = BIT(1),
    FLD_USB_PCGCCTL_PWRCLMP        = BIT(2),
    FLD_USB_PCGCCTL_RSTPDWNMODULE  = BIT(3),
    FLD_USB_PCGCCTL_ENBL_L1GATING  = BIT(5),
    FLD_USB_PCGCCTL_PHYSLEEP       = BIT(6),
    FLD_USB_PCGCCTL_L1SUSPENDED    = BIT(7),
    FLD_USB_PCGCCTL_RESTOREMODE    = BIT(9),
    FLD_USB_PCGCCTL_ESSREGRESTORED = BIT(13),
    FLD_USB_PCGCCTL_RESTOREVALUE   = BIT_RNG(14, 31),
};

#define reg_usb_pcgcctl1 REG_ADDR32(REG_USB0_BASE + 0xe04)

enum
{
    FLD_USB_PCGCCTL1_GATEEN    = BIT(0),
    FLD_USB_PCGCCTL1_COUNTER   = BIT_RNG(1, 2),
    FLD_USB_PCGCCTL1_RAMGATEEN = BIT(3),
};

/******************************* FIFOs *******************************/
/**
 * @brief FIFO size is 8K bytes. \n
 * TX FIFO: Dedicated transmit FIFO for each of the device IN endpoints. \n
 * RX FIFO: Single receive FIFO to receive the data for all the OUT endpoints.
 * 
 */
#define reg_usb_fifo(i) REG_ADDR32(REG_USB0_BASE + 0x1000 + (i) * 0x400)

/******************************* PHY control *******************************/
#define reg_usb_usbphy_ct REG_ADDR32(REG_USB0_BASE + 0x60000)

enum
{
    FLD_USB_USBPHYCT_REFECLK_MODE    = BIT(0),
    FLD_USB_USBPHYCT_SELF_TEST       = BIT(2),
    FLD_USB_USBPHYCT_RELEASE_AUDO    = BIT(3),
    FLD_USB_USBPHYCT_RELEASE_MANUAL  = BIT(4),
    FLD_USB_USBPHYCT_PLL_EN_MANUAL   = BIT(5),
    FLD_USB_USBPHYCT_SUSPENDM_MANUAL = BIT(6),
    FLD_USB_USBPHYCT_RST_N_MANUAL    = BIT(7),
    FLD_USB_USBPHYCT_TEST_BIST       = BIT(8),
    FLD_USB_USBPHYCT_PHY_DTO         = BIT(9),
    FLD_USB_USBPHYCT_PLL_EN_CNT      = BIT_RNG(16, 19),
    FLD_USB_USBPHYCT_SUSPEND_CNT     = BIT_RNG(20, 23),
    FLD_USB_USBPHYCT_PHY_RST_CNT     = BIT_RNG(24, 28),
};

#define reg_usb_tstamp REG_ADDR32(REG_USB0_BASE + 0x60000 + 0x08)

#endif

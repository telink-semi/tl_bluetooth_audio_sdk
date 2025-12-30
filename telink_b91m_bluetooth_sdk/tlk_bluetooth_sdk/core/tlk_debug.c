/********************************************************************************************************
 * @file    tlk_debug.c
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
#include "tlk_debug.h"
#if (TLK_DEBUG_ENABLE)

typedef struct
{
    unsigned char minorID;
    unsigned char logIsEn;
    unsigned char vcdIsEn;
    unsigned char logFlag;
    const char   *pItemName;
} tlk_debug_item_t;

typedef struct
{
    unsigned char    logIsEn;
    unsigned char    vcdIsEn;
    unsigned char    logFlag;
    unsigned char    unitCnt;
    const char      *pListName;
    tlk_debug_item_t unit[24];
} tlk_debug_list_t;

static const tlk_debug_list_t scTlkDebugBthInfo = {true,                    //  logIsEn
                                                   false,                   //  vcdIsEn
                                                   TLK_DEBUG_DBG_FLAG_ALL,  //  logFlag
                                                   TLK_MINOR_DBGID_BTH_MAX, //  unitCnt
                                                   "BT-Host",               //  pListName
                                                   {
                                                       {TLK_MINOR_DBGID_BTH, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[BTH]"},
                                                       {TLK_MINOR_DBGID_BTH_ACL, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[ACL]"},
                                                       {TLK_MINOR_DBGID_BTH_SCO, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SCO]"},
                                                       {TLK_MINOR_DBGID_BTH_CMD, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[CMD]"},
                                                       {TLK_MINOR_DBGID_BTH_EVT, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[EVT]"},
                                                       {TLK_MINOR_DBGID_BTH_L2C, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[L2C]"},
                                                       {TLK_MINOR_DBGID_BTH_SIG, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SIG]"},
                                                   }};
static const tlk_debug_list_t scTlkDebugBtpInfo = {
    true,                    //  logIsEn
    false,                   //  vcdIsEn
    TLK_DEBUG_DBG_FLAG_ALL,  //  logFlag
    TLK_MINOR_DBGID_BTP_MAX, //  unitCnt
    "BT-Profile",            //  pListName
    {
        {TLK_MINOR_DBGID_BTP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[BTP]"},           {TLK_MINOR_DBGID_BTP_SDP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SDP]"},
        {TLK_MINOR_DBGID_BTP_SDPC, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SDPC]"},     {TLK_MINOR_DBGID_BTP_SDPS, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SDPS]"},
        {TLK_MINOR_DBGID_BTP_RFCOMM, false, false, TLK_DEBUG_DBG_FLAG_ALL, "[RFC]"},   {TLK_MINOR_DBGID_BTP_IAP, false, false, TLK_DEBUG_DBG_FLAG_ALL, "[IAP]"},
        {TLK_MINOR_DBGID_BTP_SPP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SPP]"},       {TLK_MINOR_DBGID_BTP_ATT, false, false, TLK_DEBUG_DBG_FLAG_ALL, "[ATT]"},
        {TLK_MINOR_DBGID_BTP_A2DP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[A2DP]"},     {TLK_MINOR_DBGID_BTP_A2DPS, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[A2DPS]"},
        {TLK_MINOR_DBGID_BTP_A2DPC, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[A2DPC]"},   {TLK_MINOR_DBGID_BTP_HID, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[HID]"},
        {TLK_MINOR_DBGID_BTP_HIDS, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[HIDS]"},     {TLK_MINOR_DBGID_BTP_HIDC, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[HIDC]"},
        {TLK_MINOR_DBGID_BTP_HFP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[HFP]"},       {TLK_MINOR_DBGID_BTP_HFPAG, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[HFPAG]"},
        {TLK_MINOR_DBGID_BTP_HFPHF, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[HFPHF]"},   {TLK_MINOR_DBGID_BTP_AVRCP, false, false, TLK_DEBUG_DBG_FLAG_ALL, "[AVRCP]"},
        {TLK_MINOR_DBGID_BTP_OBEX, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[OBEX]"},     {TLK_MINOR_DBGID_BTP_PBAP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[PBAP]"},
        {TLK_MINOR_DBGID_BTP_BROWSE, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[BROWSE]"}, {TLK_MINOR_DBGID_BTP_COVERART, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[CVRART]"},
    }};

static const tlk_debug_list_t scTlkDebugMwInfo = {true,                   //  logIsEn
                                                  false,                  //  vcdIsEn
                                                  TLK_DEBUG_DBG_FLAG_ALL, //  logFlag
                                                  TLK_MINOR_DBGID_MW_MAX, //  unitCnt
                                                  "MW",                   //  pListName
                                                  {
                                                      {TLK_MINOR_DBGID_MW_CODEC, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MW_CODEC]"},
                                                      {TLK_MINOR_DBGID_MDI_TPSLL_CCHEADSET, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MDI_TPSLL_H]"},
                                                      {TLK_MINOR_DBGID_MW_MFI, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MW_MFI]"},
                                                  }};

static const tlk_debug_list_t scTlkDebugMdiAudInfo = {true,                        //  logIsEn
                                                      false,                       //  vcdIsEn
                                                      TLK_DEBUG_DBG_FLAG_ALL,      //  logFlag
                                                      TLK_MINOR_DBGID_MDI_AUD_MAX, //  unitCnt
                                                      "MDI-Audio",                 //  pListName
                                                      {
                                                          {TLK_MINOR_DBGID_MDI_AUD, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[AUD]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_PLAY, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[PLAY]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_TONE, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[TONE]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_MP3, false, false, TLK_DEBUG_DBG_FLAG_ALL, "[MP3]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_HFP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[HFP]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_SCO, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SCO]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_SNK, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SNK]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_SRC, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[SRC]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_UAC, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[UAC]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_DSCO, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[DSCO]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_RELAY, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[RELAY]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_CC_BT_AUDIO, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[BTAUD]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_CC_LL_AUDIO, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[LLAUD]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_U2S, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[U2S]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_LEA_UC, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[LE-AUD]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_LEA_US, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[LE-AUD]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_LEA_BMR, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[LE-AUD]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_LEA_BMS, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[LE-AUD]"},
                                                          {TLK_MINOR_DBGID_MDI_AUD_U2H_VOICE, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[U2H]"},
                                                      }};
static const tlk_debug_list_t scTlkDebugMdiBtInfo  = {true,                       //  logIsEn
                                                      false,                      //  vcdIsEn
                                                      TLK_DEBUG_DBG_FLAG_ALL,     //  logFlag
                                                      TLK_MINOR_DBGID_MDI_BT_MAX, //  unitCnt
                                                      "MDI-BT",                   //  pListName
                                                      {
                                                         {TLK_MINOR_DBGID_MDI_BT, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MBT]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_ACL, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MACL]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_ATT, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MATT]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_HFP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MHFP]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_HID, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MHID]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_INQ, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MINQ]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_REC, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MREC]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_IAP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MIAP]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_A2DP, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MA2DP]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_SCO, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MSCO]"},
                                                         {TLK_MINOR_DBGID_MDI_BT_AG, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[MHFPAG]"},
                                                         {TLK_MINOR_DBGID_MDI_AUD_CC_BT_AUDIO, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[BTAUD]"},
                                                         {TLK_MINOR_DBGID_MDI_AUD_CC_LL_AUDIO, true, false, TLK_DEBUG_DBG_FLAG_ALL, "[LLAUD]"},
                                                     }};

// static
static const tlk_debug_list_t *scTlkDebugInfo[TLK_MAJOR_DBGID_MAX] = {
    &scTlkDebugBthInfo,    // TLK_MAJOR_DBGID_BTH
    &scTlkDebugBtpInfo,    // TLK_MAJOR_DBGID_BTP
    &scTlkDebugMwInfo,     // TLK_MAJOR_DBGID_MW
    &scTlkDebugMdiAudInfo, // TLK_MAJOR_DBGID_MDI_AUDIO
    &scTlkDebugMdiBtInfo,  // TLK_MAJOR_DBGID_MDI_BT
};

static unsigned long sTlkDebugLogMask[TLK_MAJOR_DBGID_MAX];

void tlk_debug_init(void)
{
    tlk_debug_logLoad();
}

void tlk_debug_logLoad(void)
{
    unsigned int idxI;
    unsigned int idxJ;
    for (idxI = 0; idxI < TLK_MAJOR_DBGID_MAX; idxI++) {
        sTlkDebugLogMask[idxI] = 0;
        for (idxJ = 0; idxJ < 24; idxJ++) {
            if (scTlkDebugInfo[idxI] != NULL && idxJ < scTlkDebugInfo[idxI]->unitCnt && scTlkDebugInfo[idxI]->logIsEn && scTlkDebugInfo[idxI]->unit[idxJ].logIsEn) {
                sTlkDebugLogMask[idxI] |= (1 << idxJ);
            }
        }
    }
}

_attribute_noinline_ bool tlk_debug_logIsEnable(unsigned int flags, unsigned int logFlag)
{
    unsigned char majorID = (flags >> 24);
    unsigned char minorID = (flags >> 16) & 0xFF;
    if (flags == 0xFFFFFFFF) {
        return true;
    }
    if ((flags & logFlag) == 0 || majorID >= TLK_MAJOR_DBGID_MAX || minorID >= 32 || (sTlkDebugLogMask[majorID] & (1 << minorID)) == 0 || scTlkDebugInfo[majorID] == NULL ||
        (scTlkDebugInfo[majorID]->logFlag & logFlag) == 0 || (scTlkDebugInfo[majorID]->unit[minorID].logFlag & logFlag) == 0) {
        return false;
    }
    return true;
}

_attribute_ram_code_sec_noinline_ char *tlk_debug_getDbgSign(unsigned int flags)
{
    unsigned char majorID = (flags >> 24);
    unsigned char minorID = (flags >> 16) & 0xFF;
    if (majorID >= TLK_MAJOR_DBGID_MAX || scTlkDebugInfo[majorID] == NULL || minorID >= 24 || (sTlkDebugLogMask[majorID] & (1 << minorID)) == 0) {
        return NULL;
    }
    return (char *)scTlkDebugInfo[majorID]->unit[minorID].pItemName;
}

#endif // #if (TLK_CFG_DBG_ENABLE)

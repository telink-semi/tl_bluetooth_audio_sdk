/********************************************************************************************************
 * @file    lea.h
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

#include "prf_lea.h"

#include "../common/inc/generic_audio.h"

#include "../content/gtbs/inc/gtbs.h"
#include "../content/gtbs/inc/gtbsc.h"
#include "../content/gtbs/inc/gtbss.h"

#include "../content/tbs/inc/tbs.h"
#include "../content/tbs/inc/tbsc.h"
#include "../content/tbs/inc/tbss.h"

#include "../content/gmcs/inc/gmcs.h"
#include "../content/gmcs/inc/gmcsc.h"
#include "../content/gmcs/inc/gmcss.h"

#include "../render_cap/aics/inc/aics.h"
#include "../render_cap/aics/inc/aicsc.h"
#include "../render_cap/aics/inc/aicss.h"

#include "../render_cap/mics/inc/mics.h"
#include "../render_cap/mics/inc/micsc.h"
#include "../render_cap/mics/inc/micss.h"

#include "../render_cap/vocs/inc/vocs.h"
#include "../render_cap/vocs/inc/vocsc.h"
#include "../render_cap/vocs/inc/vocss.h"

#include "../render_cap/vcs/inc/vcs.h"
#include "../render_cap/vcs/inc/vcsc.h"
#include "../render_cap/vcs/inc/vcss.h"

#include "../stream/pacs/inc/pacs.h"
#include "../stream/pacs/inc/pacsc.h"
#include "../stream/pacs/inc/pacss.h"

#include "../stream/ascs/inc/ascs.h"
#include "../stream/ascs/inc/ascsc.h"
#include "../stream/ascs/inc/ascss.h"

#include "../stream/bass/inc/bass.h"
#include "../stream/bass/inc/bassc.h"
#include "../stream/bass/inc/basss.h"

#include "../stream/inc/bap.h"
#include "../stream/inc/bap_us.h"
#include "../stream/inc/bap_uc.h"
#include "../stream/inc/unicast_client.h"
#include "../stream/inc/bap_sink.h"
#include "../stream/inc/bap_source.h"
#include "../stream/inc/bap_assistant.h"

#include "../trans_coord/csis/inc/csis.h"
#include "../trans_coord/csis/inc/csisc.h"
#include "../trans_coord/csis/inc/csiss.h"

#include "../trans_coord/cap/inc/cap.h"
#include "../trans_coord/cap/inc/cap_uc.h"
#include "../trans_coord/cap/inc/cap_device.h"

#include "../user_case/has/inc/has.h"
#include "../user_case/has/inc/hasc.h"
#include "../user_case/has/inc/hass.h"

#include "../user_case/tmas/inc/tmas.h"
#include "../user_case/tmas/inc/tmasc.h"
#include "../user_case/tmas/inc/tmass.h"

#include "../user_case/pbp/inc/pbp_ext_adv.h"

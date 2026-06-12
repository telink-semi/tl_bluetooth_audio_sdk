/********************************************************************************************************
 * @file    host.h
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

#include "leh.h"

#include "ble_host.h"
#include "ble_host_acl.h"
#include "ble_host_sal.h"

// misc header files.
#include "../misc/inc/ble_misc.h"
#include "../misc/inc/ble_rand.h"
#include "../misc/inc/ble_crypto.h"

// include all hci/inc header files.
#include "../hci/inc/ble_acl_data.h"
#include "../hci/inc/ble_hci_cmd.h"
#include "../hci/inc/ble_hci_evt.h"
#include "../hci/inc/ble_hci.h"
#include "../hci/inc/ble_iso_data.h"

// include all hci/cmd header files.
#include "../hci/cmd/inc/hci_cmd_bb.h"
#include "../hci/cmd/inc/hci_cmd_info_param.h"
#include "../hci/cmd/inc/hci_cmd_link_ctrl.h"
#include "../hci/cmd/inc/hci_cmd_link_policy.h"
#include "../hci/cmd/inc/hci_cmd_status_param.h"
#include "../hci/cmd/inc/hci_cmd_test.h"

// include all hci/le_cmd header files.
#include "../hci/le_cmd/inc/hci_cmd_le_adv.h"
#include "../hci/le_cmd/inc/hci_cmd_le_bis.h"
#include "../hci/le_cmd/inc/hci_cmd_le_cis.h"
#include "../hci/le_cmd/inc/hci_cmd_le_ext_adv.h"
#include "../hci/le_cmd/inc/hci_cmd_le_init.h"
#include "../hci/le_cmd/inc/hci_cmd_le_iso.h"
#include "../hci/le_cmd/inc/hci_cmd_le_misc.h"
#include "../hci/le_cmd/inc/hci_cmd_le_pcl.h"
#include "../hci/le_cmd/inc/hci_cmd_le_pa.h"
#include "../hci/le_cmd/inc/hci_cmd_le_scan.h"
#include "../hci/le_cmd/inc/hci_cmd_le_test.h"

// L2CAP header files.
#include "../l2cap/inc/ble_l2cap.h"

// L2CAP ATT header files.
#include "../l2cap/att/inc/ble_att.h"
#include "../l2cap/att/inc/ble_att_pdu_format.h"
#include "../l2cap/att/inc/ble_att_uuid.h"
#include "../l2cap/att/inc/ble_att_service.h"
#include "../l2cap/att/inc/ble_att_package.h"
#include "../l2cap/att/inc/uuid16bit.h"
#include "../l2cap/att/inc/uuid128bit.h"

// L2CAP SMP header files.
#include "../l2cap/smp/inc/ble_smp.h"
#include "../l2cap/smp/inc/ble_smp_store.h"

// L2CAP Signaling header files.
#include "../l2cap/signaling/inc/ble_signaling.h"

// L2CAP CoC header files.
#include "../l2cap/coc/inc/ble_coc.h"

// GATT header files.
#include "../gatt/inc/gatt.h"
#include "../gatt/gatts/inc/ble_gatts.h"
#include "../gatt/gatts/inc/gatts_req.h"
#include "../gatt/gatts/inc/gatts_sdp.h"
#include "../gatt/gattc/inc/gattc.h"
#include "../gatt/gattc/inc/gattc_req.h"
#include "../gatt/sdp/inc/ble_sdp.h"
#include "../gatt/sdp/inc/ble_ssdp.h"

// services common header files.
#include "../services/inc/svc_format.h"

// services GATT header files.
#include "../services/svc_gatt/bas/svc_battery.h"
#include "../services/svc_gatt/dis/svc_dis.h"
#include "../services/svc_gatt/core/svc_core.h"
#include "../services/svc_gatt/scps/svc_scps.h"

// services HID header files.
#include "../services/svc_hid/hid/hid_def.h"
#include "../services/svc_hid/hid/svc_hid.h"
#include "../services/svc_hid/hid_iso/svc_hid_iso.h"

// services GTBS header files. // todo: remove this line.
#include "../services/svc_audio/tbs/svc_gtbs.h"
#include "../services/svc_audio/mcs/svc_gmcs.h"
#include "../services/svc_telink/ota/svc_ota.h"
#include "../services/svc_telink/ota_v2/svc_ota_v2.h"

// GAP header files.
#include "../gap/inc/ble_gap.h"

// GAP legacy adv & extended adv header files.
#include "../gap/adv/inc/ble_gap_adv.h"
#include "../gap/adv/inc/ble_gap_ext_adv.h"

// GAP Filter Accept List header files.
#include "../gap/filter/inc/ble_gap_filter.h"

// GAP legacy scan & extended scan header files.
#include "../gap/scan/inc/ble_gap_scan.h"
#include "../gap/scan/inc/ble_gap_ext_scan.h"

// GAP acl connection header files.
#include "../gap/acl/inc/ble_gap_acl.h"
#include "../gap/acl/inc/ble_gap_acl_central.h"
#include "../gap/acl/inc/ble_gap_acl_peripheral.h"

// GAP misc header files.
#include "../gap/misc/inc/ble_gap_addr.h"

// GAP event dispatch header files.
#include "../gap/evt_dispatch/inc/ble_gap_evt_dispatch.h"

// GAP Periodic advertising header files.
#include "../gap/pa/inc/ble_gap_pa.h"
#include "../gap/pa/inc/ble_gap_pa_sync.h"

// GAP ISO header files.
#include "../gap/iso/inc/ble_gap_iso.h"
#include "../gap/iso/inc/ble_gap_big.h"
#include "../gap/iso/inc/ble_gap_big_sync.h"
#include "../gap/iso/inc/ble_gap_cis.h"
#include "../gap/iso/inc/ble_gap_cis_central.h"
#include "../gap/iso/inc/ble_gap_cis_peripheral.h"

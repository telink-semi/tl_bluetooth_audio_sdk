## initial profile client code.

base client code:
```c
#include <string.h>

#include "common/types.h"
#include "common/utility.h"
#include "common/bit.h"

#include "../../../inc/profile.h"
#include "../../../inc/profile_internal.h"

#include "../../../../gatt/gattc/inc/gattc.h"

#include "../../inc/prf_lea.h"

// SSDP module
#include "../../../../l2cap/att/inc/ble_att_uuid.h"
#include "../../../../gatt/sdp/inc/ble_ssdp.h"

// ATT UUID define 
#include "../../../../l2cap/att/inc/uuid_def.h"

// LOG module
#include "../../../../inc/ble_host_sal.h"
#include "../../inc/prf_lea_log.h"

// BLE Host
#include "../../../../inc/ble_host.h"

#include "inc/gmcs.h"
#include "inc/gmcsc.h"
#include "inc/gmcsc_internal.h"

#define BLE_GMCS_CLIENT_MALLOC(size)            BLE_PRF_MALLOC_SPEC_PRF_CLIENT(size, SERVICE_ID_GMCS)
#define BLE_GMCS_CLIENT_FREE(ptr)               ble_prf_free(ptr)

BLE_FUNC_DEFINE_INIT_CONNECT_DISC(gmcs)

static const struct ble_prf_param s_gmcs_client_params = {
    .client = 1,
    .used_acl_role = PRF_USED_ACL_ROLE_PERIPHERAL,
    .service_id = SERVICE_ID_GMCS,
    .sec_flag = 1,
    .init = ble_gmcsc_init,
    .connect = ble_gmcsc_connect,
    .discovery = ble_gmcsc_discovery,
};

static struct ble_gmcs_client_control s_gmcs_client_ctrl = {
    .prf_process = {
        .prf_params = &s_gmcs_client_params,
    },
};

void ble_lea_register_GMCS_control_client(const struct ble_gmcsc_register_param *param)
{
    blc_prf_register_service_module(&s_gmcs_client_ctrl.prf_process, param);
}

BLE_FUNC_PRF_CLIENT_INIT_CONNECT_DISC(gmcs, GMCS)

static void ble_gmcsc_data_input(uint16_t conn_handle, struct ble_gattc_ccc_value *ntf_value)
{
    (void) conn_handle;
    (void) ntf_value;
    // TODO: implement this function
}

static void ble_gmcsc_display_information(struct ble_gmcs_client *p_gmcs_client)
{
    (void) p_gmcs_client;
}

BLE_FUNC_PRF_SDP_DISCOVERY_SERVICE(gmcs, GMCS)

BLE_SSDP_CHARACTERISTIC_NAME(gmcs) = {
};

BLE_PRF_DEFINE_SSDP_NO_INCLUDE_LIST(gmcs, serviceBatteryAttUuid);
```

/********************************************************************************************************
 * @file    planner.h
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
#include"../../tl_common.h"
#ifndef _STACK_SYSTEM_PLANNER_H_
#define _STACK_SYSTEM_PLANNER_H_
#define TLK_PLANNER_UNIT_US            625

#define TLK_PLANNER_MODE_CONCENTRATED  0
#define TLK_PLANNER_MODE_SCATTERED     1

typedef enum
{
	TLK_PLANNER_SUCCESS,
	TLK_PLANNER_BW_FULL,
	TLK_PLANNER_BW_CONFLICT,
	TLK_PLANNER_NOT_FOUND,
}tlk_planner_ret_e;

typedef enum
{
    TLK_PLANNER_0MS     = 0,
    TLK_PLANNER_1P25MS  = 2,
    TLK_PLANNER_2P5MS   = 4,
    TLK_PLANNER_3P75MS  = 6,
    TLK_PLANNER_5MS     = 8,
    TLK_PLANNER_6P25MS  = 10,
    TLK_PLANNER_7P5MS   = 12,
    TLK_PLANNER_8P75MS  = 14,
    TLK_PLANNER_10MS    = 16,
    TLK_PLANNER_12P5MS  = 20,
    TLK_PLANNER_15MS    = 24,
    TLK_PLANNER_17P5MS  = 28,
    TLK_PLANNER_20MS    = 32,
    TLK_PLANNER_22P5MS  = 36,
    TLK_PLANNER_25MS    = 40,
    TLK_PLANNER_27P5MS  = 44,
    TLK_PLANNER_30MS    = 48,
    TLK_PLANNER_35MS    = 56,
    TLK_PLANNER_40MS    = 64,
    TLK_PLANNER_45MS    = 72,
    TLK_PLANNER_50MS    = 80,
}tlk_planner_time_e;

typedef struct tlk_planner_node_t tlk_planner_node_t;
typedef void(*tlk_planner_cb_f)(tlk_planner_node_t *,u16,u16);
typedef struct __attribute__((packed)) tlk_planner_node_t
{
    u8  id;
    u8  offset;//*TLK_PLANNER_UNIT_US
    u8  shift; //*gPlannerCtrl.offsetMax
    u8  rsvd;
    u32 interval;//us
    u16 duration;//*TLK_PLANNER_UNIT_US
    u16 rsvd1;
    tlk_planner_cb_f    cb;
    tlk_planner_node_t* next;
}tlk_planner_node_t;

typedef struct __attribute__((packed))
{
    u16 baseInterval;
    u16 offsetMax;
    u32 plannerStartTick; 
    u32 plannerStartClkn;
    u32 plannerStartFcnt;
}tlk_planner_ctrl_t;
extern tlk_planner_ctrl_t gPlannerCtrl;


void              tlk_planner_init_base_interval(tlk_planner_time_e baseInterval);
void              tlk_planner_set_base_interval(tlk_planner_time_e baseInterval);
_attribute_ram_code_ tlk_planner_time_e tlk_planner_get_base_interval(void);
tlk_planner_ret_e tlk_planner_insert_node(tlk_planner_node_t* pNode);
tlk_planner_ret_e tlk_planner_delete_node(tlk_planner_node_t* pNode);
tlk_planner_ret_e tlk_planner_check_node(tlk_planner_node_t* pNode);
void tlk_planner_clear_node(void);
tlk_planner_ret_e tlk_planner_parameter_check(tlk_planner_node_t* pNode);
tlk_planner_ret_e tlk_planner_parameter_request(u8 mode,u32 interval,u16 durationMin,u16 durationMax,tlk_planner_node_t* pNode);
void              tlk_planner_update_map_point_tick(u32 mapPointTick);
u32               tlk_planner_get_ahchor_tick(tlk_planner_node_t* pNode,u32 refTick);
bool 			  tlk_planner_list_is_null();
#endif

#ifndef __SCREENS_H__
#define __SCREENS_H__

#include "scr_startup.h"
#include "scr_idle.h"
#include "scr_info.h"
#include "scr_noen.h"
#include "scr_es35sw_th_sensor.h"
#include "scr_lhio404_io_device.h"
#include "scr_game_zomwar.h"

// scr_game_zomwar
extern view_dynamic_t dyn_view_item_game_zomwar;
extern view_screen_t scr_game_zomwar;
extern void scr_zw_game_handle(ak_msg_t* msg);

// scr_game_menu
extern view_dynamic_t dyn_view_item_game_menu;
extern view_screen_t scr_game_menu;
extern void scr_game_menu_handle(ak_msg_t* msg);

// scr_game_setting
extern view_dynamic_t dyn_view_item_game_setting;
extern view_screen_t scr_game_setting;
extern void scr_game_setting_handle(ak_msg_t* msg);

// scr_game_rank
extern view_dynamic_t dyn_view_item_game_rank;
extern view_screen_t scr_game_rank;
extern void scr_game_rank_handle(ak_msg_t* msg);

#endif //__SCREENS_H__

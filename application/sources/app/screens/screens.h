#ifndef __SCREENS_H__
#define __SCREENS_H__

#include "scr_startup.h"
#include "scr_idle.h"
#include "scr_info.h"
#include "scr_noen.h"
#include "scr_es35sw_th_sensor.h"
#include "scr_lhio404_io_device.h"
#include "scr_zomwar_game.h"

// scr_zomwar_game
extern view_dynamic_t dyn_view_item_zomwar_game;
extern view_screen_t scr_zomwar_game;
extern void scr_zw_game_handle(ak_msg_t* msg);

// scr_menu_game
extern view_dynamic_t dyn_view_item_menu_game;
extern view_screen_t scr_menu_game;
extern void scr_menu_game_handle(ak_msg_t* msg);

#endif //__SCREENS_H__

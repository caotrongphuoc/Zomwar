#ifndef __SCR_GAME_MENU_H__
#define __SCR_GAME_MENU_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"

#include "buzzer.h"

#include "screens.h"
#include "scr_game_bitmap.h"
#include "scr_game_zomwar.h"
#include "scr_game_setting.h"
#include "scr_game_rank.h"
#include "scr_idle.h"

#include "zw_game_gunner.h"
#include "zw_game_bullet.h"

extern view_dynamic_t dyn_view_item_game_menu;
extern view_screen_t scr_game_menu;
extern void scr_game_menu_handle(ak_msg_t* msg);

#endif //__SCR_GAME_MENU_H__
#ifndef __scr_GAME_RANK_H__
#define __scr_GAME_RANK_H__

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

#include "eeprom.h"
#include "app_eeprom.h"

#include "screens.h"
#include "scr_game_bitmap.h"

#define SPIDER_COUNT         (3) // So luong nhen
#define SPIDER_BITMAP_MID    (7) // 
#define SPIDER_BOB_RANGE     (3)

extern view_dynamic_t dyn_view_item_charts_game;
extern view_screen_t scr_game_rank;
extern void scr_game_rank_handle(ak_msg_t* msg);

#endif //__scr_GAME_RANK_H__

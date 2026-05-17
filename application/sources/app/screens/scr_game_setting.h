#ifndef __SCR_GAME_SETTING_H__
#define __SCR_GAME_SETTING_H__

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

#define STEP_SETTING_CHOSSE 					(13)
#define SETTING_ITEM_ARRDESS_0					(0)
#define SETTING_ITEM_ARRDESS_1					(STEP_SETTING_CHOSSE)
#define SETTING_ITEM_ARRDESS_2					(STEP_SETTING_CHOSSE*2)
#define SETTING_ITEM_ARRDESS_3					(STEP_SETTING_CHOSSE*3)
#define SETTING_ITEM_ARRDESS_4					(STEP_SETTING_CHOSSE*4)
#define SETTING_ITEM_ARRDESS_5					(STEP_SETTING_CHOSSE*5)
#define ZW_GAME_SETTING_FRAMES_AXIS_X			(0)
#define ZW_GAME_SETTING_FRAMES_AXIS_Y_1			(2)
#define ZW_GAME_SETTING_FRAMES_STEP 			(12)
#define ZW_GAME_SETTING_FRAMES_SIZE_W			(123)
#define ZW_GAME_SETTING_FRAMES_SIZE_H			(11)
#define ZW_GAME_SETTING_FRAMES_SIZE_R			(3)

extern view_dynamic_t dyn_view_item_game_setting;
extern view_screen_t scr_game_setting;
extern void scr_game_setting_handle(ak_msg_t* msg);

#endif //__SCR_GAME_SETTING_H__
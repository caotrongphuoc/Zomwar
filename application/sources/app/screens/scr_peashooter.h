#ifndef __SCR_PEASHOOTER_H__
#define __SCR_PEASHOOTER_H__

#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "view_render.h"
#include "screens_bitmap.h"

extern view_dynamic_t dyn_view_peashooter;
extern view_screen_t scr_peashooter;
extern void scr_peashooter_handle(ak_msg_t* msg);

#endif //__SCR_PEASHOOTER_H__

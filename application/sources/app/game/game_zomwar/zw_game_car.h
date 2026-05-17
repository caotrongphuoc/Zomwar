#ifndef __ZW_GAME_CAR_H__
#define __ZW_GAME_CAR_H__

#include <stdio.h>

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"

#include "scr_game_zomwar.h"

#define AXIS_X_CAR        (0)       // Toa do cua car
#define NUM_LANES         (5)       // So luong lane duong
#define CAR_SPEED         (4)       // Toc do cua car = 4px
#define SIZE_BITMAP_CAR_X (12)      // Kich thuoc chieu ngang cua car
#define SIZE_BITMAP_CAR_Y (10)      // Kich thuoc chieu cao cua car
#define CAR_HIT_RANGE_Y   (8)       // Tam dung cua car
#define LANE_Y  {2, 12, 22, 32, 42} // Toa do cua cac lane duong

typedef struct {
    bool visible;
    bool running;
    uint32_t x, y;
    uint8_t lane;
    uint8_t action_image;
} zw_game_car_t;

extern zw_game_car_t car[NUM_LANES];

//void zw_game_car_handle(ak_msg_t* msg);

#endif //ZW_GAME_CAR_H__

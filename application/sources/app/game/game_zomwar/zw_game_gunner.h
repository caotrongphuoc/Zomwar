#ifndef __ZW_GAME_GUNNER_H__
#define __ZW_GAME_GUNNER_H__

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

#define STEP_GUNNER_AXIS_Y     (10) // So buoc moi lan Gunner di chuyen len xuong
#define SIZE_BITMAP_GUNNER_X   (25) // Kich thuoc chieu ngang cua Gunner
#define SIZE_BITMAP_GUNNER_Y   (10) // Kich thuoc chieu cao cua Gunner
#define AXIS_X_GUNNER          (14) // Toa do x cua Gunner ban dau
#define AXIS_Y_GUNNER          (52) // Toa do y cua Gunner ban dau
#define AXIS_Y_GUNNER_MIN      (12) // Toa do toi thieu cua Gunner
#define AXIS_Y_GUNNER_MAX      (52) // Toa do toi da cua Gunner

// Cac trang thai cua Gunner: dung yen, di chuyen len, di chuyen xuong
typedef enum {
    GUNNER_MOVE_NONE = 0,
    GUNNER_MOVE_UP,
    GUNNER_MOVE_DOWN,
} gunner_move_dir_t;

typedef struct {
    bool visible;
    uint32_t x, y;
    uint8_t action_image;
} zw_game_gunner_t;

extern zw_game_gunner_t gunner;


#endif //__ZW_GAME_GUNNER_H__

#include "zw_game_gunner.h"

zw_game_gunner_t gunner;
static uint8_t gunner_y ; // Toa do xa thu

void zw_game_gunner_handle(ak_msg_t* msg) {
    switch (msg->sig) {

    // Khoi tao trang thai ban dau cua Gunner    
    case ZW_GAME_GUNNER_SETUP: 
        APP_DBG_SIG("ZW_GAME_GUNNER_SETUP\n");
        gunner.x = AXIS_X_GUNNER;
        gunner.y = AXIS_Y_GUNNER;
        gunner.visible = WHITE;
        gunner.action_image = 1;
        gunner_y = AXIS_Y_GUNNER;
        break;

    // Gunner di chuyen len
    case ZW_GAME_GUNNER_UP: 
        APP_DBG_SIG("ZW_GAME_GUNNER_UP\n");
        gunner_y -= STEP_GUNNER_AXIS_Y;
        if(gunner_y < AXIS_Y_GUNNER_MIN) {
            gunner_y = AXIS_Y_GUNNER_MIN;
        }
        gunner.y = gunner_y; // Cap nhat vi tri Gunner
        break;

    // Gunner di chuyen xuong
    case ZW_GAME_GUNNER_DOWN: 
        APP_DBG_SIG("ZW_GAME_GUNNER_DOWN\n");
        gunner_y += STEP_GUNNER_AXIS_Y;
        if(gunner_y > AXIS_Y_GUNNER_MAX) {
            gunner_y = AXIS_Y_GUNNER_MAX;
        }
        gunner.y = gunner_y; // Cap nhat vi tri Gunner
        break;

    // Cap nhat hoat anh cua Gunner
    case ZW_GAME_GUNNER_UPDATE:
        if (gunner.action_image == 2) {
            gunner.action_image = 1;
        }
        break;

    // Reset Gunner ve trang thai ban dau
    case ZW_GAME_GUNNER_RESET: 
        APP_DBG_SIG("ZW_GAME_GUNNER_RESET\n");
        gunner.x = AXIS_X_GUNNER;
        gunner.y = AXIS_Y_GUNNER;
        gunner.visible = BLACK;
        gunner_y = AXIS_Y_GUNNER;
        break;

    default:
        break;
    }
}

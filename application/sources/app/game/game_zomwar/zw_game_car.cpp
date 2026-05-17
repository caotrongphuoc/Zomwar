#include "zw_game_car.h"
//#include "zw_game_zombie.h"

zw_game_car_t car[NUM_LANES];
static const int8_t lane_y[NUM_LANES] = LANE_Y;
//static bool game_active = false; 

void zw_game_car_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_CAR_SETUP: 
        APP_DBG_SIG("ZW_GAME_CAR_SETUP\n");
        //game_active = true; 
        for (uint8_t i = 0; i < NUM_LANES; i++) { 
            car[i].x       = AXIS_X_CAR; 
            car[i].y       = lane_y[i]; 
            car[i].lane    = i; 
            //car[i].visible = (settingsetup.num_car >> i) & 1; 
            car[i].visible = true; 
            car[i].running = false; 
            car[i].action_image = 1; 
        } 
        break;

    case ZW_GAME_CAR_RUN: {
        APP_DBG_SIG("ZW_GAME_CAR_RUN\n");
        //ZW_GAME_CAR_RUN();
    }
        break;

    case ZW_GAME_CAR_RESET: {
        APP_DBG_SIG("ZW_GAME_CAR_RESET\n");
        //game_active = false; 
        for (uint8_t i = 0; i < NUM_LANES; i++) { 
            car[i].x       = AXIS_X_CAR; 
            car[i].y       = lane_y[i]; 
            car[i].visible = false; 
            car[i].running = false; 
        } 
    }
        break;

    default:
        break;
    }
}
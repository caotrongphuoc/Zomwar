#include "zw_game_bullet.h"
#include "zw_game_gunner.h"

zw_game_bullet_t bullet[MAX_NUM_BULLET];
bool zw_game_sound_enable = true; // Kich hoat am thanh

void zw_game_bullet_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case ZW_GAME_BULLET_SETUP: 
        APP_DBG_SIG("ZW_GAME_BULLET_SETUP\n");
        for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { 
            bullet[i].x = 0;
            bullet[i].y = 0; 
            bullet[i].visible = BLACK; 
            //bullet[i].action_image = 1; 
        } 
        break;

    case ZW_GAME_BULLET_RUN: 
        APP_DBG_SIG("ZW_GAME_BULLET_RUN\n");
        for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { 
            if (bullet[i].visible == WHITE) {
                bullet[i].x += STEP_BULLET_AXIS_X;
                if (bullet[i].x >= MAX_AXIS_X_BULLET) { 
                    bullet[i].visible = BLACK; 
                    bullet[i].x = 0; 
                } 
            } 
        } 
        break;

    case ZW_GAME_BULLET_SHOOT: 
        APP_DBG_SIG("ZW_GAME_BULLET_SHOOT\n");
        for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { 
            if (bullet[i].visible == BLACK) { 
                bullet[i].visible = WHITE; 
                bullet[i].x = gunner.x + 15; 
                bullet[i].y = gunner.y -8; 
                gunner.action_image = 2;
                if(zw_game_sound_enable) {
                    BUZZER_PlayTones(tones_cc); 
                }
                break; 
            } 
        } 
        break;

    case ZW_GAME_BULLET_RESET: 
        APP_DBG_SIG("ZW_GAME_BULLET_RESET\n");
        for (uint8_t i = 0; i < MAX_NUM_BULLET; i++) { 
            bullet[i].x = 0; 
            bullet[i].y = 0; 
            bullet[i].visible = BLACK; 
            //bullet[i].action_image = 1; 
        } 
        break;

    default:
        break;
    }
}
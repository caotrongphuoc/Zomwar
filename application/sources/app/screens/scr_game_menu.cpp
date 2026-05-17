#include "scr_game_menu.h"

/*****************************************************************************/
/* Variable and Struct Declaration - Menu game */
/*****************************************************************************/

#define NUMBER_ITEMS (4)

static uint8_t shoot_tick_counter;   // Dem tick de vien dan duoc ban ra theo vong lap
static uint8_t current_location = 0; // Tua de menu hien tai

static char items_name[NUMBER_ITEMS][30] = {
	"<<   Play   >>",
	"<<  Setting >>",
	"<<   Rank   >>",
	"<<   Exit   >>",
};


/*****************************************************************************/
/* View - Menu game */
/*****************************************************************************/
static void view_scr_game_menu();

view_dynamic_t dyn_view_game_menu = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_menu
};

view_screen_t scr_game_menu = {
	&dyn_view_game_menu,
	ITEM_NULL,
	ITEM_NULL,
	.focus_item = 0,
};

void view_scr_game_menu() {
    view_render.setTextSize(2);
    view_render.setTextColor(WHITE);
    view_render.setCursor(30,0); 
    view_render.print("ZOMWAR");
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(23, 17);
	view_render.print(items_name[current_location]);
    view_render.drawBitmap(60, LCD_HEIGHT-22, bitmap_tombstone, 8 ,10, WHITE);
	for (uint8_t x = 0; x < 128; x += 5) {
		view_render.drawPixel(x,     LCD_HEIGHT-13, WHITE);
		view_render.drawPixel(x + 1, LCD_HEIGHT-13, WHITE);
		view_render.drawPixel(x + 2, LCD_HEIGHT-13, WHITE);
	}
    zw_game_bullet_display();
    zw_game_gunner_display();
}

void scr_game_menu_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: 
		APP_DBG_SIG("SCREEN_ENTRY\n");
		zw_game_sound_enable = false;
		current_location = 0;
		task_post_pure_msg(ZW_GAME_GUNNER_ID, ZW_GAME_GUNNER_SETUP);
		task_post_pure_msg(ZW_GAME_BULLET_ID, ZW_GAME_BULLET_SETUP);
        zw_game_time_tick_setup();
		break;

    case ZW_GAME_TIME_TICK: 
		APP_DBG_SIG("ZW_GAME_TIME_TICK\n");
		task_post_pure_msg(ZW_GAME_GUNNER_ID, ZW_GAME_GUNNER_UPDATE);
		task_post_pure_msg(ZW_GAME_BULLET_ID, ZW_GAME_BULLET_RUN);
        shoot_tick_counter++;
        if(shoot_tick_counter >= 20) {
            shoot_tick_counter = 0;
            task_post_pure_msg(ZW_GAME_BULLET_ID, ZW_GAME_BULLET_SHOOT);
        }
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED:
		current_location++;
		if(current_location >= NUMBER_ITEMS) {
			current_location = 0;
		}
		BUZZER_PlayTones(tones_cc);
		break;
	
	case AC_DISPLAY_BUTTON_UP_RELEASED:
		if(current_location == 0) {
			current_location = NUMBER_ITEMS - 1;
		} else {
			current_location--;
		}
		BUZZER_PlayTones(tones_cc);
		break;


	case AC_DISPLAY_BUTTON_MODE_RELEASED: 
		zw_game_sound_enable = true;
		switch (current_location) {
			case 0: SCREEN_TRAN(scr_zw_game_handle, &scr_game_zomwar); break;
			case 1: SCREEN_TRAN(scr_game_setting_handle, &scr_game_setting); break;
			case 2: SCREEN_TRAN(scr_game_rank_handle, &scr_game_rank); break;
			case 3: SCREEN_TRAN(scr_idle_handle, &scr_idle); break;
		}
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}

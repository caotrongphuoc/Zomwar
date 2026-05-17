#include "scr_game_rank.h"

/*****************************************************************************/
/* Variable Declaration - Charts game */
/*****************************************************************************/

static const uint8_t SPIDER_X[SPIDER_COUNT]        = {28, 56, 80}; // Vi tri x cua nhen
static const int8_t  SPIDER_TARGET_Y[SPIDER_COUNT] = { 4,  6,  4}; // Vi tri cuoi cung cua nhen
static const int8_t  SPIDER_INIT_Y[SPIDER_COUNT]   = {-16, -26, -36}; // Vi tri y ban dau cua nhen, < 0 do nam ngoai man hinh
static int8_t  spider_y[SPIDER_COUNT]; // Vi tri cua nhen cap nhat sau moi tick
static int8_t  spider_dir[SPIDER_COUNT]; // Huong di chuyen cua nhen, 1 - nhen di xuong, -1 - nhen di len
static uint8_t spider_state; // Trang thai cua nhen, 0 - dang bo tu tren xuong, 1 - da den dich va dang lac lu

/*****************************************************************************/
/* View - Charts game */
/*****************************************************************************/
static void view_scr_game_rank();

view_dynamic_t dyn_view_item_charts_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_rank
};

view_screen_t scr_game_rank = {
	&dyn_view_item_charts_game,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_game_rank() {
	view_render.setTextSize(1);

	view_render.drawBitmap(0,   0, bitmap_spiderweb_L, 16, 16, WHITE);
	view_render.drawBitmap(110, 0, bitmap_spiderweb_R, 16, 16, WHITE);

	for (uint8_t i = 0; i < SPIDER_COUNT; i++) {
		uint8_t tx = SPIDER_X[i] + SPIDER_BITMAP_MID;

		if (spider_y[i] > 0) {
			view_render.drawLine(tx, 0, tx, (int16_t)spider_y[i], WHITE);
		}

		view_render.drawBitmap(SPIDER_X[i], spider_y[i], bitmap_spider, 16, 16, WHITE);
	}

	view_render.setTextColor(WHITE);
	view_render.setCursor(55, 28);
	view_render.print("1st");
	view_render.drawBitmap(50, 38, bitmap_tombstone_rank, 30, 22, WHITE);
	view_render.setTextColor(BLACK);
	// if (gamescore.score_1st >= 0 && gamescore.score_1st < 10) {
	// 	view_render.setCursor(62, 45);
	// 	view_render.print(gamescore.score_1st);
	// }
	// else if (gamescore.score_1st >= 10 && gamescore.score_1st < 100) {
	// 	view_render.setCursor(59, 45);
	// 	view_render.print(gamescore.score_1st);
	// }
	// else if (gamescore.score_1st >= 100 && gamescore.score_1st < 1000) {
	// 	view_render.setCursor(56, 45);
	// 	view_render.print(gamescore.score_1st);
	// }
	// else {
	// 	view_render.setCursor(53, 45);
	// 	view_render.print(gamescore.score_1st);
	// }

	view_render.setTextColor(WHITE);
	view_render.setCursor(15, 30);
	view_render.print("2nd");
	view_render.drawBitmap(10, 40, bitmap_tombstone_rank, 30, 22, WHITE);
	view_render.setTextColor(BLACK);
	// if (gamescore.score_2nd >= 0 && gamescore.score_2nd < 10)
	// {
	// 	view_render.setCursor(22, 47);
	// 	view_render.print(gamescore.score_2nd);
	// }
	// else if (gamescore.score_2nd >= 10 && gamescore.score_2nd < 100)
	// {
	// 	view_render.setCursor(19, 47);
	// 	view_render.print(gamescore.score_2nd);
	// }
	// else if (gamescore.score_2nd >= 100 && gamescore.score_2nd < 1000){
	// 	view_render.setCursor(16, 47);
	// 	view_render.print(gamescore.score_2nd);
	// }
	// else {
	// 	view_render.setCursor(13, 47);
	// 	view_render.print(gamescore.score_2nd);
	// }

	view_render.setTextColor(WHITE);
	view_render.setCursor(95, 32);
	view_render.print("3rd");
	view_render.drawBitmap(90, 42, bitmap_tombstone_rank, 30, 22, WHITE);
	view_render.setTextColor(BLACK);
	// if (gamescore.score_3rd >= 0 && gamescore.score_3rd < 10)
	// {
	// 	view_render.setCursor(102, 49);
	// 	view_render.print(gamescore.score_3rd);
	// }
	// else if (gamescore.score_3rd >= 10 && gamescore.score_3rd < 100)
	// {
	// 	view_render.setCursor(99, 49);
	// 	view_render.print(gamescore.score_3rd);
	// }
	// else if (gamescore.score_3rd >= 100 && gamescore.score_3rd < 1000) {
	// 	view_render.setCursor(96, 49);
	// 	view_render.print(gamescore.score_3rd);
	// }
	// else {
	// 	view_render.setCursor(93, 49);
	// 	view_render.print(gamescore.score_3rd);
	// }
}

/*****************************************************************************/
/* Handle - Charts game */
/*****************************************************************************/
void scr_game_rank_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY:
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.initialize();
		view_render_display_on();
		//zw_game_score_read(&gamescore);
		for (uint8_t i = 0; i < SPIDER_COUNT; i++) {
			spider_y[i]   = SPIDER_INIT_Y[i];
			spider_dir[i] = 1;
		}
		spider_state = 0;
		timer_set(	AC_TASK_DISPLAY_ID, \
					ZW_GAME_TIME_SPIDER_UPDATE, \
					ZW_GAME_TIME_SPIDER_UPDATE_INTERVAL	, \
					TIMER_PERIODIC);
		break;

	case ZW_GAME_TIME_SPIDER_UPDATE:
		if (spider_state == 0) {
			uint8_t all_arrived = 1;
			for (uint8_t i = 0; i < SPIDER_COUNT; i++) {
				if (spider_y[i] < SPIDER_TARGET_Y[i]) {
					spider_y[i] += 4;
					if (spider_y[i] > SPIDER_TARGET_Y[i]) {
						spider_y[i] = SPIDER_TARGET_Y[i];
					}
					all_arrived = 0;
				}
			}
			if (all_arrived) {
				spider_dir[0] =  1;
				spider_dir[1] = -1;
				spider_dir[2] =  1;
				spider_state = 1;
			}
		} else {
			for (uint8_t i = 0; i < SPIDER_COUNT; i++) {
				spider_y[i] += spider_dir[i];
				if (spider_y[i] >= SPIDER_TARGET_Y[i] + SPIDER_BOB_RANGE) {
					spider_dir[i] = -1;
				} else if (spider_y[i] <= SPIDER_TARGET_Y[i] - SPIDER_BOB_RANGE) {
					spider_dir[i] = 1;
				}
			}
		}
		view_render_screen(&scr_game_rank);
		SCREEN_NONE_UPDATE_MASK();
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED:
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_RELEASED\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, ZW_GAME_TIME_SPIDER_UPDATE);
		SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_UP_RELEASED:
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_RELEASED\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, ZW_GAME_TIME_SPIDER_UPDATE);
		SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
		BUZZER_PlayTones(tones_cc);
		break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED: 
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_RELEASED\n");
		timer_remove_attr(AC_TASK_DISPLAY_ID, ZW_GAME_TIME_SPIDER_UPDATE);
		SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
		BUZZER_PlayTones(tones_cc);
		break;

	default:
		break;
	}
}

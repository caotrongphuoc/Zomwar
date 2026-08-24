#include "scr_game_zomwar.h"

/*****************************************************************************/
/* Variable Declaration - Game Zomwar */
/*****************************************************************************/

#define GUNNER_DIR_NONE (0)
#define GUNNER_DIR_UP (1)
#define GUNNER_DIR_DOWN (2)

static uint8_t zw_game_state;
static uint8_t gunner_dir = GUNNER_DIR_NONE;

zw_game_setting_t settingsetup;

void zw_game_frame_display()
{
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(8, 55);
	view_render.print("Wave:");
	view_render.print(wave_level);
	view_render.setCursor(64, 55);
	view_render.print("Score:");
	view_render.print(zw_game_score);
	view_render.drawLine(0, LCD_HEIGHT - 11, LCD_WIDTH, LCD_HEIGHT - 11, WHITE);
	view_render.drawRect(0, 0, 128, 64, 1);
}

/*****************************************************************************/
/* View - Game Zomwar */
/*****************************************************************************/

static void view_scr_game_zomwar();

view_dynamic_t dyn_view_zomwar = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_zomwar};

view_screen_t scr_game_zomwar = {
    &dyn_view_zomwar,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

void zw_game_gunner_display()
{
	if (gunner.visible != WHITE)
	{
		return;
	}
	const unsigned char* frame = bitmap_gunner_I;
	if (gunner.action_image == 2)
	{
		frame = bitmap_gunner_II;
	}
	view_render.drawBitmap(gunner.x, gunner.y - 10, frame, GUNNER_SIZE_BITMAP_X, GUNNER_SIZE_BITMAP_Y, WHITE);
}

void zw_game_bullet_display()
{
	for (uint8_t i = 0; i < BULLET_NUMBER; i++)
	{
		if (bullet[i].visible != WHITE)
		{
			continue;
		}
		view_render.drawBitmap(bullet[i].x, bullet[i].y, bitmap_bullet, BULLET_SIZE_BITMAP_X, BULLET_SIZE_BITMAP_Y, WHITE);
	}
}

void zw_game_zombie_display()
{
	for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
	{
		if (zombie[i].visible != WHITE)
		{
			continue;
		}
		const unsigned char* frame = bitmap_zombie_I;
		if (zombie[i].action_image == 2)
		{
			frame = bitmap_zombie_II;
		}
		else if (zombie[i].action_image == 3)
		{
			frame = bitmap_zombie_III;
		}
		view_render.drawBitmap(zombie[i].x, zombie[i].y, frame, ZOMBIE_SIZE_BITMAP_X, ZOMBIE_SIZE_BITMAP_Y, WHITE);
	}
}

void zw_game_car_display()
{
	for (uint8_t i = 0; i < CAR_LANE_NUMBER; i++)
	{
		if (car[i].visible != WHITE)
		{
			continue;
		}
		const unsigned char* frame = bitmap_car_I;
		if (car[i].action_image == 2)
		{
			frame = bitmap_car_II;
		}
		else if (car[i].action_image == 3)
		{
			frame = bitmap_car_III;
		}
		view_render.drawBitmap(car[i].x, car[i].y, frame, CAR_SIZE_BITMAP_X, CAR_SIZE_BITMAP_Y, WHITE);
	}
}

void zw_game_tombstone_display()
{
	for (uint8_t i = 0; i < TOMBSTONE_NUMBER; i++)
	{
		if (!tombstone[i].active)
		{
			continue;
		}
		view_render.drawBitmap(tombstone[i].x, lane_y[tombstone[i].lane], bitmap_tombstone, TOMBSTONE_SIZE_BITMAP_X, TOMBSTONE_SIZE_BITMAP_Y, WHITE);
	}
}

void zw_game_bang_display()
{
	for (uint8_t i = 0; i < BANG_NUMBER; i++)
	{
		if (bang[i].visible != WHITE)
		{
			continue;
		}
		const unsigned char* frame = bitmap_bang_I;
		uint8_t w = BANG_SIZE_BITMAP_I_X;
		uint8_t h = BANG_SIZE_BITMAP_I_Y;
		if (bang[i].action_image == 2)
		{
			frame = bitmap_bang_II;
			w = BANG_SIZE_BITMAP_II_X;
			h = BANG_SIZE_BITMAP_II_Y;
		}
		else if (bang[i].action_image == 3)
		{
			frame = bitmap_bang_III;
			w = BANG_SIZE_BITMAP_III_X;
			h = BANG_SIZE_BITMAP_III_Y;
		}
		view_render.drawBitmap(bang[i].x, bang[i].y, frame, w, h, WHITE);
	}
}

void zw_game_grass_display()
{
	for (uint8_t l = 0; l < CAR_LANE_NUMBER; l++)
	{
		uint8_t gy = lane_y[l] + 9;
		for (uint8_t x = 0; x < LCD_WIDTH; x += 5)
		{
			view_render.drawFastHLine(x, gy, 3, WHITE);
		}
	}
}

void zw_game_warning_display()
{
	if (wave_warning_active)
	{
		if ((wave_warning_timer / BORDER_WARNING_BLINK_RATE) % 2 == 0)
		{
			view_render.drawBitmap(
			    (LCD_WIDTH - BORDER_SIZE_BITMAP_WARNING_X) / 2,
			    (54 - BORDER_SIZE_BITMAP_WARNING_Y) / 2,
			    bitmap_warning,
			    BORDER_SIZE_BITMAP_WARNING_X,
			    BORDER_SIZE_BITMAP_WARNING_Y,
			    WHITE);
		}
	}
}

void view_scr_game_zomwar()
{
	if (zw_game_state == GAME_PLAY)
	{
		zw_game_frame_display();
		zw_game_gunner_display();
		zw_game_bullet_display();
		zw_game_zombie_display();
		zw_game_car_display();
		zw_game_tombstone_display();
		zw_game_bang_display();
		zw_game_grass_display();
		zw_game_warning_display();
	}
	else if (zw_game_state == GAME_OVER)
	{
		view_render.clear();
		view_render.drawBitmap(0, 0, bitmap_tomb_rip, LCD_WIDTH, LCD_HEIGHT, WHITE);
		view_render.drawBitmap(-2, 4, bitmap_branch_left, 50, 17, WHITE);
		view_render.drawBitmap(85, 8, bitmap_branch_right, 50, 17, WHITE);
		view_render.drawBitmap(0, 0, bitmap_grass, 64, 64, WHITE);
		view_render.drawBitmap(LCD_WIDTH / 2, 0, bitmap_grass, 64, 64, WHITE);
	}
}

/*****************************************************************************/
/* Handle - Game Zomwar */
/*****************************************************************************/

void scr_game_zomwar_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("ZW_GAME SCREEN_ENTRY\n");
		zw_game_setting_read(&settingsetup);
		task_post_pure_msg(ZW_GAME_GUNNER_ID, ZW_GAME_GUNNER_SETUP);
		task_post_pure_msg(ZW_GAME_BULLET_ID, ZW_GAME_BULLET_SETUP);
		task_post_pure_msg(ZW_GAME_ZOMBIE_ID, ZW_GAME_ZOMBIE_SETUP);
		task_post_pure_msg(ZW_GAME_CAR_ID, ZW_GAME_CAR_SETUP);
		task_post_pure_msg(ZW_GAME_TOMBSTONE_ID, ZW_GAME_TOMBSTONE_SETUP);
		task_post_pure_msg(ZW_GAME_BANG_ID, ZW_GAME_BANG_SETUP);
		task_post_pure_msg(ZW_GAME_BORDER_ID, ZW_GAME_BORDER_SETUP);
		zw_game_state = GAME_PLAY;
		gunner_dir = GUNNER_DIR_NONE;
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE);
		timer_set(AC_TASK_DISPLAY_ID,
		          ZW_GAME_TIME_TICK,
		          ZW_GAME_TIME_TICK_INTERVAL,
		          TIMER_PERIODIC);
	}
	break;

	case ZW_GAME_TIME_TICK:
	{
		APP_DBG_SIG("ZW_GAME_TIME_TICK\n");
		if (zw_game_state != GAME_PLAY)
		{
			break;
		}
		if (gunner_dir == GUNNER_DIR_UP)
		{
			task_post_pure_msg(ZW_GAME_GUNNER_ID, ZW_GAME_GUNNER_UP);
		}
		else if (gunner_dir == GUNNER_DIR_DOWN)
		{
			task_post_pure_msg(ZW_GAME_GUNNER_ID, ZW_GAME_GUNNER_DOWN);
		}
		task_post_pure_msg(ZW_GAME_GUNNER_ID, ZW_GAME_GUNNER_UPDATE);
		task_post_pure_msg(ZW_GAME_BULLET_ID, ZW_GAME_BULLET_RUN);
		task_post_pure_msg(ZW_GAME_ZOMBIE_ID, ZW_GAME_ZOMBIE_RUN);
		task_post_pure_msg(ZW_GAME_ZOMBIE_ID, ZW_GAME_ZOMBIE_DETONATOR);
		task_post_pure_msg(ZW_GAME_TOMBSTONE_ID, ZW_GAME_TOMBSTONE_SPAWN);
		task_post_pure_msg(ZW_GAME_CAR_ID, ZW_GAME_CAR_RUN);
		task_post_pure_msg(ZW_GAME_CAR_ID, ZW_GAME_CAR_HIT);
		task_post_pure_msg(ZW_GAME_BANG_ID, ZW_GAME_BANG_UPDATE);
		task_post_pure_msg(ZW_GAME_BORDER_ID, ZW_GAME_BORDER_CHECK_GAME_OVER);
		task_post_pure_msg(ZW_GAME_BORDER_ID, ZW_GAME_BORDER_CHECK_WAVE);
		task_post_pure_msg(ZW_GAME_BORDER_ID, ZW_GAME_BORDER_LEVEL_UP);
	}
	break;

	case ZW_GAME_RESET:
	{
		APP_DBG_SIG("ZW_GAME_RESET\n");
		if (zw_game_state != GAME_PLAY)
		{
			break;
		}
		timer_remove_attr(AC_TASK_DISPLAY_ID, ZW_GAME_TIME_TICK);
		task_post_pure_msg(ZW_GAME_GUNNER_ID, ZW_GAME_GUNNER_RESET);
		task_post_pure_msg(ZW_GAME_BULLET_ID, ZW_GAME_BULLET_RESET);
		task_post_pure_msg(ZW_GAME_ZOMBIE_ID, ZW_GAME_ZOMBIE_RESET);
		task_post_pure_msg(ZW_GAME_CAR_ID, ZW_GAME_CAR_RESET);
		task_post_pure_msg(ZW_GAME_TOMBSTONE_ID, ZW_GAME_TOMBSTONE_RESET);
		task_post_pure_msg(ZW_GAME_BANG_ID, ZW_GAME_BANG_RESET);
		task_post_pure_msg(ZW_GAME_BORDER_ID, ZW_GAME_BORDER_RESET);
		gamescore.score_now = zw_game_score;
		zw_game_state = GAME_OVER;
		BUZZER_PlaySound(BUZZER_SOUND_GOODBYE);
		timer_set(AC_TASK_DISPLAY_ID,
		          ZW_GAME_EXIT_GAME,
		          ZW_GAME_TIME_EXIT_INTERVAL,
		          TIMER_ONE_SHOT);
	}
	break;

	case AC_DISPLAY_BUTTON_UP_PRESSED:
	{
		APP_DBG_SIG("ZW_GAME BTN_UP_PRESSED\n");
		gunner_dir = GUNNER_DIR_UP;
	}
	break;

	case AC_DISPLAY_BUTTON_UP_RELEASED:
	{
		APP_DBG_SIG("ZW_GAME BTN_UP_RELEASED\n");
		if (gunner_dir == GUNNER_DIR_UP)
		{
			gunner_dir = GUNNER_DIR_NONE;
		}
	}
	break;

	case AC_DISPLAY_BUTTON_DOWN_PRESSED:
	{
		APP_DBG_SIG("ZW_GAME BTN_DOWN_PRESSED\n");
		gunner_dir = GUNNER_DIR_DOWN;
	}
	break;

	case AC_DISPLAY_BUTTON_DOWN_RELEASED:
	{
		APP_DBG_SIG("ZW_GAME BTN_DOWN_RELEASED\n");
		if (gunner_dir == GUNNER_DIR_DOWN)
		{
			gunner_dir = GUNNER_DIR_NONE;
		}
	}
	break;

	case AC_DISPLAY_BUTTON_MODE_PRESSED:
	{
		APP_DBG_SIG("ZW_GAME BTN_MODE_PRESSED\n");
		if (zw_game_state == GAME_PLAY)
		{
			task_post_pure_msg(ZW_GAME_BULLET_ID, ZW_GAME_BULLET_SHOOT);
		}
	}
	break;

	case ZW_GAME_EXIT_GAME:
	{
		APP_DBG_SIG("ZW_GAME_EXIT_GAME\n");
		zw_game_state = GAME_OFF;
		SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
	}
	break;

	default:
		break;
	}
}

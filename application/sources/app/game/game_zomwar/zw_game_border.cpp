#include "zw_game_border.h"

uint16_t zw_game_score = 0;
uint16_t wave_last_score = 0;
uint8_t wave_warning_timer = 0;
uint8_t wave_level = 0;
bool wave_warning_active = false;

static void zw_game_border_clear()
{
	zw_game_score = 0;
	wave_last_score = 0;
	wave_warning_timer = 0;
	wave_warning_active = false;
	wave_level = 0;
}

void zw_game_border_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ZW_GAME_BORDER_SETUP:
	{
		APP_DBG_SIG("ZW_GAME_BORDER_SETUP\n");
		zw_game_border_clear();
	}
	break;

	case ZW_GAME_BORDER_CHECK_GAME_OVER:
	{
		APP_DBG_SIG("ZW_GAME_BORDER_CHECK_GAME_OVER\n");
		for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
		{
			if (zombie[i].visible != WHITE)
			{
				continue;
			}
			if (zombie[i].x <= -(int32_t)ZOMBIE_MIN_LEFT_OFFSET)
			{
				if (zw_game_car_find_nearest(zombie[i].y) < 0)
				{
					task_post_pure_msg(AC_TASK_DISPLAY_ID, ZW_GAME_RESET);
					break;
				}
			}
		}
	}
	break;

	case ZW_GAME_BORDER_CHECK_WAVE:
	{
		APP_DBG_SIG("ZW_GAME_BORDER_CHECK_WAVE\n");
		if (!wave_warning_active &&
		    zw_game_score >= wave_last_score + BORDER_WAVE_SCORE_INTERVAL)
		{
			wave_warning_active = true;
			wave_warning_timer = BORDER_WARNING_BLINK_DURATION;
		}
	}
	break;

	case ZW_GAME_BORDER_LEVEL_UP:
	{
		APP_DBG_SIG("ZW_GAME_BORDER_LEVEL_UP\n");
		if (!wave_warning_active)
		{
			break;
		}
		if (wave_warning_timer > 0)
		{
			wave_warning_timer--;
			break;
		}
		wave_warning_active = false;
		wave_last_score += BORDER_WAVE_SCORE_INTERVAL;
		wave_level++;
		task_post_pure_msg(ZW_GAME_ZOMBIE_ID, ZW_GAME_ZOMBIE_WAVE_SPAWN);
	}
	break;

	case ZW_GAME_BORDER_RESET:
	{
		APP_DBG_SIG("ZW_GAME_BORDER_RESET\n");
		zw_game_border_clear();
	}
	break;

	default:
		break;
	}
}
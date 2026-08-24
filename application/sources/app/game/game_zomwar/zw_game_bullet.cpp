#include "zw_game_bullet.h"

zw_game_bullet_t bullet[BULLET_NUMBER];

static void zw_game_bullet_reset_all()
{
	for (uint8_t i = 0; i < BULLET_NUMBER; i++)
	{
		bullet[i].x = 0;
		bullet[i].y = 0;
		bullet[i].visible = BLACK;
	}
}

void zw_game_bullet_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ZW_GAME_BULLET_SETUP:
	{
		APP_DBG_SIG("ZW_GAME_BULLET_SETUP\n");
		zw_game_bullet_reset_all();
	}
	break;

	case ZW_GAME_BULLET_RUN:
	{
		APP_DBG_SIG("ZW_GAME_BULLET_RUN\n");
		for (uint8_t i = 0; i < BULLET_NUMBER; i++)
		{
			if (bullet[i].visible != WHITE)
			{
				continue;
			}
			bullet[i].x += BULLET_STEP_AXIS_X;
			if (bullet[i].x >= BULLET_MAX_AXIS_X)
			{
				bullet[i].visible = BLACK;
				bullet[i].x = 0;
			}
		}
	}
	break;

	case ZW_GAME_BULLET_SHOOT:
	{
		APP_DBG_SIG("ZW_GAME_BULLET_SHOOT\n");
		for (uint8_t i = 0; i < BULLET_NUMBER; i++)
		{
			if (bullet[i].visible == WHITE)
			{
				continue;
			}
			bullet[i].visible = WHITE;
			bullet[i].x = gunner.x + BULLET_SPAWN_OFFSET_X;
			bullet[i].y = gunner.y - BULLET_SPAWN_OFFSET_Y;
			gunner.action_image = 2;
			BUZZER_PlaySound(BUZZER_SOUND_CLICK);
			break;
		}
	}
	break;

	case ZW_GAME_BULLET_RESET:
	{
		APP_DBG_SIG("ZW_GAME_BULLET_RESET\n");
		zw_game_bullet_reset_all();
	}
	break;

	default:
		break;
	}
}

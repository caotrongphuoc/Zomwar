#include "zw_game_zombie.h"
#include "app_eeprom.h"

zw_game_zombie_t zombie[ZOMBIE_NUMBER];
uint8_t zw_game_zombie_speed;

void zw_game_zombie_spawn(uint8_t i)
{
	zombie[i].x = (rand() % (ZOMBIE_SPAWN_X_MAX - ZOMBIE_SPAWN_X_MIN + 1)) + ZOMBIE_SPAWN_X_MIN;
	zombie[i].y = (rand() % (ZOMBIE_Y_MAX - ZOMBIE_Y_MIN + 1)) + ZOMBIE_Y_MIN;
	zombie[i].visible = WHITE;
	zombie[i].action_image = rand() % 3 + 1;
	zombie[i].dy = 0;
	zombie[i].zigzag_timer = rand() % (ZOMBIE_ZIGZAG_PERIOD_MAX - ZOMBIE_ZIGZAG_PERIOD_MIN + 1) + ZOMBIE_ZIGZAG_PERIOD_MIN;
	zombie[i].rising = false;
	zombie[i].rise_ticks = 0;
}

void zw_game_zombie_spawn_from_tombstone(uint8_t i, int16_t x, uint8_t y)
{
	zombie[i].x = x;
	zombie[i].y = y;
	zombie[i].visible = WHITE;
	zombie[i].action_image = 1;
	zombie[i].dy = 0;
	zombie[i].zigzag_timer = 0;
	zombie[i].rising = true;
	zombie[i].rise_ticks = ZOMBIE_RISE_TICKS;
}

bool zw_game_zombie_check_hit(uint8_t b, uint8_t z)
{
	return ((int16_t)bullet[b].x + BULLET_SIZE_BITMAP_X > zombie[z].x + ZOMBIE_HITBOX_LEFT_OFFSET) &&
	       ((int16_t)bullet[b].x < zombie[z].x + ZOMBIE_HITBOX_RIGHT_OFFSET) &&
	       ((int16_t)bullet[b].y + BULLET_SIZE_BITMAP_Y > zombie[z].y + ZOMBIE_HITBOX_TOP_OFFSET) &&
	       ((int16_t)bullet[b].y < zombie[z].y + ZOMBIE_HITBOX_BOTTOM_OFFSET);
}

void zw_game_zombie_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ZW_GAME_ZOMBIE_SETUP:
	{
		APP_DBG_SIG("ZW_GAME_ZOMBIE_SETUP\n");
		zw_game_zombie_speed = settingsetup.zombie_speed;
		for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
		{
			zombie[i].visible = BLACK;
		}
		for (uint8_t i = 0; i < ZOMBIE_INIT_NUMBER; i++)
		{
			zw_game_zombie_spawn(i);
		}
	}
	break;

	case ZW_GAME_ZOMBIE_RUN:
	{
		APP_DBG_SIG("ZW_GAME_ZOMBIE_RUN\n");
		uint8_t alive = 0;
		for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
		{
			if (zombie[i].visible != WHITE)
			{
				continue;
			}
			alive++;
			if (zombie[i].rising)
			{
				if (zombie[i].rise_ticks > 0)
				{
					zombie[i].y--;
					zombie[i].rise_ticks--;
				}
				else
				{
					zombie[i].rising = false;
					zombie[i].zigzag_timer = rand() % (ZOMBIE_ZIGZAG_PERIOD_MAX - ZOMBIE_ZIGZAG_PERIOD_MIN + 1) + ZOMBIE_ZIGZAG_PERIOD_MIN;
				}
				zombie[i].action_image++;
				if (zombie[i].action_image > 3)
				{
					zombie[i].action_image = 1;
				}
				continue;
			}
			if (zombie[i].x - zw_game_zombie_speed < -ZOMBIE_MIN_LEFT_OFFSET)
			{
				zombie[i].x = -ZOMBIE_MIN_LEFT_OFFSET;
			}
			else
			{
				zombie[i].x -= zw_game_zombie_speed;
			}
			if (zombie[i].zigzag_timer > 0)
			{
				zombie[i].zigzag_timer--;
			}
			else
			{
				zombie[i].dy = (int8_t)(rand() % 3) - 1;
				zombie[i].zigzag_timer = rand() % (ZOMBIE_ZIGZAG_PERIOD_MAX - ZOMBIE_ZIGZAG_PERIOD_MIN + 1) + ZOMBIE_ZIGZAG_PERIOD_MIN;
			}
			int16_t new_y = (int16_t)zombie[i].y + zombie[i].dy;
			if (new_y < ZOMBIE_Y_MIN)
			{
				new_y = ZOMBIE_Y_MIN;
				zombie[i].dy = 0;
			}
			if (new_y > ZOMBIE_Y_MAX)
			{
				new_y = ZOMBIE_Y_MAX;
				zombie[i].dy = 0;
			}
			zombie[i].y = (uint8_t)new_y;
			zombie[i].action_image++;
			if (zombie[i].action_image > 3)
			{
				zombie[i].action_image = 1;
			}
		}
		for (uint8_t i = 0; i < ZOMBIE_NUMBER && alive < ZOMBIE_INIT_NUMBER; i++)
		{
			if (zombie[i].visible == WHITE)
			{
				continue; // slot in use
			}
			zw_game_zombie_spawn(i);
			alive++;
		}
	}
	break;

	case ZW_GAME_ZOMBIE_DETONATOR:
	{
		APP_DBG_SIG("ZW_GAME_ZOMBIE_DETONATOR\n");
		for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
		{
			if (zombie[i].visible != WHITE)
			{
				continue;
			}
			if (zombie[i].rising)
			{
				continue;
			}
			for (uint8_t j = 0; j < BULLET_NUMBER; j++)
			{
				if (bullet[j].visible != WHITE)
				{
					continue;
				}
				if (!zw_game_zombie_check_hit(j, i))
				{
					continue;
				}
				bullet[j].visible = BLACK;
				bullet[j].x = 0;
				zw_game_bang_spawn(zombie[i].x, zombie[i].y);
				zw_game_score += 10;
				BUZZER_PlaySound(BUZZER_SOUND_BANG);
				zombie[i].visible = BLACK;
				break;
			}
		}
	}
	break;

	case ZW_GAME_ZOMBIE_WAVE_SPAWN:
	{
		APP_DBG_SIG("ZW_GAME_ZOMBIE_WAVE_SPAWN\n");
		if (zw_game_zombie_speed < ZOMBIE_SPEED_MAX)
		{
			zw_game_zombie_speed++;
		}
		uint8_t spawned = 0;
		for (uint8_t i = 0; i < ZOMBIE_NUMBER && spawned < ZOMBIE_WAVE_SPAWN; i++)
		{
			if (zombie[i].visible == WHITE)
			{
				continue;
			}
			zw_game_zombie_spawn(i);
			spawned++;
		}
	}
	break;

	case ZW_GAME_ZOMBIE_RESET:
	{
		APP_DBG_SIG("ZW_GAME_ZOMBIE_RESET\n");
		for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
		{
			zombie[i].visible = BLACK;
		}
	}
	break;

	case ZW_GAME_ZOMBIE_SETUP_MENU:
	{
		APP_DBG_SIG("ZW_GAME_ZOMBIE_SETUP_MENU\n");
		for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
		{
			zombie[i].visible = BLACK;
		}
		zombie[0].x = LCD_WIDTH + 3;
		zombie[0].y = GUNNER_AXIS_Y - 10;
		zombie[0].rising = false;
		zombie[0].visible = WHITE;
		zombie[0].action_image = 1;
	}
	break;

	case ZW_GAME_ZOMBIE_RUN_MENU:
	{
		APP_DBG_SIG("ZW_GAME_ZOMBIE_RUN_MENU\n");
		zombie[0].x -= ZOMBIE_MENU_SPEED;
		zombie[0].visible = WHITE;
		zombie[0].action_image++;
		if (zombie[0].action_image > 3)
		{
			zombie[0].action_image = 1;
		}
		if (zombie[0].x < -ZOMBIE_SIZE_BITMAP_X)
		{
			zombie[0].x = LCD_WIDTH + 3;
			zombie[0].y = GUNNER_AXIS_Y - 10;
		}
		for (uint8_t j = 0; j < BULLET_NUMBER; j++)
		{
			if (bullet[j].visible != WHITE)
			{
				continue;
			}
			if (!zw_game_zombie_check_hit(j, 0))
			{
				continue;
			}
			bullet[j].visible = BLACK;
			bullet[j].x = 0;
			zw_game_bang_spawn(zombie[0].x, zombie[0].y);
			BUZZER_PlaySound(BUZZER_SOUND_BANG);
			zombie[0].x = LCD_WIDTH + 3;
			zombie[0].y = GUNNER_AXIS_Y - 10;
			break;
		}
	}
	break;

	default:
		break;
	}
}

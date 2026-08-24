#include "zw_game_car.h"
#include "app_eeprom.h"

zw_game_car_t car[CAR_LANE_NUMBER];
const uint8_t lane_y[CAR_LANE_NUMBER] = CAR_LANE_Y;

int8_t zw_game_car_find_nearest(uint8_t zy)
{
	int8_t best = -1;
	uint8_t best_dist = CAR_HIT_RANGE_Y + 1;
	for (uint8_t i = 0; i < CAR_LANE_NUMBER; i++)
	{
		if (!car[i].visible || car[i].running)
		{
			continue;
		}
		uint8_t dist = (zy > car[i].y) ? (zy - car[i].y) : (car[i].y - zy);
		if (dist > CAR_HIT_RANGE_Y)
		{
			continue;
		}
		if (dist < best_dist)
		{
			best_dist = dist;
			best = i;
		}
	}
	return best;
}

static void zw_game_car_reset_slot(uint8_t i)
{
	car[i].x = CAR_AXIS_X;
	car[i].y = lane_y[i];
	car[i].lane = i;
	car[i].running = false;
	car[i].action_image = 1;
}

bool zw_game_car_check_hit(uint8_t c, uint8_t z)
{
	int16_t dy = (int16_t)zombie[z].y - (int16_t)car[c].y;
	if (dy < 0)
	{
		dy = -dy;
	}
	if (dy > CAR_HIT_RANGE_Y)
	{
		return false;
	}

	int16_t car_left = car[c].x;
	int16_t car_right = car_left + CAR_SIZE_BITMAP_X;

	int16_t zombie_left = zombie[z].x + ZOMBIE_HITBOX_LEFT_OFFSET;
	int16_t zombie_right = zombie[z].x + ZOMBIE_HITBOX_RIGHT_OFFSET;

	return (car_right > zombie_left) && (car_left < zombie_right);
}

void zw_game_car_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ZW_GAME_CAR_SETUP:
	{
		APP_DBG_SIG("ZW_GAME_CAR_SETUP\n");
		for (uint8_t i = 0; i < CAR_LANE_NUMBER; i++)
		{
			zw_game_car_reset_slot(i);
			car[i].visible = (settingsetup.num_car >> i) & 1;
		}
	}
	break;

	case ZW_GAME_CAR_RUN:
	{
		APP_DBG_SIG("ZW_GAME_CAR_RUN\n");
		for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
		{
			if (zombie[i].visible != WHITE)
			{
				continue;
			}
			if (zombie[i].x <= -(int32_t)ZOMBIE_MIN_LEFT_OFFSET)
			{
				int8_t m = zw_game_car_find_nearest(zombie[i].y);
				if (m >= 0)
				{
					car[m].running = true;
					zw_game_bang_spawn(zombie[i].x, zombie[i].y);
					zw_game_score += 10;
					BUZZER_PlaySound(BUZZER_SOUND_BANG);
					zombie[i].visible = BLACK;
				}
				continue;
			}

			int8_t m = zw_game_car_find_nearest(zombie[i].y);
			if (m >= 0 && zw_game_car_check_hit((uint8_t)m, i))
			{
				car[m].running = true;
			}
		}

		for (uint8_t i = 0; i < CAR_LANE_NUMBER; i++)
		{
			if (!car[i].visible || !car[i].running)
			{
				continue;
			}
			car[i].x += CAR_SPEED;
			car[i].action_image++;
			if (car[i].action_image > 3)
			{
				car[i].action_image = 1;
			}
			if (car[i].x > LCD_WIDTH)
			{
				car[i].visible = false;
				car[i].running = false;
			}
		}
	}
	break;

	case ZW_GAME_CAR_HIT:
	{
		APP_DBG_SIG("ZW_GAME_CAR_HIT\n");
		for (uint8_t i = 0; i < CAR_LANE_NUMBER; i++)
		{
			if (!car[i].visible || !car[i].running)
			{
				continue;
			}
			for (uint8_t j = 0; j < ZOMBIE_NUMBER; j++)
			{
				if (zombie[j].visible != WHITE)
				{
					continue;
				}
				if (!zw_game_car_check_hit(i, j))
				{
					continue;
				}
				zw_game_bang_spawn(zombie[j].x, zombie[j].y);
				zw_game_score += 10;
				BUZZER_PlaySound(BUZZER_SOUND_BANG);
				zombie[j].visible = BLACK;
			}
		}
	}
	break;

	case ZW_GAME_CAR_RESET:
	{
		APP_DBG_SIG("ZW_GAME_CAR_RESET\n");
		for (uint8_t i = 0; i < CAR_LANE_NUMBER; i++)
		{
			zw_game_car_reset_slot(i);
			car[i].visible = false;
		}
	}
	break;

	default:
		break;
	}
}
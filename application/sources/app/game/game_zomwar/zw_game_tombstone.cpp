#include "zw_game_tombstone.h"
#include "app_eeprom.h"

zw_game_tombstone_t tombstone[TOMBSTONE_NUMBER];
static uint8_t tombstone_spawn_timer = 0;

void zw_game_tombstone_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case ZW_GAME_TOMBSTONE_SETUP:
	{
		APP_DBG_SIG("ZW_GAME_TOMBSTONE_SETUP\n");
		tombstone_spawn_timer = TOMBSTONE_SPAWN_INTERVAL;

		for (uint8_t l = 0; l < CAR_LANE_NUMBER; l++)
		{
			tombstone[l].x = (uint8_t)(rand() % (TOMBSTONE_LANE_1_X_MAX - TOMBSTONE_LANE_1_X_MIN + 1) + TOMBSTONE_LANE_1_X_MIN);
			tombstone[l].lane = l;
			tombstone[l].active = (bool)((settingsetup.tombstone_lane_1 >> l) & 1);

			tombstone[l + CAR_LANE_NUMBER].x = (uint8_t)(rand() % (TOMBSTONE_LANE_2_X_MAX - TOMBSTONE_LANE_2_X_MIN + 1) + TOMBSTONE_LANE_2_X_MIN);
			tombstone[l + CAR_LANE_NUMBER].lane = l;
			tombstone[l + CAR_LANE_NUMBER].active = (bool)((settingsetup.tombstone_lane_2 >> l) & 1);
		}
	}
	break;

	case ZW_GAME_TOMBSTONE_SPAWN:
	{
		APP_DBG_SIG("ZW_GAME_TOMBSTONE_SPAWN\n");
		if (tombstone_spawn_timer > 0)
		{
			tombstone_spawn_timer--;
			return;
		}
		tombstone_spawn_timer = TOMBSTONE_SPAWN_INTERVAL;
		uint8_t tidx = (uint8_t)(rand() % TOMBSTONE_NUMBER);
		if (!tombstone[tidx].active)
		{
			return;
		}
		for (uint8_t i = 0; i < ZOMBIE_NUMBER; i++)
		{
			if (zombie[i].visible == WHITE)
			{
				continue;
			}
			int16_t x = tombstone[tidx].x;
			uint8_t y = lane_y[tombstone[tidx].lane] + TOMBSTONE_SIZE_BITMAP_Y;
			zw_game_zombie_spawn_from_tombstone(i, x, y);
			break;
		}
	}
	break;

	case ZW_GAME_TOMBSTONE_RESET:
	{
		APP_DBG_SIG("ZW_GAME_TOMBSTONE_RESET\n");
		tombstone_spawn_timer = 0;

		for (uint8_t i = 0; i < TOMBSTONE_NUMBER; i++)
		{
			tombstone[i].x = 0;
			tombstone[i].lane = 0;
			tombstone[i].active = false;
		}
	}
	break;

	default:
		break;
	}
}
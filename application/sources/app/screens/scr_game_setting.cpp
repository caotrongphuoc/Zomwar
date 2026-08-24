#include "scr_game_setting.h"

/*****************************************************************************/
/* Variable Declaration - Game setting */
/*****************************************************************************/

static uint8_t setting_location_choose;

/*****************************************************************************/
/* View - Game setting */
/*****************************************************************************/

static void view_scr_game_setting();

view_dynamic_t dyn_view_item_game_setting = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_setting};

view_screen_t scr_game_setting = {
    &dyn_view_item_game_setting,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

void view_scr_game_setting()
{
	view_render.setTextSize(1);

	uint8_t sel = setting_location_choose;

	for (uint8_t f = 0; f < 5; f++)
	{
		uint8_t frame_y = SETTING_FRAMES_AXIS_Y_1 + SETTING_FRAMES_STEP * f;
		bool selected = (f == sel);
		uint8_t fg = selected ? BLACK : WHITE;

		if (selected)
		{
			view_render.fillRoundRect(
			    SETTING_FRAMES_AXIS_X, frame_y,
			    SETTING_FRAMES_SIZE_W, SETTING_FRAMES_SIZE_H,
			    SETTING_FRAMES_SIZE_R, WHITE);
		}
		else
		{
			view_render.drawRoundRect(
			    SETTING_FRAMES_AXIS_X, frame_y,
			    SETTING_FRAMES_SIZE_W, SETTING_FRAMES_SIZE_H,
			    SETTING_FRAMES_SIZE_R, WHITE);
		}

		view_render.setTextColor(fg);
		uint8_t text_y = frame_y + 2;

		switch (f)
		{
		case 0:
		{
			uint8_t car_count = 0;
			for (uint8_t i = 0; i < 5; i++)
			{
				if ((settingdata.num_car >> i) & 1)
				{
					car_count++;
				}
			}
			view_render.setCursor(2, text_y);
			view_render.print("Cars");
			view_render.setCursor(104, text_y);
			view_render.print("[");
			view_render.print(car_count);
			view_render.print("]");
		}
		break;
		case 1:
		{
			uint8_t total_t = 0;
			for (uint8_t i = 0; i < 5; i++)
			{
				if ((settingdata.tombstone_lane_1 >> i) & 1)
				{
					total_t++;
				}
				if ((settingdata.tombstone_lane_2 >> i) & 1)
				{
					total_t++;
				}
			}
			view_render.setCursor(2, text_y);
			view_render.print("Tombstones");
			view_render.setCursor(total_t >= 10 ? 98 : 104, text_y);
			view_render.print("[");
			view_render.print(total_t);
			view_render.print("]");
		}
		break;
		case 2:
			view_render.setCursor(2, text_y);
			view_render.print("Zombies speed");
			view_render.setCursor(104, text_y);
			view_render.print("[");
			view_render.print(settingdata.zombie_speed);
			view_render.print("]");
			break;
		case 3:
			view_render.setCursor(2, text_y);
			view_render.print("Sound");
			view_render.drawBitmap(110, text_y, settingdata.silent ? bitmap_speaker_2 : bitmap_speaker_1, 7, 7, fg);
			break;
		case 4:
			view_render.setCursor(45, text_y);
			view_render.print(" EXIT ");
			break;
		}
	}

	view_render.setTextColor(WHITE);
	view_render.update();
}

/*****************************************************************************/
/* Handle - Game setting */
/*****************************************************************************/

void scr_game_setting_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.clear();
		setting_location_choose = SETTING_ITEM_CARS;
		zw_game_setting_read(&settingdata);
	}
	break;

	case AC_DISPLAY_BUTTON_MODE_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED\n");
		switch (setting_location_choose)
		{
		case SETTING_ITEM_CARS:
		{
			SCREEN_TRAN(scr_game_setting_car_handle, &scr_game_setting_car);
			BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		}
		break;

		case SETTING_ITEM_TOMBSTONES:
		{
			SCREEN_TRAN(scr_game_setting_tombstone_handle, &scr_game_setting_tombstone);
			BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		}
		break;

		case SETTING_ITEM_SPEED:
		{
			settingdata.zombie_speed++;
			if (settingdata.zombie_speed > 5)
			{
				settingdata.zombie_speed = 1;
			}
			BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		}
		break;

		case SETTING_ITEM_SOUND:
		{
			settingdata.silent = !settingdata.silent;
			BUZZER_Silent(settingdata.silent ? BUZZER_SILENT_ON : BUZZER_SILENT_OFF);
			BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		}
		break;

		case SETTING_ITEM_EXIT:
		{
			zw_game_setting_write(&settingdata);
			SCREEN_TRAN(scr_game_menu_handle, &scr_game_menu);
			BUZZER_PlaySound(BUZZER_SOUND_STARTUP);
		}
		break;

		default:
			break;
		}
	}
	break;

	case AC_DISPLAY_BUTTON_UP_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED\n");
		if (setting_location_choose == SETTING_ITEM_CARS)
		{
			setting_location_choose = SETTING_ITEM_EXIT;
		}
		else
		{
			setting_location_choose--;
		}
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
	}
	break;

	case AC_DISPLAY_BUTTON_DOWN_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_PRESSED\n");
		if (setting_location_choose == SETTING_ITEM_EXIT)
		{
			setting_location_choose = SETTING_ITEM_CARS;
		}
		else
		{
			setting_location_choose++;
		}
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
	}
	break;

	default:
		break;
	}
}
#include "scr_game_setting_tombstone.h"

/*****************************************************************************/
/* Variable Declaration - Tombstone count per lane */
/*****************************************************************************/

static uint8_t tb_count_location_choose;

/*****************************************************************************/
/* View - Tombstone count per lane */
/*****************************************************************************/

static void view_scr_game_setting_tombstone();

view_dynamic_t dyn_view_game_setting_tombstone = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_setting_tombstone};

view_screen_t scr_game_setting_tombstone = {
    &dyn_view_game_setting_tombstone,
    ITEM_NULL,
    ITEM_NULL,

    .focus_item = 0,
};

static uint8_t get_lane_count(uint8_t i)
{
	uint8_t c = 0;
	if ((settingdata.tombstone_lane_1 >> i) & 1)
	{
		c++;
	}
	if ((settingdata.tombstone_lane_2 >> i) & 1)
	{
		c++;
	}
	return c;
}

static void view_scr_game_setting_tombstone()
{
	view_render.setTextSize(1);

	uint8_t sel = tb_count_location_choose;

	for (uint8_t i = 0; i <= SETTING_TOMBSTONE_NUMBER; i++)
	{
		uint8_t frame_y = SETTING_TOMBSTONE_FRAMES_AXIS_Y_1 + SETTING_TOMBSTONE_FRAMES_STEP * i;
		bool selected = (i == sel);
		uint8_t fg = selected ? BLACK : WHITE;

		if (selected)
		{
			view_render.fillRoundRect(SETTING_TOMBSTONE_FRAMES_AXIS_X, frame_y,
			                          SETTING_TOMBSTONE_FRAMES_SIZE_W, SETTING_TOMBSTONE_FRAMES_SIZE_H,
			                          SETTING_TOMBSTONE_FRAMES_SIZE_R, WHITE);
		}
		else
		{
			view_render.drawRoundRect(SETTING_TOMBSTONE_FRAMES_AXIS_X, frame_y,
			                          SETTING_TOMBSTONE_FRAMES_SIZE_W, SETTING_TOMBSTONE_FRAMES_SIZE_H,
			                          SETTING_TOMBSTONE_FRAMES_SIZE_R, WHITE);
		}

		view_render.setTextColor(fg);

		if (i < SETTING_TOMBSTONE_NUMBER)
		{
			view_render.setCursor(2, frame_y + 1);
			view_render.print("Lane ");
			view_render.print(i + 1);
			view_render.setCursor(104, frame_y + 1);
			view_render.print("[");
			view_render.print(get_lane_count(i));
			view_render.print("]");
		}
		else
		{
			view_render.setCursor(45, frame_y + 1);
			view_render.print(" EXIT ");
		}
	}

	view_render.setTextColor(WHITE);
	view_render.update();
}

/*****************************************************************************/
/* Handle - Tombstone count per lane */
/*****************************************************************************/

void scr_game_setting_tombstone_handle(ak_msg_t* msg)
{
	switch (msg->sig)
	{
	case SCREEN_ENTRY:
	{
		APP_DBG_SIG("SCREEN_ENTRY\n");
		view_render.clear();
		tb_count_location_choose = 0;
	}
	break;

	case AC_DISPLAY_BUTTON_MODE_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_MODE_PRESSED\n");
		if (tb_count_location_choose < SETTING_TOMBSTONE_NUMBER)
		{
			uint8_t idx = tb_count_location_choose;
			/* Cycle tombstone count: 0 -> 1 -> 2 -> 0 */
			uint8_t cur = get_lane_count(idx);
			cur = (cur + 1) % 3;
			/* Clear both bits of this lane */
			settingdata.tombstone_lane_1 &= ~(1 << idx);
			settingdata.tombstone_lane_2 &= ~(1 << idx);
			/* Set bits to new value */
			if (cur >= 1)
			{
				settingdata.tombstone_lane_1 |= (1 << idx);
			}
			if (cur == 2)
			{
				settingdata.tombstone_lane_2 |= (1 << idx);
			}
			BUZZER_PlaySound(BUZZER_SOUND_CLICK);
		}
		else
		{
			/* EXIT: save EEPROM and return to settings */
			zw_game_setting_write(&settingdata);
			SCREEN_TRAN(scr_game_setting_handle, &scr_game_setting);
			BUZZER_PlaySound(BUZZER_SOUND_STARTUP);
		}
	}
	break;

	case AC_DISPLAY_BUTTON_UP_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_UP_PRESSED\n");
		if (tb_count_location_choose == 0)
		{
			tb_count_location_choose = SETTING_TOMBSTONE_ITEM_EXIT;
		}
		else
		{
			tb_count_location_choose--;
		}
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
	}
	break;

	case AC_DISPLAY_BUTTON_DOWN_PRESSED:
	{
		APP_DBG_SIG("AC_DISPLAY_BUTTON_DOWN_PRESSED\n");
		if (tb_count_location_choose == SETTING_TOMBSTONE_ITEM_EXIT)
		{
			tb_count_location_choose = 0;
		}
		else
		{
			tb_count_location_choose++;
		}
		BUZZER_PlaySound(BUZZER_SOUND_CLICK);
	}
	break;

	default:
		break;
	}
}

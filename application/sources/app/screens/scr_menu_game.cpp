#include "scr_menu_game.h"

/*****************************************************************************/
/* View - Menu game */
/*****************************************************************************/
static void view_scr_menu_game();

view_dynamic_t dyn_view_menu_game = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_menu_game
};

view_screen_t scr_menu_game = {
	&dyn_view_menu_game,
	ITEM_NULL,
	ITEM_NULL,

	.focus_item = 0,
};

void view_scr_menu_game() {
    view_render.setTextSize(2);
    view_render.setTextColor(WHITE);
    view_render.setCursor(30,0); 
    view_render.print("ZOMWAR");
    view_render.drawLine(0, LCD_HEIGHT-11, 	LCD_WIDTH, LCD_HEIGHT-11,	WHITE);
    zw_game_gunner_display();
    //view_render.drawBitmap(20, LCD_HEIGHT-21, bitmap_gunner_I, 25, 10, WHITE);
    //view_render.drawBitmap(0, 0, bitmap_menu_tomb, 128, 64, WHITE);
}

void scr_menu_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case SCREEN_ENTRY: 
		APP_DBG_SIG("SCREEN_ENTRY\n");
		break;

	case AC_DISPLAY_BUTTON_MODE_RELEASED: 
		SCREEN_TRAN(scr_zw_game_handle, &scr_zomwar_game);
		break;

	default:
		break;
	}
}

#include "scr_peashooter.h"

static void view_scr_peashooter() {;
    view_render.drawBitmap(0, 0, bitmap_peashooter, 128, 64, WHITE);
}

view_dynamic_t dyn_view_peashooter = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_peashooter
};

view_screen_t scr_peashooter = {
    &dyn_view_peashooter,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

void scr_peashooter_handle(ak_msg_t* msg) {
    switch (msg->sig) {
    case SCREEN_ENTRY:
        break;

    case AC_DISPLAY_BUTON_MODE_RELEASED: {
        SCREEN_TRAN(scr_idle_handle, &scr_idle);
    }
        break;

    default:
        break;
    }
}

#ifndef UI_H
#define UI_H

#include "common.h"

static const int SCREEN_WIDTH = 160;
static const int SCREEN_HEIGHT = 144;

void ui_init();
void ui_handle_events();
void ui_update();

#endif //UI_H

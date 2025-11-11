#ifndef UI_H
#define UI_H

#include "sound_gen.h"

#define UI_WIDTH 500
#define UI_HEIGHT 400
#define UI_SCALE 1.5f

void ui_init(void);
void ui_free(void);
void ui_update(void);
void ui_draw(void);

WaveForm ui_get_waveform(void);

#endif

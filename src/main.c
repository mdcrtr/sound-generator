#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "sound_gen.h"
#include "ui.h"

int main(void) {
  InitWindow(UI_WIDTH * UI_SCALE, UI_HEIGHT * UI_SCALE, "Sound Generator");
  SetTargetFPS(60);

  InitAudioDevice();

  ui_init();

  Camera2D camera = {0};
  camera.zoom = UI_SCALE;

  while (!WindowShouldClose()) {
    ui_update();

    BeginDrawing();
    ClearBackground((Color){50, 50, 50, 255});
    BeginMode2D(camera);
    ui_draw();
    EndMode2D();
    EndDrawing();
  }

  ui_free();
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
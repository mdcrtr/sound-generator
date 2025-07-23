#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "sound_gen.h"
#include "ui.h"

const char *waveform_names[] = {"Sine", "Triangle", "Sawtooth", "Square",
                                "Noise"};

WaveForm waveform = SINE;
int semitone_offset = 0;

int main(void) {
  InitWindow(800, 450, "Sound Generator");
  SetTargetFPS(60);

  InitAudioDevice();

  Sound sound = {0};

  ui_init();

  while (!WindowShouldClose()) {
    ui_update();

    if (IsKeyPressed(KEY_S)) {
      UnloadSound(sound);

      sound = build_sound(
          (SoundParams){.duration = 1,
                        .frequency = get_note_frequency(semitone_offset),
                        .waveform = waveform});

      PlaySound(sound);
    }

    if (IsKeyPressed(KEY_W)) {
      waveform++;
      if (waveform > NOISE) {
        waveform = SINE;
      }
    }

    if (IsKeyPressed(KEY_UP)) {
      semitone_offset++;
      if (semitone_offset > 35) {
        semitone_offset = 35;
      }
    } else if (IsKeyPressed(KEY_DOWN)) {
      semitone_offset--;
      if (semitone_offset < -35) {
        semitone_offset = -35;
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);

    ui_draw();

    const char *text = TextFormat("Waveform: %s\n semitone offset: %d",
                                  waveform_names[waveform], semitone_offset);
    DrawText(text, 80, 40, 32, YELLOW);

    EndDrawing();
  }

  ui_free();
  UnloadSound(sound);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
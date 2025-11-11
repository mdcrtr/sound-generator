#include "ui.h"

#include <assert.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include <string.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui/raygui.h"

#include "sound_gen.h"

#define MAX_NOTES 8

typedef struct NoteGrid {
  Rectangle bounds;
  int low_val;
  int high_val;
  int length;
  int notes[MAX_NOTES];
} NoteGrid;

typedef struct UiState {
  SoundParams sound_params;
  Sound sound;
  NoteGrid waveform_array;
  NoteGrid note_array;
  NoteGrid volume_array;
} UiState;

static UiState state;

NoteGrid note_array_create(Rectangle bounds, int length, int low_val, int init_val, int high_val) {
   NoteGrid self = (NoteGrid) {
    .bounds = bounds,
    .low_val = low_val,
    .high_val = high_val,
    .length = length
  };
  for (int i = 0; i < length; i++) {
    self.notes[i] = init_val;
  }
  return self;
}

void note_array_draw(NoteGrid *self) {
  float resolution = self->high_val - self->low_val;
  float x_step = self->bounds.width / self->length;
  float y_step = self->bounds.height / resolution;
  float x = self->bounds.x;
  float y_max = self->bounds.y + self->bounds.height;

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		Vector2 mouse_pos = GetMousePosition();
		if (CheckCollisionPointRec(mouse_pos, self->bounds)) {
			int i = (int)((mouse_pos.x - self->bounds.x) / x_step);
			int val = (int)((y_max - mouse_pos.y) / self->bounds.height * resolution);
			if (i < self->length) {
				self->notes[i] = val;
			}
		}
	}

  for (int i = 0; i < self->length; i++) {
    int note = self->notes[i];
    float h = note * y_step;
    DrawRectangleRec((Rectangle){x + 1, y_max - h, x_step - 1, h}, GRAY);
    x += x_step;
  }

	DrawRectangleLinesEx(self->bounds, 1, RAYWHITE);
}

static void generate_pressed(void) {
  state.sound_params.length = state.note_array.length;
  for (int i = 0; i < state.note_array.length; i++) {
    state.sound_params.waveforms[i] = (WaveForm)state.waveform_array.notes[i];
    state.sound_params.tones[i] = state.note_array.notes[i];
    state.sound_params.volumes[i] = state.volume_array.notes[i];
  }

  UnloadSound(state.sound);
  state.sound = build_sound(state.sound_params);
  play_sound(state.sound);
}

void ui_init(void) {
  state = (UiState){
    .sound = {0},
    .sound_params = (SoundParams){.length = 0,
                                  .tones = {0}},
    .waveform_array = note_array_create((Rectangle){20, 50, 300, 80}, MAX_NOTES, 0, 0, 5),
    .note_array = note_array_create((Rectangle){20, 140, 300, 350}, MAX_NOTES, 0, 24, 48),
    .volume_array = note_array_create((Rectangle){20, 500, 300, 80}, MAX_NOTES, 0, 5, 8)};
}

void ui_free(void) {
  UnloadSound(state.sound);
}

void ui_process(void) {
  note_array_draw(&state.waveform_array);
  note_array_draw(&state.note_array);
  note_array_draw(&state.volume_array);

  if (GuiButton((Rectangle){20, 10, 160, 24}, "Generate")) {
    generate_pressed();
  }
}


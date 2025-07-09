#ifndef SOUND_GEN_H
#define SOUND_GEN_H

#include <raylib.h>

#define ROOT_NOTE_FREQ 440.0f
#define SAMPLE_RATE 44100

typedef enum { SINE, TRIANGLE, SAWTOOTH, SQUARE, NOISE } WaveForm;

typedef struct {
  float frequency;
  float duration;
  WaveForm waveform;
} SoundParams;

Sound build_sound(SoundParams params);

float get_note_frequency(int offset);

#endif
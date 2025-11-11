#ifndef SOUND_GEN_H
#define SOUND_GEN_H

#include <raylib.h>

#define ROOT_NOTE_FREQ 440.0f
#define SAMPLE_RATE 44100
#define MAX_NOTES 8

typedef enum { SINE, TRIANGLE, SAWTOOTH, SQUARE, NOISE } WaveForm;

typedef struct {
  float frequency;
  float duration;
  float volume;
  WaveForm waveform;
  int length;
  int tones[MAX_NOTES];
} SoundParams;

Sound build_sound(SoundParams params);

void play_sound(Sound sound);

float get_note_frequency(int offset);

const char *get_waveform_name(WaveForm waveform);

#endif
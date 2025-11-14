#include "sound_gen.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define DURATION 0.5f
#define TONE_OFFSET 36

static const char *waveform_names[] = {"Sine", "Triangle", "Sawtooth", "Square",
                                       "Noise"};

static float oscillator_step(WaveForm waveform, float index) {
  switch (waveform) {
    case SINE:
      return sin(2.0f * PI * index);
    case TRIANGLE:
      if (index < 0.5f) {
        return 4.0f * index - 1.0f;
      }
      return 1.0f - 4.0f * (index - 0.5f);
    case SAWTOOTH:
      return index * 2.0f - 1.0f;
    case SQUARE:
      if (index < 0.5f) {
        return -1.0f;
      }
      return 1.0f;
    case NOISE:
      return sin(powf(2.0f * PI * index, 3));
    default:
      return 0;
  }
}

static void generate_samples(short *buffer, int num_samples,
                             SoundParams params) {
  float wave_idx = 0.0f;
  float wave_steps[MAX_NOTES];
  for (int i = 0; i < MAX_NOTES; i++) {
    int tone = params.tones[i];
    wave_steps[i] = get_note_frequency(tone - TONE_OFFSET) / SAMPLE_RATE;
  }

  for (int i = 0; i < num_samples; i++) {
    int note_idx = (int)((float)i / (float)num_samples * (float)params.length);
    float wave_step = wave_steps[note_idx];
    
    WaveForm waveform = params.waveforms[note_idx];
    float volume = (float)params.volumes[note_idx] / MAX_VOLUME;

    float sample = oscillator_step(waveform, wave_idx);
    sample *= volume;
    buffer[i] = (short)(sample * 32767.0f);

    wave_idx += wave_step;
    if (wave_idx > 1.0f) {
      wave_idx = 0.0f;
    }

    if (waveform == NOISE && i % 64 == 0) {
      wave_idx = (float)(rand() % 128) / 128;
    }
  }
}

Sound build_sound(SoundParams params) {
  int num_samples = DURATION * SAMPLE_RATE;
  short *buffer = (short *)MemAlloc(sizeof(short) * num_samples);
  generate_samples(buffer, num_samples, params);
  Wave wave = {
      .frameCount = num_samples,
      .sampleRate = SAMPLE_RATE,
      .sampleSize = 16,
      .channels = 1,
      .data = buffer,
  };
  Sound sound = LoadSoundFromWave(wave);
  MemFree(buffer);
  return sound;
}

void play_sound(Sound sound) {
  PlaySound(sound);
}

float get_note_frequency(int semitone_offset) {
  return ROOT_NOTE_FREQ * powf(2.0f, semitone_offset / 12.0f);
}

const char *get_waveform_name(WaveForm waveform) {
  return waveform_names[waveform];
}

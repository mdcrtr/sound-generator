#include "sound_gen.h"

#include <math.h>
#include <stdlib.h>

static short oscillator_step(WaveForm waveform, float index) {
  switch (waveform) {
    case SINE:
      return 32767.0f * sin(2 * PI * index);
    case TRIANGLE:
      if (index < 0.5f) {
        return 65535.0f * index * 2.0f - 32767.0f;
      }
      return 32767.0f - 65535.0f * (index - 0.5f) * 2.0f;
    case SAWTOOTH:
      return index * 65535.0f - 32767.0f;
    case SQUARE:
      if (index < 0.5f) {
        return -32767.0f;
      }
      return 32767.0f;
    case NOISE:
      return 32767.0f * sin(powf(2 * PI * index, 3));
    default:
      return 0;
  }
}

static void generate_samples(short *buffer, int num_samples,
                             SoundParams params) {
  float volume = 0.8f;
  float wave_idx = 0.0f;
  float envelope_idx = 0.0f;
  float wave_step = params.frequency / SAMPLE_RATE;
  float envelope_step = 2.0f / SAMPLE_RATE;

  for (int i = 0; i < num_samples; i++) {
    short sample = oscillator_step(params.waveform, wave_idx);
    sample *= volume;
    sample *= 1.0f - envelope_idx;
    buffer[i] = sample;

    wave_idx += wave_step;
    if (wave_idx > 1.0f) {
      wave_idx = 0.0f;
    }

    envelope_idx += envelope_step;
    if (envelope_idx > 1.0f) {
      envelope_idx = 0;
    }

    if (params.waveform == NOISE && i % 64 == 0) {
      wave_idx = (float)(rand() % 128) / 128;
    }
  }
}

Sound build_sound(SoundParams params) {
  int num_samples = params.duration * SAMPLE_RATE;
  short *buffer = (short *)MemAlloc(sizeof(short) * num_samples);
  generate_samples(buffer, num_samples, params);
  Wave wave = {
      .frameCount = 20000,
      .sampleRate = SAMPLE_RATE,
      .sampleSize = 16,
      .channels = 1,
      .data = buffer,
  };
  Sound sound = LoadSoundFromWave(wave);
  UnloadWave(wave);
  return sound;
}

float get_note_frequency(int semitone_offset) {
  return ROOT_NOTE_FREQ * powf(2.0f, semitone_offset / 12.0f);
}

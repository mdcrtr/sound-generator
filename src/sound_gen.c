#include "sound_gen.h"

#include <math.h>
#include <stdlib.h>

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
  float volume = params.volume;
  float wave_idx = 0.0f;
  float envelope_idx = 0.0f;
  float wave_step = params.frequency / SAMPLE_RATE;
  float envelope_step = 1.0f / num_samples;

  for (int i = 0; i < num_samples; i++) {
    float sample = oscillator_step(params.waveform, wave_idx);
    sample *= volume;
    sample *= 1.0f - envelope_idx;
    buffer[i] = (short)(sample * 32767.0f);

    wave_idx += wave_step;
    if (wave_idx > 1.0f) {
      wave_idx = 0.0f;
    }

    envelope_idx += envelope_step;
    if (envelope_idx > 1.0f) {
      envelope_idx = 1.0f;
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

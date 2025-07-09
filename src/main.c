#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100
#define SAMPLE_SIZE 16

typedef enum { SINE, TRIANGLE, SAWTOOTH, SQUARE, NOISE } WaveForm;

const char *waveFormNames[] = {"Sine", "Triangle", "Sawtooth", "Square",
                               "Noise"};

const float semitones[] = {261.6f, 277.2f, 293.7f, 311.1f, 329.6f, 349.2f,
                           370.0f, 392.0f, 415.3f, 440.0f, 466.2f, 493.9f};

const float octaves[] = {0.125f, 0.25f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f};

WaveForm waveForm = SINE;
int semitoneIdx = 0;
int octaveIdx = 3;
float volume = 0.8f;
float waveIdx = 0.0f;
float envelopeIdx = 0.0f;

static short OscillatorStep(float waveIdx) {
  switch (waveForm) {
    case SINE:
      return 32767.0f * sin(2 * PI * waveIdx);
    case TRIANGLE:
      if (waveIdx < 0.5f) {
        return 65535.0f * waveIdx * 2.0f - 32767.0f;
      }
      return 32767.0f - 65535.0f * (waveIdx - 0.5f) * 2.0f;
    case SAWTOOTH:
      return waveIdx * 65535.0f - 32767.0f;
    case SQUARE:
      if (waveIdx < 0.5f) {
        return -32767.0f;
      }
      return 32767.0f;
    case NOISE:
      return 32767.0f * sin(powf(2 * PI * waveIdx, 3));
    default:
      return 0;
  }
}

static void GenerateAudioSamples(short *buffer, unsigned int frames) {
  float frequency = semitones[semitoneIdx] * octaves[octaveIdx];
  float waveStep = frequency / SAMPLE_RATE;
  float envelopeStep = 2.0f / SAMPLE_RATE;

  for (int i = 0; i < frames; i++) {
    short sample = OscillatorStep(waveIdx);
    sample *= volume;
    sample *= 1.0f - envelopeIdx;
    buffer[i] = sample;

    waveIdx += waveStep;
    if (waveIdx > 1.0f) {
      waveIdx = 0.0f;
    }

    envelopeIdx += envelopeStep;
    if (envelopeIdx > 1.0f) {
      envelopeIdx = 0;
    }

    if (waveForm == NOISE && i % 64 == 0) {
      waveIdx = (float)(rand() % 128) / 128;
    }
  }
}

static void AudioInputCallback(void *bufferData, unsigned int frames) {
  GenerateAudioSamples((short *)bufferData, frames);
}

int main(void) {
  InitWindow(800, 450, "Sound Generator");
  SetTargetFPS(60);

  InitAudioDevice();
  AudioStream stream = LoadAudioStream(SAMPLE_RATE, SAMPLE_SIZE, 1);

  SetAudioStreamBufferSizeDefault(4096);
  SetAudioStreamCallback(stream, AudioInputCallback);

  PlayAudioStream(stream);

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_S)) {
      if (IsAudioStreamPlaying(stream)) {
        StopAudioStream(stream);
      } else {
        PlayAudioStream(stream);
      }
    }

    if (IsKeyPressed(KEY_W)) {
      waveForm++;
      if (waveForm > NOISE) {
        waveForm = SINE;
      }
    }

    if (IsKeyPressed(KEY_UP)) {
      semitoneIdx++;
      envelopeIdx = 0;
      waveIdx = 0;
      if (semitoneIdx > 11) {
        semitoneIdx = 0;
        octaveIdx++;
      }
    } else if (IsKeyPressed(KEY_DOWN)) {
      semitoneIdx--;
      envelopeIdx = 0;
      waveIdx = 0;
      if (semitoneIdx < 0) {
        semitoneIdx = 11;
        octaveIdx--;
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);
    const char *text =
        TextFormat("Waveform: %s\nOctave: %d\nSemitone: %d",
                   waveFormNames[waveForm], octaveIdx, semitoneIdx);
    DrawText(text, 80, 40, 32, YELLOW);
    EndDrawing();
  }

  UnloadAudioStream(stream);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
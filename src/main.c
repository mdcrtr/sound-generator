#include <raylib.h>
#include <stdio.h>

#define SAMPLE_RATE 44100
#define SAMPLE_SIZE 16

typedef enum { TRIANGLE, SAWTOOTH, SQUARE } WaveForm;

const char *waveFormNames[] = {"Triangle", "Sawtooth", "Square"};

const float semitones[] = {261.6f, 277.2f, 293.7f, 311.1f, 329.6f, 349.2f,
                           370.0f, 392.0f, 415.3f, 440.0f, 466.2f, 493.9f};

const float octaves[] = {0.125f, 0.25f, 0.5f, 1.0f, 2.0f, 4.0f, 8.0f};

WaveForm waveForm = TRIANGLE;
int semitoneIdx = 0;
int octaveIdx = 3;
float volume = 15000.0f;
float waveIdx = 0.0f;
float envelopeIdx = 0.0f;

static short OscillatorStep(float waveIdx, float amplitude) {
  float peakToPeak = amplitude * 2.0f;
  switch (waveForm) {
    case TRIANGLE:
      if (waveIdx < 0.5f) {
        return peakToPeak * waveIdx * 2.0f - amplitude;
      }
      return amplitude - peakToPeak * (waveIdx - 0.5f) * 2.0f;
    case SAWTOOTH:
      return waveIdx * peakToPeak - amplitude;
    case SQUARE:
      if (waveIdx < 0.5) {
        return -amplitude;
      }
      return amplitude;
    default:
      return 0;
  }
}

static void GenerateAudioSamples(short *buffer, unsigned int frames) {
  float frequency = semitones[semitoneIdx] * octaves[octaveIdx];
  float waveStep = frequency / SAMPLE_RATE;
  float amplitude = volume;
  float envelopeStep = 2.0f / SAMPLE_RATE;

  for (int i = 0; i < frames; i++) {
    buffer[i] = OscillatorStep(waveIdx, amplitude);
    buffer[i] *= 1.0f - envelopeIdx;

    waveIdx += waveStep;
    if (waveIdx > 1.0f) {
      waveIdx = 0.0f;
    }

    envelopeIdx += envelopeStep;
    if (envelopeIdx > 1.0f) {
      envelopeIdx = 0;
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
      waveForm = (waveForm + 1) % 3;
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
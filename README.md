# sound-generator
Simple sound generator implemented in C using Raylib.

This is a work in progress. 

So far it can generate sine, triangle, sawtooth, square and noise waves at different frequencies and volumes.

It has a UI for editing sound parameters. The UI has three bar-chart style controls. The top one controls waveform, The middle one controls tone and the bottom one controls volume. Shift clicking sets all notes to the same value.

## Building

This project uses CMake, supporting Linux, MacOS and Windows. The following commands show how to build the project for Linux:

```
mkdir build
cd build
cmake ..
make
cd sound-generator
./sound-generator
```

Alternatively, use the CMake extension for your IDE to build the project for you.

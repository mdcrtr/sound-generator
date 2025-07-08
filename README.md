# sound-generator
Simple sound generator using Raylib and C.

This is a work in progress. 

So far it can generate triangle, sawtooth and square waves at different frequencies. It has a simple fade out envelope.

## Controls

W - Change waveform
S - Mute/Unmute
Up Arrow - Up one semitone
Down Arrow - Down one semitone

## Building

This project provides a Makefile. It assumes you are running a Unix-like environment with mkdir and gcc. For Windows, you can use MinGW and Git Bash, or manually run the commands from the Makefile.

Edit the RAYLIB_ROOT path in the Makefile to point to your Raylib installation. Then use the command ```make``` to build.

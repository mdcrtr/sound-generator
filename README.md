# sound-generator
Simple sound generator implemented in C using Raylib and microui.

This is a work in progress. 

So far it can generate sine, triangle, sawtooth, square and noise waves at different frequencies and volumes.

It has a UI for editing sound parameters. The UI has three bar-chart style controls. The top one controls waveform, The middle one controls tone and the bottom one controls volume.

## Building

This project provides a Makefile. It assumes you are running a Unix-like environment with mkdir and gcc. For Windows, you can use MinGW and Git Bash, or manually run the commands from the Makefile.

Edit the RAYLIB_ROOT path in the Makefile to point to your Raylib installation. Then use the command ```make``` to build.

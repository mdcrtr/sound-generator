# sound-generator
Simple sound generator implemented in C using Raylib and microui.

This is a work in progress. 

So far it can generate sine, triangle, sawtooth, square and noise waves at different frequencies. It has a simple fade out envelope (not yet configurable).

It has a UI for editing sound parameters. The UI scale can be changed by editing UI_SCALE in ui.h.

## Building

This project provides a Makefile. It assumes you are running a Unix-like environment with mkdir and gcc. For Windows, you can use MinGW and Git Bash, or manually run the commands from the Makefile.

Edit the RAYLIB_ROOT path in the Makefile to point to your Raylib installation. Then use the command ```make``` to build.

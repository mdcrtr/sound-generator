RAYLIB_ROOT = ../../tools/raylib/build/raylib
OBJ = obj
OBJS = obj/main.o obj/sound_gen.o obj/microui.o obj/ui.o

ifeq ($(OS),Windows_NT)
	OSLIBS = -lopengl32 -luser32 -lgdi32 -lwinmm
	EXE = main.exe
else ifeq ($(shell uname),Darwin)
	OSLIBS = -framework IOKit -framework Cocoa -framework OpenGL
	EXE = main.out
else
	OSLIBS = -lm
	EXE = main.out
endif

CC = gcc
CFLAGS = -c -O2 -std=c11 -Wall -Werror -I$(RAYLIB_ROOT)/include
LDFLAGS = -L$(RAYLIB_ROOT) -lraylib $(OSLIBS)

all: $(OBJ) $(EXE)

clean:
	rm -f $(OBJ)/*

$(OBJ):
	mkdir -p $(OBJ)

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LDFLAGS) 

obj/main.o: src/main.c
	$(CC) $(CFLAGS) $< -o $@

obj/sound_gen.o: src/sound_gen.c
	$(CC) $(CFLAGS) $< -o $@

obj/ui.o: src/ui.c
	$(CC) $(CFLAGS) $< -o $@

obj/microui.o: src/microui/microui.c
	$(CC) $(CFLAGS) $< -o $@

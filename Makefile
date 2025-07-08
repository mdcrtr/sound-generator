RAYLIB_ROOT = ../../tools/raylib/build/raylib
OBJ = obj
OBJS = obj/main.o

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

$(OBJ):
	mkdir -p $(OBJ)

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LDFLAGS) 

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ)/*
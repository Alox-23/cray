CC = gcc
FLAGS = -Wall -Wextra -O2
TARGET = bin/main
SRC = src/main.c src/input.c src/game.c src/map.c src/renderer.c src/utils.c src/player.c 
LIBS = `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lm 

all:
	$(CC) $(FLAGS) -o $(TARGET) $(SRC) $(LIBS)
	./$(TARGET)

install:
	sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-net-dev

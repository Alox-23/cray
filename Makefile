CC = gcc
FLAGS = -Wall -Wextra -O2
TARGET = main
SRC = main.c input.c game.c map.c renderer.c utils.c player.c 
LIBS = `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lm 

all:
	$(CC) $(FLAGS) -o $(TARGET) $(SRC) $(LIBS)
	./$(TARGET)

CC = gcc
DFLAGS = -Wall -Wextra -O2 -g -fsanitize=address
FLAGS = -Wall -Wextra -O2
TARGET = bin/main
SRC = src/main.c src/input.c src/game.c src/map.c src/renderer.c src/utils.c src/player.c src/texturemanager.c 
LIBS = `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lm 

all:
	$(CC) $(DFLAGS) -o $(TARGET) $(SRC) $(LIBS)
	./$(TARGET)

comp:
	$(CC) $(FLAGS) -o $(TARGET) $(SRC) $(LIBS)
	
run:
	./$(TARGET)

test:
	$(CC) $(DFLAGS) -o bin/test src/texturemanager.c src/test.c $(LIBS)
	./bin/test

prof:
	$(CC) $(FLAGS) -o $(TARGET) $(SRC) $(LIBS)
	valgrind ./bin/test

install:
	sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev libsdl2-net-dev

all:
	gcc -Wall -Wextra -O2 -o main main.c input.c game.c map.c renderer.c `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf
	./main

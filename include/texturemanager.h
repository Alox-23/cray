// texturemanager.h
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXTURES 256
#define ATLAS_WIDTH 4096
#define ATLAS_HEIGHT 4096
#define MAX_TEXTURE_SIZE 256

typedef struct {
    SDL_Texture* texture;
    SDL_Rect regions[MAX_TEXTURES];
    char* names[MAX_TEXTURES];
    int texture_count;
    int current_x, current_y;
    int row_height;
    int texture_width;
    int texture_height;
} TextureManager;

TextureManager* texturemanager_create(SDL_Renderer* renderer, int texture_width, int texture_height);
void texturemanager_destroy(TextureManager* tm);

int texturemanager_add_texture(TextureManager* tm, SDL_Renderer* renderer, const char* filename);
SDL_Texture* texturemanager_get_atlas(TextureManager* tm);
SDL_Rect texturemanager_get_texcoord(TextureManager* tm, int texture_id);
const char* texturemanager_get_name(TextureManager* tm, int texture_id);

void texturemanager_print_info(TextureManager* tm);
int texturemanager_find_texture(TextureManager* tm, const char* name);

#endif

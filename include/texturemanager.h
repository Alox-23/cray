#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
  SDL_Texture** textures;
  int texture_capacity;
  int texture_count;
}TextureManager;

TextureManager* init_TextureManager(int num_textures, SDL_Renderer* renderer);
int add_texture_TextureManager(TextureManager* texture_manager, SDL_Renderer* renderer, const char* texture_path);
void remove_texture_TextureManager(TextureManager* texture_manager, int texutre_id);
void destroy_TextureManager(TextureManager* texture_manager);
SDL_Texture* get_texture_TextureManager(TextureManager* texture_manager, int texture_id);

#endif

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
  SDL_Texture** textures;
  size_t texture_capacity;
  size_t texture_count;
  int texture_width;
  int texture_height;
}TextureManager;

TextureManager* texturemanager_create(size_t num_textures, SDL_Renderer* renderer, int texture_width, int texture_height);
int texturemanager_add_texture(TextureManager* texture_manager, SDL_Renderer* renderer, const char* texture_path);
void texturemanager_remove_texture(TextureManager* texture_manager, size_t texutre_id);
SDL_Texture* texturemanager_get_texture(TextureManager* texture_manager, size_t texture_id);
void texturemanager_destroy(TextureManager* texture_manager);

#endif

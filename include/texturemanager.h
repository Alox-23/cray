#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct{
  int* textures;
  size_t texture_capacity;
  size_t texture_count;
}TextureManager;

TextureManager* init_TextureManager(int num_textures);
int add_texture_TextureManager(TextureManager* texture_manager, int texture);
void remove_texture_TextureManager(TextureManager* texture_manager, int texutre_id);
void destroy_TextureManager(TextureManager* texture_manager);

#endif

#include "../include/texturemanager.h"
#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <stdio.h>

TextureManager* texturemanager_create(size_t num_textures, SDL_Renderer* renderer, int texture_width, int texture_height){
  TextureManager* texture_manager = malloc(sizeof(TextureManager));
  if (!texture_manager){
    printf("failed to dynamicly alocate space for TextureManager\n");
    return NULL;
  }

  texture_manager->texture_capacity = num_textures;
  texture_manager->texture_count = 0;
 
  if (num_textures <= 0 || texture_height <= 0 || texture_width <= 0){
    printf("Invalid parameters to init_TextureManager (has to be > 0)");
    return NULL;
  }

  texture_manager->texture_width = texture_width;
  texture_manager->texture_height = texture_height;
 
  texture_manager->textures = calloc(num_textures+1, sizeof(SDL_Texture*));
  if (!texture_manager->textures){
    printf("failed to dynamicly allocate space for TextureManager->textures\n");
    return NULL;
  }
  
  
  SDL_Surface* surface = IMG_Load("assets/default.png");
  if (!surface){
    printf("Error loading img SDL_Error : %s\n", IMG_GetError());
    return NULL;
  }

  SDL_Texture* original_texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!original_texture){
    printf("Error loading texture, SDL_Error: %s\n", IMG_GetError());
    return NULL;
  }
  
  SDL_FreeSurface(surface);
  SDL_Texture* resized_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_manager->texture_width, texture_manager->texture_height);

  SDL_SetRenderTarget(renderer, resized_texture);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  SDL_Rect dest_rect = {0, 0, texture_manager->texture_width, texture_manager->texture_height};

  SDL_RenderCopy(renderer, original_texture, NULL, &dest_rect);

  SDL_SetRenderTarget(renderer, NULL);

  texture_manager->textures[0] = resized_texture;

  SDL_DestroyTexture(original_texture);

  return texture_manager;
}

int texturemanager_add_texture(TextureManager* texture_manager, SDL_Renderer* renderer, const char* texture_path){
  if (!texture_manager){
    printf("invalid texture_manager\n");
    return 0;
  }

  if (!renderer){
    printf("invalid renderern");
    return 0;
  }
  
  if (texture_manager->texture_count >= texture_manager->texture_capacity){
    printf("too manny textures loaded! texture_manager capacity: %zu/%zu\n", texture_manager->texture_count, texture_manager->texture_capacity);
    return 0;
  }
  
  SDL_Surface* surface = IMG_Load(texture_path);
  if (!surface){
    printf("Error loading img: %s; SDL_Error : %s\n", texture_path, IMG_GetError());
    return 0;
  }

  SDL_Texture* original_texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!original_texture){
    printf("Error loading texture, SDL_Error: %s\n", IMG_GetError());
    return 0;
  }

  SDL_FreeSurface(surface);
  SDL_Texture* resized_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_manager->texture_width, texture_manager->texture_height);

  SDL_SetRenderTarget(renderer, resized_texture);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  SDL_Rect dest_rect = {0, 0, texture_manager->texture_width, texture_manager->texture_height};

  SDL_RenderCopy(renderer, original_texture, NULL, &dest_rect);

  SDL_SetRenderTarget(renderer, NULL);

  texture_manager->texture_count+=1;
  texture_manager->textures[texture_manager->texture_count] = resized_texture;

  SDL_DestroyTexture(original_texture);

  return texture_manager->texture_count;
}

SDL_Texture* texturemanager_get_texture(TextureManager* texture_manager, size_t texture_id){
  if (!texture_manager){
    printf("invalid texture_manager\n");
    return NULL; 
  }

  if (texture_id > texture_manager->texture_capacity){
    return texture_manager->textures[0];
  }

  if (!texture_manager->textures[texture_id]){
    return texture_manager->textures[0];
  }
 
  return texture_manager->textures[texture_id];
}

void texturemanager_remove_texture(TextureManager* texture_manager, size_t texture_id){
  if (!texture_manager){
    printf("invalid texture_manager\n");
    return; 
  }

  if (texture_id > texture_manager->texture_capacity){
    return;
  }

  SDL_DestroyTexture(texture_manager->textures[texture_id]);
  texture_manager->textures[texture_id] = NULL;
}

void texturemanager_destroy(TextureManager *texture_manager){
  if (!texture_manager){
    return;
  }

  for (size_t i = 0; i < texture_manager->texture_count; i++){
    if (texture_manager->textures[i]){
      SDL_DestroyTexture(texture_manager->textures[i]);
    }
  }
  free(texture_manager->textures);
  texture_manager->textures = NULL;
  free(texture_manager);
  texture_manager = NULL;
}

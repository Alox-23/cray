#include "../include/texturemanager.h"
#include <stdio.h>

TextureManager* init_TextureManager(int num_textures){
  TextureManager* texture_manager = malloc(sizeof(TextureManager));
  if (!texture_manager){
    printf("failed to dynamicly alocate space for TextureManager\n");
    return NULL;
  }
  
  texture_manager->textures = malloc(sizeof(int) * (num_textures+1));
  texture_manager->texture_capacity = num_textures;
  texture_manager->texture_count = 0;

  if (!texture_manager->textures){
    printf("failed to dynamicly allocate space for TextureManager->textures\n");
    return NULL;
  }

  texture_manager->textures[0] = 100;

  return texture_manager;
}

void destroy_TextureManager(TextureManager *texture_manager){
  free(texture_manager->textures);
  texture_manager->textures = NULL;
  free(texture_manager);
  texture_manager = NULL;
}

int add_texture_TextureManager(TextureManager* texture_manager, int texture){
  if (texture_manager->texture_count >= texture_manager->texture_capacity){
    printf("too manny textures loaded! texture_manager capacity: %zu/%zu\n", texture_manager->texture_count, texture_manager->texture_capacity);
    return 0;
  }

  texture_manager->textures[texture_manager->texture_count+1] = texture;
  texture_manager->texture_count++;

  return texture_manager->texture_count;
}

int get_texture_TextureManager(TextureManager* texture_manager, int texture_id){
  return texture_manager->textures[texture_id];
}

void remove_texture_TextureManager(TextureManager* texture_manager, int texture_id){
  return; 
}

int main(){
  TextureManager* tm = init_TextureManager(5);

  int id1 = add_texture_TextureManager(tm, 50);
  int id2 = add_texture_TextureManager(tm, 50);
  int id3 = add_texture_TextureManager(tm, 50);
  int id4 = add_texture_TextureManager(tm, 50);
  int id5 = add_texture_TextureManager(tm, 50);
  int id6 = add_texture_TextureManager(tm, 50);

  printf("texture with id: 0 has value of: %i\n", get_texture_TextureManager(tm, 0));
  printf("texture with id: %i has value of: %i\n", id1, get_texture_TextureManager(tm, id1));
  printf("texture with id: %i has value of: %i\n", id2, get_texture_TextureManager(tm, id2));
  printf("texture with id: %i has value of: %i\n", id3, get_texture_TextureManager(tm, id3));
  printf("texture with id: %i has value of: %i\n", id4, get_texture_TextureManager(tm, id4));
  printf("texture with id: %i has value of: %i\n", id5, get_texture_TextureManager(tm, id5));
  printf("texture with id: %i has value of: %i\n", id6, get_texture_TextureManager(tm, id6));

  destroy_TextureManager(tm);  
}

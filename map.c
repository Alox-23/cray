#include "map.h"
#include <stdio.h>

static void add_texture(Map* map, const char *str){
  if (!map){
    printf("Error adding texture value to map");
    return;
  }

  map->textures[map->texture_index] = strdup(str);
  if (map->textures[map->texture_index]){
    map->texture_index++;
  }
}


//impure function that create a contiguous array of size width * height modifies a maps buffer array probably declared in main.c
Map* init_map(size_t width, size_t height, size_t texture_count){
  Map* map = malloc(sizeof(Map));  
  if (map == NULL || width == 0 || height == 0){
    return NULL;
  }
   
  map->buffer = malloc(sizeof(int) * height * width); 
  if (!map->buffer){
    free(map);
    map = NULL;
    return NULL;
  }
  
  map->textures = malloc(sizeof(char*) * texture_count);
  if (!map->textures){
    free(map->buffer);
    map->buffer = NULL;
    free(map);
    map = NULL;
    return NULL;
  }
  
  map->texture_index = 0;
  map->texture_count = texture_count;
  
  add_texture(map, "brick");
  add_texture(map, "wall");
  
  map->height = height;
  map->width = width;

  for (size_t y = 0; y < map->height; y++){
    for (size_t x = 0; x < map->width; x++){
      if (y == 0 || y == map->height-1 || x == 0 || x == map->width-1){
        set_map_value(map, x, y, 1);
      }
      else {
        set_map_value(map, x, y, 0);
      }
    } 
  }
  
  for (size_t y = 0; y < map->height; y++){
    for (size_t x = 0; x < map->width; x++){
      printf("%i ", get_map_value(map, x, y)); 
    }
    printf("\n");
  }
  
  return map;
}

int get_map_value(Map *map, size_t x, size_t y){
  if (map == NULL || x >= map->width || y >= map->height){
    return 0;
  }
  return map->buffer[y * map->width + x];
}

void set_map_value(Map *map, size_t x, size_t y, int value){
  if (map == NULL || x >= map->width || y >= map->height){
    return;
  }
  map->buffer[y * map->width + x] = value;
}

const char* get_texture(Map *map, size_t texture_index){
  if (!map || texture_index >= map->texture_index){
    printf("Failed to fetch texture from map structure"); 
    return NULL;
  }

  return map->textures[texture_index];
}

//free that array that was created in init_ma
void clean_map(Map *map){
  if (map != NULL){
    free(map->textures);
    map->textures = NULL;
    free(map->buffer);
    map->buffer = NULL;
    free(map);
    map = NULL;
    map->height = 0;
    map->width = 0;
  }
}

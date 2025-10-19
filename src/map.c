#include "../include/map.h"
#include <stdio.h>

Map* map_create(size_t depth, size_t width, size_t height){
  Map* map = malloc(sizeof(Map));  
  if (map == NULL || width == 0 || height == 0 || depth == 0){
    return NULL;
  }
   
  map->buffer = malloc(depth * sizeof(int**)); 
  if (!map->buffer){
    return NULL;
  }
  
  map->height = height;
  map->width = width;
  map->depth = depth;

  for (size_t z = 0; z < map->depth; z++){
    map->buffer[z] = malloc(map->width * sizeof(int*));
    for (size_t x = 0; x < map->width; x++){
      map->buffer[z][x] = malloc(map->height * sizeof(int));
    } 
  }
 
  for (size_t z = 0; z < map->depth; z++){
    for (size_t x = 0; x < map->width; x++){
      for (size_t y = 0; y < map->height; y++){
        if (x == 0 || x == map->width-1 || y == 0 || y == map->height-1){
          map_set_value(map, z, x, y, (y/(height/5))+1);
        }
        else if(x % 10 == 0 && y % 10 == 0){
          map_set_value(map, z, x, y, (y/(height/5))+1);
        }
        else {
          map_set_value(map, z, x, y, 0);
        }
      }
    } 
  }
   
  return map;
}

void map_set_value(Map *map, size_t z, size_t x, size_t y, int value){
  if (map == NULL || x >= map->width || y >= map->height){
    return;
  }
  map->buffer[z][x][y] = value;
}

void map_destroy(Map *map){
  if (!map){
    return;
  }

  for (size_t z = 0; z < map->depth; z++){
    for (size_t x = 0; x < map->width; x++){
      free(map->buffer[z][x]);
    }
    free(map->buffer[z]);
  }

  free(map->buffer);
  map->buffer = NULL;
  free(map);
  map = NULL;
}

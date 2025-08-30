#include "../include/texturemanager.h"

TextureManager* texturemanager_create(SDL_Renderer* renderer, int texture_width, int texture_height) {
    TextureManager* tm = malloc(sizeof(TextureManager));
    if (!tm) return NULL;
    
    // Create the master atlas texture
    tm->texture = SDL_CreateTexture(renderer, 
                                   SDL_PIXELFORMAT_RGBA32,
                                   SDL_TEXTUREACCESS_TARGET,
                                   ATLAS_WIDTH, ATLAS_HEIGHT);
    
    if (!tm->texture) {
        printf("Failed to create atlas texture: %s\n", SDL_GetError());
        free(tm);
        return NULL;
    }
    
    // Initialize atlas to transparent
    SDL_SetRenderTarget(renderer, tm->texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    
    // Initialize state
    tm->texture_count = 0;
    tm->current_x = 0;
    tm->current_y = 0;
    tm->row_height = 0;
    tm->texture_width = texture_width;
    tm->texture_height = texture_height;
    
    // Initialize arrays
    for (int i = 0; i < MAX_TEXTURES; i++) {
        tm->names[i] = NULL;
        tm->regions[i] = (SDL_Rect){0, 0, 0, 0};
    }
    
    return tm;
}

void texturemanager_destroy(TextureManager* tm) {
    if (!tm) return;
    
    if (tm->texture) {
        SDL_DestroyTexture(tm->texture);
    }
    
    for (int i = 0; i < MAX_TEXTURES; i++) {
        if (tm->names[i]) {
            free(tm->names[i]);
        }
    }
    
    free(tm);
}

int texturemanager_add_texture(TextureManager* tm, SDL_Renderer* renderer, const char* filename) {
    if (!tm || tm->texture_count >= MAX_TEXTURES) {
        return -1;
    }
    
    // Load the texture
    SDL_Surface* surface = IMG_Load(filename);
    if (!surface) {
        printf("Failed to load texture %s: %s\n", filename, IMG_GetError());
        return -1;
    }
    
    // Convert to RGBA format
    SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);
    
    if (!converted) {
        printf("Failed to convert texture: %s\n", SDL_GetError());
        return -1;
    }
    
    // Check if texture fits in atlas
    if (converted->w > MAX_TEXTURE_SIZE || converted->h > MAX_TEXTURE_SIZE) {
        printf("Texture %s is too large (%dx%d > %dx%d)\n", 
               filename, converted->w, converted->h, MAX_TEXTURE_SIZE, MAX_TEXTURE_SIZE);
        SDL_FreeSurface(converted);
        return -1;
    }
    
    // Check if we need to move to next row
    if (tm->current_x + converted->w > ATLAS_WIDTH) {
        tm->current_x = 0;
        tm->current_y += tm->row_height;
        tm->row_height = 0;
    }
    
    // Check if we have space in atlas
    if (tm->current_y + converted->h > ATLAS_HEIGHT) {
        printf("Texture atlas full! Cannot add %s\n", filename);
        SDL_FreeSurface(converted);
        return -1;
    }
    
    // Update row height
    if (converted->h > tm->row_height) {
        tm->row_height = converted->h;
    }
    
    // Blit texture onto atlas
    SDL_SetRenderTarget(renderer, tm->texture);
    
    SDL_Rect dest_rect = {
        .x = tm->current_x,
        .y = tm->current_y,
        .w = converted->w,
        .h = converted->h
    };
    
    SDL_Texture* temp_tex = SDL_CreateTextureFromSurface(renderer, converted);
    SDL_RenderCopy(renderer, temp_tex, NULL, &dest_rect);
    SDL_DestroyTexture(temp_tex);
    
    SDL_SetRenderTarget(renderer, NULL);
    
    // Store texture info
    int texture_id = tm->texture_count;
    tm->regions[texture_id] = dest_rect;
    tm->names[texture_id] = strdup(filename);
    tm->texture_count++;
    
    // Update current position
    tm->current_x += converted->w;
    
    SDL_FreeSurface(converted);
    
    printf("Added texture %s as ID %d at (%d, %d) %dx%d\n", 
           filename, texture_id, dest_rect.x, dest_rect.y, dest_rect.w, dest_rect.h);
    
    return texture_id;
}

SDL_Texture* texturemanager_get_atlas(TextureManager* tm) {
    return tm ? tm->texture : NULL;
}

SDL_Rect texturemanager_get_texcoord(TextureManager* tm, int texture_id) {
    if (!tm || texture_id < 0 || texture_id >= tm->texture_count) {
        return (SDL_Rect){0, 0, 0, 0};
    }
    return tm->regions[texture_id];
}

const char* texturemanager_get_name(TextureManager* tm, int texture_id) {
    if (!tm || texture_id < 0 || texture_id >= tm->texture_count) {
        return "INVALID";
    }
    return tm->names[texture_id];
}

void texturemanager_print_info(TextureManager* tm) {
    if (!tm) return;
    
    printf("=== Texture Atlas Info ===\n");
    printf("Atlas size: %dx%d\n", ATLAS_WIDTH, ATLAS_HEIGHT);
    printf("Textures loaded: %d/%d\n", tm->texture_count, MAX_TEXTURES);
    printf("Current position: (%d, %d)\n", tm->current_x, tm->current_y);
    printf("Row height: %d\n", tm->row_height);
    
    for (int i = 0; i < tm->texture_count; i++) {
        printf("  ID %d: %s at (%d, %d) %dx%d\n", 
               i, tm->names[i], 
               tm->regions[i].x, tm->regions[i].y,
               tm->regions[i].w, tm->regions[i].h);
    }
}

int texturemanager_find_texture(TextureManager* tm, const char* name) {
    if (!tm || !name) return -1;
    
    for (int i = 0; i < tm->texture_count; i++) {
        if (tm->names[i] && strcmp(tm->names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

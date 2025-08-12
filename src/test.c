#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "../include/texturemanager.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(){
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Texture Manager Test",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        SCREEN_WIDTH, SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
                                             SDL_RENDERER_ACCELERATED | 
                                             SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() == -1) {
      printf("TTF_Init Error: %s\n", TTF_GetError());
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      IMG_Quit();
      SDL_Quit();
      return 1;
    }

    // Load a font
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 16);
    if (!font) {
      printf("TTF_OpenFont Error: %s\n", TTF_GetError());
      // Continue without font, or handle error
    }

    // Create texture manager
    TextureManager* tm = texturemanager_create(5, renderer, 32, 32);
    if (!tm) {
        printf("Failed to create texture manager\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load textures (replace with your actual texture paths)
    const char* texture_paths[] = {
        "assets/texture.jpeg",
        "assets/texture2.jpeg",
        "assets/texture3.png",
        "assets/texture4.jpeg",
        "assets/texture5.jpeg"
    };

    for (int i = 0; i < 5; i++) {
        if (texturemanager_add_texture(tm, renderer, texture_paths[i]) == -1) {
            printf("Failed to load texture %d\n", i);
        }
    }

    // Main game loop
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw all textures in a grid pattern
        int x = 50, y = 50;
        SDL_Color white = {255, 255, 255, 255};
        for (size_t i = 0; i <= tm->texture_count; i++) {
            SDL_Texture* tex = texturemanager_get_texture(tm, i);
            if (tex) {
                // Get texture dimensions
                int w, h;
                SDL_QueryTexture(tex, NULL, NULL, &w, &h);
                
                // Create destination rect
                SDL_Rect dst = {x, y, w, h};
                SDL_RenderCopy(renderer, tex, NULL, &dst);

                char id_text[32];
                snprintf(id_text, sizeof(id_text), "%d", i);

                if (font) {
                  SDL_Surface* text_surface = TTF_RenderText_Solid(font, id_text, white);
                  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
                  SDL_Rect text_rect = {x+1, y +5, text_surface->w, text_surface->h};
                  SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
                  SDL_FreeSurface(text_surface);
                  SDL_DestroyTexture(text_texture);
                }
                
                // Update position for next texture
                x += w + 20;
                if (x > SCREEN_WIDTH - w) {
                    x = 50;
                    y += h + 20;
                }
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }
    // Cleanup
    texturemanager_destroy(tm);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0; 
}

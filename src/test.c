#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    Uint32* buffer;
    int width;
    int height;
    bool running;
    
    // Floor casting parameters
    float posX, posY;
    float dirX, dirY;
    float planeX, planeY;
} SharedData;

// Floor casting thread function
int floorcast_thread(void* data) {
    SharedData* shared = (SharedData*)data;
    
    // Initialize player position and direction
    shared->posX = 2.0f;
    shared->posY = 2.0f;
    shared->dirX = -1.0f;
    shared->dirY = 0.0f;
    shared->planeX = 0.0f;
    shared->planeY = 0.66f;
    
    while (shared->running) {
        // Simple rotation for animation
        static float rotation = 0.0f;
        rotation += 0.00000001f;
        
        float oldDirX = shared->dirX;
        shared->dirX = shared->dirX * cos(rotation) - shared->dirY * sin(rotation);
        shared->dirY = oldDirX * sin(rotation) + shared->dirY * cos(rotation);
        float oldPlaneX = shared->planeX;
        shared->planeX = shared->planeX * cos(rotation) - shared->planeY * sin(rotation);
        shared->planeY = oldPlaneX * sin(rotation) + shared->planeY * cos(rotation);
        
        // Perform floor casting
        for (int y = shared->height / 2; y < shared->height; y++) {
            // Ray direction for floor casting
            float rayDirX0 = shared->dirX - shared->planeX;
            float rayDirY0 = shared->dirY - shared->planeY;
            float rayDirX1 = shared->dirX + shared->planeX;
            float rayDirY1 = shared->dirY + shared->planeY;
            
            int p = y - shared->height / 2;
            float posZ = 0.5f * shared->height;
            float rowDistance = posZ / p;
            
            float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / shared->width;
            float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / shared->width;
            
            float floorX = shared->posX + rowDistance * rayDirX0;
            float floorY = shared->posY + rowDistance * rayDirY0;
            
            for (int x = 0; x < shared->width; x++) {
                int cellX = (int)(floorX);
                int cellY = (int)(floorY);
                
                // Checkerboard pattern
                Uint32 color = ((cellX + cellY) & 1) ? 0xFF404040 : 0xFF808080;
                
                // Draw floor
                shared->buffer[y * shared->width + x] = color;
                // Draw ceiling (darker)
                //shared->buffer[(shared->height - y - 1) * shared->width + x] = color - 0x00202020;
                
                floorX += floorStepX;
                floorY += floorStepY;
            }
        }
    }
    
    return 0;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Window* window = SDL_CreateWindow(
        "Simple Multi-threaded Floorcasting",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );
    
    // Allocate buffer
    Uint32* buffer = (Uint32*)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
    
    // Initialize shared data - much simpler!
    SharedData shared = {
        .buffer = buffer,
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .running = true
    };
    
    // Create floor casting thread
    SDL_Thread* floorcastThread = SDL_CreateThread(floorcast_thread, "FloorcastThread", &shared);
    
    bool quit = false;
    SDL_Event event;
    Uint32 last_time = SDL_GetTicks();
    int frame_count = 0;

    while (!quit) {
        Uint32 current_time = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        // Simply copy the buffer every frame - the thread is continuously updating it
        void* pixels;
        int pitch;
        SDL_LockTexture(texture, NULL, &pixels, &pitch);
        memcpy(pixels, buffer, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
        SDL_UnlockTexture(texture);
        
        // Render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    
        
// Frame rate calculation
        frame_count++;
        if (current_time - last_time >= 1000) {
            printf("FPS: %d\n", frame_count);
            frame_count = 0;
            last_time = current_time;
        }
        

  }
    
    // Cleanup
    shared.running = false;
    SDL_WaitThread(floorcastThread, NULL);
    
    free(buffer);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

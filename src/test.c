#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define NUM_THREADS 4

typedef struct {
    SDL_Texture* texture;
    int start_y;
    int end_y;
    int pitch;
    uint32_t* pixels;
    bool work_ready;
    bool work_done;
    bool shutdown;
    SDL_sem* work_semaphore;
    SDL_sem* done_semaphore;
} ThreadData;

int thread_function(void* data) {
    ThreadData* thread_data = (ThreadData*)data;
    
    while (1) {
        // Wait for work to be available
        SDL_SemWait(thread_data->work_semaphore);
        
        // Check for shutdown
        if (thread_data->shutdown) {
            break;
        }
        
        // Do the work - fill assigned region with white
        for (int y = thread_data->start_y; y < thread_data->end_y; y++) {
            uint32_t* row = (uint32_t*)((uint8_t*)thread_data->pixels + y * thread_data->pitch);
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                row[x] = 0xFFFFFFFF; // White pixel
            }
        }
        
        // Signal that work is done
        SDL_SemPost(thread_data->done_semaphore);
    }
    
    return 0;
}

void fill_texture_optimized(SDL_Texture* texture, ThreadData* thread_data) {
    void* pixels;
    int pitch;
    
    // Lock texture once per frame
    if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
        return;
    }
    
    // Calculate rows per thread
    int rows_per_thread = SCREEN_HEIGHT / NUM_THREADS;
    int remaining_rows = SCREEN_HEIGHT % NUM_THREADS;
    int current_row = 0;
    
    // Setup work for all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].pixels = (uint32_t*)pixels;
        thread_data[i].pitch = pitch;
        thread_data[i].start_y = current_row;
        thread_data[i].end_y = current_row + rows_per_thread + (i < remaining_rows ? 1 : 0);
        
        // Signal thread to start work
        SDL_SemPost(thread_data[i].work_semaphore);
        
        current_row = thread_data[i].end_y;
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        SDL_SemWait(thread_data[i].done_semaphore);
    }
    
    SDL_UnlockTexture(texture);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Optimized Multithreaded Texture",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         SCREEN_WIDTH, SCREEN_HEIGHT,
                                         SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
                                               SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            SCREEN_WIDTH, SCREEN_HEIGHT);
    if (texture == NULL) {
        printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Create threads and synchronization objects
    SDL_Thread* threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    
    printf("Creating %d worker threads...\n", NUM_THREADS);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].texture = texture;
        thread_data[i].shutdown = false;
        thread_data[i].work_semaphore = SDL_CreateSemaphore(0);
        thread_data[i].done_semaphore = SDL_CreateSemaphore(0);
        
        char thread_name[32];
        snprintf(thread_name, sizeof(thread_name), "Worker%d", i);
        threads[i] = SDL_CreateThread(thread_function, thread_name, &thread_data[i]);
        
        if (threads[i] == NULL) {
            printf("Failed to create thread %d: %s\n", i, SDL_GetError());
            // Cleanup...
            SDL_Quit();
            return 1;
        }
    }
    
    bool running = true;
    SDL_Event event;
    int frame_count = 0;
    Uint32 start_time = SDL_GetTicks();
    
    printf("Optimized version running. Close window to exit.\n");
    
    // Even faster: Remove vsync and limit frame rate manually
    SDL_RenderSetVSync(renderer, 0);
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        // Clear and render in one go if possible
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Fill texture using optimized multithreading
        fill_texture_optimized(texture, thread_data);
        
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        frame_count++;
        
        // Display FPS every second
        Uint32 current_time = SDL_GetTicks();
        if (current_time - start_time >= 1000) {
            float fps = frame_count * 1000.0f / (current_time - start_time);
            printf("FPS: %.2f\n", fps);
            frame_count = 0;
            start_time = current_time;
        }
    }
    
    // Shutdown threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].shutdown = true;
        SDL_SemPost(thread_data[i].work_semaphore);
        SDL_WaitThread(threads[i], NULL);
        SDL_DestroySemaphore(thread_data[i].work_semaphore);
        SDL_DestroySemaphore(thread_data[i].done_semaphore);
    }
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

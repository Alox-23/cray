#include "../include/profile.h"


#ifdef PROFILING

static struct {
    const char* name;
    struct timespec start;
} current_profile;

void profile_start(const char* name) {
    current_profile.name = name;
    clock_gettime(CLOCK_MONOTONIC, &current_profile.start);
}

void profile_end() {
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    double elapsed_ms = (end.tv_sec - current_profile.start.tv_sec) * 1000.0 +
                       (end.tv_nsec - current_profile.start.tv_nsec) / 1000000.0;
    
    printf("[PROFILE] %s: %fms\n", current_profile.name, elapsed_ms);
}

#endif

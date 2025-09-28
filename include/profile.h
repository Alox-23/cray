// profiling.h
#ifndef PROFILING_H
#define PROFILING_H

#include <stdio.h>
#include <time.h>
#include <unistd.h> // for usleep

#define PROFILING

#ifdef PROFILING

// Manual timing functions
void profile_start(const char* name);
void profile_end();

// Macros for convenience
#define PROFILE_BEGIN(name) profile_start(name)
#define PROFILE_END() profile_end()

#else
#define PROFILE_BEGIN(name)
#define PROFILE_END() 
#endif

#endif // PROFILING_H

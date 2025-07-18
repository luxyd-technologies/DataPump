#ifndef MEMTRACK_H
#define MEMTRACK_H

#include <stdlib.h>

// Function declarations for tracked memory operations
void* tracked_m_alloc(size_t size, const char* file, int line);
void* tracked_c_alloc(size_t num, size_t size, const char* file, int line, const char* _objectname, short _location);
void* tracked_r_ealloc(void* ptr, size_t size, const char* file, int line);
void tracked_f_ree(void* ptr, const char* file, int line);

// Macro overrides to capture file/line information
#define m_alloc(size)        tracked_m_alloc(size, __FILE__, __LINE__)
#define c_alloc(num, size, _objectname, _location)  tracked_c_alloc(num, size, __FILE__, __LINE__, _objectname, _location)
#define r_ealloc(ptr, size)  tracked_r_ealloc(ptr, size, __FILE__, __LINE__)
#define f_ree(ptr)           tracked_f_ree(ptr, __FILE__, __LINE__)

// Functions to report memory leaks
void report_leaks(const char*);

#endif
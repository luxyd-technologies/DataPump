
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Parameters.h"
#include "MemTrack.h"
#include "common.h"
#include <iostream>

// Structure to track allocation details
typedef struct Allocation {
    void* ptr;
    size_t size;
    char* file;
    int line;
    struct Allocation* next;
} Allocation;

static Allocation* alloc_head = NULL;

// Helper to allocate a new tracker node
static Allocation* create_allocation_node(void* ptr, size_t size, const char* file, int line) {
    Allocation* node = (Allocation*) malloc(sizeof(Allocation));
    if (!node) return NULL;

    node->ptr = ptr;
    node->size = size;
    node->line = line;
    node->file = _strdup(file); // Copy filename string
    node->next = alloc_head;
    return node;
}

// Tracked m_alloc implementation
void* tracked_m_alloc(size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    if (ptr) {
        Allocation* node = create_allocation_node(ptr, size, file, line);
        if (node) {
            alloc_head = node;
        }
        else {
            fprintf(stderr, "Failed to track malloc at %s:%d\n", file, line);
        }
    }
    return ptr;
}

// Tracked c_alloc implementation
void* tracked_c_alloc(size_t num, size_t size, const char* file, int line, const char* _objectname, short _location) {
    void* ptr = calloc(num, size);
    if (ptr) {
        Allocation* node = create_allocation_node(ptr, num * size, file, line);
        if (node) {
            alloc_head = node;
            if (DEBUG_MEMORY_ALLOC) {
                printf("Object:%s allocated (%zu X %zu) = %zu bytes at %p (calloc) at %s:%d\n", _objectname,
                    num, size, num * size, ptr, file, line);
            }
        }
        else {
            fprintf(stderr, "Failed to track calloc at %s:%d\n", file, line);
        }
    }
    return ptr;
}

// Tracked r_ealloc implementation
void* tracked_r_ealloc(void* ptr, size_t size, const char* file, int line) {
    if (ptr) {
        // Remove old allocation tracking
        Allocation** node = &alloc_head;
        while (*node) {
            if ((*node)->ptr == ptr) {
                Allocation* tmp = *node;
                *node = tmp->next;
                free(tmp->file);
                free(tmp);
                break;
            }
            node = &(*node)->next;
        }
    }

    void* new_ptr = realloc(ptr, size);
    if (new_ptr && size > 0) {
        Allocation* node = create_allocation_node(new_ptr, size, file, line);
        if (node) {
            node->next = alloc_head;
            alloc_head = node;
        }
        else {
            fprintf(stderr, "Failed to track realloc at %s:%d\n", file, line);
        }
    }
    return new_ptr;
}

// Tracked f_ree implementation
void tracked_f_ree(void* ptr, const char* file, int line) {
    if (!ptr) return; // free(NULL) is safe

    Allocation** node = &alloc_head;
    while (*node) {
        if ((*node)->ptr == ptr) {
            Allocation* tmp = *node;
            *node = tmp->next;
			printf("Freeing %zu bytes at %p (allocated at %s:%d) \n", tmp->size, ptr, tmp->file, tmp->line);
			// TODO : print out memory data for tmp->size bytes
            //printMemoryContents(ptr, tmp->size);
            free(tmp->file);
            free(tmp);
			//report_leaks(); // Report leaks after freeing
            try {
                free(ptr); // Actual memory deallocation
            }
            catch (const std::runtime_error& e) {
                std::cerr << "Caught exception type:" << e.what() << std::endl;
            }
            catch (...) { // Catch-all handler for any other exceptions
                std::cerr << "Caught unknown error type\n"  << std::endl;
            }
            //report_leaks("Just before track free function exit point"); // Report leaks before actual free

            return;
        }
        node = &(*node)->next;
    }

    // Pointer not found in tracker
    fprintf(stderr, "ERROR: Attempt to free untracked pointer %p at %s:%d\n", ptr, file, line);
    assert(0); // Halt execution in debug mode
}

// Report memory leaks
void report_leaks(const char* _message_string) {
    size_t leak_count = 0;
    size_t total_bytes = 0;
	printf("\n -- Memory Leak Report: %s\n", _message_string);
    for (Allocation* node = alloc_head; node; node = node->next) {
        fprintf(stderr, "LEAK: %zu bytes at %p (allocated at %s:%d)\n",
            node->size, node->ptr, node->file, node->line);
        leak_count++;
        total_bytes += node->size;
    }

    if (leak_count > 0) {
        fprintf(stderr, "\nTOTAL LEAKS: %zu leaks, %zu bytes\n", leak_count, total_bytes);
    }
    else {
        fprintf(stderr, "No memory leaks detected.\n");
    }
}

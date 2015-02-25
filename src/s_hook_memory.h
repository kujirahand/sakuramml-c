/**
 * Hook memory function for debug
 * @file s_hook_memory.h
 */
#ifndef __S_HOOK_MEMORY_H__
#define __S_HOOK_MEMORY_H__

// SWITCH hook(1) or nohook(0)
#define S_HOOK_MEMORY_USED 1 

#include <stdlib.h>

#define s_malloc(size) s_malloc_(size, __FILE__, __LINE__)
#define s_free(ptr)    s_free_(ptr, __FILE__, __LINE__)
#define s_new(t)       (t*)s_malloc_(sizeof(t), __FILE__, __LINE__)
#define s_new_a(t, count) (t*)s_malloc_(sizeof(t) * count, __FILE__, __LINE__)

/** start memory hook function */
void s_hook_memory_open();
/** end memory hook function */
void s_hook_memory_close();

/** malloc and free */
void* s_malloc_(size_t size, const char* file, int line);
void  s_free_(void *ptr, const char* file, int line);

#endif /* __S_HOOK_MEMORY_H_ */


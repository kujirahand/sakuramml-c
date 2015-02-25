/**
 * hook memory
 * @file s_hook_memory.c
 * @auth kujirahand.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "s_hook_memory.h"

#define S_MEM_TABLE_SIZE 1033
// 131
// 256 + 27
// 512 + 9
// 1024 + 9

typedef struct s_mem_t {
  void        *ptr;
  const char  *file;
  int         line; 
  size_t      size;
  int         released;
  struct s_mem_t  *next;
} s_mem_t;

typedef union {
  unsigned int  value;
  void          *ptr;
} s_pint;

s_mem_t* s_mem_table[S_MEM_TABLE_SIZE];

int s_hook_memory_init = 0;

void s_hook_memory_print_freeD(const char *file, int line);
void s_hook_memory_print_leak(s_mem_t *p);
void s_hook_memory_print(const char *msg);
int s_hook_memory_remove(void *ptr, const char *file, int line);



// print error
void s_hook_memory_print(const char *msg) {
  fprintf(stderr, "%s", msg);
}

void s_hook_memory_print_leak(s_mem_t *p) {
  char buf[1024];
  sprintf(
    buf, 
    "[MEMORY_LEAK] %s:%d (size:%lu)\n", 
    p->file, p->line, (unsigned long)p->size);
  s_hook_memory_print(buf);
}

void s_hook_memory_print_freeD(const char *file, int line) {
  char buf[1024];
  sprintf(
    buf, 
    "[MEMORY_FREE_DBL] %s:%d \n", 
    file, line);
  s_hook_memory_print(buf);
}


// start memory hook function
void s_hook_memory_open() {
  int i;
  s_hook_memory_init = 1;
  // init hash table
  for (i = 0; i < S_MEM_TABLE_SIZE; i++) {
    s_mem_t* top = (s_mem_t*)malloc(sizeof(s_mem_t));
    memset(top, sizeof(s_mem_t), 0);
    top->released = 0;
    top->ptr = NULL;
    top->next = NULL;
    s_mem_table[i] = top;
  }
}

// end memory hook function
void s_hook_memory_close() {
  int i;
  s_mem_t *p, *next;
  for (i = 0; i < S_MEM_TABLE_SIZE; i++) {
    p = s_mem_table[i];
    while (p != NULL) {
      next = p->next;
      if (p->ptr != NULL) {
        s_hook_memory_print_leak(p);
        free(p->ptr);
      }
      free(p);
      p = next;
    }
  }
  s_hook_memory_init = 0;
}

// calc hash from pointer
unsigned int s_hook_memory_hash(void *p) {
  s_pint n;
  n.ptr = p;
  return (n.value % S_MEM_TABLE_SIZE);
}


void s_hook_memory_add(void *ptr, size_t size, const char *file, int line) {
  s_mem_t *p, *pNew, *pTop;
  unsigned int hash = s_hook_memory_hash(ptr);

  if (s_hook_memory_init == 0) s_hook_memory_open();

  pNew = (s_mem_t*)malloc(sizeof(s_mem_t));
  pNew->ptr = ptr;
  pNew->size = size;
  pNew->file = file;
  pNew->line = line;
  pNew->next = NULL;

  // [*]->[a]
  // [*]->[b]->[a]
  // [*]->[c]->[b]->[a]
  
  p = s_mem_table[hash];
  pTop = p->next;
  p->next = pNew;
  pNew->next = pTop;
}

int s_hook_memory_remove(void *ptr, const char *file, int line) {
  unsigned int hash;
  s_mem_t *p, *pp = NULL;
  int result = 0;

  if (ptr == NULL) return 0;
  hash = s_hook_memory_hash(ptr);
  p = s_mem_table[hash];

  while (p != NULL) {
    if (p->ptr != ptr) {
      pp = p;
      p = p->next;
      continue;
    }
    // found ptr
    if (pp != NULL) { // has parent
      pp->next = p->next;
    }
    free(p);
    result = 1;
    break;
  }
  
  if (result == 0) {
    s_hook_memory_print_freeD(file, line);
    return 0;
  }
  return 1;
}

void* s_malloc_(size_t size, const char* file, int line) {
  void* ptr = malloc(size);
  memset(ptr, 0, size); // force set zero
  #if S_HOOK_MEMORY_USED == 1
  s_hook_memory_add(ptr, size, file, line);
  #endif
  return ptr;
}

void  s_free_(void *ptr, const char* file, int line) {
  int i;
  if (ptr == NULL) return;
  #if S_HOOK_MEMORY_USED == 1
  i = s_hook_memory_remove(ptr, file, line);
  if (i > 0) free(ptr);
  #else
  free(ptr);
  #endif
}

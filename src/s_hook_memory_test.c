
#include "s_lib.h"
#include "s_hook_memory.h"
#include "s_test.h"
#include "k_test.h"

void s_hook_memory_test() {
  char *p;
  
  s_hook_memory_open();
  p = s_malloc(1024);
  s_free(p);
  s_hook_memory_close();
}


#include "s_hook_memory.h"
#include "k_function.h"

KFunction* KFunction_new() {
  KFunction *f = s_new(KFunction);
  return f;
}

void KFunction_free(KFunction *f) {
  KFuncArg *arg;
  int i;
  // name
  s_free(f->name);
  // args
  if (f->args_len > 0) {
    for (i = 0; i < f->args_len; i++) {
      arg = f->args[i];
      KFuncArg_free(arg);
    }
  }
  if (f->args != NULL) {
      s_free(f->args);
  }
  // token -> link || so no need to free
  // free
  s_free(f);
}

void SValue_freeItem_KFunction(void *value) {
  SValue *v = (SValue*)value;
  KFunction_free(v->value.f);
}


KFuncArg* KFuncArg_new() {
  KFuncArg *arg;
  arg = s_new(KFuncArg);
  return arg;
}

void KFuncArg_free(KFuncArg *a) {
  s_free(a->name);
  SValue_free(a->value);
  s_free(a);
}

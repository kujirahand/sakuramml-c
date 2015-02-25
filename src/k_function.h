/* 
 * File:   k_function.h
 * Author: kujira
 *
 * Created on 2014/12/14, 22:01
 */

#ifndef K_FUNCTION_H
#define	K_FUNCTION_H

#include "s_lib.h"
#include "s_str.h"
#include "s_value.h"
#include "k_token.h"

typedef struct {
  int value_type;
  char *name;
  SValue *value;
} KFuncArg;

KFuncArg* KFuncArg_new();
void KFuncArg_free(KFuncArg *a);

typedef struct {
  char *name;
  KFuncArg **args;
  int args_len;
  KToken *tokens;
} KFunction;

KFunction* KFunction_new();
void KFunction_free(KFunction *f);

void SValue_freeItem_KFunction(void *value);

#endif	/* K_FUNCTION_H */


/**
 * @file s_lib.h
 */

#ifndef __S_LIB_H__
#define __S_LIB_H__

#include "s_types.h"
#include "s_hook_memory.h"
#include "s_str.h"
#include "s_stream.h"
#include "s_list.h"
#include "s_hash.h"
#include "s_value.h"

/** macro */
#define IN_RANGE(min, max, v) (min <= v && v <= max)
#define SET_IN_RANGE(min, max, v) \
  ((min > v) ? min : ((max < v) ? max : v))
#define IS_NUMERIC(v) ('0' <= v && v <= '9')
#define IS_NUMERIC_HEX(v) (('0' <= v && v <= '9')||('a' <= v && v <= 'f')||('A' <= v && v <= 'F'))
#define IS_NUMERIC_OR_FLAG(v) (('0' <= v && v <= '9') || (v == '+') || (v == '-'))
#define IS_ALPHA(v) (('a' <= v && v <= 'z') || ('A' <= v && v <= 'Z'))

/** utility */
s_bool file_exists(const char *filename);
char* file_changeExt(const char *fname, const char *ext);
int s_pow(int v, int count);

/*** auto free function for slib
 */
typedef void (* s_lib_callback_ptr)();
/** add free func */
void s_lib_addFreeFunc(s_lib_callback_ptr callback);
/** free all func */
void s_lib_free();

#define S_ERROR_SYSTEM  0x100
#define S_ERROR_IO      0x101
/** add error */
void s_lib_addError(int code, char *msg, const char* file, int line);
/** get last error */
int s_lib_get_lastError();
/** get all error */
char *s_lib_get_errors();

#endif /* __S_LIB_H__ */



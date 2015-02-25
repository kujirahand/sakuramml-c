/* 
 * File:   s_value.h
 * Author: kujira
 *
 * Created on 2014/10/15, 22:32
 */

#ifndef S_VALUE_H
#define	S_VALUE_H

#include "s_types.h"
#include "s_str.h"

#define SVALUE_TYPE_NULL      0
#define SVALUE_TYPE_INT       1
#define SVALUE_TYPE_STRING    2
#define SVALUE_TYPE_ARRAY     3
#define SVALUE_TYPE_USER_FUNC 4
#define SVALUE_TYPE_SYS_FUNC  5


typedef void (*SValueFreeFunc)(void *value);


/**
 * SArray struct
 */
typedef struct {
  int size;
  int len;
  struct SValue **list;
} SArray;

/**
 * SValue struct
 */
typedef struct SValue {
  int     v_type;       /// value type
  union {
    int     i;          /// int value
    SString *s;         /// string value
    void    *f;         /// function link
    SArray  *a;         /// SArray value
  } value;              /// value
  SValueFreeFunc free_f;/// free func
} SValue;

#define SValue_new() SValue_new_(__FILE__,__LINE__)
#define SValue_newInt(v) SValue_newInt_(v, __FILE__,__LINE__)
#define SValue_newString(str) SValue_newString_(str, __FILE__,__LINE__)
#define SValue_newArray() SValue_newArray_(__FILE__,__LINE__)

SValue *SValue_new_(const char *file, int lineno);
SValue *SValue_newInt_(int v, const char *file, int lineno);
SValue *SValue_newString_(const char *str, const char *file, int lineno);
SValue *SValue_newSysFunc(void *link);
SValue *SValue_newArray_(const char *file, int lineno);
SValue *SValue_clone(SValue *v);

void SValue_free(SValue *value);
s_bool SValue_free_retTrue(void *value);
void SValue_clear(SValue *value);
void SValue_initArray(SValue *value);

void SValue_setString(SValue* value, const char *str);
void SValue_setInt(SValue *value, int i);
SString *SValue_getString(SValue *value);
int SValue_getInt(SValue *value);
int SValue_getIntFree(SValue *value);
s_bool SValue_getBool(SValue *value);
SValue *SValue_getArrayElement(SValue *value, int index);
void SValue_setArrayElement(SValue *value, int index, SValue *v);
SString *SValue_joinArray(SValue *value, const char *delimiter);

s_bool SValue_eq(SValue *a , SValue *b);
void SValue_add(SValue *value, SValue *v2);


#endif	/* S_VALUE_H */


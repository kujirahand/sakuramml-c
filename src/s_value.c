
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s_lib.h"
#include "s_value.h"

SValue *SValue_new_(const char *file, int lineno) {
  SValue *v;
  
  v = (SValue*)s_malloc_(sizeof(SValue), file, lineno);
  v->v_type = SVALUE_TYPE_NULL;
  v->value.i = 0;
  
  return v;
}

SValue *SValue_newInt_(int v, const char *file, int lineno) {
  SValue *value = SValue_new_(file, lineno);
  value->v_type = SVALUE_TYPE_INT;
  value->value.i = v;
  return value;
}

SValue *SValue_newString_(const char *str, const char *file, int lineno) {
  SValue *v = SValue_new_(file, lineno);
  SValue_setString(v, str);
  return v;
}

SValue *SValue_newArray_(const char *file, int lineno) {
  SValue *v;
  
  v = SValue_new_(file, lineno);
  SValue_initArray(v);
  
  return v;
}

void SValue_initArray(SValue *value) {
  SArray *a;
  
  if (value->v_type != SVALUE_TYPE_NULL) {
    SValue_clear(value);
  }
  
  // default new array
  a = s_new(SArray);
  a->len = 0;
  a->size = 16;
  a->list = s_new_a(SValue*, a->size);
  
  value->v_type = SVALUE_TYPE_ARRAY;
  value->value.a = a;
}



SValue *SValue_newSysFunc(void *link) {
  SValue *v = SValue_new();
  v->v_type = SVALUE_TYPE_SYS_FUNC;
  v->value.f = link;
  return v;
}


SValue *SValue_clone(SValue *v) {
  int i;
  SArray *a;
  SValue *nv = SValue_new();
  SValue *tmp;

  if (v == NULL) return nv;
  switch (v->v_type) {
    case SVALUE_TYPE_NULL:
      nv->v_type = SVALUE_TYPE_NULL;
      break;
    case SVALUE_TYPE_INT:
      nv->v_type = v->v_type;
      nv->value.i = v->value.i;
      break;
    case SVALUE_TYPE_STRING:
      nv->v_type = v->v_type;
      nv->value.s = SString_new(v->value.s->ptr);
      break;
    case SVALUE_TYPE_ARRAY:
      a = v->value.a;
      for (i = 0; i < a->len; i++) {
        tmp = SValue_clone(a->list[i]);
        SValue_setArrayElement(nv, i, tmp);
      }
      break;
  }
  nv->free_f = v->free_f;
  return nv;
}


void SValue_clear(SValue *value) {
  SArray *a;
  int i;
  SValue *v;
  if (value == NULL) return;
  
  // clean up object
  switch (value->v_type) {
    case SVALUE_TYPE_STRING:
      SString_free(value->value.s);
      value->value.s = NULL;
      break;
    case SVALUE_TYPE_INT:
      break;
    case SVALUE_TYPE_ARRAY:
      // clear contents
      a = value->value.a;
      for (i = 0; i < a->len; i++) {
        v = a->list[i];
        SValue_free(v);
        a->list[i] = NULL;
      }
      s_free(a->list);
      s_free(a);
      break;
  }
  // free extra data
  if (value->free_f != NULL) {
    value->free_f(value);
  }
  
  value->v_type = SVALUE_TYPE_NULL;
  value->value.i = 0;
}

void SValue_free(SValue *value) {
  SValue_clear(value);
  s_free(value);
}

s_bool SValue_free_retTrue(void *value) {
  SValue_free(value);
  return S_TRUE;
}

void SValue_setString(SValue* value, const char *str) {
  SValue_clear(value);
  value->v_type = SVALUE_TYPE_STRING;
  value->value.s = SString_new(str);
}

void SValue_setInt(SValue *value, int i) {
  SValue_clear(value);
  value->v_type = SVALUE_TYPE_INT;
  value->value.i = i;
}
SString *SValue_getString(SValue *value) {
  char buf[256];
  
  if (value == NULL) {
    return SString_new("NULL");
  }
  // convert to string
  switch (value->v_type) {
    case SVALUE_TYPE_INT:
      sprintf(buf, "%d", value->value.i);
      return SString_new(buf);
    case SVALUE_TYPE_STRING:
      return SString_new(value->value.s->ptr);
    case SVALUE_TYPE_ARRAY:
      return SValue_joinArray(value, ",");
    default:
      return SString_new("NULL");
  }
}

int SValue_getInt(SValue *value) {
  int res = 0;
  
  if (value == NULL) return 0;
  // convert to int
  switch (value->v_type) {
    case SVALUE_TYPE_INT:
      res = value->value.i;
      break;
    case SVALUE_TYPE_STRING:
      res = (int)atol(value->value.s->ptr);
      break;
    case SVALUE_TYPE_ARRAY:
      res = value->value.a->len;
      break;
    default:
      res = 0;
  }
  return res;
}

int SValue_getIntFree(SValue *value) {
  int v = SValue_getInt(value);
  SValue_free(value);
  return v;
}

s_bool SValue_getBool(SValue *value) {
  int i = SValue_getInt(value);
  
  return (i != S_FALSE);
}

s_bool SValue_eq(SValue *a , SValue *b) {
  SString *a_s, *b_s;
  s_bool result;
  if (b->v_type == SVALUE_TYPE_STRING) {
    a_s = SValue_getString(a);
    b_s = SValue_getString(b);
    result = (strcmp(a_s->ptr, b_s->ptr) == 0) ? S_TRUE : S_FALSE;
    SString_free(a_s);
    SString_free(b_s);
  }
  else {
    result = (SValue_getInt(a) == SValue_getInt(b)) ? S_TRUE : S_FALSE;
  }
  return result;
}

void SValue_add(SValue *value, SValue *v2) {
  SString *a, *b;
  if (v2 == NULL) return;
  if (v2->v_type == SVALUE_TYPE_NULL) {
    SValue_clear(value);
    return;
  }
  if (value->v_type == SVALUE_TYPE_STRING || v2->v_type == SVALUE_TYPE_STRING) {
    a = SValue_getString(value);
    b = SValue_getString(v2);
    SString_append(a, b->ptr);
    SValue_setString(value, a->ptr);
    SString_free(b);
    SString_free(a);
    return;
  }
  SValue_setInt(
    value,
    SValue_getInt(value) + SValue_getInt(v2)
  );
  return;
}

SValue *SValue_getArrayElement(SValue *value, int index) {
  SArray *a;
  
  if (value == NULL) return NULL;
  if (value->v_type != SVALUE_TYPE_ARRAY) return NULL;
  
  a = value->value.a;
  if (a->len <= index) return NULL;
  
  return a->list[index];
}

void SValue_setArrayElement(SValue *value, int index, SValue *v) {
  SArray *a;
  SValue **newlist;
  int newsize;
  SValue *tmp;
  
  if (value == NULL) return;
  if (index < 0) return;
  
  // force convert to array
  if (value->v_type != SVALUE_TYPE_ARRAY) {
    tmp = SValue_clone(value);
    SValue_clear(value);
    SValue_initArray(value);
    value->value.a->list[0] = tmp;
    value->value.a->len = 1;
  }
  
  a = value->value.a;
  if (index >= a->size) { // grow
    newsize = index * 2;
    newlist = s_new_a(SValue*, newsize);
    memcpy(newlist, a->list, a->len * sizeof(SValue*));
    s_free(a->list);
    a->list = newlist;
    a->size = newsize;
  }
  // free index object
  if (a->list[index] != NULL) {
    SValue_free(a->list[index]);
  }
  a->list[index] = v;
  if (a->len <= index) {
    a->len = index + 1;
  }
}

SString *SValue_joinArray(SValue *value, const char *delimiter) {
  SString *res, *s;
  SArray *a;
  int i;
  
  if (value == NULL) return SString_new(NULL);
  
  if (value->v_type != SVALUE_TYPE_ARRAY) {
    return SValue_getString(value);
  }
  
  res = SString_new(NULL);
  a = value->value.a;
  for (i = 0; i < a->len; i++) {
    s = SValue_getString(a->list[i]);
    SString_append(res, s->ptr);
    SString_free(s);
    if (i != (a->len - 1)) {
      SString_append(res, delimiter);
    }
  }
  return res;
}




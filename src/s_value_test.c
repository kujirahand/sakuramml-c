/**
 * SValue test
 */

#include <stdio.h>
#include "s_test.h"
#include "s_lib.h"
#include "s_value.h"


void s_value_basic_test() {
  SValue *v;
  SString *r;
  
  v = SValue_new();
  SValue_setInt(v, 300);
  s_test_eq(SValue_getInt(v), 300, "SValue test int");
  SValue_free(v);
  
  v = SValue_newString("abc");
  r = SValue_getString(v);
  s_test_str_eq(r->ptr, "abc", "SValue test string");
  SValue_free(v);
  SString_free(r);

}

void s_value_array_test() {
  SValue *v;
  SArray *a;
  SString *s;
  int i;
  char buf[1024];
  
  // 1
  v = SValue_newArray();
  a = v->value.a;
  SValue_setArrayElement(v, 2, SValue_newString("c"));
  SValue_setArrayElement(v, 1, SValue_newString("b"));
  SValue_setArrayElement(v, 0, SValue_newString("a"));
  s = SValue_joinArray(v, ":");
  s_test_str_eq(s->ptr, "a:b:c", "SValue array test1");
  SValue_free(v);
  SString_free(s);
  
  // 2
  strcpy(buf, "");
  v = SValue_newArray();
  a = v->value.a;
  for (i = 0; i < 100; i++) {
    SValue_setArrayElement(v, i, SValue_newString("a"));
    strcat(buf, "a");
  }
  s = SValue_joinArray(v, "");
  s_test_str_eq(s->ptr, buf, "SValue array test2 grow array");
  SValue_free(v);
  SString_free(s);
}


void s_value_test() {
  s_value_basic_test();
  s_value_array_test();
}


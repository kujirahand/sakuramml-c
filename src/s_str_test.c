/**
 * test for s_str.c
 * @file s_str_test.c
 */

#include <stdio.h>
#include <string.h>

#include "s_str.h"
#include "s_hook_memory.h"
#include "s_test.h"

void s_str_test_char();
void s_string_append_test();
void s_string_test();
void s_stringlist_test();
void s_str_test_readNum();
void s_string_replace();


void s_str_test_char() {
  char *s = s_str_dup("hoge");
  s_test_str_eq("hoge", s, "s_str_new");
  s_free(s);
}

void s_string_append_test() {
  // append
  SString *s;

  s = SString_new("abc");
  SString_appendChar(s, 'd');
  s_test_str_eq(s->ptr, "abcd", "SString_appendChar");

  SString_free(s);
}

void s_string_test() {
  // test1
  SString *str = SString_new("abc");
  SString_append(str, "gogo");
  s_test_str_eq("abcgogo", str->ptr, "append");
  // test2
  // size_t len = SString_load(str, "./test.mml");
  // s_test_true(len > 0, "SString_load size = 0");
  // ---
  // p = strstr(str->ptr, "set");
  // s_test_true((p != NULL), "SString_load load error");
  SString_free(str);
  // append test
  s_string_append_test();
}

void s_stringlist_test() {
  SStringList *list;
  SString *tmp, *t;

  // test1
  list = SStringList_new();
  SStringList_push(list, SString_new("test1"));
  SStringList_push(list, SString_new("te2"));
  SStringList_push(list, SString_new("te3"));
  s_test_eq(list->len, 3, "SStringList_push len");

  tmp = SStringList_join(list, "::");
  s_test_str_eq(tmp->ptr, "test1::te2::te3", "SStringList_join");
  SString_free(tmp);
  SStringList_free(list);

  // test2 split
  tmp = SString_new("1:2:3:4:5:6");
  list = SString_split(tmp, ":", 0);
  s_test_eq(list->len, 6, "SString_split.len");
  t = SStringList_join(list, "-");
  s_test_str_eq(t->ptr, "1-2-3-4-5-6", "SString_split join");
  SString_free(t);
  SString_free(tmp);
  SStringList_free(list);
  
  // test delete(0)
  tmp = SString_new("1:2:3:4:5:6");
  list = SString_split(tmp, ":", 0);
  SStringList_delete(list, 0);
  t = SStringList_join(list, "-");
  s_test_str_eq(t->ptr, "2-3-4-5-6", "SStringList_delete:0");
  SString_free(t);
  SString_free(tmp);
  SStringList_free(list);
  
  // test delete(1)
  tmp = SString_new("1:2:3:4:5:6");
  list = SString_split(tmp, ":", 0);
  SStringList_delete(list, 1);
  t = SStringList_join(list, "-");
  s_test_str_eq(t->ptr, "1-3-4-5-6", "SStringList_delete:1");
  SString_free(t);
  SString_free(tmp);
  SStringList_free(list);

  // test3 delete
  tmp = SString_new("1:2:3:4:5:6");
  list = SString_split(tmp, ":", 2);
  s_test_eq(list->len, 2, "SString_split(s, 2)");
  SString_free(tmp);
  SStringList_free(list);
}

void s_str_test_readNum() {
  char buf[100];
  char *p, *p2;
  int n;
  strcpy(buf, "123");
  p = buf;
  n = s_readInt10n(p, 0, NULL);
  s_test_eq(n, 123, "s_str_readNum:1");
  
  strcpy(buf, "5ab");
  p = buf;
  n = s_readInt10n(p, 0, NULL);
  s_test_eq(n, 5, "s_str_readNum:2");
  
  strcpy(buf, "ab");
  p = buf;
  n = s_readInt10n(p, 0, NULL);
  s_test_eq(n, 0, "s_str_readNum:3");
  
  strcpy(buf, "123abc");
  p = buf;
  n = s_readInt10n(p, 0, &p2);
  s_test_eq(n, 123, "s_str_readNum:4a");
  s_test_eq(*p2, 'a', "s_str_readNum:4b");

  strcpy(buf, "abc");
  p = buf;
  n = s_readInt10n(p, 50, &p2);
  s_test_eq(n, 50, "s_str_readNum:5a");
  s_test_eq(*p2, 'a', "s_str_readNum:5b");
  
  strcpy(buf, "123abc");
  p = buf;
  n = s_readInt(p, 0, &p);
  s_test_eq(n, 123, "s_readInt:1a");
  s_test_eq(*p, 'a', "s_readInt:1b");

  strcpy(buf, "$FF:");
  p = buf;
  n = s_readInt(p, 0, &p);
  s_test_eq(n, 255, "s_readInt:2a");
  s_test_eq(*p, ':', "s_readInt:2b");

  strcpy(buf, "0xFF:");
  p = buf;
  n = s_readInt(p, 0, &p);
  s_test_eq(n, 255, "s_readInt:3a");
  s_test_eq(*p, ':', "s_readInt:3b");
}
 
void s_string_replace() {
  SString *s, *r;
 
  s = SString_new(NULL);
  
  // 1
  SString_set(s, "012345");
  r = SString_replace(s, "23", "**");
  s_test_str_eq(r->ptr, "01**45", "s_string_replace:1");
  SString_free(r);

  // 2
  SString_set(s, "012345");
  r = SString_replace(s, "012", "*");
  s_test_str_eq(r->ptr, "*345", "s_string_replace:2");
  SString_free(r);

  // 3
  SString_set(s, "012345");
  r = SString_replace(s, "345","*");
  s_test_str_eq(r->ptr, "012*", "s_string_replace:3");
  SString_free(r);

  SString_free(s);
  
}

  
/** test for s_str main */
void s_str_test() {
  s_str_test_char();
  s_str_test_readNum();
  s_string_test();
  s_string_replace();
  s_stringlist_test();
}



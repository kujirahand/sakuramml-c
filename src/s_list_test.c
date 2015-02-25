
#include <stdio.h>
#include "s_test.h"
#include "s_list.h"

int s_list_test_sort(const void *a, const void *b) {
  char *ca = *(char**)a;
  char *cb = *(char**)b;
  return strcmp(ca, cb);
}
int s_list_test_sort_len(const void *a, const void *b) {
  char *ca = *(char**)a;
  char *cb = *(char**)b;
  return (int)((int)strlen(ca) - (int)strlen(cb));
}

void s_list_test() {
  SList *list;
  char *p, *c;
  int i;
  //
  // --- push & pop ---
  //
  list = SList_new();
  SList_push(list, "cde");
  SList_push(list, "efg");
  SList_push(list, "gab");
  //
  p = SList_pop(list);
  s_test_str_eq(p, "gab", "SList_push/pop");
  p = SList_pop(list);
  s_test_str_eq(p, "efg", "SList_push/pop");
  //
  SList_free(list);
  //
  // --- grow test ---
  // 
  // size grow test
  list = SList_new();
  for (i = 0; i < 1024; i++) {
    SList_push(list, "aaa");
  }
  SList_sort(list, s_list_test_sort);
  s_test_eq(list->len, 1024, "SList->len:1");
  SList_free(list);
  //
  // --- sort test ---
  //
  list = SList_new();
  SList_push(list, "bat");
  SList_push(list, "cocoa");
  SList_push(list, "errors");
  SList_push(list, "a");
  SList_push(list, "is");
  // sort
  SList_sort(list, s_list_test_sort_len);
  c = SList_get(list, 0);
  s_test_str_eq(c, "a", "SList_sort1");
  c = SList_get(list, 1);
  s_test_str_eq(c, "is", "SList_sort2");
  c = SList_get(list, 2);
  s_test_str_eq(c, "bat", "SList_sort3");
  c = SList_get(list, 3);
  s_test_str_eq(c, "cocoa", "SList_sort4");
  c = SList_get(list, 4);
  s_test_str_eq(c, "errors", "SList_sort5");
  // msort
  SList_msort(list, s_list_test_sort_len);
  c = SList_get(list, 0);
  s_test_str_eq(c, "a", "SList_msort1");
  c = SList_get(list, 1);
  s_test_str_eq(c, "is", "SList_msort2");
  c = SList_get(list, 2);
  s_test_str_eq(c, "bat", "SList_msort3");
  c = SList_get(list, 3);
  s_test_str_eq(c, "cocoa", "SList_msort4");
  c = SList_get(list, 4);
  s_test_str_eq(c, "errors", "SList_msort5");
  //
  SList_free(list);
  //
  //s_test_eq(list->len, 87, "SList->len:2");
  // s_test_str_eq((char*)SList_peekLast(list), "zzz", "SList_sort");
  //
}





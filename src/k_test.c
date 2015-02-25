
#include<stdio.h>
#include "k_test.h"
#include "s_test.h"

void s_lib_test() {
  int i;
  i = SET_IN_RANGE(5,10,20);
  s_test_eq(i, 10, "SET_IN_RANGE:1");
  i = SET_IN_RANGE(5,10,3);
  s_test_eq(i, 5, "SET_IN_RANGE:2");
  i = SET_IN_RANGE(0,10,10);
  s_test_eq(i, 10, "SET_IN_RANGE:3");
}


void k_test_go() {
  printf("<k_test_go>\n");
  s_hook_memory_test();
  s_list_test();
  s_lib_test();
  s_str_test();
  s_value_test();
  k_sutoton_test();
  k_parser_test();
  k_smf_test();
  k_run_test();
  printf("</k_test_go>\n");
}



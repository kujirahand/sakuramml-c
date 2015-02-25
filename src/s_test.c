
#include <stdio.h>
#include <string.h>

#include "s_test.h"

int s_test_ok_count = 0;
int s_test_ng_count = 0;

void s_test_ok_(const char *msg, const char *file, int lineno) {
  if (S_TEST_REPORT_OK) {
    // show ok
    fprintf(
      stderr,
      "[TEST]  ok  : %s - %s:%i\n",
      msg, file, lineno);
  }
  s_test_ok_count++;
}

void s_test_ng_(const char *msg, const char *file, int lineno) {
  // show error
  fprintf(
    stderr,
    "[TEST]* NG *: %s - %s:%i\n",
    msg, file, lineno);
  // count
  s_test_ng_count++;
}

void s_test_str_eq_(const char *a, const char *b, const char *msg, const char *file, int lineno) {
  if (strcmp(a, b) == 0) {
    s_test_ok_(msg, file, lineno);
  } else {
    s_test_ng_(msg, file, lineno);
    printf("\t\t[%s]!=[%s]\n", a, b);
  }
}

void s_test_long_eq_(long a, long b, const char *msg, const char *file, int lineno) {
  if (a == b) {
    s_test_ok_(msg, file, lineno);
  } else {
    s_test_ng_(msg, file, lineno);
    printf("\t\t[%ld]!=[%ld]\n", a, b);
  }
}

int s_test_getNGCount() {
  return s_test_ng_count;
}








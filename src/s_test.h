/**
 * for test
 * @file s_test.h
 */

#ifndef __S_TEST_H__
#define __S_TEST_H__

#include <string.h>

#define S_TEST_REPORT_OK 1

/** for test */
void s_test_ok_(const char *msg, const char *file, int lineno);
void s_test_ng_(const char *msg, const char *file, int lineno);
void s_test_str_eq_(const char *a, const char *b, const char *msg, const char *file, int lineno);
void s_test_long_eq_(long a, long b, const char *msg, const char *file, int lineno);
int s_test_getNGCount();

/** show erro  */
#define s_error(msg) s_test_ng_(msg, __FILE__, __LINE__)

/** test equals strings */
#define s_test_str_eq(a, b, msg) s_test_str_eq_(a, b, msg, __FILE__, __LINE__)

/** test equals value */
#define s_test_eq(a, b, msg) s_test_long_eq_(a, b, msg, __FILE__, __LINE__)

#define s_test_true(a, msg) \
  if (a) { s_test_ok_(msg, __FILE__, __LINE__); } else { s_test_ng_(msg, __FILE__, __LINE__); }

#endif /* __S_TEST_H__ */



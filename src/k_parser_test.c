/**
 * @file k_parser_test.c
 */

#include <stdio.h>
#include "k_parser.h"
#include "k_main.h"
#include "s_test.h"

void k_parser_test_parts() {
  // test for KParser_readNum
  SakuraObj *skr = SakuraObj_new();
  KFile *file = SakuraObj_loadString(skr, "35abc");
  SString *s = KParser_readNum(skr, file);
  s_test_str_eq(s->ptr, "35", "KParser_readNum1");
  SString_free(s);
  SakuraObj_free(skr);
  // test for KParser_readNum2
  skr = SakuraObj_new();
  file = SakuraObj_loadString(skr, "3");
  s = KParser_readNum(skr, file);
  s_test_str_eq(s->ptr, "3", "KParser_readNum2");
  SString_free(s);
  SakuraObj_free(skr);
  // test for KParser_readNum2
  skr = SakuraObj_new();
  file = SakuraObj_loadString(skr, "abc");
  s = KParser_readNum(skr, file);
  s_test_true((s == NULL), "KParser_readNum3");
  SString_free(s);
  SakuraObj_free(skr);
  // comment test
  skr = SakuraObj_new();
  file = SakuraObj_loadString(skr, "/* cde */cde");
  s_test_str_eq(file->pos, "cde", "KFormatter_trimComment:1");
  SakuraObj_free(skr);
  // comment test2
  skr = SakuraObj_new();
  file = SakuraObj_loadString(skr, "cde/* cde\n */fga");
  s_test_str_eq(file->pos, "cde\nfga", "KFormatter_trimComment:2");
  SakuraObj_free(skr);
  // comment test3
  skr = SakuraObj_new();
  file = SakuraObj_loadString(skr, "c\nd//***\ne");
  s_test_str_eq(file->pos, "c\nd\ne", "KFormatter_trimComment:3");
  SakuraObj_free(skr);
  // TODO: rhythm mode
  skr = SakuraObj_new();
  file = SakuraObj_loadString(skr, "RHYTHM{}");
  s = KParser_readNum(skr, file);
  // s_test_true((s == NULL), "KParser_readNum3");
  SString_free(s);
  SakuraObj_free(skr);

}



void k_parser_test() {
  k_parser_test_parts();
}



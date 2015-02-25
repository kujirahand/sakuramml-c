
#include <string.h>
#include <stdio.h>

#include "s_test.h"
#include "k_run.h"
#include "k_parser.h"

void k_run_test_len() {
  char buf[100];
  int len, v;
  
  strcpy(buf, "4");
  len = KRun_calcNoteLen(buf, 96, 96);
  s_test_eq(len, 96, "KRun_calcNoteLen:1");
  
  strcpy(buf, "^");
  len = KRun_calcNoteLen(buf, 96, 96);
  v = 96 * 2;
  s_test_eq(len, v, "KRun_calcNoteLen:2");
  // printf("%d != %d\n", len, 96*2);

  strcpy(buf, "4.");
  len = KRun_calcNoteLen(buf, 96, 96);
  v = 96 + 96 / 2;
  s_test_eq(len, v, "KRun_calcNoteLen:3");  
  // printf("%d != %d\n", len, v);
  
  strcpy(buf, "1");
  len = KRun_calcNoteLen(buf, 96, 96);
  v = 96 * 4;
  s_test_eq(len, v, "KRun_calcNoteLen:4");
  // printf("%d != %d\n", len, v);

  strcpy(buf, "1^2");
  len = KRun_calcNoteLen(buf, 96, 96);
  v = 96*4 + 96*2;
  s_test_eq(len, v, "KRun_calcNoteLen:4");
  // printf("%d != %d\n", len, v);

  strcpy(buf, "4^^^");
  len = KRun_calcNoteLen(buf, 96, 96);
  v = 96*4;
  s_test_eq(len, v, "KRun_calcNoteLen:4");
  // printf("%d != %d\n", len, v);
  //
  strcpy(buf, "%24^%24");
  len = KRun_calcNoteLen(buf, 96, 96);
  s_test_eq(len, 48, "KRun_calcNoteLen:5");
}

void k_run_test_variable() {
  // basic
  SakuraObj *skr = SakuraObj_new();
  KToken *t = KParser_parseString(skr, "INT A=30; PRINT(A);", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "30\n", "Variable and print:1");
  SakuraObj_free(skr);
  
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "STR A={abc}; PRINT(A);", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "abc\n", "Variable and print:2");
  SakuraObj_free(skr);
  
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "INT A=1+2*3; PRINT(A);", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "7\n", "Variable and print:3");
  SakuraObj_free(skr);
  
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "INT A=(1+2)*3; PRINT(A);", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "9\n", "Variable and print:3");
  SakuraObj_free(skr);
  
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "INT A=1;IF(A==1){ PRINT{OK} } ELSE { PRINT{NG} }", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "OK\n", "Variable and if");
  SakuraObj_free(skr);
  
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "INT A=0xFF; INT B=$FF; PRINT(A);PRINT(B);", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "255\n255\n", "Hex Int");
  SakuraObj_free(skr);
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "INT MP; INT C=1; INT P=1; MP = (C - 1) + (P * $10); PRINT(MP)", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "16\n", "Calc Paren");
  SakuraObj_free(skr);
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "STR MP;PRINT(MP)", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "\n", "EMPTY STRING");
  SakuraObj_free(skr);
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "INT I=3;I++;PRINT(I)", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "4\n", "INC");
  SakuraObj_free(skr);
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "INT I=3;I+=4;PRINT(I)", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "7\n", "ADDNUM");
  SakuraObj_free(skr);
}

void k_run_test_kick() {
  SakuraObj *skr;
  int len;
  
  skr = SakuraObj_new();
  len = skr->command_hash->len;
  s_test_true(len > 10, "SakuraObj command not set:1");
  SakuraObj_free(skr);
}

void k_run_test_array() {
  // array function
  SakuraObj *skr;
  KToken *t;
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "Array A = (1,2,3); PRINT(A);", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "1,2,3\n", "Array Script:1");
  SakuraObj_free(skr);
  
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "Array A = (1,2,3); PRINT(SizeOf(A));", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "3\n", "Array Script:2");
  SakuraObj_free(skr);
  
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "Array A = (0,1,2,3); PRINT(A(3));", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "3\n", "Array element");
  SakuraObj_free(skr);
}

void k_run_test_v() {
  SakuraObj *skr;
  KToken *t;
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "v100;PRINT(MML(v))", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "100\n", "test v:1");
  SakuraObj_free(skr);
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "Str N={12345}; PRINT(MID(N,2,3))", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "234\n", "test MID");
  SakuraObj_free(skr);
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "Str N={a}; PRINT(ASC(N))", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "97\n", "test ASC");
  SakuraObj_free(skr);
  //
  skr = SakuraObj_new();
  t = KParser_parseString(skr, "Str N={aabbcc}; N.s({bb},{**}); PRINT(N)", NULL);
  KRun_runTokenList(skr, t);
  s_test_str_eq(skr->print_buf->ptr, "aa**cc\n", "var.s()");
  SakuraObj_free(skr);
}

void k_run_test() {
  k_run_test_len();
  k_run_test_kick();
  k_run_test_variable();
  k_run_test_array();
  k_run_test_v();
}


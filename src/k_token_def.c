// ---
// auto generated from k_token_def.h by k_token_def.php
// ---
#include <stdio.h>
#include "k_token_def.h"
// definet to string
char *KTokenDef_toString(int n) {
  if (n==0x0000) { return "KTOKEN_UNKNOWN"; }
  if (n==0x0001) { return "KTOKEN_TOP"; }
  if (n==0x0002) { return "KTOKEN_NOP"; }
  if (n==0x0003) { return "KTOKEN_SPLITTER"; }
  if (n==0x0004) { return "KTOKEN_MARKER"; }
  if (n==0x0010) { return "KTOKEN_NOTE"; }
  if (n==0x0011) { return "KTOKEN_NOTE_NO"; }
  if (n==0x0012) { return "KTOKEN_REST"; }
  if (n==0x0020) { return "KTOKEN_COMMAND_L"; }
  if (n==0x0021) { return "KTOKEN_COMMAND_O"; }
  if (n==0x0022) { return "KTOKEN_COMMAND_Q"; }
  if (n==0x0023) { return "KTOKEN_COMMAND_V"; }
  if (n==0x0024) { return "KTOKEN_COMMAND_T"; }
  if (n==0x0030) { return "KTOKEN_OCTAVE_UD"; }
  if (n==0x0031) { return "KTOKEN_LOOP_BEGIN"; }
  if (n==0x0032) { return "KTOKEN_LOOP_END"; }
  if (n==0x0033) { return "KTOKEN_LOOP_BREAK"; }
  if (n==0x0034) { return "KTOKEN_HARMONY"; }
  if (n==0x0035) { return "KTOKEN_DIV"; }
  if (n==0x0036) { return "KTOKEN_OCTAVE1UD"; }
  if (n==0x0037) { return "KTOKEN_RHYTHM"; }
  if (n==0x0038) { return "KTOKEN_KEYFLAG"; }
  if (n==0x0039) { return "KTOKEN_TIME_SIGNATURE"; }
  if (n==0x003A) { return "KTOKEN_TRACK_SUB"; }
  if (n==0x003B) { return "KTOKEN_V_ADD"; }
  if (n==0x0050) { return "KTOKEN_V_ON_NOTE"; }
  if (n==0x0051) { return "KTOKEN_V_ON_TIME"; }
  if (n==0x0052) { return "KTOKEN_V_ON_CYCLE"; }
  if (n==0x0060) { return "KTOKEN_T_ON_NOTE"; }
  if (n==0x0061) { return "KTOKEN_T_ON_TIME"; }
  if (n==0x0062) { return "KTOKEN_T_ON_CYCLE"; }
  if (n==0x0100) { return "KTOKEN_TRACK"; }
  if (n==0x0101) { return "KTOKEN_CHANNEL"; }
  if (n==0x0102) { return "KTOKEN_PROGRAM"; }
  if (n==0x0103) { return "KTOKEN_CC"; }
  if (n==0x0104) { return "KTOKEN_TIME"; }
  if (n==0x0105) { return "KTOKEN_KEY"; }
  if (n==0x0106) { return "KTOKEN_TRACK_KEY"; }
  if (n==0x0107) { return "KTOKEN_MEASURE_SHIFT"; }
  if (n==0x0108) { return "KTOKEN_TEMPO"; }
  if (n==0x0109) { return "KTOKEN_METATEXT"; }
  if (n==0x010A) { return "KTOKEN_CH_PREFIX"; }
  if (n==0x010B) { return "KTOKEN_PORT"; }
  if (n==0x0120) { return "KTOKEN_SYS_EX"; }
  if (n==0x0121) { return "KTOKEN_RPN"; }
  if (n==0x0122) { return "KTOKEN_NRPN"; }
  if (n==0x0123) { return "KTOKEN_PITCH_BEND"; }
  if (n==0x0124) { return "KTOKEN_TRACK_SYNC"; }
  if (n==0x0125) { return "KTOKEN_CC_UPPER"; }
  if (n==0x1000) { return "KTOKEN_VALUE"; }
  if (n==0x1001) { return "KTOKEN_NUMBER"; }
  if (n==0x1002) { return "KTOKEN_STRING"; }
  if (n==0x1003) { return "KTOKEN_VARIABLE"; }
  if (n==0x1004) { return "KTOKEN_ARRAY_ELEMENT"; }
  if (n==0x1010) { return "KTOKEN_DEF_INT"; }
  if (n==0x1011) { return "KTOKEN_DEF_STR"; }
  if (n==0x1012) { return "KTOKEN_DEF_ARRAY"; }
  if (n==0x1100) { return "KTOKEN_PRINT"; }
  if (n==0x1101) { return "KTOKEN_ADD"; }
  if (n==0x1102) { return "KTOKEN_SUB"; }
  if (n==0x1103) { return "KTOKEN_MUL"; }
  if (n==0x1104) { return "KTOKEN_CALC_DIV"; }
  if (n==0x1105) { return "KTOKEN_MOD"; }
  if (n==0x1106) { return "KTOKEN_GT"; }
  if (n==0x1107) { return "KTOKEN_GTEQ"; }
  if (n==0x1108) { return "KTOKEN_LT"; }
  if (n==0x1109) { return "KTOKEN_LTEQ"; }
  if (n==0x110A) { return "KTOKEN_EQ"; }
  if (n==0x110B) { return "KTOKEN_NOT_EQ"; }
  if (n==0x110C) { return "KTOKEN_AND"; }
  if (n==0x110D) { return "KTOKEN_OR"; }
  if (n==0x110E) { return "KTOKEN_INC"; }
  if (n==0x110F) { return "KTOKEN_DEC"; }
  if (n==0x2000) { return "KTOKEN_INCLUDE"; }
  if (n==0x2100) { return "KTOKEN_IF"; }
  if (n==0x2101) { return "KTOKEN_WHILE"; }
  if (n==0x2102) { return "KTOKEN_FOR"; }
  if (n==0x2103) { return "KTOKEN_DO_VARIABLE"; }
  if (n==0x2104) { return "KTOKEN_DEF_FUNCTION"; }
  if (n==0x2105) { return "KTOKEN_CALL_USER_FUNC"; }
  if (n==0x2106) { return "KTOKEN_RETURN"; }
  if (n==0x2107) { return "KTOKEN_CALL_SYS_FUNC"; }
  if (n==0x2108) { return "KTOKEN_SHARP_MACRO_DEFINE"; }
  if (n==0x2109) { return "KTOKEN_SHARP_MACRO_EXEC"; }
  if (n==0x210A) { return "KTOKEN_VAR_REPLACE"; }
  if (n==0x2110) { return "KTOKEN_GOTO"; }
  if (n==0x2111) { return "KTOKEN_GOTO_IF_TRUE"; }
  if (n==0x2112) { return "KTOKEN_GOTO_IF_FALSE"; }
  if (n==0x2200) { return "KTOKEN_READ_SYS_VALUE"; }
  if (n==0x2300) { return "KTOKEN_PLAY_FROM"; }
  // UNKNOWN
  return NULL;
}

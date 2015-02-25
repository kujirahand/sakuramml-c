/* 
 * File:   k_token_def.h
 * Author: kujira
 *
 * Created on 2015/01/05, 7:55
 */

#ifndef K_TOKEN_DEF_H
#define	K_TOKEN_DEF_H

// TODO: @Token Type Number Define

///<TOKEN_DEFINE>
#define KTOKEN_UNKNOWN    0x0000
#define KTOKEN_TOP        0x0001
#define KTOKEN_NOP        0x0002
#define KTOKEN_SPLITTER   0x0003
#define KTOKEN_MARKER     0x0004

#define KTOKEN_NOTE       0x0010
#define KTOKEN_NOTE_NO    0x0011
#define KTOKEN_REST       0x0012
#define KTOKEN_COMMAND_L  0x0020
#define KTOKEN_COMMAND_O  0x0021
#define KTOKEN_COMMAND_Q  0x0022
#define KTOKEN_COMMAND_V  0x0023
#define KTOKEN_COMMAND_T  0x0024
#define KTOKEN_OCTAVE_UD  0x0030
#define KTOKEN_LOOP_BEGIN 0x0031
#define KTOKEN_LOOP_END   0x0032
#define KTOKEN_LOOP_BREAK 0x0033
#define KTOKEN_HARMONY    0x0034
#define KTOKEN_DIV        0x0035
#define KTOKEN_OCTAVE1UD  0x0036
#define KTOKEN_RHYTHM     0x0037
#define KTOKEN_KEYFLAG    0x0038
#define KTOKEN_TIME_SIGNATURE 0x0039
#define KTOKEN_TRACK_SUB  0x003A
#define KTOKEN_V_ADD      0x003B

#define KTOKEN_V_ON_NOTE  0x0050
#define KTOKEN_V_ON_TIME  0x0051
#define KTOKEN_V_ON_CYCLE 0x0052
#define KTOKEN_T_ON_NOTE  0x0060
#define KTOKEN_T_ON_TIME  0x0061
#define KTOKEN_T_ON_CYCLE 0x0062

#define KTOKEN_TRACK      0x0100
#define KTOKEN_CHANNEL    0x0101
#define KTOKEN_PROGRAM    0x0102
#define KTOKEN_CC         0x0103
#define KTOKEN_TIME       0x0104
#define KTOKEN_KEY        0x0105
#define KTOKEN_TRACK_KEY  0x0106
#define KTOKEN_MEASURE_SHIFT 0x0107
#define KTOKEN_TEMPO      0x0108
#define KTOKEN_METATEXT   0x0109
#define KTOKEN_CH_PREFIX  0x010A
#define KTOKEN_PORT       0x010B
#define KTOKEN_SYS_EX     0x0120
#define KTOKEN_RPN        0x0121
#define KTOKEN_NRPN       0x0122
#define KTOKEN_PITCH_BEND 0x0123
#define KTOKEN_TRACK_SYNC 0x0124
#define KTOKEN_CC_UPPER   0x0125

#define KTOKEN_VALUE      0x1000
#define KTOKEN_NUMBER     0x1001
#define KTOKEN_STRING     0x1002
#define KTOKEN_VARIABLE   0x1003
#define KTOKEN_ARRAY_ELEMENT 0x1004

#define KTOKEN_DEF_INT    0x1010
#define KTOKEN_DEF_STR    0x1011
#define KTOKEN_DEF_ARRAY  0x1012

#define KTOKEN_PRINT      0x1100

// Calc Operator
#define KTOKEN_ADD        0x1101
#define KTOKEN_SUB        0x1102
#define KTOKEN_MUL        0x1103
#define KTOKEN_CALC_DIV   0x1104
#define KTOKEN_MOD        0x1105
#define KTOKEN_GT         0x1106
#define KTOKEN_GTEQ       0x1107
#define KTOKEN_LT         0x1108
#define KTOKEN_LTEQ       0x1109
#define KTOKEN_EQ         0x110A
#define KTOKEN_NOT_EQ     0x110B
#define KTOKEN_AND        0x110C
#define KTOKEN_OR         0x110D
#define KTOKEN_INC        0x110E
#define KTOKEN_DEC        0x110F

#define KTOKEN_INCLUDE    0x2000

#define KTOKEN_IF             0x2100
#define KTOKEN_WHILE          0x2101
#define KTOKEN_FOR            0x2102
#define KTOKEN_DO_VARIABLE    0x2103
#define KTOKEN_DEF_FUNCTION   0x2104
#define KTOKEN_CALL_USER_FUNC  0x2105
#define KTOKEN_RETURN         0x2106
#define KTOKEN_CALL_SYS_FUNC  0x2107
#define KTOKEN_SHARP_MACRO_DEFINE 0x2108
#define KTOKEN_SHARP_MACRO_EXEC   0x2109
#define KTOKEN_VAR_REPLACE    0x210A

#define KTOKEN_GOTO           0x2110
#define KTOKEN_GOTO_IF_TRUE   0x2111
#define KTOKEN_GOTO_IF_FALSE  0x2112

#define KTOKEN_READ_SYS_VALUE 0x2200

#define KTOKEN_PLAY_FROM      0x2300
///</TOKEN_DEFINE>

// Macro
#define KToken_isCalcOp(n)    (0x1101 <= n && n <= 0x11FF)

// tostr
char *KTokenDef_toString(int n);

#endif	/* K_TOKEN_DEF_H */


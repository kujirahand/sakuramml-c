/**
 * Parser for MML
 * @file k_parser.h
 * @author kujirahand.com
 */

#ifndef __k_parser_h__
#define __k_parser_h__

#include "s_hash.h"
#include "k_token.h"
#include "k_main.h"
#include "k_main.h"
#include "k_function.h"

typedef s_bool (*KParserCommand)(SakuraObj *skr, KFile *file);

// parse function
s_bool KParser_parse(SakuraObj *skr, KFile *file);
KToken *KParser_parseString(SakuraObj *skr, const char* str, char *hint_pos);

// utility function
SString *KParser_readLen(SakuraObj *skr, KFile *file);
SString *KParser_readNum(SakuraObj *skr, KFile *file); 
SString *KParser_readString(SakuraObj *skr, KFile *file);
SString *KParser_readParen(SakuraObj *skr, KFile *file);
SString *KParser_readNestRange(SakuraObj *skr, KFile *file, char c1, char c2);
SString *KParser_readVarName(SakuraObj *skr, KFile *file);
char *KParser_readVarNameStr(SakuraObj *skr, KFile *file);
KToken *KParser_readValue(SakuraObj *skr, KFile *file);
KToken *KParser_readValueList(SakuraObj *skr, KFile *file);
KToken *KParser_readValue_checkCalc(SakuraObj *skr, KFile *file, KToken *t);
s_bool KParser_readCommandOption(SakuraObj *skr, KFile *file, int ttype, int *num);

// read command
s_bool KParser_readLowerCommand(SakuraObj *skr, KFile *file);
s_bool KParser_readUpperCommand(SakuraObj *skr, KFile *file);
s_bool KParser_readFlagCommand(SakuraObj *skr, KFile *file);

// append command
void KParser_appendCommand(SHash *hash);
void KParser_freeCommandHash(SHash *hash);
void KParser_appendReaderCommand(SHash *hash);
void KParser_appendSystemVairables(SHash *hash);

// command
s_bool KParser_readNoteOn(SakuraObj *skr, KFile *file);
s_bool KParser_readNoteOnNo(SakuraObj *skr, KFile *file);
s_bool KParser_readRest(SakuraObj *skr, KFile *file);
s_bool KParser_readCommandL(SakuraObj *skr, KFile *file);
s_bool KParser_readCommandO(SakuraObj *skr, KFile *file);
s_bool KParser_readCommandQ(SakuraObj *skr, KFile *file);
s_bool KParser_readCommandT(SakuraObj *skr, KFile *file);
s_bool KParser_readCommandV(SakuraObj *skr, KFile *file);
s_bool KParser_readOctaveCommand(SakuraObj *skr, KFile *file);
s_bool KParser_readSplitter(SakuraObj *skr, KFile *file);
s_bool KParser_readLoop(SakuraObj *skr, KFile *file);
s_bool KParser_readCC(SakuraObj *skr, KFile *file);
s_bool KParser_readCC2(SakuraObj *skr, KFile *file, KToken *cno);
s_bool KParser_readSharpMacro(SakuraObj *skr, KFile *file);
s_bool KParser_rpn(SakuraObj *skr, KFile *file);
s_bool KParser_nrpn(SakuraObj *skr, KFile *file);

s_bool KParser_track(SakuraObj *skr, KFile *file);
s_bool KParser_channel(SakuraObj *skr, KFile *file);
s_bool KParser_readVoice(SakuraObj *skr, KFile *file);
s_bool KParser_include(SakuraObj *skr, KFile *file);
s_bool KParser_backslash(SakuraObj *skr, KFile *file);
s_bool KParser_harmony(SakuraObj *skr, KFile *file);
s_bool KParser_div(SakuraObj *skr, KFile *file);
s_bool KParser_int(SakuraObj *skr, KFile *file);
s_bool KParser_str(SakuraObj *skr, KFile *file);
s_bool KParser_array(SakuraObj *skr, KFile *file);
s_bool KParser_print(SakuraObj *skr, KFile *file);
s_bool KParser_time(SakuraObj *skr, KFile *file);
s_bool KParser_system(SakuraObj *skr, KFile *file);
s_bool KParser_timebase(SakuraObj *skr, KFile *file);
s_bool KParser_key(SakuraObj *skr, KFile *file);
s_bool KParser_trackKey(SakuraObj *skr, KFile *file);
s_bool KParser_if(SakuraObj *skr, KFile *file);
s_bool KParser_while(SakuraObj *skr, KFile *file);
s_bool KParser_defineUserFunc(SakuraObj *skr, KFile *file);
s_bool KParser_callUserFunc(SakuraObj *skr, KFile *file, KFunction *f); // sp sub func
s_bool KParser_return(SakuraObj *skr, KFile *file);
s_bool KParser_break(SakuraObj *skr, KFile *file);
s_bool KParser_octaveOnetime(SakuraObj *skr, KFile *file);
s_bool KParser_measureShift(SakuraObj *skr, KFile *file);
s_bool KParser_tempo(SakuraObj *skr, KFile *file);
s_bool KParser_metaText(SakuraObj *skr, KFile *file);
s_bool KParser_ChannelPrefix(SakuraObj *skr, KFile *file);
s_bool KParser_port(SakuraObj *skr, KFile *file);
s_bool KParser_rhythm(SakuraObj *skr, KFile *file);
s_bool KParser_rhythm_define(SakuraObj *skr, KFile *file);
s_bool KParser_keyflag(SakuraObj *skr, KFile *file);
s_bool KParser_timeSignature(SakuraObj *skr, KFile *file);
s_bool KParser_sysex(SakuraObj *skr, KFile *file);
s_bool KParser_sysex_hex(SakuraObj *skr, KFile *file);
s_bool KParser_sysex_(SakuraObj *skr, KFile *file, s_bool is_hex);
s_bool KParser_pitchBend(SakuraObj *skr, KFile *file);
s_bool KParser_trackSync(SakuraObj *skr, KFile *file);
s_bool KParser_trackSub(SakuraObj *skr, KFile *file);
s_bool KParser_playFrom(SakuraObj *skr, KFile *file);
s_bool KParser_playFromNow(SakuraObj *skr, KFile *file);
s_bool KParser_timekeyflag(SakuraObj *skr, KFile *file);

// Control Change
s_bool KParser_readCCUpper(SakuraObj *skr, KFile *file, int ccno);
s_bool KParser_chorus(SakuraObj *skr, KFile *file);
s_bool KParser_reverb(SakuraObj *skr, KFile *file);
s_bool KParser_expression(SakuraObj *skr, KFile *file);
s_bool KParser_panpot(SakuraObj *skr, KFile *file);
s_bool KParser_modulation(SakuraObj *skr, KFile *file);
s_bool KParser_volume(SakuraObj *skr, KFile *file);
s_bool KParser_var_effect(SakuraObj *skr, KFile *file);


#endif /* __k_parser_h__ */



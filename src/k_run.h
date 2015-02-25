/**
 * @file k_run.h
 */

#ifndef __k_run_h__
#define __k_run_h__

#include "k_main.h"
#include "s_lib.h"

typedef s_bool (*KRun_Command)(SakuraObj *skr, KToken *tok);


s_bool KRun_runFile(SakuraObj *skr, KFile *file);
s_bool KRun_runTokenList(SakuraObj *skr, KToken *token_top);

s_bool KRun_evalToken(SakuraObj *skr, KToken *tok);
s_bool KRun_evalTokenList(SakuraObj *skr, KToken *tok);
SValue *KRun_evalTokenPop(SakuraObj *skr, KToken *tok);

int KRun_calcNoteLen(const char *s, int def, int timebase);
int KRun_cmd_value(KToken *tok, int base);

s_bool KRun_pushValue(SakuraObj *skr, SValue *v);
SValue* KRun_popValue(SakuraObj *skr);
int KRun_popInt(SakuraObj *skr);

s_bool KRun_number(SakuraObj *skr, KToken *tok);
s_bool KRun_string(SakuraObj *skr, KToken *tok);
s_bool KRun_variable(SakuraObj *skr, KToken *tok);

// command
s_bool KRun_NoteOn(SakuraObj *skr, KToken *tok);
s_bool KRun_Rest(SakuraObj *skr, KToken *tok);
s_bool KRun_L(SakuraObj *skr, KToken *tok);
s_bool KRun_O(SakuraObj *skr, KToken *tok);
s_bool KRun_Q(SakuraObj *skr, KToken *tok);
s_bool KRun_V(SakuraObj *skr, KToken *tok);
s_bool KRun_T(SakuraObj *skr, KToken *tok);
s_bool KRun_octaveUD(SakuraObj *skr, KToken *tok);
s_bool KRun_loopBegin(SakuraObj *skr, KToken *tok);
s_bool KRun_loopEnd(SakuraObj *skr, KToken *tok);
s_bool KRun_loopBreak(SakuraObj *skr, KToken *tok);
s_bool KRun_track(SakuraObj *skr, KToken *tok);
s_bool KRun_channel(SakuraObj *skr, KToken *tok);
s_bool KRun_program(SakuraObj *skr, KToken *tok); 
s_bool KRun_CC(SakuraObj *skr, KToken *tok);
s_bool KRun_rpn(SakuraObj *skr, KToken *tok);
s_bool KRun_nrpn(SakuraObj *skr, KToken *tok);
s_bool KRun_rpn_nrpn(SakuraObj *skr, KToken *tok, int msb_cc, int lsb_cc);
s_bool KRun_include(SakuraObj *skr, KToken *tok);
s_bool KRun_harmony(SakuraObj *skr, KToken *tok);
s_bool KRun_div(SakuraObj *skr, KToken *tok);
s_bool KRun_print(SakuraObj *skr, KToken *tok);
s_bool KRun_defineINT(SakuraObj *skr, KToken *tok);
s_bool KRun_defineSTR(SakuraObj *skr, KToken *tok);
s_bool KRun_defineARRAY(SakuraObj *skr, KToken *tok);
s_bool KRun_evalTokens_operator(SakuraObj *skr, KToken *tok);
s_bool KRun_time(SakuraObj *skr, KToken *tok);
s_bool KRun_measureShift(SakuraObj *skr, KToken *tok);
s_bool KRun_tempo(SakuraObj *skr, KToken *tok);
s_bool KRun_writeText(SakuraObj *skr, KToken *tok, s_byte meta_type);
s_bool KRun_metaText(SakuraObj *skr, KToken *tok);
s_bool KRun_key(SakuraObj *skr, KToken *tok);
s_bool KRun_trackKey(SakuraObj *skr, KToken *tok);
s_bool KRun_if(SakuraObj *skr, KToken *tok);
s_bool KRun_while(SakuraObj *skr, KToken *tok);
s_bool KRun_function_skip(SakuraObj *skr, KToken *tok);
s_bool KRun_callUserFunc(SakuraObj *skr, KToken *tok);
s_bool KRun_return(SakuraObj *skr, KToken *tok);
s_bool KRun_doVariable(SakuraObj *skr, KToken *tok);
s_bool KRun_goto(SakuraObj *skr, KToken *tok);
s_bool KRun_goto_if_true(SakuraObj *skr, KToken *tok);
s_bool KRun_goto_if_false(SakuraObj *skr, KToken *tok);
s_bool KRun_octaveOnetime(SakuraObj *skr, KToken *tok);
s_bool KRun_writeMeta1b(SakuraObj *skr, KToken *tok, s_byte data1);
s_bool KRun_channelPrefix(SakuraObj *skr, KToken *tok);
s_bool KRun_port(SakuraObj *skr, KToken *tok);
s_bool KRun_sysex(SakuraObj *skr, KToken *tok);
s_bool KRun_rhythm(SakuraObj *skr, KToken *tok);
s_bool KRun_keyflag(SakuraObj *skr, KToken *tok);
s_bool KRun_timeSignature(SakuraObj *skr, KToken *tok);
s_bool KRun_pitchBend(SakuraObj *skr, KToken *tok);
s_bool KRun_v_onNote(SakuraObj *skr, KToken *tok);
s_bool KRun_t_onNote(SakuraObj *skr, KToken *tok);
s_bool KRun_trackSync(SakuraObj *skr, KToken *tok);
s_bool KRun_callSysFunc(SakuraObj *skr, KToken *tok);
s_bool KRun_trackSub(SakuraObj *skr, KToken *tok);
s_bool KRun_vAdd(SakuraObj *skr, KToken *tok);
s_bool KRun_sharp_macro_define(SakuraObj *skr, KToken *tok);
s_bool KRun_sharp_macro_exec(SakuraObj *skr, KToken *tok);
s_bool KRun_playFrom(SakuraObj *skr, KToken *tok);
s_bool KRun_CCUpper(SakuraObj *skr, KToken *tok);
s_bool KRun_array_element(SakuraObj *skr, KToken *tok);
s_bool KRun_var_replace(SakuraObj *skr, KToken *tok);

// reader command
s_bool KRun_tempo_r(SakuraObj *skr, KToken *tok);
s_bool KRun_track_r(SakuraObj *skr, KToken *tok);
s_bool KRun_channel_r(SakuraObj *skr, KToken *tok);
s_bool KRun_MML_r(SakuraObj *skr, KToken *tok);
s_bool KRun_SizeOf_r(SakuraObj *skr, KToken *tok);
s_bool KRun_version_r(SakuraObj *skr, KToken *tok);
s_bool KRun_random_r(SakuraObj *skr, KToken *tok);
s_bool KRun_step_r(SakuraObj *skr, KToken *tok);
s_bool KRun_time_r(SakuraObj *skr, KToken *tok);
s_bool KRun_mid_r(SakuraObj *skr, KToken *tok);
s_bool KRun_asc_r(SakuraObj *skr, KToken *tok);
s_bool KRun_strtonum_r(SakuraObj *skr, KToken *tok);
s_bool KRun_NoteNo_r(SakuraObj *skr, KToken *tok);

#endif /* __k_run_h__ */


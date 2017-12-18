/**
 * @file k_run.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "s_lib.h"
#include "k_main.h"
#include "k_run.h"
#include "s_test.h"
#include "s_value.h"
#include "k_parser.h"
#include "k_function.h"
#include "k_token.h"

KToken *k_run_next_token = NULL;
int func_return_count = 0;
const int NOTE_TONE[7] = {
    // a b  c d e f g
       9,11,0,2,4,5,7
  };

/**
 * Run tokens
 * @param skr
 * @param file
 * @return
 */
s_bool KRun_runTokenList(SakuraObj *skr, KToken *token_top) {
  s_bool result = S_TRUE, res = S_TRUE;
  KToken *tok = token_top;
  SString *tmp;

  while (tok != NULL) {
    if (func_return_count > 0) {
      if (skr->debug) { printf("<func_return_count no=%d>\n", func_return_count); }
      func_return_count--;
      return S_TRUE;
    }
    if (skr->debug) {
      tmp = KToken_toString(tok);
      printf("[KRun_tokens] %s\n", tmp->ptr);
      SString_free(tmp);
    }
    switch (tok->token_type) {
      // TODO: KRun_runTokenList switch
      case KTOKEN_UNKNOWN:
      case KTOKEN_TOP:
      case KTOKEN_NOP: KRun_NOP(skr, tok); break;
      case KTOKEN_NOTE: res = KRun_NoteOn(skr, tok); break;
      case KTOKEN_NOTE_NO: res = KRun_NoteOn(skr, tok); break;
      case KTOKEN_REST: res = KRun_Rest(skr, tok); break;
      case KTOKEN_COMMAND_L: res = KRun_L(skr, tok); break;
      case KTOKEN_COMMAND_O: res = KRun_O(skr, tok); break;
      case KTOKEN_COMMAND_Q: res = KRun_Q(skr, tok); break;
      case KTOKEN_COMMAND_V: res = KRun_V(skr, tok); break;
      case KTOKEN_COMMAND_T: res = KRun_T(skr, tok); break;
      case KTOKEN_COMMAND_H: res = KRun_H(skr, tok); break;
      case KTOKEN_OCTAVE_UD: res = KRun_octaveUD(skr, tok); break;
      case KTOKEN_LOOP_BEGIN: res = KRun_loopBegin(skr, tok); break;
      case KTOKEN_LOOP_END: res = KRun_loopEnd(skr, tok); break;
      case KTOKEN_LOOP_BREAK: res = KRun_loopBreak(skr, tok); break;
      case KTOKEN_TRACK: res = KRun_track(skr, tok); break;
      case KTOKEN_CHANNEL: res = KRun_channel(skr, tok); break;
      case KTOKEN_PROGRAM: res = KRun_program(skr, tok); break;
      case KTOKEN_CC: res = KRun_CC(skr, tok); break;
      case KTOKEN_TIME: res = KRun_time(skr, tok); break;
      case KTOKEN_MEASURE_SHIFT: res = KRun_measureShift(skr, tok); break;
      case KTOKEN_TEMPO: res = KRun_tempo(skr, tok); break;
      case KTOKEN_CH_PREFIX: res = KRun_channelPrefix(skr, tok); break;
      case KTOKEN_PORT: res = KRun_port(skr, tok); break;
      case KTOKEN_TIME_SIGNATURE: res = KRun_timeSignature(skr, tok); break;
      case KTOKEN_METATEXT: res = KRun_metaText(skr, tok); break;
      case KTOKEN_SYS_EX: res = KRun_sysex(skr, tok); break;
      case KTOKEN_INCLUDE: res = KRun_include(skr, tok); break;
      case KTOKEN_OCTAVE1UD: res = KRun_octaveOnetime(skr, tok); break;
      case KTOKEN_HARMONY: res = KRun_harmony(skr, tok); break;
      case KTOKEN_DIV: res = KRun_div(skr, tok); break;
      case KTOKEN_RHYTHM: res = KRun_rhythm(skr, tok); break;
      case KTOKEN_KEYFLAG: res = KRun_keyflag(skr, tok); break;
      case KTOKEN_V_ON_NOTE: res = KRun_v_onNote(skr, tok); break;
      case KTOKEN_T_ON_NOTE: res = KRun_t_onNote(skr, tok); break;
      case KTOKEN_PRINT: res = KRun_print(skr, tok); break;
      case KTOKEN_DEF_INT: res = KRun_defineINT(skr, tok); break;
      case KTOKEN_DEF_STR: res = KRun_defineSTR(skr, tok); break;
      case KTOKEN_DEF_ARRAY: res = KRun_defineARRAY(skr, tok); break;
      case KTOKEN_KEY: res = KRun_key(skr, tok); break;
      case KTOKEN_TRACK_KEY: res = KRun_trackKey(skr, tok); break;
      case KTOKEN_TRACK_SYNC: res = KRun_trackSync(skr, tok); break;
      case KTOKEN_TRACK_SUB: res = KRun_trackSub(skr, tok); break;
      case KTOKEN_V_ADD: res = KRun_vAdd(skr, tok); break;
      case KTOKEN_SHARP_MACRO_DEFINE: res = KRun_sharp_macro_define(skr, tok); break;
      case KTOKEN_SHARP_MACRO_EXEC: res = KRun_sharp_macro_exec(skr, tok); break;
      case KTOKEN_CC_UPPER: res = KRun_CCUpper(skr, tok); break;
      case KTOKEN_RPN: res = KRun_rpn(skr, tok); break;
      // FOR SCRIPT
      case KTOKEN_IF: res = KRun_if(skr, tok); break;
      case KTOKEN_WHILE: res = KRun_while(skr, tok); break;
      case KTOKEN_DEF_FUNCTION: res = KRun_function_skip(skr, tok); break;
      case KTOKEN_CALL_USER_FUNC: res = KRun_callUserFunc(skr, tok); break;
      case KTOKEN_RETURN: res = KRun_return(skr, tok); break;
      case KTOKEN_DO_VARIABLE: res = KRun_doVariable(skr, tok); break;
      case KTOKEN_GOTO: res = KRun_goto(skr, tok); break;
      case KTOKEN_GOTO_IF_TRUE: res = KRun_goto_if_true(skr, tok); break;
      case KTOKEN_GOTO_IF_FALSE: res = KRun_goto_if_false(skr, tok); break;
      case KTOKEN_ADD:
      case KTOKEN_SUB:
      case KTOKEN_CALC_DIV:
      case KTOKEN_MOD:
      case KTOKEN_MUL:
      case KTOKEN_GT:
      case KTOKEN_LT:
      case KTOKEN_EQ:
      case KTOKEN_GTEQ:
      case KTOKEN_LTEQ:
      case KTOKEN_AND:
      case KTOKEN_OR: res = KRun_evalTokens_operator(skr, tok); break;
      case KTOKEN_NUMBER: res = KRun_number(skr, tok);   break;
      case KTOKEN_STRING: res = KRun_string(skr, tok);   break;
      case KTOKEN_VARIABLE: res = KRun_variable(skr, tok); break;
      case KTOKEN_CALL_SYS_FUNC: res = KRun_callSysFunc(skr, tok); break;
      case KTOKEN_PLAY_FROM: res = KRun_playFrom(skr, tok); break;
      case KTOKEN_INC: KRun_pushValue(skr, SValue_newInt(KRun_popInt(skr) + 1)); break;
      case KTOKEN_DEC: KRun_pushValue(skr, SValue_newInt(KRun_popInt(skr) - 1)); break;
      case KTOKEN_ARRAY_ELEMENT: res = KRun_array_element(skr, tok); break;
      case KTOKEN_VAR_REPLACE: res = KRun_var_replace(skr, tok); break;
      case KTOKEN_PLAY_BEGIN: res = KRun_playBegin(skr, tok); break;
      case KTOKEN_PLAY_END: res = KRun_playEnd(skr, tok); break;
      case KTOKEN_PLAY_TRACK: res = KRun_playTrack(skr, tok); break;
      default:
        printf("[ERROR] (Runtime) unknown 0x%x\n", tok->token_type);
        break;
    }
    if (res == S_FALSE) {
      if (skr->debug) {
        printf("[FALSE] 0x%X\n", tok->token_type);
      }
    }
    // normaly do next token
    if (k_run_next_token == NULL) {
      tok = tok->next;
      continue;
    }
    // jump token
    tok = k_run_next_token;
    k_run_next_token = NULL;
  }
  // return
  if (func_return_count > 0) func_return_count--;

  return result;
}

/**
 * run token list
 */
s_bool KRun_runFile(SakuraObj *skr, KFile *file) {
  KToken *tok = file->token_top;
  return KRun_runTokenList(skr, tok);
}

s_bool KRun_NOP(SakuraObj *skr, KToken *tok) {
  if (!skr->debug) return S_TRUE;
  if (tok->value != NULL) {
    printf("[NOP] %s\n", tok->value->ptr);
  } else {
    printf("[NOP]\n");
  }
  return S_TRUE;
}

s_bool KRun_playBegin(SakuraObj *skr, KToken *tok) {
  KTrack *cur = SakuraObj_curTrack(skr);
  skr->info->play_temp_track = skr->info->track_no;
  skr->info->play_temp_time = cur->time;
  return S_TRUE;
}
s_bool KRun_playEnd(SakuraObj *skr, KToken *tok) {
  skr->info->track_no = skr->info->play_temp_track;
  return S_TRUE;
}
s_bool KRun_playTrack(SakuraObj *skr, KToken *tok) {
  int no;
  KTrack *tr;
  //
  KRun_evalTokenList(skr, tok->arg);
  no = KRun_popInt(skr);
  skr->info->track_no = SET_IN_RANGE(1, 256, no);
  tr = SakuraObj_curTrack(skr);
  tr->time = skr->info->play_temp_time;
  //
  if (skr->debug) {
    printf("[TR=%d](PLAY)", skr->info->track_no);
  }
  return S_TRUE;
}

s_bool KRun_NoteOn(SakuraObj *skr, KToken *tok) {
  KTrack *tr;
  KSmfTrack *smf_tr;
  int o, l, q, v, t, len, qlen, sharp, natural, noteno, notechar = -1;
  KSmfEvent *e;
  char name;
  SValue *val;
  KTokenNoteOnOption *opt = (KTokenNoteOnOption*)tok->extra;

  // default
  tr = SakuraObj_curTrack(skr);
  smf_tr = SakuraObj_curSmfTrack(skr);
  o = tr->o + skr->info->octaveOnetime;
  l = tr->l;
  q = tr->q;
  v = tr->v;
  t = tr->t;
  skr->info->octaveOnetime = 0;

  // ---------------------------
  // note no
  if (tok->token_type == KTOKEN_NOTE) {
    name = (char)tok->no;
    if (IN_RANGE('a', 'g', name)) {
      notechar = (name - 'a');
      noteno = o * 12 + NOTE_TONE[notechar];
    } else {
      return S_FALSE;
    }
  } else {
    // eval [n]
    KRun_evalTokenList(skr, tok->arg);
    val = SList_pop(skr->valueStack);
    noteno = SValue_getInt(val);
    SValue_free(val);
  }

  // ---------------------------
  // sharp & key shift
  sharp = tok->flag & 0xFF;
  natural = (tok->flag >> 8);
  // keyflag
  if (notechar >= 0 && natural == 0) {
    noteno += skr->info->keyflag[notechar];
  }
  noteno += sharp;
  // key shift
  noteno += skr->info->key;
  // track key
  noteno += tr->track_key;

  // ---------------------------
  // length
  if (tok->value != NULL) {
    len = KRun_calcNoteLen(tok->value->ptr, l, skr->info->timebase);
  } else {
    len = l;
  }

  // ---------------------------
  // qlen
  if (tr->q_step_mode) {
    qlen = q;
  } else {
    qlen = (int)floor(len * (q / 100.0)) - 1;
  }
  switch (opt->q_mode) {
    case K_NOTEON_MODE_ABSOLUTE:
      if (opt->q_step_mode) {
        qlen = opt->q;
      } else {
        q = opt->q;
        qlen = (int)floor(len * (q / 100.0)) - 1;
      }
      break;
    case K_NOTEON_MODE_RELATIVE:
      if (opt->q_step_mode) {
        qlen += opt->q;
      } else {
        q += opt->q;
        qlen = (int)floor(len * (q / 100.0)) - 1;
      }
      break;
  }

  switch (opt->v_mode) {
    case K_NOTEON_MODE_ABSOLUTE:
      v = opt->v;
      break;
    case K_NOTEON_MODE_RELATIVE:
      v += opt->v;
      break;
  }

  switch (opt->t_mode) {
    case K_NOTEON_MODE_ABSOLUTE:
      t = opt->t;
      break;
    case K_NOTEON_MODE_RELATIVE:
      t += opt->t;
      break;
  }

  // minus 'h' param
  qlen -= tr->h;

  // v.onNote
  if (tr->v_onNote->len > 0) {
    val = SList_shift(tr->v_onNote);
    v = SValue_getInt(val);
    SValue_free(val);
  }
  // t.onNote
  if (tr->t_onNote->len > 0) {
    val = SList_shift(tr->t_onNote);
    v = SValue_getInt(val);
    SValue_free(val);
  }
  // write smf
  // note on
  e = KSmfEvent_new();
  e->time = tr->time + t;
  e->ch = tr->ch;
  e->status = 0x90;
  e->data1 = noteno;
  e->data2 = v;
  KSmfTrack_append(smf_tr, e);
  // note off
  e = KSmfEvent_new();
  e->time = tr->time + qlen + t;
  e->ch = tr->ch;
  e->status = 0x80;
  e->data1 = noteno;
  e->data2 = v;
  KSmfTrack_append(smf_tr, e);

  if (skr->debug) {
    printf("%04d:(KRun_NoteOn note=%d(len=%d/%d,v%dt%d)\n",
      tr->time, noteno, qlen, len, v, t);
  }

  if (!skr->info->is_harmony) {
    // inc time
    tr->time += len;
  }

  return S_TRUE;
}

s_bool KRun_Rest(SakuraObj *skr, KToken *tok) {
  // default
  KTrack *tr = SakuraObj_curTrack(skr);
  int l = tr->l;
  int len;

  // length
  len = KRun_calcNoteLen(tok->value->ptr, l, skr->info->timebase);
  if (tok->flag == -1) len *= -1;
  tr->time += len;

  if (skr->debug) {
    printf("(KRun_Rest len=%d)", len);
  }

  // TODO: CC.onNote/.onTime

  return S_TRUE;
}


s_bool KRun_L(SakuraObj *skr, KToken *tok) {
  KTrack *tr = SakuraObj_curTrack(skr);
  int len;
  int tb;

  // get length
  tb = skr->info->timebase;
  len = KRun_calcNoteLen(tok->value->ptr, tb, tb);
  // set new lengtth
  tr->l = len;

  return S_TRUE;
}

int KRun_cmd_value(KToken *tok, int base) {
  char *p;
  int v;

  if (tok == NULL || tok->value == NULL) {
    return 0;
  }
  p = tok->value->ptr;
  v = (int)strtol(p, NULL, 0);
  if (tok->flag == 0) return v;
  v = (tok->flag > 0) ? base + v : base - v;

  return (int)v;
}

s_bool KRun_O(SakuraObj *skr, KToken *tok) {
  KTrack *tr;

  tr = SakuraObj_curTrack(skr);
  tr->o = KRun_cmd_value(tok, tr->o);
  tr->o = SET_IN_RANGE(0, 10, tr->o);

  return S_TRUE;
}

s_bool KRun_Q(SakuraObj *skr, KToken *tok) {
  KTrack *tr;

  tr = SakuraObj_curTrack(skr);
  tr->q = KRun_cmd_value(tok, tr->q);
  tr->q = SET_IN_RANGE(1, 1000, tr->q);
  tr->q_step_mode = (tok->no == 1);

  return S_TRUE;
}

s_bool KRun_V(SakuraObj *skr, KToken *tok) {
  KTrack *tr;
  int v;

  tr = SakuraObj_curTrack(skr);
  // direct value?
  if (tok->arg != NULL) {
    KRun_evalToken(skr, tok->arg);
    v = KRun_popInt(skr);
  } else {
    v = tok->no;
  }
  // flag
  if (tok->flag == 0) {
    tr->v = v;
  } else {
    // inc/dec
    if (tok->no == -1) {
      v = tr->v + (tok->flag * tr->vAdd);
    }
    else {
      v = tr->v + tok->flag * tok->no;
    }
  }
  tr->v = SET_IN_RANGE(1, 127, tr->v);

  return S_TRUE;
}

s_bool KRun_T(SakuraObj *skr, KToken *tok) {
  KTrack *tr;

  tr = SakuraObj_curTrack(skr);
  tr->t = KRun_cmd_value(tok, tr->t);

  return S_TRUE;
}

s_bool KRun_H(SakuraObj *skr, KToken *tok) {
  KTrack *tr;

  tr = SakuraObj_curTrack(skr);
  tr->h = KRun_cmd_value(tok, tr->h);

  return S_TRUE;
}


/**
 * Calc n's style note length
 */
int KRun_calcNoteLen(const char *s, int def, int timebase) {
  char *p;
  int base, n, nstep, total;

  if (*s == '\0') return def;
  total = 0;
  base = timebase * 4;
  p = (char*)s;
  for (;;) {
    if (*p == '%') {
      p++;
      n = s_readInt(p, 0, &p);
      total += n;
    } else {
      n = s_readInt(p, 0, &p);
      if (n == 0) {
        total += def;
      } else {
        nstep = base / n;
        total += nstep;
        while (*p == '.') {
          p++;
          nstep = nstep / 2;
          total += nstep;
        }
      }
    }
    SKIP_SPACE(p);
    if (*p == '\0') break;
    if (*p != '^') break;
    p++;
  }
  return total;
}

s_bool KRun_octaveUD(SakuraObj *skr, KToken *tok) {
  KTrack *tr;

  tr = SakuraObj_curTrack(skr);
  tr->o += tok->flag;
  tr->o = SET_IN_RANGE(0, 10, tr->o);

  return S_TRUE;
}



s_bool KRun_loopBegin(SakuraObj *skr, KToken *tok) {
  KLoopItem *item;
  int loop_count = 2;

  // get loop count
  if (tok->arg != NULL) {
    KRun_evalToken(skr, tok->arg);
    loop_count = KRun_popInt(skr);
  }

  // set loop
  item = s_new(KLoopItem);
  item->begin = tok;
  item->index = 0;
  item->count = loop_count;
  SList_push(skr->loopStack, item);


  return S_TRUE;
}

s_bool KRun_loopEnd(SakuraObj *skr, KToken *tok) {
  KLoopItem *item;

  if (skr->loopStack->len == 0) {
    k_error(skr, K_ERROR_LOOP_RANGE, "']'", tok->pos);
    return S_FALSE;
  }
  item = (KLoopItem*)SList_peekLast(skr->loopStack);
  item->end = tok;
  item->index++;
  // loop finished?
  if (item->count <= item->index) {
    SList_pop(skr->loopStack);
    s_free(item);
    return S_TRUE;
  }
  // continune loop
  k_run_next_token = item->begin->next;
  return S_TRUE;
}

s_bool KRun_loopBreak(SakuraObj *skr, KToken *tok) {
  KLoopItem *item;
  KToken *t;

  if (skr->loopStack->len == 0) {
    k_error(skr, K_ERROR_LOOP_RANGE, "':'", tok->pos);
    return S_FALSE;
  }
  item = (KLoopItem*)SList_peekLast(skr->loopStack);

  // loop finished? break
  if (item->index == item->count - 1) {
    SList_pop(skr->loopStack);
    if (item->end != NULL) {
      // set last pos
      k_run_next_token = item->end->next;
    } else {
      // if not yet reach last, then find end pos
      t = tok->next;
      while (t != NULL) {
        if (t->token_type == KTOKEN_LOOP_END) {
          k_run_next_token = t->next;
          break;
        }
        t = t->next;
      }
    }
    s_free(item);
    return S_TRUE;
  }
  // continune loop
  return S_TRUE;
}

s_bool KRun_track(SakuraObj *skr, KToken *tok) {
  int no;
  KTrack *tr;
  s_bool b;
  //
  KRun_evalTokenList(skr, tok->arg);
  no = KRun_popInt(skr);
  skr->info->track_no = SET_IN_RANGE(1, 256, no);
  b = KTrackList_exists(skr->tracks, no);
  tr = SakuraObj_curTrack(skr);
  if (!b) {
    tr->time = skr->info->timeTrackSync;
    // printf("[TR=%d] new time=%d\n", skr->info->track_no, tr->time);
  }
  //
  if (skr->debug) {
    printf("[TR=%d]", skr->info->track_no);
  }
  return S_TRUE;
}

s_bool KRun_channel(SakuraObj *skr, KToken *tok) {
  SValue *no;
  KTrack *tr;
  int n;

  tr = SakuraObj_curTrack(skr);

  KRun_evalTokenList(skr, tok->arg);
  no = SList_pop(skr->valueStack);
  n = SValue_getInt(no);
  SValue_free(no);

  n = SET_IN_RANGE(1, 16, n);
  tr->ch = n;

  return S_TRUE;
}

s_bool KRun_program(SakuraObj *skr, KToken *tok) {
  SValue *no;
  KTrack *tr;
  int n;
  KSmfTrack *smf_tr;
  KSmfEvent *e;

  tr = SakuraObj_curTrack(skr);
  smf_tr = SakuraObj_curSmfTrack(skr);

  KRun_evalTokenList(skr, tok->arg);
  no = KRun_popValue(skr);
  if (no != NULL && no->v_type != SVALUE_TYPE_NULL) {
    n = SValue_getInt(no);
  } else {
    n = 1;
  }
  SValue_free(no);
  if (skr->debug) {
    printf("-@%d\n", n);
  }
  tr->Voice = n;

  // voice change
  e = KSmfEvent_new();
  e->time = tr->time;
  e->ch = tr->ch;
  e->status = 0xC0;
  e->data1 = n - 1;
  e->data2 = 0;
  KSmfTrack_append(smf_tr, e);

  return S_TRUE;
}

s_bool KRun_CC(SakuraObj *skr, KToken *tok) {
  SValue *value;
  KTrack *tr;
  int no, v;
  KSmfTrack *smf_tr;
  KSmfEvent *e;

  tr = SakuraObj_curTrack(skr);
  smf_tr = SakuraObj_curSmfTrack(skr);

  KRun_evalTokenList(skr, tok->arg);

  // (2/2) value
  value = SList_pop(skr->valueStack);
  if (value == NULL) {
    k_error(skr, K_ERROR_SYSTEM, "KRun_CC not enough arguments", tok->pos);
    return S_FALSE;
  }
  v = SValue_getInt(value);
  SValue_free(value);

  // (1/2) Control no
  value = SList_pop(skr->valueStack);
  if (value == NULL) {
    k_error(skr, K_ERROR_SYSTEM, "KRun_CC not enough arguments", tok->pos);
    return S_FALSE;
  }
  no = SValue_getInt(value);
  SValue_free(value);

  // control change
  if (no == 130) { // PitchBend Full range
    e = KSmfEvent_newPitchBend(tr->time, tr->ch, v);
  }
  else if (no == 131) { // PitchBend Half range
    e = KSmfEvent_newPitchBend(tr->time, tr->ch, ((v & 0x7F) << 7));
  }
  else {
    e = KSmfEvent_newCC(tr->time, tr->ch, no, v);
  }
  KSmfTrack_append(smf_tr, e);
  return S_TRUE;
}

s_bool KRun_rpn_nrpn(SakuraObj *skr, KToken *tok, int msb_cc, int lsb_cc) {
  int n_msb, n_lsb, v_msb, v_lsb = 0, t;
  KTrack *tr;
  KSmfTrack *smf_tr;
  KSmfEvent *e;

  // eval
  KRun_evalTokenList(skr, tok->arg);
  if (tok->no >= 4) { v_lsb = KRun_popInt(skr); }
  v_msb = KRun_popInt(skr);
  n_lsb = KRun_popInt(skr);
  n_msb = KRun_popInt(skr);

  // write
  tr = SakuraObj_curTrack(skr);
  smf_tr = SakuraObj_curSmfTrack(skr);

  // RPN/NRPN Time Shift
  t = 0;
  if (tr->rpn_nrpn_time_last != tr->time) {
    tr->rpn_nrpn_time_last = tr->time;
    tr->rpn_nrpn_time_shift = 0;
  } else {
    tr->rpn_nrpn_time_shift++;
    t = tr->rpn_nrpn_time_shift;
  }

  e = KSmfEvent_newCC(tr->time + t, tr->ch, msb_cc, n_msb);
  KSmfTrack_append(smf_tr, e);
  e = KSmfEvent_newCC(tr->time + t, tr->ch, lsb_cc, n_lsb);
  KSmfTrack_append(smf_tr, e);

  e = KSmfEvent_newCC(tr->time + t + 1, tr->ch,   6, v_msb);
  KSmfTrack_append(smf_tr, e);
  if (tok->no >= 4) {
    e = KSmfEvent_newCC(tr->time + t + 1, tr->ch, 38, v_lsb);
    KSmfTrack_append(smf_tr, e);
  }

  return S_TRUE;
}


s_bool KRun_rpn(SakuraObj *skr, KToken *tok) {
  return KRun_rpn_nrpn(skr, tok, 101, 100);
}

s_bool KRun_nrpn(SakuraObj *skr, KToken *tok) {
  return KRun_rpn_nrpn(skr, tok, 99, 98);
}


// Push Value
s_bool KRun_pushValue(SakuraObj *skr, SValue *v) {
  SString *tmp;
  if (skr->debug) {
    tmp = SValue_getString(v);
    printf("[PUSH] %s\n", tmp->ptr);
    SString_free(tmp);
  }
  SList_push(skr->valueStack, v);
  return S_TRUE;
}
// Pop Value
SValue* KRun_popValue(SakuraObj *skr) {
  SValue *v;
  SString *tmp;

  v = SList_pop(skr->valueStack);
  if (skr->debug) {
    tmp = SValue_getString(v);
    printf("[POP] %s\n", tmp->ptr);
    SString_free(tmp);
  }
  return v;
}
int KRun_popInt(SakuraObj *skr) {
  SValue *v = KRun_popValue(skr);
  int iv = SValue_getInt(v);
  if (v != NULL) SValue_free(v);
  return iv;
}

// Push Number
s_bool KRun_number(SakuraObj *skr, KToken *tok) {
  SValue *v = SValue_newInt(tok->no);
  KRun_pushValue(skr, v);
  return S_TRUE;
}

// Push String
s_bool KRun_string(SakuraObj *skr, KToken *tok) {
  SValue *v = SValue_newString(tok->value->ptr);
  KRun_pushValue(skr, v);
  return S_TRUE;
}

// Push Variable
s_bool KRun_variable(SakuraObj *skr, KToken *tok) {
  SValue *v;

  // get variable
  v = SHash_get(skr->local_vars, tok->value->ptr);
  if (v == NULL) {
    v = SHash_get(skr->variables, tok->value->ptr);
  }
  // clone
  v = SValue_clone(v);
  KRun_pushValue(skr, v);
  return S_TRUE;
}


s_bool KRun_evalTokens_operator(SakuraObj *skr, KToken *tok) {
  SValue *a, *b;
  int v;

  a = KRun_evalTokenPop(skr, tok->arg);
  b = KRun_evalTokenPop(skr, tok->extra);
  if (skr->debug) {
    printf("(CALC/POP=[%d],[%d])", SValue_getInt(a), SValue_getInt(b));
  }
  switch (tok->token_type) {
    case KTOKEN_ADD:
      SValue_add(a, b);
      KRun_pushValue(skr, SValue_clone(a));
      break;
    case KTOKEN_SUB:
      v = SValue_getInt(a) - SValue_getInt(b);
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_MUL:
      v = SValue_getInt(a) * SValue_getInt(b);
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_CALC_DIV:
      v = SValue_getInt(a) / SValue_getInt(b);
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_MOD:
      v = SValue_getInt(a) % SValue_getInt(b);
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_GT:
      v = (SValue_getInt(a) > SValue_getInt(b)) ? S_TRUE : S_FALSE;
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_GTEQ:
      v = (SValue_getInt(a) >= SValue_getInt(b)) ? S_TRUE : S_FALSE;
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_LT:
      v = (SValue_getInt(a) < SValue_getInt(b)) ? S_TRUE : S_FALSE;
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_LTEQ:
      v = (SValue_getInt(a) <= SValue_getInt(b)) ? S_TRUE : S_FALSE;
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_EQ:
      v = SValue_eq(a, b);
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_NOT_EQ:
      v = (SValue_getInt(a) != SValue_getInt(b)) ? S_TRUE : S_FALSE;
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_AND:
      v = (SValue_getInt(a) & SValue_getInt(b));
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    case KTOKEN_OR:
      v = (SValue_getInt(a) | SValue_getInt(b));
      KRun_pushValue(skr, SValue_newInt(v));
      break;
    default:
      printf("[SYSTEM ERROR] KRun_evalTokens_operator\n");
      break;
  }

  // free
  SValue_free(a);
  SValue_free(b);

  return S_TRUE;
}

SValue *KRun_evalTokenPop(SakuraObj *skr, KToken *tok) {
  SValue *v;
  SString *tmp;

  if (tok == NULL) {
      k_error(skr, K_ERROR_SYSTEM, "Stack not enough.", NULL);
      return NULL;
  }

  if (!KRun_evalTokenList(skr, tok)) {
    return NULL;
  }
  v = KRun_popValue(skr);

  if (skr->debug) {
    if (v == NULL) {
      k_error(skr, K_ERROR_SYSTEM, "Stack not enough.", tok->pos);
    } else {
      tmp = SValue_getString(v);
      printf("[POP] %s\n", tmp->ptr);
      SString_free(tmp);
    }
  }

  return v;
}

s_bool KRun_evalToken(SakuraObj *skr, KToken *tok) {
  KToken *t = tok;
  SString *vname, *tmp;
  SValue *v;
  KFunction *f;
  char buf[1024];
  KRun_Command runcmd;

  if (tok == NULL) return S_TRUE;
  switch (t->token_type) {
    case KTOKEN_NUMBER:
      if (skr->debug) { printf("(push number:%d)", t->no); }
      KRun_pushValue(skr, SValue_newInt(t->no));
      break;
    case KTOKEN_STRING:
      if (skr->debug) { printf("(push string:%s)", t->value->ptr); }
      v = SValue_newString(t->value->ptr);
      KRun_pushValue(skr, v);
      break;
    case KTOKEN_VARIABLE:
      vname = t->value;
      // find variable
      v = SHash_get(skr->local_vars, vname->ptr);
      if (v == NULL) {
        v = SHash_get(skr->variables, vname->ptr);
      }
      if (v == NULL) {
        sprintf(buf, "Varible[%s] is not defined.", vname->ptr);
        k_warn(skr, K_WARN_VAR_NOT_DEFINE, buf, t->pos);
        KRun_pushValue(skr, SValue_new());
      } else {
        if (skr->debug) {
          printf("(PUSH variable:%s=%d)", vname->ptr, SValue_getInt(v));
        }
        //printf("Varible[%s] is [%d]", vname->ptr, SValue_getInt(v));
        SList_push(skr->valueStack, SValue_clone(v));
      }
      break;
    case KTOKEN_CALL_USER_FUNC:
      f = (KFunction*)t->extra;
      if (skr->debug) { printf("<CALL_USER_FUNC name='%s'>\n", f->name); }
      KRun_callUserFunc(skr, t);
      if (skr->debug) { printf("</CALL_USER_FUNC name='%s'>\n", f->name); }
      break;
    case KTOKEN_CALL_SYS_FUNC:
      runcmd = (KRun_Command)t->extra;
      runcmd(skr, t);
      break;
    case KTOKEN_INC:
      KRun_pushValue(skr, SValue_newInt(KRun_popInt(skr) + 1));
      break;
    case KTOKEN_DEC:
      KRun_pushValue(skr, SValue_newInt(KRun_popInt(skr) - 1));
      break;
    case KTOKEN_ARRAY_ELEMENT:
      KRun_array_element(skr, tok);
      break;
    case KTOKEN_ADD:
    case KTOKEN_SUB:
    case KTOKEN_MUL:
    case KTOKEN_CALC_DIV:
    case KTOKEN_MOD:
    case KTOKEN_GT:
    case KTOKEN_GTEQ:
    case KTOKEN_LT:
    case KTOKEN_LTEQ:
    case KTOKEN_EQ:
    case KTOKEN_NOT_EQ:
    case KTOKEN_AND:
    case KTOKEN_OR:
      if (skr->debug) { printf("(eval operator)"); }
      KRun_evalTokens_operator(skr, tok);
      break;
    default:
      tmp = KToken_toString(t);
      sprintf(buf, "Unknown Eval Token: %s", tmp->ptr);
      k_error(skr, K_ERROR_SYSTEM, buf, tok->pos);
      SString_free(tmp);
      break;
  } // end of switch
  return S_TRUE;
}

s_bool KRun_evalTokenList(SakuraObj *skr, KToken *tok) {
  KToken *t;

  t = tok;
  while (t != NULL) {
    if (KRun_evalToken(skr, t)) {
      t = t->next;
      continue;
    }
    break;
  }

  return S_TRUE;
}

s_bool KRun_time(SakuraObj *skr, KToken *tok) {
  KTrack *cur;
  int mes, beat, tick;
  SValue *m, *b, *t;
  int step;

  // eval time params
  if (tok->no == 1) {
    KRun_evalToken(skr, tok->arg);
    step = KRun_popInt(skr);
  } else {
    KRun_evalTokenList(skr, tok->arg);
    t = SList_pop(skr->valueStack);
    b = SList_pop(skr->valueStack);
    m = SList_pop(skr->valueStack);
    //
    tick = SValue_getInt(t);
    beat = SValue_getInt(b);
    mes  = SValue_getInt(m);
    // printf("[TIME(%d:%d:%d)]", mes, beat, tick);
    //
    SValue_free(t);
    SValue_free(b);
    SValue_free(m);

    step = SakuraObj_time2step(skr, mes, beat, tick);
  }
  cur = SakuraObj_curTrack(skr);
  cur->time = step;

  return S_TRUE;
}

s_bool KRun_measureShift(SakuraObj *skr, KToken *tok) {
  SValue *v;

  // eval time params
  KRun_evalTokenList(skr, tok->arg);
  v = KRun_popValue(skr);
  skr->info->measureShift = SValue_getInt(v);
  SValue_free(v);

  return S_TRUE;
}

s_bool KRun_tempo(SakuraObj *skr, KToken *tok) {
  SValue *tempo;
  KTrack *cur = SakuraObj_curTrack(skr);
  KSmfTrack *smf_tr;
  KSmfEvent *e;
  double usec_d;
  unsigned long usec;


  tempo = KRun_evalTokenPop(skr, tok->arg);
  skr->info->tempo = SValue_getInt(tempo);
  SValue_free(tempo);

  usec_d = 60 * pow(10, 6) / skr->info->tempo;
  usec = (unsigned long)usec_d;

  // Tempo
  smf_tr = SakuraObj_curSmfTrack(skr);
  e = KSmfEvent_new();
  e->time = cur->time;
  e->status = 0xFF;
  e->data1 = 0x51;
  e->data2 = 0x03;
  e->meta = s_new_a(s_byte, 3);
  e->meta[0] = (usec >> 16) & 0xFF;
  e->meta[1] = (usec >> 8 ) & 0xFF;
  e->meta[2] = (usec      ) & 0xFF;
  KSmfTrack_append(smf_tr, e);

  return S_TRUE;
}

s_bool KRun_tempo_r(SakuraObj *skr, KToken *tok) {
  SValue *v;
  v = SValue_newInt(skr->info->tempo);
  KRun_pushValue(skr, v);
  return S_TRUE;
}
s_bool KRun_track_r(SakuraObj *skr, KToken *tok) {
  KRun_pushValue(skr, SValue_newInt(skr->info->track_no));
  return S_TRUE;
}

s_bool KRun_channel_r(SakuraObj *skr, KToken *tok) {
  KTrack *tr = SakuraObj_curTrack(skr);
  KRun_pushValue(skr, SValue_newInt(tr->ch));
  return S_TRUE;
}

s_bool KRun_version_r(SakuraObj *skr, KToken *tok) {
  KRun_pushValue(skr, SValue_newString(SAKURA3_VERSION));
  return S_TRUE;
}

s_bool KRun_random_r(SakuraObj *skr, KToken *tok) {
  int n, m, r, w;
  // eval
  KRun_evalTokenList(skr, tok->arg);
  if (tok->no == 1) {
    n = KRun_popInt(skr);
    r = rand() % n;
  } else if (tok->no >= 2) {
    n = KRun_popInt(skr);
    m = KRun_popInt(skr);
    w = m - n;
    r = n + (rand() % w);
  } else {
    r = rand();
  }
  KRun_pushValue(skr, SValue_newInt(r));
  return S_TRUE;
}

s_bool KRun_randomSelect_r(SakuraObj *skr, KToken *tok) {
  int i, r, num;
  int *v;
  // eval
  KRun_evalTokenList(skr, tok->arg);
  num = tok->no;
  // pop values
  v = s_new_a(int, num);
  for (i = 0; i < num; i++) {
    v[num - i - 1] = KRun_popInt(skr);
  }
  r = rand() % num;
  KRun_pushValue(skr, SValue_newInt(v[r]));
  s_free(v);
  return S_TRUE;
}

s_bool KRun_step_r(SakuraObj *skr, KToken *tok) {
  KTrack *tr = SakuraObj_curTrack(skr);
  int len = KRun_calcNoteLen(tok->value->ptr, tr->l, skr->info->timebase);
  KRun_pushValue(skr, SValue_newInt(len));
  return S_TRUE;
}

s_bool KRun_writeMeta1b(SakuraObj *skr, KToken *tok, s_byte data1) {
  SValue *v;
  int no;
  KTrack *cur = SakuraObj_curTrack(skr);
  KSmfTrack *smf_tr;
  KSmfEvent *e;

  // get value
  v = KRun_evalTokenPop(skr, tok->arg);
  no = SValue_getInt(v);
  SValue_free(v);

  // memory to track info
  if (data1 == 0x21) { // PORT info
    cur->Port = no;
  }

  // write smf
  smf_tr = SakuraObj_curSmfTrack(skr);
  e = KSmfEvent_new();
  e->time = cur->time;
  e->status = 0xFF;
  e->data1 = data1;
  e->data2 = 0x01;
  e->meta = s_new_a(s_byte, 1);
  e->meta[0] = (s_byte)(0xF7 & no);
  KSmfTrack_append(smf_tr, e);

  return S_TRUE;
}

s_bool KRun_channelPrefix(SakuraObj *skr, KToken *tok) {
  return KRun_writeMeta1b(skr, tok, 0x20);
}

s_bool KRun_port(SakuraObj *skr, KToken *tok) {
  return KRun_writeMeta1b(skr, tok, 0x21);
}

s_bool KRun_timeSignature(SakuraObj *skr, KToken *tok) {
  int si, bo, cbo, i, n;
  KTrack *tr;
  KSmfTrack * smf_tr;
  KSmfEvent *e;

  // eval argments
  KRun_evalTokenList(skr, tok->arg);
  bo = KRun_popInt(skr);
  si = KRun_popInt(skr);

  skr->info->timeSig_nume = si;
  skr->info->timeSig_deno = bo;

  // calc bunbo
  i = 1;
  for (;;) {
    n = pow(2, i);
    if (n >= bo) break;
    i++;
  }
  cbo = i;

  //
  tr = SakuraObj_curTrack(skr);
  smf_tr = SakuraObj_curSmfTrack(skr);
  // make event
  e = KSmfEvent_new();
  e->time = tr->time;
  e->status = 0xFF;
  e->data1 = 0x58;
  e->data2 = 0x04;
  e->meta = s_new_a(s_byte, 4);
  e->meta[0] = si;
  e->meta[1] = cbo;
  e->meta[2] = skr->info->timebase;
  e->meta[3] = (s_byte)(skr->info->timebase / 8);
  KSmfTrack_append(smf_tr, e);
  //
  return S_TRUE;
}

s_bool KRun_sysex(SakuraObj *skr, KToken *tok) {
  KToken *arg = tok->arg;
  KTrack *cur = SakuraObj_curTrack(skr);
  KSmfTrack *smf_tr;
  KSmfEvent *e;
  s_byte buf[256]; // SysEx max length??
  SValue *val;
  int i = 0, n = 0, nn = 0, len = 0;
  s_bool is_count = S_FALSE;
  int counter = 0;

  memset(buf, 0, sizeof(buf)); // zero
  while (arg != NULL) {
    // for CHECK SUM
    if (arg->token_type == KTOKEN_NOP) {
      // top
    }
    else if (arg->token_type == KTOKEN_MARKER) {
      if (arg->no == 0x10) {
        is_count = S_TRUE;
        counter = 0;
      }
      else if (arg->no == 0x11) {
        is_count = S_FALSE;
        nn = 0x100 - (counter & 0xFF);
        buf[i++] = (s_byte)nn;
      }
    }
    else if (arg->token_type == KTOKEN_NUMBER) {
      buf[i++] = (s_byte)arg->no & 0xFF;
      if (is_count) { counter += arg->no; }
    } else {
      KRun_evalToken(skr, arg);
      val = KRun_popValue(skr);
      n = SValue_getInt(val);
      buf[i++] = (s_byte)n;
      if (is_count) { counter += n; }
      SValue_free(val);
    }
    arg = arg->next;
    if (i >= 255) break;
  }
  len = i;
  if (len == 0) return S_FALSE;

  if (skr->debug) {
    printf("SysEx$=");
    for (i = 0; i < len; i++) {
      printf("%02X,", buf[i]);
    }
    printf("\n");
  }

  smf_tr = SakuraObj_curSmfTrack(skr);
  e = KSmfEvent_new();
  e->time = cur->time;
  e->status = 0xF0;
  e->data1 = len - 1; // without top byte(0xF0)
  e->data2 = 0;
  e->meta = s_new_a(s_byte, len - 1);
  memcpy(e->meta, &buf[1], len - 1);
  KSmfTrack_append(smf_tr, e);

  return S_TRUE;
}


s_bool KRun_writeText(SakuraObj *skr, KToken *tok, s_byte meta_type) {
  SValue *text_v;
  SString *text;
  KTrack *cur = SakuraObj_curTrack(skr);
  KSmfTrack *smf_tr;
  KSmfEvent *e;

  text_v = KRun_evalTokenPop(skr, tok->arg);
  text = SValue_getString(text_v);
  SValue_free(text_v);

  // text
  smf_tr = SakuraObj_curSmfTrack(skr);
  e = KSmfEvent_new();
  e->time = cur->time;
  e->status = 0xFF;
  e->data1 = meta_type;
  e->data2 = text->len;
  e->meta = s_new_a(s_byte, text->len);
  memcpy(e->meta, text->ptr, text->len);
  KSmfTrack_append(smf_tr, e);
  SString_free(text);

  return S_TRUE;
}


s_bool KRun_metaText(SakuraObj *skr, KToken *tok) {
  return KRun_writeText(skr, tok, tok->no);
}


s_bool KRun_include(SakuraObj *skr, KToken *tok) {
  KFile *include_file;

  // find file
  include_file = tok->extra;

  return KRun_runFile(skr, include_file);
}

s_bool KRun_harmony(SakuraObj *skr, KToken *tok) {
  SString *harm_len;
  KFile *harm_file;
  KTrack *track;
  int len, tmp_len;
  s_bool result = S_TRUE;

  track = SakuraObj_curTrack(skr);
  harm_len = tok->value;
  harm_file = tok->extra;
  // memory current len
  tmp_len = track->l;
  len = KRun_calcNoteLen(harm_len->ptr, track->l, skr->info->timebase);
  // set harmony len
  track->l = len;
  skr->info->is_harmony = S_TRUE;
  result = KRun_runFile(skr, harm_file);
  track->time += len;
  skr->info->is_harmony = S_FALSE;
  track->l = tmp_len;

  return result;
}

s_bool KRun_div(SakuraObj *skr, KToken *tok) {
  SString *div_len;
  KFile *div_file;
  KTrack *track;
  int cur_time;
  int len, tmp_len, cnt;
  s_bool result = S_TRUE;

  track = SakuraObj_curTrack(skr);
  div_len = tok->value;
  div_file = tok->extra;
  cur_time = track->time;
  cnt = tok->no;
  // memory current len
  tmp_len = track->l;
  len = KRun_calcNoteLen(div_len->ptr, track->l, skr->info->timebase);
  // set div len
  track->l = len / cnt;
  result = KRun_runFile(skr, div_file);
  track->time = cur_time + len;
  track->l = tmp_len;

  return result;
}

s_bool KRun_rhythm(SakuraObj *skr, KToken *tok) {
  return KRun_runTokenList(skr, tok->cache);
}


s_bool KRun_key(SakuraObj *skr, KToken *tok) {
  SValue *v = KRun_evalTokenPop(skr, tok->arg);
  skr->info->key = SValue_getInt(v);
  SValue_free(v);
  return S_TRUE;
}

s_bool KRun_trackKey(SakuraObj *skr, KToken *tok) {
  SValue *v = KRun_evalTokenPop(skr, tok->arg);
  KTrack *tr = SakuraObj_curTrack(skr);
  tr->track_key = SValue_getInt(v);
  SValue_free(v);
  return S_TRUE;
}

s_bool KRun_print(SakuraObj *skr, KToken *tok) {
  SValue *v;
  SString *buf;

  // eval arugment
  v = KRun_evalTokenPop(skr, tok->arg);
  if (v == NULL) {
    k_error(skr, K_ERROR_SYNTAX, "Print Argument Error", tok->pos);
    return S_FALSE;
  }

  // print
  buf = SValue_getString(v);
  if (skr->use_print_console) {
    printf("PRINT> %s\n", buf->ptr);
  }
  SString_append(skr->print_buf, buf->ptr);
  SString_append(skr->print_buf, "\n");
  SString_free(buf);
  SValue_free(v);

  return S_TRUE;
}


s_bool KRun_if(SakuraObj *skr, KToken *tok) {
  SValue *v;
  KToken *expr_t = tok->arg;
  KTokenFlowOption *opt = tok->extra;
  s_bool res;

  v = KRun_evalTokenPop(skr, expr_t);
  res = SValue_getBool(v);
  SValue_free(v);

  if (res) {
    if (skr->debug) { printf("TRUE\n"); }
    KRun_runTokenList(skr, opt->true_t);
  } else {
    if (skr->debug) { printf("FALSE\n"); }
    if (opt->false_t != NULL) {
      KRun_runTokenList(skr, opt->false_t);
    }
  }

  return S_TRUE;
}

s_bool KRun_while(SakuraObj *skr, KToken *tok) {
  SValue *v;
  KToken *expr_t = tok->arg;
  KTokenFlowOption *opt = tok->extra;
  s_bool res;

  for (;;) {
    v = KRun_evalTokenPop(skr, expr_t);
    res = SValue_getBool(v);
    SValue_free(v);

    if (res) {
      if (skr->debug) { printf("TRUE\n"); }
      KRun_runTokenList(skr, opt->true_t);
    } else {
      k_run_next_token = opt->false_t;
      break;
    }
  }

  return S_TRUE;
}


s_bool KRun_defineINT(SakuraObj *skr, KToken *tok) {
  SString *vname;
  SValue *v;

  vname = tok->value;

  // eval params
  v = KRun_popValue(skr);
  if (skr->debug) {
    printf("INT_DEF %s=%d\n", vname->ptr, SValue_getInt(v));
  }
  // set to variables
  SHash_set(skr->variables, vname->ptr, v);

  return S_TRUE;
}

s_bool KRun_defineSTR(SakuraObj *skr, KToken *tok) {
  SString *vname;
  SValue *v;
  SString *tmp;

  vname = tok->value;

  // eval params
  v = SList_pop(skr->valueStack);
  if (skr->debug) {
    tmp = SValue_getString(v);
    printf("STR_DEF [%s]=%s\n", vname->ptr, tmp->ptr);
    SString_free(tmp);
  }
  // set to variable list
  SHash_set(skr->variables, vname->ptr, v);

  return S_TRUE;
}

s_bool KRun_defineARRAY(SakuraObj *skr, KToken *tok) {
  SValue *va, *vi;
  int sz, i;
  SString *vname;

  sz = KToken_count(tok->arg);
  // printf("[ARRAY count=%d]\n", sz);
  va = SValue_newArray();

  if (tok->arg != NULL) {
    // eval array list
    KRun_evalTokenList(skr, tok->arg);
    for (i = 0; i < sz; i++) {
      vi = KRun_popValue(skr);
      SValue_setArrayElement(va, (sz - i - 1), vi);
    }
  }
  // set to variable list
  vname = tok->value;
  SHash_set(skr->variables, vname->ptr, va);

  return S_TRUE;
}

s_bool KRun_doVariable(SakuraObj *skr, KToken *tok) {
  SValue *var_value;
  SString *src;
  char buf[1024];

  // (note) can not cache variable contents, maybe change
  // get variable object
  var_value = SHash_get(skr->variables, tok->value->ptr);
  if (var_value == NULL) {
    sprintf(buf, "Variable[%s] is not defined.", tok->value->ptr);
    k_warn(skr, K_WARN_VAR_NOT_DEFINE, buf, tok->pos);
    return S_TRUE; // not important
  }
  // check value pointer same?
  if (tok->cache != var_value) {
    tok->cache = var_value;
    // Extract String (Compile)
    src = SValue_getString(var_value);
    tok->extra = KParser_parseString(skr, src->ptr, tok->pos);
    SString_free(src);
  }
  // run
  KRun_runTokenList(skr, tok->extra);
  return S_TRUE;
}

s_bool KRun_goto(SakuraObj *skr, KToken *tok) {
  k_run_next_token = tok->arg;
  return S_TRUE;
}

s_bool KRun_goto_if_true(SakuraObj *skr, KToken *tok) {
  SValue *v;

  v = SList_pop(skr->valueStack);
  if (SValue_getBool(v)) {
    k_run_next_token = tok->arg;
  }

  return S_TRUE;
}

s_bool KRun_goto_if_false(SakuraObj *skr, KToken *tok) {
  SValue *v;

  v = SList_pop(skr->valueStack);
  if (!SValue_getBool(v)) {
    k_run_next_token = tok->arg;
  }

  return S_TRUE;

}

s_bool KRun_octaveOnetime(SakuraObj *skr, KToken *tok) {
  int n = tok->no;
  skr->info->octaveOnetime = n;
  return S_TRUE;
}

s_bool KRun_function_skip(SakuraObj *skr, KToken *tok) {
  KTokenFlowOption *opt;

  // skip define function
  opt = tok->extra;
  k_run_next_token = opt->false_t;

  return S_TRUE;
}

s_bool KRun_callUserFunc(SakuraObj *skr, KToken *tok) {
  int i, no;
  KFunction *func = (KFunction*)tok->extra;
  KFuncArg *arg;
  SValue *v;

  if (skr->debug) {
    printf("[callUserFunc:%s]\n", func->name);
  }

  // eval arguments
  if (tok->arg != NULL) {
    KRun_runTokenList(skr, tok->arg);
  }
  // register local variable
  for (i = 0; i < func->args_len; i++) {
    no = func->args_len - i - 1;
    arg = func->args[no];
    v = KRun_popValue(skr);
    if (v == NULL) {
      v = SValue_clone(arg->value);
      if (v == NULL) {
        k_error(skr, K_ERROR_ARG_COUNT, "not enough arguments", tok->pos);
        return S_FALSE;
      }
    }
    SHash_set(skr->local_vars, arg->name, v);
    if (skr->debug) {
      printf("[arg:%s=%d]",arg->name, SValue_getInt(v));
    }
  }
  // run
  KRun_runTokenList(skr, func->tokens);
  // clear local variable
  SHash_clear(skr->local_vars);

  return S_TRUE;
}

s_bool KRun_return(SakuraObj *skr, KToken *tok) {
  if (tok->arg != NULL) {
    KRun_evalTokenList(skr, tok->arg);
  }
  func_return_count++;
  return S_TRUE;
}

s_bool KRun_MML_r(SakuraObj *skr, KToken *tok) {
  char cmd[1024], buf[1024];
  KTrack *tr;

  // get command
  if (tok->value->len < 1024) {
    strcpy(cmd, tok->value->ptr);
  } else {
    strcpy(cmd, "");
    k_error(skr, K_ERROR_SYNTAX, "MML Command Too Long", tok->pos);
  }

  tr = SakuraObj_curTrack(skr);

  // Check Command Value
  if (strcmp(cmd, "@")==0) KRun_pushValue(skr, SValue_newInt(tr->Voice));
  else if (strcmp(cmd, "CH")==0) KRun_pushValue(skr, SValue_newInt(tr->ch));
  else if (strcmp(cmd, "Channel")==0) KRun_pushValue(skr, SValue_newInt(tr->ch));
  else if (strcmp(cmd, "TR")==0) KRun_pushValue(skr, SValue_newInt(skr->info->track_no));
  else if (strcmp(cmd, "Track")==0) KRun_pushValue(skr, SValue_newInt(skr->info->track_no));
  else if (strcmp(cmd, "l")==0) KRun_pushValue(skr, SValue_newInt(tr->l));
  else if (strcmp(cmd, "o")==0) KRun_pushValue(skr, SValue_newInt(tr->o));
  else if (strcmp(cmd, "q")==0) KRun_pushValue(skr, SValue_newInt(tr->q));
  else if (strcmp(cmd, "t")==0) KRun_pushValue(skr, SValue_newInt(tr->t));
  else if (strcmp(cmd, "TIME")==0) KRun_pushValue(skr, SValue_newInt(tr->time));
  else if (strcmp(cmd, "Time")==0) KRun_pushValue(skr, SValue_newInt(tr->time));
  else if (strcmp(cmd, "TrackKey")==0) KRun_pushValue(skr, SValue_newInt(tr->track_key));
  else if (strcmp(cmd, "v")==0) KRun_pushValue(skr, SValue_newInt(tr->v));
  else if (strcmp(cmd, "Key")==0) KRun_pushValue(skr, SValue_newInt(skr->info->key));
  else if (strcmp(cmd, "KEY")==0) KRun_pushValue(skr, SValue_newInt(skr->info->key));
  else if (strcmp(cmd, "PORT")==0) KRun_pushValue(skr, SValue_newInt(tr->Port));
  //TODO: MML(cmd) BR
  else if (strcmp(cmd, "BR")==0) KRun_pushValue(skr, SValue_newInt(tr->v));
  else {
    sprintf(buf, "MML(%s) command not found.", cmd);
    k_error(skr, K_ERROR_SYNTAX, buf, tok->pos);
    return S_FALSE;
  }
  return S_TRUE;
}

s_bool KRun_SizeOf_r(SakuraObj *skr, KToken *tok) {
  SValue *v, *n;
  v = KRun_evalTokenPop(skr, tok->arg);
  if (v->v_type == SVALUE_TYPE_ARRAY) {
    KRun_pushValue(skr, SValue_newInt(v->value.a->len));
  } else {
    n = SValue_newInt(SValue_getInt(v));
    KRun_pushValue(skr, n);
  }
  SValue_free(v);
  return S_TRUE;
}

s_bool KRun_keyflag(SakuraObj *skr, KToken *tok) {
  int i, nn;
  SValue *v;
  char *p;

  // set (a,b,c,d,e,f,g)
  if (tok->flag == 0) {
    KRun_evalTokenList(skr, tok->arg);
    for (i = 6; i >= 0; i--) {
      v = KRun_popValue(skr);
      skr->info->keyflag[i] = SValue_getInt(v);
      SValue_free(v);
    }
    return S_TRUE;
  }

  // set + - note
  for (i = 0; i < 7; i++) { // reset
    skr->info->keyflag[i] = 0;
  }
  //
  p = tok->value->ptr;
  while (*p != '\0') {
    nn = (int)(*p - 'a');
    if (0 <= nn && nn <= 6) {
      skr->info->keyflag[nn] = tok->flag;
    }
    p++;
  }
  return S_TRUE;
}

s_bool KRun_v_onNote(SakuraObj *skr, KToken *tok) {
  KTrack *tr = SakuraObj_curTrack(skr);
  // eval
  KRun_evalTokenList(skr, tok->arg);
  SList_free(tr->v_onNote); // clear .onNote
  tr->v_onNote = SList_popNum(skr->valueStack, tok->no);
  tr->v_onNote->free_item_f = SValue_free_retTrue;
  //
  return S_TRUE;
}

s_bool KRun_t_onNote(SakuraObj *skr, KToken *tok) {
  KTrack *tr = SakuraObj_curTrack(skr);
  // eval
  KRun_evalTokenList(skr, tok->arg);
  SList_free(tr->t_onNote); // clear .onNote
  tr->t_onNote = SList_popNum(skr->valueStack, tok->no);
  tr->t_onNote->free_item_f = SValue_free_retTrue;
  //
  return S_TRUE;
}

s_bool KRun_trackSync(SakuraObj *skr, KToken *tok) {
  int i, cur_time;
  KTrack *tr, *t;
  // get current time
  tr = SakuraObj_curTrack(skr);
  cur_time = tr->time;
  // all tracks set same time
  for (i = 0; i < skr->tracks->len; i++) {
    t = skr->tracks->ptr[i];
    t->time = cur_time;
  }
  // set sync time
  skr->info->timeTrackSync = cur_time;
  if (skr->debug) {
    printf("TrackSync=%d\n", cur_time);
  }
  //
  return S_TRUE;
}

s_bool KRun_callSysFunc(SakuraObj *skr, KToken *tok) {
  KRun_Command runcmd;
  runcmd = (KRun_Command)tok->extra;
  return runcmd(skr, tok);
}

s_bool KRun_trackSub(SakuraObj *skr, KToken *tok) {
  s_bool r;
  int time;
  KTrack *tr;
  //
  tr = SakuraObj_curTrack(skr);
  time = tr->time;
  r = KRun_runTokenList(skr, tok->arg);
  tr->time = time;
  //
  return r;
}

s_bool KRun_vAdd(SakuraObj *skr, KToken *tok) {
  int v;
  KTrack *tr;
  //
  KRun_evalToken(skr, tok->arg);
  v = KRun_popInt(skr);
  tr = SakuraObj_curTrack(skr);
  tr->v = v;
  //
  return S_TRUE;
}

s_bool KRun_sharp_macro_define(SakuraObj *skr, KToken *tok) {
  SValue *mml;

  KRun_evalToken(skr, tok->arg);
  mml = KRun_popValue(skr);
  SHash_set(skr->variables, tok->value->ptr, mml);

  return S_TRUE;
}

s_bool KRun_sharp_macro_exec(SakuraObj *skr, KToken *tok) {
  SValue *mml_v, *v;
  SString *mml, *s, *tmp;
  int no, i;
  char keyname[256];
  KToken *t;
  // get variable
  mml_v = SHash_get(skr->variables, tok->value->ptr);
  if (mml_v == NULL) {
    return S_TRUE;
  }
  mml = SValue_getString(mml_v);
  // has parameters?
  if (tok->no > 0) {
    KRun_evalTokenList(skr, tok->arg);
    for (i = 0; i < tok->no; i++) {
      no = tok->no - i;
      sprintf(keyname, "#?%d", no);
      v = KRun_popValue(skr);
      s = SValue_getString(v);
      SValue_free(v);
      tmp = SString_replace(mml, keyname, s->ptr);
      SString_free(mml);
      SString_free(s);
      mml = tmp;
    }
  }
  // compile
  t = KParser_parseString(skr, mml->ptr, tok->pos);
  SString_free(mml);
  return KRun_runTokenList(skr, t);
}

s_bool KRun_playFrom(SakuraObj *skr, KToken *tok) {
  KTrack *tr;
  int time;
  if (tok->arg != NULL) {
    KRun_evalToken(skr, tok->arg);
    time = KRun_popInt(skr);
  } else {
    tr = SakuraObj_curTrack(skr);
    time = tr->time;
  }
  skr->play_from = time;
  if (skr->debug) {
    printf("[PlayFrom] %d\n", time);
  }
  return S_TRUE;
}

s_bool KRun_time_r(SakuraObj *skr, KToken *tok) {
  KTrack *tr;
  //
  tr = SakuraObj_curTrack(skr);
  KRun_pushValue(skr, SValue_newInt(tr->time));
  //
  return S_TRUE;
}

s_bool KRun_CCUpper(SakuraObj *skr, KToken *tok) {
  int value;
  KSmfTrack *smf_tr;
  KSmfEvent *e;
  KTrack *tr;
  //
  KRun_evalToken(skr, tok->arg);
  value = KRun_popInt(skr);
  tr = SakuraObj_curTrack(skr);
  smf_tr = SakuraObj_curSmfTrack(skr);
  //
  e = KSmfEvent_newCC(tr->time, tr->ch, tok->no, value);
  KSmfTrack_append(smf_tr, e);
  //
  return S_TRUE;
}

s_bool KRun_mid_r(SakuraObj *skr, KToken *tok) {
  SValue *sv;
  int index, len;
  SString *s, *res;

  // get params
  KRun_evalTokenList(skr, tok->arg);
  len = KRun_popInt(skr);
  index = KRun_popInt(skr);
  sv = KRun_popValue(skr);
  s = SValue_getString(sv);
  SValue_free(sv);

  // mid
  res = SString_substr(s, index - 1, len);
  SString_free(s);
  KRun_pushValue(skr, SValue_newString(res->ptr));
  SString_free(res);

  return S_TRUE;
}

s_bool KRun_asc_r(SakuraObj *skr, KToken *tok) {
  SValue *v;
  SString *s;
  char c;
  // pop argument
  v = KRun_evalTokenPop(skr, tok->arg);
  s = SValue_getString(v);
  c = *s->ptr;
  KRun_pushValue(skr, SValue_newInt((int)c));
  SValue_free(v);
  SString_free(s);
  return S_TRUE;
}

s_bool KRun_strtonum_r(SakuraObj *skr, KToken *tok) {
  SValue *v;
  int n;
  // pop argument
  v = KRun_evalTokenPop(skr, tok->arg);
  n = SValue_getInt(v);
  KRun_pushValue(skr, SValue_newInt(n));
  SValue_free(v);
  return S_TRUE;
}

s_bool KRun_NoteNo_r(SakuraObj *skr, KToken *tok) {
  SValue *v;
  SString *s;
  int o;
  KTrack *tr;
  char *p;
  int notechar, noteno;

  // pop argument
  v = KRun_evalTokenPop(skr, tok->arg);
  s = SValue_getString(v);
  // get default value
  tr = SakuraObj_curTrack(skr);
  o = tr->o;
  noteno = 0;
  p = s->ptr;
  while (*p != '\0') {
    if (*p == 'o') {
      p++;
      o = s_readInt(p, o, &p);
      continue;
    }
    if ('a' <= *p && *p <= 'g') {
      notechar = (*p - 'a');
      noteno = o * 12 + NOTE_TONE[notechar];
      noteno += skr->info->keyflag[notechar];
      p++;
      continue;
    }
    if (*p == '+' || *p == '#') { noteno++; p++; }
    if (*p == '-') { noteno--; }
    if (*p == 'n') {
      p++;
      noteno = s_readInt(p, 0, &p);
      continue;
    }
    p++; // other chars
  }
  // shift
  noteno += tr->track_key;
  noteno += skr->info->key;
  // TODO: timekey flag
  //
  KRun_pushValue(skr, SValue_newInt(noteno));
  SValue_free(v);
  return S_TRUE;
}

s_bool KRun_array_element(SakuraObj *skr, KToken *tok) {
  SValue *a, *v;
  int i;

  KRun_evalToken(skr, tok->arg);
  i = KRun_popInt(skr);
  a = KRun_popValue(skr);
  v = SValue_getArrayElement(a, i);
  KRun_pushValue(skr, SValue_clone(v));
  SValue_free(a);

  return S_TRUE;
}

s_bool KRun_var_replace(SakuraObj *skr, KToken *tok) {
  SValue *v;
  SString *vname = tok->value;
  SValue *key, *rep;
  SString *s = NULL, *key_s = NULL, *rep_s = NULL, *res = NULL;
  s_bool result = S_TRUE;
  // get parameters
  KRun_evalTokenList(skr, tok->arg);
  rep = KRun_popValue(skr);
  key = KRun_popValue(skr);
  // get variable
  v = SHash_get(skr->local_vars, vname->ptr);
  if (v == NULL) {
    v = SHash_get(skr->variables, vname->ptr);
  }
  if (v == NULL) {
    result = S_FALSE;
    goto lbl_free;
  }
  //
  s = SValue_getString(v);
  key_s = SValue_getString(key);
  rep_s = SValue_getString(rep);
  res = SString_replace(s, key_s->ptr, rep_s->ptr);
  SValue_setString(v, res->ptr);

lbl_free:
  SString_free(key_s);
  SString_free(rep_s);
  SString_free(res);
  SString_free(s);
  SValue_free(key);
  SValue_free(rep);
  //
  return result;
}

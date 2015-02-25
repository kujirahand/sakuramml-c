/**
 * Parser for MML
 * @file k_parser.c
 */

#include <string.h>
#include <stdio.h>

#include "s_lib.h"
#include "k_token.h"
#include "k_main.h"
#include "k_parser.h"
#include "k_run.h"
#include "k_function.h"

SHashNode *KParser_last_command = NULL;

/**
 * Register Upper Case Command
 * @return
 */
void KParser_appendCommand(SHash *hash) {

  // TODO: @Add Command
  SHash_set(hash, "TR", KParser_track);
  SHash_set(hash, "Track", KParser_track);
  SHash_set(hash, "CH", KParser_channel);
  SHash_set(hash, "Channel", KParser_channel);
  SHash_set(hash, "Include", KParser_include);
  SHash_set(hash, "INCLUDE", KParser_include);
  SHash_set(hash, "Div", KParser_div);
  SHash_set(hash, "DIV", KParser_div);
  SHash_set(hash, "Rythm", KParser_rhythm); // for sakura ver.1
  SHash_set(hash, "RYTHM", KParser_rhythm); // 
  SHash_set(hash, "Rhythm", KParser_rhythm);
  SHash_set(hash, "RHYTHM", KParser_rhythm);
  SHash_set(hash, "Print", KParser_print);
  SHash_set(hash, "PRINT", KParser_print);
  SHash_set(hash, "MsgBox", KParser_print);
  SHash_set(hash, "Time", KParser_time);
  SHash_set(hash, "TIME", KParser_time);
  SHash_set(hash, "System", KParser_system);
  SHash_set(hash, "Timebase", KParser_timebase);
  SHash_set(hash, "TimeBase", KParser_timebase);
  SHash_set(hash, "TIMEBASE", KParser_timebase);
  SHash_set(hash, "Key", KParser_key);
  SHash_set(hash, "TrackKey", KParser_trackKey);
  SHash_set(hash, "MeasureShift", KParser_measureShift);
  SHash_set(hash, "KeyFlag", KParser_keyflag);
  SHash_set(hash, "PitchBend", KParser_pitchBend);
  SHash_set(hash, "Sub", KParser_trackSub);
  SHash_set(hash, "SUB", KParser_trackSub);
  SHash_set(hash, "S",   KParser_trackSub);
  // Script variable
  SHash_set(hash, "Int", KParser_int);
  SHash_set(hash, "INT", KParser_int);
  SHash_set(hash, "Str", KParser_str);
  SHash_set(hash, "STR", KParser_str);
  SHash_set(hash, "Array", KParser_array);
  SHash_set(hash, "ARRAY", KParser_array);
  // Script flow
  SHash_set(hash, "If", KParser_if);
  SHash_set(hash, "IF", KParser_if);
  SHash_set(hash, "While", KParser_while);
  SHash_set(hash, "WHILE", KParser_while);
  SHash_set(hash, "Function", KParser_defineUserFunc);
  SHash_set(hash, "FUNCTION", KParser_defineUserFunc);
  SHash_set(hash, "Return", KParser_return);
  SHash_set(hash, "RETURN", KParser_return);
  SHash_set(hash, "TrackSync", KParser_trackSync);
  SHash_set(hash, "TimeKeyFlag", KParser_timekeyflag);
  // Meta event
  SHash_set(hash, "Tempo", KParser_tempo);
  SHash_set(hash, "TEMPO", KParser_tempo);
  SHash_set(hash, "ChannelPrefix", KParser_ChannelPrefix);
  SHash_set(hash, "Port", KParser_port);
  SHash_set(hash, "PORT", KParser_port);
  SHash_set(hash, "TimeSignature", KParser_timeSignature);
  // Meta text event
  SHash_setTag(hash, "MetaText",        KParser_metaText, 0x01);
  SHash_setTag(hash, "Copyright",       KParser_metaText, 0x02);
  SHash_setTag(hash, "TrackName",       KParser_metaText, 0x03);
  SHash_setTag(hash, "InstrumentName",  KParser_metaText, 0x04);
  SHash_setTag(hash, "Lyric",           KParser_metaText, 0x05);
  SHash_setTag(hash, "Marker",          KParser_metaText, 0x06);
  SHash_setTag(hash, "CuePoint",        KParser_metaText, 0x07);
  SHash_setTag(hash, "ProgramName",     KParser_metaText, 0x08);
  SHash_setTag(hash, "DeviceName",      KParser_metaText, 0x09);
  // SysEx
  SHash_set(hash, "SysEx", KParser_sysex);
  SHash_set(hash, "SYSEX", KParser_sysex);
  SHash_set(hash, "SysEx$", KParser_sysex_hex);
  SHash_set(hash, "SYSEX$", KParser_sysex_hex);
  // RPN/NRPN
  SHash_set(hash, "RPN", KParser_rpn);
  SHash_set(hash, "NRPN", KParser_nrpn);
  // CC
  SHash_set(hash, "CHO", KParser_chorus);
  SHash_set(hash, "Chorus", KParser_chorus);
  SHash_set(hash, "REV", KParser_reverb);
  SHash_set(hash, "Reverb", KParser_reverb);
  SHash_set(hash, "EP", KParser_expression);
  SHash_set(hash, "Expression", KParser_expression);
  SHash_set(hash, "P", KParser_panpot);
  SHash_set(hash, "Panpot", KParser_panpot);
  SHash_set(hash, "M", KParser_modulation);
  SHash_set(hash, "Modulation", KParser_modulation);
  SHash_set(hash, "V", KParser_volume);
  SHash_set(hash, "MainVolume", KParser_volume);
  SHash_set(hash, "VAR", KParser_var_effect);
  // FOR SMF OUTPUT
  SHash_set(hash, "PlayFrom", KParser_playFrom);
}

void KParser_appendReaderCommand(SHash *hash) {
  SHash_set(hash, "Tempo", SValue_newSysFunc(KRun_tempo_r));
  SHash_set(hash, "Track", SValue_newSysFunc(KRun_track_r));
  SHash_set(hash, "TR", SValue_newSysFunc(KRun_track_r));
  SHash_set(hash, "Channel", SValue_newSysFunc(KRun_channel_r));
  SHash_set(hash, "CH", SValue_newSysFunc(KRun_channel_r));
  SHash_set(hash, "MML", SValue_newSysFunc(KRun_MML_r));
  SHash_set(hash, "SizeOf", SValue_newSysFunc(KRun_SizeOf_r));
  SHash_set(hash, "SIZEOF", SValue_newSysFunc(KRun_SizeOf_r));
  SHash_set(hash, "VERSION", SValue_newSysFunc(KRun_version_r));
  SHash_set(hash, "VERSION", SValue_newSysFunc(KRun_version_r));
  SHash_set(hash, "Random", SValue_newSysFunc(KRun_random_r));
  SHash_set(hash, "Step", SValue_newSysFunc(KRun_step_r));
  SHash_set(hash, "STEP", SValue_newSysFunc(KRun_step_r));
  SHash_set(hash, "Time", SValue_newSysFunc(KRun_time_r));
  SHash_set(hash, "TIME", SValue_newSysFunc(KRun_time_r));
  SHash_set(hash, "MID", SValue_newSysFunc(KRun_mid_r));
  SHash_set(hash, "ASC", SValue_newSysFunc(KRun_asc_r));
  SHash_set(hash, "StrToNum", SValue_newSysFunc(KRun_strtonum_r));
  SHash_set(hash, "NoteNo", SValue_newSysFunc(KRun_NoteNo_r));
}



/** read Lowercase Command */
s_bool KParser_readLowerCommand(SakuraObj *skr, KFile *file) {

  switch (*file->pos) {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g': return KParser_readNoteOn(skr, file);
    case 'n': return KParser_readNoteOnNo(skr, file);
    case 'r':
    case '_': return KParser_readRest(skr, file);
    case 'l': return KParser_readCommandL(skr, file);
    case 'o': return KParser_readCommandO(skr, file);
    case 'q': return KParser_readCommandQ(skr, file);
    case 't': return KParser_readCommandT(skr, file);
    case 'v': return KParser_readCommandV(skr, file);
    case 'y': return KParser_readCC(skr, file);
    default:
      k_error(skr, K_ERROR_SYNTAX,
        "Syntax Error: unknown Command", file->pos);
      break;
  }
  file->pos++;
  return S_TRUE;
}

/** read Flag Command */
s_bool KParser_readFlagCommand(SakuraObj *skr, KFile *file) {
  char buf[64];
  // flag
  switch (*file->pos) {
    case '>':
    case '<': return KParser_readOctaveCommand(skr, file);
    case '[':
    case ']':
    case ':': return KParser_readLoop(skr, file);
    case '|': return KParser_readSplitter(skr, file);
    case '@': return KParser_readVoice(skr, file);
    case '/': return KParser_backslash(skr, file);
    case '\'':return KParser_harmony(skr, file);
    case '`':
    case '"': return KParser_octaveOnetime(skr, file);
    case '#': return KParser_readSharpMacro(skr, file);
    case ';':
      file->pos++;
      break;
    case '$': return KParser_rhythm_define(skr, file);
    case '?': file->pos++; return KParser_playFromNow(skr, file);
    default:
      sprintf(buf, "Unknown Command ('%c':0x%02X)", *file->pos, (unsigned char)*file->pos);
      k_error(skr, K_ERROR_SYNTAX, buf, file->pos);
      file->pos++;
  }
  return S_TRUE;
}


/**
 * Parse mml file (main)
 */
s_bool KParser_parse(SakuraObj *skr, KFile *file) {
  unsigned char rch;
  char bom[3] = {0xEF, 0xBB, 0xBF};
  char buf[128], tmp[32];
  size_t sz;

  // check UTF-8 BOM
  if (memcmp(bom, file->pos, 3) == 0) {
    file->pos += 3;
  }
  // loop read
  for (;;) {
    SKIP_SPACE_CRLF(file->pos);
    rch = (unsigned char)*file->pos;
    // if (skr->debug) { putchar(rch); }
    if (rch == 0) break;
    // Lower command
    if (IN_RANGE('a', 'z', rch)) {
      KParser_readLowerCommand(skr, file);
    }
    // Upper command
    else if (IN_RANGE('A', 'Z', rch) || rch == '_') {
      KParser_readUpperCommand(skr, file);
    }
    // flag command
    else if (rch < 0x80) {
      KParser_readFlagCommand(skr, file);
    }
    // Multibytes command
    else {
      sz = s_utf8_width(file->pos);
      strcpy(buf, "Unknown Multibyte Char=");
      s_strlcpy(tmp, file->pos, sz);
      strcat(buf, tmp);
      sprintf(tmp, "(0x%02X)", rch);
      strcat(buf, tmp);
      k_error(skr, K_ERROR_SYNTAX, buf, file->pos);
      file->pos++;
    }
  }
  // debug
  if (skr->debug) {
    KToken_printAll(file->token_top);
  }
  return S_TRUE;
}


void KParser_freeCommandHash(SHash *hash) {
  SHash_free(hash);
}


/** Read Number */
SString *KParser_readNum(SakuraObj *skr, KFile *file) {
  char buf[256];
  char *p;
  SString *str;
  size_t len;

  if (!IS_NUMERIC_OR_FLAG(*file->pos)) return NULL;
  p = file->pos;
  // Numeric or Flag
  if (IS_NUMERIC_OR_FLAG(*file->pos)) {
    file->pos++;
  }
  while (IS_NUMERIC(*file->pos)) {
    file->pos++;
  }
  len = file->pos - p + 1;
  if (len > 255) {
    k_error(skr, K_ERROR_SYNTAX, "Numeric is too big.", file->pos);
    return NULL;
  }
  s_strlcpy(buf, p, (file->pos - p) + 1);
  str = SString_new(buf);
  return str;
}


/** Read Note Length  */
SString *KParser_readLen(SakuraObj *skr, KFile *file) {
  SString *s = SString_new(NULL);

  // step type?
  if (*file->pos == '%') {
    SString_appendChar(s, '%');
    file->pos++;
    if (!IS_NUMERIC(*file->pos)) {
      skr->error_stop = S_TRUE;
      return S_FALSE;
    }
    while (IS_NUMERIC(*file->pos)) {
      SString_appendChar(s, *file->pos);
      file->pos++;
    }
    return s;
  }

  // n-onpu type
  for (;;) {
    SKIP_SPACE_N_SPLITEER(file->pos);
    if (IS_NUMERIC(*file->pos) ||
        *file->pos == '^' ||
        *file->pos == '.') {
      SString_appendChar(s, *file->pos);
      file->pos++;
      continue;
    }
    break;
  }
  return s;
}

/**
 * NoteOn
 */
s_bool KParser_readNoteOn(SakuraObj *skr, KFile *file) {
  KToken *t;
  SString *len;
  int natural = 0;

  // Token
  t = KToken_new(KTOKEN_NOTE, file->pos);
  KFile_appendToken(file, t);
  // Note Name
  t->no = (int)*file->pos;
  file->pos++;
  // Check natural
  if (*file->pos == '*') {
    file->pos++;
    natural = 0x100;
  }
  // Check Sharp
  t->flag = 0;
  for (;;) {
    if (*file->pos == '+' || *file->pos == '#') {
      t->flag++;
    } else if (*file->pos == '-') {
      t->flag--;
    } else {
      break;
    }
    file->pos++;
  }
  t->flag = t->flag | natural; // for Natural & Sharp
  // length
  len = KParser_readLen(skr, file);
  t->value = len;
  // velocity etc
  if (*file->pos == ',') {
    file->pos++;
    t->arg = KParser_readValueList(skr, file);
  }
  // tie
  if (*file->pos == '&') {
    file->pos++;
  }
  
  return S_TRUE;
}

s_bool KParser_readNoteOnNo(SakuraObj *skr, KFile *file) {
  KToken *t;
  SString *len;

  file->pos++; // skip 'n'
  // Token
  t = KToken_new(KTOKEN_NOTE_NO, file->pos);
  KFile_appendToken(file, t);
  // Read Note No
  t->arg = KParser_readValue(skr, file);
  // length
  if (*file->pos == ',') {
    file->pos++; // skip comma
    len = KParser_readLen(skr, file);
    t->value = len;
  }
  // velocity etc
  if (*file->pos == ',') {
    file->pos++;
    t->arg = KParser_readValueList(skr, file);
  }
  // tie
  if (*file->pos == '&') {
    file->pos++;
  }
  return S_TRUE;
}

/** read rest */
s_bool KParser_readRest(SakuraObj *skr, KFile *file) {
  KToken *t;

  // skip command
  if (*file->pos == 'r' || *file->pos == '_') {
    file->pos++;
  }
  // add to token list
  t = KToken_new(KTOKEN_REST, file->pos);
  KFile_appendToken(file, t);
  // check event flag
  if (*file->pos == '*') { // (note on event) is off
    file->pos++;
    t->no = 0; // note on event off
  } else {
    t->no = 1; // note on event on
  }
  // check length
  if (*file->pos == '-') { // minus dir
    file->pos++;
    t->flag = -1;
  }
  else if (*file->pos == '+') {
    file->pos++;
    t->flag = 1;
  }
  t->value = KParser_readLen(skr, file);
  //
  return S_TRUE;
}

s_bool KParser_readCommandL(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_COMMAND_L, file->pos);
  if (*file->pos == 'l') file->pos++;
  KFile_appendToken(file, t);

  // check len
  t->value = KParser_readLen(skr, file);

  return S_TRUE;
}

int KParser_readFlag(KFile *file) {
  if (*file->pos == '+') {
    file->pos++;
    return 1;
  }
  else if (*file->pos == '-') {
    file->pos++;
    return -1;
  }
  return 0;
}

s_bool KParser_readCommandOption(SakuraObj *skr, KFile *file, int ttype, int *num) {
  char *p;
  char buf[256];
  int n = 0;
  KToken *t;

  // read num
  if (memcmp(file->pos, "__", 2) == 0) {
    file->pos += 2;
    n = s_readInt(file->pos, 0, &file->pos);
  }
  else if (*file->pos == '_') {
    file->pos++;
  }
  *num = n;
  
  // .onNote
  if (*file->pos == '.') {
    file->pos++;
    p = KParser_readVarNameStr(skr, file);
    strcpy(buf, p);
    s_free(p);
    if (strcmp(buf, "onNote") == 0 || strcmp(buf, "N") == 0) {
      t = KToken_new(ttype, file->pos);
      t->arg = KParser_readValueList(skr, file);
      t->no = KToken_count(t->arg);
      KFile_appendToken(file, t);
      return S_TRUE;
    }
    if (strcmp(buf, "onTime") == 0 || strcmp(buf, "T") == 0) {
      // TODO: v.onTime
      return S_TRUE;
    }
    if (strcmp(buf, "onCycle") == 0 || strcmp(buf, "C") == 0) {
      // TODO: v.onTime
      return S_TRUE;
    }
    if (strcmp(buf, "Random") == 0) {
      // TODO: v.onRandom
      return S_TRUE;
    }
  }
  return S_FALSE;
}


s_bool KParser_readCommandO(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_COMMAND_O, file->pos);
  if (*file->pos == 'o') file->pos++;
  KFile_appendToken(file, t);

  // check flag
  t->flag = KParser_readFlag(file);
  // check num
  t->value = KParser_readNum(skr, file);
  return S_TRUE;
}

s_bool KParser_readCommandQ(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_COMMAND_Q, file->pos);
  if (*file->pos == 'q') file->pos++;
  KFile_appendToken(file, t);

  // check flag
  t->flag = KParser_readFlag(file);
  // check num
  t->value = KParser_readNum(skr, file);
  return S_TRUE;
}

s_bool KParser_readCommandT(SakuraObj *skr, KFile *file) {
  KToken *t;
  s_bool b_read_opt;
  int num;

  if (*file->pos == 't') file->pos++;
  
  // option
  b_read_opt = KParser_readCommandOption(skr, file, KTOKEN_T_ON_NOTE, &num);
  if (b_read_opt) {
    return S_TRUE;
  }

  t = KToken_new(KTOKEN_COMMAND_T, file->pos);
  KFile_appendToken(file, t);

  // check flag
  t->flag = KParser_readFlag(file);
  // check num
  t->value = KParser_readNum(skr, file);
  return S_TRUE;
}

s_bool KParser_readCommandV(SakuraObj *skr, KFile *file) {
  KToken *t;
  int num;
  s_bool b_read_opt;

  if (*file->pos == 'v') file->pos++;
  
  // option
  b_read_opt = KParser_readCommandOption(skr, file, KTOKEN_T_ON_NOTE, &num);
  if (b_read_opt) {
    return S_TRUE;
  }
  
  // normal
  t = KToken_new(KTOKEN_COMMAND_V, file->pos);
  KFile_appendToken(file, t);
  // check inc/dec
  t->flag = 0;
  for(;;) {
    if (*file->pos == '+') {
      t->flag++;
      file->pos++;
      continue;
    }
    else if (*file->pos == '-') {
      t->flag--;
      file->pos++;
      continue;
    }
    break;
  }
  if (*file->pos == '(') {
    t->arg = KParser_readValue(skr, file);
  } else {
    t->no = s_readInt(file->pos, -1, &file->pos);
  }
  KFile_appendToken(file, t);

  return S_TRUE;
}



/** read Octave Command */
s_bool KParser_readOctaveCommand(SakuraObj *skr, KFile *file) {
  KToken *t;
  char flag;

  flag = *file->pos;
  file->pos++;

  t = KToken_new(KTOKEN_OCTAVE_UD, file->pos);
  KFile_appendToken(file, t);

  t->flag = (flag == '>') ? 1 : -1;
  if (skr->info->revOctaveFlag) t->flag *= -1;

  return S_TRUE;
}

/** read Uppercase Command */
s_bool KParser_readUpperCommand(SakuraObj *skr, KFile *file) {
  char *p, *pTop;
  char cmd[256];
  long len;
  KParserCommand f;
  KToken *t;
  SHashNode *node;
  SValue *sv;

  // cut command
  pTop = p = file->pos;
  while (IS_ALPHA(*p) || IS_NUMERIC(*p) || *p == '_' || *p == '$') {
    p++;
  }
  len = (p - file->pos) + 1;
  if (len > 255) {
    k_error(skr, K_ERROR_SYNTAX, "Command name too long", file->pos);
    file->pos++;
    return S_FALSE;
  }
  s_strlcpy(cmd, file->pos, len);
  file->pos = p;

  // check function
  node = Shash_getNode(skr->command_hash, cmd);
  KParser_last_command = node;
  if (node == NULL) {
    // LET COMMAND?
    SKIP_SPACE(file->pos);
    // FUNCTION ?
    sv = SHash_get(skr->variables, cmd);
    if (sv != NULL && sv->v_type == SVALUE_TYPE_USER_FUNC) {
      // printf("USER_FUNC:%s\n", cmd);
      return KParser_callUserFunc(skr, file, sv->value.f);
    }
    // maybe let variable
    if (*file->pos == '=') {
      file->pos = pTop;
      return KParser_int(skr, file);
    }
    // directly calc
    if (memcmp(file->pos, "-=", 2) == 0 || memcmp(file->pos, "+=", 2) == 0 ||
        memcmp(file->pos, "++", 2) == 0 || memcmp(file->pos, "--", 2) == 0) {
      file->pos = pTop;
      return KParser_int(skr, file);
    }
    // replace command
    if (memcmp(file->pos, ".s", 2) == 0) {
      file->pos += 2;
      t = KToken_new(KTOKEN_VAR_REPLACE, file->pos);
      t->value = SString_new(cmd);
      t->arg = KParser_readValueList(skr, file);
      t->no = KToken_count(t->arg);
      if (t->no != 2) {
        k_error(skr, K_ERROR_ARG_COUNT, "(VAR).s(key, replace)", file->pos);
        KToken_free(t);
        return S_TRUE;
      }
      KFile_appendToken(file, t);
      return S_TRUE;
    }
    // maybe variable
    t = KToken_new(KTOKEN_DO_VARIABLE, file->pos);
    t->value = SString_new(cmd);
    KFile_appendToken(file, t);
    return S_TRUE;
  }

  // run read function
  f = node->value;
  return f(skr, file);
}


s_bool KParser_readSplitter(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_SPLITTER, file->pos);
  KFile_appendToken(file, t);
  file->pos++; // skip '|'

  return S_TRUE;
}

s_bool KParser_readLoop(SakuraObj *skr, KFile *file) {
  char flag;
  KToken *t = NULL;

  flag = *file->pos;
  file->pos++;
  switch (flag) {
    case '[':
      t = KToken_new(KTOKEN_LOOP_BEGIN, file->pos);
      if (*file->pos == '(' || IS_NUMERIC(*file->pos)) {
        t->arg = KParser_readValue(skr, file);
      }
      break;
    case ']':
      t = KToken_new(KTOKEN_LOOP_END, file->pos);
      break;
    case ':':
      t = KToken_new(KTOKEN_LOOP_BREAK, file->pos);
      break;
  }
  if (t == NULL) return S_FALSE;
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_readCC(SakuraObj *skr, KFile *file) {
  KToken *t;
  KToken *cno, *cv;
  //
  file->pos++; // skip 'y'
  t = KToken_new(KTOKEN_CC, file->pos);
  cno = KParser_readValue(skr, file);
  SKIP_SPACE(file->pos);
  if (*file->pos == ',') {
    file->pos++;
  } else if (*file->pos == '(') {
  } else {
    k_error(skr, K_ERROR_ARG_COUNT, "y(CC),(Value)", file->pos);
    KToken_free(t);
    return S_FALSE;
  }
  cv = KParser_readValue(skr, file);
  if (cv == NULL) {
    k_error(skr, K_ERROR_SYNTAX, "Command 'y' not enogh arguments.", file->pos);
    return S_FALSE;
  }
  
  t->arg = cno;
  t->arg->next = cv;

  KFile_appendToken(file, t);
  return S_TRUE;
}

s_bool KParser_rpn(SakuraObj *skr, KFile *file) {
  KToken *t;
  //
  t = KToken_new(KTOKEN_RPN, file->pos);
  t->arg = KParser_readValueList(skr, file);
  t->no = KToken_count(t->arg);
  if (t->arg == NULL || t->no < 3) {
    k_error(skr, K_ERROR_SYNTAX, "'RPN' not enogh arguments.", file->pos);
    return S_FALSE;
  }
  KFile_appendToken(file, t);
  //
  return S_TRUE;
}

s_bool KParser_nrpn(SakuraObj *skr, KFile *file) {
  KToken *t;
  //
  t = KToken_new(KTOKEN_NRPN, file->pos);
  t->arg = KParser_readValueList(skr, file);
  t->no = KToken_count(t->arg);
  if (t->arg == NULL || t->no < 3) {
    k_error(skr, K_ERROR_SYNTAX, "'NRPN' not enogh arguments.", file->pos);
    return S_FALSE;
  }
  KFile_appendToken(file, t);
  //
  return S_TRUE;
}

/**
 * read value list
 */
KToken *KParser_readValueList(SakuraObj *skr, KFile *file) {
  KToken *top_t, *cur_t, *t;
  s_bool use_paren = S_FALSE;

  SKIP_SPACE(file->pos);
  if (*file->pos == '=') {
    file->pos++;
    SKIP_SPACE(file->pos);
  }
  else if (*file->pos == '(') {
    file->pos++;
    SKIP_SPACE(file->pos);
    if (*file->pos == ')') {
      file->pos++;
      return KToken_new(KTOKEN_NOP, file->pos);
    }
    use_paren = S_TRUE;
  }

  top_t = cur_t = NULL;
  for (;;) {
    t = KParser_readValue(skr, file);
    if (top_t == NULL) {
      top_t = cur_t = t;
    } else {
      cur_t->next = t;
      cur_t = t;
    }
    SKIP_SPACE(file->pos);
    if (*file->pos == ',' || *file->pos == ':') {
      file->pos++;
      SKIP_SPACE(file->pos);
      continue;
    }
    break;
  }
  
  if (use_paren) {
    if (*file->pos == ')') {
      file->pos++;
      SKIP_SPACE(file->pos);
    }
  }

  return top_t;
}

SString *KParser_readParen(SakuraObj *skr, KFile *file) {
  return KParser_readNestRange(skr, file, '(', ')');
}

SString *KParser_readString(SakuraObj *skr, KFile *file) {
  return KParser_readNestRange(skr, file, '{', '}');
}

SString *KParser_readNestRange(SakuraObj *skr, KFile *file, char c1, char c2) {
  SString *s;
  int level = 1;
  char *ps;
  size_t len;

  if (*file->pos == c1) file->pos++;

  ps = file->pos;
  while (*ps != '\0') {
    if (*ps == c1) {
      level++;
      ps++;
      continue;
    }
    if (*ps == c2) {
      level--;
      ps++;
      if (level == 0) break;
    }
    ps++;
  }
  len = ps - file->pos - 1;
  s = SString_new(NULL);
  SString_appendMem(s, file->pos, len);
  file->pos = ps;

  return s;
}

// check Calc flag
KToken *KParser_readValue_checkCalc(SakuraObj *skr, KFile *file, KToken *t) {
  char c;
  KToken *t_op, *t_ri, *tmp;
  
  SKIP_SPACE(file->pos);
  
  // flag?
  c = *file->pos;
  if (c != '+' && c != '-' && c != '*' && c != '/' && c != '%' && 
    c != '&' && c != '|' && c != '>' && c != '<' && c != '=' && c != '!') {
    return t;
  }
  
  // printf("[[calc:%c]]", c);
  t_op = KToken_new(KTOKEN_ADD, file->pos);
  switch (c) {
    case '*':
      t_op->token_type = KTOKEN_MUL;
      t_op->no = 4;
      file->pos++;
      break;
    case '/':
      t_op->token_type = KTOKEN_CALC_DIV;
      t_op->no = 4;
      file->pos++;
      break;
    case '%':
      t_op->token_type = KTOKEN_MOD;
      t_op->no = 4;
      file->pos++;
      break;
    case '+':
      t_op->token_type = KTOKEN_ADD;
      t_op->no = 3;
      file->pos++;
      break;
    case '-':
      t_op->token_type = KTOKEN_SUB;
      t_op->no = 3;
      file->pos++;
      break;
    case '>':
      if (memcmp(file->pos, ">=", 2) == 0) {
        t_op->token_type = KTOKEN_GTEQ;
        t_op->no = 2;
        file->pos += 2;
      } else {
        t_op->token_type = KTOKEN_GT;
        t_op->no = 2;
        file->pos++;
      }
      break;
    case '<':
      if (memcmp(file->pos, "<=", 2) == 0) {
        t_op->token_type = KTOKEN_LTEQ;
        t_op->no = 2;
        file->pos += 2;
      } else if (memcmp(file->pos, "<>", 2) == 0) {
        t_op->token_type = KTOKEN_NOT_EQ;
        t_op->no = 2;
        file->pos += 2;
      } else {
        t_op->token_type = KTOKEN_LT;
        t_op->no = 2;
        file->pos++;
      }
      break;
    case '=':
      if (memcmp(file->pos, "==", 2) == 0) {
        file->pos += 2;
      } else {
        file->pos++;
      }
      t_op->token_type = KTOKEN_EQ;
      t_op->no = 2;
      break;
    case '!':
      if (memcmp(file->pos, "!=", 2) == 0) {
        file->pos += 2;
      } else {
        file->pos++;
      }
      t_op->token_type = KTOKEN_NOT_EQ;
      t_op->no = 2;
      break;
    case '&':
      if (memcmp(file->pos, "&&", 2) == 0) {
        file->pos += 2;
      } else {
        file->pos++;
      }
      t_op->token_type = KTOKEN_AND;
      t_op->no = 1;
      break;
    case '|':
      if (memcmp(file->pos, "||", 2) == 0) {
        file->pos += 2;
      } else {
        file->pos++;
      }
      t_op->token_type = KTOKEN_OR;
      t_op->no = 1;
      break;
    default:
      KToken_free(t_op);
      return t;
  }
  //
  // check priority
  //
  // (ex) 1 + 2 * 3
  // | t_op(+:3) -- 1
  // |   |       -- t_ri(*:4) -- 2
  // |   |            |       -- 3
  //
  // (ex) 1 * 2 + 3
  // | t_op(*) - 1
  // |         - t_ri(+) - 2
  // |                   - 3
  // swap operator
  // | t_ri(+) - t_op(*) - 1
  // |                   - 2
  // |         - 3
  t_op->arg = t;
  t_op->extra = t_ri = KParser_readValue(skr, file);
  if (t_op->extra == NULL) {
    k_error(skr, K_ERROR_SYNTAX, "No Value", file->pos);
    t_ri = KToken_new(KTOKEN_NUMBER, file->pos);
    t_ri->no = 0;
    t_ri->flag = 0;
    t_op->extra = t_ri;
    file->pos++;
  }
  
  // check operator priority
  if (KToken_isCalcOp(t_ri->token_type)) {
    if (t_ri->no < t_op->no) {
      tmp = t_ri->arg;
      t_ri->arg = t_op;
      t_op->extra = tmp;
      return t_ri;
    }
  }
  return t_op;
}

KToken *KParser_readValue(SakuraObj *skr, KFile *file) {
  KToken *t;
  char *p;
  long n;
  char vname[256];
  SValue *value;
  int flag;
  void *f;

  SKIP_SPACE(file->pos);
  // (..)
  if (*file->pos == '(') {
    file->pos++;
    SKIP_SPACE(file->pos);
    if (*file->pos == ')') {
      file->pos++;
      return KToken_new(KTOKEN_NOP, file->pos);
    }
    t = KParser_readValue(skr, file);
    SKIP_SPACE(file->pos);
    if (*file->pos != ')') {
      k_error(skr, K_ERROR_SYNTAX, "Not found ')'", file->pos);
      KToken_free(t);
      return NULL;
    }
    file->pos++; // skip ')'
    t = KParser_readValue_checkCalc(skr, file, t);
    return t;
  }
  // number
  if (IN_RANGE('0', '9', *file->pos) || 
    *file->pos == '$' || memcmp(file->pos, "0x", 2) == 0 ||
    *file->pos == '-') {
    t = KToken_new(KTOKEN_NUMBER, file->pos);
    flag = 1;
    if (*file->pos == '-') {
      file->pos++;
      flag = -1;
    }
    n = s_readInt(file->pos, 0, &file->pos);
    t->no = (int)n * flag;
    t = KParser_readValue_checkCalc(skr, file, t);
    return t;
  }
  // string
  if (*file->pos == '{') {
    t = KToken_new(KTOKEN_STRING, file->pos);
    t->value = KParser_readString(skr, file);
    t = KParser_readValue_checkCalc(skr, file, t);
    return t;
  }
  // variable name or function
  if (IS_ALPHA(*file->pos) || *file->pos == '_' || *file->pos == '#') {
    p = KParser_readVarNameStr(skr, file);
    strcpy(vname, p); s_free(p);
    SKIP_SPACE(file->pos);
    // check reader function?
    value = SHash_get(skr->local_vars, vname); // local
    if (value == NULL) {
      value = SHash_get(skr->variables, vname); // global
    }
    if (value != NULL) {
      // TODO: read command value?
      if (value->v_type == SVALUE_TYPE_SYS_FUNC) {
        t = KToken_new(KTOKEN_CALL_SYS_FUNC, file->pos);
        if (*file->pos == '(') {
          f = value->value.f;
          // for Special SYS Command that need string
          if (f == KRun_MML_r || f == KRun_step_r) {
            t->value = KParser_readParen(skr, file);
          } else {
            t->arg = KParser_readValueList(skr, file);
            t->no = KToken_count(t->arg);
          }
        }
        t->extra = value->value.f;
        t = KParser_readValue_checkCalc(skr, file, t);
        return t;
      }
      // user function?
      else if (value->v_type == SVALUE_TYPE_USER_FUNC) {
        t = KToken_new(KTOKEN_CALL_USER_FUNC, file->pos);
        if (*file->pos == '(') {
          t->arg = KParser_readValueList(skr, file);
        }
        t->extra = (KFunction*)value->value.f;
        t = KParser_readValue_checkCalc(skr, file, t);
        return t;
      }
    }
    // other : read variable value
    t = KToken_new(KTOKEN_VARIABLE, file->pos);
    t->value = SString_new(vname);
    if (*file->pos == '(') {
      // array element
      t->next = KToken_new(KTOKEN_ARRAY_ELEMENT, file->pos);
      t->next->arg = KParser_readValue(skr, file);
    }
    
    t = KParser_readValue_checkCalc(skr, file, t);
    return t;
  }
  
  // unknown value
  k_error(skr, K_ERROR_SYNTAX, "Unknown Value", file->pos);
  return NULL;
}

s_bool KParser_track(SakuraObj *skr, KFile *file) {
  KToken *t, *arg;

  arg = KParser_readValueList(skr, file);

  t = KToken_new(KTOKEN_TRACK, file->pos);
  t->arg = arg;
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_channel(SakuraObj *skr, KFile *file) {
  KToken *t, *arg;

  arg = KParser_readValueList(skr, file);

  t = KToken_new(KTOKEN_CHANNEL, file->pos);
  t->arg = arg;
  KFile_appendToken(file, t);

  return S_TRUE;
}

/** include source file */
s_bool KParser_include(SakuraObj *skr, KFile *file) {
  KToken *t;
  char *p, *p2;
  char ch;
  SString *fname;
  KFile *infile;
  char buf[4096];

  // compatible sakura v2
  // (ex) Include(stdmsg.h)
  SKIP_SPACE(file->pos);
  p = file->pos;
  if (*p != '(' && *p != '{') {
    k_error(skr, K_ERROR_ARG_COUNT, "Include(filename)", file->pos);
    return S_FALSE;
  }
  // set close char
  /**/ if (*p == '(') { ch = ')'; }
  else if (*p == '{') { ch = '}'; } else { ch = ')'; }
  p++; // skip '('
  p2 = p;
  while (*p != '\0') {
    if (*p == ch) {
      p++;
      break;
    }
    p++;
  }
  fname = SString_new(NULL);
  SString_appendMem(fname, p2, (p - p2) + 1 - 2);
  file->pos = p;
  if (skr->debug) {
    printf("\nIncude(%s)\n", fname->ptr);
  }
  // check file
  if (!file_exists(fname->ptr)) {
    s_strlcpy(buf, fname->ptr, 4095);
    SString_set(fname, "./Include/");
    SString_append(fname, buf);
    if (!file_exists(fname->ptr)) {
      sprintf(buf, "Include(%s): File Not Found.", buf);
      k_error(skr, K_ERROR_FILE_READ, buf, file->pos);
      SString_free(fname);
      return S_FALSE;
    }
  }
  // load
  infile = SakuraObj_loadFromFile(skr, fname->ptr);
  if (infile == NULL) {
    k_error(skr, K_ERROR_SYNTAX, "Include can not include same file.", file->pos);
    SString_free(fname);
    return S_FALSE;
  }
  SakuraObj_parse(skr, infile);
  // token
  t = KToken_new(KTOKEN_INCLUDE, file->pos);
  t->extra = infile;
  KFile_appendToken(file, t);
  //
  SString_free(fname);

  return S_TRUE;
}

s_bool KParser_readVoice(SakuraObj *skr, KFile *file) {
  KToken *t, *arg;

  file->pos++; // skip '@'
  t = KToken_new(KTOKEN_PROGRAM, file->pos);
  arg = KParser_readValueList(skr, file);
  t->arg = arg;
  t->no = KToken_count(t->arg);
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_backslash(SakuraObj *skr, KFile *file) {
  int level;

  // comment ?
  if (memcmp(file->pos, "//", 2) == 0) {
    file->pos += 2;
    while (*file->pos != '\0') {
      if (*file->pos == '\r' || *file->pos == '\n') break;
      file->pos++;
    }
    return S_TRUE;
  }
  // range comment ?
  if (memcmp(file->pos, "/*", 2) == 0) {
    file->pos += 2;
    level = 1;
    while (*file->pos != '\0') {
      if (memcmp(file->pos, "*/", 2) == 0) {
        file->pos += 2;
        level--;
        if (level <= 0) break;
        continue;
      }
      file->pos++;
    }
    return S_TRUE;
  }
  // Other Command?
  k_error(skr, K_ERROR_SYNTAX, "Command '/' is unknown", file->pos);
  file->pos++;
  return S_FALSE;
}

s_bool KParser_harmony(SakuraObj *skr, KFile *file) {
  SString *harm_s, *harm_len;
  char *p1, *p2;
  KToken *t;
  KFile *harm_file;

  // start pos
  if (*file->pos == '\'') {
    file->pos++;
  }
  p1 = p2 = file->pos;
  // end pos
  while (*p2 != '\0') {
    if (*p2 == '\'') {
      file->pos = p2 + 1;
      break;
    }
    p2++;
  }
  // set buf
  harm_s = SString_new(NULL);
  SString_appendMem(harm_s, p1, (p2 - p1) + 1 - 1);
  // read len
  harm_len = KParser_readLen(skr, file);
  // read harmony string
  harm_file = SakuraObj_loadString(skr, harm_s->ptr);
  SString_free(harm_s);
  // parse
  SakuraObj_parse(skr, harm_file);
  
  // append to token
  t = KToken_new(KTOKEN_HARMONY, file->pos);
  t->value = harm_len;
  t->extra = harm_file;
  KFile_appendToken(file, t);
  
  return S_FALSE;
}

s_bool KParser_div(SakuraObj *skr, KFile *file) {
  char *p;
  int cnt = 0;
  SString *div_s, *div_len;
  KToken *t;
  KFile *div_file;
  
  // Skip '{'
  SKIP_SPACE(file->pos);
  if (*file->pos != '{') {
    k_error(skr, K_ERROR_SYNTAX, "DIV{ ... }", file->pos);
    return S_FALSE;
  }
  file->pos++; // skip '{'
  SKIP_SPACE(file->pos);
  
  p = file->pos;
  while (*p != '\0') {
    // count Note
    if (IN_RANGE('a', 'g', *p) || *p == 'r') {
      cnt++;
      p++;
      continue;
    }
    // is Upper command
    if (IN_RANGE('A', 'Z', *p)) {
      while (IS_ALPHA(*p) || IS_NUMERIC(*p) || *p == '_') p++;
      continue;
    }
    // end of div
    if (*p == '}') {
      p++;
      break;
    }
    p++;
  }
  
  // Div
  div_s = SString_new(NULL);
  SString_appendMem(div_s, file->pos, (p - file->pos) + 1 - 2);
  // printf("(DIV{%s}:%d)", div_s->ptr, cnt);
  file->pos = p;
  div_file = SakuraObj_loadString(skr, div_s->ptr);
  SakuraObj_parse(skr, div_file);
  SString_free(div_s);
  
  // len
  div_len = KParser_readLen(skr, file);
  
  // append to token
  t = KToken_new(KTOKEN_DIV, file->pos);
  t->value = div_len;
  t->extra = div_file;
  t->no = cnt;
  KFile_appendToken(file, t);
  
  return S_TRUE;
}

char *KParser_readVarNameStr(SakuraObj *skr, KFile *file) {
  char *p;
  long sz;
  char vname[256];
  
  SKIP_SPACE(file->pos);
  // read Variable's name
  p = file->pos;
  if (*p == '#') p++; // only first character
  while (IS_ALPHA(*p) || IS_NUMERIC(*p) || *p == '_') p++;
  sz = p - file->pos + 1;
  if (sz > 255) {
    k_error(skr, K_ERROR_SYNTAX, "Variable name is too long.", file->pos);
    while (*p != '\0' && *p != '\n') p++;
    file->pos = p;
    return NULL;
  }
  s_strlcpy(vname, file->pos, sz);
  file->pos = p;
  
  return s_str_dup(vname);
}

SString *KParser_readVarName(SakuraObj *skr, KFile *file) {
  SString *s;
  char *str;
  
  str = KParser_readVarNameStr(skr, file);
  s = SString_new(str);
  s_free(str);
  
  return s;
}

s_bool KParser_int(SakuraObj *skr, KFile *file) {
  KToken *let_int_t;
  KToken *value_t, *lt, *rt;
  SString *varname;
  char *pos_tmp;
  
  pos_tmp = file->pos;
  // Variable name
  varname = KParser_readVarName(skr, file);
  // skip SPACE
  SKIP_SPACE(file->pos);
  if (*file->pos == '=') {
    file->pos++;
    SKIP_SPACE(file->pos);
    // read value
    value_t = KParser_readValue(skr, file);
  } else if (memcmp(file->pos, "+=", 2) == 0) {
    file->pos += 2;
    rt = KParser_readValue(skr, file);
    lt = KToken_new(KTOKEN_VARIABLE, pos_tmp);
    lt->value = SString_new(varname->ptr);
    value_t = KToken_new(KTOKEN_ADD, pos_tmp);
    value_t->arg = rt;
    value_t->extra = lt;
    value_t->no = 3;
  } else if (memcmp(file->pos, "-=", 2) == 0) {
    file->pos += 2;
    rt = KParser_readValue(skr, file);
    lt = KToken_new(KTOKEN_VARIABLE, pos_tmp);
    lt->value = SString_new(varname->ptr);
    value_t = KToken_new(KTOKEN_SUB, pos_tmp);
    value_t->arg = rt;
    value_t->extra = lt;
    value_t->no = 3;
  } else if (memcmp(file->pos, "++", 2) == 0) {
    file->pos += 2;
    value_t = KToken_new(KTOKEN_VARIABLE, pos_tmp);
    value_t->value = SString_new(varname->ptr);
    value_t->next = KToken_new(KTOKEN_INC, pos_tmp);
  } else if (memcmp(file->pos, "--", 2) == 0) {
    file->pos += 2;
    value_t = KToken_new(KTOKEN_VARIABLE, pos_tmp);
    value_t->value = SString_new(varname->ptr);
    value_t->next = KToken_new(KTOKEN_DEC, pos_tmp);
  } else {
    value_t = KToken_new(KTOKEN_NUMBER, file->pos);
    value_t->no = 0;
  }
  
  // append INT token to file
  let_int_t = KToken_new(KTOKEN_DEF_INT, file->pos);
  let_int_t->value = varname;
  
  // append arg
  KFile_appendToken(file, value_t);
  // append to file
  KFile_appendToken(file, let_int_t);
  
  SHash_set((skr->is_local ? skr->local_vars : skr->variables), varname->ptr, SValue_newInt(0));
  return S_TRUE;
}

s_bool KParser_str(SakuraObj *skr, KFile *file) {
  KToken *def_str_t;
  KToken *value_t;
  SString *varname;
  
  // Variable name
  varname = KParser_readVarName(skr, file);
  // skip SPACE
  SKIP_SPACE(file->pos);
  if (*file->pos == '=') {
    file->pos++;
    SKIP_SPACE(file->pos);
    // read value
    value_t = KParser_readValue(skr, file);
  } else {
    value_t = KToken_new(KTOKEN_STRING, file->pos);
    value_t->value = SString_new(NULL);
  }
  // append INT token to file
  def_str_t = KToken_new(KTOKEN_DEF_STR, file->pos);
  def_str_t->value = varname;
  // append to file
  KFile_appendToken(file, value_t);
  KFile_appendToken(file, def_str_t);
  
  SHash_set((skr->is_local ? skr->local_vars : skr->variables), varname->ptr, SValue_newString(NULL));
  
  return S_TRUE;
}

s_bool KParser_array(SakuraObj *skr, KFile *file) {
  KToken *def_array_t;
  KToken *value_t;
  SString *varname;
  
  // Variable name
  varname = KParser_readVarName(skr, file);
  // skip SPACE
  SKIP_SPACE(file->pos);
  if (*file->pos == '=') {
    file->pos++;
    SKIP_SPACE(file->pos);
    // read value
    value_t = KParser_readValueList(skr, file);
  } else {
    value_t =  NULL;
  }
  
  // append INT token to file
  def_array_t = KToken_new(KTOKEN_DEF_ARRAY, file->pos);
  def_array_t->value = varname;
  def_array_t->arg = value_t;
  // append to file
  KFile_appendToken(file, def_array_t);
  
  // register to variable list
  SHash_set((skr->is_local ? skr->local_vars : skr->variables), varname->ptr, SValue_newArray());
  
  return S_TRUE;
}

s_bool KParser_print(SakuraObj *skr, KFile *file) {
  KToken *t, *t_print;
  
  t_print = KToken_new(KTOKEN_PRINT, file->pos);
  t = KParser_readValue(skr, file);
  t_print->arg = t;
  // print
  KFile_appendToken(file, t_print);
  
  return S_TRUE;
}

s_bool KParser_time(SakuraObj *skr, KFile *file) {
  KToken *t, *t_time;
  
  t_time = KToken_new(KTOKEN_TIME, file->pos);
  t = KParser_readValueList(skr, file);
  t_time->arg = t;
  t_time->no = KToken_count(t);
  if (t_time->no != 3 && t_time->no != 1) {
    k_error(skr, K_ERROR_ARG_COUNT, "Time(?:?:?)", file->pos);
    KToken_free(t_time);
    return S_FALSE;
  }
  
  KFile_appendToken(file, t_time);
  
  return S_TRUE;
}

s_bool KParser_system(SakuraObj *skr, KFile *file) {
  KToken *t;
  //
  if (memcmp(file->pos, ".vAdd", 5) == 0) {
    t = KToken_new(KTOKEN_V_ADD, file->pos);
    t->arg = KParser_readValue(skr, file);
    KFile_appendToken(file, t);
    return S_TRUE;
  }
  //
  if (*file->pos == '.') file->pos++;
  return S_TRUE;
}

s_bool KParser_timebase(SakuraObj *skr, KFile *file) {
  // *** timebase direct set KSongInfo ***
  long n;
  SKIP_SPACE(file->pos);
  if (*file->pos == '=') file->pos++;
  if (*file->pos == '(') file->pos++;
  SKIP_SPACE(file->pos);
  n = s_readInt(file->pos, 96, &file->pos);
  skr->info->timebase = (int)n;
  SKIP_SPACE(file->pos);
  if (*file->pos == ')') file->pos++;
  if (skr->debug) {
    printf("[TimeBase] %d\n", skr->info->timebase);
  }
  
  return S_TRUE;
}

s_bool KParser_key(SakuraObj *skr, KFile *file) {
  KToken *t = KToken_new(KTOKEN_KEY, file->pos);
  KToken *arg = KParser_readValueList(skr, file);
  
  t->arg = arg;
  KFile_appendToken(file, t);
  
  return S_TRUE;
}

s_bool KParser_trackKey(SakuraObj *skr, KFile *file) {
  KToken *t = KToken_new(KTOKEN_TRACK_KEY, file->pos);
  KToken *arg = KParser_readValueList(skr, file);
  
  t->arg = arg;
  KFile_appendToken(file, t);
  
  return S_TRUE;
}

s_bool KParser_if(SakuraObj *skr, KFile *file) {
  KToken *if_t;
  SString *true_s = NULL;
  SString *false_s = NULL;
  KTokenFlowOption *opt;
  char *hint_p;
  
  if_t = KToken_new(KTOKEN_IF, file->pos);
  if_t->free_f = KTokenFlowOption_free;
  if_t->extra = opt = s_new(KTokenFlowOption);
  
  // set expr
  if_t->arg = KParser_readValue(skr, file);
  
  SKIP_SPACE(file->pos);
  if (*file->pos != '{') {
    k_error(skr, K_ERROR_SYNTAX, "IF(e) { .. } ELSE { .. }", file->pos);
    return S_FALSE;
  }
  // parser
  hint_p = file->pos;
  true_s = KParser_readString(skr, file);
  opt->true_t = KParser_parseString(skr, true_s->ptr, hint_p);
  SString_free(true_s);
  
  // Check ELSE sentence
  SKIP_SPACE_CRLF(file->pos);
  if (memcmp(file->pos, "ELSE", 4) == 0 || memcmp(file->pos, "Else", 4) == 0) {
    file->pos += 4;
    SKIP_SPACE_CRLF(file->pos);
    if (*file->pos != '{') {
      k_error(skr, K_ERROR_SYNTAX, "IF(e) { .. } ELSE { .. }", file->pos);
      return S_FALSE;
    }
    hint_p = file->pos;
    false_s = KParser_readString(skr, file);
    opt->false_t = KParser_parseString(skr, false_s->ptr, hint_p);
    SString_free(false_s);
  } else {
    opt->false_t = NULL; // explicitly set null
  }
  
  KFile_appendToken(file, if_t);
  
  return S_TRUE;
}

s_bool KParser_while(SakuraObj *skr, KFile *file) {
  KToken *while_t, *body_t, *end_t;
  SString *str;
  KTokenFlowOption *opt;
  char *hint_p;
 
  while_t = KToken_new(KTOKEN_WHILE, file->pos);
  while_t->arg = KParser_readValue(skr, file);
  
  // while body
  SKIP_SPACE(file->pos);
  if (*file->pos != '{') {
    k_error(skr, K_ERROR_SYNTAX, "WHILE(expr) { ... }", file->pos);
    return S_FALSE;
  }
  hint_p = file->pos;
  str = KParser_readString(skr, file);
  body_t = KParser_parseString(skr, str->ptr, hint_p);
  SString_free(str);
  
  end_t = KToken_new(KTOKEN_NOP, file->pos);
  
  opt = s_new(KTokenFlowOption);
  opt->true_t = body_t;
  opt->false_t = end_t;
  while_t->free_f = KTokenFlowOption_free;
  while_t->extra = opt;
  
  // connect
  KFile_appendToken(file, while_t);
  KFile_appendToken(file, end_t);
    
  return S_TRUE;
}

s_bool KParser_defineUserFunc(SakuraObj *skr, KFile *file) {
  KToken *func_t, *body_t, *end_t;
  SString *str, *ds;
  KTokenFlowOption *opt;
  KFunction *func;
  KFuncArg *arg;
  SValue *func_v;
  int i, n;
  char *s, *hint_p;
 
  // new
  func_t = KToken_new(KTOKEN_DEF_FUNCTION, file->pos);
  func = KFunction_new();
  
  // function name
  func->name = KParser_readVarNameStr(skr, file);
  func_t->value = SString_new(func->name);
  
  // read function args
  SKIP_SPACE_CRLF(file->pos);
  if (*file->pos == '(') {
    file->pos++;
    func->args = s_new_a(KFuncArg*, 50); // Max 50 args ... hahaha tenuki
    i = 0;
    for (;;) {
      SKIP_SPACE_CRLF(file->pos);
      if (*file->pos == ')') {
        func->args_len = i;
        file->pos++;
        break;
      }
      arg = KFuncArg_new();
      // read type and name
      s = KParser_readVarNameStr(skr, file);
      if (strcmp(s, "Str") == 0 || strcmp(s, "STR") == 0) {
        arg->value_type = SVALUE_TYPE_STRING;
        arg->value = SValue_newString(NULL);
        s_free(s);
        arg->name = KParser_readVarNameStr(skr, file);
      }
      else if (strcmp(s, "Int") == 0 || strcmp(s, "INT") == 0) {
        arg->value_type = SVALUE_TYPE_INT;
        arg->value = SValue_newInt(0);
        s_free(s);
        arg->name = KParser_readVarNameStr(skr, file);
      }
      else if (strcmp(s, "Array") == 0 || strcmp(s, "ARRAY") == 0) {
        arg->value_type = SVALUE_TYPE_ARRAY;
        arg->value = SValue_newArray();
        s_free(s);
        arg->name = KParser_readVarNameStr(skr, file);
      }
      else {
        arg->value_type = SVALUE_TYPE_NULL;
        arg->value = SValue_new();
        arg->name = s;
      }
      func->args[i] = arg;
      // check default value
      if (*file->pos == '=') {
        file->pos++;
        SKIP_SPACE(file->pos);
        if (IS_NUMERIC(*file->pos) || *file->pos == '{') {
          if (IS_NUMERIC(*file->pos)) {
            n = s_readInt(file->pos, 0, &file->pos);
            SValue_setInt(arg->value, n);
          } else {
            ds = KParser_readString(skr, file);
            SValue_setString(arg->value, ds->ptr);
            SString_free(ds);
          }
        } else {
          k_error(skr, K_ERROR_SYNTAX, "Function arg default value must be const value.", file->pos);
          file->pos++;
        }
      }
      i++;
      if (i > 50) {
        k_error(skr, K_ERROR_SYNTAX, "Too many function argments...", file->pos);
        break;
      }
      // comma
      SKIP_SPACE_CRLF(file->pos);
      if (*file->pos == ',') file->pos++;
    }
  }
  
  // function body
  SKIP_SPACE(file->pos);
  if (*file->pos != '{') {
    k_error(skr, K_ERROR_SYNTAX, "function name(args) { ... }", file->pos);
    KToken_free(func_t);
    return S_FALSE;
  }
  hint_p = file->pos;
  str = KParser_readString(skr, file);
  skr->is_local = S_TRUE;
  // register func arg to local_vars
  SHash_clear(skr->local_vars);
  for (i = 0; i < func->args_len; i++) {
    arg = func->args[i];
    SHash_set(skr->local_vars, arg->name, SValue_clone(arg->value));
  }
  
  body_t = KParser_parseString(skr, str->ptr, hint_p);
  SString_free(str);
  
  end_t = KToken_new(KTOKEN_NOP, file->pos);
  
  opt = s_new(KTokenFlowOption);
  opt->true_t = body_t;
  opt->false_t = end_t;
  func_t->free_f = KTokenFlowOption_free;
  func_t->extra = opt;
  // register tokens to function
  func->tokens = body_t;
  
  func_v = SValue_new();
  func_v->v_type = SVALUE_TYPE_USER_FUNC;
  func_v->value.f = func;
  func_v->free_f = SValue_freeItem_KFunction;
  SHash_set(skr->variables, func->name, func_v);
  
  // connect
  KFile_appendToken(file, func_t);
  KFile_appendToken(file, end_t);
  
  skr->is_local = S_FALSE;
  
  return S_TRUE;
}

s_bool KParser_callUserFunc(SakuraObj *skr, KFile *file, KFunction *f) {
  // @see KParser_readUpperCommand
  KToken *func_t;
  
  func_t = KToken_new(KTOKEN_CALL_USER_FUNC, file->pos);
  SKIP_SPACE(file->pos);
  if (*file->pos == '=' || *file->pos == '(' || *file->pos == '{') {
    func_t->arg = KParser_readValueList(skr, file);
  }
  func_t->extra = f;
  KFile_appendToken(file, func_t);
  
  return S_TRUE;
}

s_bool KParser_return(SakuraObj *skr, KFile *file) {
  KToken *t = KToken_new(KTOKEN_RETURN, file->pos);
  
  SKIP_SPACE(file->pos);
  t->arg = KParser_readValue(skr, file);
  KFile_appendToken(file, t);
  
  return S_TRUE;
}



s_bool KParser_octaveOnetime(SakuraObj *skr, KFile *file) {
  KToken *t;
  char c = *file->pos;

  file->pos++; // skip '"' or '`'
  t = KToken_new(KTOKEN_OCTAVE1UD, file->pos);
  t->no = (c == '"') ? -1 : 1;
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_measureShift(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_MEASURE_SHIFT, file->pos);
  t->arg = KParser_readValueList(skr, file);
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_keyflag(SakuraObj *skr, KFile *file) {
  // TODO: keyflag
  // [Syntax] KeyFlag[+|-](note)|KeyFlag=(a,b,c,d,e,f,g)
  KToken *t;
  
  t = KToken_new(KTOKEN_KEYFLAG, file->pos);
  // '='
  SKIP_SPACE(file->pos);
  if (*file->pos == '=') {
    file->pos++; // skip '='
    t->flag = 0;
    t->arg = KParser_readValueList(skr, file);
    KFile_appendToken(file, t);
    return S_TRUE;
  }
  // '+' or '-'
  if (*file->pos == '+' || *file->pos == '#') {
    t->flag = 1;
    file->pos++;
  }
  else if (*file->pos == '-') {
    t->flag = -1;
    file->pos++;
  }
  SKIP_SPACE(file->pos);
  t->value = KParser_readParen(skr, file);
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_timeSignature(SakuraObj *skr, KFile *file) {
  KToken *t;
  int c;
  
  t = KToken_new(KTOKEN_TIME_SIGNATURE, file->pos);
  t->arg = KParser_readValueList(skr, file);
  c = KToken_count(t->arg);
  if (c != 2) {
    k_error(skr, K_ERROR_ARG_COUNT, "TimeSignature(si,bo)", file->pos);
    KToken_free(t);
    return S_FALSE;
  }
  KFile_appendToken(file, t);
  
  return S_TRUE;
}


s_bool KParser_tempo(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_TEMPO, file->pos);
  t->arg = KParser_readValueList(skr, file);
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_port(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_PORT, file->pos);
  t->arg = KParser_readValueList(skr, file);
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_ChannelPrefix(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_CH_PREFIX, file->pos);
  t->arg = KParser_readValueList(skr, file);
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_metaText(SakuraObj *skr, KFile *file) {
  KToken *t;

  t = KToken_new(KTOKEN_METATEXT, file->pos);
  t->arg = KParser_readValueList(skr, file);
  t->no = KParser_last_command->tag;
  KFile_appendToken(file, t);

  return S_TRUE;
}

s_bool KParser_sysex(SakuraObj *skr, KFile *file) {
  return KParser_sysex_(skr, file, S_FALSE);
}

s_bool KParser_sysex_hex(SakuraObj *skr, KFile *file) {
  return KParser_sysex_(skr, file, S_TRUE);
}

s_bool KParser_sysex_(SakuraObj *skr, KFile *file, s_bool is_hex) {
	// Example :: SysEx$=F0,41,(DeviceNumber),42,12,40,00,7F,00,41,F7;
  int n;
  KToken *sysex_t, *t, *arg;
  
  sysex_t = KToken_new(KTOKEN_SYS_EX, file->pos);
  arg = KToken_new(KTOKEN_NOP, file->pos);
  sysex_t->arg = arg;
  
  SKIP_SPACE(file->pos);
  if (*file->pos == '=') file->pos++;
  while (*file->pos != '\0') {
    SKIP_SPACE(file->pos);
    // '{' .. '}'
    if (*file->pos == '{') { // check MARK
      file->pos++;
      t = KToken_new(KTOKEN_MARKER, file->pos);
      t->no = 0x10;
      arg->next = t;
      arg = t;
    }
    
    // get number
    if ((is_hex == S_FALSE) && 
        ((IS_NUMERIC(*file->pos) || *file->pos == '$' || memcmp(file->pos, "0x", 2) == 0))) {
      n = s_readInt(file->pos, 0, &file->pos);
      t = KToken_new(KTOKEN_NUMBER, file->pos);
      t->no = n;
      arg->next = t;
      arg = t;
    }
    else if ((is_hex == S_TRUE) && (IS_NUMERIC_HEX(*file->pos))) {
      n = s_readHex(file->pos, 0, &file->pos);
      t = KToken_new(KTOKEN_NUMBER, file->pos);
      t->no = n;
      arg->next = t;
      arg = t;
    }
    else {
      t = KParser_readValue(skr, file);
      if (t == NULL) {
        k_error(skr, K_ERROR_SYNTAX, "Could not read value in SysEx$", file->pos);
        break;
      }
      arg->next = t;
      arg = t;
    }
    //
    SKIP_SPACE(file->pos);
    if (*file->pos == '}') { // check MARK
      file->pos++;
      t = KToken_new(KTOKEN_MARKER, file->pos);
      t->no = 0x11;
      arg->next = t;
      arg = t;
    }
    SKIP_SPACE(file->pos);
    if (*file->pos != ',') {
      break;
    }
    file->pos++;
  }
  
  KFile_appendToken(file, sysex_t);
  return S_TRUE;
}

s_bool KParser_rhythm_define(SakuraObj *skr, KFile *file) {
  char rch;
  SString *s;
  
  if (*file->pos == '$') file->pos++;
  rch = *file->pos;
  file->pos++;
  SKIP_SPACE(file->pos);
  if (*file->pos == '=') file->pos++;
  SKIP_SPACE(file->pos);
  if (*file->pos != '{') {
    k_error(skr, K_ERROR_SYNTAX, "Macro should define $c{...}", file->pos);
    return S_FALSE;
  }
  s = KParser_readString(skr, file);
  skr->rhythm[(s_byte)rch] = s_str_dup(s->ptr);
  SString_free(s);
  
  return S_TRUE;
}

s_bool KParser_rhythm(SakuraObj *skr, KFile *file) {
  SString *scope;
  SString *src;
  char *p, *mac, *hint_p;
  int nest;
  KToken *t, *rhthm_t;
  
  SKIP_SPACE(file->pos);
  if (*file->pos != '{') {
    k_error(skr, K_ERROR_SYNTAX, "Rhythm{ ... }", file->pos);
    return S_FALSE;
  }
  
  rhthm_t = KToken_new(KTOKEN_RHYTHM, file->pos);
  scope = KParser_readString(skr, file);
  // printf("[Rhythm] org=%s\n", scope->ptr);
  src = SString_new(NULL);
  p = scope->ptr;
  while (*p != '\0') {
    // is Rhtym macro?
    mac = skr->rhythm[(s_byte)*p];
    if (mac != NULL) {
      SString_append(src, mac);
      p++;
      continue;
    }
    // without rhythm mode
    if (*p == '(') {
      nest = 1; p++;
      while (*p != '\0') {
        if (*p == '(') {
          nest++;
          p++;
          continue;
        }
        if (*p == ')') {
          nest--;
          if (nest == 0) {
            p++;
            break;
          }
          continue;
        }
        SString_appendChar(src, *p);
        p++;
      }
      continue;
    }
    // other char
    SString_appendChar(src, *p);
    p++;
  }
  if (skr->debug) {
    printf("[Rhythm] %s\n", src->ptr);
  }
  //
  hint_p = file->pos;
  t = KParser_parseString(skr, src->ptr, hint_p);
  rhthm_t->cache = t;
  KFile_appendToken(file, rhthm_t);
    
  SString_free(scope);
  SString_free(src);
  
  return S_TRUE;
}

KToken *KParser_parseString(SakuraObj *skr, const char* str, char *hint_pos) {
  KFile *file;
  SString *tmp;

  // set string
  file = SakuraObj_loadString(skr, str);
  
  // renew string
  if (hint_pos != NULL) {
    tmp = SakuraObj_getPosInfo(skr, hint_pos);
    SString_set(file->filename, tmp->ptr);
    SString_free(tmp);
  }
  // parse
  SakuraObj_parse(skr, file);

  return file->token_top;
}

s_bool KParser_pitchBend(SakuraObj *skr, KFile *file) {
  KToken *pb;
  
  pb = KToken_new(KTOKEN_PITCH_BEND, file->pos);
  pb->arg = KParser_readValue(skr, file);
  KFile_appendToken(file, pb);
  
  return S_TRUE;
}

s_bool KParser_trackSync(SakuraObj *skr, KFile *file) {
  KToken *t = KToken_new(KTOKEN_TRACK_SYNC, file->pos);
  KFile_appendToken(file, t);
  return S_TRUE;
}

s_bool KParser_trackSub(SakuraObj *skr, KFile *file) {
  KToken *t;
  //
  t = KToken_new(KTOKEN_TRACK_SUB, file->pos);
  t->value = KParser_readString(skr, file);
  t->arg = KParser_parseString(skr, t->value->ptr, t->pos);
  KFile_appendToken(file, t);
  //
  return S_TRUE;
}

s_bool KParser_readSharpMacro(SakuraObj *skr, KFile *file) {
  char *tmp;
  char name[256];
  KToken *t;
  // skip #
  file->pos++;
  // get macro name
  tmp = KParser_readVarNameStr(skr, file);
  sprintf(name, "#%s", tmp);
  s_free(tmp);
  SKIP_SPACE(file->pos);
  // LET or EXEC
  if (*file->pos == '=') {
    // LET
    file->pos++;
    SKIP_SPACE(file->pos);
    t = KToken_new(KTOKEN_SHARP_MACRO_DEFINE, file->pos);
    t->arg = KParser_readValue(skr, file);
    t->value = SString_new(name);
    KFile_appendToken(file, t);
    return S_TRUE;
  }
  else {
    // EXEC
    t = KToken_new(KTOKEN_SHARP_MACRO_EXEC, file->pos);
    t->value = SString_new(name);
    KFile_appendToken(file, t);
    SKIP_SPACE(file->pos);
    // ARG?
    if (*file->pos == '(') {
      t->arg = KParser_readValueList(skr, file);
      t->no = KToken_count(t->arg);
    }
    return S_TRUE;
  }
}

s_bool KParser_playFrom(SakuraObj *skr, KFile *file) {
  KToken *t = KToken_new(KTOKEN_PLAY_FROM, file->pos);
  t->arg = KParser_readValueList(skr, file);
  t->no = KToken_count(t->arg);
  KFile_appendToken(file, t);
  return S_TRUE;
}

s_bool KParser_playFromNow(SakuraObj *skr, KFile *file) {
  KToken *t = KToken_new(KTOKEN_PLAY_FROM, file->pos);
  KFile_appendToken(file, t);
  return S_TRUE;
}

s_bool KParser_readCCUpper(SakuraObj *skr, KFile *file, int ccno) {
  KToken *t;
  char *vname;
  // CC Option
  if (*file->pos == '.') {
    t = KToken_new(KTOKEN_NOP, file->pos); // TODO: CC.onTime
    file->pos++;
    vname = KParser_readVarNameStr(skr, file);
    if (strcmp(vname, "onTime") == 0 || strcmp(vname, "T") == 0) {
      t->arg = KParser_readValueList(skr, file);
    }
    else if (strcmp(vname, "onNote") == 0 || strcmp(vname, "N") == 0) {
      t->arg = KParser_readValueList(skr, file);
    }
    else if (strcmp(vname, "onCycle") == 0 || strcmp(vname, "C") == 0) {
      t->arg = KParser_readValueList(skr, file);
    }
    KFile_appendToken(file, t);
    s_free(vname);
    return S_TRUE;
  }
  // KTOKEN_CC_UPPER
  t = KToken_new(KTOKEN_CC_UPPER, file->pos);
  t->arg = KParser_readValue(skr, file);
  t->no = ccno;
  KFile_appendToken(file, t);
  return S_TRUE;
}

s_bool KParser_chorus(SakuraObj *skr, KFile *file) {
  return KParser_readCCUpper(skr, file, 93);
}
s_bool KParser_reverb(SakuraObj *skr, KFile *file) {
  return KParser_readCCUpper(skr, file, 91);
}
s_bool KParser_expression(SakuraObj *skr, KFile *file) {
  return KParser_readCCUpper(skr, file, 11);
}
s_bool KParser_panpot(SakuraObj *skr, KFile *file) {
  return KParser_readCCUpper(skr, file, 10);
}
s_bool KParser_modulation(SakuraObj *skr, KFile *file) {
  return KParser_readCCUpper(skr, file, 1);  
}
s_bool KParser_volume(SakuraObj *skr, KFile *file) {
  return KParser_readCCUpper(skr, file, 7);  
}
s_bool KParser_var_effect(SakuraObj *skr, KFile *file) {
  return KParser_readCCUpper(skr, file, 70);  
}

s_bool KParser_timekeyflag(SakuraObj *skr, KFile *file) {
  SString *s;
  KToken *t;
  // TODO: TimeKeyFlag
  SKIP_SPACE(file->pos);
  if (*file->pos == '(') {
    s = KParser_readParen(skr, file);
    SString_free(s);
  }
  else if (*file->pos == '=') {
    file->pos++;
    t = KParser_readValueList(skr, file);
    KToken_free(t);
  }
  return S_TRUE;
}


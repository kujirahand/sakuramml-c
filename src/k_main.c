/**
 * @file k_main.c
 *
 * @date Created on: 2014/09/29
 * @author kujirahand.com
 * @version 1.0
 *
 * MML Compiler main file
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "s_lib.h"
#include "s_value.h"
#include "k_token.h"
#include "k_main.h"
#include "k_parser.h"
#include "k_track.h"
#include "k_run.h"
#include "k_sutoton.h"
#include "k_function.h"
#include "k_smf_writer.h"
#include "k_formatter.h"

// forward def
void SakuraObj_initTokenPos(SakuraObj *skr, KFile *file);
void SakuraObj_addFile(SakuraObj *skr, KFile *file);
s_bool k_main_valueStack_freeItem(void *ptr);
s_bool k_main_varialbe_freeItem(const char *key, void *value);
s_bool k_main_func_freeItem(const char *key, void *value);

/** Easy method */
s_bool Sakura_mml2smf(const char* infile, const char* outfile, SakuraCommandOpt *opt) {
  SakuraObj *skr;
  KFile *file;
  s_bool result = S_FALSE;
  char *stdmsg_h = "Include/stdmsg.h";

  skr = SakuraObj_new();
  skr->debug = opt->is_debug;
  skr->use_print_console = opt->use_print_console;

  // include "stdmsg.h"
  if (opt->include_stdmsg && file_exists(stdmsg_h)) {
    if (skr->debug) {
      printf("- Include: %s\n", stdmsg_h);
    }
    file = SakuraObj_loadFromFile(skr, stdmsg_h);
    SakuraObj_parse(skr, file);
    SakuraObj_run(skr, file);
    // printf("- End of Include: %s\n", stdmsg_h);
  }

  // load main file
  if (skr->debug) {
    printf("- load file: %s\n", infile);
  }
  file = SakuraObj_loadFromFile(skr, infile);
  if (file == NULL) goto finally_label;

  if (skr->debug) {
    printf("--- parse ---\n");
  }
  SakuraObj_parse(skr, file);
  if (skr->debug) {
    printf("--- run ---\n");
  }
  SakuraObj_run(skr, file);

  if (skr->debug) {
    printf("--- save to smf ---\n");
  }
  KSmfWriter_saveToSmf1(skr, outfile);
  result = S_TRUE;

finally_label:
  if (skr->debug) {
    printf("--- free memory ---\n");
  }
  SakuraObj_free(skr);
  return result;
}

s_bool Sakura_mmlstr2smf(const char* mml, const char* outfile, SakuraCommandOpt *opt) {
  SakuraObj *skr;
  KFile *file;
  s_bool result = S_FALSE;

  skr = SakuraObj_new();
  skr->debug = opt->is_debug;
  skr->use_print_console = opt->use_print_console;

  file = SakuraObj_loadString(skr, mml);
  if (file == NULL) goto finally_label;

  SakuraObj_parse(skr, file);
  SakuraObj_run(skr, file);

  KSmfWriter_saveToSmf1(skr, outfile);
  result = S_TRUE;

finally_label:
  SakuraObj_free(skr);
  return result;
}

/** SakuraObj method */

/**
 * @param[in] void
 * @return return SakuraObj
 * @detail create SakuraObj
 * @see SakuraObj_free()
 */
SakuraObj *SakuraObj_new() {
  SakuraObj *skr = s_new(SakuraObj);
  // randomize
  srand((unsigned)time(NULL));
  // SongInfo
  skr->info = s_new(KSongInfo);
  skr->info->timebase = 96;
  skr->info->timeSig_deno = 4;
  skr->info->timeSig_nume = 4;
  skr->info->measureShift = 0;
  skr->info->track_no = 1; // Set default track
  skr->info->revOctaveFlag = S_FALSE;
  skr->info->is_harmony = S_FALSE;
  skr->info->key = 0;
  skr->info->octaveOnetime = 0;
  // Error info
  skr->error_stop = S_FALSE;
  skr->errors = SStringList_new();
  // print
  skr->print_buf = SString_new(NULL);
  skr->use_print_console = S_FALSE;
  // Tracks
  skr->tracks = KTrackList_new(skr->info);
  skr->smf = KSmfTrackList_new();

  // Stack for Loop
  skr->loopStack = SList_new();
  skr->valueStack = SList_new();
  skr->valueStack->free_item_f = k_main_valueStack_freeItem;

  // Command hash
  skr->command_hash = SHash_new(0);
  KParser_appendCommand(skr->command_hash);

  skr->sutoton = KSutotonList_new();
  skr->rhythm = s_new_a(char*, 256);

  // Variables
  skr->variables = SHash_new(0);
  skr->variables->callback_free_item = k_main_varialbe_freeItem;
  KParser_appendReaderCommand(skr->variables);
  KParser_appendSystemVairables(skr->variables);
  // Local Variables
  skr->local_vars = SHash_new(0);
  skr->local_vars->callback_free_item = k_main_varialbe_freeItem;
  skr->is_local = S_FALSE;

  return skr;
}

s_bool k_main_varialbe_freeItem(const char *key, void *value) {
  SValue_free(value);
  return S_TRUE;
}

/**
 * @param obj SakuraObj Object
 * @detail free SakuraObj
 */
void SakuraObj_free(SakuraObj *skr) {
  char *p;
  int i;

  KFile_freeAll(skr->file_top);
  SStringList_free(skr->errors);
  KTrackList_free(skr->tracks);
  KSmfTrackList_free(skr->smf);
  KLoopStack_free(skr->loopStack);
  KValueStack_free(skr->valueStack);
  KParser_freeCommandHash(skr->command_hash);
  KSutotonList_free(skr->sutoton);
  SHash_free(skr->variables);
  SHash_free(skr->local_vars);
  SString_free(skr->print_buf);
  s_free(skr->info);
  // free rhythm
  for (i = 0; i < 256; i++) {
    p = skr->rhythm[i];
    if (p != NULL) s_free(p);
  }
  s_free(skr->rhythm);
  // free sakura object
  s_free(skr);
}

s_bool k_main_valueStack_freeItem(void *ptr) {
  SValue *v;

  v = (SValue*)ptr;
  if (v == NULL) return S_TRUE;
  SValue_free(v);

  return S_TRUE;
}


/** initialize token status in KFile object */
void SakuraObj_initTokenPos(SakuraObj *skr, KFile *file) {
  KToken *t;

  // check double
  if (file->token_top != NULL) {
    return;
  }

  // trim comment
  KFormatter_trimComment(file->source);
  // sutoton
  KSutotonList_replace(skr->sutoton, file->source);
  file->pos = file->source->ptr;
  /*
  if (skr->debug) { // DEBUG Sutoton
    printf("SUTOTON=%s\n----\n", file->source->ptr);
  }
  */

  // set top token
  file->pos = file->source->ptr;
  t = KToken_new(KTOKEN_TOP, file->pos);
  file->token_top = file->token_cur = t;
}

void SakuraObj_addFile(SakuraObj *skr, KFile *file) {
  KFile *f;

  // add to file list
  if (skr->file_top == NULL) {
    skr->file_top = file;
  } else {
    f = skr->file_top;
    while (f->next != NULL) f = f->next;
    f->next = file;
  }

  // set topken top
  SakuraObj_initTokenPos(skr, file);
}

/**
 * load from source string
 * @param *obj
 * @param *src soruce string
 * @return file object
 */
KFile *SakuraObj_loadString(SakuraObj *skr, const char *src) {
  KFile *file;

  file = KFile_new("string", src);
  SakuraObj_addFile(skr, file);

  // set token top
  return file;
}

/**
 * load from file
 */
KFile *SakuraObj_loadFromFile(SakuraObj *skr, const char *filename) {
  KFile *file, *f;
  SString *str;
  size_t r;

  // loaded check
  f = skr->file_top;
  while (f != NULL) {
    if (strcmp(f->filename->ptr, filename) == 0) {
      k_error(skr, K_ERROR_SYNTAX, "File already loaded.", NULL);
      return NULL;
    }
    // printf("load = [%s]\n", f->filename->ptr);
    f = f->next;
  }

  // load file
  str = SString_new(NULL);
  r = SString_load(str, filename);
  if (r == 0) {
    k_error(skr, K_ERROR_FILE_READ, "Failed to load file", filename);
    SString_free(str);
    return NULL;
  }
  file = KFile_new(filename, str->ptr);
  SString_free(str);

  // add file to list
  SakuraObj_addFile(skr, file);

  return file;
}

SString *k_main_get_near_str(SString *s, const char *near) {
  SString *r;

  if (near != NULL) {
    SString_appendSubstrUTF8(s, near, 16);
    r = SString_replace(s, "\r", ".");
    SString_set(s, r->ptr);
    SString_free(r);
    r = SString_replace(s, "\n", ".");
    SString_set(s, r->ptr);
    SString_free(r);
  }

  return s;
}

SString *SakuraObj_getPosInfo(SakuraObj *skr, const char *pos) {
  SString *res = SString_new("Unknown:0");
  char *p1, *p2, *p;
  int lno;
  if (skr == NULL) return res;

  KFile *f = skr->file_top;
  while (f != NULL) {
    p1 = f->source->ptr;
    p2 = p1 + f->source->len;
    if (p1 <= pos && pos <= p2) {
      // check lineNo
      lno = 1; p = p1;
      while (*p != '\0') {
        if (p == pos) {
          SString_clear(res);
          SString_append(res, f->filename->ptr);
          SString_appendChar(res, ':');
          SString_appendInt(res, lno);
          break;
        }
        if (*p == '\n') lno++;
        p++;
      }
    }
    f = f->next;
  }

  return res;
}



/**
 * Add Error message
 * @param errno
 * @param *msg
 * @param *file
 * @param lineno
 */
void SakuraObj_error(SakuraObj *skr, int errorno, const char *msg, const char *near, const char *file, int lineno) {
  SString *s;
  char buf[K_ERR_BUFSIZE];
  SString *fi;

  if (skr->error_count > 20) return;
  skr->error_count++;

  s = SString_new(NULL);
  k_main_get_near_str(s, near);

  if (skr != NULL) {
    skr->error_stop = S_TRUE;
  }
  // get error file info
  fi = SakuraObj_getPosInfo(skr, near);

  SakuraObj_getErrorMsg(errorno, buf, K_ERR_BUFSIZE);

  // show error
  fprintf(stderr,
    "[%s] %s { %s } (%s)(%s:%d)\n",
	buf,
    msg,
    s->ptr,
    fi->ptr,
    file, lineno);
  // free
  SString_free(s);
  SString_free(fi);
}

void SakuraObj_warn(SakuraObj *skr, int errorno, const char *msg, const char *near, const char *file, int lineno) {
  SString *s;

  if (skr->error_count > 20) return;
  skr->error_count++;

  s = SString_new(NULL);
  SString_append(s, msg);
  k_main_get_near_str(s, near);

  // TODO: エラーメッセージを正しく出す
  if (skr == NULL) {
    // skr->errosに追加する
    skr->error_stop = S_TRUE;
  }

  // show error
  fprintf(stderr,
    "[WARNING] (%s:%d) %s(%d)\n",
    file,
    lineno,
    s->ptr, errorno);
  // free
  SString_free(s);
}

char *SakuraObj_getErrorMsg(int errno, char *buf, int len) {

	switch (errno) {
    case K_ERROR_SYSTEM:      s_strlcpy(buf, "K_ERROR_SYSTEM", len-1); return buf;
    case K_ERROR_SYNTAX:      s_strlcpy(buf, "K_ERROR_SYNTAX", len-1); return buf;
    case K_ERROR_FILE_READ:   s_strlcpy(buf, "K_ERROR_FILE_READ", len-1); return buf;
    case K_ERROR_LOOP_RANGE:  s_strlcpy(buf, "K_ERROR_LOOP_RANGE", len-1); return buf;
    case K_ERROR_FILE_WRITE:  s_strlcpy(buf, "K_ERROR_FILE_WRITE", len-1); return buf;
    case K_ERROR_ARG_COUNT:   s_strlcpy(buf, "K_ERROR_ARG_COUNT", len-1); return buf;
  }
  s_strlcpy(buf, "K_ERROR_UNKNOWN", len-1);
  return buf;
}



/**
 * parse sourcefile
 * @see SakuraObj_loadString()
 */
s_bool SakuraObj_parse(SakuraObj *obj, KFile *file) {
  return KParser_parse(obj, file);
}

/**
 * run token list
 */
s_bool SakuraObj_run(SakuraObj *skr, KFile *file){
  return KRun_runFile(skr, file);
  /*
  // Run all files
  file = skr->file_top;
  while (file != NULL) {
    if (!KRun_tokens(skr, file)) {
      result = S_FALSE; break;
    }
    file = file->next;
  }
  */
}

KTrack *SakuraObj_curTrack(SakuraObj *skr) {
  int no = skr->info->track_no - 1;
  KTrack *t = KTrackList_get(skr->tracks, no);
  return t;
}

KSmfTrack *SakuraObj_curSmfTrack(SakuraObj *skr) {
  int no = skr->info->track_no - 1;
  KSmfTrack *t = KSmfTrackList_get(skr->smf, no);
  return t;
}

int SakuraObj_time2step(SakuraObj *skr, int mes, int beat, int tick) {

  KSongInfo *i = skr->info;
  int base, result;

  // memo:
  // if 4/4
  // (mes-1)*(timebase*4) + (beat-1)*timebase + tick
  // ---
  // if 6/8
  // base = timebase div 2
  // (mes-1)*(base*6) + (beat-1)*base + tick

  mes += skr->info->measureShift;

  base = i->timebase * 4 / i->timeSig_deno;
  result = (mes - 1) * (base * i->timeSig_nume) + (beat - 1) * base + tick;

  return result;
}

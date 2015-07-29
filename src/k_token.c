/**
 * @file k_token.c
 *
 * @date 2014/09/29
 * @author kujirahand.com
 * @version 1.0
 */

#include <stdio.h>
#include <string.h>
#include "s_lib.h"
#include "k_token.h"
#include "k_function.h"

/**
 * @param token_type
 * @param *pos token's position
 * @return KToken type
 * @see KToken_free()
 */
KToken *KToken_new_(int token_type, char *pos, const char *file, int lineno) {
  KToken *t;
  
  t = (KToken *)s_malloc_(sizeof(KToken), file, lineno);
  t->token_type = token_type;
  t->pos = pos;
  t->next = NULL;
  t->b_arg_free = S_TRUE;
  return t;
}

/**
 * @param t token obj to free
 * @see KToken_new()
 */
void KToken_free(KToken *t) {
  if (t == NULL) return;
  
  // free arg
  if (t->arg != NULL) {
    if (t->b_arg_free) {
      KToken_freeAll(t->arg);
    }
  }
  
  // free value
  SString_free(t->value);
  
  // calc operator ?
  if (KToken_isCalcOp(t->token_type)) {
    KToken_freeAll((KToken *)t->extra);
  }
  
  // free function
  if (t->free_f != NULL) t->free_f(t);
  
  s_free(t);
}

/**
 * free all object from top to last
 * @param top obj to free
 */
void KToken_freeAll(KToken *top) {
  KToken *p = top, *next;
  while (p != NULL) {
    next = p->next;
    KToken_free(p);
    p = next;
  }
}

void KToken_appendTokenStr(KToken *t, SString *s, s_bool deco) {
  SString *tmp;
  
  tmp = KToken_toString(t);
  if (deco) SString_append(s, "(");
  SString_append(s, tmp->ptr);
  if (deco) SString_append(s, ")");
  SString_free(tmp);
}

/**
 * token to string
 * @param t
 * @return string to debug
 * @see KToken_printAll()
 */
SString *KToken_toString(KToken *t) {
  SString *s = SString_new(NULL);
  char *lp;

  if (t == NULL) {
    SString_append(s, "NULL");
    return s;
  }

  lp = KTokenDef_toString(t->token_type);
  if (lp == NULL) {
    SString_appendIntFmt(s, "Unknown Token 0x%04X", t->token_type);
    return s;
  }
  SString_append(s, lp);
  SString_append(s, ":");
  // option
  switch (t->token_type) {
    case KTOKEN_MARKER:
      SString_appendInt(s, t->no);
      break;
    case KTOKEN_NOTE:
      SString_appendChar(s, (char)t->no);
      break;
    case KTOKEN_OCTAVE_UD:
      SString_append(s, (t->flag > 0) ? ">" : "<");
      break;
    case KTOKEN_NUMBER:
      SString_appendIntFmt(s, "%d", t->no);
      SString_appendIntFmt(s, "(0x%02X)", t->no);
      break;
    case KTOKEN_STRING:
      SString_append(s, t->value->ptr);
      break;
    case KTOKEN_VARIABLE:
      SString_append(s, t->value->ptr);
      break;
    case KTOKEN_DEF_INT:
      SString_append(s, t->value->ptr);
      break;
    case KTOKEN_DEF_STR:
      SString_append(s, t->value->ptr);
      break;
    case KTOKEN_DEF_ARRAY:
      SString_append(s, t->value->ptr);
      break;
    case KTOKEN_DEF_FUNCTION:
      SString_append(s, t->value->ptr);
      break;
    case KTOKEN_DO_VARIABLE:
      SString_append(s, t->value->ptr);
      break;
  }
  return s;
}

int KToken_count(KToken *t) {
  KToken *tt = t;
  int cnt = 0;
  while (tt != NULL) {
    tt = tt->next;
    cnt++;
  }
  return cnt;
}

void KToken_printAll_sub(KToken *tok, int level) {
  int i, t;
  KToken *a, *b;
  SString *s;
  KToken *nt;
  KTokenFlowOption *opt;
  KFunction *f;
  
  // token
  printf("[T] ");
  s = KToken_toString(tok);
  for (i = 0; i < level; i++) printf("|- ");
  printf("%s\n", s->ptr);
  SString_free(s);
  if (tok == NULL) return;
  // show arguments
  t = tok->token_type;
  if ((KTOKEN_DEF_INT <= t && t <= KTOKEN_DEF_ARRAY) || 
    t == KTOKEN_SYS_EX ||
    t == KTOKEN_RETURN || t == KTOKEN_PRINT) {
    nt = tok->arg;
    while (nt != NULL) {
      KToken_printAll_sub(nt, level + 1);
      nt = nt->next;
    }
  }
  // Calc Token
  else if (KToken_isCalcOp(t)) {
    a = tok->arg;
    KToken_printAll_sub(a, level + 1);
    b = tok->extra;
    KToken_printAll_sub(b, level + 1);
  }
  // Script Token
  else if (t == KTOKEN_CALL_USER_FUNC) {
    nt = tok->arg;
    while (nt != NULL) {
      KToken_printAll_sub(nt, level + 1);
      nt = nt->next;
    }
    f = (KFunction*)tok->extra;
    nt = f->tokens;
    while (nt != NULL) {
      KToken_printAll_sub(nt, level + 1);
      nt = nt->next;
    }
  }
  else if (t == KTOKEN_IF) {
    opt = (KTokenFlowOption*)tok->extra;
    nt = opt->true_t;
    while (nt != NULL) {
      KToken_printAll_sub(nt, level + 1);
      nt = nt->next;
    }
    nt = opt->false_t;
    while (nt != NULL) {
      KToken_printAll_sub(nt, level + 1);
      nt = nt->next;
    }
  }
}

/**
 * for debug : print all tokens
 * @param top
 * @see KToken_toString()
 */
void KToken_printAll(KToken *top) {
  KToken *p = top;
  while (p != NULL) {
    KToken_printAll_sub(p, 0);
    p = p->next;
  }
}

void KTokenFlowOption_free(void *token) {
  KToken *t = (KToken*)token;
  KTokenFlowOption *opt = (KTokenFlowOption*)t->extra;
  // [warn] opt->true_t/false_t already added skr->file_top
  //        so no need to free these token
  // KToken_freeAll(opt->true_t);
  // KToken_freeAll(opt->false_t);
  s_free(opt);
}



/**
 * create KFile object
 * @param *filename
 * @param *source
 */
KFile *KFile_new(const char *filename, const char *source) {
  KFile *file = s_new(KFile);
  file->filename = SString_new(filename);
  file->source = SString_new(source);
  return file;
}

/**
 * free KFile Object
 * @param *file
 * @see KFile_new()
 * @see KFile_freeAll()
 */
void KFile_free(KFile *file) {
  SString_free(file->filename);
  SString_free(file->source);
  KToken_freeAll(file->token_top);
  s_free(file);
}

/**
 * free all KFile from top to last
 * @see KFile_free()
 */
void KFile_freeAll(KFile *file_top) {
  KFile *file, *next;
  file = file_top;
  while (file != NULL) {
    next = file->next;
    KFile_free(file);
    file = next;
  }
}

/**
 * append token
 * @param file
 * @param t
 */
void KFile_appendToken(KFile *file, KToken *t) {
  if (t == NULL) return;
  if (t != file->token_cur) {
    file->token_cur->next = t;
  }
  // find tail node for multi length node
  while (t->next != NULL) {
    t = t->next;
  }
  file->token_cur = t;
}


void KLoopStack_free(SList *list) {
  KLoopItem *item;
  for (;;) {
    item = SList_pop(list);
    if (item == NULL) break;
    s_free(item);
  }
  SList_free(list);
}

void KValueStack_free(SList *list) {
  KLoopItem *item;
  for (;;) {
    item = SList_pop(list);
    if (item == NULL) break;
    s_free(item);
  }
  SList_free(list);
}

// KTokenNoteOnOption
KTokenNoteOnOption *KTokenNoteOnOption_new() {
  KTokenNoteOnOption *o = s_new(KTokenNoteOnOption);
  
  o->v = 0;
  o->q = 0;
  o->t = 0;
  o->v_mode = o->q_mode = o->t_mode = K_NOTEON_MODE_UNSET;
  o->q_step_mode = S_FALSE;
  
  return o;
}
void KTokenNoteOnOption_free(void *token) {
  KToken *t = (KToken*)token;
  KTokenNoteOnOption *opt = (KTokenNoteOnOption*)t->extra;
  s_free(opt);
  t->extra = NULL;
}





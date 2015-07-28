/**
 * @file k_token.h
 * @author kujirahand.com
 * Token/File Object
 */

#ifndef sakurammlc_stoken_h
#define sakurammlc_stoken_h

#include "s_str.h"
#include "k_token_def.h"

typedef void (* KTokenFreeFunc)(void *token);

/**
 * Token Object
 */
typedef struct _KToken {
  int token_type;         /// token type
  SString *value;         /// string value
  int no;                 /// data no
  int flag;               /// data flag (-1 / 0:direct / 1)
  char *pos;              /// Data Pos
  struct _KToken *arg;    /// Token Argument
  void *extra;            /// Extra data
  void *cache;            /// for cache contents
  KTokenFreeFunc free_f;  /// Free Func
  struct _KToken *next;
} KToken;

/**
 * NoteOn option
 */
typedef struct {
  // ノート[音長][,ゲート][,ベロシティ][,タイミング]
  // Note len,q,v,t
  int v;
  int v_mode;
  int q;
  int q_mode;
  s_bool q_step_mode;
  int t;
  int t_mode;
} KTokenNoteOnOption;

#define K_NOTEON_MODE_UNSET     0
#define K_NOTEON_MODE_ABSOLUTE  1
#define K_NOTEON_MODE_RELATIVE  2

/**
 * If Token option
 */
typedef struct {
  KToken *true_t;
  KToken *false_t;
} KTokenFlowOption;

/**
 * Source file
 */
typedef struct _KFile {
  SString *filename;
  SString *source;
  char *pos;
  struct _KToken *token_top, *token_cur;
  struct _KFile *next;
} KFile;

/**
 * Loop object
 */
typedef struct {
  KToken *begin;
  KToken *end;
  int index;
  int count;
} KLoopItem;


// KToken method
#define KToken_new(token_type, pos) KToken_new_(token_type, pos, __FILE__, __LINE__)

KToken *KToken_new_(int token_type, char *pos, const char *file, int lineno);
void KToken_freeAll(KToken *top);
void KToken_free(KToken *t);
void KToken_printAll(KToken *top);
SString *KToken_toString(KToken *t);
int KToken_count(KToken *t);

void KTokenFlowOption_free(void *token);

// KFile method
KFile *KFile_new(const char *filename, const char *source);
void KFile_free(KFile *file);
void KFile_freeAll(KFile *file);
void KFile_appendToken(KFile *file, KToken *t);

// KLoopItem method
void KLoopStack_free(SList *list);
void KValueStack_free(SList *list);

// KTokenNoteOnOption
KTokenNoteOnOption *KTokenNoteOnOption_new();
void KTokenNoteOnOption_free();

#endif



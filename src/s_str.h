/**
 * @file s_str.h
 */
#ifndef __S_STR_H__
#define __S_STR_H__

#include "s_types.h"

// s_str
#define s_str_dup(str) s_str_dup_(str, __FILE__, __LINE__)

char* s_str_dup_(const char *str, const char *file, int line);
int s_readInt10n(char *s, int def, char **endp);
int s_readInt(char *p, int def, char **endp);
int s_readHex(char *p, int def, char **endp);
int s_hexCharToInt(char c);
size_t s_utf8_width(const char *p);
size_t s_strlcpy(char *dst, const char *src, size_t size);

// SString
typedef struct {
  char *ptr;
  size_t len;
  size_t size;
} SString;

// SStingList
typedef struct {
  SString** ptr;
  size_t len;
  size_t size;
} SStringList;


// SString functions
#define SString_new(str) SString_new_(str, __FILE__, __LINE__)
#define SString_free(str) SString_free_(str, __FILE__, __LINE__)

SString *SString_new_(const char *str, const char *file, int line);
void SString_free_(SString* s, const char *file, int line);
size_t SString_append(SString *s, const char *str);
size_t SString_appendChar(SString *s, char ch); 
size_t SString_appendMem(SString *s, const char *str, size_t len);
size_t SString_appendSubstrUTF8(SString *s, const char *str, size_t len);
size_t SString_appendInt(SString *s, int no);
size_t SString_appendIntFmt(SString *s, const char *fmt, int no);

void SString_grow(SString *s, size_t newsize);
size_t SString_load(SString *s, const char *fname);
void SString_clear(SString *s);
SStringList *SString_split(SString *s, const char *delimiter, int count);
SString* SString_replace(SString *s, const char *key, const char *rep);
void SString_set(SString *s, const char *str);
size_t SString_insertTop(SString *s, const char *str);
SString *SString_substr(SString *s, int index, int len);

// SStringList functions
#define SStringList_new() SStringList_new_(__FILE__, __LINE__)
SStringList *SStringList_new_(const char *file, int line);
void SStringList_free(SStringList *list);
void SStringList_clear(SStringList *list);
void SStringList_grow(SStringList *list, size_t size);
size_t SStringList_append(SStringList *list, SString *s);
s_bool SStringList_delete(SStringList *list, size_t index);
size_t SStringList_indexOf(SStringList *list, const char *str);
size_t SStringList_find(SStringList *list, SString *s);
size_t SStringList_remove(SStringList *list, SString *s);
size_t SStringList_push(SStringList *list, SString *s);
SString* SStringList_pop(SStringList *list);
SString* SStringList_shift(SStringList *list);
SString* SStringList_join(SStringList *list, const char *glue);

// Macro for Parser
/** Skip Space Char */
#define SKIP_SPACE(pos) \
  while (*pos == '\t' || *pos == ' ') pos++;
#define SKIP_SPACE_N_SPLITEER(pos) \
  while (*pos == '\t' || *pos == ' ' || *pos == '|') pos++;
#define SKIP_SPACE_CRLF(pos) \
  while (*pos == '\t' || *pos == ' ' || *pos == '\r' || *pos == '\n') pos++;

#endif /* S_STR_H__ */




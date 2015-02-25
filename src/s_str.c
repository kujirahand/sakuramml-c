/**
 * string function for s_lib
 * @file s_str.c 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "s_lib.h"
#include "s_hook_memory.h"
#include "s_str.h"

#define SSTRING_DEFAULT_SIZE 256
#define SSTRING_FILE_BUF (1024 * 8)
#define SSTRINGLIST_DEFAULT_SIZE 32

/** new str */
char *s_str_dup_(const char *str, const char *file, int line) {
  size_t len = strlen(str);
  char *s = (char *)s_malloc_(len + 1, file, line);
  strcpy(s, str);
  return s;
}

/** get int from string */
int s_readInt10n(char *s, int def, char **endp) {
  int r = 0;
  char ch;

  if (!IS_NUMERIC(*s)) { r = def; goto finally_label; }
  while (*s != '\0') {
    if ('0' <= *s && *s <= '9') {
      ch = (*s - '0');
      r *= 10;
      r += ch;
      s++;
    } else break;
  }
finally_label:
  if (endp != NULL) *endp = (char*)s;
  return r;
}

int s_hexCharToInt(char c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  }
  if ('a' <= c && c <= 'f') {
    return c - 'a' + 10;
  }
  if ('A' <= c && c <= 'F') {
    return c - 'A' + 10;
  }
  return 0;
} 

int s_readHex(char *p, int def, char **endp) {
  // Hex Int
  int n;
  
  if (*p == '$' || memcmp(p, "0x", 2) == 0) {
    p += (*p == '$') ? 1 : 2;
  }
  if (!IS_NUMERIC_HEX(*p)) {
    return def;
  }
  n = s_hexCharToInt(*p);
  p++;
  while (IS_NUMERIC_HEX(*p)) {
    n <<= 4;
    n += s_hexCharToInt(*p);
    p++;
  }
  *endp = p;
  return n;
}

int s_readInt(char *p, int def, char **endp) {
  int n;
  if (p == NULL) return def;
  
  // Hex Int
  if (*p == '$' || memcmp(p, "0x", 2) == 0) {
    return s_readHex(p, def, endp);
  }
  
  // 10 Int
  if (!IS_NUMERIC(*p)) {
    return def;
  }
  n = s_hexCharToInt(*p);
  p++;
  while (IS_NUMERIC(*p)) {
    n *= 10;
    n += s_hexCharToInt(*p);
    p++;
  }
  *endp = p;
  return n;
}


size_t s_strlcpy(char *dst, const char *src, size_t size) {
	register char *d = dst;
	register const char *s = src;
	register size_t n = size;

	/* Copy as many bytes as will fit */
	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (size != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}



/** new SString */
SString *SString_new_(const char *str, const char *file, int line) {
  SString *s = (SString *)s_malloc_(sizeof(SString), file, line);
  if (str == NULL) {
    s->len = 0;
    s->size = SSTRING_DEFAULT_SIZE;
    s->ptr = s_new_a(char, SSTRING_DEFAULT_SIZE);
  } else {
    s->len = strlen(str);
    s->size = s->len + 1;
    if (s->size < SSTRING_DEFAULT_SIZE) {
      s->size = SSTRING_DEFAULT_SIZE;
    }
    s->ptr = (char *)s_malloc(s->size);
    strcpy(s->ptr, str);
  }
  return s;
}

void SString_grow(SString *s, size_t newsize) {
  char *tmp;
  if (s == NULL)
    return;
  if (newsize < s->size)
    return;
  tmp = (char *)s_malloc(newsize);
  memcpy(tmp, s->ptr, s->len + 1);
  s_free(s->ptr);
  s->ptr = tmp;
}

void SString_free_(SString* s, const char *file, int line) {
  if (s == NULL) return;
  if (s->ptr != NULL) s_free_(s->ptr, file, line);
  s_free_(s, file, line);
}

size_t SString_append(SString *s, const char *str) {
  size_t len;
  
  if (str == NULL) return 0;
  len = strlen(str);
  return SString_appendMem(s, str, len);
}

size_t SString_appendChar(SString *s, char ch) {
  char *p;
  size_t len = s->len + 1;
  if (len >= s->size) {
    SString_grow(s, len + 1);
  }
  p = s->ptr + s->len;
  *p = ch;
  *(p+1) = '\0';
  s->len++;
  return len;
}

size_t SString_appendInt(SString *s, int no) {
  char buf[256];
  sprintf(buf, "%d", no);
  return SString_append(s, buf);
} 

size_t SString_appendIntFmt(SString *s, const char *fmt, int no) {
  char buf[1024];
  sprintf(buf, fmt, no);
  return SString_append(s, buf);
} 



size_t s_utf8_width(const char *p) {
  if (*p == '\0') return 0;
  // 1ch
  if ((unsigned char)*p <= 0x7F) return 1;
  // 2ch
  if (IN_RANGE(0xC2, 0xDF, (unsigned char)*p)) return 2;
  // 3ch
  if (IN_RANGE(0xE0, 0xEF, (unsigned char)*p)) return 3;
  // 4ch
  if (IN_RANGE(0xF0, 0xF7, (unsigned char)*p)) return 4;
  // 5ch
  if (IN_RANGE(0xF8, 0xFB, (unsigned char)*p)) return 5;
  // 6ch
  if (IN_RANGE(0xFC, 0xFD, (unsigned char)*p)) return 6;
  // other
  return 1;
}

size_t SString_appendSubstrUTF8(SString *s, const char *str, size_t len) {
  char *p;
  size_t append_len = 0;
  size_t chw;
  
  p = (char*)str;
  while (len > 0) {
    if (*p == '\0') break;
    chw = s_utf8_width(p);
    len--;
    SString_appendMem(s, p, chw);
    p += chw;
    append_len += chw;
  }
  
  return append_len;
}

size_t SString_appendMem(SString *s, const char *str, size_t len) {
  size_t newlen = s->len + len;
  size_t newsize = newlen + 1;
  char *tmp, *p;
  // grow
  if (newsize > s->size) {
    newsize *= 2; // for less resize
    tmp = (char *)s_malloc(newsize);
    memcpy(tmp, s->ptr, s->len);
    // copy
    p = tmp;
    p += s->len;
    // change ptr
    s_free(s->ptr);
    s->ptr = tmp;
  } else {
    p = s->ptr;
    p += s->len;
  }
  memcpy(p, str, len);
  p += len;
  *p = '\0';
  s->len = newlen;
  return newlen;
}

void SString_clear(SString *s) {
  if (s == NULL)
    return;
  memset(s->ptr, 0, s->size);
  s->len = 0;
}

SString* SString_replace(SString *s, const char *key, const char *rep) {
  char *p;
  char *tmp;
  SString *res;
  size_t sz;
  
  res = SString_new(s->ptr);
  while ((p = strstr(res->ptr, key)) != NULL) {
    *p = '\0';
    p += strlen(key); // next pos
    sz = (res->len + strlen(rep) + 1);
    tmp = s_new_a(char, sz);
    strcpy(tmp, p); // copy after pos
    strcat(res->ptr, rep);
    strcat(res->ptr, tmp);
    s_free(tmp);
  }
  return res;
}

void SString_set(SString *s, const char *str) {
  size_t len;
  len = strlen(str);
  if (s->size <= (len + 1)) {
    SString_grow(s, (len * 2 + 1));
  }
  strcpy(s->ptr, str);
  s->len = len;
}

size_t SString_load(SString *s, const char *fname) {
  size_t sz;
  char buf[SSTRING_FILE_BUF];
  FILE *fp = fopen(fname, "rb");
  if (fp == NULL) {
    s_lib_addError(S_ERROR_IO, "File Open Error", __FILE__, __LINE__);
    return 0;
  }
  fseek(fp, 0, SEEK_SET);
  SString_clear(s);
  while (1) {
    sz = fread(buf, 1, SSTRING_FILE_BUF, fp);
    if (sz > 0)
      SString_appendMem(s, buf, sz);
    if (sz < SSTRING_FILE_BUF)
      break;
  }
  return s->len;
}

SStringList *SString_split(SString *s, const char *delimiter, int count) {
  SStringList *list;
  char *ss, *sp;
  SString *sub;
  size_t dlen = strlen(delimiter);
  size_t i, cnt = count;

  list = SStringList_new();
  sp = ss = s->ptr;

  while (*ss != '\0') {
    if (strncmp(ss, delimiter, dlen) == 0) {
      sub = SString_new(NULL);
      SString_appendMem(sub, sp, (ss - sp));
      SStringList_push(list, sub);
      cnt--;
      if (count > 0 && cnt <= 1)
        break;
      for (i = 0; i < dlen; i++) {
        if (*ss != '\0')
          ss++;
      }
      sp = ss;
    } else {
      ss++;
    }
  }
  if (sp != ss) {
    sub = SString_new(NULL);
    SString_appendMem(sub, sp, (ss - sp));
    SStringList_push(list, sub);
  }

  return list;
}

size_t SString_insertTop(SString *s, const char *str) {
  size_t sz = strlen(str);
  size_t newlen = s->len + sz;
  char *tmp = (char*)s_malloc(newlen + 1);
  strcpy(tmp, str);
  strcat(tmp, s->ptr);
  s_free(s->ptr);
  s->ptr = tmp;
  s->size = newlen + 1;
  s->len = newlen;
  return s->size;
}

SString *SString_substr(SString *s, int index, int len) {
  SString *res;
  char *p;
  
  res = SString_new(NULL);
  if (len <= 0) return res;
  SString_grow(res, len + 1);
  // index len check
  if (index >= s->len) return res;
  p = s->ptr + index;
  // len
  s_strlcpy(res->ptr, p, len + 1);
  return res;
}


/** SStringList */
SStringList *SStringList_new_(const char *file, int line) {
  SStringList *obj = s_new(SStringList);
  obj->size = SSTRINGLIST_DEFAULT_SIZE;
  obj->len = 0;
  obj->ptr = s_new_a(SString *, obj->size);
  return obj;
}

void SStringList_free(SStringList *list) {
  size_t i;
  SString *p;
  if (list == NULL)
    return;
  for (i = 0; i < list->len; i++) {
    p = list->ptr[i];
    SString_free(p);
  }
  s_free(list->ptr);
  s_free(list);
}

SString *SStringList_join(SStringList *list, const char *glue) {
  size_t i;
  SString *s, *item;

  s = SString_new("");
  for (i = 0; i < list->len; i++) {
    item = list->ptr[i];
    SString_append(s, item->ptr);
    if (i != (list->len - 1))
      SString_append(s, glue);
  }
  return s;
}

void SStringList_clear(SStringList *list) {
  list->len = 0;
  memset(list->ptr, 0, list->size);
}

void SStringList_grow(SStringList *list, size_t size) {
  SString **ptr;
  // if wrong size -> fix size
  if (size < list->len) {
    size = list->len * 2;
  }
  ptr = s_new_a(SString *, size);
  memcpy(ptr, list->ptr, list->len);
  s_free(list->ptr);
  list->ptr = ptr;
}

size_t SStringList_append(SStringList *list, SString *s) {
  // check size
  if (list->size < list->len + 1) {
    SStringList_grow(list, list->size * 2);
  }
  // add SString
  list->ptr[list->len] = s;
  list->len++;
  return list->len;
}

s_bool SStringList_delete(SStringList *list, size_t index) {
  SString **items;
  size_t i, j;
  
  if (index >= list->len)
    return S_FALSE;
  // (ex) 0123456
  // (ex) 3 | 012*456
  items = s_new_a(SString*, list->size);
  
  // TODO: 非効率なアルゴリズムのため修正する
  j = 0;
  for (i = 0; i < list->len; i++) {
    if (i != index) {
      items[j] = list->ptr[i];
      j++;
    }
  }
  SString_free(list->ptr[index]);
  s_free(list->ptr);
  list->ptr = items;
  list->len--;
  
  return S_TRUE;
}

size_t SStringList_indexOf(SStringList *list, const char *str) {
  size_t i;
  SString *p;
  for (i = 0; i < list->len; i++) {
    p = list->ptr[i];
    if (strcmp(str, p->ptr) == 0)
      return i;
  }
  return (size_t) - 1;
}

size_t SStringList_find(SStringList *list, SString *s) {
  size_t i;
  SString *p;
  for (i = 0; i < list->len; i++) {
    p = list->ptr[i];
    if (p == s)
      return i;
  }
  return (size_t) - 1;
}

size_t SStringList_remove(SStringList *list, SString *s) {
  size_t i = SStringList_find(list, s);
  if (i == (size_t) - 1)
    return (size_t) - 1;
  SStringList_delete(list, i);
  return S_TRUE;
}

size_t SStringList_push(SStringList *list, SString *s) {
  return SStringList_append(list, s);
}

SString *SStringList_pop(SStringList *list) {
  SString *tmp;
  if (list->len == 0)
    return NULL;
  tmp = list->ptr[list->len - 1];
  list->ptr[list->len - 1] = NULL;
  list->len--;
  return tmp;
}

SString *SStringList_shift(SStringList *list) {
  SString *p;
  if (list->len == 0)
    return NULL;
  p = list->ptr[0];
  list->ptr++;
  list->len--;
  return p;
}

/**
 * @file s_lib.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s_lib.h"
#include "s_str.h"

typedef struct s_lib_error_t {
  int code;
  char *msg;
  const char* file;
  int line;
  struct s_lib_error_t *next;
} s_lib_error_t;

typedef struct s_lib_cb_t {
  s_lib_callback_ptr callback;
  struct s_lib_cb_t *next;
} s_lib_cb_t;

s_lib_cb_t *s_lib_callback_top = NULL;
s_lib_error_t *s_lib_error_top = NULL;

void s_lib_addError(int code, char *msg, const char* file, int line) {
  s_lib_error_t *p;
  s_lib_error_t *t = s_new(s_lib_error_t);

  t->code = code;
  t->msg = msg;
  t->file = file;
  t->line = line;
  t->next = NULL;

  if (s_lib_error_top == NULL) {
    s_lib_error_top = t;
  } else {
    p = s_lib_error_top;
    while (p->next != NULL) p = p->next;
    p->next = t;  
  } 
}

int s_lib_get_lastError() {
  s_lib_error_t *p = s_lib_error_top;
  if (p == NULL) return 0;
  while (p->next != NULL) p = p->next;
  return p->code;
}

char *s_lib_get_errors() {
  char buf[1024 * 4] = ""; // 4KB buf
  SString *str;
  s_lib_error_t *p = s_lib_error_top;
  char *res = NULL;
  if (p == NULL) return NULL;
  str = SString_new("");
  while (p != NULL) {
    p = p->next;
    sprintf(buf, "[ERROR] %s(%d): %s\n", p->file, p->line, p->msg);
    SString_append(str, buf);
  }
  res = (char *)s_malloc(str->len + 1);
  strcpy(res, str->ptr);
  SString_free(str);
  return res;
}


void s_lib_addFreeFunc(s_lib_callback_ptr callback) {
  s_lib_cb_t *last;
  s_lib_cb_t *p;

  // new memory
  p = s_new(s_lib_cb_t);
  p->callback = callback;
  p->next = NULL;

  if (s_lib_callback_top == NULL) {
    s_lib_callback_top = p;
  }

  last = s_lib_callback_top;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = p;
}

void s_lib_free() {
  s_lib_cb_t *cur, *tmp;

  if (s_lib_callback_top == NULL) return;

  cur = s_lib_callback_top;
  while (cur != NULL) {
    tmp = cur;
    cur->callback();
    cur = cur->next;
    s_free(tmp);
  }
}

/** file exists?  */
s_bool file_exists(const char *filename) {
  FILE  *fp;

  if ((fp = fopen(filename, "r")) == NULL) {
    return S_FALSE;
  } else {
    fclose(fp);
    return S_TRUE;
  }
}

char* file_changeExt(const char *fname, const char *ext) {
  char *p, *buf, *last;
  
  // copy to buffer
  buf = s_malloc(strlen(fname) + strlen(ext));
  strcpy(buf, fname);
  // scan last period
  last = buf + strlen(buf);
  p = buf;
  while (*p != '\0') {
    if (*p == '.') last = p;
    p++;
  }
  strcpy(last, ext);
  
  return buf;
}

int s_pow(int v, int count) {
  int r = 1, i;
  for (i = 0; i < count; i++) {
    r *= v;
  }
  return r;
}

/**
 * @file s_stream.c
 */

#include <string.h>
#include <stdlib.h>
#include "s_stream.h"
#include "s_lib.h"

/** create SStream */
SStream *SStream_new_(const char *file, int line) {
  SStream *st;

  st = (SStream*)s_malloc_(sizeof(SStream), file, line);
  st->size = 256;
  st->len = 0;
  st->ptr = s_new_a(s_byte, st->size);

  return st;
}

/** release SStream  */
void SStream_free(SStream *st) {
  s_free(st->ptr);
  s_free(st);
}

/** grow memory */
void SStream_grow(SStream *st, size_t size) {
  s_byte *newptr;

  // need resize?
  if (st->size > size) return;
  
  // create new size
  newptr = s_new_a(s_byte, size);
  memcpy(newptr, st->ptr, st->len);
  s_free(st->ptr);
  st->ptr = newptr;
  st->size = size;
}

/** set index */
void SStream_seek(SStream *st, size_t index) {
  st->index = index;
  if (st->len >= index) {
    st->index = index - 1;
  } 
}

/** write data 
 * @param *st SStream object
 * @param *buf write data pointer
 * @param len data length
 * @return SStream total size
 */
size_t SStream_write(SStream *st, const s_byte *buf, size_t len) {
  size_t newi;
  s_byte *p;

  // 012345678
  // ******--
  //     abcd
  // (ex) st->index = 4
  //      st->len = 6
  //      len = 4

  newi = st->index + len;
  if (st->size <= newi) {
    SStream_grow(st, newi * 2);
  }
  if (st->len < newi) {
    st->len = newi;
  }
  p = st->ptr + st->index;
  memcpy(p, buf, len);
  st->index += len;
  return st->len;
}

size_t SStream_read(SStream *st, s_byte *buf, size_t len) {
  s_byte *p;
  size_t newi;

  // 0123456789
  // *****
  //   +----
  // (ex) st->index = 2
  //      st->len   = 5
  //      len = 5 (real: 3)

  newi = st->index + len;
  if (st->len > newi) {
    len = st->len - st->index;
  }
  p = st->ptr + st->index;
  memcpy(buf, p, len);
  st->index += len;

  return len;
}





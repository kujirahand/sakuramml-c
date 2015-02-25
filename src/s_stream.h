/**
 * @file s_stream.h
 */

#ifndef __s_stream_h__
#define __s_stream_h__

#include <stdlib.h>
#include "s_types.h"

/** SStream Type */
typedef struct {
  size_t size;
  size_t len;
  size_t index;
  s_byte *ptr;
} SStream;

/** SStream function */

#define SStream_new() SStream_new_(__FILE__, __LINE__)
SStream *SStream_new_(const char *file, int line);
void SStream_free(SStream *st);
void SStream_grow(SStream *st, size_t size);
void SStream_seek(SStream *st, size_t index);
size_t SStream_write(SStream *st, const s_byte *buf, size_t len);
size_t SStream_read(SStream *st, s_byte *buf, size_t len);

#endif /* __s_stream_h__ */


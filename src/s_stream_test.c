#include "s_test.h"
#include "s_lib.h"
#include "k_test.h"
#include "s_stream.h"

void s_stream_test() {
  char buf[5] = {'a','b','c','d'};
  char tmp[32];
  SStream *strm;
  int i;
  // (1)
  strm = SStream_new();
  SStream_write(strm, (s_byte*)buf, 4);
  SStream_write(strm, (s_byte*)buf, 4);
  SStream_seek(strm, 0);
  SStream_read(strm, (s_byte*)tmp, 8);
  tmp[8] = '\0';
  s_test_str_eq(tmp, "abcdabcd", "SStream:1");
  SStream_free(strm);
  
  // (2)
  strm = SStream_new();
  for (i = 0; i < 1024; i++) {
    SStream_write(strm, (s_byte*)buf, 2);
  }
  SStream_seek(strm, 0);
  SStream_read(strm, (s_byte*)tmp, 8);
  tmp[8] = '\0';
  s_test_str_eq(tmp, "ababab", "SStream:2");
  SStream_free(strm);
  
}
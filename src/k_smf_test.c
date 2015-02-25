
#include <stdio.h>
#include <stdlib.h>
#include "k_smf.h"
#include "k_smf_writer.h"
#include "s_lib.h"
#include "s_test.h"


void k_smf_test_delta() {
  SStream *d1 = makeDelta(0x7f);
  s_test_eq(d1->len, 1, "makeDelta:1");
  s_test_eq(d1->ptr[0], 0x7f, "makeDelta:1");
  SStream_free(d1);
  //
  d1 = makeDelta(0x80);
  // printf("[%d:%d]", d1->ptr[0], d1->ptr[1]);
  s_test_eq(d1->len, 2, "makeDelta:2:len");
  s_test_eq(d1->ptr[0], 0x81, "makeDelta:2:byte1");
  s_test_eq(d1->ptr[1], 0x00, "makeDelta:2:byte2");
  SStream_free(d1);
  //
}


void k_smf_test() {
  
  k_smf_test_delta();

  KSmfEvent *e;
  KSmfTrack *tr = KSmfTrack_new();

  e = KSmfEvent_new();
  e->time = 100;
  KSmfTrack_append(tr, e);

  e = KSmfEvent_new();
  e->time = 80;
  KSmfTrack_append(tr, e);

  KSmfTrack_sort(tr);

  s_test_eq(tr->ptr[0]->time, 80, "sort1");
  s_test_eq(tr->ptr[1]->time, 100, "sort2");

  KSmfTrack_free(tr);
}




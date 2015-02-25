
#include <stdio.h>
#include <string.h>

#include "k_track.h"
#include "s_lib.h"

/** Create KTrackList */
KTrackList *KTrackList_new(KSongInfo *info) {
  KTrackList *list = s_new(KTrackList);
  list->info = info;
  list->size = 256;
  list->len = 0;
  list->ptr = s_new_a(KTrack*, list->size);

  return list;
}

/** Release KTrackList */ 
void KTrackList_free(KTrackList *list) {
  int i ;
  KTrack *p;
  
  // release all track
  for (i = 0; i < list->len; i++) {
    p = list->ptr[i];
    KTrack_free(p);
  }
  s_free(list->ptr);

  // release KTrackList
  s_free(list);
}

void KTrackList_grow(KTrackList *list, int size) {
  int newsize;
  KTrack **newptr;
  
  if (list->size <= size) {
    newsize = size * 2;
    newptr = s_new_a(KTrack*, newsize);
    // copy
    memcpy(newptr, list->ptr, list->len);
    s_free(list->ptr);
    list->ptr = newptr;
    list->size = newsize;
  }  
}

/** get track */
KTrack *KTrackList_get(KTrackList *list, int index) {
  KTrack *tr;
  
  // over size => grow
  KTrackList_grow(list, index + 1);
  // new track ?
  if (list->len <= index) {
    list->len = index + 1;
  }
  if (list->ptr[index] == NULL) {
    tr = list->ptr[index] = KTrack_new(list->info->timebase);
    tr->ch = index + 1; // default channel
  }
  return list->ptr[index];
}

s_bool KTrackList_exists(KTrackList *list, int index) {
  KTrackList_grow(list, index + 1);
  return (list->ptr[index] != NULL);
}

/** set trackbase */
void KTrackList_setTimebase(KTrackList *list, int timebase) {
  int i;
  KTrack *t;

  list->info->timebase = timebase;
  // change every track timebase
  for (i = 0; i < list->len; i++) {
    t = list->ptr[i];
    if (t != NULL) {
      t->l = timebase;
    }
  }
}


KTrack *KTrack_new_(int timebase, const char *file, int lineno) {
  KTrack *p = (KTrack*)s_malloc_(sizeof(KTrack), file, lineno);
  p->time = 0;
  p->track_key = 0;
  p->ch = 1;
  p->o = 5;
  p->q = 80;
  p->l = timebase;
  p->v = 100;
  p->v_onNote = SList_new();
  p->v_onNote->free_item_f = SValue_free_retTrue;
  p->t_onNote = SList_new();
  p->t_onNote->free_item_f = SValue_free_retTrue;
  p->vAdd = 8;
  return p;
}

void KTrack_free(KTrack *t) {
  if (t == NULL) return;
  SList_free(t->v_onNote);
  SList_free(t->t_onNote);
  s_free(t);
}




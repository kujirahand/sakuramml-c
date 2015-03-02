
#include <string.h>
#include <stdio.h>

#include "k_smf.h"
#include "s_lib.h"
#include "s_test.h"
#include "k_main.h"

/* Merge Sort for track */
void KSmfTrack__mergeSort(KSmfEvent **x, int left, int right, KSmfEvent **temp);


KSmfEvent *KSmfEvent_new() {
  KSmfEvent *e = s_new(KSmfEvent);
  e->meta = NULL;
  e->ch = 1;
  return e;
}

void KSmfEvent_free(KSmfEvent *event) {
  s_free(event->meta);
  s_free(event);
}

KSmfEvent *KSmfEvent_newCC(int time, int ch, int no, int value) {
  KSmfEvent *e = KSmfEvent_new();
  e->time = time;
  e->status = 0xB0;
  e->ch = ch;
  e->data1 = no;
  e->data2 = value;
  return e;
}

KSmfEvent *KSmfEvent_newPitchBend(int time, int ch, int value) {
  int n;
  KSmfEvent *e;
  // En mm ll = -8192,0,8191 = 0x0000,0x4000,0x7F7F
  e = KSmfEvent_new();
  e->time = time;
  e->status = 0xE0;
  e->ch = ch;
  n = SET_IN_RANGE(-8192, 8191, value) + 8192;
  e->data2 = n & 0x7F;
  n = n >> 7;
  e->data1 = n & 0x7F;
  return e;
}

KSmfTrack *KSmfTrack_new() {
  KSmfTrack *track = s_new(KSmfTrack);
  track->size = 256;
  track->len = 0;
  track->ptr = s_new_a(KSmfEvent*, track->size);
  return track;
}

void KSmfTrack_free(KSmfTrack *track) {
  int i;
  KSmfEvent *e;

  if (track == NULL) return;
  for (i = 0; i < track->len; i++) {
    e = track->ptr[i];
    KSmfEvent_free(e);    
  }
  s_free(track->ptr);
  s_free(track);
}

void KSmfTSrack_grow(KSmfTrack *track, int newsize) {
  KSmfEvent **newptr;
  int i;
  //
  newptr = s_new_a(KSmfEvent*, newsize);
  for (i = 0; i < track->len; i++) {
    newptr[i] = track->ptr[i];
  }
  s_free(track->ptr);
  track->ptr = newptr;
  track->size = newsize;
}

int KSmfTrack_append(KSmfTrack *track, KSmfEvent *event) {
  int newlen;
  
  // todo: for DEBUG
  if (event == NULL) {
    fprintf(stderr, "[Fatal System Error] KSmfTrack_append, event == NULL\n");
    return -1;
  }

  // check new size
  newlen = track->len + 1;
  if (track->size <= newlen) {
    KSmfTSrack_grow(track, newlen * 2);
  }

  // append
  track->ptr[track->len] = event;
  track->len++;
  return track->len;
}

void KSmfTrack__mergeSort(KSmfEvent **x, int left, int right, KSmfEvent **temp) {
  int mid, i, j, k;
  
  // only one element, no need to sort
  if (left >= right) return;
  
  mid = (left + right) / 2; //center
  KSmfTrack__mergeSort(x, left, mid, temp); // left
  KSmfTrack__mergeSort(x, mid + 1, right, temp); // right

  // copy to temp
  for (i = left; i <= mid; i++) {
    temp[i] = x[i];
  }
  for (i = mid + 1, j = right; i <= right; i++, j--) {
    temp[i] = x[j];
  }

  i = left;
  j = right;
  // sort from min
  for (k = left; k <= right; k++) {
    if (temp[i]->time <= temp[j]->time) {
      x[k] = temp[i++];
    } else {
      x[k] = temp[j--];
    }
  }
}

void KSmfTrack_sort(KSmfTrack *track) {
  KSmfEvent **temp;

  temp = s_new_a(KSmfEvent*, track->len);
  KSmfTrack__mergeSort(track->ptr, 0, track->len-1, temp);
  s_free(temp);
}

KSmfTrackList *KSmfTrackList_new() {
  KSmfTrackList *list = s_new(KSmfTrackList);
  list->size = 32;
  list->len = 0;
  list->ptr = s_new_a(KSmfTrack*, list->size);
  return list;
}

void KSmfTrackList_free(KSmfTrackList *list) {
  KSmfTrack *t;
  int i;
  for (i = 0; i < list->len; i++) {
    t = list->ptr[i];
    KSmfTrack_free(t);
  }
  s_free(list->ptr);
  s_free(list);
}

KSmfTrack *KSmfTrackList_get(KSmfTrackList *list, int index) {
  KSmfTrack **newptr;
  int newsize;
  
  if (list == NULL) return NULL;
  // check over size
  if (list->size <= index) {
    newsize = index * 2;
    newptr = s_new_a(KSmfTrack*, newsize);
    memcpy(newptr, list->ptr, list->len);
    s_free(list->ptr);
    list->ptr = newptr;
    list->size = newsize;
  }
  // cehck over length
  if (list->len <= index) {
    list->len = index + 1;
  }
  // auto create track
  if (list->ptr[index] == NULL) {
    list->ptr[index] = KSmfTrack_new();
  }
  return list->ptr[index];
}









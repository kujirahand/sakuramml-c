/**
 * SMF format
 * @file k_smf.h
 */

#ifndef __k_smf_h__
#define __k_smf_h__

#include "s_types.h"
#include "s_lib.h"

/** KSmfEvent */
typedef struct {
  int time;
  s_byte ch;
  s_byte status;
  s_byte data1;
  s_byte data2;
  s_byte *meta; // if meta data, data2 is meta length
} KSmfEvent;

/** KSmfTrack */
typedef struct {
  int size;
  int len;
  KSmfEvent **ptr;
} KSmfTrack;

/** KSmfTrackList */
typedef struct {
  int size;
  int len;
  KSmfTrack **ptr;
} KSmfTrackList; 


/** KSmfTrack function */
KSmfTrack *KSmfTrack_new();
void KSmfTrack_free(KSmfTrack *track);
int KSmfTrack_append(KSmfTrack *track, KSmfEvent *event);
void KSmfTrack_sort(KSmfTrack *track);
SStream *KSmfTrack_getStream(KSmfTrack *tr); 
void KSmfTSrack_grow(KSmfTrack *track, int newsize);

/** KSmfEvent function */
KSmfEvent *KSmfEvent_new();
void KSmfEvent_free(KSmfEvent *event);
KSmfEvent *KSmfEvent_newCC(int time, int ch, int no, int value);
KSmfEvent *KSmfEvent_newPitchBend(int time, int ch, int value);

/** KSmfTrackList function */
KSmfTrackList *KSmfTrackList_new();
void KSmfTrackList_free(KSmfTrackList *list);
KSmfTrack *KSmfTrackList_get(KSmfTrackList *list, int index);

#endif /* __k_smf_h__ */


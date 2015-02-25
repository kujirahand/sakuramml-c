/**
 * @file k_track.h
 */

#ifndef __k_track_h__
#define __k_track_h__

#include "k_types.h"
#include "s_list.h"

/** Track info */
typedef struct {
  int time;         /// track time
  int track_key;    /// track key
  int ch;           /// channel
  int o;            /// octave
  int v;            /// velocity
  int v_[10];       /// velocity(3)
  int t;            /// timing
  int q;            /// gate rate (percent)
  int l;            /// step
  int Voice;        /// Voice (Default=0)
  int Port;         /// Port (Default=0)
  int rpn_nrpn_time_shift; /// for RPN/NRPN
  int rpn_nrpn_time_last;  /// for RPN/NRPN
  int vAdd;         /// for vAdd
  SList *v_onNote;  /// for v.onNote
  SList *t_onNote;  /// for t.onNote
} KTrack;

/** Track List */
typedef struct {
  // for tracks
  KTrack **ptr;
  int size;
  int len;
  KSongInfo *info; // only link
} KTrackList;

/** KTrack function */
#define KTrack_new(timebase) KTrack_new_(timebase, __FILE__, __LINE__)
KTrack *KTrack_new_(int timebase, const char *file, int lineno);
void KTrack_free(KTrack *t);

/** KTrackList function */
KTrackList *KTrackList_new(KSongInfo *info);
void KTrackList_free(KTrackList *list);
KTrack *KTrackList_get(KTrackList *list, int index);
s_bool KTrackList_exists(KTrackList *list, int index);
void KTrackList_grow(KTrackList *list, int size);
void KTrackList_setTimebase(KTrackList *list, int timebase);

#endif /* __k_track_h__ */






/** 
 * @file: k_types.h
 * @author: kujirahand.com
 */

#ifndef K_TYPES_H
#define	K_TYPES_H

#include "s_types.h"

/**
 * song info 
 */
typedef struct {
  int track_no;
  int timebase;
  s_bool revOctaveFlag;
  s_bool is_harmony;
  int measureShift;
  int timeSig_deno; // bunbo
  int timeSig_nume; // bunsi
  int key;
  int octaveOnetime;
  int tempo;
  int keyflag[7];   // for KeyFlag
  int timeTrackSync; // for TrackSync
  int play_temp_track;
  int play_temp_time;
} KSongInfo;


#endif	/* K_TYPES_H */


/* 
 * File:   k_smf_writer.c
 * Author: kujira
 *
 * Created on 2014/12/31, 23:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s_lib.h"
#include "k_smf_writer.h"

/** write event to stream */
void KSmfWriter_writeEvent(SStream *strm, KSmfEvent *e) {
  s_byte buf[3];

  buf[0] = (e->ch - 1) | e->status;
  buf[1] = e->data1;
  buf[2] = e->data2;

  switch (e->status) {
    case 0x80: // note off
    case 0x90: // note on
    case 0xB0: // Control Change
      SStream_write(strm, buf, 3);
      break;
    case 0xC0: // Program Change
      SStream_write(strm, buf, 2);
      break;
    case 0xE0: // Pitch Bend
      SStream_write(strm, buf, 3);
      break;
    case 0xF0: // SysEx
      SStream_write(strm, buf, 2);
      SStream_write(strm, e->meta, e->data1);
      break;
    case 0xFF: // meta
      SStream_write(strm, buf, 3);
      SStream_write(strm, e->meta, e->data2);
      break;
    default:
      printf("[KSmfEvent_write] error status 0x%x\n", e->status);
      break;
  }
}

void KSmfWriter_sortAll(KSmfTrackList *list) {
  int i;
  KSmfTrack *tr;
  //
  for (i = 0; i < list->len; i++) {
    tr = list->ptr[i];
    if (tr == NULL) continue;
    KSmfTrack_sort(tr);
  }
}

/** save to file (smf format0) */
s_bool KSmfWriter_saveToSmf0(s_bool is_debug, KSmfTrackList *list, const char *filename, int timebase) {
  s_byte head_chunk[14];
  s_byte track_chunk[8];
  SStream *strm;
  FILE *fp;
  int i, j;
  s_bool result = S_TRUE;
  size_t slen;
  KSmfEvent *e;
  KSmfTrack *t;
  KSmfTrack *tr = KSmfTrack_new();

  // append all event to tr
  for (i = 0; i < list->len; i++) {
    t = list->ptr[i];
    if (t == NULL) continue;
    for (j = 0; j < t->len; j++) {
      e = t->ptr[j];
      KSmfTrack_append(tr, e);
    }
  }
  KSmfTrack_sort(tr);
  
  // debug
  /*
  for (i = 0; i < tr->len; i++) {
    e = tr->ptr[i];
    printf("| %04d:%02x,%02x,%02x\n", e->time, e->status, e->data1, e->data2);
  }
  */
  
  // get stream
  strm = KSmfTrack_getStream(tr);
  slen = strm->len;
  // printf("- streamed.\n");
  
  // write
  memset(head_chunk, 0, sizeof(head_chunk));
  head_chunk[0] = 0x4D; // chunk type "MThd"
  head_chunk[1] = 0x54;
  head_chunk[2] = 0x68;
  head_chunk[3] = 0x64;
  head_chunk[4] = 0x00; // data length
  head_chunk[5] = 0x00;
  head_chunk[6] = 0x00; 
  head_chunk[7] = 0x06;
  head_chunk[8] = 0x00; // format
  head_chunk[9] = 0x00;
  head_chunk[10] = 0x00; // track num
  head_chunk[11] = 0x01;
  head_chunk[12] = 0xFF & (timebase >> 8); // Timebase
  head_chunk[13] = 0xFF & timebase;
  //
  track_chunk[0] = 0x4D; // chunk type "MTrk"
  track_chunk[1] = 0x54;
  track_chunk[2] = 0x72;
  track_chunk[3] = 0x6B;
  track_chunk[4] = 0xFF & (slen >> 24);
  track_chunk[5] = 0xFF & (slen >> 16);
  track_chunk[6] = 0xFF & (slen >>  8);
  track_chunk[7] = 0xFF & slen;
  // write to file
  fp = fopen(filename, "wb");
  if (fp == NULL) {
    result = S_FALSE;
    goto finally_clear_label;
  }
  // smf chunk
  fseek(fp, 0L, SEEK_SET);
  fwrite(head_chunk, sizeof(head_chunk), 1, fp);
  fwrite(track_chunk, sizeof(track_chunk), 1, fp);
  fwrite(strm->ptr, slen, 1, fp);
  // smf track
  fclose(fp);
  
  // printf("- file wrote.\n");

finally_clear_label:
  // release strm
  SStream_free(strm);
  // clear event
  for (i = 0; i < tr->len; i++) {
    tr->ptr[i] = NULL;
  }
  tr->len = 0;
  KSmfTrack_free(tr);
  return result;
}

/** save to file (smf format1) */
s_bool KSmfWriter_saveToSmf1(SakuraObj *skr, const char *filename) {
  s_byte head_chunk[14];
  s_byte track_chunk[8];
  SStream *strm;
  FILE *fp;
  int track_nums;
  int i, j;
  size_t slen;
  KSmfTrack *tr;
  size_t sz;
  s_bool result = S_FALSE;
  KSmfEvent *e;
  KSmfTrackList *list = skr->smf;
  int timebase = skr->info->timebase;
  char *note_name[12] = {"c","c+","d","d+","e","f","f+","g","g+","a","a+","b"};
  
  // file open
  fp = fopen(filename, "wb");
  if (fp == NULL) return S_FALSE;
  fseek(fp, 0L, SEEK_SET);

  // sort all event
  KSmfWriter_sortAll(skr->smf);
  KSmfWriter_checkPlayFrom(skr);
  track_nums = skr->smf->len;
  
  if (skr->debug) {
    for (i = 0; i < list->len; i++) {
      tr = list->ptr[i];
      if (tr == NULL) continue;
      printf("+ Track=%d\n", i);
      for (j = 0; j < tr->len; j++) {
        e = tr->ptr[j];
        if (e->time < 0) continue;
        if (e->status == 0x80 || e->status == 0x90) {
          printf("- %04d:%02x,%02x,%02x | o%d%s\n",
            e->time, (e->status | e->ch -1), e->data1, e->data2,
            e->data1 / 12, note_name[e->data1 % 12]);
        } else {
          printf("- %04d:%02x,%02x,%02x\n", e->time, (e->status | e->ch -1), e->data1, e->data2);
        }
      }
    }
  }
  
  // write
  memset(head_chunk, 0, sizeof(head_chunk));
  head_chunk[0] = 0x4D; // chunk type "MThd"
  head_chunk[1] = 0x54;
  head_chunk[2] = 0x68;
  head_chunk[3] = 0x64;
  head_chunk[4] = 0x00; // data length
  head_chunk[5] = 0x00;
  head_chunk[6] = 0x00; 
  head_chunk[7] = 0x06;
  head_chunk[8] = 0x00; // format
  head_chunk[9] = 0x01;
  head_chunk[10] = 0xFF & (track_nums >> 8); // track num
  head_chunk[11] = 0xFF & track_nums;
  head_chunk[12] = 0xFF & (timebase >> 8); // Timebase
  head_chunk[13] = 0xFF & timebase;
  // to file
  sz = fwrite(head_chunk, sizeof(head_chunk), 1, fp);
  if (sz == 0) {
    k_error(NULL, K_ERROR_FILE_WRITE, filename, NULL);
    goto finally_close_file_label;
  }
  
  // every tracks
  for (i = 0; i < track_nums; i++) {
    tr = list->ptr[i];
    // get stream
    strm = KSmfTrack_getStream(tr);
    slen = strm->len;
    // Track Chunk
    track_chunk[0] = 0x4D; // chunk type "MTrk"
    track_chunk[1] = 0x54;
    track_chunk[2] = 0x72;
    track_chunk[3] = 0x6B;
    track_chunk[4] = 0xFF & (slen >> 24);
    track_chunk[5] = 0xFF & (slen >> 16);
    track_chunk[6] = 0xFF & (slen >>  8);
    track_chunk[7] = 0xFF & slen;
    // to file
    sz = fwrite(track_chunk, sizeof(track_chunk), 1, fp);
    if (sz == 0) {
      k_error(NULL, K_ERROR_FILE_WRITE, filename, NULL);
      goto finally_close_file_label;
    }
    sz = fwrite(strm->ptr, slen, 1, fp);
    if (sz == 0) {
      k_error(NULL, K_ERROR_FILE_WRITE, filename, NULL);
      goto finally_close_file_label;
    }
    SStream_free(strm);
  }
  result = S_TRUE;
  
 finally_close_file_label:
  fclose(fp);
  return result;
}

void KSmfWriter_checkPlayFrom(SakuraObj *skr) {
  int i, tr, ch, cc;
  int pf_time;
  KSmfTrack *smf, *smf_tr;
  KSmfEvent *e;
  int cc_cache[2048]; // = 16 * 128
  int voice_cache[16];
  
  // use playfrom?
  pf_time = skr->play_from;
  if (pf_time <= 0) return;
  
  // cache clear
  for (i = 0; i < 2048; i++) {
    cc_cache[i] = -1; // not set
  }
  for (i = 0; i < 16; i++) {
    voice_cache[i] = -1;
  }
  
  // scan event
  for (tr = 0; tr < skr->smf->len; tr++) {
    smf = skr->smf->ptr[tr];
    // every event
    for (i = 0; i < smf->len; i++) {
      e = smf->ptr[i];
      if (e->time > pf_time) break;
      if (e->status == 0xB0) { // CC
        cc_cache[e->ch * e->data1] = e->data2;
      }
      else if (e->status == 0xC0) { // Voice
        voice_cache[e->ch] = e->data1;
      }
    }
  }
  
  // remove event
  for (tr = 0; tr < skr->smf->len; tr++) {
    smf_tr = skr->smf->ptr[tr];
    // every event
    for (i = 0; i < smf_tr->len; i++) {
      e = smf_tr->ptr[i];
      if (e->time < pf_time) {
        // SysEx should be send
        if (e->status == 0xf0 || e->status == 0xff) {
          e->time = 0;
          continue;
        }
        e->time = -1;
        continue;
      }
      e->time = e->time - pf_time;
      // printf("[%04d] %0X:%0X \n", e->time, e->status, e->data1);
    }
  }
  
  // add control change
  smf = skr->smf->ptr[0]; // top(0)
  for (i = 0; i < 2048; i++) {
    if (cc_cache[i] < 0) continue;
    ch = i / 128 + 1;
    cc = i % 128;
    e = KSmfEvent_newCC(pf_time, ch, cc, cc_cache[i]);
    KSmfTrack_append(smf, e);
    if (skr->debug) {
      printf("[cc] %i: %d=%d\n", i, cc, cc_cache[i]);
    }
  }
  // add voice
  for (i = 0; i < 16; i++) {
    if (voice_cache[i] < 0) continue;
    ch = i + 1;
    e = KSmfEvent_new();
    e->time = pf_time;
    e->ch = i + 1;
    e->data1 = voice_cache[i];
    KSmfTrack_append(smf, e);
  }
  KSmfWriter_sortAll(skr->smf);
  
  return;
}

/** value to delta  */
SStream *makeDelta(int v) {
  int i = 0, len;
  s_byte buf[64], rev[64];
  SStream *s;
  
  for (;;) {
    buf[i] = 0x7F & v;
    if (i > 0) buf[i] = 0x80 | buf[i];
    if (v < 0x80) break;
    v = v >> 7;
    i++;
  }
  len = i + 1;
  
  for (i = 0; i < len; i++) {
    rev[i] = buf[len - i - 1];
  }

  s = SStream_new();
  SStream_write(s, rev, len);
  return s;
}

SStream *KSmfTrack_getStream(KSmfTrack *tr) {
  int i;
  SStream *strm;
  KSmfEvent *e;
  int time, tm;
  SStream *delta;
  s_byte eot[4] = {0x00, 0xFF, 0x2F, 0x00};

  strm = SStream_new();
  if (tr != NULL) {
    time = 0;
    for (i = 0; i < tr->len; i++) {
      e = tr->ptr[i];
      if (e->time < 0) continue; // skip event
      // write time
      tm = e->time - time;
      delta = makeDelta(tm);
      SStream_write(strm, delta->ptr, delta->len);
      SStream_free(delta);
      time = e->time;
      // write event
      KSmfWriter_writeEvent(strm, e);
    }
  }
  // write END OF TRACK
  SStream_write(strm, eot, 4);

  return strm;
}



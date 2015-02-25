/* 
 * File:   k_smf_writer.h
 * Author: kujira
 *
 * Created on 2014/12/31, 23:25
 */

#ifndef K_SMF_WRITER_H
#define	K_SMF_WRITER_H

#include "s_lib.h"
#include "k_main.h"
#include "k_smf.h"

/** Write SMF File */
s_bool KSmfWriter_saveToSmf0(s_bool is_debug, KSmfTrackList *list, const char *filename, int timebase);
s_bool KSmfWriter_saveToSmf1(SakuraObj *skr, const char *filename);
void KSmfWriter_writeEvent(SStream *strm, KSmfEvent *e);
void KSmfWriter_sortAll(KSmfTrackList *list);

/** value to delta time */
SStream *makeDelta(int v); 

/** PlayFrom */
void KSmfWriter_checkPlayFrom(SakuraObj *skr);

#endif	/* K_SMF_WRITER_H */


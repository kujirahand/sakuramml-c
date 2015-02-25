/**
 * @file k_main.h
 */

#ifndef __k_main_h__
#define __k_main_h__

#include "s_lib.h"
#include "k_token.h"
#include "k_track.h"
#include "k_types.h"
#include "k_sutoton.h"
#include "k_smf.h"

#define SAKURA_MML_DEBUG 1
#define SAKURA3_VERSION "3.0"

/**
 * SakuraObj Struct
 */
typedef struct {
  s_bool  debug;        // debug mode
  KFile   *file_top;    // mml source files (linked list)
  s_bool  error_stop;   // Stop for error
  SStringList *errors;  // Erros include Warning
  int error_count;      // Error Count
  KTrackList *tracks;   // Track list
  KSmfTrackList *smf;   // SmfTrack List
  KSongInfo *info;      // Song info
  SList *loopStack;     // stack for loop
  SList *valueStack;    // stack for value
  SHash *command_hash;  // for parser
  SHash *variables;     // for Variables
  SHash *local_vars;    // for Local variables
  KSutotonList *sutoton;// for sutoton
  char **rhythm;        // for Rhythm mode
  s_bool use_print_console;   // for print
  SString *print_buf;   // for print
  int play_from;        // for PlayFrom || ?
  s_bool is_local;      // for function
} SakuraObj;

typedef struct {
  s_bool is_debug;
  s_bool use_print_console;
  s_bool include_stdmsg;
} SakuraCommandOpt;

/** Easy method */
s_bool Sakura_mml2smf(const char* infile, const char* outfile, SakuraCommandOpt *opt);
s_bool Sakura_mmlstr2smf(const char* mml, const char* outfile, SakuraCommandOpt *opt);

/** SakuraObj method */

SakuraObj *SakuraObj_new();
void SakuraObj_free(SakuraObj *obj);

KFile *SakuraObj_loadString(SakuraObj *obj, const char *src); 
KFile *SakuraObj_loadFromFile(SakuraObj *skr, const char *filename); 
s_bool SakuraObj_parse(SakuraObj *obj, KFile *file);
s_bool SakuraObj_run(SakuraObj *obj, KFile *file);

void SakuraObj_error(SakuraObj *skr, int errorno, const char *msg, const char *near, const char *file, int lineno);
void SakuraObj_warn(SakuraObj *skr, int errorno, const char *msg, const char *near, const char *file, int lineno);

KTrack *SakuraObj_curTrack(SakuraObj *skr);
KSmfTrack *SakuraObj_curSmfTrack(SakuraObj *skr);
int SakuraObj_time2step(SakuraObj *skr, int mes, int beat, int tick);

#define K_ERROR_SYSTEM 0x1100
#define K_ERROR_SYNTAX 0x1101
#define K_ERROR_FILE_READ 0x1102
#define K_ERROR_LOOP_RANGE 0x1103
#define K_ERROR_FILE_WRITE 0x1104
#define K_ERROR_ARG_COUNT 0x1105

#define K_WARN_VAR_NOT_DEFINE 0x2100

char *SakuraObj_getErrorMsg(int errno, char *buf);
SString *SakuraObj_getPosInfo(SakuraObj *skr, const char *pos);

/** error */
#define k_error(skr, errno, msg, near) \
  SakuraObj_error(skr, errno, msg, near, __FILE__, __LINE__)
/** worning */
#define k_warn(skr, errno, msg, near) \
  SakuraObj_warn(skr, errno, msg, near, __FILE__, __LINE__)

#endif

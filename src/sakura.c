/**
 * Main file
 * @file sakura.c
 */

#include <stdio.h>
#include <string.h>
#include "sakura.h"
#include "s_test.h"
#include "s_file.h"
#include "k_main.h"
#include "k_smf.h"
#include "k_test.h"

void show_help() {
  printf("usage: sakurac [option] (infile) [outfile]\n");
  printf("options:\n");
  printf("  -debug  # set debug mode\n");
  printf("  -help   # show this help\n");
}

int goTest() {
  char *infile;
  char *outfile = "./test.mid";
  char *infile1 = "test.mml";
  char infile2[2049];
  char *p;
  SakuraCommandOpt opt;

  // go test
  k_test_go();
  if (s_test_getNGCount() > 0) {
    return -1;
  }

  // go Sakura
  if (s_file_exists(infile1)) { // current dir
    infile = infile1;
  }
  else {
    // source dir
    strcpy(infile2, __FILE__);
    p = strstr(infile2, "sakura.c");
    if (p != NULL) {
      strcpy(p, infile1);
    }
    if (s_file_exists(infile2)) {
      infile = infile2;
    } else {
      fprintf(stderr, "[test] no test.mml file\n");
      return -1;
    }
  }

  opt.is_debug = S_TRUE;
  opt.use_print_console = S_TRUE;
  return Sakura_mml2smf(infile, outfile, &opt);
}

int gomml(int argc, char* argv[]) {
  int i;
  char *infile = NULL;
  char *outfile = NULL;
  char *p;
  SakuraCommandOpt opt;
  int retcode;

  memset(&opt, 0, sizeof(opt));
  opt.use_print_console = S_TRUE;
  opt.include_stdmsg = S_TRUE;

  // check params
  for (i = 1; i < argc; i++) {
    p = argv[i];
    // check options
    if (*p == '-') {
      if (strcmp(p, "-debug") == 0 || strcmp(p, "--debug") == 0) {
        opt.is_debug = S_TRUE;
        continue;
      }
      // test ?
      if (strcmp(p, "-test") == 0 || strcmp(p, "--test") == 0) {
        goTest();
        return 0;
      }
      fprintf(stderr, "Unknown option %s\n", p);
      show_help();
      continue;
    }
    // check in/out file
    if (infile == NULL) {
      infile = s_malloc(strlen(p) + 5);
      strcpy(infile, p);
      if (!s_file_exists(p)) {
        strcat(infile, ".mml");
      }
      continue;
    }
    if (outfile == NULL) {
      outfile = p;
      continue;
    }
  }

  // check in/out
  if (infile == NULL) {
    fprintf(stderr, "no input mml file\n");
    show_help();
    return -1;
  }
  if (outfile == NULL) {
    outfile = file_changeExt(infile, ".mid");
  }

  // compile
  retcode = Sakura_mml2smf(infile, outfile, &opt);
  s_free(outfile);
  s_free(infile);

  return retcode;
}

int main(int argc, char* argv[]) {
  int retcode = 0;

  // check arguments
  retcode = gomml(argc, argv);

  // free library
  s_lib_free();
  s_hook_memory_close();

  if (retcode == S_TRUE) printf("ok.\n");
  return retcode;
}

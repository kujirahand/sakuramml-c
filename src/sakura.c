/**
 * Main file
 * @file sakura.c
 */

#include <stdio.h>
#include <string.h>
#include "sakura.h"
#include "s_test.h"
#include "k_main.h"
#include "k_smf.h"
#include "k_test.h"

int goTest() {
  char *infile;
  char *outfile = "./test.mid";
  char *infile1 = "test.mml";
  char *infile2 = "~/repos/sakura-c/src/test.mml";
  SakuraCommandOpt opt;
  
  // go test
  k_test_go();
  if (s_test_getNGCount() > 0) {
    return -1;
  }

  // go Sakura
  if (file_exists(infile1)) {
    infile = infile1;
  }
  else if (file_exists(infile2)) {
    infile = infile2;
  } else {
    fprintf(stderr, "[test] no test.mml file\n");
    return -1;
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
    if (strcmp(p, "-debug") == 0) {
      opt.is_debug = S_TRUE;
      continue;
    }
    // check in/out file
    if (infile == NULL) {
      infile = p;
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
    return -1;
  }
  if (outfile == NULL) {
    outfile = file_changeExt(infile, ".mid");
  }
  
  // compile
  retcode = Sakura_mml2smf(infile, outfile, &opt);
  s_free(outfile);
  
  return retcode;
}

int main(int argc, char* argv[]) {
  int retcode = 0;
  /*
  int i;
  for (i = 0; i < argc; i++) {
    printf("- %d: %s\n", i, argv[i]);
  }
  */
  // printf("[%d]\n", argc);
  // check arguments
  if (argc <= 1) {
    retcode = goTest();
  } else {
    retcode = gomml(argc, argv);
  }

  // free library
  s_lib_free();
  s_hook_memory_close();
  
  if (retcode == S_TRUE) printf("ok.\n");
  return retcode;
}


#include <stdio.h>
#include <sys/stat.h>
#include "s_file.h"

s_bool s_file_exists(const char *path) {
  struct stat  st;
  return stat(path, &st) >= 0;
}

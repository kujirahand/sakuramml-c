/* 
 * File:   k_fomatter.c
 * Author: kujira
 *
 * Created on 2015/01/05, 6:41
 */

#include <string.h>
#include "k_formatter.h"

s_bool KFormatter_trimComment(SString *src) {
  SString *res = SString_new(NULL);
  char *p = src->ptr;
  int nest;
  
  while (*p != '\0') {
    // check comment
    if (*p == '/') {
      // line comment
      if (memcmp(p, "//", 2) == 0) {
        p += 2;
        while (*p != '\0') {
          if (*p == '\n') break;
          p++;
        }
        continue;
      }
      // range comment
      if (memcmp(p, "/*", 2) == 0) {
        nest = 1;
        p += 2;
        while (*p != '\0') {
          if (memcmp(p, "*/", 2) == 0) { // out
            nest--;
            p += 2;
            if (nest <= 0) break;
          }
          if (memcmp(p, "/*", 2) == 0) { // in
            nest++;
            p += 2;
            continue;
          }
          if (*p == '\n') {
            SString_appendChar(res, '\n');
            p++;
            continue;
          }
          p++;
        }
      }
    }
    // append char
    SString_appendChar(res, *p);
    p++;
  }
  // replace source
  SString_set(src, res->ptr);
  SString_free(res);
  return S_TRUE;
}


#include "s_test.h"
#include "k_test.h"
#include "k_sutoton.h"

void k_sutoton_test() {
  KSutotonList *list = KSutotonList_new();
  
  //(1)
  SString *s = SString_new("ドレミ");
  KSutotonList_replace(list, s);
  s_test_str_eq(s->ptr, "cde", "Sutoton Change:1");
  
  //(2)
  SString_set(s, "トラック1チャンネル1ドードレミ");
  KSutotonList_replace(list, s);
  s_test_str_eq(s->ptr, "TR=1CH=1c^cde", "Sutoton Change:2");
  
  //(3)
  SString_set(s, "~{男}={cb}男男男");
  KSutotonList_replace(list, s);
  s_test_str_eq(s->ptr, "cbcbcb", "Sutoton Change:3 register macro");

  //(4)
  SString_set(s, "TrackName={\"ドレミ\"}");
  KSutotonList_replace(list, s);
  s_test_str_eq(s->ptr, "TrackName={\"ドレミ\"}", "Sutoton Change:3 register macro");
  
  SString_free(s);
  KSutotonList_free(list);
}


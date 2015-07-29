#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "s_lib.h"
#include "k_sutoton.h"

int KSutotonList_sortFunc(const void *a, const void *b);
void KSutotonList_addDefaultCommand(KSutotonList *list);
 s_bool KSutotonList_addMacro(KSutotonList *list, char **pstr);


KSutotonList *KSutotonList_new() {
  KSutotonList *klist;
  
  klist = s_new(KSutotonList);
  klist->list = SList_new();
  klist->list->free_item_f = KSutotonItem_free;
  
  KSutotonList_addDefaultCommand(klist);
  
  return klist;
}

void KSutotonList_free(KSutotonList *list) {
  SList_free(list->list);
  s_free(list);
}

void KSutotonList_addDefaultCommand(KSutotonList *list) {
  //
  KSutotonList_append(list, "トラック", "TR=");
  KSutotonList_append(list, "チャンネル", "CH=");
  KSutotonList_append(list, "音階", "o");
  KSutotonList_append(list, "音符", "l");
  KSutotonList_append(list, "音量", "v");
  KSutotonList_append(list, "ゲート", "q");
  KSutotonList_append(list, "音色", "@");
  KSutotonList_append(list, "テンポ", "Tempo=");
  KSutotonList_append(list, "読む", "Include");
  //
  KSutotonList_append(list, "調", "KeyFlag");
  KSutotonList_append(list, "時間", "TIME");
  KSutotonList_append(list, "予約", ".onNote=");
  KSutotonList_append(list, "拍子", "TimeSignature=");
  KSutotonList_append(list, "曖昧さ", ".Random=");
  KSutotonList_append(list, "曲名", "TrackName=");
  KSutotonList_append(list, "作者", "Copyright=");
  KSutotonList_append(list, "コメント", "MetaText=");
  KSutotonList_append(list, "音源初期化", "MeasureShift(1);ResetGM;Time(1:1:0);TrackSync;");
  KSutotonList_append(list, "演奏位置", "PlayFrom");
  KSutotonList_append(list, "連符", "Div");
  KSutotonList_append(list, "リズム", "Rythm");
  KSutotonList_append(list, "ここから演奏", "PlayFrom(TIME)");
  //
  KSutotonList_append(list, "ド", "c");
  KSutotonList_append(list, "レ", "d");
  KSutotonList_append(list, "ミ", "e");
  KSutotonList_append(list, "ファ", "f");
  KSutotonList_append(list, "フ", "f");
  KSutotonList_append(list, "ソ", "g");
  KSutotonList_append(list, "ラ", "a");
  KSutotonList_append(list, "シ", "b");
  KSutotonList_append(list, "ど", "c");
  KSutotonList_append(list, "れ", "d");
  KSutotonList_append(list, "み", "e");
  KSutotonList_append(list, "ふぁ", "f");
  KSutotonList_append(list, "ふ", "f");
  KSutotonList_append(list, "そ", "g");
  KSutotonList_append(list, "ら", "a");
  KSutotonList_append(list, "し", "b");
  KSutotonList_append(list, "ッ", "r");
  KSutotonList_append(list, "っ", "r");
  KSutotonList_append(list, "イ", "a");
  KSutotonList_append(list, "ロ", "b");
  KSutotonList_append(list, "ハ", "c");
  KSutotonList_append(list, "ニ", "d");
  KSutotonList_append(list, "ホ", "e");
  KSutotonList_append(list, "ヘ", "f");
  KSutotonList_append(list, "ト", "g");
  KSutotonList_append(list, "ん", "r");
  KSutotonList_append(list, "ン", "r");
  KSutotonList_append(list, "っ", "r");
  KSutotonList_append(list, "ッ", "r");
  KSutotonList_append(list, "つ", "r");
  KSutotonList_append(list, "ツ", "r");
  //
  KSutotonList_append(list, "＞", ">");
  KSutotonList_append(list, "＜", "<");
  KSutotonList_append(list, "↑", ">");
  KSutotonList_append(list, "↓", "<");
  KSutotonList_append(list, "上", ">");
  KSutotonList_append(list, "下", "<");
  //
  KSutotonList_append(list, "ー", "^");
  KSutotonList_append(list, "＃", "+");
  KSutotonList_append(list, "♭", "-");
  KSutotonList_append(list, "＋", "+");
  KSutotonList_append(list, "−", "-");
  KSutotonList_append(list, "’", "`");
  KSutotonList_append(list, "‘", "`");
  KSutotonList_append(list, "”", "\"");
  KSutotonList_append(list, "変", "-");
  KSutotonList_append(list, "嬰", "+");
  KSutotonList_append(list, "．", ".");
  KSutotonList_append(list, "，", ",");
  //
  KSutotonList_append(list, "【", "[");
  KSutotonList_append(list, "】", "]");
  KSutotonList_append(list, "：", ":");
  KSutotonList_append(list, "「", "'");
  KSutotonList_append(list, "」", "'");
  KSutotonList_append(list, "｜", "|");
  KSutotonList_append(list, "（", "(");
  KSutotonList_append(list, "）", ")");
  //
  KSutotonList_append(list, "０", "0");
  KSutotonList_append(list, "１", "1");
  KSutotonList_append(list, "２", "2");
  KSutotonList_append(list, "３", "3");
  KSutotonList_append(list, "４", "4");
  KSutotonList_append(list, "５", "5");
  KSutotonList_append(list, "６", "6");
  KSutotonList_append(list, "７", "7");
  KSutotonList_append(list, "８", "8");
  KSutotonList_append(list, "９", "9");
  KSutotonList_append(list, "　", " "); // SPACE
  KSutotonList_append(list, "〇", "0");
  KSutotonList_append(list, "一", "1");
  KSutotonList_append(list, "二", "2");
  KSutotonList_append(list, "三", "3");
  KSutotonList_append(list, "四", "4");
  KSutotonList_append(list, "五", "5");
  KSutotonList_append(list, "六", "6");
  KSutotonList_append(list, "七", "7");
  KSutotonList_append(list, "八", "8");
  KSutotonList_append(list, "九", "9");
  KSutotonList_append(list, "十", "10");
  //
  KSutotonList_append(list, "ず", "n36,");
  KSutotonList_append(list, "た", "n38,");
  KSutotonList_append(list, "つ", "n42,");
  KSutotonList_append(list, "ち", "n46,");
  KSutotonList_append(list, "ぱ", "n49,");
  KSutotonList_append(list, "と", "n50,");
  KSutotonList_append(list, "む", "n47,");
  KSutotonList_append(list, "ろ", "n43,");
  
  // printf("[Sutoton.Length=%zu]\n", list->list->len);
  
  // sort
  KSutotonList_sort(list);
 }



KSutotonItem *KSutotonList_append(KSutotonList *list, const char *key, const char *value) {
  KSutotonItem *item;
  
  item = s_new(KSutotonItem);
  item->key = s_str_dup(key);
  item->value = s_str_dup(value);
  item->key_len = (int)strlen(key);
  SList_push(list->list, item);
  
  return item;
}

int KSutotonList_sortFunc(const void *a, const void *b) {
  KSutotonItem *sa = *(KSutotonItem**)a;
  KSutotonItem *sb = *(KSutotonItem**)b;
  int len_a = (a == NULL) ? 0 : (int)sa->key_len;
  int len_b = (b == NULL) ? 0 : (int)sb->key_len;
  return len_b - len_a;
}

void KSutotonList_sort(KSutotonList *list) {
  SList *slist = list->list;
  SList_msort(slist, KSutotonList_sortFunc);
  
  // print sutoton list
  // for (int i = 0; i < list->list->len; i++) {
  //   KSutotonItem *item = list->list->ptr[i];
  //   printf("%d: %s(%d)\n", i, item->key, item->key_len);
  // }
}

KSutotonItem *KSutotonList_find(KSutotonList *stt_list, const char *str) {
  SList *list = stt_list->list;
  size_t i;
  KSutotonItem *item;
  char *key;
  
  for (i = 0; i < list->len; i++) {
    item = SList_get(list, i);
    key = item->key;
    if (memcmp(key, str, item->key_len) == 0) {
      return item;
    }
  }
  return NULL;
}

 s_bool KSutotonList_addMacro(KSutotonList *list, char **pstr) {
   char *p, *pp;
   char key[256];
   char *val;
   int level;
   size_t len;
   
   p = *pstr;
   if (memcmp(p, "~{", 2) != 0) return S_FALSE;
   p += 2;
   pp = p;
   while (*p != '\0') {
     if (*p == '}') {
       p++;
       if ((p - pp) > 255) { // size check
         *pstr = p;
         return S_FALSE;
       }
       s_strlcpy(key, pp, (p - pp));
       break;
     }
     p++;
   }
   SKIP_SPACE(p);
   if (*p == '=') p++;
   SKIP_SPACE(p);
   if (*p == '{') p++;
   level = 1;
   pp = p;
   while (*p != '\0') {
     if (*p == '{') {
       p++; level++;
       continue;
     }
     if (*p == '}') {
       p++;
       level--;
       if (level == 0) break;
       continue;
     }
     p++;
   }
   *pstr = p;
   if (level != 0) return S_FALSE;
   len = p - pp + 1 - (1/*"}"*/);
   val = s_new_a(char, len + 1);
   s_strlcpy(val, pp, len);
   //
   KSutotonList_append(list, key, val);
   KSutotonList_sort(list);
   //
   s_free(val);
   return S_TRUE;
 }

s_bool KSutotonList_replace(KSutotonList *stt_list, SString *str) {
  char *p = str->ptr;
  SString *res;
  KSutotonItem *item;
  
  res = SString_new(NULL);
  while (*p != '\0') {
    if ((unsigned char)*p < 0xE0) {
      if (memcmp(p, "~{", 2) == 0) {
        KSutotonList_addMacro(stt_list, &p);
        continue;
      }
      if (memcmp(p, "{\"", 2) == 0) {
        SString_append(res, "{\"");
        p += 2;
        while (*p != '\0') {
          if (memcmp(p, "\"}", 2) == 0) {
            SString_append(res, "\"}");
            p += 2;
            break;
          }
          SString_appendChar(res, *p);
          p++;
        }
        continue;
      }
      SString_appendChar(res, *p);
      p++; continue;
    }
    // Sutoton Charset
    item = KSutotonList_find(stt_list, p);
    if (item == NULL) {
      SString_appendMem(res, p, 3);
      p += 3; continue;
    }
    SString_append(res, item->value);
    p += strlen(item->key);
  }
  SString_set(str, res->ptr);
  SString_free(res);
  
  return S_TRUE;
}

s_bool KSutotonItem_free(void *item) {
  KSutotonItem *p = item;
    
  if (p == NULL) return S_TRUE;
  s_free(p->key);
  s_free(p->value);
  s_free(p);
  
  return S_TRUE;
}

/* 
 * File:   k_sutoton.h
 * Author: kujira
 *
 * Charset of Sutoton List for UTF-8
 * Created on 2014/10/17, 15:15
 */

#ifndef K_SUTOTON_H
#define	K_SUTOTON_H

#include "s_lib.h"
#include "s_list.h"

typedef struct {
  char *key;
  char *value;
  int key_len;
} KSutotonItem;

typedef struct {
  SList *list;
  int tag;
} KSutotonList;

KSutotonList *KSutotonList_new();
void KSutotonList_free(KSutotonList *list);
KSutotonItem *KSutotonList_append(KSutotonList *list, const char *key, const char *value);
void KSutotonList_sort(KSutotonList *list);

s_bool KSutotonList_replace(KSutotonList *stt_list, SString *str);
KSutotonItem *KSutotonList_find(KSutotonList *stt_list, const char *str);

s_bool KSutotonItem_free(void *item);




#endif	/* K_SUTOTON_H */


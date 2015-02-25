/* 
 * File:   s_hash.h
 * Author: kujira
 *
 * Created on 2014/10/15, 13:59
 */

#ifndef S_HASH_H
#define	S_HASH_H

#include "s_lib.h"

#define S_HASH_DEFAULT_TABLE_SIZE 131

typedef struct _SHashNode {
  char *key;
  void *value;
  int tag;
  struct _SHashNode *next;
} SHashNode;

/// callback func for iterator
typedef s_bool (*SHashIterator)(const char *key, void *value);

typedef struct {
  SHashNode **table;
  int size;
  int len;
  SHashIterator callback_free_item;
} SHash;

SHash *SHash_new(int size);
void SHash_clear(SHash *hash);
void SHash_free(SHash *hash);
int SHash_calcHash(SHash *hash, const char *key);

SHashNode *SHash_set(SHash *hash, const char *key, void *value);
SHashNode *SHash_setTag(SHash *hash, const char *key, void *value, int tag);
void *SHash_get(SHash *hash, const char *key);
SHashNode *Shash_getNode(SHash *hash, const char *key);
s_bool SHash_remove(SHash *hash, const char *key);
s_bool SHash_walk(SHash *hash, SHashIterator callback);

void SHash_freeNode(SHash *hash, SHashNode *node);

#endif	/* S_HASH_H */


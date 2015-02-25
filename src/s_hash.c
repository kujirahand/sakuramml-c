
#include <string.h>

#include "s_hook_memory.h"
#include "s_lib.h"
#include "s_hash.h"
#include "s_str.h"

SHash *SHash_new(int size) {
  SHash *hash = s_new(SHash);
  if (size <= 16) size = S_HASH_DEFAULT_TABLE_SIZE;
  hash->len = 0;
  hash->size = size;
  hash->table = s_new_a(SHashNode*, size);
  return hash;
}

void SHash_clear(SHash *hash) {
  int i;
  SHashNode *node, *tmp;
  
  for (i = 0; i < hash->size; i++) {
    node = hash->table[i];
    while (node != NULL) {
      tmp = node->next;
      SHash_freeNode(hash, node);
      node = tmp;
    }
    hash->table[i] = NULL;
  }
  hash->len = 0;
}

s_bool SHash_walk(SHash *hash, SHashIterator callback) {
  int i;
  SHashNode *node;
  
  for (i = 0; i < hash->size; i++) {
    node = hash->table[i];
    while (node != NULL) {
      if (S_FALSE == callback(node->key, node->value)) return S_FALSE;
      node = node->next;
    }
  }
  return S_TRUE;
}


void SHash_free(SHash *hash) {
  SHash_clear(hash);
  s_free(hash->table);
  s_free(hash);
}

int SHash_calcHash(SHash *hash, const char *key) {
  int i, sum = 0, no;
  const char *p = key;
  for (i = 0; i < 5; i++) {
    if (*p == '\0') break;
    sum += *p;
  }
  no = sum % hash->size;
  return no;
}

SHashNode *SHash_setTag(SHash *hash, const char *key, void *value, int tag) {
  SHashNode *node;
  node = SHash_set(hash, key, value);
  node->tag = tag;
  return node;
}

SHashNode *SHash_set(SHash *hash, const char *key, void *value) {
  SHashNode *node, *n;
  int no;

  // search node
  node = Shash_getNode(hash, key);
  if (node != NULL) {
    if (hash->callback_free_item != NULL) {
      hash->callback_free_item(key, node->value);
    }
    node->value = value; // renew
    return node;
  }

  // new key
  node = s_new(SHashNode);
  node->key = s_str_dup(key);
  node->value = value;

  no = SHash_calcHash(hash, key);
  n = hash->table[no];
  if (n == NULL) {
    hash->table[no] = node;
  } else {
    while (n->next != NULL) n = n->next;
    n->next = node;
  }
  hash->len++;
  
  return node;
}

void *SHash_get(SHash *hash, const char *key) {
  SHashNode *node;
  node = Shash_getNode(hash, key);
  if (node == NULL) return NULL;
  return node->value;
}

SHashNode *Shash_getNode(SHash *hash, const char *key) {
  SHashNode *node;
  int no;
  
  no = SHash_calcHash(hash, key);
  node = hash->table[no];
  if (node == NULL) return NULL;
  while (node != NULL) {
    if (strcmp(node->key, key) == 0) return node;
    node = node->next;
  }
  return NULL;
}

s_bool SHash_remove(SHash *hash, const char *key) {
  SHashNode *n, *pn;
  int no;
  
  no = SHash_calcHash(hash, key);
  n = hash->table[no];
  if (n == NULL) return S_FALSE;
  pn = NULL;
  while (n != NULL) {
    if (strcmp(n->key, key) == 0) {
      SHash_freeNode(hash, n);
      if (pn == NULL) {
        hash->table[no] = NULL;
      } else {
        pn->next = NULL;
      }
      return S_TRUE;
    }
    pn = n;
    n = n->next;
  }
  hash->len--;
  
  return S_FALSE;
}

void SHash_freeNode(SHash *hash, SHashNode *node) {
  if (hash->callback_free_item != NULL) {
    hash->callback_free_item(node->key, node->value);
  }
  s_free(node->key);
  s_free(node);
}


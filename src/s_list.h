/**
 * File:   s_list.h
 * Author: kujira
 *
 * Created on 2014/10/14, 23:38
 */

#ifndef S_LIST_H
#define	S_LIST_H

#include <stdlib.h>
#include "s_types.h"

#define S_LIST_DEFAULT_SIZE 256

/// SList Iterator
typedef s_bool (*SListIterator)(void *ptr, void *obj);
typedef s_bool (*SListFreeFunc)(void *ptr);
typedef int (*SListSortFunc) (const void *a, const void *b);

/** SList stcut */
typedef struct _SList {
  void **ptr;                 /// array of pointer
  size_t len;                 /// list length
  size_t size;                /// list size
  SListFreeFunc free_item_f;  /// auto destroy func
} SList;


#define SList_new() SList_new_(S_LIST_DEFAULT_SIZE, __FILE__, __LINE__)

/** SList_new  */
SList *SList_new_(size_t size, const char *file, int lineno);
/** SList_free */
void SList_free(SList *list);
/** SList_clear */
void SList_clear(SList *list);
/** SList_grow */
void SList_grow(SList *list, size_t newsize);

/** SList_peekLast */
void *SList_peekLast(SList *list);

size_t SList_push(SList *list, void *ptr);
void *SList_pop(SList *list);
SList *SList_popNum(SList *list, size_t num);
void *SList_get(SList *list, size_t index);
void SList_set(SList *list, size_t index, void *v);
void *SList_shift(SList *list);

s_bool SList_walk(SList *list, SListIterator callback, void *obj);

/** sort function */
void SList_sort(SList *list, SListSortFunc callback);
void SList_msort(SList *list, SListSortFunc callback);

#endif	/* S_LIST_H */




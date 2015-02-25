
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "s_list.h"
#include "s_hook_memory.h"

void msort(void **x, size_t size, SListSortFunc callback);
void msort_r(void **x, void **temp, SListSortFunc callback, size_t left, size_t right);

/**
 * create SList object
 * @param file
 * @param lineno
 * @return SList object
 */
SList *SList_new_(size_t size, const char *file, int lineno) {
  SList *list;
  //
  list = (SList*)s_malloc_(sizeof(SList), file, lineno);
  list->len = 0;
  list->ptr = s_new_a(void*, size);
  list->size = size;
  //
  return list;
}

/**
 * free SList
 * @param list
 */
void SList_free(SList *list) {
  if (list == NULL) return;
  SList_clear(list);
  s_free(list->ptr);
  s_free(list);
}

/**
 * clear SList object
 * @param list
 */
void SList_clear(SList *list) {
  size_t i;
  void *item;
  
  if (list->free_item_f != NULL) {
    // free every object
    for (i = 0; i < list->len; i++) {
      item = list->ptr[i];
      list->free_item_f(item);
      list->ptr[i] = NULL;
    }
  }
  list->len = 0;
}

void SList_grow(SList *list, size_t newsize) {
  void **ptr_new;
  
  if (list->size > newsize) return;
  ptr_new = s_new_a(void*, newsize);
  list->size = newsize;
  memcpy(ptr_new, list->ptr, sizeof(void*) * list->len);
  s_free(list->ptr);
  list->ptr = ptr_new;
  // printf("[grow=%zu]\n",newsize);
}

void *SList_peekLast(SList *list) {
  if (list == NULL) return NULL;
  if (list->len == 0) return NULL;
  return list->ptr[list->len - 1];
}

size_t SList_push(SList *list, void *ptr) {
  if (list == NULL) return 0;
  if ((list->len + 1) >= list->size) {
    SList_grow(list, list->size * 2);
  }
  list->ptr[list->len] = ptr;
  list->len++;
  return list->len;
}

void *SList_pop(SList *list) {
  void *r;
  size_t i;
  
  if (list == NULL) return NULL;
  if (list->len == 0) return NULL;
  
  i = list->len - 1;
  r = list->ptr[i];
  list->ptr[i] = NULL;
  list->len--;
  
  return r;
}

SList *SList_popNum(SList *list, size_t num) {
  SList *newlist;
  void *p;
  int i;
  //
  if (list->len < num) {
    num = list->len;
  }
  newlist = SList_new();
  for (i = 0; i < num; i++) {
    p = list->ptr[list->len - num + i];
    SList_push(newlist, p);
  }
  list->len -= num;
  //
  return newlist;
}


void *SList_get(SList *list, size_t index) {
  if (index >= list->len) return NULL;
  return list->ptr[index];
}

void SList_set(SList *list, size_t index, void *v) {
  // grow
  if (index >= list->len) {
    SList_grow(list, list->len + 1);
  }
  // set
  list->ptr[index] = v;
}

void *SList_shift(SList *list) {
  void *result;
  void **newlist;
  size_t i;
  
  if (list->len == 0) return NULL;
  result = list->ptr[0];
  
  newlist = s_new_a(void*, list->size);
  for (i = 0; i < list->len - 1; i++) {
    newlist[i] = list->ptr[i + 1];
  }
  list->len--;
  s_free(list->ptr);
  list->ptr = newlist;
  
  return result;
}

s_bool SList_walk(SList *list, SListIterator callback, void *obj) {
  size_t i;
  void *p;
  
  for (i = 0; i < list->len; i++) {
    p = list->ptr[i];
    if (!callback(p, obj)) break;
  }
  
  return S_TRUE;
}

void SList_sort(SList *list, SListSortFunc callback) {
  // sort
  qsort(list->ptr, list->len, sizeof(void*), callback);
}

void SList_msort(SList *list, SListSortFunc callback) {
  // merge sort
  msort(list->ptr, list->len, callback);
}

void msort(void **x, size_t size, SListSortFunc callback) {
  void **temp;
  //
  temp = s_new_a(void*, size);
  msort_r(x, temp, callback, 0, size - 1);
  s_free(temp);
}

void msort_r(void **x, void **temp, SListSortFunc callback, size_t left, size_t right) {
  size_t mid, i, j, k;
  //
  if (left >= right) return;
  // split center
  mid = (left + right) / 2;
  // sort left
  msort_r(x, temp, callback, left, mid);
  // sort right
  msort_r(x, temp, callback, mid + 1, right);
  // copy x[left] to x[mid]
  for (i = left; i <= mid; i++) {
    temp[i] = x[i];
  }
  // copy x[mid+1] to x[right] reverse
  for (i = mid + 1, j = right; i <= right; i++, j--) {
    temp[i] = x[j];
  }
  // 
  i = left;
  j = right;
  // recover array
  for (k = left; k <= right; k++) {
    if (callback(&temp[i], &temp[j]) < 0) {
      x[k] = temp[i++];
    } else {
      x[k] = temp[j--];
    }
  }
}





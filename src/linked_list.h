#include <stdlib.h>

#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_
struct Node {
  int x;
  int y;
  struct Node *next;
};

static inline struct Node *NodeNew(int x, int y) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->x = x;
  node->y = y;
  node->next = NULL;
  return node;
}

static inline void NodeDeleteSelfAndTail(struct Node *node) {
  struct Node *prev = node;
  struct Node *next = node;
  while (next != NULL) {
    prev = next;
    next = prev->next;
    free(prev);
  }
}

#endif

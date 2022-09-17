#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdio.h>

struct node{
  int x;
  int y;
  int dir;
  struct node *next;
};

typedef struct node node_t;

void insert_at_end(node_t *head, node_t *new_node);

#endif

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
int get_node_x(node_t *node);
int get_node_y(node_t *node);
void follow_up(node_t *head, node_t *node);
void follow_down(node_t *head, node_t *node);
void follow_right(node_t *head, node_t *node);
void follow_left(node_t *head, node_t *node);
int get_node_dir(node_t *node);
void set_node_dir(node_t *node, int dir);

#endif

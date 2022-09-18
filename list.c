#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define WIDTH 40
#define HEIGHT 15

void insert_at_end(node_t *head, node_t *new_node){
  node_t *tmp = head;
  if(new_node == head){
    return;
  }
  while(tmp != NULL){
    if(tmp->next == NULL){
      tmp->next = new_node;
      return;
    }
    tmp = tmp->next;
  }
}

int get_node_x(node_t *node){
  return node->x;
}

int get_node_y(node_t *node){
  return node->y;
}

int get_node_dir(node_t *node){
  return node->dir;
}


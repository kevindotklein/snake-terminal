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

void set_node_dir(node_t *node, int dir){
  node->dir = dir;
}

void follow_up(node_t *head, node_t *node){
  //if(head == node) return;
  node_t *tmp = head;
  while(tmp != NULL){
    if(tmp->next == node){
      node->dir = tmp->dir;
      node->x = get_node_x(tmp);
      node->y = get_node_y(tmp) + 1;
    }
    tmp = tmp->next;
  }
}

void follow_down(node_t *head, node_t *node){
  //if(head == node) return;
  node_t *tmp = head;
  while(tmp != NULL){
    if(tmp->next == node){
      node->dir = tmp->dir;
      node->x = get_node_x(tmp);
      node->y = get_node_y(tmp) - 1;
    }
    tmp = tmp->next;
  }
}

void follow_right(node_t *head, node_t *node){
  //if(head == node) return;
  node_t *tmp = head;
  while(tmp != NULL){
    if(tmp->next == node){
      node->dir = tmp->dir;
      node->x = get_node_x(tmp) - 1;
      node->y = get_node_y(tmp);
    }
    tmp = tmp->next;
  }
}

void follow_left(node_t *head, node_t *node){
  //if(head == node) return;
  node_t *tmp = head;
  while(tmp != NULL){
    if(tmp->next == node){
      node->dir = tmp->dir;
      node->x = get_node_x(tmp) + 1;
      node->y = get_node_y(tmp);
    }
    tmp = tmp->next;
  }
}

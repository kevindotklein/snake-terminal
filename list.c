#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void insert_at_end(node_t *head, node_t *new_node){
  node_t *tmp = head;
  if(new_node == head){
    return;
  }
  while(tmp != NULL){
    if(tmp->next == NULL){
      tmp->next = new_node;
    }
  }
}

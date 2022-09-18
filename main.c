#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include "list.h"

#define WIDTH 40
#define HEIGHT 15
#define FPS 15

void term_mode(int mode);
void init_board();
void render_board();
void update();
void clear();
void render_nodes(node_t *head);
void node_up(node_t *node);
void node_down(node_t *node);
void node_right(node_t *node);
void node_left(node_t *node);
void add_node(node_t *head);
//void snake_up();//
//void snake_down();//
//void snake_right();//
//void snake_left();//
//void render_node(node_t *head);//
//void set_node(node_t **head);//
//int get_snake_x();//
//int get_snake_y();//
//void node_up(node_t *tmp);//
//void node_down(node_t *tmp);//
//void node_right(node_t *tmp);//
//void node_left(node_t *tmp);//

char board[WIDTH * HEIGHT];
char input;
node_t *head;

int main(void){

  head = malloc(sizeof(node_t));
  head->x = 9;
  head->y = 4;
  head->dir = 0;

  term_mode(1);
  do{
    clear();
    init_board();
    update();
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    scanf("%c", &input);
    switch(input){
    case 'w':
      if(head->dir != 2){
	head->dir = 1;
	break;
      }
      break;
    case 's':
      if(head->dir != 1){
	head->dir = 2;
	break;
      }
      break;
    case 'd':
      if(head->dir != 4){
	head->dir = 3;
	break;
      }
      break;
    case 'a':
      if(head->dir != 3){
	head->dir = 4;
	break;
      }
      break;
    }
    clear();
    usleep(1200 * 1200/FPS);
  }while(input != 'q');
  term_mode(0);
  
  return 0;
}

void term_mode(int mode){
  static struct termios origt, newt;

  if(mode == 1){
    tcgetattr(STDIN_FILENO, &origt);
    newt = origt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  }else{
    tcsetattr(STDIN_FILENO, TCSANOW, &origt);
  }
  
}

void init_board(){
  memset(board, '.', WIDTH * HEIGHT);
}

void render_board(){
  for(int i=0; i<HEIGHT; i++){
    fwrite(&board[i*WIDTH], WIDTH, 1, stdout);
    fputc('\n', stdout);
  }
}

void update(){

  render_nodes(head);
  render_board();
  
  if(head->dir == 1) node_up(head);
  else if(head->dir == 2) node_down(head);
  else if(head->dir == 3) node_right(head);
  else if(head->dir == 4) node_left(head);
  
  if(get_node_x(head) == 20 && get_node_y(head) == 4){
  board[(4 * WIDTH) + 20] = 'X';
  add_node(head);
  }
  
}

void render_nodes(node_t *head){
  node_t *tmp = head;
  
  while(tmp != NULL){
    if(tmp->x >= WIDTH) tmp->x = 0;
    else if(tmp->x < 0) tmp->x = WIDTH - 1;
    if(tmp->y >= HEIGHT) tmp->y = 0;
    else if(tmp->y < 0) tmp->y = HEIGHT - 1;
    board[(tmp->y * WIDTH) + tmp->x] = '#';

    if(!(tmp->next == NULL)){
      //definir x,y dos proxs
      switch(tmp->dir){
      case 1:
	tmp->next->x = get_node_x(tmp);
	tmp->next->y = get_node_y(tmp) + 1;
	break;
      case 2:
	tmp->next->x = get_node_x(tmp);
	tmp->next->y = get_node_y(tmp) - 1;
	break;
      case 3:
	tmp->next->x = get_node_x(tmp) - 1;
	tmp->next->y = get_node_y(tmp);
	break;
      case 4:
	tmp->next->x = get_node_x(tmp) + 1;
	tmp->next->y = get_node_y(tmp);
	break;
      }
    }

    if(tmp != head){
      node_t *aux = head;
      while(aux != NULL){
	if(aux->next == tmp){
	  //definir dir baseado no x,y do da frente
	  if(aux->y < tmp->y) tmp->dir = 1;
	  else if(aux->y > tmp->y) tmp->dir = 2;
	  else if(aux->x > tmp->x) tmp->dir = 3;
	  else if(aux->x < tmp->x) tmp->dir = 4;
	}
	aux = aux->next;
      }
    }
    
    tmp = tmp->next;
  }
  //redefinir dir baseado no x,y da node da frente (tirando o head obviamente)
}

void node_up(node_t *node){
  node->y--;
}

void node_down(node_t *node){
  node->y++;
}

void node_right(node_t *node){
  node->x++;
}

void node_left(node_t *node){
  node->x--;
}

void add_node(node_t *head){
  node_t *node = malloc(sizeof(node_t));
  insert_at_end(head, node);
  node_t *tmp = head;
  while(tmp != NULL){
    if(tmp->next == node){
      switch(tmp->dir){
      case 1:
	node->x = get_node_x(tmp);
	node->y = get_node_y(tmp) + 1;
	node->dir = 1;
	break;
      case 2:
	node->x = get_node_x(tmp);
	node->y = get_node_y(tmp) - 1;
	node->dir = 2;
	break;
      case 3:
	node->x = get_node_x(tmp) - 1;
	node->y = get_node_y(tmp);
	node->dir = 3;
	break;
      case 4:
	node->x = get_node_x(tmp) + 1;
	node->y = get_node_y(tmp);
	node->dir = 4;
	break;
      }
    }
    tmp = tmp->next;
  }
}

/*
void set_node(node_t **head){
  node_t *node = malloc(sizeof(node_t));
  node_t *tmp = *head;
  int prev_dir = 0;
  
  if(*head == NULL){
    switch(snake->dir){
    case 1:
      node->x = get_snake_x();
      node->y = get_snake_y() + 1;
      break;
    case 2:
      node->x = get_snake_x();
      node->y = get_snake_y() - 1;
      break;
    case 3:
      node->x = get_snake_x() - 1;
      node->y = get_snake_y();
      break;
    case 4:
      node->x = get_snake_x() + 1;
      node->y = get_snake_y();
      break;
    }
    *head = node;
  }else{
    while(tmp != NULL){
      if(tmp->next == NULL){
	prev_dir = tmp->dir;
	switch(prev_dir){
	case 1:
	  node->dir = prev_dir;
	  node->x = get_node_x(tmp);
	  node->y = get_node_y(tmp) + 1;
	  break;
	case 2:
	  node->dir = prev_dir;
	  node->x = get_node_x(tmp);
	  node->y = get_node_y(tmp) - 1;
	  break;
	case 3:
	  node->dir = prev_dir;
	  node->x = get_node_x(tmp) - 1;
	  node->y = get_node_y(tmp);	  
	  break;
	case 4:
	  node->dir = prev_dir;
	  node->x = get_node_x(tmp) + 1;
	  node->y = get_node_y(tmp);
	  break;
	}
      }
      tmp = tmp->next;
    }
    insert_at_end(*head, node);
  }
}
*/

void clear(){
  fprintf(stdout, "\033[2J");
  fprintf(stdout, "\033[H");
}

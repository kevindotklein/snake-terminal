#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "list.h"
#include "egg.h"

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
void spawn_egg(int x, int y);
int gen_allowed_x(node_t *head);
int gen_allowed_y(node_t *head);

char board[WIDTH * HEIGHT];
char input;
node_t *head;
egg_t *egg;
int egg_x;//
int egg_y;//

int main(void){
  srand(time(NULL));

  head = malloc(sizeof(node_t));
  head->x = 9;
  head->y = 4;
  head->dir = 0;

  egg_x = gen_allowed_x(head);//
  egg_y = gen_allowed_y(head);

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
  //
  board[(4 * WIDTH) + 20] = 'X';
}

void render_board(){
  for(int i=0; i<HEIGHT; i++){
    fwrite(&board[i*WIDTH], WIDTH, 1, stdout);
    fputc('\n', stdout);
  }
}

void update(){

  if(head->dir == 1) node_up(head);
  else if(head->dir == 2) node_down(head);
  else if(head->dir == 3) node_right(head);
  else if(head->dir == 4) node_left(head);

  spawn_egg(egg_x, egg_y);
  render_nodes(head);
  
  render_board();

  if(head->x >= WIDTH) input = 'q';
  else if(head->x < 0) input = 'q';
  if(head->y >= HEIGHT) input = 'q';
  else if(head->y < 0) input = 'q';
  
  if(get_node_x(head) == 20 && get_node_y(head) == 4){
    add_node(head);
  }
  
}

void render_nodes(node_t *head){
  node_t *tmp = head;
  
  while(tmp != NULL){

    if(!(tmp->next == NULL)){
   
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
	 
	  if(aux->y < tmp->y) tmp->dir = 1;
	  if(aux->y > tmp->y) tmp->dir = 2;
	  if(aux->x > tmp->x) tmp->dir = 3;
	  if(aux->x < tmp->x) tmp->dir = 4;
	}
	aux = aux->next;
      }
    }
    board[(tmp->y * WIDTH) + tmp->x] = '#';
    tmp = tmp->next;
  }
  
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

int gen_allowed_x(node_t *head){
  int pass=-1;
  int x;
  while(pass == -1){
    node_t *tmp = head;
    x = rand() % WIDTH;
    while(tmp != NULL){
      if(x == get_node_x(tmp)){
	break;
      }else{
	pass=0;
	break;
      }
      tmp = tmp->next;
    }
  }
  return x;
}

int gen_allowed_y(node_t *head){
  int pass=-1;
  int y;
  while(pass == -1){
    node_t *tmp = head;
    y = rand() % HEIGHT;
    while(tmp != NULL){
      if(y == get_node_y(tmp)){
	break;
      }else{
	pass=0;
	break;
      }
      tmp = tmp->next;
    }
  }
  return y;
}

void spawn_egg(int x, int y){

  egg_t *egg = malloc(sizeof(egg_t));
  egg->x = x;
  egg->y = y;

  board[(egg->y * WIDTH) + egg->x] = 'O';
}

void clear(){
  fprintf(stdout, "\033[2J");
  fprintf(stdout, "\033[H");
}

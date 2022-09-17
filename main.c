#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "list.h"

#define WIDTH 40
#define HEIGHT 15
#define FPS 15

typedef struct{
  int x;
  int y;
  int dir;
}snake_t;

void term_mode(int mode);
void init_board();
void update();
void clear();
void render_snake();
void snake_up();
void snake_down();
void snake_right();
void snake_left();
void render_node(node_t *head);
void set_node(node_t **head);
int get_snake_x();
int get_snake_y();
void node_up(node_t *tmp);
void node_down(node_t *tmp);
void node_right(node_t *tmp);
void node_left(node_t *tmp);

char board[WIDTH * HEIGHT];
char input;
snake_t *snake;
//node_t *test;
//node_t *test2;
node_t *head;

int main(void){
  
  snake = malloc(sizeof(snake_t));
  snake->x = 9;
  snake->y = 4;
  snake->dir = 0;
  head = NULL;

  set_node(&head);
  
  /*
  test = malloc(sizeof(node_t));
  test->x = 8;
  test->y = 4;
  test->next = NULL;
  insert_at_end(test, test);
  test2 = malloc(sizeof(node_t));
  test2->x = 7;
  test2->y = 4;
  test2->next = NULL;
  insert_at_end(test, test2);
  */

  term_mode(1);
  do{
    clear();
    init_board();
    update();
    fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
    scanf("%c", &input);
    switch(input){
    case 'w':
      if(snake->dir != 2){
	snake->dir = 1;
	break;
      }
      break;
    case 's':
      if(snake->dir != 1){
	snake->dir = 2;
	break;
      }
      break;
    case 'd':
      if(snake->dir != 4){
	snake->dir = 3;
	break;
      }
      break;
    case 'a':
      if(snake->dir != 3){
	snake->dir = 4;
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

void update(){
  render_snake();
  render_node(head);
  for(int i=0; i<HEIGHT; i++){
    fwrite(&board[i*WIDTH], WIDTH, 1, stdout);
    fputc('\n', stdout);
  }
  if(snake->dir == 1) snake_up();
  else if(snake->dir == 2) snake_down();
  else if(snake->dir == 3) snake_right();
  else if(snake->dir == 4) snake_left();
}

void render_snake(){
  if(snake->x >= WIDTH) snake->x = 0;
  else if(snake->x < 0) snake->x = WIDTH - 1;
  if(snake->y >= HEIGHT) snake->y = 0;
  else if(snake->y < 0) snake->y = HEIGHT - 1;
  board[(snake->y * WIDTH) + snake->x] = '#';
}

void render_node(node_t *head){
  node_t *tmp = head;
  while(tmp != NULL){
    switch(snake->dir){
    case 1:
      node_up(tmp);
      break;
    case 2:
      node_down(tmp);
      break;
    case 3:
      node_right(tmp);
      break;
    case 4:
      node_left(tmp);
      break;
    }
    board[(tmp->y * WIDTH) + tmp->x] = 'A';
    tmp = tmp->next;
  }
  
}

void set_node(node_t **head){
  node_t *node = malloc(sizeof(node_t));
  node->x = get_snake_x() - 1;
  node->y = get_snake_y();
  if(*head == NULL){
    /*
      switch(snake->dir){
      case 1:
	node->y = get_snake_y() + 1;
	node->dir = 1;
	break;
      case 2:
	node->x = get_snake_x() - 1;
	node->dir = 2;
	break;
      case 3:
	node->y = get_snake_y() - 1;
	node->dir = 3;
	break;
      case 4:
	node->x = get_snake_x() + 1;
	node->dir = 4;
	break;
      }
    */

    *head = node;
  }else{
    insert_at_end(*head, node);
  }
}

void snake_up(){
  snake->y--;
}

void snake_down(){
  snake->y++;
}

void snake_right(){
  snake->x++;
}

void snake_left(){
  snake->x--;
}

int get_snake_x(){
  return snake->x;
}

int get_snake_y(){
  return snake->y;
}

void node_right(node_t *tmp){
  if(get_snake_x() == 0){
    tmp->x = WIDTH - 1;
  }else{
    tmp->y = get_snake_y();
    tmp->x = get_snake_x() - 1;
  }
}

void node_up(node_t *tmp){
  if(get_snake_y() == HEIGHT - 1){
    tmp->y = 0;
  }else{
    tmp->x = get_snake_x();
    tmp->y = get_snake_y() + 1;
  }
}

void node_down(node_t *tmp){
  if(get_snake_y() == 0){
    tmp->y = HEIGHT - 1;
  }else{
    tmp->x = get_snake_x();
    tmp->y = get_snake_y() - 1;
  }
}

void node_left(node_t *tmp){
  if(get_snake_x() == HEIGHT - 1){
    tmp->x = 0;
  }else{
    tmp->y = get_snake_y();
    tmp->x = get_snake_x() + 1;
  }
}

void clear(){
  fprintf(stdout, "\033[2J");
  fprintf(stdout, "\033[H");
}

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <stdbool.h>

#define WIDTH 20
#define HEIGHT 10
#define FPS 15

typedef struct{
  int x;
  int y;
}Snake;

void term_mode(int mode);
void init_board();
void update();
void clear();
void render_snake();
void snake_up();
void snake_down();
void snake_right();
void snake_left();

char board[WIDTH * HEIGHT];
char input;
Snake *snake;

int main(void){
  
  snake = malloc(sizeof(Snake));
  snake->x = 9;
  snake->y = 4;
  
  term_mode(1);
  do{
    clear();
    init_board();
    update();
    scanf("%c", &input);
    switch(input){
    case 'w':
      snake_up();
      break;
    case 's':
      snake_down();
      break;
    case 'd':
      snake_right();
      break;
    case 'a':
      snake_left();
      break;
    }
    clear();
    usleep(1000 * 1000/FPS);
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
  for(int i=0; i<HEIGHT; i++){
    fwrite(&board[i*WIDTH], WIDTH, 1, stdout);
    fputc('\n', stdout);
  }
}

void render_snake(){
  if(snake->x >= WIDTH) snake->x = 0;
  else if(snake->x < 0) snake->x = WIDTH - 1;
  if(snake->y >= HEIGHT) snake->y = 0;
  else if(snake->y < 0) snake->y = HEIGHT - 1;
  board[(snake->y * WIDTH) + snake->x] = '#';
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

void clear(){
  fprintf(stdout, "\033[2J");
  fprintf(stdout, "\033[H");
}

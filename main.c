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

void term_mode(int mode);
void init_board();
void update();
void clear();

char board[WIDTH * HEIGHT];
char input;

int main(void){
  
  term_mode(1);
  do{
    clear();
    init_board();
    update();
    scanf("%c", &input);
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
  for(int i=0; i<HEIGHT; i++){
    fwrite(&board[i*WIDTH], WIDTH, 1, stdout);
    fputc('\n', stdout);
  }
}

void clear(){
  fprintf(stdout, "\033[2J");
  fprintf(stdout, "\033[H");
}

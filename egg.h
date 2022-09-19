#ifndef EGG_H
#define EGG_H

#include <stdio.h>
#include <stdlib.h>

struct egg{
  int x;
  int y;
};

typedef struct egg egg_t;

int get_egg_x(egg_t *egg);
int get_egg_y(egg_t *egg);

#endif

#ifndef BIND_FUNCTIONS_H
#define BIND_FUNCTIONS_H

#include <menu.h>

struct Argument {
  int i;
};

extern void move(struct Menu* menu, const struct Argument* argument);

#endif

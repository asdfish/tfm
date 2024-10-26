#ifndef BIND_FUNCTIONS_H
#define BIND_FUNCTIONS_H

#include <menu.h>

struct Argument {
  int i;
};

extern int change_directory(struct Menu* menu, const struct Argument* unused);
extern int move(struct Menu* menu, const struct Argument* argument);

#endif

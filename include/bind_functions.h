#ifndef BIND_FUNCTIONS_H
#define BIND_FUNCTIONS_H

#include <menu.h>

struct Argument {
  int i;
};

extern int change_directory(struct Menu* menu, const struct Argument* unused);
extern int cursor_bottom(struct Menu* menu, const struct Argument* unused);
extern int cursor_top(struct Menu* menu, const struct Argument* unused);
extern int move(struct Menu* menu, const struct Argument* argument);
extern int switch_cursor_with_selection(struct Menu* menu, const struct Argument* unused);
extern int toggle_visual_mode(struct Menu* menu, const struct Argument* unused);

#endif

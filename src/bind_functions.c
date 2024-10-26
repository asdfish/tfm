#include <bind_functions.h>

void move(struct Menu* menu, const struct Argument* argument) {
  menu_move_cursor(menu, argument->i);
}

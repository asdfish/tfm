#ifndef BIND_FUNCTIONS_H
#define BIND_FUNCTIONS_H

#include <command_line.h>
#include <menu.h>

struct Argument {
  const int i;
  const char c;
  const char* str;
};

extern int bind_function_change_command_line_mode(const struct Argument* argument);
extern int bind_function_exit_command_mode(const struct Argument* argument);

extern int bind_function_command_line_cursor_move(const struct Argument* argument);
extern int bind_function_command_line_delete_char(const struct Argument* argument);
extern int bind_function_command_line_execute(const struct Argument* argument);

extern int bind_function_menu_change_directory(const struct Argument* argument);
extern int bind_function_menu_cursor_move(const struct Argument* argument);
extern int bind_function_menu_cursor_move_bottom(const struct Argument* argument);
extern int bind_function_menu_cursor_move_top(const struct Argument* argument);
extern int bind_function_menu_switch_cursor_with_selection(const struct Argument* argument);
extern int bind_function_menu_toggle_visual_mode(const struct Argument* argument);

#endif

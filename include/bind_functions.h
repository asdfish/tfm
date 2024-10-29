#ifndef BIND_FUNCTIONS_H
#define BIND_FUNCTIONS_H

#include <command_line.h>
#include <menu.h>

struct Argument {
  const int i;
};

extern int bind_function_enter_command_mode(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);

extern int bind_function_command_line_cursor_move(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
/*extern int bind_function_command_line_submit(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);*/

extern int bind_function_menu_change_directory(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int bind_function_menu_cursor_move(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int bind_function_menu_cursor_move_bottom(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int bind_function_menu_cursor_move_top(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int bind_function_menu_switch_cursor_with_selection(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int bind_function_menu_toggle_visual_mode(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);

#endif

#ifndef BIND_FUNCTIONS_H
#define BIND_FUNCTIONS_H

#include <command_line.h>
#include <menu.h>

struct Argument {
  int i;
};

extern int change_directory(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int cursor_bottom(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int cursor_top(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int cursor_move(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int enter_command_mode(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int switch_cursor_with_selection(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);
extern int toggle_visual_mode(struct Menu* menu, struct CommandLine* command_line, const struct Argument* argument);

#endif

#ifndef COMMAND_LINE_H
#define COMMAND_LINE_H

#include <orchestra.h>
#include <termbox2.h>
#include <stdbool.h>

struct CommandLine {
  unsigned int x, y, width;                             // must be set
  uintattr_t foreground, foreground_reversed,
             background, background_reversed;           // must be set

  const char* message;                                  // do not change
  char mode;                                            // do not change
  unsigned int cursor, camera;                          // do not change
  o_string command;                                     // do not change
};

extern int command_line_add_char(struct CommandLine* command_line, char new_char);
extern int command_line_delete_char(struct CommandLine* command_line);
extern void command_line_draw(struct CommandLine* command_line);
extern int command_line_execute(struct CommandLine* command_line, bool* refresh_menu);
extern int command_line_init(struct CommandLine* command_line);
extern void command_line_move_cursor(struct CommandLine* command_line, int step);
extern int command_line_run(struct CommandLine* command_line, char mode);
extern void command_line_uninit(struct CommandLine* command_line);
extern void command_line_verify_cursor_position(struct CommandLine* command_line);

#endif

#ifndef CONFIG_H
#define CONFIG_H

#ifdef INCLUDE_CONFIG_FOREGROUNDS
#include <menu.h>
#include <termbox2.h>
#include <dirent.h>

static const uintattr_t foregrounds[] = {
  [DT_UNKNOWN] = TB_WHITE,
  [DT_FIFO] = TB_WHITE,
  [DT_CHR] = TB_WHITE,
  [DT_DIR] = TB_BLUE,
  [DT_BLK] = TB_WHITE,
  [DT_REG] = TB_WHITE,
  [DT_LNK] = TB_GREEN,
};

static const uintattr_t foregrounds_reversed[] = {
  [DT_UNKNOWN] = TB_BLACK,
  [DT_FIFO] = TB_BLACK,
  [DT_CHR] = TB_BLACK,
  [DT_DIR] = TB_GREEN,
  [DT_BLK] = TB_BLACK,
  [DT_REG] = TB_BLACK,
  [DT_LNK] = TB_BLUE,
};
#endif // INCLUDE_FOREGROUNDS

#ifdef INCLUDE_CONFIG_BINDINGS
#include <bind_functions.h>

struct BindingStroke {
  const char mode; // set to ' ' for all
  const char* chars;

  int (*function) (const struct Argument*);
  const struct Argument argument;
};

static const struct BindingStroke binding_strokes[] = {
  // normal
  { 'n', " ",  bind_function_menu_change_directory },
  { 'n', "v",  bind_function_menu_toggle_visual_mode },
  { 'n', ":",  bind_function_change_command_line_mode, { .c = ':' } },
  { 'n', "/",  bind_function_change_command_line_mode, { .c = '/' } },

  { 'n', "k",  bind_function_menu_cursor_move, { .i = -1 } },
  { 'n', "j",  bind_function_menu_cursor_move, { .i = 1 } },
  { 'n', "G",  bind_function_menu_cursor_move_bottom },
  { 'n', "gg", bind_function_menu_cursor_move_top },

  // visual
  { 'v', "o",  bind_function_menu_switch_cursor_with_selection },
  { 'v', "i",  bind_function_menu_toggle_visual_mode },

  { 'v', "k",  bind_function_menu_cursor_move, { .i = -1 } },
  { 'v', "j",  bind_function_menu_cursor_move, { .i = 1 } },
  { 'v', "G",  bind_function_menu_cursor_move_bottom },
  { 'v', "gg", bind_function_menu_cursor_move_top },
};

struct BindingKey {
  const char mode;
  const uint16_t key; // TB_KEY_*

  int (*function) (const struct Argument*);
  const struct Argument argument;
};

static const struct BindingKey binding_keys[] = {
  // command
  { '/', TB_KEY_ESC,         bind_function_exit_command_mode },
  { '/', TB_KEY_ARROW_LEFT,  bind_function_command_line_cursor_move, { .i = -1 } },
  { '/', TB_KEY_ARROW_RIGHT, bind_function_command_line_cursor_move, { .i = 1 } },
  { '/', TB_KEY_BACKSPACE,   bind_function_command_line_delete_char },
  { '/', TB_KEY_BACKSPACE2,  bind_function_command_line_delete_char },
  { '/', TB_KEY_ENTER,       bind_function_command_line_execute },
  { ':', TB_KEY_ESC,         bind_function_exit_command_mode },
  { ':', TB_KEY_ARROW_LEFT,  bind_function_command_line_cursor_move, { .i = -1 } },
  { ':', TB_KEY_ARROW_RIGHT, bind_function_command_line_cursor_move, { .i = 1 } },
  { ':', TB_KEY_BACKSPACE,   bind_function_command_line_delete_char },
  { ':', TB_KEY_BACKSPACE2,  bind_function_command_line_delete_char },
  { ':', TB_KEY_ENTER,       bind_function_command_line_execute },
};

#endif

#ifdef INCLUDE_CONFIG_COMMANDS

#include <commands.h>
#include <stdbool.h>

#define MESSAGE_COMMAND_NOT_FOUND "Command %s not found."
#define MESSAGE_COMMAND_NOT_ENOUGH_ARGUMENTS "Not enough arguments were submitted."
#define MESSAGE_CREATE_PATHS "Successfully created %u/%u paths."
#define MESSAGE_REMOVE_PATHS "Successfully removed %u/%u paths."

struct Command {
  const char* name;

  int (*function) (const char**, unsigned int, bool*, char**);
};

static const struct Command commands[] = {
  { "create_paths", command_create_paths },
  { "remove_paths", command_remove_paths },
  { "quit", command_quit },
};

#endif

// the max amount of directories that ntfw will store
#define REMOVE_RECURSIVE_MAX_DIRECTORIES 64

#endif

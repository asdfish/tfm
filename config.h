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

struct Binding {
  char mode; // set to ' ' for all
  const char* strokes;

  int (*function) (struct Menu*, struct CommandLine*, const struct Argument*);
  const struct Argument argument;
};

static const struct Binding bindings[] = {
  // normal
  { 'n', " ", change_directory },
  { 'n', "v", toggle_visual_mode },
  { 'n', ":", enter_command_mode },

  { 'n', "G", cursor_bottom },
  { 'n', "gg", cursor_top },
  { 'n', "k", cursor_move, { .i = -1 } },
  { 'n', "j", cursor_move, { .i = 1 } },

  // visual

  { 'v', "o", switch_cursor_with_selection },
  { 'v', "i", toggle_visual_mode },

  { 'v', "G", cursor_bottom },
  { 'v', "gg", cursor_top },
  { 'v', "k", cursor_move, { .i = -1 } },
  { 'v', "j", cursor_move, { .i = 1 } },
};

#endif

#ifdef INCLUDE_CONFIG_COMMANDS

#include <commands.h>
#include <stdbool.h>

#define MESSAGE_COMMAND_NOT_FOUND "Command %s not found."
#define MESSAGE_CREATE_PATHS "Successfully created %u/%u paths."

struct Command {
  const char* name;

  int (*function) (const char**, unsigned int, bool*, char**);
};

static const struct Command commands[] = {
  { "create_paths", create_paths }
};

#endif

#endif

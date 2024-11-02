#ifndef COMMANDS_H
#define COMMANDS_H

#include <macros.h>
#include <stdbool.h>

extern int command_move_paths(const char** argv, unsigned int argc, bool* refresh, char** message);
extern int command_quit(const char** argv, unsigned int argc, bool* refresh, char** message);

extern GENERATE_PATH_COMMAND_DEFINITION(command_create_paths);
extern GENERATE_PATH_COMMAND_DEFINITION(command_remove_paths);

#endif

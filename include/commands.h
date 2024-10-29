#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>

extern int command_create_paths(const char** paths, unsigned int paths_length, bool* refresh, char** message);
extern int command_quit(const char** argv, unsigned int argc, bool* refresh, char** message);

#endif

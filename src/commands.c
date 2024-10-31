#define INCLUDE_CONFIG_COMMANDS
#include <config.h>
#include <commands.h>
#include <utils/filesystem.h>
#include <utils/string.h>
#include <tfm.h>
#include <stddef.h>
#include <stdio.h>

int command_quit(const char** argv, unsigned int argc, bool* refresh, char** message) {
  run = false;
  return 0;
}

GENERATE_PATH_COMMAND_DEFINITION(command_create_paths)
  GENERATE_PATH_COMMAND_SOURCE(create_path, MESSAGE_CREATE_PATHS)

GENERATE_PATH_COMMAND_DEFINITION(command_remove_paths)
  GENERATE_PATH_COMMAND_SOURCE(remove_path, MESSAGE_REMOVE_PATHS)

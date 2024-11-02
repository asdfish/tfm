#define INCLUDE_CONFIG_COMMANDS
#include <config.h>
#include <commands.h>
#include <utils/filesystem.h>
#include <utils/string.h>
#include <tfm.h>
#include <stddef.h>
#include <stdio.h>

int command_move_paths(const char** argv, unsigned int argc, bool* refresh, char** message) {
  if(argc == 0 || argc % 2 != 0) {
    *message = strdup(MESSAGE_COMMAND_NOT_ENOUGH_ARGUMENTS);
    if(*message == NULL)
      return -1;
    return 0;
  }

  unsigned int successes = 0;
  for(unsigned int i = 0; i < argc; i += 2)
    if(rename(argv[i], argv[i + 1]) == 0)
      successes ++;

  unsigned int message_length = strlen(MESSAGE_MOVE_PATHS) +
    NUMBER_STRING_LENGTH(successes) + NUMBER_STRING_LENGTH(argc / 2) -
    (string_count(MESSAGE_MOVE_PATHS, "%u") * 2);
  *message = (char*) malloc(message_length * sizeof(char));

  sprintf(*message, MESSAGE_MOVE_PATHS, successes, argc / 2);
  *refresh = true;
  return 0;
}

int command_quit(const char** argv, unsigned int argc, bool* refresh, char** message) {
  run = false;
  return 0;
}

GENERATE_PATH_COMMAND_DEFINITION(command_create_paths)
  GENERATE_PATH_COMMAND_SOURCE(create_path, MESSAGE_CREATE_PATHS)

GENERATE_PATH_COMMAND_DEFINITION(command_remove_paths)
  GENERATE_PATH_COMMAND_SOURCE(remove_path, MESSAGE_REMOVE_PATHS)

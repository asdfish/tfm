#define INCLUDE_CONFIG_COMMANDS
#include <config.h>
#include <commands.h>
#include <macros.h>
#include <utils/filesystem.h>
#include <utils/string.h>
#include <tfm.h>
#include <stddef.h>
#include <stdio.h>

int command_create_paths(const char** paths, unsigned int paths_length, bool* refresh, char** message) {
  if(paths_length == 0) {
    *message = strdup(MESSAGE_COMMAND_NOT_ENOUGH_ARGUMENTS);
    if(*message == NULL)
      return -1;
    return 0;
  }

  unsigned int successes = 0;
  for(unsigned int i = 0; i < paths_length; i ++)
    if(create_path(paths[i]) == 0)
      successes ++;

  unsigned int message_length = strlen(MESSAGE_CREATE_PATHS) +
    NUMBER_STRING_LENGTH(successes) + NUMBER_STRING_LENGTH(paths_length) -
    (string_count(MESSAGE_CREATE_PATHS, "%u") * 2);

  *message = (char*) malloc((message_length + 1) * sizeof(char));
  if(*message == NULL)
    return -1;

  sprintf(*message, MESSAGE_CREATE_PATHS, successes, paths_length);

  *refresh = true;
  return 0;
}

int command_quit(const char** argv, unsigned int argc, bool* refresh, char** message) {
  run = false;
  return 0;
}

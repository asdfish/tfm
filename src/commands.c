#define INCLUDE_CONFIG_COMMANDS
#include <config.h>
#include <commands.h>
#include <macros.h>
#include <utils.h>
#include <stddef.h>
#include <stdio.h>

int create_paths(const char** paths, unsigned int paths_length, bool* refresh, char** message) {
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

#ifndef MACROS_H
#define MACROS_H

// misc
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#define NUMBER_STRING_LENGTH(number) ((number / 10) + 1)

// path commands
#define GENERATE_PATH_COMMAND_DEFINITION(name) \
  int name(const char** paths, unsigned int paths_length, bool* refresh, char** message)

#define GENERATE_PATH_COMMAND_SOURCE(function, message_format) {           \
  if(paths_length == 0) {                                                  \
    *message = strdup(MESSAGE_COMMAND_NOT_ENOUGH_ARGUMENTS);               \
    if(*message == NULL)                                                   \
      return -1;                                                           \
    return 0;                                                              \
  }                                                                        \
                                                                           \
  unsigned int successes = 0;                                              \
  for(unsigned int i = 0; i < paths_length; i ++)                          \
    if(function(paths[i]) == 0)                                            \
      successes ++;                                                        \
                                                                           \
  unsigned int message_length = strlen(message_format) +                   \
    NUMBER_STRING_LENGTH(successes) + NUMBER_STRING_LENGTH(paths_length) - \
    (string_count(message_format, "%u") * 2);                              \
                                                                           \
  *message = (char*) malloc((message_length + 1) * sizeof(char));          \
  if(*message == NULL)                                                     \
    return -1;                                                             \
                                                                           \
  sprintf(*message, message_format, successes, paths_length);              \
                                                                           \
  *refresh = true;                                                         \
  return 0;                                                                \
}

#endif

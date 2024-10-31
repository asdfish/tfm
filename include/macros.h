#ifndef MACROS_H
#define MACROS_H

// misc
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#define NUMBER_STRING_LENGTH(number) ((number / 10) + 1)

// dirent
#define GENERATE_GET_DIRENT_NAMES_FUNCTION_DEFINITION(name) \
  int name(const char* path, const char*** output, unsigned int* output_length)

#define GENERATE_GET_DIRENT_NAMES_FUNCTION_SOURCE(condition) {                                  \
  DIR* directory_pointer = opendir(path);                                                       \
  if(directory_pointer == NULL)                                                                 \
    goto exit_failure;                                                                          \
                                                                                                \
  struct dirent* dirent_pointer = NULL;                                                         \
                                                                                                \
  unsigned int dirent_names_length = 0;                                                         \
  while((dirent_pointer = readdir(directory_pointer)) != NULL)                                  \
    if(condition)                                                                               \
      dirent_names_length ++;                                                                   \
  closedir(directory_pointer);                                                                  \
  directory_pointer = NULL;                                                                     \
                                                                                                \
  const char** dirent_names = (const char**) malloc(dirent_names_length * sizeof(const char*)); \
  if(dirent_names == NULL)                                                                      \
    goto exit_failure;                                                                          \
                                                                                                \
  directory_pointer = opendir(path);                                                            \
  if(directory_pointer == NULL)                                                                 \
    goto free_dirent_names;                                                                     \
                                                                                                \
  unsigned int i = 0;                                                                           \
  while((dirent_pointer = readdir(directory_pointer)) != NULL)                                  \
    if(condition) {                                                                             \
      dirent_names[i] = strdup(dirent_pointer->d_name);                                         \
      if(dirent_names[i] == NULL)                                                               \
        goto free_dirent_names_contents;                                                        \
      i ++;                                                                                     \
    }                                                                                           \
  closedir(directory_pointer);                                                                  \
  directory_pointer = NULL;                                                                     \
                                                                                                \
  *output = dirent_names;                                                                       \
  *output_length = dirent_names_length;                                                         \
                                                                                                \
  return 0;                                                                                     \
                                                                                                \
free_dirent_names_contents:                                                                     \
  for(unsigned int j = 0; j < i; j ++) {                                                        \
    free((char*) dirent_names[i]);                                                              \
    dirent_names[i] = NULL;                                                                     \
  }                                                                                             \
  closedir(directory_pointer);                                                                  \
  directory_pointer = NULL;                                                                     \
free_dirent_names:                                                                              \
  free(dirent_names);                                                                           \
  dirent_names = NULL;                                                                          \
exit_failure:                                                                                   \
  return -1;                                                                                    \
}

#define IS_DIRECTORY_CONDITION dirent_pointer->d_type == DT_DIR &&                                 \
                                                       strcmp(dirent_pointer->d_name, ".") != 0 && \
                                                       strcmp(dirent_pointer->d_name, "..") != 0

#define IS_FILE_CONDITION dirent_pointer->d_type == DT_REG

// path commands
#define GENERATE_PATH_COMMAND_DEFINITION(name) \
  int name(const char** paths, unsigned int paths_length, bool* refresh, char** message)

#define GENERATE_PATH_COMMAND_SOURCE(function, message_format_success) {   \
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
  unsigned int message_length = strlen(MESSAGE_CREATE_PATHS) +             \
    NUMBER_STRING_LENGTH(successes) + NUMBER_STRING_LENGTH(paths_length) - \
    (string_count(MESSAGE_CREATE_PATHS, "%u") * 2);                        \
                                                                           \
  *message = (char*) malloc((message_length + 1) * sizeof(char));          \
  if(*message == NULL)                                                     \
    return -1;                                                             \
                                                                           \
  sprintf(*message, MESSAGE_CREATE_PATHS, successes, paths_length);        \
                                                                           \
  *refresh = true;                                                         \
  return 0;                                                                \
}

#endif

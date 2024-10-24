#include <actions.h>
#include <utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Action* actions = NULL;
unsigned int actions_length = 0;

int actions_add(enum ActionType type, const char** arguments, unsigned int arguments_length, bool free_arguments_contents) {
  const char* working_directory = realpath(".", NULL);
  if(working_directory == NULL)
    goto exit_failure;

  unsigned int old_length = actions_length;
  int result = actions_resize(actions_length + 1);
  if(result != 0)
    goto free_action_working_directory;

  actions[old_length].type = type;
  actions[old_length].arguments = arguments;
  actions[old_length].arguments_length = arguments_length;
  actions[old_length].working_directory = working_directory;
  actions[old_length].free_arguments_contents = free_arguments_contents;

  return 0;
free_action_working_directory:
  free((char*) working_directory);
  working_directory = NULL;
exit_failure:
  return -1;
}

void actions_free(void) {
  if(actions == NULL || actions_length == 0)
    return;
  
  for(unsigned int i = 0; i < actions_length; i ++)
    actions_free_at(i);

  free(actions);
  actions = NULL;
  actions_length = 0;
}

void actions_free_at(int index) {
  if(actions[index].free_arguments_contents)
    for(unsigned int j = 0; j < actions[index].arguments_length; j ++)
      if(actions[index].arguments[j] != NULL) {
        free((char*) actions[index].arguments[j]);
        actions[index].arguments[j] = NULL;
      }

  if(actions[index].arguments != NULL) {
    free(actions[index].arguments);
    actions[index].arguments = NULL;
  }
  if(actions[index].working_directory != NULL) {
    free((char*) actions[index].working_directory);
    actions[index].working_directory = NULL;
  }
}

int actions_resize(unsigned int new_size) {
  if(new_size == 0) {
    actions_free();
    return 0;
  }

  if(actions == NULL) {
    actions = (struct Action*) malloc(new_size * sizeof(struct Action));
    if(actions == NULL)
      return -1;
    goto exit_success;
  }

  if(new_size < actions_length)
    for(unsigned int i = new_size; i < actions_length; i ++)
      actions_free_at(i);

  void* temp_pointer = realloc(actions, new_size * sizeof(struct Action));
  if(temp_pointer == NULL)
    return -1;

exit_success:
  actions_length = new_size;
  return 0;
}

int actions_create_path(const char* path) {
  const char** directories = NULL;
  unsigned int directories_length = 0;
  const char* file = NULL;

  int result = path_separate(path, &directories, &directories_length, &file);
  if(result != 0)
    return result;

  if(file != NULL && directories == NULL) {
    const char** arguments = (const char**) malloc(sizeof(const char*));
    if(arguments == NULL)
      goto single_file_free_file;

    arguments[0] = file;
    if(actions_add(CREATE_FILE, arguments, 1, true) != 0) {
      actions_length --;
      actions_free_at(actions_length);
      goto single_file_exit_failure;
    }

    return 0;

single_file_free_file:
    free((char*) file);
single_file_exit_failure:
    return -1;
  }

  unsigned int path_max_length = 0;
  for(unsigned int i = 0; i < directories_length; i ++)
    path_max_length += strlen(directories[i]) + 1;

  char* directory = (char*) malloc(path_max_length * sizeof(char));
  if(directory == NULL)
    goto free_directories;

  unsigned int old_length = actions_length;

  for(unsigned int i = 0; i < directories_length; i ++) {
    const char** arguments = (const char**) malloc(sizeof(const char*));
    arguments[0] = strdup(directory);
    actions_add(CREATE_DIRECTORY, arguments, 1, true);

    strcat(directory, directories[i]);
    strcat(directory, "/");
  }

  free(directory);
  return 0;

free_directories:
  for(unsigned int i = 0; i < directories_length; i ++) {
    free((char*) directories[i]);
    directories[i] = NULL;
  }
  free(directories);
  directories = NULL;
  return -1;
}

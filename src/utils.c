#define INCLUDE_CONFIG_FOREGROUNDS
#define INCLUDE_CONFIG_BINDINGS
#include <config.h>
#include <macros.h>
#include <utils.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int dirents_to_menu_items(struct dirent** dirents, unsigned int dirents_length, struct MenuItem** output) {
  *output = (struct MenuItem*) malloc(dirents_length * sizeof(struct MenuItem));
  if(*output == NULL)
    return -1;

  for(unsigned int i = 0; i < dirents_length; i ++) {
    (*output)[i].contents = dirents[i]->d_name;
    (*output)[i].foreground = foregrounds[dirents[i]->d_type];
    (*output)[i].foreground_reversed = foregrounds_reversed[dirents[i]->d_type];
  }

  return 0;
}

int get_dirents(const char* path, struct dirent*** output, unsigned int* output_length) {
  DIR* directory_pointer = NULL;
  if((directory_pointer = opendir(path)) == NULL)
    goto failure_exit;

  struct dirent* dirent_pointer = NULL;

  unsigned int directory_length = 0;
  while((dirent_pointer = readdir(directory_pointer)) != NULL)
    directory_length ++;

  closedir(directory_pointer);
  directory_pointer = NULL;

  *output = (struct dirent**) malloc(directory_length * sizeof(struct dirent*));
  if(*output == NULL)
    goto failure_exit;

  directory_pointer = opendir(path);
  if(directory_pointer == NULL)
    goto free_output;

  unsigned int i = 0;
  while((dirent_pointer = readdir(directory_pointer)) != NULL) {
    (*output)[i] = (struct dirent*) malloc(sizeof(struct dirent));
    if((*output)[i] == NULL)
      goto free_output_contents;
    memmove((*output)[i], dirent_pointer, sizeof(struct dirent));
    i ++;
  }

  closedir(directory_pointer);
  directory_pointer = NULL;

  *output_length = directory_length;
  return 0;

free_output_contents:
  for(unsigned int j = 0; j <= i; j ++) {
    free((*output)[j]);
    (*output)[j] = NULL;
  }
free_output:
  free(*output);
  *output = NULL;
failure_exit:
  return -1;
}

unsigned int string_count(const char* string, const char* query) {
  unsigned int count = 0;

  char* string_pointer = (char*) string;
  while((string_pointer = strstr(string_pointer, query)) != NULL) {
    string_pointer ++;
    count ++;
  }

  return count;
}

enum PathType path_type(const char* path) {
  if(path[strlen(path) - 1] == '/')
    return PATH_DIRECTORY;

  return PATH_FILE;
}

int path_separate(const char* path, const char*** directories, unsigned int* directories_length, const char** file) {
  if(strstr(path, "/") == NULL) {
    if((*file = strdup(path)) == NULL) // consistency
      return -1;
    else
      return 0;
  }

  enum PathType type = path_type(path);

  unsigned int directories_count = string_count(path, "/");
  /*if(type == PATH_DIRECTORY)*/
  /*  directories_count --;*/

  *directories = (const char**) malloc(directories_count * sizeof(const char*));
  if(*directories == NULL)
    goto exit_failure;

  char* path_copy = strdup(path);
  if(path_copy == NULL)
    goto free_directories;

  unsigned int i = 0;
  const char* current_path = NULL;
  char* path_copy_pointer = path_copy;
  while((current_path = strsep(&path_copy_pointer, "/")) != NULL) {
    if(i == directories_count) {
      if(type == PATH_DIRECTORY)
        break;

      *file = strdup(current_path);
      if(*file == NULL)
        goto free_path_copy;

      break;
    }

    (*directories)[i] = strdup(current_path);
    if((*directories)[i] == NULL)
      goto free_directories_contents;
    i ++;
  }

  free(path_copy);
  path_copy = NULL;
  *directories_length = directories_count;
  return 0;

free_path_copy:
  free(path_copy);
  path_copy = NULL;
free_directories_contents:
  for(unsigned int j = 0; j < i; j ++) {
    free((char*) (*directories)[j]);
    (*directories)[j] = NULL;
  }
free_directories:
  free(*directories);
  *directories = NULL;
exit_failure:
  return -1;
}

#include <config.h>
#include <utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int create_menu_items(struct dirent** dirents, unsigned int dirents_length, struct MenuItem** output) {
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

int directory_dirents(const char* path, struct dirent*** output, unsigned int* output_length) {
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

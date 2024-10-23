#include <utils.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    memcpy((*output)[i], dirent_pointer, sizeof(struct dirent));
    i ++;
  }

  closedir(directory_pointer);
  directory_pointer = NULL;

  *output_length = directory_length;
  return 0;

free_output_contents:
  for(unsigned int j = 0; j < i; j ++) {
    free((*output)[j]);
    (*output)[j] = NULL;
  }
free_output:
  free(*output);
  *output = NULL;
failure_exit:
  return -1;
}

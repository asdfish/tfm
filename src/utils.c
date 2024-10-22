#include <utils.h>

#include <stdlib.h>

int directory_dirents(const char* path, struct dirent* output) {
  DIR* directory_pointer = opendir(path);
  if(directory_pointer == NULL)
    return -1;

  unsigned int directory_length = 0;
  struct dirent* dirent_pointer = NULL;
  while((dirent_pointer = readdir(directory_pointer)) != NULL)
    directory_length ++;

  closedir(directory_pointer);
  directory_pointer = NULL;

  if(directory_length == 0)
    return -1;

  output = (struct dirent*) malloc(directory_length * sizeof(struct dirent));
  if(output == NULL)
    return -1;

  directory_pointer = opendir(path);
  unsigned int i = 0;
  while((dirent_pointer = readdir(directory_pointer)) != NULL) {
    output[i] = *dirent_pointer;
    i ++;
  }
  closedir(directory_pointer);
  directory_pointer = NULL;
  return 0;
}

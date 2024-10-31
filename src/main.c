/*#include <tfm.h>*/
/*int main(void) {*/
/*  return tfm();*/
/*}*/

/*int get_dirent_names_recursive(const char* path, const char*** output, unsigned int* output_length)*/

#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
int main(void) {
  const char** dirs = NULL;
  unsigned int dirs_len = 0;

  if(get_dirent_names_recursive(".", &dirs, &dirs_len) != 0) {
    printf("get_dirent_names_recursive\n");
    return -1;
  }

  for(unsigned int i = 0; i < dirs_len; i ++) {
    printf("%s\n", dirs[i]);
    free((char*) dirs[i]);
    dirs[i] = NULL;
  }
  free(dirs);
  dirs = NULL;

  return 0;
}

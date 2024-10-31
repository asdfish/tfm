/*#include <tfm.h>*/
/*int main(void) {*/
/*  return tfm();*/
/*}*/

/*int get_dirent_names_recursive(const char* path, const char*** output, unsigned int* output_length)*/

#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
int main(void) {
  const char** names = NULL;
  unsigned int names_length = 0;

  get_dirent_names_recursive(".", &names, &names_length);

  return 0;
}

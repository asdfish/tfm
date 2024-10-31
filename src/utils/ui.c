#define INCLUDE_CONFIG_FOREGROUNDS
#include <config.h>
#include <utils/ui.h>

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

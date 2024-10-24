#include <actions.h>
/*#include <utils.h>*/

#include <stdio.h>
#include <stdlib.h>

/*extern int actions_add(enum ActionType type, const char** arguments, unsigned int argument_length);*/
int main(void) {
  if(actions_create_path("asdf/") != 0) {
    printf("error\n");
    actions_free();
    return -1;
  }

  actions_free();
  return 0;
}

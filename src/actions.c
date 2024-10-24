#include <actions.h>

#include <stdlib.h>

struct Action* actions = NULL;
unsigned int actions_length = 0;

int actions_add(enum ActionType type, const char** arguments, unsigned int arguments_length) {
  unsigned int old_length = actions_length;
  int result = actions_resize(actions_length + 1);
  if(result != 0)
    return result;

  actions[old_length].type = type;
  actions[old_length].arguments = arguments;
  actions[old_length].arguments_length = arguments_length;

  return 0;
}

void actions_free(void) {
  if(actions == NULL)
    return;
  
  for(unsigned int i = 0; i < actions_length; i ++) {
    free(actions[i].arguments);
    actions[i].arguments = NULL;
  }
  free(actions);
  actions = NULL;
  actions_length = 0;
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
    for(unsigned int i = new_size; i < actions_length; i ++) {
      free(actions[i].arguments);
      actions[i].arguments = NULL;
    }

  void* temp_pointer = realloc(actions, new_size * sizeof(struct Action));
  if(temp_pointer == NULL)
    return -1;

exit_success:
  actions_length = new_size;
  return 0;
}

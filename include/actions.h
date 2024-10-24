#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdbool.h>

enum ActionType {
  CREATE_FILE,      // file file ...
  CREATE_DIRECTORY, // directory directory ...
  DELETE_FILE,      // file file ...
  DELETE_DIRECTORY, // directory directory ...
  MOVE,             // file new_position file new_position ...
};

struct Action {
  enum ActionType type;
  const char** arguments;
  unsigned int arguments_length;
  const char* working_directory;

  bool free_arguments_contents;
};

extern struct Action* actions;
extern unsigned int actions_length;

extern int actions_add(enum ActionType type, const char** arguments, unsigned int argument_length, bool free_arguments);
extern void actions_free(void);
extern void actions_free_at(int index);
extern int actions_resize(unsigned int new_size);

extern int actions_create_path(const char* path);
extern int actions_move_path(const char* from, const char* to);

#endif

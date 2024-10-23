#ifndef ACTIONS_H
#define ACTIONS_H

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
};

extern struct Action* actions;
extern unsigned int actions_length;

extern int actions_add(enum ActionType type, const char** arguments, unsigned int argument_length);
extern void actions_free(void);
extern int actions_resize(unsigned int new_size);

extern int create_file(const char* path);
extern int create_directory(const char* path);
extern int move(const char* from, const char* to);

#endif

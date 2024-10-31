#ifndef UI_H
#define UI_H

#include <menu.h>
#include <dirent.h>

extern int dirents_to_menu_items(struct dirent** dirents, unsigned int dirents_length, struct MenuItem** output);

#endif

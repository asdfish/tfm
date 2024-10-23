#ifndef UTILS_H
#define UTILS_H

#include <menu.h>

#include <dirent.h>

extern int create_menu_items(struct dirent** dirents, unsigned int dirents_length, struct MenuItem** output);
extern int directory_dirents(const char* path, struct dirent*** output, unsigned int* output_length);

#endif

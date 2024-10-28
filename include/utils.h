#ifndef UTILS_H
#define UTILS_H

#include <menu.h>
#include <dirent.h>
#include <stdbool.h>

enum PathType {
  PATH_DIRECTORY,
  PATH_FILE
};

extern int create_file(const char* path);
extern int create_directory(const char* path);
extern int create_path(const char* path);
extern int dirents_to_menu_items(struct dirent** dirents, unsigned int dirents_length, struct MenuItem** output);
extern int get_dirents(const char* path, struct dirent*** output, unsigned int* output_length);
extern int sentence_separate(const char* sentence, const char*** output, unsigned int* output_length);
extern unsigned int string_count(const char* string, const char* query);
extern bool path_exists(const char* path);
extern int path_separate(const char* path, const char*** directories, unsigned int* directories_length, const char** file);
extern enum PathType path_type(const char* path);

#endif

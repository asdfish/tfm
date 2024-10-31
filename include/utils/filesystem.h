#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <macros.h>
#include <dirent.h>
#include <ftw.h>
#include <stdbool.h>

enum PathType {
  PATH_DIRECTORY,
  PATH_FILE,
  PATH_UNKNOWN,
};

extern int create_file(const char* path);
extern int create_directory(const char* path);
extern int create_path(const char* path);
extern int get_dirents(const char* path, struct dirent*** output, unsigned int* output_length);
extern bool path_exists(const char* path);
extern int path_separate(const char* path, const char*** directories, unsigned int* directories_length, const char** file);
extern int path_actual_type(const char* path, enum PathType* type); // checks os file type
extern enum PathType path_type(const char* path); // checks file type based on string
extern int remove_path(const char* path);
extern int remove_directory(const char* path);
extern int remove_directory_nftw_callback(const char* fpath, const struct stat* sb, int typeflag, struct FTW* ftwbuf);

extern GENERATE_GET_DIRENT_NAMES_FUNCTION_DEFINITION(get_directory_names);
extern GENERATE_GET_DIRENT_NAMES_FUNCTION_DEFINITION(get_file_names);

#endif

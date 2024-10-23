#ifndef UTILS_H
#define UTILS_H

#include <dirent.h>

extern int directory_dirents(const char* path, struct dirent*** output, unsigned int* output_length);

#endif

#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE 500
#include <config.h>
#include <utils/filesystem.h>
#include <utils/string.h>
#include <orchestra/orchestra.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int create_file(const char* path) {
  if(path_exists(path))
    return -1;

  FILE* file_pointer = fopen(path, "w");
  if(file_pointer == NULL)
    return -1;
  fclose(file_pointer);

  return 0;
}

int create_directory(const char* path) {
  if(path_exists(path))
    return -1;

  return mkdir(path, 0700);
}

int create_path(const char* path) {
  const char** directories = NULL;
  unsigned int directories_length = 0;
  const char* file = NULL;

  if(path_separate(path, &directories, &directories_length, &file) != 0)
    return -1;

  int exit_code = 0;
  
  if(file != NULL && directories == NULL) {
    exit_code = create_file(file);
    goto free_paths;
  }

  o_string directory;
  if(o_string_init(&directory) != 0) {
    exit_code = -1;
    goto free_paths;
  }

  for(unsigned int i = 0; i < directories_length; i ++) {
    if(o_string_cat(&directory, directories[i]) != 0 ||
        o_string_cat(&directory, "/") != 0) {
      exit_code = -1;
      goto free_directory;
    }

    if(create_directory(directory.contents) != 0) {
      exit_code = -1;
      goto free_directory;
    }
  }

  if(file != NULL) {
    if(o_string_cat(&directory, file) != O_SUCCESS) {
      exit_code = -1;
      goto free_directory;
    }

    if(create_file(directory.contents) != 0) {
      exit_code = -1;
      goto free_directory;
    }
  }

free_directory:
  o_string_uninit(&directory);
free_paths:
  if(directories != NULL) {
    for(unsigned int i = 0; i < directories_length; i ++) {
      free((char*) directories[i]);
      directories[i] = NULL;
    }
    free(directories);
    directories = NULL;
  }
  if(file != NULL) {
    free((char*) file);
    file = NULL;
  }
  return exit_code;
}

int get_dirents(const char* path, struct dirent*** output, unsigned int* output_length) {
  DIR* directory_pointer = NULL;
  if((directory_pointer = opendir(path)) == NULL)
    goto failure_exit;

  struct dirent* dirent_pointer = NULL;

  unsigned int directory_length = 0;
  while((dirent_pointer = readdir(directory_pointer)) != NULL)
    if(strcmp(dirent_pointer->d_name, ".") != 0 &&
        strcmp(dirent_pointer->d_name, "..") != 0)
      directory_length ++;

  closedir(directory_pointer);
  directory_pointer = NULL;

  *output = (struct dirent**) malloc(directory_length * sizeof(struct dirent*));
  if(*output == NULL)
    goto failure_exit;

  directory_pointer = opendir(path);
  if(directory_pointer == NULL)
    goto free_output;

  unsigned int i = 0;
  while((dirent_pointer = readdir(directory_pointer)) != NULL)
    if(strcmp(dirent_pointer->d_name, ".") != 0 &&
        strcmp(dirent_pointer->d_name, "..") != 0) {
      (*output)[i] = (struct dirent*) malloc(sizeof(struct dirent));
      if((*output)[i] == NULL)
        goto free_output_contents;
      memmove((*output)[i], dirent_pointer, sizeof(struct dirent));
      i ++;
    }

  closedir(directory_pointer);
  directory_pointer = NULL;

  *output_length = directory_length;
  return 0;

free_output_contents:
  for(unsigned int j = 0; j <= i; j ++) {
    free((*output)[j]);
    (*output)[j] = NULL;
  }
free_output:
  free(*output);
  *output = NULL;
failure_exit:
  return -1;
}

bool path_exists(const char* path) {
  struct stat st;
  int result = stat(path, &st);

  if(result == 0)
    return true;
  else
    return false;
}

int path_actual_type(const char* path, enum PathType* type) {
  struct stat stat_buffer;
  if(stat(path, &stat_buffer) != 0)
    return -1;

  if(stat_buffer.st_mode & S_IFDIR)
    *type = PATH_DIRECTORY;
  else if(stat_buffer.st_mode & S_IFREG)
    *type = PATH_FILE;
  else
    *type = PATH_UNKNOWN;

  return 0;
}

enum PathType path_type(const char* path) {
  if(path[strlen(path) - 1] == '/')
    return PATH_DIRECTORY;

  return PATH_FILE;
}

int path_separate(const char* path, const char*** directories, unsigned int* directories_length, const char** file) {
  if(strstr(path, "/") == NULL) {
    if((*file = strdup(path)) == NULL) // consistency
      return -1;
    else
      return 0;
  }

  enum PathType type = path_type(path);

  unsigned int directories_count = string_count(path, "/");
  /*if(type == PATH_DIRECTORY)*/
  /*  directories_count --;*/

  *directories = (const char**) malloc(directories_count * sizeof(const char*));
  if(*directories == NULL)
    goto exit_failure;

  char* path_copy = strdup(path);
  if(path_copy == NULL)
    goto free_directories;

  unsigned int i = 0;
  const char* current_path = NULL;
  char* path_copy_pointer = path_copy;
  while((current_path = strsep(&path_copy_pointer, "/")) != NULL) {
    if(i == directories_count) {
      if(type == PATH_DIRECTORY)
        break;

      *file = strdup(current_path);
      if(*file == NULL)
        goto free_path_copy;

      break;
    }

    (*directories)[i] = strdup(current_path);
    if((*directories)[i] == NULL)
      goto free_directories_contents;
    i ++;
  }

  free(path_copy);
  path_copy = NULL;
  *directories_length = directories_count;
  return 0;

free_path_copy:
  free(path_copy);
  path_copy = NULL;
free_directories_contents:
  for(unsigned int j = 0; j < i; j ++) {
    free((char*) (*directories)[j]);
    (*directories)[j] = NULL;
  }
free_directories:
  free(*directories);
  *directories = NULL;
exit_failure:
  return -1;
}

int remove_path(const char* path) {
  int result = 0;

  enum PathType type = PATH_UNKNOWN;
  if(path_actual_type(path, &type) != 0)
    return -1;

  switch(type) {
    case PATH_DIRECTORY:
      result = remove_directory(path);
      break;
    case PATH_FILE:
      result = remove(path);
      break;
    case PATH_UNKNOWN:
      result = -1;
      break;
  }

  return result;
}

int remove_directory(const char* path) {
  return nftw(path, remove_directory_nftw_callback, REMOVE_RECURSIVE_MAX_DIRECTORIES, FTW_DEPTH | FTW_PHYS);
}

int remove_directory_nftw_callback(const char* fpath, const struct stat* sb, int typeflag, struct FTW* ftwbuf) {
  return remove(fpath);
}

#define INCLUDE_CONFIG_FOREGROUNDS
#include <config.h>
#include <macros.h>
#include <utils.h>
#include <orchestra.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define GENERATE_DIRENT_FUNCTION(condition) {                                                   \
  DIR* directory_pointer = opendir(path);                                                       \
  if(directory_pointer == NULL)                                                                 \
    goto exit_failure;                                                                          \
                                                                                                \
  struct dirent* dirent_pointer = NULL;                                                         \
                                                                                                \
  unsigned int dirent_names_length = 0;                                                         \
  while((dirent_pointer = readdir(directory_pointer)) != NULL)                                  \
    if(condition)                                                                               \
      dirent_names_length ++;                                                                   \
  closedir(directory_pointer);                                                                  \
  directory_pointer = NULL;                                                                     \
                                                                                                \
  const char** dirent_names = (const char**) malloc(dirent_names_length * sizeof(const char*)); \
  if(dirent_names == NULL)                                                                     \
    goto exit_failure;                                                                          \
                                                                                                \
  directory_pointer = opendir(path);                                                            \
  if(directory_pointer == NULL)                                                                 \
    goto free_dirent_names;                                                                     \
                                                                                                \
  unsigned int i = 0;                                                                           \
  while((dirent_pointer = readdir(directory_pointer)) != NULL)                                  \
    if(condition) {                                                                             \
      dirent_names[i] = strdup(dirent_pointer->d_name);                                         \
      if(dirent_names[i] == NULL)                                                               \
        goto free_dirent_names_contents;                                                        \
      i ++;                                                                                     \
    }                                                                                           \
  closedir(directory_pointer);                                                                  \
  directory_pointer = NULL;                                                                     \
                                                                                                \
  *output = dirent_names;                                                                       \
  *output_length = dirent_names_length;                                                         \
                                                                                                \
  return 0;                                                                                     \
                                                                                                \
free_dirent_names_contents:                                                                     \
  for(unsigned int j = 0; j < i; j ++) {                                                        \
    free((char*) dirent_names[i]);                                                              \
    dirent_names[i] = NULL;                                                                     \
  }                                                                                             \
  closedir(directory_pointer);                                                                  \
  directory_pointer = NULL;                                                                     \
free_dirent_names:                                                                              \
  free(dirent_names);                                                                           \
  dirent_names = NULL;                                                                          \
exit_failure:                                                                                   \
  return -1;                                                                                    \
}

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

int dirents_to_menu_items(struct dirent** dirents, unsigned int dirents_length, struct MenuItem** output) {
  *output = (struct MenuItem*) malloc(dirents_length * sizeof(struct MenuItem));
  if(*output == NULL)
    return -1;

  for(unsigned int i = 0; i < dirents_length; i ++) {
    (*output)[i].contents = dirents[i]->d_name;
    (*output)[i].foreground = foregrounds[dirents[i]->d_type];
    (*output)[i].foreground_reversed = foregrounds_reversed[dirents[i]->d_type];
  }

  return 0;
}

int get_dirents(const char* path, struct dirent*** output, unsigned int* output_length) {
  DIR* directory_pointer = NULL;
  if((directory_pointer = opendir(path)) == NULL)
    goto failure_exit;

  struct dirent* dirent_pointer = NULL;

  unsigned int directory_length = 0;
  while((dirent_pointer = readdir(directory_pointer)) != NULL)
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
  while((dirent_pointer = readdir(directory_pointer)) != NULL) {
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

int get_dirent_names_recursive(const char* path, const char*** output, unsigned int* output_length) {
  const char** directories = (const char**) malloc(sizeof(const char*));
  if(directories == NULL)
    goto exit_failure;
  unsigned int directories_length = 1;

  directories[0] = NULL;
  directories[0] = strdup(path);
  if(directories[0] == NULL)
    goto free_directories;

  unsigned int i = 0;
  while(true) {
    if(i >= directories_length)
      break;

    const char** new_directories = NULL;
    unsigned int new_directories_length = 0;

    if(get_directories(directories[i], &new_directories, &new_directories_length) != 0)
      goto free_directories_contents;

    unsigned int old_length = directories_length;
    unsigned int new_length = directories_length + new_directories_length;

    const char** temp_pointer = (const char**) realloc(directories, new_length * sizeof(const char*));
    if(temp_pointer == NULL)
      goto free_new_directories_contents;
    directories = temp_pointer;

    for(unsigned int j = 0; j < new_directories_length; j ++)
      directories[j + old_length] = new_directories[j];

    free(new_directories);
    new_directories = NULL;

    directories_length = new_length;

    i ++;
    continue;

free_new_directories_contents:
    for(unsigned int j = 0; j < new_directories_length; j ++) {
      free((char*) new_directories[j]);
      new_directories[j] = NULL;
    }
    free(new_directories);
    new_directories = NULL;
    goto free_directories_contents;
  }

  *output = directories;
  *output_length = directories_length;
  return 0;

free_directories_contents:
  for(unsigned int j = 0; j < i; j ++) {
    free((char*) directories[j]);
    directories[j] = NULL;
  }
free_directories:
  free(directories);
  directories = NULL;
exit_failure:
  return -1;
}

int get_directories(const char* path, const char*** output, unsigned int* output_length) 
  GENERATE_DIRENT_FUNCTION(dirent_pointer->d_type == DT_DIR &&
      strcmp(dirent_pointer->d_name, ".") != 0 &&
      strcmp(dirent_pointer->d_name, "..") != 0)

int get_files(const char* path, const char*** output, unsigned int* output_length)
  GENERATE_DIRENT_FUNCTION(dirent_pointer->d_type == DT_REG)

int sentence_separate(const char* sentence, const char*** output, unsigned int* output_length) {
  if(strstr(sentence, " ") == NULL) {
    *output = (const char**) malloc(sizeof(const char*));
    if(*output == NULL)
      goto single_word_exit_failure;

    (*output)[0] = strdup(sentence);
    if((*output)[0] == NULL)
      goto single_word_free_output;

    *output_length = 1;
    return 0;

single_word_free_output:
    free((char**) *output);
    *output = NULL;

single_word_exit_failure:
    return -1;
  }

  unsigned int words_count = string_count(sentence, " ") + 1;
  *output_length = words_count;

  *output = (const char**) malloc(words_count * sizeof(const char*));
  if(*output == NULL)
    goto exit_failure;

  char* sentence_copy = strdup(sentence);
  if(sentence_copy == NULL)
    goto free_output;

  char* sentence_copy_pointer = sentence_copy;

  char* word = NULL;
  unsigned int i = 0;
  while((word = strsep(&sentence_copy_pointer, " ")) != NULL) {
    (*output)[i] = strdup(word);
    if((*output)[i] == NULL)
      goto free_output_contents;
    i ++;
  }

  free(sentence_copy);
  sentence_copy = NULL;

  return 0;

free_output_contents:
  for(unsigned int j = 0; j < i; j ++) {
    free((char*) (*output)[i]);
    (*output)[i] = NULL;
  }
  free(sentence_copy);
  sentence_copy = NULL;
free_output:
  free((char**) *output);
  *output = NULL;
exit_failure:
  return -1;
}

unsigned int string_count(const char* string, const char* query) {
  unsigned int count = 0;

  char* string_pointer = (char*) string;
  while((string_pointer = strstr(string_pointer, query)) != NULL) {
    string_pointer ++;
    count ++;
  }

  return count;
}

bool path_exists(const char* path) {
  struct stat st;
  int result = stat(path, &st);

  if(result == 0)
    return true;
  else
    return false;
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

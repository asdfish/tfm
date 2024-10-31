#define INCLUDE_CONFIG_FOREGROUNDS
#include <config.h>
#include <utils/string.h>
#include <stdlib.h>
#include <string.h>

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

#ifndef STRING_H
#define STRING_H

extern int sentence_separate(const char* sentence, const char*** output, unsigned int* output_length);
extern unsigned int string_count(const char* string, const char* query);

#endif

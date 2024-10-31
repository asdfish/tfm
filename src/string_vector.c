#define INCLUDE_VECTOR_GENERATE_SOURCE_MACRO
#include <string_vector.h>

VECTOR_GENERATE_SOURCE(const char*,
    int,
    string_vector,
    0, -1,
    string_vector_at,
    string_vector_init,
    string_vector_length,
    string_vector_pop,
    string_vector_push,
    string_vector_realloc_contents,
    string_vector_reserve,
    string_vector_uninit)

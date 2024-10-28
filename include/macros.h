#ifndef MACROS_H
#define MACROS_H

#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

#define NUMBER_STRING_LENGTH(number) ((number / 10) + 1)

#endif

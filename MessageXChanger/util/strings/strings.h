#ifndef MESSAGEXCHANGER_STRINGS_H
#define MESSAGEXCHANGER_STRINGS_H

#include "string.h"
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"

#define SMALL_SIZE 128
#define MEDIUM_SIZE 256
#define LARGE_SIZE 512
#define LARGEST_SIZE 1024
#define true 1
#define false 0

#define true 1
#define false 0

extern char * append(char * first, char * second);
extern int get_input(char * expected, char * result);
extern int starts_with_ignore_case(char * string, char * string2);
extern char * trim_string(char * s);

extern int is_numeric(const char * string, size_t size);

#endif //MESSAGEXCHANGER_STRINGS_H

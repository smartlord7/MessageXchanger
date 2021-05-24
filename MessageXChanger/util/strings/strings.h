//
// Created by joao on 24/05/21.
//

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

extern char * append(char * first, char * second);

extern int get_input(char * expected, char * result);

#endif //MESSAGEXCHANGER_STRINGS_H

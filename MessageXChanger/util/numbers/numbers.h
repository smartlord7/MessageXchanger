//
// Created by joao on 24/05/21.
//

#ifndef MESSAGEXCHANGER_NUMBERS_H
#define MESSAGEXCHANGER_NUMBERS_H

#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>

#define FLOAT_SIZE_EXCEEDED (-1)
#define FLOAT_CONVERSION_FAILURE 0
#define FLOAT_CONVERSION_SUCCESS 1

int to_float(char * num_string, float * num_float);

#endif //MESSAGEXCHANGER_NUMBERS_H

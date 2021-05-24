//
// Created by joao on 24/05/21.
//

#include "numbers.h"

int to_float(char * num_string, float * num_float){
    assert(num_string != NULL && num_float != NULL);

    double num_d;
    char * tail = NULL;
    errno = 0;

    num_d = strtod(num_string, &tail);

    if (errno) {
        return FLOAT_CONVERSION_FAILURE;
    }

    if ((num_d <= FLT_MAX && num_d >= FLT_MIN) || num_d < 0) {
        * num_float = (float) num_d;
    } else if (num_d == 0.0){
        return FLOAT_CONVERSION_FAILURE;
    } else {
        return FLOAT_SIZE_EXCEEDED;
    }

    return FLOAT_CONVERSION_SUCCESS;
}
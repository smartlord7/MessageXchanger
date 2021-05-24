//
// Created by joao on 24/05/21.
//

#include "strings.h"

char * append(char * first, char * second) {
    assert(first != NULL && second != NULL);

    if (strcat(first, second) == NULL) {
        return NULL;
    }

    return first;
}

int get_input(char * expected, char * result) {
    assert(expected != NULL);

    char buffer[SMALL_SIZE];

    printf("%s: ", expected);
    if(fgets(result, SMALL_SIZE, stdin) == NULL) {

        snprintf(buffer, SMALL_SIZE, "Couldn't get %s\n", expected);
        perror(buffer);

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
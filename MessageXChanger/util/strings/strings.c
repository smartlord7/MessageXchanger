#include "string.h"
#include "stddef.h"
#include "ctype.h"
#include "assert.h"
#include "strings.h"

int starts_with_ignore_case(char * string, char * string2) {
    assert(string != NULL && string2 != NULL);

    char * i, * j;

    i = string;
    j = string2;

    while ((* j) != '\0') {
        if (toupper((int) * i++) != toupper((int) (* j++))) {
            return false;
        }
    }

    return true;
}

char * trim_string(char * s){
    assert(s != NULL);

    int i;

    while (isspace (*s) || (* s) == '\n') s++;   // skip left side white spaces
    for (i = (int) strlen(s) - 1; (isspace (s[i] || s[i] == '\n')); i--) ;   // skip right side white spaces
    s[i + 1] = '\0';

    return s;
}

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
int is_numeric(const char * string, size_t size) {
    assert(string != NULL && size > 0);

    for(int i = 0; i < (int) size; i++) {
        if(!isdigit(string[i])) {
            return false;
        }
    }

    return true;
}
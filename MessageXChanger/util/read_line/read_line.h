/** Project MessageXChanger - LEI, University of Coimbra, 2nd year, 2nd semester - Communication Networks
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 17/05/2021
*/

#ifndef MESSAGEXCHANGER_READ_LINE_H
#define MESSAGEXCHANGER_READ_LINE_H

// region dependencies

#include "stdio.h"

// endregion dependencies

// region constants

#define BUFFER_SIZE_EXCEEDED (-2)
#define LINE_READ_SUCCESSFULLY 1

// endregion constants

// public functions prototypes

/**
 * @def read_line
 * @brief Function that copies a line of a certain file to a specified buffer.
 *
 * @param buffer
 * The buffer.
 *
 * @param file
 * The file from which a line will be read.
 *
 * @param buffer_size
 * The size of the buffer aka the max size of the line.
 *
 * @return -2 if the buffer size is exceeded.
 *         -1 (EOF) if the end of file is reached.
 *         1 if the line is read successfully.
 */
int read_line(char * buffer, FILE * file, int buffer_size);

// public functions prototypes

#endif //MESSAGEXCHANGER_READ_LINE_H
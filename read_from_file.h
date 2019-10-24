#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include "types.h"
#define MAX_SIZE 1024

/*
    Reads a line from a given file ignoring 
    lines starting with # and blank lines
*/
Result read_line(FILE *f, char *line);

#endif
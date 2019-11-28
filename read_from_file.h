#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include "types.h"
#include "map.h"
#include "print_on_screen.h"

#define MAX_SIZE 1024

/*
    Reads a line from a given file ignoring 
    lines starting with # and blank lines
*/
Result read_line(FILE *f, char *line);

Box** map_char_to_box(char** map, Screen *screen);

char** map_from_file(char *file, Screen *screen);

#endif
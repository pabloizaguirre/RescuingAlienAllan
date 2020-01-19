#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include "types.h"
#include "map.h"
#include "print_on_screen.h"

/*
    Inputs:
        - *f: pointer to file to be read
        - *line: pointer to character where the read line
        should be stored
    Description:
        Reads a line ignoring empty lines and lines starting
        with #
*/
Result read_line(FILE *f, char *line);

/*
    Inputs:
        - **map: pointer double array of character to be converted
        - *screen: pointer to the main screen
    Outputs:
        Map *: pointer to the initialized map
        NULL in case of error
    Description:
        initializes a map from the given array of characters
        initializes de boxes, boxes_design and boxes_merge arrays
        boxes and boxes_merge are initialized to the contents in the file
        and boxes_design is initialized to AIR
        
        The char to box conversion goes as follows:
        .   AIR
        W   WALL
        S   START
        E   END
        L   LAVA
        A   PORTALA
        B   PORTALB
        #   LADDER
        Z   ZONAPORTAL
        w   WALL_MERGE
        D   DISAPPEAR_WALL
        l   LADDER_MERGE
*/
Map* map_char_to_box(char** map, Screen *screen);

/*
    Inputs:
        - *file: name of the file to open
        - *screen: pointer to the main screen
    Outputs:
        Char **: contents read from the file
        NULL in case of error
    Description:
        Reads a file initializing a double array of characters
        with the contents of the file
*/
char** map_from_file(char *file, Screen *screen);

#endif

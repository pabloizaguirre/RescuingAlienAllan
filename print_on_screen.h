#ifndef print_on_screen_h
#define print_on_screen_h

#include "types.h"

/*
    Changes the color you are printing with to the color given by the string argument
*/
int change_color(char *color);

/*
    Changes the custor to the given position
*/
int change_cursor(Position position);

/*
    Prints the given string in the given position
*/
int print_on_position(Position position, char *text);

#endif
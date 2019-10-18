#ifndef print_on_screen_h
#define print_on_screen_h

#include "types.h"
#include <stdio.h>

#define SCREEN_HEIGHT 30
#define SCREEN_WIDTH 130

/*
    Initialize screen
*/
Result init_screen();

/*
    Clears screen
*/
Result clear_screen();

/*
    Prints the magins between the map and the message boxes from a given file
*/
Result print_margins(char *file_name);

/*
    Changes the color you are printing with to the color given by the string argument
*/
int change_color(char *foreground_color, char *background_color);

/*
    Changes the custor to the given position
*/
int change_cursor(Position position);

/*
    Prints the given string in the given position
*/
int print_on_position(Position position, char *text);

#endif

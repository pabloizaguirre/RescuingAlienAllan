#ifndef map_h
#define map_h

#include "types.h"
/* #include "people.h" */
#include <stdio.h>
#include "print_on_screen.h"

//Celdas
/* This enumeration defines the type Box, which represents an element in the map */
typedef enum {AIR, WALL, START, END, LAVA, PORTALA, PORTALB, LADDER, ZONAPORTAL} Box;

/* The structure Map includes an array of arrays of elements of type Box 
Las posiciones son de tipo mapa, no de tipo */
typedef struct {
    Box **boxes;
    Box **boxes_design;
    Box **boxes_merge;
    Position *Start_pos;
    Position *End_pos;
    Position *PORTALB_pos;
} Map;

/* Creates a map of type Map out of a text file */
Map *create_map(char *file, Screen *screen);

/* Deallocates a map */
void free_map(Map *map);

/* It recieves as an argument a position of the screen and returns a map position 
If the position is not in the map then the function returns a position which has a -1 in pos.x 
Esta función está hecha para que se use así con el mapa: 
map->boxes[map_position().x][map_position().y]*/
Position map_position(Position position, Screen *screen);

/* It recieves as an argument a map position and returns a screen position
if the function is not in the map then the function returns a position which has a -1 in pos.x */
Position screen_position(Position position, Screen *screen);

/* Checks if a position can be occupied by a person */
Bool is_position_occupable(Position position, Level *level, Screen *screen);

/* Checks if you can place a resource on a given position */
Bool is_position_valid_resources(Position position, Level *level, Screen *screen);

Result map_merge(Screen *screen, Map *map);

Position* map_get_start_position(Map *map);

Position* map_get_end_position(Map *map);

/* Takes a map and returns the position where the box of type PORTALB is located */
Position* map_get_portalb_position(Map *map);

#endif

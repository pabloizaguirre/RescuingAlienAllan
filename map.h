#ifndef map_h
#define map_h

#include "types.h"
/* #include "people.h" */
#include <stdio.h>
#include "print_on_screen.h"

//Celdas
/* This enumeration defines the type Box, which represents an element in the map */
typedef enum {AIR, WALL, START, END, LAVA, PORTALA, PORTALB, LADDER, ZONAPORTAL} Box;

/* The structure Map includes an array of arrays of elements of type Box */
typedef struct {
    Box **boxes;
    Position *Start_pos;
    Position *End_pos;
    Position *PORTALB_pos;
} Map;

typedef struct {
    Box *center;
    Box *top;
    Box *bottom;
    Box *right;
    Box *left;
} Surroundings;

/* Creates a map of type Map out of a text file */
Map *create_map(char *file, Screen *screen);

/* Deallocates a map */
void *free_map(Map *map);

/* Checks if a position can be occupied by a person */
Bool is_position_occupable(Position position, Level *level, Screen *screen);
/* Checks if you can place a resource on a given position */
Bool is_position_valid_resources(Position position, Screen *screen, Map *map);

/* Takes as arguments a position in the map and a map and returns an element
of type Surrounding that includes the boxes that surround the position and the box
in the given position */
Surroundings map_get_position_surroundings(Position position, Map *map);

Position* map_get_start_position(Map *map);

Position* map_get_end_position(Map *map);

/* Takes a map and returns the position where the box of type PORTALB is located */
Position* map_get_portalb_position(Map *map);

#endif

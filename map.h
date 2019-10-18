#ifndef map_h
#define map_h

#include "types.h"

/* This enumeration defines the type Box, which represents an element in the map */
typedef enum {AIR, WALL, START, END, LAVA, TPA, TPB, LADDER} Box;

/* The structure Map includes an array of arrays of elements of type Box */
typedef struct {
    Box **boxes;
} Map;

typedef struct {
    Box center;
    Box top;
    Box bottom;
    Box right;
    Box left;
} Surroundings;

/* Creates a map of type Map out of a text file */
Map *create_map(FILE *f);

/* Deallocates a map */
void *free_map(Map *map);

/* Displays the map on the screen */
void *display_map(Map *map);

/* Takes as arguments a position in the map and a map and returns an element
of type Surrounding that includes the boxes that surround the position and the box
in the given position */
Box *map_get_position_surroundings(Position position, Map *map);

/* Takes a map and returns the position where the box of type TPB is located */
Position map_get_tpb_position(Map *map);

#endif

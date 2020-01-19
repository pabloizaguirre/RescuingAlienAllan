#ifndef map_h
#define map_h

#include "types.h"
#include <stdio.h>
#include "print_on_screen.h"

/* This enumeration defines the type Box, which represents an element in the map */
typedef enum {AIR, WALL, START, END, LAVA, PORTALA, PORTALB, LADDER, ZONAPORTAL, WALL_MERGE, DISAPPEAR_WALL} Box;

typedef struct {
    Box **boxes;
    Box **boxes_design;
    Box **boxes_merge;
    Position *Start_pos;
    Position *End_pos;
    Position *PORTALB_pos;
} Map;

/*
    Inputs:
        - *map_file: char pointer to the name of the file
        - *screen: pointer to the main screen
    Outputs:
        Map *: pointer to the initialized map
        NULL in case of error
    Description:
        Allocates memeory for all the pointers in the map
        Creates a Box** boxes to store the original map
                a Box** boxes_design to be changed in the design loop
                a Box** boxes_merge where the user placed resources and
                            the original map objects should be merged in
                            order to print it
        Initializes the values of the boxes and boxes_merge arrays to 
        the map found in the file and the boxes_design is initialized
        to AIR
        When the start and end doors and the portal b are found the
        pointers start_pos, end_pos, portalb_pos are initialized
        NOTE:
            This properties are set in the map_char_to_box function
*/
Map *create_map(char *file, Screen *screen);

/*
    Inputs:
        - position: position to be processed
        - *screen: pointer to main screen
    Outputs:
        Position: position now relative to the map coordinates
    Description:
        Takes in a position relative to the screen coordinates
        and returns a position relative to the map coordinates
        
*/
Position map_position(Position position, Screen *screen);

/*
    Inputs:
        - position: position to be processed
        - *screen: pointer to main screen
    Outputs:
        Position: position now relative to the screen coordinates
    Description:
        Takes in a position relative to the map coordinates
        and returns a position relative to the screen coordinates        
*/
Position screen_position(Position position, Screen *screen);

/*
    Inputs:
        - position: position to be processed
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        Bool: whether or not the given position is occupable by a person
    Description:
        Takes in a position relative to the screen coordinates
        sees if box at that position in the boxes_merge isn't a
        WALL / WALL_MERGE / DISAPPEAR_WALL
        and if it is occupied for any other person
        NOTE:
            Stairs are not considered to be an obstacle because they can
            be occupied from a side but not from above     
            Finished and desintegrated people are not taken into account  
*/
Bool is_position_occupable(Position position, Level *level, Screen *screen);

/*
    Inputs:
        - position: position to be processed
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        Bool: whether or not the given position is occupable by a resource
    Description:
        Takes in a position relative to the screen coordinates
        sees if box at that position in the boxes array is a
        ZONAPORTAL / AIR
        everything else is considered invalid for resources
*/
Bool is_position_valid_resources(Position position, Level *level, Screen *screen);

/*
    Inputs:
        - *map: pointer to map to be merged
        - *screen: pointer to main screen
    Outputs:
        Result: to notify if an error ocurred
    Description:
        Goes through the boxes and boxes_design arrays saving at
        boxes_merge a copy of the boxes array where the AIR and
        ZONAPORTAL boxes are replaces with the contents of the
        boxes_design array
*/
Result map_merge(Screen *screen, Map *map);

/*
    Inputs:
        - *map: pointer to map to be freed
        - *screen: pointer to main screen
    Description:
        Frees all the memory allocated for the given map
        Check if pointers are set to NULL before calling free()
*/
void free_map(Map *map, Screen * screen);

#endif

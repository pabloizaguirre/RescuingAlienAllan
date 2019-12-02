
#ifndef level_h
#define level_h

#include "map.h"
#include "people.h"

#define NUM_LEVELS 4

struct _Level {
    Map *map;
    Level *next_level;
    int level_number;
    char message[256];
    int num_people;
    People **people;
    int num_ladder_floor;
    int num_ladder;
    int num_floor;
    int portal; 
};

/*
    Initialices a double linked list of levels from a name of a file containing
    the names of all the files of all the levels
    
    Levels_init() searches for a progress.txt file, and if it exists, it initializes
    the game from the last level you have played.
*/
Level *levels_init(Screen *screen);

/*
    Prints the level mesage in the screen
*/
int level_print_mesaje(Level *level);

/*
    Get functions for the level structure
*/
Map *level_get_map(Level *level);
Level *level_get_next_level(Level *level);
Level *level_get_last_level(Level *level);
People **level_get_peoples(Level *level);
People *level_get_alien(Level *level);

/*
    Set functions for the level structure
*/
Level *level_set_map(Level *level, Map *map);
Level *level_set_next_level(Level *level, Level *next_level);
Level *level_set_last_level(Level *level, Level *last_level);
Level *level_set_peoples(Level *level, People **peoples);
Level *level_set_alien(Level *level, People *alien);

void level_destroy(Level* levels);

#endif

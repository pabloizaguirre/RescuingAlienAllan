
#ifndef level_h
#define level_h

#include "map.h"
#include "people.h"

#define NUM_LEVELS 4

struct _Level {
    Map *map;
    Level *first_level;
    Level *next_level;
    int level_number;
    char message[256];
    int num_people;
    People **people;
    int n_stars;
    
    // Currently unused resources
    int num_ladder_floor_act;
    int num_ladder_act;
    int num_floor_act;
    int portal_act;
    
    // minimum spare resources
    int num_ladder_floor;
    int num_ladder;
    int num_floor;
    int portal; 
    int min_people;
    
    
    // maximum spare resources for 2 stars
    int num_ladder_floor_2;
    int num_ladder_2;
    int num_floor_2;
    int portal_2; 
    int min_people_2;

    // maximum spare resources for 3 stars
    int num_ladder_floor_3;
    int num_ladder_3;
    int num_floor_3;
    int portal_3; 
    int min_people_3;

    
};

/*
    Initialices a double linked list of levels from a name of a file containing
    the names of all the files of all the levels
    
    Levels_init() searches for a progress.dat file, and if it exists, it initializes
    the game from the last level you have played.
*/
Level *levels_init(Screen *screen);

/*
    Prints the level mesage in the screen
*/
int level_print_mesaje(Level *level);

/* 
    Returns the state of the game (LOST, STARS_1, STARS_2, STARS_3, SUPREME) and RES_ERROR if there
    was an error also changes the number of stars of the level
 */
Level_result game_status(Level *level);


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

void free_level(Level* first_level, Screen * screen);

#endif

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
    Input:
        - *screen: pointer to main screen
    Outputs:
        Level *: pointer to the current level
        levels act as a foward linked list
        each level has a pointer to the first level just in case 
        the current level is not the first one
    Description:
        Goes through all the files named level**.txt inside the level_files folder
        for each level initializes a level structure setting all it's values appropiately
        and links each level to the next one creating a linked list structure
        NOTES:
            - The map inside each level is created from a file wich name shold be especified in
            the corresponding level**.txt file
            - The n_starts property is set from the progress.dat file wich is created and initialized
            to 0 if it didn't exist
            - The level number property acts as an index but it starts at 1 so that it can be printed
            in a user friendly way
            - A double pointer to the persons for the level is also created and initialized inside
            this function
*/
Level *levels_init(Screen *screen);

/*
    Input:
        - *level: pointer to the current level
    Outputs:
        Flag indicating if the player has lost or the number of starts won
    Description:
        Checks how many resources the player has spent and how many people
        have reached the end. Based on that determines if the player has
        lost and the number of stars won if he didn't lose
        Also saves the won stars in the progress.dat file
*/
Level_result game_status(Level *level);

/*
    Inputs:
        - *level: pointer to the current level
        - *screen: pointer to the main screen
        - *first_level: pointer to the first_level
    Outputs:
        Level *: pointer to the level chosen by the player
        NULL it the player exits
    Description:
        Let's the player repeat the level, open the choose level menu
        or go to the next level if he has won at least 1 star
        returns a pointer to the level chosen by the player of NULL
        it the player exits the game
*/
Level *level_menu(Level *level, Screen *screen, Level *first_level);

/*
    Inputs:
        - *first_level: pointer to the first_level
        - *screen: pointer to the main screen
    Description:
        Frees the level structure
*/
void free_level(Level* first_level, Screen * screen);

#endif

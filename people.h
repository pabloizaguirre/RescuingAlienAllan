
#ifndef PEOPLE_H
#define PEOPLE_H
#include "types.h"
#include "map.h"
#include <stdio.h>


typedef struct {
    char character;
    Position position;
    State state;
    int wait;
} People;

/*
    Inputs:
        - character: character to display
        - position: original position for the person
        - state: original state
        - wait: delay before it starts moving
    Outputs:
        - People *: pointer to the initialized person
    Description:
        Creates a People pointer, allocates memory and sets its properties to 
        the given values
*/
People* create_people(char character, Position position, State state, int wait);

/*
    Inputs:
        - *p: person to be updated
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        Returns -1 if something went wrong, 0 if it didn't move and 1 if it did
    Description:
        Checks the state of the person and the contents of the map arround him
        based on that moves him and updates its state
        Finally returns the appropiate number
*/
int people_update(People *p, Level *level, Screen *screen);

/*
    Inputs:
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        Reslut: indiacting if an error ocurred
    Description:
        Prints all the people using their character
        The last person is printed in green to represent the alien
*/
Result print_people(Level *level, Screen *screen);

/*
    Inputs:
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        FLAG: indicating whether the level has finished
    Description:
        Performs each iteration of the people movement loop
        For each person it calls people update to move them and 
        determine their state
        This function gets a delay based on the SPEED macro defined
        in types.h
        If no person moved then the game finishes
*/
FLAG movement_loop(Level *level, Screen * screen);

/*
    Inputs:
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        Result: indicating whether an error ocurred
    Description:
        Resets each person to its default values so 
        that the level can be restarted
*/
Result reset_people(Level *level, Screen * screen);

/*
    Inputs:
        - *level: pointer to current level
    Outputs:
        int: indicating whether an error ocurred
    Description:
        Frees all memory allocated for the people
        Checks if pointers are set to NULL before calling free()
*/
int free_people(Level *level);

#endif


#ifndef PEOPLE_H
#define PEOPLE_H
#include "types.h"
#include <stdio.h>

typedef struct{}

typedef struct {
    char character;
    Position position;
    State state;
}
  } People;

People* create_people(char character, Position position, State state);

/* This functions receive a pointer to people and */
/* return a certain atribute of that character.   */

Position people_get_position(People *p);
State people_get_state(People *p);

/* This function receives a pointer to people and */
/* returns an array of 5 boxes which correspond   */
/* to the boxes center, up, don, right and left  */
/* of that person                                 */ 
Surroundings* People_get_surroundings(People *p);

int People_update()









#endif 

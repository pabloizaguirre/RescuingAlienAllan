
#ifndef PEOPLE_H
#define PEOPLE_H
#include "types.h"
#include <stdio.h>
#include "map.h"

typedef struct {
    char character;
    Position position;
    State state;
} People;

/*Allocates memory for a person*/

People* create_people(char character, Position position, State state);

/* This functions receive a pointer to people and */
/* return a certain atribute of that character.   */

Position People_get_position(People *p);
State People_get_state(People *p);
Surroundings* People_get_surroundings(People *p);

/* Return a certain atribute of a person*/

int People_set_character(People *p, char c);
int People_set_position(People *p, Position position);
int People_set_state(People *p, State state);

/*Updates the position of a person depending on the map. Returns 1 if
it moves, 0 if it doesnt't, -1 if error.*/

int People_update(People *p, Map *map);

/*Frees the memory allocated for a person*/

int free_people(People *p);



#endif

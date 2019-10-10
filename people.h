#ifndef PEOPLE_H
#define PEOPLE_H

typedef enum {DEAD, ALIVE} State;

typedef struct {
    char character;
    int position_x;
    int position_y;
    State state;
    int id;
} People;

int people_get_x(People *p);
int people_get_y(People *p);
State people_get_state(People *p);


#endif PEOPLE_H

#include "people.h"
#include "map.h"
#include "level.h"
#include <stdlib.h>

// Returns the state of the person when it reaches the b Box
// Done for AIR, WALL, START, END, LAVA, PORTALA, PORTALB, LADDER
State change_state(Box b) {
    if (b == LAVA || b == WALL) {
        return DESINTEGRATED;
    } else if (b == END) {
        return FINISHED;
    } else {
        return ALIVE;
    }
}

People* create_people(char character, Position position, State state){
    People *p;
    p = (People*)malloc(sizeof(People));
    if (!p){
        printf("Error when allocating memory in create_people()\n");
        return NULL;
    }
    p->character = character;
    p->position = position;
    p->state = state;
    return p;
}

Position People_get_position(People *p){
    return p->position;
}

State People_get_state(People *p){
    return p->state;
}

int People_set_character(People *p, char c){
    p->character = c;
    return 1;
}
int People_set_position(People *p, Position position){
    p->position = position;
    return 1;
}
int People_set_state(People *p, State state){
    p->state = state;
    return 1;
}


// Returns -1 if something went wrong, 0 if it doesn't move and 1 if it succeeded
int people_update(People *p, Level *level, Screen *screen){
    Surroundings surr = map_get_position_surroundings(p->position, level->map);
    Position pos_aux = p->position;

    if (p->state != ALIVE) {
        return -1;
    }

    /* To do list:
        -
     */

    // First check if the person is in a special box (i.e: ladder, portal ...)
    switch (*(surr.center)){
        case LADDER:
            pos_aux.y++;
            if (is_position_occupable(pos_aux, level, screen)){
                // Moves up
                p->position.y--;
                p->state = change_state(level->map->boxes[p->position.x][p->position.y]);
                print_message(screen, "ladder");
                return 1;
            }
            return 0;
            break;
        case PORTALA:
            if(level->map->PORTALB_pos){
                pos_aux = *(level->map->PORTALB_pos);
                if (is_position_occupable(pos_aux, level, screen)){
                    // Moves to portal B
                    p->position = pos_aux;
                    p->state = change_state(level->map->boxes[p->position.x][p->position.y]);
                    print_message(screen, "portala");
                    return 1;
                }
            }
            return 0;
            break;
    }

    // Check if the person can move down
    pos_aux = p->position;
    pos_aux.y++;
    if (is_position_occupable(pos_aux, level, screen)){
        // Moves down
        p->position = pos_aux;
        p->state = change_state(level->map->boxes[p->position.x][p->position.y]);
        print_message(screen, "moves down");
        return 1;
    } else { // If the person cannot move down then it will move right if it is possible
        pos_aux = p->position;
        pos_aux.x++;
        if (is_position_occupable(pos_aux, level, screen)) {
            // Moves right
            p->position = pos_aux;
            p->state = change_state(level->map->boxes[p->position.x][p->position.y]);
            print_message(screen, "moves right");
            return 1;
        }
    }

    // The person hasn't moved
    return 0;
}

//state
Result print_people(Level *level, Screen *screen){
    int i;
    if (!level || !screen){
        printf("Error in print_people(): !level or !screen");
        fflush(stdout);
        return ERROR;
    }

    for(i = 0; i < level->num_people - 1; i++){
        if(level->people[i]->state != DESINTEGRATED && level->people[i]->state != FINISHED){
            change_cursor(level->people[i]->position, screen);
            change_color("reset", "reset");
            printf("%c", level->people[i]->character);
        }
    }
    if(level->people[i]->state != DESINTEGRATED && level->people[i]->state != FINISHED){
        change_cursor(level->people[i]->position, screen);
        change_color("reset", "green");
        printf("%c", level->people[i]->character);
    }
}

//problematico creo que esta mal
int free_people(Level *level){
    int i;
    for (i = 0; i < level->num_people; i++){
        if (level->people[i] != NULL){
            free(level->people[i]);
        }
    }
    free(level->people);
    return 1;
}
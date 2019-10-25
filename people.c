#include "people.h"
#include "map.h"

// Returns the state of the person when it reaches the b Box
// Done for AIR, WALL, START, END, LAVA, PORTALA, PORTALB, LADDER
State Change_state(People *p, Box b) {
    if (b == LAVA || b == WALL) {
        return DESINTEGRATED;
    } else if (b == END) {
        return FINISHED;
    } else {
        return ALIVE
    }
}

// Returns -1 if something went wrong and 1 if it succeded
int People_update(People *p, People **people, Map *map){
    Surroundings surr = map_get_position_surroundings(p->position, map);
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
            if (is_position_occupable(pos_aux, people, map)){
                // Moves up
                p->position.y++;
                p->state = Change_state(p, map->boxes[p->position.x][p->position.y]);
                return 1;
            }
            return 0;
            break;
        case PORTALA:
            if(map->PORTALB_pos){
                pos_aux = *(map->PORTALB_pos);
                if (is_position_occupable(pos_aux, people, map)){
                    // Moves to portal B
                    p->position = pos_aux;
                    p->state = Change_state(p, map->boxes[p->position.x][p->position.y]);
                    return 1;
                }
            }
            return 0;
            break;
    }

    // Check if the person can move down
    pos_aux = p->position;
    pos_aux.y--;
    if (is_position_occupable(pos_aux, people, map)){
        // Moves down
        p->position = pos_aux;
        p->state = Change_state(p, map->boxes[p->position.x][p->position.y]);
        return 1;
    } else { // If the person cannot move down then it will move right if it is possible
        pos_aux = p->position;
        pos_aux.x++;
        if (is_position_occupable(pos_aux, people, map)) {
            // Moves right
            p->position = pos_aux;
            p->state = Change_state(p, map->boxes[p->position.x][p->position.y]);
            return 1;
        }
    }

    // The person hasn't moved
    return 0;
}
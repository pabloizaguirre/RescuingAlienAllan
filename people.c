#include "people.h"
#include "map.h"
#include "level.h"
#include <stdlib.h>

//celdas
// Returns the state of the person when it reaches the b Box
// Done for AIR, WALL, START, END, LAVA, PORTALA, PORTALB, LADDER, WALL_MERGE, PORTAL_ZONE
State change_state(Box b) {
    if (b == LAVA || b == WALL || b == WALL_MERGE) {
        return DESINTEGRATED;
    } else if (b == END) {
        return FINISHED;
    } else {
        return ALIVE;
    }
}

People* create_people(char character, Position position, State state, int wait){
    People *p;
    p = (People*)malloc(sizeof(People));
    if (!p){
        printf("Error when allocating memory in create_people()\n");
        return NULL;
    }
    p->character = character;
    p->position = position;
    p->state = state;
    p->wait = wait;
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
    Position pos_aux = p->position;
    Position map_pos = map_position(pos_aux, screen);
    Box b;
    
    if (map_pos.x < 0) {
        return -1;
    }

    if (p->state != ALIVE && p->wait < 0) {
        return 0;
    }

    if (p->wait > 0){
        p->wait--;
        return 0;
    }

    if (p->wait == 0){
        p->wait--;
        People_set_state(p, ALIVE);
        return 0;
    }

    b = level->map->boxes_merge[map_pos.x][map_pos.y];

    // First check if the person is in a special box (i.e: ladder, portal ...)
    switch (b){
        case LADDER:
            pos_aux.y--;
            if (is_position_occupable(pos_aux, level, screen)){
                // Moves up
                p->position.y--;
                map_pos = map_position(p->position, screen);
                p->state = change_state(level->map->boxes_merge[map_pos.x][map_pos.y]);
                return 1;
            }
            return 0;
            break;
        case PORTALA:
            if(level->map->PORTALB_pos != NULL){
                map_pos = *(level->map->PORTALB_pos);
                pos_aux = screen_position(map_pos, screen);
                if (is_position_occupable(pos_aux, level, screen)){
                    // Moves to portal B
                    p->position = pos_aux;
                    p->state = change_state(level->map->boxes_merge[map_pos.x][map_pos.y]);
                    return 1;
                }
            }
            break;
        default:
            break;
    }

    // Check if the person can move down
    pos_aux = p->position;
    pos_aux.y++;
    map_pos = map_position(pos_aux, screen);

    if(map_pos.x != -1){
        b = level->map->boxes_merge[map_pos.x][map_pos.y];
    } else b = AIR;//This is just so it works when the cell below is the end of the map
    
    //The person cannot move down if there is a ladder underneath
    if (is_position_occupable(pos_aux, level, screen) && b != LADDER){
        // Moves down
        p->position = pos_aux;
        map_pos = map_position(p->position, screen);
        p->state = change_state(level->map->boxes_merge[map_pos.x][map_pos.y]);
        return 1;
    } else { // If the person cannot move down then it will move right if it is possible
        if(level->map->boxes_merge[map_pos.x][map_pos.y] == DISAPPEAR_WALL){
            level->map->boxes_merge[map_pos.x][map_pos.y] = AIR;
        }
        pos_aux = p->position;
        pos_aux.x++;
        if (is_position_occupable(pos_aux, level, screen)) {
            // Moves right
            p->position = pos_aux;
            map_pos = map_position(p->position, screen);
            p->state = change_state(level->map->boxes_merge[map_pos.x][map_pos.y]);
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
    return OK;
}


FLAG movement_loop(Level *level, Screen * screen){
    int k, i;
    FLAG flag = LEVEL_FINISHED;
    printf("\e[?25l");
    usleep(100*1000);

    print_map(level->map->boxes_merge, screen);
    print_resources(screen, level);
    print_people(level, screen);
    printf("\e[?25l");
    fflush(stdout);
    
    for (i = 0; i < level->num_people; i++){
        k = people_update(level->people[i], level, screen);
        printf("\e[?25l");
        fflush(stdout);
        if (k < 0){
            printf("ERROR EN PEOPLE_UPDATE()\n");
            fflush(stdout);
            return GAME_FINISHED;
        }
        if (k > 0){
            flag = LEVEL_NOT_FINISHED;
        }
    }
    if (flag == LEVEL_FINISHED){
        return LEVEL_FINISHED;
    }
    return LEVEL_NOT_FINISHED;
}

Result reset_people(Level *level, Screen * screen){
    Position spos = screen_position(*(level->map->Start_pos), screen);
        
    for (int k = 0; k < level->num_people; k++){
        level->people[k]->position = spos;
        level->people[k]->state = DESINTEGRATED;
        level->people[k]->wait = 3*k;                
    }
}


int free_people(Level *level){
    int i;
    if(level->people){
        for (i = 0; i < level->num_people; i++){
            if (level->people[i] != NULL){
                free(level->people[i]);
            }
        }
        free(level->people);
    }
    return 1;
}

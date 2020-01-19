#include "people.h"
#include "map.h"
#include "level.h"
#include <stdlib.h>

/* PRIVATE FUNCTION
    Inputs:
        - b: box to be processed
    Outputs:
        - State: resulting state
    Description:
        Returns the state a person should have after being occupying
        the box b
        If b is LAVA / WALL / WALL_MERGE the person gets DESINTEGRATED
        If b is END the person gets FINISHED
        Otherwise gets ALIVE
*/
State change_state(Box b) {
    if (b == LAVA || b == WALL || b == WALL_MERGE) {
        return DESINTEGRATED;
    } else if (b == END) {
        return FINISHED;
    } else {
        return ALIVE;
    }
}

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
int people_update(People *p, Level *level, Screen *screen){
    Position pos_aux = p->position;
    Position map_pos = map_position(pos_aux, screen);
    Box b;
    
    if (map_pos.x < 0) {
        return -1;
    }

    // If the person is dead or it still has to wait
    if (p->state != ALIVE && p->wait < 0) {
        return 0;
    }

    if (p->wait > 0){
        p->wait--;
        return 0;
    }

    if (p->wait == 0){
        p->wait--;
        p->state = ALIVE;
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
        case LADDER_MERGE:
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
    if (is_position_occupable(pos_aux, level, screen) && b != LADDER && b != LADDER_MERGE){
        // Moves down
        p->position = pos_aux;
        map_pos = map_position(p->position, screen);
        p->state = change_state(level->map->boxes_merge[map_pos.x][map_pos.y]);
        return 1;
    } else { // If the person cannot move down then it will move right if it is possible
        if(b == DISAPPEAR_WALL){
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
Result print_people(Level *level, Screen *screen){
    int i;
    if (!level || !screen){
        printf("Error in print_people(): !level or !screen");
        fflush(stdout);
        return ERROR;
    }

    // print all the people except the last one
    for(i = 0; i < level->num_people - 1; i++){
        if(level->people[i]->state != DESINTEGRATED && level->people[i]->state != FINISHED){
            change_cursor(level->people[i]->position, screen);
            change_color("reset", "reset");
            printf("%c", level->people[i]->character);
        }
    }

    // print the alien
    if(level->people[i]->state != DESINTEGRATED && level->people[i]->state != FINISHED){
        change_cursor(level->people[i]->position, screen);
        change_color("reset", "green");
        printf("%c", level->people[i]->character);
    }
    
    return OK;
}

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
FLAG movement_loop(Level *level, Screen * screen){
    int k, i;
    FLAG flag = LEVEL_FINISHED;
    disable_terminal_cursor();
    usleep(SPEED*1000); // delay

    // print screen
    print_map(level->map->boxes_merge, screen);
    print_resources(screen, level);
    print_people(level, screen);
    disable_terminal_cursor();
    fflush(stdout);
    
    // iterate through all persons
    for (i = 0; i < level->num_people; i++){
        k = people_update(level->people[i], level, screen);
        disable_terminal_cursor();
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
    
    return flag;
}

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
Result reset_people(Level *level, Screen * screen){
    Position spos = screen_position(*(level->map->Start_pos), screen);
        
    for (int k = 0; k < level->num_people; k++){
        level->people[k]->position = spos;
        level->people[k]->state = DESINTEGRATED;
        level->people[k]->wait = 3*k;                
    }
}

/*
    Inputs:
        - *level: pointer to current level
    Outputs:
        int: indicating whether an error ocurred
    Description:
        Frees all memory allocated for the people
        Checks if pointers are set to NULL before calling free()
*/
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

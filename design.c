#include "design.h"
#include "read_keys.h"
#include "level.h"
#include "print_on_screen.h"


/* PRIVATE FUNCTION
    Input:
        - c: character to be converted
    Outputs:
        Box object
    Description:
        Given a character returns the corresponding resouce
        l -> LADDER
        f -> WALL_MERGE
        p -> PORTALA
        default -> AIR
*/
Box get_resource(char c){
    switch (c)
    {
    case 'l':
        return LADDER;    
    case 'f':
        return WALL_MERGE;
    case 'p':
        return PORTALA;
    default:
        return AIR;
    }
}

/* PRIVATE FUNCTION
    Input:
        - *screen: pointer to main screen
        - *level: pointer to current level
        - resource: resource to be placed
    Outputs:
        integer
    Description:
        Places or deletes resource depending on the current position of the cursor
        updates the remaining resources inside the level and the boxes_design map
        returns 0 if a invalid action is requested
                1 if a resource is placed
                2 if a resource is deleted
*/
int place_resource(Screen *screen, Level *level, char resource){
    Position pos = map_position(screen->cursor, screen);
    Box current = (level->map->boxes_design)[pos.x][pos.y];
    Box res = get_resource(resource);
    int k = 0;

    // Delete resource
    if(resource == (char)127 || res == LADDER || res == WALL_MERGE || res == PORTALA){
        if(current == LADDER){
            if(level->num_ladder_floor_act < level->num_ladder_floor){
                (level->num_ladder_floor_act)++;
            }else if(level->num_ladder_act < level->num_ladder){
                (level->num_ladder_act)++;
            }
        }else if(current == WALL_MERGE){
            if(level->num_ladder_floor_act < level->num_ladder_floor){
                (level->num_ladder_floor_act)++;
            }else if(level->num_floor_act < level->num_floor){
                (level->num_floor_act)++;
            }
        }else if(current == PORTALA){
            if(level->portal_act < level->portal){
                (level->portal_act)++;
            }
        }
        if((level->map->boxes_design)[pos.x][pos.y] != AIR){
            (level->map->boxes_design)[pos.x][pos.y] = AIR;
            (level->map->boxes_merge)[pos.x][pos.y] = AIR;
            k = 2;
        }
    }

    // Add resource
    if(res == LADDER){
        if(level->num_ladder_act > 0){
            (level->num_ladder_act)--;
            (level->map->boxes_design)[pos.x][pos.y] = res;
            return 1;
        }else if(level->num_ladder_floor_act > 0){
            (level->num_ladder_floor_act)--;
            (level->map->boxes_design)[pos.x][pos.y] = res;
            return 1;
        }
    }else if(res == WALL_MERGE){
        if(level->num_floor_act > 0){
            (level->num_floor_act)--;
            (level->map->boxes_design)[pos.x][pos.y] = res;
            return 1;
        }else if(level->num_ladder_floor_act > 0){
            (level->num_ladder_floor_act)--;
            (level->map->boxes_design)[pos.x][pos.y] = res;
            return 1;
        }
    }else if(res == PORTALA && (level->map->boxes)[pos.x][pos.y] == ZONAPORTAL){
        if(level->portal_act > 0){
            (level->portal_act)--;
            (level->map->boxes_design)[pos.x][pos.y] = res;
            return 1;
        }
    }

    return k;

}

/*
    Input:
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        FLAG: DESIGN_FINISHED or DESIGN_NOT_FINISHED
    Description:
        Called for each iteration of the design loop
        Waits for a key input and places / deletes the corresponding resource
        Returns a flag indicating wheter the design loop should finish or not
*/
FLAG design(Level *level, Screen *screen){
    enable_terminal_cursor();
    print_resources(screen, level);
    change_cursor(screen->cursor, screen);

    // Read key and change cursor of place resource
    char c = read_key();
    switch(c) {
    // Change cursor
    case('w'):
        if(screen->cursor.y > screen->map.y)
            screen->cursor.y--;
        break;
    case('s'):
        if(screen->cursor.y < screen->map.y + screen->map_height - 1)
            screen->cursor.y++;
        break;
    case('d'):
        if(screen->cursor.x < screen->map.x + screen->map_width - 1)
            screen->cursor.x++;
        break;
    case('a'):
        if(screen->cursor.x > screen->map.x)
            screen->cursor.x--;
        break;
    // Exit loop
    case('\n'):
        return DESIGN_FINISHED;
    // Place / delete corresponding resource
    default:
        if(is_position_valid_resources(screen->cursor, level, screen)){
            if(place_resource(screen, level, c) == 0){ // if action is invalid
                disable_terminal_cursor();
                change_color("red", "red");
                printf(" ");
                fflush(stdout);
                usleep(100*1000);
                enable_terminal_cursor();
            }
            
            // merge design map and print the resulting map
            map_merge(screen, level->map);
            print_map(level->map->boxes_merge, screen);
            enable_terminal_cursor();
            print_resources(screen, level);

        } else { // if position is invalid
            disable_terminal_cursor();
            change_color("red", "red");
            printf(" ");
            fflush(stdout);
            usleep(100*1000);
            enable_terminal_cursor();
            print_map(level->map->boxes_merge, screen);
            enable_terminal_cursor();
            print_resources(screen, level);
        }
    }

    change_cursor(screen->cursor, screen);
    return DESIGN_NOT_FINISHED;
}
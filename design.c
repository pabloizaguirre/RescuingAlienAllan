#include "design.h"
#include "read_keys.h"
#include "level.h"


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

/* returns 2 if it deletes a resource, 1 if it places a resource, and 0 if it hasn't done anything*/
int place_resource(Screen *screen, Level *level, char resource){
    Position pos = map_position(screen->cursor, screen);
    Box current = (level->map->boxes_design)[pos.x][pos.y];
    Box res = get_resource(resource);
    int k = 0;

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

FLAG design(Level *level, Screen *screen){
    print_resources(screen, level);
    change_cursor(screen->cursor, screen);
    //fflush(stdin);
    setvbuf(stdin, NULL, _IONBF, 0);
    char c = read_key();

    switch(c) {
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
    case('\n'):
        return DESIGN_FINISHED;
    default:
        if(is_position_valid_resources(screen->cursor, level, screen)){
            if(place_resource(screen, level, c) == 0){
                printf("\e[?25l");
                change_color("red", "red");
                printf(" ");
                fflush(stdout);
                usleep(100*1000);
                printf("\e[?25h");
            }
            map_merge(screen, level->map);
            print_map(level->map->boxes_merge, screen);
            print_resources(screen, level);
        } else {
            printf("\e[?25l");
            change_color("red", "red");
            printf(" ");
            fflush(stdout);
            usleep(100*1000);
            printf("\e[?25h");
            print_map(level->map->boxes_merge, screen);
            print_resources(screen, level);
        }
    }

    change_cursor(screen->cursor, screen);
    return DESIGN_NOT_FINISHED;
}
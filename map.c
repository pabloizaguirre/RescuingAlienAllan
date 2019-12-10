#include <stdio.h>
#include "map.h"
#include "read_from_file.h"
#include "print_on_screen.h"
#include "level.h"


Map* create_map(char *map_file, Screen *screen){
    
    char **map_char;
    Map *map;

    map_char = map_from_file(map_file, screen);
    if(map_char == NULL) return NULL;

    map = map_char_to_box(map_char, screen);
    if(map ==  NULL) return NULL;

    //The atributes from map are stored in map_char_to_box function.

    return map;
}

void free_map(Map *map){
    free(map);
    return;
}

Position map_position(Position position, Screen *screen){
    Position map_pos;

    if (position.x < screen->map.x || position.y < screen->map.y || position.x > screen->map.x + screen->map_width - 1 || position.y > screen->map.y + screen->map_height - 1){
        map_pos.x = -1;
        return map_pos;
    }
    map_pos.y = position.x - screen->map.x;
    map_pos.x = screen->map_height - (position.y - screen->map.y + 1);

    return map_pos;
}

Position screen_position(Position position, Screen *screen){
    Position screen_pos;

    if (position.x < 0 || position.y < 0 || position.x > screen->map_height - 1 || position.y > screen->map_width - 1){
        screen_pos.x = -1;
        return screen_pos;
    }

    screen_pos.x = position.y + screen->map.x;
    screen_pos.y = -position.x + screen->map_height + screen->map.y -1;

    return screen_pos;
}


//Problematico this function returns true if there is a ladder, but if the person comes from above, the position is
//not occupable 
Bool is_position_occupable(Position position, Level *level, Screen *screen) {
    int i, x, y;
    Box b;
    Position map_pos;

    map_pos = map_position(position, screen);
    if (map_pos.x < 0){
        return FALSE;
    }

    b = level->map->boxes_merge[map_pos.x][map_pos.y];

    if(b == WALL){
        return FALSE;
    }

    //state
    for(i = 0; i < level->num_people; i++){
        if ((level->people[i]->state != DESINTEGRATED) && (level->people[i]->state != FINISHED) && (level->people[i]->position.x == position.x) && (level->people[i]->position.y == position.y)){
            return FALSE;
        }
    }

    return TRUE;
}


Bool is_position_valid_resources(Position position, Level *level, Screen *screen){
    Box b;
    Position map_pos;

    map_pos = map_position(position, screen);
    if (map_pos.x < 0){
        return FALSE;
    }

    b = level->map->boxes[map_pos.x][map_pos.y];

    if (b != AIR && b != ZONAPORTAL){
        return FALSE;
    }
    return TRUE;
}

Result map_merge(Screen *screen, Map *map){
    int i, j;
    if (!map || !screen){
        return ERROR;
    }

    for (i = 0; i < screen->map_height; i++){
        for (j = 0; j < screen->map_width; j++){
            if(((map->boxes)[i][j] == AIR || (map->boxes)[i][j] == ZONAPORTAL) && (map->boxes_design)[i][j] != AIR){
                (map->boxes_merge)[i][j] = (map->boxes_design)[i][j];
            }
            else{
                (map->boxes_merge)[i][j] = (map->boxes)[i][j];
            }
        }
    }

    return OK;
}



Surroundings map_get_position_surroundings(Position position, Map *map){
    Surroundings sur;

    sur.center = &map->boxes[position.x][position.y];
    sur.bottom = &map->boxes[position.x][position.y-1];
    sur.top = &map->boxes[position.x][position.y+1];
    sur.left = &map->boxes[position.x-1][position.y];
    sur.right = &map->boxes[position.x+1][position.y];

    return sur;
}

Position* map_get_start_position(Map *map){
    return map->Start_pos;
}

Position* map_get_end_position(Map *map){
    return map->End_pos;
}

Position* map_get_portalb_position(Map *map){
    return map->PORTALB_pos;
}


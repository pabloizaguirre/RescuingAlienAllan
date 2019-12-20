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

Position map_position(Position position, Screen *screen){
    Position map_pos;

    if (position.x < screen->map.x || position.y < screen->map.y || position.x > screen->map.x + screen->map_width - 1 || position.y > screen->map.y + screen->map_height - 1){
        map_pos.x = -1;
        map_pos.y = -1;
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
        screen_pos.y = -1;
        return screen_pos;
    }

    screen_pos.x = position.y + screen->map.x;
    screen_pos.y = -position.x + screen->map_height + screen->map.y -1;

    return screen_pos;
}

//celdas
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

    if(b == WALL || b == WALL_MERGE || b == DISAPPEAR_WALL){
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

Position* map_get_start_position(Map *map){
    return map->Start_pos;
}

Position* map_get_end_position(Map *map){
    return map->End_pos;
}

Position* map_get_portalb_position(Map *map){
    return map->PORTALB_pos;
}

void free_map(Map *map, Screen *screen){
    if(map->boxes){
        for(int i = 0; i < screen->map_height; i++){
            if(map->boxes[i])free(map->boxes[i]);
        }
        free(map->boxes);
    }

    if(map->boxes_design){
        for(int i = 0; i < screen->map_height; i++){
            if(map->boxes_design[i])free(map->boxes_design[i]);
        }
        free(map->boxes_design);
    }

    if(map->boxes_merge){
        for(int i = 0; i < screen->map_height; i++){
            if(map->boxes_merge[i])free(map->boxes_merge[i]);
        }
        free(map->boxes_merge);
    }

    free(map->Start_pos);
    free(map->End_pos);
    free(map->PORTALB_pos);
    free (map);
    return;
}


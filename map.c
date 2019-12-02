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

Bool is_position_occupable(Position position, Level *level, Screen *screen) {
    int i, x, y;
    Box b;

    //falta comprobar que la posición esté dentro de la caja del mapa

    //transformar porisción en posición del mapa:
    x = position.x - screen->map.x;
    y = screen->map_height - (position.y - screen->map.y - 1);


    b = level->map->boxes[y][x];

    printf("la casilla de debajo es %d, %d, %d, pos %d, %d", x, y, b, position.x, position.y);

    if(b == WALL){
        return FALSE;
    }

    //state
    for(i = 0; i < level->num_people; i++){
        if ((level->people[i]->state != DESINTEGRATED) && (level->people[i]->position.x == position.x) && (level->people[i]->position.y == position.y)){
            return FALSE;
        }
    }

    return TRUE;
}


Bool is_position_valid_resources(Position position, Screen *screen, Map *map){
    //falta comprobar que la posición esté dentro de la caja del mapa
    //falta cambiar la posicion a tipo posicion de mapa
    if (map->boxes[position.x][position.y] != AIR){
        return FALSE;
    }
    return TRUE;
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


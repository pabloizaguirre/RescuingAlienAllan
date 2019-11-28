#include <stdio.h>
#include "map.h"
#include "types.h"
#include "read_from_file.h"
#include "print_on_screen.h"


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

void *free_map(Map *map){
    free(map);
    return;
}

/* 
Bool is_position_valid_resources(Position position, Map *map){
    if(position.x < map.)
} 
*/

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


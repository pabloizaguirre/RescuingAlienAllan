#include <stdio.h>
#include "map.h"


Map* create_map(char *map_file, Screen screen){
    

    
    char **map_char;
    map_char = map_from_file(map_file, screen);
    if(map_char == NULL) return NULL;

    Box **map_box;
    map_box = map_char_to_box(map_char, screen);
    if(map_box ==  NULL) return NULL;

    


}
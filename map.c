#include <stdio.h>
#include "map.h"
#include "read_from_file.h"
#include "print_on_screen.h"
#include "level.h"

/*
    Inputs:
        - *map_file: char pointer to the name of the file
        - *screen: pointer to the main screen
    Outputs:
        Map *: pointer to the initialized map
        NULL in case of error
    Description:
        Allocates memeory for all the pointers in the map
        Creates a Box** boxes to store the original map
                a Box** boxes_design to be changed in the design loop
                a Box** boxes_merge where the user placed resources and
                            the original map objects should be merged in
                            order to print it
        Initializes the values of the boxes and boxes_merge arrays to 
        the map found in the file and the boxes_design is initialized
        to AIR
        When the start and end doors and the portal b are found the
        pointers start_pos, end_pos, portalb_pos are initialized
        NOTE:
            This properties are set in the map_char_to_box function
*/
Map* create_map(char *map_file, Screen *screen){
    char **map_char;
    Map *map;

    // get the double pointer char array
    map_char = map_from_file(map_file, screen);
    if(map_char == NULL) return NULL;

    // create the map from that char arrray
    map = map_char_to_box(map_char, screen);
    if(map ==  NULL) return NULL;

    return map;
}

/*
    Inputs:
        - position: position to be processed
        - *screen: pointer to main screen
    Outputs:
        Position: position now relative to the map coordinates
    Description:
        Takes in a position relative to the screen coordinates
        and returns a position relative to the map coordinates
        
*/
Position map_position(Position position, Screen *screen){
    Position map_pos;

    // Check if position is valid
    if (position.x < screen->map.x || position.y < screen->map.y || position.x > screen->map.x + screen->map_width - 1 || position.y > screen->map.y + screen->map_height - 1){
        map_pos.x = -1;
        map_pos.y = -1;
        return map_pos;
    }

    // Calculate new coordinates
    map_pos.y = position.x - screen->map.x;
    map_pos.x = screen->map_height - (position.y - screen->map.y + 1);

    return map_pos;
}

/*
    Inputs:
        - position: position to be processed
        - *screen: pointer to main screen
    Outputs:
        Position: position now relative to the screen coordinates
    Description:
        Takes in a position relative to the map coordinates
        and returns a position relative to the screen coordinates        
*/
Position screen_position(Position position, Screen *screen){
    Position screen_pos;

    // checks if position is valid
    if (position.x < 0 || position.y < 0 || position.x > screen->map_height - 1 || position.y > screen->map_width - 1){
        screen_pos.x = -1;
        screen_pos.y = -1;
        return screen_pos;
    }

    // calculate new coordintates
    screen_pos.x = position.y + screen->map.x;
    screen_pos.y = -position.x + screen->map_height + screen->map.y -1;

    return screen_pos;
}

/*
    Inputs:
        - position: position to be processed
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        Bool: whether or not the given position is occupable by a person
    Description:
        Takes in a position relative to the screen coordinates
        sees if box at that position in the boxes_merge isn't a
        WALL / WALL_MERGE / DISAPPEAR_WALL
        and if it is occupied for any other person
        NOTE:
            Stairs are not considered to be an obstacle because they can
            be occupied from a side but not from above     
            Finished and desintegrated people are not taken into account  
*/
Bool is_position_occupable(Position position, Level *level, Screen *screen) {
    int i, x, y;
    Box b;
    Position map_pos;

    // make the position relative to the map
    map_pos = map_position(position, screen);
    if (map_pos.x < 0){
        return FALSE;
    }

    // check the boxes_merge array
    b = level->map->boxes_merge[map_pos.x][map_pos.y];

    if(b == WALL || b == WALL_MERGE || b == DISAPPEAR_WALL){
        return FALSE;
    }

    // chdeck the other people postion and state
    for(i = 0; i < level->num_people; i++){
        if ((level->people[i]->state != DESINTEGRATED) && (level->people[i]->state != FINISHED) && (level->people[i]->position.x == position.x) && (level->people[i]->position.y == position.y)){
            return FALSE;
        }
    }

    return TRUE;
}

/*
    Inputs:
        - position: position to be processed
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        Bool: whether or not the given position is occupable by a resource
    Description:
        Takes in a position relative to the screen coordinates
        sees if box at that position in the boxes array is a
        ZONAPORTAL / AIR
        everything else is considered invalid for resources
*/
Bool is_position_valid_resources(Position position, Level *level, Screen *screen){
    Box b;
    Position map_pos;

    // make position relative to the map coordinates
    map_pos = map_position(position, screen);
    if (map_pos.x < 0){
        return FALSE;
    }
    b = level->map->boxes[map_pos.x][map_pos.y];

    // check if position is valid
    if (b != AIR && b != ZONAPORTAL){
        return FALSE;
    }
    return TRUE;
}

/*
    Inputs:
        - *map: pointer to map to be merged
        - *screen: pointer to main screen
    Outputs:
        Result: to notify if an error ocurred
    Description:
        Goes through the boxes and boxes_design arrays saving at
        boxes_merge a copy of the boxes array where the AIR and
        ZONAPORTAL boxes are replaces with the contents of the
        boxes_design array
*/
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

/*
    Inputs:
        - *map: pointer to map to be freed
        - *screen: pointer to main screen
    Description:
        Frees all the memory allocated for the given map
        Check if pointers are set to NULL before calling free()
*/
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


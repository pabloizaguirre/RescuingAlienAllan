#include "print_on_screen.h"
#include "types.h"
#include "map.h"
#include "level.h"
#include "read_from_file.h"
#include "read_keys.h"

Box get_resource(char c){
    switch (c)
    {
    case 'e':
        return LADDER;    
    case 'q':
        return WALL;
    case 'p':
        return PORTALA;
    default:
        return AIR;
    }
}

int place_resource(Screen *screen, Level *level, char resource){
    Position pos = screen->cursor;
    Box current = (level->map->boxes_design)[pos.y][pos.x];
    Box res = get_resource(resource);

    if(current == LADDER){
        if(level->num_ladder_floor_act < level->num_ladder_floor){
            (level->num_ladder_floor_act)++;
        }else if(level->num_ladder_act < level->num_ladder){
            (level->num_ladder_act)++;
        }
    }else if(current == WALL){
        if(level->num_ladder_floor_act < level->num_ladder_floor){
            (level->num_ladder_floor_act)++;
        }else if(level->num_floor_act < level->num_ladder){
            (level->num_floor_act)++;
        }
    }
    (level->map->boxes_design)[pos.y][pos.x] = res;

    if(res == LADDER){
        if(level->num_ladder_act > 0){
            (level->num_ladder_act)--;
        }else if(level->num_ladder_floor_act > 0){
            (level->num_ladder_floor_act)--;
        }
    }else if(res == WALL){
        if(level->num_floor_act > 0){
            (level->num_floor_act)--;
        }else if(level->num_ladder_floor_act > 0){
            (level->num_ladder_floor_act)--;
        }
    }

}



//it returns the number of resources used
FLAG design(Level *level, Screen *screen){
    print_resources(screen, level);
    change_cursor(screen->cursor, screen);
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
    default:
        if(is_position_valid_resources(screen->cursor, level, screen)){
            place_resource(screen, level, c);
            map_merge(&screen, level->map);
            print_map(level->map->boxes_merge, &screen);
            print_resources(&screen, level);
        }
    }

    change_cursor(screen->cursor, screen);
}


int main(int argc, char** argv){
    Screen screen;
    Level *level;
    char interface_file[] = "size_test.txt";
    FLAG rec;
    int i = 0;

    init_screen(interface_file, &screen);
    print_message(&screen, "Start");
    //print_title(&screen, "Saving alien Alan");
    
    level = levels_init(&screen);

    screen.cursor = screen.map;
    map_merge(&screen, level->map);
    print_map(level->map->boxes_merge, &screen);
    print_resources(&screen, level);
    while(TRUE){
        
        rec = design(level, &screen);
        /* if (rec < level->cond_recursos[0]){
            return ERR;
        } else if (rec > level->cond_recursos[1]){
            n_estrellas = 1;
        } else if (rec > level->cond_recursos[2]){
            n_estrellas = 2;
        } else {
            n_estrellas = 3;
        } */
        
        /* do{
            for(int i = 0; i < level->num_people; i++){
                people_update(level->people[i]);
            }
        }while(ret == LEVEL_FINISHED); */

        i++;
    }
    print_resources(&screen, level);
    restore_screen(&screen);

    return OK;
}

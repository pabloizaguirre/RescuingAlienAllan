#include "print_on_screen.h"
#include "types.h"
#include "map.h"
#include "level.h"
#include "read_from_file.h"
#include "read_keys.h"

//it returns the number of resources used
FLAG design(Level *level, Screen *screen){
    //print_map(level->map->boxes, &screen);
    //print_resources(level, screen);
    //change_cursor(screen->map, screen);
    char c = read_key();
    
    switch(c) {
    case('w'):
        screen->cursor.y--;
        break;
    case('s'):
        screen->cursor.y++;
        break;
    case('d'):
        screen->cursor.x++;
        break;
    case('a'):
        screen->cursor.x--;
        break;
    }

    change_cursor(screen->cursor, screen);
    //rintf(" %c", read_key());
}


int main(int argc, char** argv){
    Screen screen;
    Level *level;
    char interface_file[] = "size_test.txt";
    FLAG rec;

    init_screen(interface_file, &screen);
    //print_message(screen, "Start");
    //print_title(screen, "Saving alien Alan");
    
    //level = levels_init(screen);

    while(TRUE){
        screen.cursor = screen.map;
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

        
    }

    restore_screen(&screen);

    return OK;
}

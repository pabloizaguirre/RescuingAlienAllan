#include "print_on_screen.h"
#include "types.h"
#include "map.h"
#include "level.h"
#include "read_from_file.h"
#include "read_keys.h"
#include "people.h"
#include <time.h>

//it returns the number of resources used
FLAG design(Level *level, Screen *screen){
    char text[40];
    int i;
    printf("\e[?25l");
    print_map(level->map->boxes, screen);
    print_resources(screen, level);
    
    for (i = 0; i < level->num_people; i++){
        if (people_update(level->people[i], level, screen) < 0){
            printf("ERROR EN PEOPLE_UPDATE()\n");
            fflush(stdout);
            return GAME_FINISHED;
        }
    }

    print_people(level, screen);

    sprintf(text, "position: (%d, %d)", screen->cursor.x, screen->cursor.y);
    //sprintf(text, "position occupable: %d", is_position_occupable(screen->cursor, level, screen));
    print_message(screen, text);
    change_cursor(screen->cursor, screen);
    printf("\e[?25h");
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
    }

    change_cursor(screen->cursor, screen);
}


FLAG movement_loop(Level *level, Screen * screen){
    int k, i;
    FLAG flag = LEVEL_FINISHED;

    usleep(100*1000);

    print_map(level->map->boxes, screen);
    print_resources(screen, level);
    print_people(level, screen);
    fflush(stdout);
    
    for (i = 0; i < level->num_people; i++){
        k = people_update(level->people[i], level, screen);
        fflush(stdout);
        if (k < 0){
            printf("ERROR EN PEOPLE_UPDATE()\n");
            fflush(stdout);
            return GAME_FINISHED;
        }
        if (k > 0){
            flag = LEVEL_NOT_FINISHED;
        }
    }
    if (flag == LEVEL_FINISHED){
        return LEVEL_FINISHED;
    }
    return LEVEL_NOT_FINISHED;
}


int main(int argc, char** argv){
    Screen screen;
    Level *level;
    char interface_file[] = "size_test.txt";
    char text[40];
    FLAG rec;
    int i = 0;
    Position pos;
    Position map_pos;

    init_screen(interface_file, &screen);
    print_message(&screen, "Start");
    //print_title(&screen, "Saving alien Alan");
    
    level = levels_init(&screen);
    
    

    screen.cursor = screen.map;


    movement_loop(level, &screen);

    printf("\e[?25l");
    while(TRUE){
        rec = design(level, &screen);

        /* if (movement_loop(level, &screen) == LEVEL_FINISHED){
            print_message(&screen, "Level finished");
            fflush(stdout);
            usleep(2*1000*1000);
            break;
        } */
        i++;
    } 
    printf("\e[?25h");

    print_resources(&screen, level);
    restore_screen(&screen);

    return OK;
}

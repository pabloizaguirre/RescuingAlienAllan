#include "print_on_screen.h"
#include "types.h"
#include "map.h"
#include "level.h"
#include "read_from_file.h"
#include "read_keys.h"
#include "people.h"

//it returns the number of resources used
FLAG design(Level *level, Screen *screen){
    char text[40];
    print_map(level->map->boxes, screen);
    print_resources(screen, level);
    print_people(level, screen);
    if (people_update(level->people[0], level, screen) < 0){
        printf("ERROR EN PEOPLE_UPDATE()\n");
        fflush(stdout);
        return GAME_FINISHED;
    }
    sprintf(text, "position: (%d, %d)", screen->cursor.x, screen->cursor.y);
    //sprintf(text, "position occupable: %d", is_position_occupable(screen->cursor, level, screen));
    print_message(screen, text);
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
    }

    change_cursor(screen->cursor, screen);
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

    level->map->Start_pos->y = 10;
    level->map->Start_pos->x = 30;
    
    printf("start pos: %d, %d", level->map->Start_pos->x, level->map->Start_pos->y);

    screen.cursor = screen.map;

    pos.x = screen.map.x + screen.map_width - 1;
    pos.y = screen.map.y;

    change_cursor(pos, &screen);
    printf("X");
    //sprintf(text, "position occupable: %d", is_position_occupable(pos, level, &screen));
    //print_message(&screen, text);

    //is_position_occupable(pos, level, &screen);
    map_pos = map_position(screen.map, &screen);
    if (map_pos.x < 0){
        printf("Error\n");
        return ERROR;
    }
    sprintf(text, "%d, %d", map_pos.x, map_pos.y);
    print_message(&screen, text);

    printf("%d, %d", level->map->PORTALB_pos->x, level->map->PORTALB_pos->y);


    /* level->map->boxes[map_pos.x][map_pos.y] = LAVA;*/
    print_map(level->map->boxes, &screen); 

    while(TRUE){
        
        rec = design(level, &screen);
        i++;
    } 

    print_resources(&screen, level);
    restore_screen(&screen);

    return OK;
}

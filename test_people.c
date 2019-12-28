#include "print_on_screen.h"
#include "types.h"
#include "map.h"
#include "level.h"
#include "design.h"
#include <time.h>

void free_all(Screen *screen, Level * first_level);


int main(int argc, char** argv){
    Screen *screen;
    Level *first_level, *actual_level;
    char interface_file[] = "./designs/screen_info.txt";
    FLAG rec;
    Position pos, map_pos;
    int i = 0;
    Level_result lr;

    screen = init_screen(interface_file);
    if(!screen) free_all(screen, first_level);
    print_message(screen, "Start");
    
    first_level = levels_init(screen);
    if(!first_level) free_all(screen, first_level);
    actual_level = first_level;
    
    pos.x = 33;
    pos.y = 15;
    
    map_merge(screen, actual_level->map);
    print_map(actual_level->map->boxes_merge, screen);
    print_resources(screen, actual_level);

    map_pos.x = screen->map_height/2 + 5;   
    map_pos.y = screen->map_width/2 - 8;
    pos = screen_position(map_pos, screen);
    print_file("./designs/star.txt", pos, screen, TRUE);
    fflush(stdout);

    pos.x = 45;
    pos.y = 38;

    map_pos = map_position(pos, screen);

    change_cursor(pos, screen);
    change_color("yellow", "black");
    printf("%d, %d, is position occupable: %d", map_pos.x, map_pos.y, is_position_occupable(pos, actual_level, screen));




    restore_screen(screen);

    return OK;
}

void free_all(Screen *screen, Level * first_level){
    free_level(first_level, screen);
    restore_screen(screen);
}
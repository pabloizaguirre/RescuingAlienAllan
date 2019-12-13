#include "print_on_screen.h"
#include "types.h"
#include "level.h"
#include "design.h"


int main(int argc, char** argv){
    Screen *screen;
    Level *first_level, *actual_level;
    char interface_file[] = "size_test.txt";
    FLAG rec;
    int i = 0;

    screen = init_screen(interface_file);
    print_message(screen, "Start");
    
    first_level = levels_init(screen);
    actual_level = first_level;

    while(actual_level->next_level != NULL){
        /* Design loop */
        screen->cursor = screen->map;
        map_merge(screen, actual_level->map);
        print_map(actual_level->map->boxes_merge, screen);
        print_resources(screen, actual_level);
        while(design(actual_level, screen) == DESIGN_NOT_FINISHED);

        /* People movement loop */
        movement_loop(actual_level, screen);
        while(movement_loop(actual_level, screen) != LEVEL_FINISHED);
        

        
        if(level_end(game_status(actual_level), screen));

        /* Load next level */
        print_message(screen, "Level finished");
        fflush(stdout);
        usleep(2*1000*1000);
        actual_level = actual_level->next_level;
    } 
    
    
    
    print_message(screen, "Goodbye");
    restore_screen(screen);

    return OK;
}

#include "print_on_screen.h"
#include "types.h"
#include "level.h"
#include "design.h"
#include <termios.h>

void free_all(Screen *screen, Level * first_level);

int main(int argc, char** argv){
    Screen *screen;
    Level *first_level, *actual_level;
    char interface_file[] = "./designs/screen_info.txt";
    FLAG rec;
    int i = 0;
    Level_result lr;

    screen = init_screen(interface_file);
    if(!screen) {
        free_all(screen, first_level);
        return ERROR;
    }
    
    actual_level = levels_init(screen);
    if(!actual_level) {
        free_all(screen, actual_level->first_level);
        return ERROR;
    }
    
    first_level = actual_level->first_level;

    while(actual_level != NULL){
        print_message(screen, actual_level->message);

        /* Design loop */
        screen->cursor = screen->map;
        map_merge(screen, actual_level->map);
        print_map(actual_level->map->boxes_merge, screen);
        print_resources(screen, actual_level);
        tcflush(fileno(stdin), TCIFLUSH); // get rid of the keys pressed in the game loop
        while(design(actual_level, screen) == DESIGN_NOT_FINISHED);


        /* People movement loop */
        printf("\e[?25l");
        movement_loop(actual_level, screen);
        while(movement_loop(actual_level, screen) != LEVEL_FINISHED);
        printf("\e[?25h");
        
        lr = game_status(actual_level);
        if(lr == RES_ERROR) return ERROR;
        
        
        if(level_end(lr, screen) == ERROR) return ERROR;

        reset_people(actual_level, screen);

        actual_level = level_menu(actual_level, screen, first_level);
    } 
      
    
    print_message(screen, "Goodbye");
    free_all(screen, first_level);

    return OK;
}

void free_all(Screen *screen, Level * first_level){
    free_level(first_level, screen);
    restore_screen(screen);
}

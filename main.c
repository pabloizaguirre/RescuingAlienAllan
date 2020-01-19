#include "print_on_screen.h"
#include "types.h"
#include "level.h"
#include "design.h"
#include <termios.h>

/*
    Input:
        - *first_level: pointer to the first level
    Description:
        Checks if given pointers are set to NULL
        and frees the allocated memory inside
        And resotres the terminal
*/
void free_all(Screen *screen, Level * first_level);

int main(int argc, char** argv){
    Screen *screen;
    Level *first_level, *actual_level;
    char interface_file[] = "./designs/screen_info.txt";
    FLAG rec;
    int i = 0;
    Level_result lr;

    // initialize screen
    screen = init_screen(interface_file);
    if(!screen) {
        free_all(screen, first_level);
        return ERROR;
    }

    // open a menu to choose wheter you want to continue
    // with the last game
    continue_or_newgame_screen(screen);
    
    // initialize levels
    actual_level = levels_init(screen);
    if(!actual_level) {
        free_all(screen, actual_level->first_level);
        return ERROR;
    }    
    first_level = actual_level->first_level;

    // main game loop
    while(actual_level != NULL){
        print_message(screen, actual_level->message);
        print_title(screen, screen->title);
        print_margins(screen);

        // Design loop
        screen->cursor = screen->map;
        map_merge(screen, actual_level->map);
        print_map(actual_level->map->boxes_merge, screen);
        print_resources(screen, actual_level);
        tcflush(fileno(stdin), TCIFLUSH); // clear the input buffer
        while(design(actual_level, screen) == DESIGN_NOT_FINISHED);


        // People movement loop
        disable_terminal_cursor();
        movement_loop(actual_level, screen);
        while(movement_loop(actual_level, screen) != LEVEL_FINISHED);
        enable_terminal_cursor();
        
        lr = game_status(actual_level);
        if(lr == RES_ERROR) return ERROR;
        
        usleep(1000*1000);
        if(level_end(lr, screen) == ERROR) return ERROR;

        // reset the people array
        reset_people(actual_level, screen);

        // open a menu to choose a level
        actual_level = level_menu(actual_level, screen, first_level);
    } 
      
    // Exit
    print_message(screen, "Goodbye");
    free_all(screen, first_level);

    enable_terminal_cursor();

    return OK;
}

/*
    Inputs:
        - *first_level: pointer to the first level
    Description:
        Checks if given pointers are set to NULL
        and frees the allocated memory inside
        And resotres the terminal
*/
void free_all(Screen *screen, Level * first_level){
    free_level(first_level, screen);
    restore_screen(screen);
}

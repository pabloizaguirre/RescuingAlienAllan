#include "level.h"
#include "interface.h"

Level *levels_init(screen screen){
    FILE *f;
    

    /*
    if there is no progress.txt file, which means that you have restarted the game,
    then the main.c initializes everything and starts the game on level 1
     */
    f = fopen("progress.txt", r+);
    if (!f) {
        //Initialization of the game
        f = fopen("progress.txt", w+);
        if (!f) {
            print_message(screen, "Failed to save the game (progress.txt file couldn't be created)");
            return ERROR;
        }
        //initialization of the game from cero
        
    } else {
        //initialization of the game from a progress.txt file
    }
}
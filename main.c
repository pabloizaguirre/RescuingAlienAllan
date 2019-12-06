#include "print_on_screen.h"
#include "types.h"
#include "level.h"
#include "design.h"

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
    while(design(level, &screen) == DESIGN_NOT_FINISHED);
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
    
    
    print_message(&screen, "Goodbye");
    restore_screen(&screen);

    return OK;
}

#include "level.h"
#include "interface.h"
#include "print_on_screen.h"

#include <string.h>

Level *levels_init(Screen screen){
    FILE *f;
    Level *plevel;      
    plevel = (Level*)malloc(sizeof(Level)*NUM_LEVELS);
    if(plevel == NULL) return NULL;

    for(int i = 0; i < NUM_LEVELS; i++){
        char level_file[256];
        sprintf(level_file, "level%d.txt", i+1);
        FILE *f_level;

        f_level = fopen(level_file, "r");
        if(f == NULL) return NULL;

        plevel[i].next_level = &plevel[(i+1)%NUM_LEVELS];
        if(i == 0){
            plevel[i].last_level = &plevel[NUM_LEVELS];
        }
        else{
            plevel[i].last_level = &plevel[i-1];
        }
        char level_number[64];
        if(read_line(f_level, level_number) != OK){
            return NULL;
        }
        plevel[i].level_number = atoi(level_number);

        char level_message[256];
        if(read_line(f_level, level_message) != OK){
            return NULL;
        }
        strcpy(plevel[i].message, level_message);

        char num_people[64];
        if(read_line(f_level, num_people) != OK){
            return NULL;
        }
        plevel[i].num_people = atoi(num_people);

        char num_ladder_floor[64];
        if(read_line(f_level, num_ladder_floor) != OK){
            return NULL;
        }
        plevel[i].num_ladder_floor = atoi(num_ladder_floor);

        char num_ladder[64];
        if(read_line(f_level, num_ladder) != OK){
            return NULL;
        }
        plevel[i].num_ladder = atoi(num_ladder);

        char num_floor[64];
        if(read_line(f_level, num_floor) != OK){
            return NULL;
        }
        plevel[i].num_floor = atoi(num_floor);

        char num_portal[64];
        if(read_line(f_level, num_portal) != OK){
            return NULL;
        }
        plevel[i].portal = atoi(num_portal);

        char map_file[256];
        if(read_line(f_level,map_file) != OK){
            return NULL;
        }

        plevel[i].map = create_map(map_file);
        if(plevel[i].map == NULL) return NULL;
    }
}
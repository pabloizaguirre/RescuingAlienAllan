#include "level.h"
#include "interface.h"
#include "print_on_screen.h"
#include "map.h"

#include <string.h>
#include <stdlib.h>

Level *levels_init(Screen *screen){
    Level *plevel_first, *plevel, *plevel_last;      
    FILE *f_level;
    char level_file[256];
    char level_number[64];
    char level_message[256];
    char num_people[64];
    char num_ladder_floor[64];
    char map_file[256];
    char num_portal[64];
    char num_floor[64];
    char num_ladder[64];

    //esto es para probar con people_create
    Position spos;
    spos.x = 30;
    spos.y = 19;

    for(int i = 0; i < NUM_LEVELS; i++){

        sprintf(level_file, "level%d.txt", i+1);

        f_level = fopen(level_file, "r");
        if(f_level == NULL){
            free(plevel);
            return NULL;
        } 

        plevel = (Level*)malloc(sizeof(Level));
        if(plevel == NULL) return NULL;

        plevel->next_level = NULL;
        if(i == 0){
            plevel_first = plevel;
            plevel_last = plevel;
        }else{
            plevel_last->next_level = plevel;
            plevel_last = plevel;
        }

        if(read_line(f_level, level_number) != OK){
            return NULL;
        }
        plevel->level_number = atoi(level_number);

        
        if(read_line(f_level, level_message) != OK){
            return NULL;
        }
        strcpy(plevel->message, level_message);


        if(read_line(f_level, num_people) != OK){
            return NULL;
        }
        plevel->num_people = atoi(num_people);

        if(read_line(f_level, num_ladder_floor) != OK){
            return NULL;
        }
        plevel->num_ladder_floor = atoi(num_ladder_floor);

       
        if(read_line(f_level, num_ladder) != OK){
            return NULL;
        }
        plevel->num_ladder = atoi(num_ladder);

        
        if(read_line(f_level, num_floor) != OK){
            return NULL;
        }
        plevel->num_floor = atoi(num_floor);

        
        if(read_line(f_level, num_portal) != OK){
            return NULL;
        }
        plevel->portal = atoi(num_portal);

        if(read_line(f_level,map_file) != OK){
            return NULL;
        }

        plevel->map = create_map(map_file, screen);
        if(plevel->map == NULL) return NULL;

        plevel->people = (People**)malloc(sizeof(People*)*plevel->num_people);
        if(plevel->people == NULL) return NULL;


        //falta hacer esto bien que está hecho para que solo este viva la primera persona y que la posicion
        //donde empieza esté bien
        plevel->people[0] = create_people(64, spos, ALIVE);
        for(int k = 1; k < plevel->num_people; k++){
            plevel->people[k] = create_people(64, spos, DESINTEGRATED);
        }


        
        fclose(f_level);
    }
    return plevel_first;
} 

Map *level_get_map(Level *level){
    return level->map;
}
Level *level_get_next_level(Level *level){
    return level->next_level;
}
People **level_get_peoples(Level *level){
    return level->people;
}

void level_destroy(Level *levels){
    free (levels);
    return;
}




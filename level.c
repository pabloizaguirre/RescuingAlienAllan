#include "level.h"
#include "interface.h"
#include "print_on_screen.h"
#include "map.h"

#include <string.h>
#include <stdlib.h>

Level *levels_init(Screen *screen){
    Level *plevel_first, *plevel, *plevel_last;      
    FILE *f_level;
    Position spos;
    char level_file[256];
    char level_number[64];
    char level_message[256];
    char num_people[64];
    char map_file[256];
    char folder_name[256];
    char num_portal[64], num_floor[64], num_ladder[64], num_ladder_floor[64], min_people[64];
    char num_portal_2[64], num_floor_2[64], num_ladder_2[64], num_ladder_floor_2[64], min_people_2[64];
    char num_portal_3[64], num_floor_3[64], num_ladder_3[64], num_ladder_floor_3[64], min_people_3[64];
    
    

    for(int i = 0; i < NUM_LEVELS; i++){

        sprintf(folder_name, "./level_files/");
        sprintf(level_file, "./level_files/level%d.txt", i+1);

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
        plevel->num_ladder_floor_act = atoi(num_ladder_floor);
       
        if(read_line(f_level, num_ladder) != OK){
            return NULL;
        }
        plevel->num_ladder = atoi(num_ladder);
        plevel->num_ladder_act = atoi(num_ladder);

        if(read_line(f_level, num_floor) != OK){
            return NULL;
        }
        plevel->num_floor = atoi(num_floor);
        plevel->num_floor_act = atoi(num_floor);

        
        if(read_line(f_level, num_portal) != OK){
            return NULL;
        }
        plevel->portal = atoi(num_portal);
        plevel->portal_act = atoi(num_portal);

        if(read_line(f_level, min_people) != OK){
            return NULL;
        }
        plevel->min_people = atoi(min_people);
        
        //2 stars
        if(read_line(f_level, num_ladder_floor_2) != OK){
            return NULL;
        }
        plevel->num_ladder_floor_2 = atoi(num_ladder_floor_2);
       
        if(read_line(f_level, num_ladder_2) != OK){
            return NULL;
        }
        plevel->num_ladder_2 = atoi(num_ladder_2);

        if(read_line(f_level, num_floor_2) != OK){
            return NULL;
        }
        plevel->num_floor_2 = atoi(num_floor_2);

        
        if(read_line(f_level, num_portal_2) != OK){
            return NULL;
        }
        plevel->portal_2 = atoi(num_portal_2);

        if(read_line(f_level, min_people_2) != OK){
            return NULL;
        }
        plevel->min_people_2 = atoi(min_people_2);

        //3 stars
        if(read_line(f_level, num_ladder_floor_3) != OK){
            return NULL;
        }
        plevel->num_ladder_floor_3 = atoi(num_ladder_floor_3);
       
        if(read_line(f_level, num_ladder_3) != OK){
            return NULL;
        }
        plevel->num_ladder_3 = atoi(num_ladder_3);

        if(read_line(f_level, num_floor_3) != OK){
            return NULL;
        }
        plevel->num_floor_3 = atoi(num_floor_3);

        
        if(read_line(f_level, num_portal_3) != OK){
            return NULL;
        }
        plevel->portal_3 = atoi(num_portal_3);
        
        if(read_line(f_level, min_people_3) != OK){
            return NULL;
        }
        plevel->min_people_3 = atoi(min_people_3);

        if(read_line(f_level,map_file) != OK){
            return NULL;
        }
        strcat(folder_name, map_file);
        plevel->map = create_map(folder_name, screen);
        if(plevel->map == NULL) return NULL;

        plevel->people = (People**)malloc(sizeof(People*)*plevel->num_people);
        if(plevel->people == NULL) return NULL;

        

        spos = screen_position(*(plevel->map->Start_pos), screen);
        
        for (int k = 0; k < plevel->num_people; k++){
            plevel->people[k] = create_people(64, spos, DESINTEGRATED, k);
        }

        fclose(f_level);
    }
    return plevel_first;
} 

/* 
    Returns the state of the game 
 */
Level_result game_status(Level *level){
    int i, people_finished = 0;
    if (!level) return RES_ERROR;

    for (i = 0; i < level->num_people; i++){
        if(level->people[i]->state == FINISHED) people_finished++;
    }

    if (level->num_ladder_floor_act > level->num_ladder_floor_3 && level->num_ladder_act > level->num_ladder_3
                    && level->num_floor_act > level->num_floor_3 && level->portal_act > level->portal_3 
                    && people_finished > level->min_people_3){
        return SUPREME;


    } else if (level->num_ladder_floor_act == level->num_ladder_floor_3 && level->num_ladder_act == level->num_ladder_3
                    && level->num_floor_act == level->num_floor_3 && level->portal_act == level->portal_3 
                    && people_finished == level->min_people_3){
        return STARS_3;


    } else if (level->num_ladder_floor_act >= level->num_ladder_floor_2 && level->num_ladder_act >= level->num_ladder_2
                    && level->num_floor_act >= level->num_floor_2 && level->portal_act >= level->portal_2
                    && people_finished >= level->min_people_2){
        return STARS_2;


    } else if (level->num_ladder_floor_act >= level->num_ladder_floor && level->num_ladder_act >= level->num_ladder
                    && level->num_floor_act >= level->num_floor && level->portal_act >= level->portal
                    && people_finished >= level->min_people){
        return STARS_1;


    } else {
        return LOST;
    }
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




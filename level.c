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

Result print_resources(Screen *screen, Level *level){
    int end_flag = 0;
    int w = screen->screen_width - screen->map_width - 3;
    int h = screen->map_height;
    Position res_pos, pos;
    res_pos.x = 2;
    res_pos.y = (screen->map).y;

    pos = res_pos;
	
	change_color("reset", "reset");
    //Clear the messagebox
    change_cursor(pos, screen);
    for(int i=0; i < h; i++){
        for(int j=0; j < w; j++){
            printf(" ");
        }
        pos.y += 1;
        change_cursor(pos, screen);
    }
    //Print the message croping it to the right dimensions
    pos = res_pos;
    change_cursor(pos, screen);
    if(level->num_ladder_floor > 0){
        printf("LADDERS/FLOORS: x%d", level->num_ladder_floor);
    }

    pos.y++;
    change_cursor(pos, screen);
    if(level->num_ladder > 0){
        printf("LADDERS: x%d", level->num_ladder);
    }

    pos.y++;
    change_cursor(pos, screen);
    if(level->num_floor > 0){
        printf("FLOORS: x%d", level->num_floor);
    }

    pos.y++;
    change_cursor(pos, screen);
    if(level->portal > 0){
        printf("PORTAl: x%d", level->portal);
    }
    return OK;
}

void level_destroy(Level *levels){
    free (levels);
    return;
}




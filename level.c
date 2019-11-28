#include "level.h"
#include "interface.h"
#include "print_on_screen.h"
#include "map.h"

#include <string.h>
#include <stdlib.h>

Level *levels_init(Screen *screen){
    Level *plevel;      
    plevel = (Level*)malloc(sizeof(Level)*NUM_LEVELS);
    if(plevel == NULL) return NULL;

    for(int i = 0; i < NUM_LEVELS; i++){
        char level_file[256];
        sprintf(level_file, "level%d.txt", i+1);
        FILE *f_level;

        f_level = fopen(level_file, "r");
        if(f_level == NULL) return NULL;

        plevel[i].next_level = &plevel[(i+1)%NUM_LEVELS];
        plevel[i].last_level = &plevel[NUM_LEVELS];
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

        plevel[i].map = create_map(map_file, screen);
        if(plevel[i].map == NULL) return NULL;
    }
    return plevel;
} 

Map *level_get_map(Level *level){
    return level->map;
}
Level *level_get_next_level(Level *level){
    return level->next_level;
}
Level *level_get_last_level(Level *level){
    return level->last_level;
}
People **level_get_peoples(Level *level){
    return level->people;
}

Result print_resources(Screen *screen, Level level){
    int end_flag = 0;
    int w = screen->screen_width - screen->map_width;
    int h = screen->map_height;
    Position res_pos, pos;
    res_pos.x = 1;
    res_pos.y = 3 + (screen->screen_height - screen->messagebox_height - screen->map_height);

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
    if(level.num_ladder_floor > 0){
        printf("LADDERS/FLOORS: x%d", level.num_ladder_floor);
    }

    pos.y++;
    change_cursor(pos, screen);
    if(level.num_ladder > 0){
        printf("LADDERS: x%d", level.num_ladder);
    }

    pos.y++;
    change_cursor(pos, screen);
    if(level.num_floor > 0){
        printf("FLOORS: x%d", level.num_floor);
    }

    pos.y++;
    change_cursor(pos, screen);
    if(level.portal > 0){
        printf("PORTAl: x%d", level.portal);
    }
    return OK;
}






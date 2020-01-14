#include "level.h"
#include "interface.h"
#include "print_on_screen.h"
#include "map.h"

#include <string.h>
#include <stdlib.h>
#include <termios.h>

Level *levels_init(Screen *screen){
    Level *plevel_first, *plevel, *plevel_last, *act_level;      
    FILE *f_level, *f_progress;
    Position spos;
    int k = 1, act_level_flag = 0;
    char n_stars[64];
    char level_file[256];
    char level_number[10];
    char level_message[256];
    char num_people[64];
    char map_file[256];
    char folder_name[256];
    char num_portal[64], num_floor[64], num_ladder[64], num_ladder_floor[64], min_people[64];
    char num_portal_2[64], num_floor_2[64], num_ladder_2[64], num_ladder_floor_2[64], min_people_2[64];
    char num_portal_3[64], num_floor_3[64], num_ladder_3[64], num_ladder_floor_3[64], min_people_3[64];
    
    f_progress = fopen("progress.dat", "r+");
    if(!f_progress){
        //Initialization of the game
        f_progress = fopen("progress.dat", "w");
        if (!f_progress) {
            print_message(screen, "Failed to save the game (progress.dat file couldn't be created)");
            return ERROR;
        }
        fprintf(f_progress, "#Stars achieved in each level in order. 4 stars is SUPREME\n");
        k = 0;
    }
    

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

        if(k == 0){
            fprintf(f_progress, "0\n");
            fflush(f_progress);
            plevel->n_stars = 0;
            if (act_level_flag == 0){
                act_level = plevel;
                act_level_flag = 1;
            }
        } else {
            fflush(stdout);
            if(read_line(f_progress, n_stars) != OK){
                return NULL;
            }
            plevel->n_stars = atoi(n_stars);
            if (plevel->n_stars == 0 && act_level_flag == 0){
                act_level = plevel;
                act_level_flag = 1;
            }
            if (feof(f_progress)) k = 0;
        }

        plevel->next_level = NULL;
        if(i == 0){
            plevel_first = plevel;
            plevel_last = plevel;
        }else{
            plevel_last->next_level = plevel;
            plevel_last = plevel;
        }

        plevel->first_level = plevel_first;

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
            plevel->people[k] = create_people(64, spos, DESINTEGRATED, 3*k);
        }

        fclose(f_level);
    }
    fclose(f_progress);
    if (act_level_flag != 0){
        return act_level;
    } else {
        return plevel;
    }
} 

/* 
    Returns the state of the game 
 */
Level_result game_status(Level *level){
    int i, people_finished = 0;
    Level *l;
    FILE *f_progress;
    if (!level) return RES_ERROR;

    for (i = 0; i < level->num_people; i++){
        if(level->people[i]->state == FINISHED) people_finished++;
    }


    if (level->num_ladder_floor_act >= level->num_ladder_floor - level->num_ladder_floor_3
                    && level->num_ladder_act >= level->num_ladder - level->num_ladder_3
                    && level->num_floor_act >= level->num_floor - level->num_floor_3 
                    && level->portal_act >= level->portal - level->portal_3 
                    && people_finished >= level->min_people_3){

        if (level->num_ladder_floor_act > level->num_ladder_floor - level->num_ladder_floor_3 
                    || level->num_ladder_act > level->num_ladder - level->num_ladder_3
                    || level->num_floor_act > level->num_floor - level->num_floor_3 
                    || level->portal_act > level->portal - level->portal_3 
                    || people_finished > level->min_people_3){
            if (level->n_stars < 4){
                level->n_stars = 4;
                f_progress = fopen("progress.dat", "w");
                if (!f_progress){
                    printf("Error al abrir el archivo de progreso en game_status\n");
                    fflush(stdout);
                    return RES_ERROR;
                }
                l = level->first_level;
                fprintf(f_progress, "#Stars achieved in each level in order. 4 stars is SUPREME\n");
                while(l != NULL){
                    fprintf(f_progress, "%d\n", l->n_stars);
                    fflush(f_progress);
                    l = l->next_level;
                }
                fclose(f_progress);
            }
            return SUPREME;
        }

        if (level->n_stars < 3){
            level->n_stars = 3;
            f_progress = fopen("progress.dat", "w");
            if (!f_progress){
                printf("Error al abrir el archivo de progreso en game_status\n");
                fflush(stdout);
                return RES_ERROR;
            }
            l = level->first_level;
            fprintf(f_progress, "#Stars achieved in each level in order. 4 stars is SUPREME\n");
            while(l != NULL){
                fprintf(f_progress, "%d\n", l->n_stars);
                fflush(f_progress);
                l = l->next_level;
            }
            fclose(f_progress);
        }

        return STARS_3;


    } else if (level->num_ladder_floor_act >= level->num_ladder_floor - level->num_ladder_floor_2 
                    && level->num_ladder_act >= level->num_ladder - level->num_ladder_2
                    && level->num_floor_act >= level->num_floor - level->num_floor_2 
                    && level->portal_act >= level->portal - level->portal_2
                    && people_finished >= level->min_people_2){
        
        if (level->n_stars < 2){
            level->n_stars = 2;
            f_progress = fopen("progress.dat", "w");
            if (!f_progress){
                printf("Error al abrir el archivo de progreso en game_status\n");
                fflush(stdout);
                return RES_ERROR;
            }
            l = level->first_level;
            fprintf(f_progress, "#Stars achieved in each level in order. 4 stars is SUPREME\n");
            while(l != NULL){
                fprintf(f_progress, "%d\n", l->n_stars);
                fflush(f_progress);
                l = l->next_level;
            }
            fclose(f_progress);
        }

        return STARS_2;


    } else if (level->num_ladder_floor_act >= 0 
                    && level->num_ladder_act >= 0
                    && level->num_floor_act >= 0
                    && level->portal_act >= 0
                    && people_finished >= level->min_people){
        
        if (level->n_stars < 1){
            level->n_stars = 1;
            f_progress = fopen("progress.dat", "w");
            if (!f_progress){
                printf("Error al abrir el archivo de progreso en game_status\n");
                fflush(stdout);
                return RES_ERROR;
            }
            l = level->first_level;
            fprintf(f_progress, "#Stars achieved in each level in order. 4 stars is SUPREME\n");
            while(l != NULL){
                fprintf(f_progress, "%d\n", l->n_stars);
                fflush(f_progress);
                l = l->next_level;
            }
            fclose(f_progress);
        }

        return STARS_1;


    } else {
        return LOST;
    }
}

int read_record(){
    FILE *f = NULL;
    char line[10];
    int record;
    
    f = fopen("record.txt", "r+");
    if(f == NULL){
        f = fopen("record.txt", "w+");
        if(f == NULL) return -1;
        fprintf(f, "0");
        return 0;
    }

    read_line(f, line);
    record = atoi(line);

    fclose(f);
    return record;
}

Result save_record(int stars){
    FILE *f = NULL;
    int record = read_record();

    if(stars < record) return OK;

    f = fopen("record.txt", "w+");
    if(f == NULL) return ERROR;
    fprintf(f, "%d", stars);

    fclose(f);
    return OK;
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

void print_boxes_menu(Screen *screen, int a){
    Position map_pos, pos;
    int color = 124;

    if (a == 0){
        color = 255;
    }
    map_pos.x = screen->map_height/2 + 2; 
    map_pos.y = screen->map_width/2 - 30;
    pos = screen_position(map_pos, screen);
    print_box(pos, color, color, color, 17, 5, screen);

    change_color_rgb(0, 0, 0, 38);
    change_color_rgb(color, color, color, 48);
    pos.x += 2;
    pos.y += 2;
    change_cursor(pos, screen);
    printf("Repeat  level");


    if (a == 1){
        color = 255;
    } else {
        color = 124;
    }
    map_pos.y += 22;
    pos = screen_position(map_pos, screen);
    print_box(pos, color, color, color, 17, 5, screen);

    change_color_rgb(0, 0, 0, 38);
    change_color_rgb(color, color, color, 48);
    pos.x += 2;
    pos.y += 2;
    change_cursor(pos, screen);
    printf("Choose  level");


    if (a == 2){
        color = 255;
    } else {
        color = 124;
    }
    map_pos.y += 22;
    pos = screen_position(map_pos, screen);
    print_box(pos, color, color, color, 17, 5, screen);

    change_color_rgb(0, 0, 0, 38);
    change_color_rgb(color, color, color, 48);
    pos.x += 3;
    pos.y += 2;
    change_cursor(pos, screen);
    printf("Next  level");
}

int get_actual_level(Level *first_level, Level *level){
    int i = 0;
    Level *tmp = first_level;

    while(tmp->next_level){
        if(tmp == level) return i;
        tmp = tmp->next_level;
        i++;
    }

    return -1;
}

int get_total_stars(Level *first_level){
    int t = 0;
    Level *tmp = first_level;
    for(int i = 0; i < NUM_LEVELS; i++){
        t += tmp->n_stars;
        tmp = tmp->next_level;
    }
    save_record(t);
    return t;
}

/*
  012345678
0 ---------
1 |   5   |
2 | * * * |
3 ---------
*/


void draw_choosing_menu(Level * first_level, Screen *screen, int selected){
    int pages, cols, rows, n_per_page, sel_page, sel_col, sel_row, in_page, stars;
    int w = 9, h = 4;
    Position pos;
    Level *tmp = first_level;
    char instructions[100] = "Choose your level and press ENTER or press Q to quit the game";
    char end_message[100];

    cols = (screen->screen_width - 4) / h;
    rows = (screen->screen_height - 7) / w;
    n_per_page = cols * rows;
    pages = NUM_LEVELS % n_per_page == 0 ? NUM_LEVELS / n_per_page : NUM_LEVELS / n_per_page + 1;

    sel_page = selected / n_per_page;
    sel_row = (selected - sel_page * n_per_page) / rows;
    sel_col = selected - sel_page * n_per_page - sel_row * rows;

    in_page = (sel_page == pages - 1) ? NUM_LEVELS - n_per_page * (pages - 1) : n_per_page;

    pos.x = (screen->screen_width - strlen(instructions)) / 2;
    pos.y = 2;
    change_cursor(pos, screen);
    printf(instructions);

    for(int i = 0; i < sel_page * n_per_page; i++) tmp = tmp->next_level;

    for(int i = 0; i < in_page; i++){
        if(sel_page * n_per_page + i == selected) change_color("white", "black");

        pos.x = (i % cols) * w + 2;
        pos.y = (i / cols) * h + 4;
        change_cursor(pos, screen);
        for(int j = 0; j < w; j++) printf("-");

        pos.y++;
        change_cursor(pos, screen);
        printf("|");
        for(int j = 0; j < (w - 5) / 2; j++) printf(" ");
        printf("%03d", sel_page * n_per_page + i + 1);
        for(int j = 0; j < (w - 5) / 2; j++) printf(" ");
        printf("|");

        pos.y++;
        change_cursor(pos, screen);
        printf("|");
        for(int j = 0; j < (w - 7) / 2; j++) printf(" ");
        switch (tmp->n_stars)
        {
        case 0:
            printf("     ");
            break;
        case 1:
            printf(" * * ");
            break;
        case 2:
            printf("* * *");
            break;
        case 3:
            printf("  *  ");
            break;
        case 4:
            printf(" S S ");
            break;
        }
        for(int j = 0; j < (w - 7) / 2; j++) printf(" ");
        printf("|");

        pos.y++;
        change_cursor(pos, screen);
        for(int j = 0; j < w; j++) printf("-");

        if(sel_page * n_per_page + i == selected) change_color("reset", "reset");
        
        tmp = tmp->next_level;
    }

    stars = get_total_stars(first_level);
    sprintf(end_message, "Total stars %d, record %d", stars, read_record());
    pos.x = (screen->screen_width - strlen(end_message)) / 2;
    pos.y = screen->screen_height - 1;
    change_cursor(pos, screen);
    printf(end_message);
}

Level *get_selected_level(Level *first_level, int selected){
    Level *tmp = first_level;
    for(int i = 0; i < selected; i++) tmp = tmp->next_level;
    return tmp;
}

int get_completed_levels(Level *first_level){
    Level *tmp = first_level;
    for(int i = 0; i < NUM_LEVELS; i++){
        if(tmp->n_stars == 0) return i;
        tmp = tmp->next_level;
    }
    return NUM_LEVELS - 1;
}

Level *choose_level_menu(Level *first_level, Level *level, Screen *screen){
    int actual_lvl, selected, completed;
    char c;

    change_color("reset", "reset");
    clear_screen();

    actual_lvl = get_actual_level(first_level, level);
    selected = actual_lvl;
    completed = get_completed_levels(first_level);

    draw_choosing_menu(first_level, screen, selected);
    while(TRUE){
        tcflush(fileno(stdin), TCIFLUSH);
        c = read_key();
        switch(c) {
        case('a'):
            if(selected > 0) selected--;
            break;
        case('d'):
            if(selected < completed) selected++;
            break;
        case('q'):
            clear_screen();
            print_margins(screen);
            return NULL;
        case('\n'):
            clear_screen();
            print_margins(screen);
            return get_selected_level(first_level, selected);
        }
        draw_choosing_menu(first_level, screen, selected);
    }
}

Level *level_menu(Level *level, Screen *screen, Level *first_level){
    Position map_pos, pos;
    char key;
    int a, n = 3;

    printf("\e[?25l");

    map_pos.x = screen->map_height/2 + 8;   
    map_pos.y = screen->map_width/2 - 37;
    pos = screen_position(map_pos, screen);
    print_file("./designs/star_background.txt", pos, screen, FALSE);    

    if(level->n_stars < 3 || level->level_number == NUM_LEVELS){
        a = 0;
        if (level->n_stars == 0){
            n = 2;
        }
    } else {
        a = 2;
    }

    do{
        print_boxes_menu(screen, a);
        tcflush(fileno(stdin), TCIFLUSH);
        key = read_key();

        if (key == 'd'){
            a = (a + 1) % n;
        } else if (key == 'a'){
            if (a == 0) a = n - 1;
            else a = (a - 1) % n;
        }
    } while (key != '\n');

    if (a == 0){
        return level;
    } else if(a == 1){
        return choose_level_menu(first_level, level, screen);
    }else if (a == 2){
        return level->next_level;
    }

    return level;
}

void free_level(Level* first_level, Screen * screen){
    Level *next_level;
    if(!screen) return;
    while(first_level){
        next_level = first_level->next_level;
        free_map(first_level->map, screen);
        free_people(first_level);
        free(first_level);
        first_level = next_level;
    }
}




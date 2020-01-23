#include "level.h"
#include "print_on_screen.h"
#include "map.h"
#include "read_keys.h"
#include "read_from_file.h"

#include <string.h>
#include <stdlib.h>
#include <termios.h>

/*
    Input:
        - *screen: pointer to main screen
    Outputs:
        Level *: pointer to the current level
        levels act as a foward linked list
        each level has a pointer to the first level just in case 
        the current level is not the first one
    Description:
        Goes through all the files named level**.txt inside the level_files folder
        for each level initializes a level structure setting all it's values appropiately
        and links each level to the next one creating a linked list structure
        NOTES:
            - The number of levels must be set in the NUM_LEVELS macro at level.h
            - The map inside each level is created from a file wich name shold be especified in
            the corresponding level**.txt file
            - The n_stars property is set from the progress.dat file wich is created and initialized
            to 0 if it didn't exist
            - The level number property acts as an index but it starts at 1 so that it can be printed
            in a user friendly way
            - A double pointer to the persons for the level is also created and initialized inside
            this function
*/
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
    
    // Open the progress.dat file if it exists of create one if it doesn't
    f_progress = fopen("progress.dat", "r+");
    if(!f_progress){
        f_progress = fopen("progress.dat", "w");
        if (!f_progress) {
            print_message(screen, "Failed to save the game (progress.dat file couldn't be created)");
            return NULL;
        }
        fprintf(f_progress, "#Stars achieved in each level in order. 4 stars is SUPREME\n");
        k = 0;
    }
    
    // Initialize all levels
    for(int i = 0; i < NUM_LEVELS; i++){
        // Open it's file
        sprintf(folder_name, "./level_files/");
        sprintf(level_file, "./level_files/level%d.txt", i+1);
        f_level = fopen(level_file, "r");
        if(f_level == NULL){
            if(plevel) free(plevel);
            return NULL;
        } 

        // Initialize the structure
        plevel = (Level*)malloc(sizeof(Level));
        if(plevel == NULL) return NULL;

        // Set it's n_stars property from the progress.dat file
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

        // link levels
        plevel->next_level = NULL;
        if(i == 0){
            plevel_first = plevel;
            plevel_last = plevel;
        }else{
            plevel_last->next_level = plevel;
            plevel_last = plevel;
        }
        plevel->first_level = plevel_first;

        // read all levels properties from it's file
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

        // Get map file name
        if(read_line(f_level,map_file) != OK){
            return NULL;
        }
        strcat(folder_name, map_file);

        plevel->map = create_map(folder_name, screen); // initialize the map
        if(plevel->map == NULL) return NULL;

        // initialize the People** structure
        plevel->people = (People**)malloc(sizeof(People*)*plevel->num_people);
        if(plevel->people == NULL) return NULL;

        spos = screen_position(*(plevel->map->Start_pos), screen);
        
        for (int k = 0; k < plevel->num_people; k++){
            plevel->people[k] = create_people(64, spos, DESINTEGRATED, 3*k);
        }

        // close level file
        fclose(f_level);
    }
    // close progress file
    fclose(f_progress);

    //return file
    if (act_level_flag != 0){
        return act_level;
    } else {
        return plevel;
    }
} 

/*
    Input:
        - *level: pointer to the current level
    Outputs:
        Flag indicating if the player has lost or the number of stars won
    Description:
        Checks how many resources the player has spent and how many people
        have reached the end. Based on that determines if the player has
        lost and the number of stars won if he didn't lose
        Also saves the won stars in the progress.dat file
*/
Level_result game_status(Level *level){
    int i, people_finished = 0;
    Level *l;
    FILE *f_progress;
    if (!level) return RES_ERROR;

    // check how many people finished
    for (i = 0; i < level->num_people; i++){
        if(level->people[i]->state == FINISHED) people_finished++;
    }

    if (level->people[i - 1]->state != FINISHED){
        return LOST;
    }

    // determine the number of won stars and save the progress file
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


/* PRIVATE FUNCTION
    Outputs:
        int indicting the current stars record
        -1 in case of error
    Description:
        Opens the record file and reads the current record
        if it doesn't exsist creates one and sets the record to 0
*/
int read_record(){
    FILE *f = NULL;
    char line[10];
    int record;
    
    // Open file
    f = fopen("record.txt", "r+");
    if(f == NULL){
        f = fopen("record.txt", "w+");
        if(f == NULL) return -1;
        fprintf(f, "0");
        return 0;
    }

    // read record
    read_line(f, line);
    record = atoi(line);

    // close file and return record
    fclose(f);
    return record;
}

/* PRIVATE FUNCTION
    Inputs:
        stars: number of stars to be compared to the record
    Outputs:
        flag indicating if an error happened
    Description:
        Reads the current record and if given number of stars
        is greater than the record overwrites the record file
*/
Result save_record(int stars){
    FILE *f = NULL;
    int record = read_record();

    if(record < 0) return ERROR;
    if(stars < record) return OK;

    // Overwrite file
    f = fopen("record.txt", "w+");
    if(f == NULL) return ERROR;
    fprintf(f, "%d", stars);

    fclose(f);
    return OK;
}

/* PRIVATE FUNCTION
    Inputs:
        - *screen: pointer to main screen
        - a: int for the selected option
    Outputs:
        int indicting the current stars record
    Description:
        Displays the |Repeat level / Choose level / Next level| menu
        Displays the selected option in a different color
*/
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

    // Repeat level box
    change_color_rgb(0, 0, 0, 38);
    change_color_rgb(color, color, color, 48);
    pos.x += 2;
    pos.y += 2;
    change_cursor(pos, screen);
    printf("Repeat  level");

    // Choose level box
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

    // Next level box
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

/* PRIVATE FUNCTION
    Inputs:
        - *first_level: pointer to the first_level
        - *level: pointer to the current level
    Outputs:
        int corresponding to the index of the current level
    Description:
        Iterates through the level linked list until
        it finds the current level
*/
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

/* PRIVATE FUNCTION
    Inputs:
        - *first_level: pointer to the first_level
    Outputs:
        int corresponding to the total number of stars
        won by the player
    Description:
        Iterates through the level linked list adding all
        stars from all levels and saves the record
*/
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

/* PRIVATE FUNCTION
    Inputs:
        - *first_level: pointer to the first_level
        - *screen: pointer to main screen
        - selected: index of the selected level
    Description:
        Displays the menu for selecting a level given the level that
        is currently selected.
        The selected level is displayed in a different color
        If the nuber of levels overflows the number of levels that fit
        int the screen the levels are split into pages
        To change between pages the selected level needs to be in the 
        desired page
        For each level show it index + 1 and the number of stars
        Example: box for level 5 with 3 stars
          012345678
        0 ---------
        1 |   5   |
        2 | * * * |
        3 ---------
        The display of the number of stars goes as follows:
            1 star :   *   
            2 stars:  * *  
            3 stars: * * * 
            4 stars:  S S  (Supreme)
*/
void draw_choosing_menu(Level * first_level, Screen *screen, int selected){
    int pages; // total number of pages
    int cols, rows; // total number of columns and rows per page
    int n_per_page; // max number of levels displayed per page
    int sel_page, sel_col, sel_row; // page, column and row of the selected level
    int in_page; // number of levels displayed in the selected page
    int stars; // total stars
    int w = 9, h = 4; // width an height of each box
    Position pos;
    Level *tmp = first_level;
    char instructions[100] = "Choose your level and press ENTER or press Q to quit the game";
    char end_message[100];

    //set all variables
    cols = (screen->screen_width - 4) / w;
    rows = (screen->screen_height - 7) / h;
    n_per_page = cols * rows;
    pages = NUM_LEVELS % n_per_page == 0 ? NUM_LEVELS / n_per_page : NUM_LEVELS / n_per_page + 1;

    sel_page = selected / n_per_page;
    sel_row = (selected - sel_page * n_per_page) / rows;
    sel_col = selected - sel_page * n_per_page - sel_row * rows;

    in_page = (sel_page == pages - 1) ? NUM_LEVELS - n_per_page * (pages - 1) : n_per_page;

    // print instructions
    pos.x = (screen->screen_width - strlen(instructions)) / 2;
    pos.y = 2;
    change_cursor(pos, screen);
    printf("%s", instructions);

    // get tmp to point to the first level to be displayed
    for(int i = 0; i < sel_page * n_per_page; i++) tmp = tmp->next_level;
    // draw the box for each level
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
            printf("  *  ");
            break;
        case 2:
            printf(" * * ");
            break;
        case 3:
            printf("* * *");
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

    //print the message at the bottom of the screen
    stars = get_total_stars(first_level);
    sprintf(end_message, "Total stars %d, record %d", stars, read_record());
    pos.x = (screen->screen_width - strlen(end_message)) / 2;
    pos.y = screen->screen_height - 1;
    change_cursor(pos, screen);
    printf("%s", end_message);
}

/* PRIVATE FUNCTION
    Inputs:
        - *first_level: pointer to the first_level
        - selected: integer indicationg the index
                    of the selected level
    Outputs:
        Level *: pointing to the level with the given index
    Description:
        Iterates through the level linked list 
        and returns the appropiate pointer
*/
Level *get_selected_level(Level *first_level, int selected){
    Level *tmp = first_level;
    for(int i = 0; i < selected; i++) tmp = tmp->next_level;
    return tmp;
}

/* PRIVATE FUNCTION
    Inputs:
        - *first_level: pointer to the first_level
    Outputs:
        int corresponding to the number of completed levels
    Description:
        Iterates through the level linked list and stops
        when it finds an uncopleted level
*/
int get_completed_levels(Level *first_level){
    Level *tmp = first_level;
    for(int i = 0; i < NUM_LEVELS; i++){
        if(tmp->n_stars == 0) return i;
        tmp = tmp->next_level;
    }
    return NUM_LEVELS - 1;
}

/* PRIVATE FUNCTION
    Inputs:
        - *first_level: pointer to the first_level
        - *level: pointer to the current level
        - *screen: pointer to the main screen
    Outputs:
        Level *: pointer to the level chosen by the player
        NULL it the player exits
    Description:
        Draws the choosing level menu and waits for a user
        input in a loop until the player hits enter to 
        choose a level of q to quit the game
        The selected level defaults to the current level
        It doesn't let the user select an uncompleted level
        other than the first uncompleted level
*/
Level *choose_level_menu(Level *first_level, Level *level, Screen *screen){
    int actual_lvl, selected, completed;
    char c;

    change_color("reset", "reset");
    clear_screen();

    actual_lvl = get_actual_level(first_level, level);
    selected = actual_lvl;
    completed = get_completed_levels(first_level);

    //draw the menu for the first time
    draw_choosing_menu(first_level, screen, selected);
    while(TRUE){
        tcflush(fileno(stdin), TCIFLUSH); // clear input buffer
        // wait for input and process the input
        c = read_key();
        switch(c) {
        case('a'):
            if(selected > 0) selected--;
            break;
        case('d'):
            if(selected < completed) selected++;
            break;
        case('q'): // quit game
            clear_screen();
            print_margins(screen);
            return NULL;
        case('\n'): // select level
            clear_screen();
            print_margins(screen);
            return get_selected_level(first_level, selected);
        }
        //redraw the menu
        draw_choosing_menu(first_level, screen, selected);
    }
}

/*
    Inputs:
        - *level: pointer to the current level
        - *screen: pointer to the main screen
        - *first_level: pointer to the first_level
    Outputs:
        Level *: pointer to the level chosen by the player
        NULL it the player exits
    Description:
        Let's the player repeat the level, open the choose level menu
        or go to the next level if he has won at least 1 star
        returns a pointer to the level chosen by the player of NULL
        it the player exits the game
*/
Level *level_menu(Level *level, Screen *screen, Level *first_level){
    Position map_pos, pos;
    char key;
    int a, n = 3;

    disable_terminal_cursor();

    // print background
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
        //print menu
        print_boxes_menu(screen, a);
        tcflush(fileno(stdin), TCIFLUSH); //clear the input buffer

        // get the input and process it
        key = read_key();
        if (key == 'd'){
            a = (a + 1) % n;
        } else if (key == 'a'){
            if (a == 0) a = n - 1;
            else a = (a - 1) % n;
        }
    } while (key != '\n');

    // return the corresponding level or open the choose level menu
    if (a == 0){
        return level;
    } else if(a == 1){
        return choose_level_menu(first_level, level, screen);
    }else if (a == 2){
        return level->next_level;
    }

    return level;
}

/*
    Inputs:
        - *first_level: pointer to the first_level
        - *screen: pointer to the main screen
    Description:
        Frees the level structure
*/
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




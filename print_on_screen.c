#include "print_on_screen.h"
#include "read_from_file.h"
#include "map.h"
#include "level.h"
#include <string.h>
#include <termios.h>


#define FG 38
#define BK 48

struct termios initial;

void _term_init();

/*
    Changes the color you are printing with to the color given by the string argument
*/
int change_color(char *background_color, char *foreground_color){
    char *foreground_color_code = "40";
    char *background_color_code = "30";

    if(!strcmp(background_color, "black")) background_color_code = "40";
    else if(!strcmp(background_color, "red")) background_color_code = "41";
    else if(!strcmp(background_color, "green")) background_color_code = "42";
    else if(!strcmp(background_color, "yellow")) background_color_code = "43";
    else if(!strcmp(background_color, "blue")) background_color_code = "44";
    else if(!strcmp(background_color, "magenta")) background_color_code = "45";
    else if(!strcmp(background_color, "cyan")) background_color_code = "46";
    else if(!strcmp(background_color, "white")) background_color_code = "47";
    else if(!strcmp(background_color, "reset")) background_color_code = "0";

    if(!strcmp(foreground_color, "black")) foreground_color_code = "30";
    else if(!strcmp(foreground_color, "red")) foreground_color_code = "31";
    else if(!strcmp(foreground_color, "green")) foreground_color_code = "32";
    else if(!strcmp(foreground_color, "yellow")) foreground_color_code = "33";
    else if(!strcmp(foreground_color, "blue")) foreground_color_code = "34";
    else if(!strcmp(foreground_color, "magenta")) foreground_color_code = "35";
    else if(!strcmp(foreground_color, "cyan")) foreground_color_code = "36";
    else if(!strcmp(foreground_color, "white")) foreground_color_code = "37";
    else if(!strcmp(foreground_color, "reset")) foreground_color_code = "0";

    printf("%c[%s;1m", 27, background_color_code);
    return printf("%c[%s;1m", 27, foreground_color_code);
}

void change_color_rgb(int r, int g, int b, int bf){
    if(r < 0) r = 0;
    if(g < 0) g = 0;
    if(b < 0) b = 0;

    if(r > 255) r = 255;
    if(g > 255) g = 255;
    if(b > 255) b = 255;
    
    fprintf(stdout, "%c[%d;%d;%d;%d;%dm", 27, bf, 2, r, g, b);
    fflush(stdout);
    return;
}

/*
    Changes the custor to the given position
*/
int change_cursor(Position position, Screen *screen){
    int x, y;
    
    if (position.x < 0 || position.x > screen->screen_width || position.y < 2 || position.y > screen->screen_height + 1){
        return -1;
    } else {  
        x = position.x;
        y = position.y;
        //screen->cursor = position;
        return printf ("%c[%d;%dH", 27, y, x);
    }
    printf("Error en change_cursor()\n");
    fflush(stdout);
    return -1;
}

/*
    Clears screen
*/
Result clear_screen(){
    printf("%c[2J", 27);
    return OK;
}

/*
    Initialize screen
*/
Screen *init_screen(char *file_name){
    //Open file
    char line[MAX_SIZE];
    char title[MAX_SIZE];
    FILE *f;
    Result r;
    Screen *screen = NULL;
    
    screen = (Screen *)malloc(sizeof(Screen));
    if(!screen) return NULL;

    f = fopen(file_name, "r");
    if(f == NULL){
        free(screen);
    }

    //Initialice screen variables
    r = read_line(f, &title);
    if(r == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }

    r = read_line(f, &line);
    if(r == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->screen_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->screen_height = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->map.x = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->map.y = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) {
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->map_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->map_height = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) {
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->messagebox.x = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->messagebox.y = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) {
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->messagebox_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }
    screen->messagebox_height = atoi(line);
    screen->cursor.x = 10;
    screen->cursor.y = 10;

    //Set screen size
    _term_init(screen);
    change_color("reset", "reset");
    printf("%c[8;%d;%dt", 27, screen->screen_height, screen->screen_width);
    clear_screen();
    if(start_screen(screen) == ERROR) {
        free(screen);
        fclose(f);
        return NULL;
    }
    if(print_margins(screen) == ERROR){
        free(screen);
        fclose(f);
        return NULL;
    }
    printf("\n");
    print_title(screen, title);
    return screen;
}

Result start_screen(Screen *screen){
    Position pos;
    pos.x = 0;
    pos.y = 2;
    if(print_file("./designs/start_screen.txt", pos, screen, TRUE) == ERROR) return ERROR;
    read_key();
    clear_screen();
    return OK;
}

Result restore_screen(Screen *screen){
    Position final_pos;
    if(!screen) return ERROR;
    final_pos.x = 0;
    final_pos.y = screen->screen_height+1;
    change_color("reset", "reset");
    change_cursor(final_pos,screen);
    tcsetattr(fileno(stdin), TCSANOW, &initial);
    free(screen);
    return OK;
}

Result print_title(Screen *screen, char *title){
    Position pos;
    int l = strlen(title);
    pos.y = 3;
    pos.x = (screen->screen_width  - strlen(title))/ 2 - 2;
    change_color("red", "white");
    change_cursor(pos, screen);
    for(int i = 0; i < l + 3; i++)printf(" ");

    pos.y++;
    change_cursor(pos, screen);
    printf("  ");
    for(int i = 0; i < l-1; i++)printf("%c", title[i]);
    printf("  ");

    pos.y++;
    change_cursor(pos, screen);
    for(int i = 0; i < l + 3; i++)printf(" ");
    return OK;
}

/*
    Prints the magins between the map and the message boxes
*/
Result print_margins2(FILE *f, Screen *screen){
    char line[MAX_SIZE];
    Result r;

    Position pos;
    pos.x = 0;
    pos.y = 2;
    change_cursor(pos,screen);
    do{
        r = read_line(f, &line);
        printf(line);

        if(r == ERROR){
            printf("error reading file");
            return ERROR;
        }
    }while(!feof(f));    
}

/* 
    Prints the margins between the map and the message boxes without a file
*/
Result print_margins(Screen *screen){
    int i=0, k;
    Position pos;
    pos.x = 0;
    pos.y = 2;
    change_cursor(pos, screen);

    change_color("reset", "reset");
    printf("+");
    /*
        +------+
    */

    for (i = 1; i < screen->screen_width - 1; i++){
        printf("-");
    }
    printf("+");
    /*
        +------+
        |
        |
        +
        funciona
    */
    pos.x = 0;
    pos.y = 2;
    printf("+");
    for(i = 1; i < screen->screen_height - 3; i++){
        pos.x = 0;
        pos.y++;
        change_cursor(pos, screen);
        printf("|");
    }
    pos.x = 0;
    pos.y++;
    change_cursor(pos, screen);
    printf("+");
    /*
        +------+
        |
        |
        +------+
        Funciona
    */
    for (i = 1; i < screen->screen_width - 1; i++){
       printf("-");
    }
    printf("+");
    /*
        +------+
        |      | 
        |      |
        +------+
        Funciona
    */
    pos.x = screen->screen_width;
    pos.y = 2;
    for (i = 1; i < screen->screen_height - 3; i++){
       pos.y++;
       change_cursor(pos,screen);
       printf("|");
    }

    /*
        +------+
        +------+ 
        |      |
        +------+
        Funciona
    */
    pos.y = screen->map.y - 1;
    pos.x = 0;
    change_cursor(pos, screen);
    printf("+");
    for(i = 1; i < screen->screen_width - 1; i++){
        printf("-");
    }
    printf("+");

    /*
        +------+
        +------+ 
        |      |
        +------+
        +------+
        Funciona
    */
    pos.y = screen->messagebox.y - 1;
    pos.x = 0;
    change_cursor(pos, screen);
    printf("+");
    for(i = 1; i < screen->screen_width - 1; i++){
        printf("-");
    }
    printf("+");

    /*
        +------+
        +--+---+ 
        |  |   |
        +--+---+
        +------+
    */
    pos.y = screen->map.y - 1;
    pos.x = screen->map.x - 1;
    change_cursor(pos, screen);
    printf("+");
    for(i = 1; i < screen->map_height + 2; i++){
        pos.y++;
        change_cursor(pos,screen);
        printf("|");
    }
    change_cursor(pos,screen);
    printf("+");

    return OK;
}
//celdas
//falta comprobar errores
Result print_map (Box **map, Screen *s) {
    int i, j;
    Position p;
    if (!map || !s) {
        return ERROR;
        printf("Error en los argumentos");
    }
    
    p = s->map;
    
    if (change_cursor(p, s) < 0) return ERROR;
    printf("\e[?25l");
    for (i = s->map_height - 1; i >= 0; i--) {
        for (j = 0; j < s->map_width; j++) {
            switch (map[i][j]){
                case AIR:
                change_color("reset", "reset");
                    fprintf(stdout, " ");
                    break;
                case WALL:
                    change_color("white", "black");
                    fprintf(stdout, " ");
                    break;
                case WALL_MERGE:
                    change_color_rgb(124, 124, 124, BK);
                    fprintf(stdout, " ");
                    break;
                case START:
                    change_color("green", "black");
                    fprintf(stdout, " ");
                    break;
                case END:
                    change_color("cyan", "black");
                    fprintf(stdout, " ");
                    break;
                case LAVA:
                    change_color("red", "black");
                    fprintf(stdout, " ");
                    break;
                case PORTALA:
                    change_color("reset", "magenta");
                    fprintf(stdout, "O");
                    break;
                case PORTALB:
                    change_color("reset", "yellow");
                    fprintf(stdout, "O");
                    break;
                case LADDER:
                    change_color("reset", "white");
                    fprintf(stdout, "#");
                    break;
                case ZONAPORTAL:
                    change_color("reset", "magenta");
                    fprintf(stdout, "·");
                    break;
                case DISAPPEAR_WALL:
                    change_color_rgb(60,60,60, BK);
                    fprintf(stdout, " ");
                    break;
                default:
                    change_color("black", "red");
                    fprintf(stdout, "X");
                    break;
            }
        }
        (p.y)++;
        if (change_cursor(p, s) < 0) return ERROR;
    }
    printf("\e[?25h");
    return OK;
}


Result print_message(Screen *screen, char *text){
    int end_flag = 0;
    int w = screen->messagebox_width - 1;
    int h = screen->messagebox_height;
    Position pos = screen->messagebox;
	
	change_color("reset", "reset");
    //Clear the messagebox
    change_cursor(pos,screen);
    for(int i=0; i < h; i++){
        for(int j=0; j < w; j++){
            printf(" ");
        }
        pos.y += 1;
        change_cursor(pos,screen);
    }
    //Print the message croping it to the right dimensions
    pos = screen->messagebox;
    change_cursor(pos,screen);
    for(int i=0; i < h; i++){
        for(int j=0; j < w; j++){
            if(text[i*w + j] == (char)0){
                end_flag=1;
                break;
            }
            printf("%c", text[i*w + j]);
        }
        if(end_flag) break;
        pos.y += 1;
        change_cursor(pos,screen);
    }
}

Result print_resources_line(char *line, Position *p, Screen *screen){
    int w = screen->screen_width - screen->map_width - 3;
    int l = strlen(line);
    // take the minimum between the width of the box and the length of the line
    int m = w < l ? w : l; 
    
    change_cursor(*p, screen);
    for(int i = 0; i < m; i++)printf("%c", line[i]);
    for(int i = m; i < w; i++)printf(" ");

    (p->y)++;
    return OK;
}

Result print_resources(Screen *screen, Level *level){
    char array[256]; //Sirve para pasar los valores de los resources a print_resource
    Position res_pos, pos;
    int h = screen->map_height;
    res_pos.x = 2;
    res_pos.y = (screen->map).y;

    //Clear the resources box
    change_color_rgb(110, 31, 18, BK);
    for(int i=0; i < h; i++){
        print_resources_line(" ", &res_pos, screen);
    }
    res_pos.x = 2;
    res_pos.y = (screen->map).y;
	
    //Print the resources and instrucctions
    pos = res_pos;

    change_color_rgb(171, 147, 106, BK);
    print_resources_line(" ", &pos, screen);
    print_resources_line("RESOURCES:", &pos, screen);
    print_resources_line(" ", &pos, screen);
    
    if(level->num_ladder_floor > 0){
        sprintf(array,"LADDERS/FLOORS (l/f): x%d", level->num_ladder_floor_act);
        print_resources_line(array, &pos, screen);
    }

    if(level->num_ladder > 0){
        sprintf(array,"LADDERS (l): x%d", level->num_ladder_act);
        print_resources_line(array, &pos, screen);
    }

    if(level->num_floor > 0){
        sprintf(array," FLOORS (f): x%d", level->num_floor_act);
        print_resources_line(array, &pos, screen);
    }

    if(level->portal > 0){
        sprintf(array,"PORTAL (p): x%d", level->portal_act);
        print_resources_line(array, &pos, screen);
    }
    print_resources_line(" ", &pos, screen);
    print_resources_line(" ", &pos, screen);

    
    // 2 starts
    change_color_rgb(184, 141, 68, BK);
    print_resources_line("2 stars:", &pos, screen);
    
    if(level->num_ladder_floor_2 > 0){
        sprintf(array,"LADDERS/FLOORS: x%d", level->num_ladder_floor_2);
        print_resources_line(array, &pos, screen);
    }

    if(level->num_ladder_2 > 0){
        printf(array,"LADDERS: x%d", level->num_ladder_2);
        print_resources_line(array, &pos, screen);
    }

    if(level->num_floor_2 > 0){
        sprintf(array,"FLOORS: x%d", level->num_floor_2);
        print_resources_line(array, &pos, screen);
    }

    if(level->portal_2 > 0){
        sprintf(array,"PORTALS: x%d", level->portal_2);
        print_resources_line(array, &pos, screen);
    }

    // 3 starts
    change_cursor(pos, screen);
    change_color_rgb(143, 100, 29, BK);
    print_resources_line("3 stars use:", &pos, screen);
    
    if(level->num_ladder_floor_3 > 0){
        sprintf(array,"LADDERS/FLOORS: x%d", level->num_ladder_floor_3);
        print_resources_line(array, &pos, screen);
    }

    if(level->num_ladder_3 > 0){
        sprintf(array,"LADDERS: x%d", level->num_ladder_3);
        print_resources_line(array, &pos, screen);
    }

    if(level->num_floor_3 > 0){
        sprintf(array,"FLOORS: x%d", level->num_floor_3);
        print_resources_line(array, &pos, screen);
    }

    if(level->portal_3 > 0){
        sprintf(array,"PORTALS: x%d", level->portal_3);
        print_resources_line(array, &pos, screen);
    }
    
    //Instructions
    change_color_rgb(110, 31, 18, BK);

    print_resources_line(" ", &pos, screen);
    print_resources_line(" ", &pos, screen);
    print_resources_line(" ", &pos, screen);
    print_resources_line("Press ENTER to play", &pos, screen);

    print_resources_line(" ", &pos, screen);
    print_resources_line(" ", &pos, screen);
    print_resources_line("Press DEL to delete", &pos, screen);

    return OK;
}

void supreme_animation(Screen *screen){
    int n, spacing, r, w = screen->map_width, h = screen->map_height;
    Position map_pos, pos;

    for(int i = 15; i > 0; i--){
        n = w / 17;
        spacing = (w % 17)/(n - 1);

        /* Print the background */
        map_pos.x = screen->map_height/2 + 7;   
        map_pos.y = screen->map_width/2 - 36;
        pos = screen_position(map_pos, screen);
        print_file("./designs/star_background_reduced.txt", pos, screen, FALSE);
        /* Print the stars on screen */
        map_pos.x = screen->map_height/2 + 4;   
        map_pos.y = screen->map_width/2 - 28;
        pos = screen_position(map_pos, screen);
        r = rand()%2;
        if(r){
            print_file("./designs/empty_star.txt", pos, screen, FALSE);
        } else {
            print_file("./designs/star.txt", pos, screen, FALSE);
        }
        map_pos.y = screen->map_width/2 - 8;
        pos = screen_position(map_pos, screen);
        r = rand()%2;
        if(r){
            print_file("./designs/empty_star.txt", pos, screen, FALSE);
        } else {
            print_file("./designs/star.txt", pos, screen, FALSE);
        }
        map_pos.y = screen->map_width/2 + 12;
        pos = screen_position(map_pos, screen);
        r = rand()%2;
        if(r){
            print_file("./designs/empty_star.txt", pos, screen, FALSE);
        } else {
            print_file("./designs/star.txt", pos, screen, FALSE);
        }

        /* print top row: */
        map_pos.x = h - 1;
        map_pos.y = 1;
        pos = screen_position(map_pos, screen);
        while (n > 0){
            r = rand()%2;
            if(r){
                print_file("./designs/empty_star.txt", pos, screen, FALSE);
            } else {
                print_file("./designs/star.txt", pos, screen, FALSE);
            }
            pos.x += (17 + spacing);
            n--;
        }

        /* Print bottom row: */
        n = w / 17;

        map_pos.x = 8;
        map_pos.y = 1;
        pos = screen_position(map_pos, screen);
        
        while (n > 0){
            r = rand()%2;
            if(r){
                print_file("./designs/empty_star.txt", pos, screen, FALSE);
            } else {
                print_file("./designs/star.txt", pos, screen, FALSE);
            }
            pos.x += (17 + spacing);
            n--;
        }
    
        fflush(stdout);
        usleep(100*1000);
    }

}

Result level_end(Level_result res, Screen *screen){
    Position map_pos, pos;

    if(!screen){
        return ERROR;
    }
    printf("\e[?25l");
    switch(res){
            case LOST:
                print_message(screen, "Level not completed");
                fflush(stdout);
                break;
            case STARS_1:
                print_message(screen, "Level completed with 1 star");
                fflush(stdout);
                break;
            case STARS_2:
                print_message(screen, "Level completed with 2 stars");
                fflush(stdout);
                break;
            case STARS_3:
                print_message(screen, "Level completed with 3 stars");
                fflush(stdout);
                break;
            case SUPREME:
                print_message(screen, "You found a better way to solve the level than what the developers of the game thought!!!");
                fflush(stdout);
                break;
        }


        if (res == SUPREME){
                supreme_animation(screen);
        } else {
            /* Print the background */
            map_pos.x = screen->map_height/2 + 8;   
            map_pos.y = screen->map_width/2 - 36;
            pos = screen_position(map_pos, screen);
            print_file("./designs/star_background.txt", pos, screen, FALSE);
            /* Print the stars on screen */
            map_pos.x = screen->map_height/2 + 5;   
            map_pos.y = screen->map_width/2 - 28;
            pos = screen_position(map_pos, screen);
            print_file("./designs/empty_star.txt", pos, screen, TRUE);
            map_pos.y = screen->map_width/2 - 8;
            pos = screen_position(map_pos, screen);
            print_file("./designs/empty_star.txt", pos, screen, TRUE);
            map_pos.y = screen->map_width/2 + 12;
            pos = screen_position(map_pos, screen);
            print_file("./designs/empty_star.txt", pos, screen, TRUE);
            fflush(stdout);
            

            if (res == SUPREME){
                supreme_animation(screen);
            } else if (res != LOST){
                usleep(600*1000);
                map_pos.x = screen->map_height/2 + 5;   
                map_pos.y = screen->map_width/2 - 28;
                pos = screen_position(map_pos, screen);
                print_file("./designs/star.txt", pos, screen, TRUE);
                fflush(stdout);

                if(res != STARS_1){
                    usleep(600*1000);
                    map_pos.y = screen->map_width/2 - 8;
                    pos = screen_position(map_pos, screen);
                    print_file("./designs/star.txt", pos, screen, TRUE);
                    fflush(stdout);

                    if(res != STARS_2){
                        usleep(600*1000);
                        map_pos.y = screen->map_width/2 + 12;
                        pos = screen_position(map_pos, screen);
                        print_file("./designs/star.txt", pos, screen, TRUE);
                        fflush(stdout);
                    }
                }
            }

            usleep(2*1000*1000); 
            printf("\e[?25h");
            return OK;
        }
}

//This function doesn't crop the file, so make sure it fits on screen. If transparency is True spaces will be ignored.
Result print_file(char *path, Position pos, Screen *screen, Bool transparency){
    FILE *f;
    int i;
    char line[1024];
    Position pos_aux = pos;

    f = fopen(path, "r");
    if(!f) return ERROR;

    change_cursor(pos, screen);

    do{
        i = 0;
        if(read_line(f, line) == ERROR){
            printf("Error reading file in print_file()\n");
            return ERROR;
        }
        while (line[i] != 0){
            if (transparency == FALSE || line[i] != 32){
                printf("%c", line[i]);
                pos.x++;
            } else{
                pos.x++;
                change_cursor(pos, screen);
            }
            i++;
        }
        pos_aux.y++;
        pos = pos_aux;
        change_cursor(pos, screen);
    }while(!feof(f));

    return OK;
}

/*
    Prints the given map in the appropiate place of the 
    screen, croping it to the right dimensions if it
    exceeds the limits set in the screen object
*/
/* Result print_map(Screen *screen, Map map){
    int w = screen->map_width - 1;
    int h = screen->map_height;
    Position pos = screen->map;
    char c;

    change_cursor(pos, screen);
    for(int i=0; i < h; i++){
        for(int j=0; j < w; j++){
            c = box_to_char((map.boxes)[i][j]);
            printf("%c", c);
        }
        pos.y += 1;
        change_cursor(pos, screen);
    }
} */
/*
  Initializes the terminal in such a way that we can read the input
  without echo on the screen
*/
void _term_init(Screen*screen) {
	struct termios new;	          /*a termios structure contains a set of attributes about 
					  how the terminal scans and outputs data*/
	setvbuf(stdin, NULL, _IONBF, 0);
	tcgetattr(fileno(stdin), &initial);    /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
	new = initial;	                      /*then we copy them into another one, as we aren't going 
						to change ALL the values. We'll keep the rest the same */
	new.c_lflag &= ~ICANON;	              /*here we are setting up new. This line tells to stop the 
						canonical mode (which means waiting for the user to press 
						enter before sending)*/
	new.c_lflag &= ~ECHO;                 /*by deactivating echo, we tell the terminal NOT TO 
						show the characters the user is pressing*/
	new.c_cc[VMIN] = 1;                  /*this states the minimum number of characters we have 
					       to receive before sending is 1 (it means we won't wait 
					       for the user to press 2,3... letters)*/
	new.c_cc[VTIME] = 0;	              /*I really have no clue what this does, it must be somewhere in the book tho*/
	//new.c_lflag &= ~ISIG;                 /*here we discard signals: the program won't end even if we 
						/*press Ctrl+C or we tell it to finish*/

	tcsetattr(fileno(stdin), TCSANOW, &new);  /*now we SET the attributes stored in new to the 
						    terminal. TCSANOW tells the program not to wait 
						    before making this change*/
    setvbuf(stdin, NULL, _IONBF, 0);
}

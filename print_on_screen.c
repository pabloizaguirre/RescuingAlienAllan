#include "print_on_screen.h"
#include "read_from_file.h"
#include "map.h"
#include "level.h"
#include <string.h>
#include <termios.h>

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

/*
    Changes the custor to the given position
*/
int change_cursor(Position position, Screen *screen){
    int x, y;
    
    if (position.x < 0 || position.x > screen->screen_width || position.y < 2 || position.y > screen->screen_height + 1){
        print_message(screen, "Se pide cambiar el cursor a una posición no válida");
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
    if(r == ERROR) return NULL;

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->screen_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->screen_height = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->map.x = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->map.y = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->map_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->map_height = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->messagebox.x = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->messagebox.y = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->messagebox_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return NULL;
    screen->messagebox_height = atoi(line);
    screen->cursor.x = 10;
    screen->cursor.y = 10;

    //Set screen size
    _term_init(screen);
    clear_screen();
    change_color("reset", "reset");
    printf("%c[8;%d;%dt", 27, screen->screen_height, screen->screen_width);
    if(print_margins(screen) == ERROR) return NULL;
    printf("\n");
    print_title(screen, title);
    return screen;
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
    pos.y = 4;
    pos.x = (screen->screen_width  - strlen(title))/ 2;
    change_color("reset", "reset");
    change_cursor(pos, screen);
    printf(title);
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

Result print_resources(Screen *screen, Level *level){
    int end_flag = 0;
    int w = screen->screen_width - screen->map_width - 4;
    int h = screen->map_height;
    Position res_pos, pos;
    res_pos.x = 3;
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
    //Print the resources and instrucctions
    pos = res_pos;
    change_cursor(pos, screen);

    pos.y++;
    change_cursor(pos, screen);
    printf("RESOURCES:");
    pos.y++;

    
    
    if(level->num_ladder_floor > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("LADDERS/FLOORS (l/f): x%d", level->num_ladder_floor_act);
    }

    if(level->num_ladder > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("LADDERS (l): x%d", level->num_ladder_act);
    }

    if(level->num_floor > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("FLOORS (f): x%d", level->num_floor_act);
    }

    if(level->portal > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("PORTAL (p): x%d", level->portal_act);
    }

    // 2 starts
    pos.y+=2;
    change_cursor(pos, screen);
    printf("2 stars:");
    pos.y++;
    
    if(level->num_ladder_floor_2 > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("LADDERS/FLOORS: x%d", level->num_ladder_floor_2);
    }

    if(level->num_ladder_2 > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("LADDERS: x%d", level->num_ladder_2);
    }

    if(level->num_floor_2 > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("FLOORS: x%d", level->num_floor_2);
    }

    if(level->portal_2 > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("PORTALS: x%d", level->portal_2);
    }

    // 3 starts
    pos.y+=2;
    change_cursor(pos, screen);
    printf("3 stars use:");
    pos.y++;
    
    if(level->num_ladder_floor_3 > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("LADDERS/FLOORS: x%d", level->num_ladder_floor_3);
    }

    if(level->num_ladder_3 > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("LADDERS: x%d", level->num_ladder_3);
    }

    if(level->num_floor_3 > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("FLOORS: x%d", level->num_floor_3);
    }

    if(level->portal_3 > 0){
        pos.y++;
        change_cursor(pos, screen);
        printf("PORTALS: x%d", level->portal_3);
    }
    

    pos.y += 3;
    change_cursor(pos, screen);
    printf("Press ENTER to play");

    pos.y += 2;
    change_cursor(pos, screen);
    printf("Press DEL to delete");

    return OK;
}

Result level_end(Level_result res, Screen *screen){
    if(!screen){
        return ERROR;
    }

    switch(res){
            case LOST:
                print_message(screen, "Level not completed");
                fflush(stdout);
                usleep(2*1000*1000);
                break;
            case STARS_1:
                print_message(screen, "Level completed with 1 star");
                fflush(stdout);
                usleep(2*1000*1000);
                break;
            case STARS_2:
                print_message(screen, "Level completed with 2 stars");
                fflush(stdout);
                usleep(2*1000*1000);
                break;
            case STARS_3:
                print_message(screen, "Level completed with 3 stars");
                fflush(stdout);
                usleep(2*1000*1000);
                break;
            case SUPREME:
                print_message(screen, "You found a better way to solve the level than what the developers of the game thought!!!");
                fflush(stdout);
                usleep(2*1000*1000);
                break;
        }
        return OK;
}

//This function doesn't crop the file, so make sure it fits on screen
Result print_file(char *path, Position pos, Screen *screen){
    FILE *f;
    int i;
    char line[MAX_SIZE];
    Result r;

    f = fopen(path, "r");
    if(!f) return -1;

    change_cursor(pos, screen);

    do{
        i = 0;
        r = read_line(f, &line);
        while (line[i]){
            if (line[i] != " "){
                printf("%c", line[i]);
                pos.x++;
            } else{
                pos.x++;
                change_cursor(pos, screen);
            }
        }
        pos.y++;

        if(r == ERROR){
            printf("error reading file");
            return ERROR;
        }
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
}

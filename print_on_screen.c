#include "print_on_screen.h"
#include "read_from_file.h"
#include "map.h"
#include <string.h>
#include <termios.h>

struct termios initial;

void _term_init(Screen screen);

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
int change_cursor(Position position){
    //falta comprobar que la posición está bien
    int x = position.x;
    int y = position.y;

    return printf ("%c[%d;%dH", 27, y, x);
    
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
Result init_screen(char *file_name, Screen *screen){
    //Open file
    char line[MAX_SIZE];
    FILE *f;
    f = fopen(file_name, "r");
    Result r;
    //Initialice screen variables
    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->screen_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->screen_height = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->map.x = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->map.y = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->map_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->map_height = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->messagebox.x = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->messagebox.y = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->messagebox_width = atoi(line);

    r = read_line(f, &line);
    if(r == ERROR) return ERROR;
    screen->messagebox_height = atoi(line);


    //Set screen size
    _term_init(*screen);
    clear_screen();
    change_color("reset", "reset");
    printf("%c[8;%d;%dt", 27, screen->screen_height, screen->screen_width);
    if(print_margins(f) == ERROR) return ERROR;
    printf("\n");
    return OK;
}

Result restore_screen(Screen screen){
    Position final_pos;
    final_pos.x = 0;
    final_pos.y = screen.screen_height+1;
    change_color("reset", "reset");
    change_cursor(final_pos);
    tcsetattr(fileno(stdin), TCSANOW, &initial);
}

/*
    Prints the magins between the map and the message boxes
*/
Result print_margins(FILE *f){
    char line[MAX_SIZE];
    Result r;

    Position pos;
    pos.x = 0;
    pos.y = 2;
    change_cursor(pos);
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
Result print_margins2(Screen screen){
    int i=0, k;
    Position pos;
    pos.x = 0;
    pos.y = 2;
    change_cursor(pos);

    change_color("reset", "reset");
    printf("+");
    /*
        +------+
    */
    while (i < screen.screen_width - 1){
        printf("-");
    }
    printf("+");
    /*
        +------+
        |
        |
        +
    */
    pos.x = 0;
    pos.y = 3;
    printf("+");
    i = 1;
    while (i < screen.screen_height - 1){
        pos.x = 0;
        pos.y++;
        printf("|");
        i++;
    }
    pos.x = 0;
    pos.y++;
    printf("+");
    /*
        +------+
        |
        |
        +------+
    */
    i = 1;
    while(i < screen.screen_width - 1){
       printf("-");
       i++;
    }
    printf("+");
    /*
        +------+
        |      | 
        |      |
        +------+
    */
    i = 1;
    pos.x--;
    k = pos.x;
    while(i < screen.screen_height - 1){
       pos.x--;
       pos.y--;
       printf("|");
       i++;
    }
    return OK;
}

//comprobar errores
Result print_map (Box **map, Screen *s) {
    int i, j;
    Position p;
    if (!map || !s) {
        return ERROR;
        printf("Error en los argumentos");
    }
    
    p = s->map;
    
    if (change_cursor(p) < 0) return ERROR;

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
                default:
                    change_color("black", "red");
                    fprintf(stdout, "X");
                    break;
            }
        }
        (p.y)++;
        if (change_cursor(p) < 0) return ERROR;
    }
    return OK;
}


Result print_message(Screen screen, char *text){
    int end_flag = 0;
    int w = screen.messagebox_width - 1;
    int h = screen.messagebox_height;
    Position pos = screen.messagebox;
	
	change_color("reset", "reset");
    //Clear the messagebox
    change_cursor(pos);
    for(int i=0; i < h; i++){
        for(int j=0; j < w; j++){
            printf(" ");
        }
        pos.y += 1;
        change_cursor(pos);
    }
    //Print the message croping it to the right dimensions
    pos = screen.messagebox;
    change_cursor(pos);
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
        change_cursor(pos);
    }
}

/*
    Prints the given map in the appropiate place of the 
    screen, croping it to the right dimensions if it
    exceeds the limits set in the screen object
*/
/* Result print_map(Screen screen, Map map){
    int w = screen.map_width - 1;
    int h = screen.map_height;
    Position pos = screen.map;
    char c;

    change_cursor(pos);
    for(int i=0; i < h; i++){
        for(int j=0; j < w; j++){
            c = box_to_char((map.boxes)[i][j]);
            printf("%c", c);
        }
        pos.y += 1;
        change_cursor(pos);
    }
} */
/*
  Initializes the terminal in such a way that we can read the input
  without echo on the screen
*/
void _term_init() {
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

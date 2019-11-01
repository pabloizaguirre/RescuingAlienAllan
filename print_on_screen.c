#include "print_on_screen.h"
#include "read_from_file.h"
#include "map.h"
#include <string.h>

/*
    Changes the color you are printing with to the color given by the string argument
*/
int change_color(char *foreground_color, char *background_color){
    char *foreground_color_code = "30";
    char *background_color_code = "40";

    if(!strcmp(background_color, "black")) background_color_code = "30";
    else if(!strcmp(background_color, "red")) background_color_code = "31";
    else if(!strcmp(background_color, "green")) background_color_code = "32";
    else if(!strcmp(background_color, "yellow")) background_color_code = "33";
    else if(!strcmp(background_color, "blue")) background_color_code = "34";
    else if(!strcmp(background_color, "magenta")) background_color_code = "35";
    else if(!strcmp(background_color, "cyan")) background_color_code = "36";
    else if(!strcmp(background_color, "white")) background_color_code = "37";

    if(!strcmp(foreground_color, "black")) foreground_color_code = "40";
    else if(!strcmp(foreground_color, "red")) foreground_color_code = "41";
    else if(!strcmp(foreground_color, "green")) foreground_color_code = "42";
    else if(!strcmp(foreground_color, "yellow")) foreground_color_code = "43";
    else if(!strcmp(foreground_color, "blue")) foreground_color_code = "44";
    else if(!strcmp(foreground_color, "magenta")) foreground_color_code = "45";
    else if(!strcmp(foreground_color, "cyan")) foreground_color_code = "46";
    else if(!strcmp(foreground_color, "white")) foreground_color_code = "47";
        

    return printf("%c[%s;%sm", 27, foreground_color_code, background_color_code);

}

/*
    Changes the custor to the given position
*/
int change_cursor(Position position){
    //falta comprobar que la posición está bien
    int x = position.x;
    int y = position.y;

    printf ("%c[%d;%dH", 27, y, x);
    
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
    clear_screen();
    printf("%c[8;%d;%dt", 27, screen->screen_height, screen->screen_width);
    if(print_margins(f) == ERROR) return ERROR;
    printf("\n");
    return OK;
}

/*
    Prints the magins between the map and the message boxes
*/
Result print_margins(FILE *f){
    char line[MAX_SIZE];
    Result r;
    do{
        r = read_line(f, &line);
        printf(line);

        if(r == ERROR){
            printf("error reading file");
            return ERROR;
        }
    }while(!feof(f));    
}

//comprobar errores
Result print_map (Box **map, Screen *s) {
    int i, j;
    Position p;
    if (!map || !s) return ERROR;
    
    p = s->map;

    if (change_cursor(p) < 0) return ERROR;

    for (i = s->map_height - 1; i >= 0; i--) {
        for (j = 0; j < s->map_width; j++) {
            switch (map[i][j]){
                case AIR:
                    fprintf(stdout, " ");
                case WALL:
                    change_color("black", "white");
                    fprintf(stdout, " ");
                case START:
                    change_color("black", "green");
                    fprintf(stdout, " ");
                case END:
                    change_color("black", "cyan");
                    fprintf(stdout, " ");
                case LAVA:
                    change_color("black", "red");
                    fprintf(stdout, " ");
                case PORTALA:
                    change_color("black", "cyan");
                    fprintf(stdout, " ");
                case PORTALB:
                    change_color("black", "cyan");
                    fprintf(stdout, " ");
                case LADDER:
                    change_color("white", "black");
                    fprintf(stdout, "#");
                default:
                    change_color("red", "black");
                    fprintf(stdout, "X");
            }
        }
    p.y++;
    if (change_cursor(p) < 0) return ERROR;
    }
    return OK;
}


Result print_message(Screen screen, char *text){
    int end_flag = 0;
    int w = screen.messagebox_width - 1;
    int h = screen.messagebox_height;
    Position pos = screen.messagebox;
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

#include "print_on_screen.h"
#include "types.h"
#include "map.h"
#include "read_from_file.h"

int main(int argc, char** argv){
    int i, j;
    Screen screen;
    //Level level;
    char interface_file[] = "size_test.txt";
    FILE *f;
    Box **map_bx;
    char **map_ch;

    init_screen(interface_file, &screen);
    // comprobar
    print_message(screen, "hola mundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    print_message(screen, "Hola esto es una prueba");

    change_color("black", "white");
    printf("hola");
    Position pos;
    pos.x = 0;
    pos.y = screen.screen_height+3;
    change_cursor(pos);

    /*  map_ch = map_from_file("map_test.txt", screen);
    if (!map_ch) {
        printf("ERROR en map_from_file()");
        return ERROR;
    }
    for (i = screen.map_height - 1; i >= 0; i--) {
        for (j = 0; j < screen.map_width; j++){
            printf("%c", map_ch[i][j]);
        }
    }
    //map_ch[29][0] == S
    printf("*************%c***********", map_ch[29][0]); */
    
    /*
    map_bx = map_char_to_box(map_ch, screen);
    if (!map_bx) {
        printf("ERROR en map_char_to_box()");
        return ERROR;
    } */

    map_bx = (char**)malloc(sizeof(char*)*screen.screen_height);
    if(map_bx == NULL) return ERROR;

    //falta hacer free de todo map cuando hay un error
    for(i=0;i<screen.map_width;i++){
        map_bx[i]=(char*)malloc(sizeof(char)*screen.map_width);
        if(map_bx[i] == NULL) return NULL;
    }

    for (i = 0; i < screen.map_height; i++){
        for (j = 0; j < screen.map_width; j++) {
            map_bx[i][j] = LAVA;
        }
    } */

    printf("hasta aqui sí");
    
    /*if (print_map(map_bx, screen) == ERROR) return ERROR; */



    return OK;
}

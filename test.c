#include "print_on_screen.h"
#include "types.h"
#include "map.h"
#include "read_from_file.h"

#define MAPA "map_test4.txt"

int main(int argc, char** argv){
    int i, j, k;
    Screen *screen;
    //Level level;
    char interface_file[] = "size_test.txt";
    FILE *f;
    Box **map_bx;
    char **map_ch;

    init_screen(interface_file, &screen);
    // comprobar
    print_message(screen, "hola mundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaamundoaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    print_message(screen, "Hola esto es una prueba");

    Position pos;
    pos.x = 0;
    pos.y = screen->screen_height+3;
    change_cursor(pos, screen);

    map_ch = map_from_file(MAPA, screen);
    if (!map_ch) {
        printf("ERROR en map_from_file()");
        return ERROR;
    }    
    
    map_bx = map_char_to_box(map_ch, screen);
    if (!map_bx) {
        printf("ERROR en map_char_to_box()");
        return ERROR;
    }


    if (print_map(map_bx, screen) == ERROR) {
        return ERROR;
        printf("ERROR en print_map\n");
    }
    print_message(screen, "Se ha impreso el mapa");

    pos.x = -1;
    pos.y = 10;
    change_cursor(pos, screen);

    restore_screen(screen);

    return OK;
}

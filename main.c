#include "print_on_screen.h"
#include "types.h"

int main(int argc, char** argv){
    init_screen();
    change_color("black", "white");
    printf("hola");

    return OK;
}
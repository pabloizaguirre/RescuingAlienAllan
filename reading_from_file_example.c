#include "read_from_file.h"
#include "types.h"

/*
    Example on how to read a file
    using the read_from_file library
*/
int main(int argc, char** argv){
    FILE *f;
    char line[MAX_SIZE];
    Result r;
    f = fopen("test.txt", "r+");
    if(!f) return -1;

    while(!feof(f)){
        r = read_line(f, &line);
        printf(line);

        if(r == ERROR){
            printf("error reading file");
            return -1;
        }

        printf("Todavia no ha acabado el archivo: %d\n", atoi(line));
    }


    return OK;
}
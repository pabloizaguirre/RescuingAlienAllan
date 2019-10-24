#include "read_from_file.h"
#include "types.h"

int main(int argc, char** argv){
    FILE *f;
    char line[MAX_SIZE];
    Result r;
    f = fopen("test.txt", "r");
    if(!f) return -1;

    do{
        r = read_line(f, &line);
        printf(line);

        if(r == ERROR){
            printf("error reading file");
            return -1;
        }
    }while(!feof(f));



    return OK;
}
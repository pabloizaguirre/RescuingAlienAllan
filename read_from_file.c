#include "read_from_file.h"
#define MAX_SIZE 1024

char *read_line(FILE *f){
    char ret[MAX_SIZE];
    do{
        if(fgets(ret, MAX_SIZE, f) == NULL) return NULL;
    }while(ret[0] == '#' || ret[0] == (char)0);

    return ret;
}
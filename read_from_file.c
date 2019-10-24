#include "read_from_file.h"

Result read_line(FILE *f, char *line){

    do{
        if(fgets(line, MAX_SIZE, f) == NULL){
            if(feof(f)) return OK;
            return ERROR;
        }
    }while(line[0] == '#' || line[0] == '\n');

    return OK;
}
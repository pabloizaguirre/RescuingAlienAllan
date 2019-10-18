#include <stdio.h>
#include <stdlib.h>

char** map_from_file(char *file){
    FILE *fp;
    int i,j,rows = 40,columns = 130;
    char **map;

    map = (char**)malloc(sizeof(*char)*columns);

    for(i=0;i<rows;i++){
        map[i]=(char*)malloc(sizeof(char)*rows);
    }

    fp = fopen(file, "r");

    for(i=0;i<columns;i++){
        for(j=0;j<rows;j++){
            map[columns-i-1][rows-j-1]=fgetc(fp);
        }
        fgetc(fp);
    }
    return map;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "print_on_screen.h"
#include "map.h"
#include "level.h"
#include "people.h"


Level* level_from_file(Level *level, char *file){
    if (level == NULL) return NULL;

    Result r;
    FILE *fp;
    char line[256];
    
    fp = fopen(file,"r");

    r = read_line(fp, line);
    if (r == ERROR) return NULL;
    level->level_number = atoi(line);

    r = read_line(fp, line);
    if (r == ERROR) return NULL;
    strcpy(level->message,line);

    r = read_line(fp, line);
    if (r == ERROR) return NULL;
    int number_people = atoi(line);
    People *people;
    for(int i = 0; i < number_people; i++){
        people[i]=people
    }

}


/*
This function transforms the characters to boxes.
map[0][0] es el de abajo a la izquierda.
arriba a la izquierda seria map[100][0]
 */

Box** map_char_to_box(char** map, Screen screen){
    char lista[8] = " %SXWAB#";
    int i,j,rows = screen.screen_width,columns = screen.screen_height;
    Box **mapB;

    map = (Box**)malloc(sizeof(Box*)*columns);
    if(map == NULL) return NULL;

    for(i=0;i<rows;i++){
    map[i]=(Box*)malloc(sizeof(Box)*rows);
    if(map[i] == NULL) return NULL;
    }

    for(i=0;i<columns;i++){
        for(j=0;j<rows;j++){
            if (map[i][j] == lista[0]) mapB[i][j] = AIR;
            else if (map[i][j] == lista[1]) mapB[i][j] = WALL;
            else if (map[i][j] == lista[2]) mapB[i][j] = START;
            else if (map[i][j] == lista[3]) mapB[i][j] = END;
            else if (map[i][j] == lista[4]) mapB[i][j] = LAVA;
            else if (map[i][j] == lista[5]) mapB[i][j] = PORTALA;
            else if (map[i][j] == lista[6]) mapB[i][j] = PORTALB;
            else if (map[i][j] == lista[7]) mapB[i][j] = LADDER; 
        }
    }
}
/*
Receives a file with a map and writes it in a char table.
 */

char** map_from_file(char *file, Screen screen){
    FILE *fp;
    int i,j,rows = screen.screen_width,columns = screen.screen_height;
    char **map;

    map = (char**)malloc(sizeof(char*)*columns);
    if(map == NULL) return NULL;

    for(i=0;i<rows;i++){
        map[i]=(char*)malloc(sizeof(char)*rows);
        if(map[i] == NULL) return NULL;
    }

    fp = fopen(file, "r");
    if(fp == NULL) return NULL;

    for(i=0;i<columns;i++){
        read_line(fp,map[columns-i-1]);
    }
    return map;
}
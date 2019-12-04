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

/*
This function transforms the characters to boxes.
map[0][0] es el de abajo a la izquierda.
arriba a la izquierda seria map[100][0]
 */

//Celdas
Map* map_char_to_box(char** map, Screen *screen){
    char lista[9] = ".WSELAB#Z";
    int i, j, rows = screen->map_height, columns = screen->map_width;
    Map* map_obj = NULL;
    Box **mapB = NULL;

    //falta control de errores.
    map_obj = (Map *)malloc(sizeof(Map));
    if(map == NULL) return NULL;

    map_obj->Start_pos = NULL;

    map_obj->End_pos = NULL;

    map_obj->PORTALB_pos = NULL;

    mapB = (Box**)malloc(sizeof(Box*)*rows);
    if(mapB == NULL){
        free(map);
        free(map_obj);
        return NULL;
    }
    
    //falta hacer free bien
    for(i=0;i<rows;i++){
        mapB[i]=(Box*)malloc(sizeof(Box)*(columns));
        if(mapB[i] == NULL) {
            //falta cambiar free(map_obj) por free_map(map_obj)
            free(map);
            free(map_obj); 
            free(mapB);
            return NULL;
        }
    }
    //falta arreglar la start_pos, end_pos y portal_pos
    for(i=0;i<rows;i++){
        for(j=0;j<columns;j++){
            if (map[i][j] == lista[0]) mapB[i][j] = AIR;
            else if (map[i][j] == lista[1]) mapB[i][j] = WALL;
            else if (map[i][j] == lista[2]){
                mapB[i][j] = START;
                map_obj->Start_pos = (Position*)malloc(sizeof(Position));
                if(!map_obj->Start_pos) return NULL;
                map_obj->Start_pos->x = i;
                map_obj->Start_pos->y = j;
            }
            else if (map[i][j] == lista[3]){
                mapB[i][j] = END;
                map_obj->End_pos = (Position*)malloc(sizeof(Position));
                if(!map_obj->End_pos) return NULL;
                map_obj->End_pos->x = i;
                map_obj->End_pos->y = j;
            }
            else if (map[i][j] == lista[4]) mapB[i][j] = LAVA;
            else if (map[i][j] == lista[5]) mapB[i][j] = PORTALA;
            else if (map[i][j] == lista[6]){
                mapB[i][j] = PORTALB;
                map_obj->PORTALB_pos = (Position*)malloc(sizeof(Position));
                if(!map_obj->PORTALB_pos) return NULL;
                map_obj->PORTALB_pos->x = i;
                map_obj->PORTALB_pos->y = j;
            }
            else if (map[i][j] == lista[7]) mapB[i][j] = LADDER;
            else if (map[i][j] == lista[8]) mapB[i][j] = ZONAPORTAL;
        }
        free(map[i]);
    }

    free(map);
    
    map_obj->boxes = mapB;

    return map_obj;
}

/*
Receives a file with a map and writes it in a char table.
 */

char** map_from_file(char *file, Screen *screen){
    FILE *fp;
    //problematico
    int i,j,rows = screen->map_height,columns = screen->map_width;
    char **map;

    map = (char**)malloc(sizeof(char*)*rows);
    if(map == NULL) return NULL;

    //falta hacer free de todo map cuando hay un error
    for(i=0;i<rows;i++){
        map[i]=(char*)malloc(sizeof(char)*columns);
        if(map[i] == NULL) return NULL;
    }

    fp = fopen(file, "r");
    if(fp == NULL) return NULL;

    for(i=0;i<rows;i++){
        read_line(fp,map[rows-i-1]);
    }

    fclose(fp);

    return map;
}

#include "read_from_file.h"

/*
    Inputs:
        - *f: pointer to file to be read
        - *line: pointer to character where the read line
        should be stored
    Description:
        Reads a line ignoring empty lines and lines starting
        with #
*/
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
    Inputs:
        - **map: pointer double array of character to be converted
        - *screen: pointer to the main screen
    Outputs:
        Map *: pointer to the initialized map
        NULL in case of error
    Description:
        initializes a map from the given array of characters
        initializes de boxes, boxes_design and boxes_merge arrays
        boxes and boxes_merge are initialized to the contents in the file
        and boxes_design is initialized to AIR

        The char to box conversion goes as follows:
        .   AIR
        W   WALL
        S   START
        E   END
        L   LAVA
        A   PORTALA
        B   PORTALB
        #   LADDER
        Z   ZONAPORTAL
        w   WALL_MERGE
        D   DISAPPEAR_WALL
        l   LADDER_MERGE
*/
Map* map_char_to_box(char** map, Screen *screen){
    char lista[12] = ".WSELAB#ZwDl";
    int i, j, rows = screen->map_height, columns = screen->map_width;
    Map* map_obj = NULL;
    Box **mapB = NULL;
    Box **boxes_design = NULL;
    Box **boxes_merge = NULL;

    map_obj = (Map *)malloc(sizeof(Map));
    if(map == NULL) return NULL;
    
    boxes_design = (Box **)malloc(sizeof(Box*)*rows);
    if(boxes_design == NULL){
        free(map);
        free(map_obj);
        return NULL;
    }

    boxes_merge = (Box **)malloc(sizeof(Box*)*rows);
    if(boxes_merge == NULL){
        free(map);
        free(map_obj);
        return NULL;
    }

    map_obj->Start_pos = NULL;

    map_obj->End_pos = NULL;

    map_obj->PORTALB_pos = NULL;

    mapB = (Box**)malloc(sizeof(Box*)*rows);
    if(mapB == NULL){
        free(map);
        free(map_obj);
        return NULL;
    }
    
    for(i=0;i<rows;i++){
        mapB[i]=(Box*)malloc(sizeof(Box)*(columns));
        if(mapB[i] == NULL) {
            free_map(map_obj, screen);
            return NULL;
        }
    }

    for(i=0;i<rows;i++){
        boxes_design[i]=(Box*)malloc(sizeof(Box)*(columns));
        if(boxes_design[i] == NULL) {
            free_map(map_obj, screen);
            return NULL;
        }
        for(j=0; j<columns; j++){
            boxes_design[i][j] = AIR;
        }
    }

    for(i=0;i<rows;i++){
        boxes_merge[i]=(Box*)malloc(sizeof(Box)*(columns));
        if(boxes_merge[i] == NULL) {
            free_map(map_obj, screen);
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
            else if (map[i][j] == lista[9]) mapB[i][j] = WALL_MERGE;
            else if (map[i][j] == lista[10]) mapB[i][j] = DISAPPEAR_WALL;
            else if (map[i][j] == lista[11]) mapB[i][j] = LADDER_MERGE;
        }
        free(map[i]);
    }

    free(map);
    
    map_obj->boxes = mapB;

    map_obj->boxes_design = boxes_design;
    map_obj->boxes_merge = boxes_merge;

    return map_obj;
}

/*
    Inputs:
        - *file: name of the file to open
        - *screen: pointer to the main screen
    Outputs:
        Char **: contents read from the file
        NULL in case of error
    Description:
        Reads a file initializing a double array of characters
        with the contents of the file
*/
char** map_from_file(char *file, Screen *screen){
    FILE *fp;
    int i,rows = screen->map_height,columns = screen->map_width;
    char **map;

    map = (char**)malloc(sizeof(char*)*rows);
    if(map == NULL) return NULL;

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

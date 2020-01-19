#ifndef types_h
#define types_h

//normal speed is: 100 (in ms)
#define SPEED 100

#define MAX_SIZE 1024

#include <time.h>
#include <unistd.h>
#include <stdlib.h>

/*
    General purpose enumerations and structures
*/

typedef struct _Level Level;

/* This enumeration defines the type Box, which represents an element in the map */
typedef enum {AIR, WALL, START, END, LAVA, PORTALA, PORTALB, LADDER, ZONAPORTAL, WALL_MERGE, DISAPPEAR_WALL, LADDER_MERGE} Box;

typedef struct {
    int x;
    int y;    
} Position;

typedef enum {FALSE, TRUE} Bool;

typedef enum {DEAD, ALIVE, DESINTEGRATED, FINISHED} State;

typedef enum {OK, ERROR} Result;

typedef enum {GAME_FINISHED, GAME_NOT_FINISHED, LEVEL_FINISHED, LEVEL_NOT_FINISHED, DESIGN_FINISHED, DESIGN_NOT_FINISHED} FLAG;

typedef enum {RES_ERROR, LOST, STARS_1, STARS_2, STARS_3, SUPREME} Level_result;

#endif

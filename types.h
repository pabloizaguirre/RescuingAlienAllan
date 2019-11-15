#ifndef types_h
#define types_h


typedef struct {
    int x;
    int y;    
} Position;

typedef enum {FALSE, TRUE} Bool;

typedef enum {DEAD, ALIVE, DESINTEGRATED, FINISHED} State;

typedef enum {OK, ERROR} Result;

typedef enum {GAME_FINISHED, GAME_NOT_FINISHED, LEVEL_FINISHED, LEVEL_NOT_FINISHED, DESIGN_FINISHED, DESIGN_NOT_FINISHED} FLAG;

#endif
#ifndef types_h
#define types_h


typedef struct {
    int x;
    int y;    
} Position;

typedef enum {FALSE, TRUE} Bool;

typedef enum {DEAD, ALIVE, DESINTEGRATED, FINISHED} State;

typedef enum {OK, ERROR} Result;

#endif
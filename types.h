#ifndef types_h
#define types_h


typedef struct {
    int x;
    int y;    
} Position;

typedef enum {FALSE, TRUE} Bool;

typedef enum {DEAD, ALIVE} State;

typedef enum {OK, ERROR} Result;

#endif
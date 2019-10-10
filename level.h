#include "map.h"

#ifndef level_h
#define level_h


typedef struct{
    Map *map;
    Level *next_level;
    Level *last_level;
    int level_number;
    char *mesaje;
} Level;

#endif
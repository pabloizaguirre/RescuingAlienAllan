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

int level_print_mesaje(Level *level);

Map *level_get_map(Level *level);
Level *level_get_next_level(Level *level);
Level *level_get_last_level(Level *level);

#endif
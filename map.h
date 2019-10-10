#ifndef map_h
#define map_h

typedef enum {AIR, WALL, START, END, LAVA} Box;

typedef struct {
    Box **boxes;
} Map;

Map *create_map(FILE *f);

void *free_map(Map *map);

void *display_map(Map *map);

Box *map_get_player_surroundings(People *player, Map *map);



#endif
#include "people.h"
#include "map.h"

int People_update(People *p, Map *map){
    Surroundings surr = map_get_position_surroundings(p->position, map);
    Position pos_aux = p->position;

    // Falta comprobar que la persona no está en un sitio que mate

    // First check if the person is in a special box (i.e: ladder, portal ...)
    switch (*(surr.center)){
        case LADDER:
            pos_aux.y++;
            if (is_position_occupable(pos_aux, map)){
                p->position.y++;
                return 1;
            }
            return 0;
            break;
        case PORTALA:
            if(map->PORTALB_pos){
                pos_aux = *(map->PORTALB_pos);
                if (is_position_occupable(pos_aux, map)){
                    p->position = pos_aux;
                    return 1;
                }
            }
            return 0;
            break;
    }

    // Check if the person can move down
    pos_aux = p->position
    pos_aux.y--;
    if (is_position_occupable(pos_aux, map)){
        p->position = pos_aux;
        return 1;
    } else {
        pos_aux = p->position;
        pos_aux.x++;
        if (is_position_occupable(pos_aux, map)) {
            p->position = pos_aux;
            return 1;
        }
    }
    return 0;
}
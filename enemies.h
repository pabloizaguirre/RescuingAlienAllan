#include "types.h"

#ifndef enemies_h
#define enemies_h

typedef enum {GUARD1, GUARD2} EnemyType;

typedef struct {
    EnemyType enemy_type;
    char character;
    Position position;

} Enemy;

/*
    Allocates memory for a new enemy object
*/
Enemy *create_enemy(EnemyType type, char character, Position position);

/*
    Update enemy position
*/
Enemy *enemy_update(Enemy *enemy, Level *level);

/*
    Get functions for the enemy structure
*/
EnemyType enemy_get_enemytype(Enemy *enemy);
char enemy_get_char(Enemy *enemy);
Position enemy_get_position(Enemy *enemy);

/*
    Set fuctions for the enemy structure
*/
Enemy *enemy_set_enemytype(Enemy *enemy, EnemyType type);
Enemy *enemy_set_char(Enemy *enemy, char char);
Enemy *enemy_set_position(Enemy *enemy, Position position);


/*
    Frees the memory ocupied by the enemy
*/
Enemy *free_enemy(Enemy *enemy);



#endif

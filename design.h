#ifndef DESIGN_H
#define DESIGN_H

#include "types.h"
#include "print_on_screen.h"

/*
    Input:
        - *level: pointer to current level
        - *screen: pointer to main screen
    Outputs:
        FLAG: DESIGN_FINISHED or DESIGN_NOT_FINISHED
    Description:
        Called for each iteration of the design loop
        Waits for a key input and places / deletes the corresponding resource
        Returns a flag indicating wheter the design loop should finish or not
*/
FLAG design(Level *level, Screen *screen);

#endif

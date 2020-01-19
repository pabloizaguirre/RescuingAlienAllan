#ifndef READ_KEYS_H
#define READ_KEYS_H

/*
    Output:
        Character representing the pressed key
    Description:
        Waits for the user to input a key and returns it
        If the key pressed is an arrow key the returned character
        is the corresponding WASD character.
*/
char read_key();

#endif

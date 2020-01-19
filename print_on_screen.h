#ifndef print_on_screen_h
#define print_on_screen_h

#include "types.h"
//#include "map.h"
#include <stdio.h>

#define FG 38
#define BK 48   

typedef struct {
    int screen_height;
    int screen_width;
    Position map;
    int map_width;
    int map_height;
    Position cursor;
    Position messagebox;
    int messagebox_width;
    int messagebox_height;
    char title[MAX_SIZE];
} Screen;

/*
    Inputs:
        - *file_name: name of the file where the information
        about the screen is stored
    Outputs:
        - Screen *: pointer to the initialized screen
    Description:
        Opens the given file
        Creates a screen structure
        Set all its values based on the contents of the file
        Sets the terminal size
        Displays a welcome start screen and waits for any key
        Clears the screen
        Prints the margins and the game title
*/
Screen *init_screen(char *file_name);


/*
    Inputs:
        - *screen: pointer to the main screen
    Description:
        Restores all the terminal settings and frees the screen
*/
Result restore_screen(Screen *screen);

/*
    Description:
        Clears the screen
*/
Result clear_screen();

/*
    Inputs:
        - *screen: pointer to the main screen
    Description:
        Pritns all the margins in the game
*/
Result print_margins(Screen *screen);

/*
    Inputs:
        - *background_color: name of the background color
        - *foreground_color: name of the foreground color
    Description:
        Changes the terminal background and foreground colors
        available options are:
            -black
            -red
            -green
            -yellow
            -blue
            -magenta
            -cyan
            -white
            -reset (default terminal color)
    NOTE:
        Setting one of the colors to reset resets both colors
*/
int change_color(char *background_color, char *foreground_color);

/*
    Inputs:
        - r, g, b: RGB values for the color
        - bf: integer indicating whether you want to change
        the background or the foreground color
    Description:
        Changes the terminal color to the given RGB values
        to indicate if you want to change the background or
        the foreground color use the macros defined earlier
        in this file
*/
void change_color_rgb(int r, int g, int b, int bf);

/*
    Inputs:
        - position: where to change the cursor
        - *screen: pointer to the main screen
    Description:
        Changes the cursor to the given position
        Checks if the position is valid
*/
int change_cursor(Position position, Screen *screen);

/*
    Prints the given string in the given position
*/
int print_on_position(Position position, char *text);

/*
    Inputs:
        - *screen: pointer to the main screen
        - *text: message to be displayed
    Description:
        Pritns the given message in the message box
        crops it to the right dimensions and splits it into lines
*/
Result print_message(Screen *screen, char *text);

/*
    Inputs:
        - *screen: pointer to the main screen
        - *level: pointer to the current level
    Description:
        Clears the resources box
        Pritns all the information about the level resources in the screen
*/
Result print_resources(Screen *screen, Level *level);

/*
    Inputs:
        - *screen: pointer to the main screen
        - *title: title of the game
    Description:
        Pritns the title at the top of the screen
*/
Result print_title(Screen *screen, char *text);

/*
    Inputs:
        - res: result of the level indicating the number of stars won
        - *screen: pointer to the main screen
    Description:
        Prints an animation depending on the number of stars the player won
*/
Result level_end(Level_result res, Screen *screen);

/*
    Inputs:
        - *path: name of the file to be printed
        - pos: position where the file should be printed
        - *screen: pointer to the main screen
        - transparency: whether or not to ignore the spaces
    Description:
        Prints the contents of the given file in the screen
        NOTE:
            This function doesn't crop the file, so make sure it fits on screen.
            If transparency is True spaces will be ignored.
*/
Result print_file(char *path, Position pos, Screen *screen, Bool transparency);

/*
    Inputs:
        - *screen: pointer to the main screen
    Description:
        Pritns the contents of the designs/start_screen.txt file
        and waits for any key to be pressed
*/
Result start_screen(Screen *screen);

/*
    Inputs:
        - **map: map to be printed
        - *screen: pointer to the main screen
    Description:
        Pritns the given map in the screen
        Makes the appropiate conversions from box to
        character and colors
*/
//Result print_map (Box **map, Screen *s);

/*
    Inputs:
        - *screen: pointer to the main screen
    Description:
        Prints the contents of the given file in the screen
        NOTE:
            Ereses the contents inside the mapbox
*/
Result erase_mapbox(Screen *screen);

/*
    Inputs:
        - pos: position to print the box
        - r, g, b: RGB values of the box
        - width, height: size of the box
        - *screen: pointer to the main screen
    Description:
        Prints a rectangular box of the given size at the given position
        with the give colors
*/
Result print_box(Position pos, int r, int g, int b, int width, int height, Screen *screen);

/*
    Inputs:
        - *screen: pointer to the main screen
    Description:
        Prints the start screen where you choose if you want to continue or start a new game
        if there isn't a progress file or you choose to start a new game, it returns 0, if you decide
        to continue the game it returns 1, and if there was a problem it returns -1
*/
int continue_or_newgame_screen(Screen *screen);

/*
    Description:
        Disables the terminal cursor
*/
void disable_terminal_cursor();

/*
    Description:
        Enables the terminal cursor
*/
void enable_terminal_cursor();

#endif

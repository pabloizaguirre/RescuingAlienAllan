/*
    Project: demo game for PPROG
    File: interface.h
    Rev:  1.0
    Date: 9/10/2019
    
    This file contains the functions and structures necessary to manage 
    the interface of the game. An interface is composed of a number of 
    windows, in which the program can write. If there are N windows, they
    will be identified by a number. The number can be given by the program
    at the time of window creation or be read from a file if the interface
    is loaded from a file. 
    
    When a program write in a window, the position at which it writes will
    be specified with respect to the upper-left corner of the window, and the 
    library will truncate strings that would exceed the window margin.
    
    Consider, for example, a window placed in position (2,2) of the screen
    with dimensions of 5 rows and 10 columns:
    
     position (0,0): first row, first column
     |
    +|------------------------------------------------------------
    |V
    |
    |  +----------+
    |  |          |
    |  |          |  (the lines are indicative: the window 
    |  |          |   is not actually marked on the screen)
    |  |          |
    |  |          |
    |  +----------+
    |
    |
    |

    Writing "the" in position (1,1) of the window will result in a screen 
    like

    +-------------------------------------------------------------
    |
    |
    |  
    |   the
    |
    |
    |
    
    Note that the word "the" appears in absolute position (3,3): (1,1) 
    with respect to a window in position (2,2)!
    
    Writing, in the same position, "the quick brown fox jumped over the 
    lazy dog" will result in 

    +-------------------------------------------------------------
    |
    |
    |  
    |   the quick 
    |
    |
    |

    As the library will prevent writing outside the window. Attempting to 
    write with a starting position outside of the window will produce nothing,
    even if the string would indeed overlap the window. For example, writing
    the same sentence in position (1,-1) will not write anything.
    
    
    In addition to a number of display windos, the interface defines one (and 
    only one) command window. The command window can be used to receive commands
    and return them to the calling program. It will display a prompt that can
    be configured by the caller. The command window will manage the input and 
    the "delete" button, guaranteeing that no string longer than the width
    of the window can be entered.
    
    
    The interface can be created by calling the appropiate functions (see 
    below) or created from a file. The latter is the most common option.
    
    The file has the following format (the # character indicates comments that
    are stripped from the file).
    
    
    ###################################################################
    # INTERFACE demo
    #
    INTERFACE 1.0   # keyword with the version of the library
    WINNUMBER   3     # number of display windows 

    WINDOW  0       # starts a window giving its identifier
    2, 2, 10, 40,    # first row, first column, number of rows and columns
    COLOR  5, 2,    # background and foreground color
    
    WINDOW  1     # starts a window giving its identifier
    15, 2, 10, 40  # first row, first column, number of rows and columns
    COLOR  5, 2   # background and foreground color
    
    WINDOW  2     # starts a window giving its identifier
    15, 45, 10, 40 # first row, first column, number of rows and columns
    COLOR  5, 2   # background and foreground color
    
    COMMAND         # declares the command window 
    15, 45, 10, 40, # first row, first column, number of rows and columns
    COLOR  5, 2,      # background and foreground color

    GLOBALCOLOR  5, 2     # background and foreground color for the parts 
                         # not covered by windows
                         
    SCHEMA      # a drawing that will be made on the screen to mark the 
                # windows 
    10, 50       # dimensions of the schema
    +------------+-----------------------------------+
    |            |                                   |
    |            |                                   |
    |            |                                   |
    |            |                                   |
    |            |                                   |
    |            +-----------------------------------+
    |            |                                   |
    |            |                                   |
    +------------+-----------------------------------+

    ENDINTERFACE    

*/

#ifndef ___INTERFACE___DEFINED___
#define ___INTERFACE___DEFINED___

typedef struct _interface interface;


/*
    Function: interface *i_create(FILE *fp)
    
    Creates an interface loading it from an open stream. If the stream is
    null or invalid the function will return NULL; is there is a format error in 
    the file, the function will abort the program.
    
    Note that this function does not display the interface nor does it access
    the screen in any way: this function simply creates the interface structure
    and returns it. Call the appropiate functions to display the interface. writing
    on an interface or receiving a command without a previous call to i_diisplay
    will have unpredictable results.
*/
interface *i_create(FILE *fp);


/*
    Function i_window_no(interface *it)
    
    Returns the number of windows in an interface. Returns -1 if the paramter
    is invalid.
*/
int i_window_no(interface *it);



/*
    Function i_win_rows(interface *it, int win_no)
    
    Returns the number of rows of the window number win_no of the interface it.
    Returns -1 if the window doesn't exist or the parameter it is invalid
*/
int i_win_rows(interface *it, int win_no);

/*
    Function i_win_cols(interface *it, int win_no)
    
    Returns the number of columns of the window number win_no of the interface it.
    Returns -1 if the window doesn't exist or the parameter it is invalid
*/
int i_win_cols(interface *it, int win_no);


/*
    Function i_win_bg(interface *it, int win_no)
    
    Returns the background color of the window number win_no of the interface it.
    Returns -1 if the window doesn't exist or the parameter it is invalid
*/
int i_win_bg(interface *it, int win_no);


/*
    Function i_win_fg(interface *it, int win_no)
    
    Returns the foreground color of the window number win_no of the interface it.
    Returns -1 if the window doesn't exist or the parameter it is invalid
*/
int i_win_fg(interface *it, int win_no);

/*
    Function i_win_bg_set(interface *it, int win_no, int color)
    
    Changes the background color of the window number win_no of the interface it.
    Returns the previous background color.
    Returns -1 if the window doesn't exist, the parameter it is invalid, or the 
    color is not in the range [0,..,7]
*/
int i_win_bg_set(interface *it, int win_no, int color);

/*
    Function i_win_fg_set(interface *it, int win_no, int color)
    
    Changes the background color of the window number win_no of the interface it.
    Returns the previous background color.
    Returns -1 if the window doesn't exist, the parameter it is invalid, or the 
    color is not in the range [0,..,7]
*/
int i_win_fg_set(interface *it, int win_no, int color);

/*
    Function: char *i_prompt(interface *it)
    
    Returns the prompt of the command window of the interface it. Returns
    NULL if the paramter it is invalid or if the interface has no command
    window.
    
    NOTE: the function returns a pointer wo the actual prompt: the contents
    of the returned value should not be changed in any way. Doing so will
    result in erratic behavior.
*/
char *i_prompt(interface *it);


/*
    Function: char *i_prompt_set(interface *it, char *prompt)
    
    Changes the prompt of the command window of the interface it. Returns
    0 if the paramter it is invalid or if the interface has no command
    window, 1 if everything worked out fine.
    
    NOTE: the function will make a copy of the prompt for internal use. 
    After this call, the program will retain ownership of the parameter and 
    will be responsible to release it if necessary.
*/
int i_prompt_set(interface *it, char *prompt);


/*
    Function: int i_write(interface *it, int win_no, int r, int c, char *str, int wcrop)
    
        
    Writes the string str in the given position of the window win_no 
    of the interface. 
    
    Parameters:
    it:   interface structure on which the string is writte. If it is invalid,
          the function returns -1.
    win_no: number of the window in which the string is written. If invalid,
            the function doesn't write anything and returns -1.
    r:  row in which the string is written. If invalid, the function
        doesn't write anything and returns -1.
    c:  column in which the first character of the string is written. If 
        invalid, the function doesn't write anything and returns -1.
    str: string to be written
    wcrop: flag for word cropping.          
            If the string str is too long to be written in the window, it will
            be cropped so that only the portion that fits in the window will be
            written. If wcrop=0, everything that fits in the window will be
            written even if this means breaking a word. If wcrop=1 only the 
            words that fit entirely in the window will be written.
    
            For example, is str is "Nothing to fear but fear itself"
    
            and the available space is 13 characters, a call with wcrop=0 
            will write
    
            Nothing to fe
    
            while one with wcrop=1 will write
    
            Nothing to


    The function returns the number of characters actually written, or -1
    if some parameter was invalid.
*/
int i_write(interface *it, int win_no, int r, int c, char *str, int wcrop);
 

/*
    Function: int i_write_wrap(interface *it, int win_no, int r, int c, char *str)
    
    Writes a string in the window number win_no of the interface it wrapping
    it and displaying in several lines if it is too long to fit in the window.
    If there are too many lines to fit in the window, then only the lines that
    fit will be displayed. 
    
        Parameters:
    it:   interface structure on which the string is writte. If it is invalid,
          the function returns -1.
    win_no: number of the window in which the string is written. If invalid,
            the function doesn't write anything and returns -1.
    r:  row in which the string is written. If invalid, the function
        doesn't write anything and returns -1.
    c:  column in which the first character of the string is written. If 
        invalid, the function doesn't write anything and returns -1.
    str: string to be written

    Example: consider the call
    
    i_write_wrap(it, 0, 
                0, 0, 
                "Full many a gem of purest ray serene the dark unfathomed caves of ocean bear")

    If the window has 20 columns and at least four rows, the function will 
    print 

    Full many a gem of
    purest ray serene
    the dark unfathomed
    caves of ocean bear

    If the window has two rows, the result will be 

    Full many a gem of
    purest ray serene

    Note that leading and trailing blanks will be removed from all the lines.

    Returns the number of lines written, 0 if some parameter is invalid.

*/
int i_write_wrap(interface *it, int win_no, int r, int c, char *str);

/*
    Function: void i_erase(interface *it)

    Clears the screen and redraws the interface it. Note that, unlike
    i_redraw, this function clears the whole terminal screen. The difference
    is evident only if the interface does not cover the whole screen.
*/
void i_erase(interface *it);


/*
    Function: void i_redraw(interface *it)

    Eedraws the interface it. Note that, unlike i_erase, this function 
    does no clear the whole terminal screen. The difference
    is evident only if the interface does not cover the whole screen.
*/
void i_redraw(interface *it);


/*
    Temporary function, defined in interface.c, that debugs the interface.
    The function will be redefined during development and in the end will be 
    a complete test suite for the module
*/
void i_debug();

#endif 

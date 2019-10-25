/*
    Project: demo game for PPROG
    File: interface.c
    Rev:  1.0
    Date: 9/10/2019
    
    
    See interface.h for detailed comments on the general characteristics
    and funcionality of this module.
    
*/  
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <termios.h>
#include "interface.h"
#include "lineread.h"

/* 
    Structure that contains the information relative to a window
*/
typedef struct {
    int r1;   // first row (0 is the top of the screen)
    int c1;   // first column (0 is the leftmost)
    int nr;   // number of rows
    int nc;   // numbre of columns
    int bg;   // background color
    int fg;   // foreground color
    int active; // active flag (writing is tuend off if inactive)
    int id;     // identifier of the window
    char *prompt; // prompt for the command window.
    int is_command; // flag to indicate that this is a command window
} window;


struct _interface {
    int version;   // Version number multiplied by 100 (1.0 --> 100)
    int bg;        // General background color (out of the windows)
    int fg;        // General foreground color (out of the windows)
    int n_window;  // Number of windows in the interface
    window **w;    // pointers to the windows 
    window *cmd;   // command window 
    
    int schema_r;  // rows of the general schema 
    int schema_c;  // columns of the general schema 
    unsigned char **schema; // drawing of the general schema 
};

/* Maximum acceptable code for colors */
#define MAX_COLOR_CODE   7

/*
    Tokens for the interface File
*/
#define INTERFACE   "INTERFACE"
#define WINNUMBER   "WINNUMBER"
#define WINDOW      "WINDOW"
#define COLOR       "COLOR"
#define COMMAND     "COMMAND"
#define GLOBALCOLOR "GLOBALCOLOR"
#define SCHEMA      "SCHEMA"
#define ENDINTERFACE "ENDINTERFACE"
#define PROMPT       "PROMPT"

/***********************************************************************
 *
 *    INTERNAL FUNCTIONS TO CREATE WINDOWS AND DISPLAYING ON THEM
 * 
 **********************************************************************/



/*
    Internal funcion window *_create(id, r1, c1, nr, nc, bg, fg)
    
    Creates a window for the screen and returns the relative structure.
    Note that this function simply creates the structure, it does not 
    do anything on the screen. In particular, it will not clear the 
    window area. For this, the program must call the function _clear.
    
    Parameters:
    id:      identifier of the window
    r1, c1:  upper-left corner of the window (r1, c1 >= 0)
    nr, nc:  number of rows and columns (nr, nc >= 1)
    bg, fg:  background and foreground colors (0<=bg, fg<=7)
    
    Returns:
    a pointer to a newly allocated window structure (the caller is 
    responsible for its release), or NULL if some parameter 
    is out of range.
*/
static window *_create(int id, int r1, int c1, int nr, int nc, int bg, int fg) {
    if (r1 < 0 || c1 < 0 || nr < 1 || nc < 1) 
        return NULL;
    if (bg < 0 || bg > MAX_COLOR_CODE || fg < 0 || fg > MAX_COLOR_CODE)
        return NULL;
    window *w = (window *) malloc(sizeof(window));

    w->id = id;
    w->r1 = r1;
    w->c1 = c1;
    w->nr = nr;
    w->nc = nc;
    w->fg = fg;
    w->bg = bg;
    w->active = 1;
    w->is_command = 0;
    return w;
}


/*
    Internal function:  void _clear(window *w)
    
    Clears a window filling it with blanks in the given 
    background color.
    
    Note: the values 30 and 40 added to teh colors distinguish the 
    background from the foreground.
    The 1 added to the positions is due to the fact that we begin counting
    rows and columns with 0 (as real computing scientists do...), while the 
    ANSI codes begin with 1.
*/
static void _clear(window *w) {
    char *buf;
    buf = (char *) malloc(w->nc + 1);
    memset(buf, ' ', w->nc);
    buf[w->nc] = 0;
    
    printf("%c[%d;%dm", 27, 30+w->fg, 40+w->bg);
    for (int r=w->r1; r<w->r1+w->nr; r++) {
        printf("%c[%d;%dH", 27, r+1, w->c1+1);
        printf("%s", buf);
    }
    if (w->is_command && w->prompt) {
        printf("%c[%d;%dH", 27, w->r1+1, w->c1+1);
        printf("%s", w->prompt);        
    }
    fflush(stdout);
}

/*
    Internal function: char *_crop(char *str, int, space, int wcrop)
    
    Auxiliary function of _winwrite. Crops the string str to fit in the 
    allotted space. The parameter wcrop controls the cropping, as explained 
    in _winwrite.
    
    The return value is a newly allocated string that the caller must release.
    
    NOTE: if wcrop = 1 and the portion of the string that fits in the allotted 
    space contains no blank, the behavior will be the same as wcrop=0, that is,
    the word will be broken.
*/
static char *_crop(char *str, int space, int wcrop) {
    char *buf = strdup(str);
    if (space>=strlen(str))
        return buf;
    char *p = buf + space;
    *p = 0;
    if (wcrop) {
        while (p>buf && !isspace(*p)) 
            p--;
        if (p==buf) {
            free(buf);
            return _crop(str, space, 0);
        }
        else {
            *p = 0;
        }
    }
    return buf;
}


/* 
    Internal function: int _winwrite(window *w, char *str, int row, int col, int wcrop)
    
    Writes a string beginning at a given position of the window.
    
    Parameters:
    w:   the window in which we write
    str: string to be written.
    row, col: coordinates at which the first character of str is
              placed, relative to the window. That is, row=0 col=3
              will begin writing at the upper-left corner of the window.
    wcrop: flag for word cropping (see below).          
    
    If the string str is too long to be written in the window, it will
    be cropped so that only the portion that fits in the window will be
    written. If wcrop=0, everything that fits in the window will be written,
    even if this means breaking a word. If wcrop=1 only the words that 
    fit entirely in the window will be written.
    
    For example, is str is "Nothing to fear but fear itself"
    
    and the available space is 13 characters, a call with wcrop=0 will write
    
    Nothing to fe
    
    while one with wcrop=1 will write
    
    Nothing to
    
    The function returns the number of characters actually written, or -1
    if some parameter was invalid.
*/
static int _winwrite(window *w, char *str, int row, int col, int wcrop) {
    if (row < 0 || col < 0 || row >= w->nr || col >= w->nc) 
        return -1;
    int space = w->nc - col;
    char *s = _crop(str, space, wcrop);
    printf("%c[%d;%dm", 27, 30+w->fg, 40+w->bg);
    printf("%c[%d;%dH", 27, w->r1+row+1, w->c1+col+1);
    printf("%s", s);
    int q = strlen(s);
    free(s);
    return q;
}

/* Internal function:  void _init(interface *it, int erase)

   Displays the empty interface on the screen.

    If erase=1, the function will erase the whole screen before 
    displaying the interface, if erase=0 the interface will be drawn over
    the contents of the screen. The result will be different only if the screen
    is larger than the inteface.
*/
static void _init(interface *it, int erase) {
    printf("%c[%d;%dm", 27, 30+it->fg, 40+it->bg);
    if (erase)
        printf("%c[2J", 27);
    printf("%c[1;1H", 27);
    
    for (int i=0; i<it->schema_r; i++) 
        printf("%s\n", it->schema[i]);
        
    for (int i=0; i<it->n_window; i++) 
        _clear(it->w[i]);

    if (it->cmd)
        _clear(it->cmd);

}


/*
    Internal function: char **_wrap(char *str, int space)
    
    Breaks a string into a series of strings (breaking it at word boundaries)
    in such a way that each line will fit into the given space.
    
    The function returns an array of pointers to the lines, each one 
    individually allocated. The final pointer is NULL, to indicate that
    the lines are finished. 
    
    The following example shows how to print and free the value returned
    by this function. 
    
    char **p = _wrap(str, space);
    
    char **q;
    for (q=p; *q, q++)
        printf("%s\n", *q);
        
    for (q=p; *q, q++)
        free(*q);
    free(p);
*/
char **_wrap(char *str, int space) {
    char *buf = strdup(str);
    char **lines = (char **) malloc(2*sizeof(char *));
    lines[0] = NULL;
    char *p = buf + strlen(buf) - 1;
    while (p>=buf && isspace(*p))   // remove trailing blanks
        p--;
    if (p == buf) {                // empty string
        free(buf);
        return lines;
    }
    *(p+1) = 0;
    int sz = strlen(buf);
    lines[0] = _crop(buf, space, 1);
    lines[1] = NULL; 
    int  c = strlen(lines[0]);
    int pt_no = 2;  // This includes the last pointer to NULL
    while (c<sz) {
        lines = realloc(lines, (++pt_no)*sizeof(char **));
        lines[pt_no-1] = NULL;
        int c1 = c;
        while(isspace(buf[c]))
            c++;
        int c2 = c;
        lines[pt_no-2] = _crop(buf+c, space, 1);
        c += strlen(lines[pt_no-2]);
    }
    free(buf);
    return lines;
}


/*
    Internal function: _do_plot(window *w, char **plot, int r)

    Receives an array of r strings (possible of various lenghts). Draws 
    it on the window w, centering it with respect to its borders. The 
    number of columns of the array will be considered as the length
    of the longest string.
    
    If the array is too big to fit in the window, it will be cropped 
    accordingly in all four sides to fit in the window and still be centered.

    Returns:
        1: the whole plot has been frawn
        2: the plot has been cropped
        0: error in the paramters or empty plot
        
    Note about the local variables: 
        r1, c1 are the row and column of the window where the plot begins
        pr, pc are the forst row and column of the plot that are being drawn
    
        If the plot fits in the window, then pr=pc=0 and r1, c1 have positive
        values. If the plot doewn´t fit, then r1=c1=0 and pr, pc have positive
        values.
*/
static int _do_plot(window *w, char **plot, int r) {
    int cols = 0;
    int r1, c1, pr, pc;
    if (!w || !plot) 
        return 0;
    for (int i=0; i<r; i++) {  
        if (!plot[i]) return 0;
        int q = strlen(plot[i]);
        cols = (q > cols) ? q : cols;
    }

    if (cols > w->nc) {
        c1 = 0;
        pc = (cols-w->nc) >> 1;
    }
    else {
        pc = 0;
        c1 = (w->nc-cols) >> 1;
    }
    
    if (r > w->nr) {
        r1 = 0;
        pr = (r-w->nr) >> 1;
    }
    else {
        pr = 0;
        r1 = (w->nr-r) >> 1;
    }

    int i, k;
    for (i=r1, k=0; i<w->nr && k<r; i++, k++) {
        _winwrite(w, plot[k]+pc, i, c1, 0);
    }

    // CHECK

    
}


/***********************************************************************
 *
 *    INTERNAL FUNCTIONS TO CREATE AN INTERFACE FROM A FILE
 * 
 **********************************************************************/

/*
    Internal function:  char **_tokenize(char *str)
    
    Tokenizes a string: returns a series of pointers to the individual
    words in the string.
    
    Several things to notice: the string is modified, so the function 
    doesn't function with a constant parameter. The array of pointers is 
    constituted by pointers that point to parts of the original string,
    so they should not be released individually. For example:
    
    char **ptr = _tokenize(str);
    
    To release the pointers and the string, simply do 
    
    free(ptr[0]);  // this releases str 
    free(ptr);     // this releases ptr
    
    The function returns the array ptr with an extra pointer at the end set to 
    NULL to mark the end of the tokens. That is, the following works:
    
    for (char **p = ptr; *p; p**)
        printf("%s\n", *p);
        
    The tokens must be separated by a comma and an arbitrary number of spaces
    (0 is allowed)

*/
static char **_tokenize(char *str) {
    int n_tok = 1;
    
    while (*str && isspace(*str))
        str++;                     // remove traililng blanks
    if (!*str) return NULL;
    
    for (char *q=str; *q; q++) {
        if (*q == ',') n_tok++;
    }
    
    n_tok++;
    char **ptr = (char **) malloc(n_tok*sizeof(char *));
    
    int idx = 0;
    ptr[idx++] = str;
    if (n_tok == 1) return ptr;

    for (char *q=str; *q; q++) {
        if (*q == ',') {
            *q++ = 0;
            while (*q && isspace(*q)) 
                *q++ = 0;
            if (*q) ptr[idx++] = q;
        }
        else if (isspace(*q)) {
            *q = 0;
        }
    }
    ptr[idx] = NULL;
    return ptr;
}

/* 
    Internal function:  window *_read_win(int id, FILE *fp)
    
    Auxiliary function of _read: given the id of a window, reads the 
    window data from an open stream, creates and returns the suitable 
    structure.
*/
static window *_read_win(int id, FILE *fp) {
    window *w = (window *) malloc(sizeof(window));
    w->id = id;
    w->fg = 0;
    w->bg = 7;
    w->is_command = 0;

    char *buf = fgetll(fp);
    char **p = _tokenize(buf);
    if (p == NULL || p[0] == NULL || p[1] == NULL || p[2] == NULL || p[3] == NULL) {
        fprintf(stdout, "Error in window limits; window %d, line:\n%s\n", id, buf);
        abort();
    }
    w->r1 = atoi(p[0]);     w->c1 = atoi(p[1]);
    w->nr = atoi(p[2]);     w->nc = atoi(p[3]);
    free(buf);
    free(p);  
    
    do {
        buf = fgetll(fp);
        if (!buf) {
            return w;
        }
        else if (!strncmp(buf, COLOR, strlen(COLOR))) {
            p = _tokenize(buf+strlen(COLOR));
            if (p == NULL || p[0] == NULL || p[1] == NULL) {
                fprintf(stdout, "Error in window colors; window %d, line:\n%s\n", id, buf);
                abort();
            }
            w->fg = atoi(p[0]);
            w->bg = atoi(p[1]);
            free(buf);
            free(p);
        }
        else if (!strncmp(buf, PROMPT, strlen(PROMPT))) {
            p = _tokenize(buf+strlen(PROMPT));
            if (p == NULL || p[0] == NULL) {
                fprintf(stdout, "Error in prompr specification; window %d, line:\n%s\n", id, buf);
                abort();
            }
            w->prompt = strdup(p[0]);
            free(buf);
            free(p);
        }
        else {
            ungetll(buf);
            return w;
        }
    } while(1);
    return w;
}

/*
    Internal function: _globalcolors(char *buf, interface *it)
    
    Auxiliary function of _read. Receives the line with the globalcolor 
    specification (the whole line, includig the keyword GLOBALCOLOR), and 
    sets the global color parameters in the interface structure.
    
    NOTE: This function does NOT release the buffer "buf".
*/
static void _globalcolor(char *buf, interface *it) {
    char **p = _tokenize(buf+strlen(GLOBALCOLOR));

    if (p == NULL || p[0] == NULL || p[1] == NULL) {
        fprintf(stdout, "Error in global colors; line:\n%s\n", buf);
        abort();
    }
    it->fg = atoi(p[0]);
    it->bg = atoi(p[1]);
    free(p);
    
    return;
}

/*
    Internal function: _schema(FILE *fp, interface *it)
    
    Auxiliary function of _read. Receives an open stream positioned after the 
    keyword SCHEMA. Reads the schema into the interface it.
*/
static void _schema(FILE *fp, interface *it) {
    char *buf = fgetll(fp);
    char **p = _tokenize(buf);
    if (p == NULL || p[0] == NULL || p[1] == NULL) {
        fprintf(stdout, "Error in schema sizes; line:\n%s\n", buf);
        abort();
    }
    it->schema_r = atoi(p[0]);
    it->schema_c = atoi(p[1]);

    it->schema = (unsigned char **) malloc (it->schema_r * sizeof(unsigned char *));
    for (int i=0; i<it->schema_r; i++) 
        it->schema[i] = fgetll(fp);
    return;
}


/*
    Internal function:  interface *_read(FILE *fp)
    
    Reads an interface from an open stream according to the format given in 
    interface.h. Returns an interface structure with all the data about the 
    interface.
*/
static interface *_read(FILE *fp) {
    interface *it = (interface *) malloc(sizeof (interface));
    char *buf = fgetll(fp);
    
    if (strncmp(buf, INTERFACE, strlen(INTERFACE))) {
        fprintf(stdout, "Abort: incorrect format in interface file. Line:\n%s\n", buf);
        abort();
    }
    it->version = (int) 100*atof(buf+strlen(INTERFACE));
    free(buf);
    buf = fgetll(fp);
    if (strncmp(buf, WINNUMBER, strlen(WINNUMBER))) {
        fprintf(stdout, "Abort: incorrect format in interface file. Line:\n%s\n", buf);
        abort();
    }
    it->n_window = atoi(buf+strlen(WINNUMBER));
    it->w = (window **) malloc(it->n_window*sizeof(window *));
    it->cmd = NULL; 
    free(buf);
    int n_w = 0;   // index to assign windows in the w array

    do {
        buf = fgetll(fp);
        if (!strncmp(buf, WINDOW, strlen(WINDOW))) {
            if (n_w >= it->n_window) {
                fprintf(stdout, "Abort: Too many windows declared (%d expected)\n", it->n_window);
                abort();
            }
            it->w[n_w++] = _read_win(atoi(buf+strlen(WINDOW)), fp);
            free(buf);
        }
        else if (!strncmp(buf, COMMAND, strlen(COMMAND))) {
            it->cmd = _read_win(-1, fp);
            it->cmd->is_command = 1;
            free(buf);
        }
        else if (!strncmp(buf, GLOBALCOLOR, strlen(GLOBALCOLOR))) {
            _globalcolor(buf, it);
            free(buf);
        }
        else if (!strncmp(buf, SCHEMA, strlen(SCHEMA))) {
            _schema(fp, it);
            free(buf);
        }
        else if (!strncmp(buf, ENDINTERFACE, strlen(ENDINTERFACE))) {
            free(buf);
            return it;
        }
        else {
            printf("unknown line: %s\nskipped\n", buf);
            free(buf);
        }
    } while(1);

    return it;
    
}

/***********************************************************************
 *
 *    INTERNAL FUNCTIONS TO READ A COMMAND IN THE COMMAND WINDOW
 * 
 **********************************************************************/

/*
    Internal function: _term_init()
    
    Auxiliary function of _cmd_read, prepares the terminal in such a way that
    keys are served without buffering and without echo on the screen.
*/
struct termios initial;
static void _term_init() {
	struct termios new; /*a termios structure contains a set of attributes about 
					    how the terminal scans and outputs data*/
		
	tcgetattr(fileno(stdin), &initial);    /*first we get the current settings of out 
						                  terminal (fileno returns the file descriptor 
						                  of stdin) and save them in initial. We'd better 
						                    restore them later on*/
	new = initial;	    /*then we copy them into another one, as we aren't going 
						to change ALL the values. We'll keep the rest the same */
	new.c_lflag &= ~ICANON;     /*here we are setting up new. This line tells to stop the 
						        canonical mode (which means waiting for the user to press 
						        enter before sending)*/
	new.c_lflag &= ~ECHO;  /*by deactivating echo, we tell the terminal NOT TO 
						    show the characters the user is pressing*/
	new.c_cc[VMIN] = 1;    /*this states the minimum number of characters we have 
					       to receive before sending is 1 (it means we won't wait 
					       for the user to press 2,3... letters)*/
	new.c_cc[VTIME] = 0;  /*I really have no clue what this does, it must be somewhere in the book tho*/
	new.c_lflag &= ~ISIG; /*here we discard signals: the program won't end even if we 
						    press Ctrl+C or we tell it to finish*/

	tcsetattr(fileno(stdin), TCSANOW, &new);  /*now we SET the attributes stored in new to the 
						    terminal. TCSANOW tells the program not to wait 
						    before making this change*/
}


/*
    Internal function: char *_cmd_read(window *w)
    
    Reads a string from the terminal inside a window. The input is managed
    so that it won't go outside the window. In this version, only the printable
    characters and the DELETE key are accepted.
    
    The string returnd is a newly allocated buffer that must be released by the caller.
*/
static char *_cmd_read(window *w) {
    if (!w->is_command) return NULL;
    _clear(w);
    printf("%c[%d;%dH", 27, w->r1+1, w->c1+1);
    printf("%s", w->prompt);        

    int max_len = w->nc - strlen(w->prompt) - 2;
    int begin = w->c1 + strlen(w->prompt) + 2;
    char *res = (char *) malloc((max_len+1)*sizeof(char));
    int cur = 0;
    
    _term_init();
    while(1) {
        printf("%c[%d;%dH", 27, w->r1+1, begin+cur);
        char ch = fgetc(stdin);
        if (ch == '\n') {
            tcsetattr(fileno(stdin), TCSANOW, &initial);
            res[cur] = 0;
            return res;
        }
        else if (ch == 127) {
            if (cur > 0) {
                cur--;
                printf("%c[%d;%dH ", 27, w->r1+1, begin+cur);
                res[cur] = 0;
            }
        }
        else if (ch >= 32) {
            res[cur] = ch;
            printf("%c", ch);
            if (cur < max_len) cur++;
        }
    }
}

/***********************************************************************
 *
 *                      PUBLIC FUNCTIONS
 * 
 **********************************************************************/

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
interface *i_create(FILE *fp) {
    if (!fp) return NULL;
    return _read(fp);
}    


/*
    Function i_window_no(interface *it)
    
    Returns the number of windows in an interface. Returns -1 if the paramter
    is invalid.
*/
int i_window_no(interface *it) {
    if (!it) return -1;
    return it->n_window;
}



/*
    Function i_win_rows(interface *it, int win_no)
    
    Returns the number of rows of the window number win_no of the interface it.
    Returns -1 if the window doesn't exist or the parameter it is invalid
*/
int i_win_rows(interface *it, int win_no) {
    if (!it) return -1;
    if (win_no < 0 || win_no >= it->n_window) return -1;
    
    return it->w[win_no]->nr;
}

/*
    Function i_win_cols(interface *it, int win_no)
    
    Returns the number of columns of the window number win_no of the interface it.
    Returns -1 if the window doesn't exist or the parameter it is invalid
*/
int i_win_cols(interface *it, int win_no) {
    if (!it) return -1;
    if (win_no < 0 || win_no >= it->n_window) return -1;
    
    return it->w[win_no]->nc;
}


/*
    Function i_win_bg(interface *it, int win_no)
    
    Returns the background color of the window number win_no of the interface it.
    Returns -1 if the window doesn't exist or the parameter it is invalid
*/
int i_win_bg(interface *it, int win_no) {
    if (!it) return -1;
    if (win_no < 0 || win_no >= it->n_window) return -1;
    
    return it->w[win_no]->bg;
}


/*
    Function i_win_fg(interface *it, int win_no)
    
    Returns the foreground color of the window number win_no of the interface it.
    Returns -1 if the window doesn't exist or the parameter it is invalid
*/
int i_win_fg(interface *it, int win_no) {
    if (!it) return -1;
    if (win_no < 0 || win_no >= it->n_window) return -1;
    
    return it->w[win_no]->fg;
}

/*
    Function i_win_bg_set(interface *it, int win_no, int color)
    
    Changes the background color of the window number win_no of the interface it.
    Returns the previous background color.
    Returns -1 if the window doesn't exist, the parameter it is invalid, or the 
    color is not in the range [0,..,7]
*/
int i_win_bg_set(interface *it, int win_no, int color) {
    if (!it) return -1;
    if (win_no < 0 || win_no >= it->n_window) return -1;
    if (color < 0 || color > 7) return -1;

    int ret = it->w[win_no]->bg;    
    it->w[win_no]->bg = color;
    return ret;
} 

/*
    Function i_win_fg_set(interface *it, int win_no, int color)
    
    Changes the background color of the window number win_no of the interface it.
    Returns the previous background color.
    Returns -1 if the window doesn't exist, the parameter it is invalid, or the 
    color is not in the range [0,..,7]
*/
int i_win_fg_set(interface *it, int win_no, int color) {
    if (!it) return -1;
    if (win_no < 0 || win_no >= it->n_window) return -1;
    if (color < 0 || color > 7) return -1;

    int ret = it->w[win_no]->fg;    
    it->w[win_no]->fg = color;
    return ret;
} 


/*
    Function: char *i_prompt(interface *it)
    
    Returns the prompt of the command window of the interface it. Returns
    NULL if the paramter it is invalid or if the interface has no command
    window.
    
    NOTE: the function returns a pointer wo the actual prompt: the contents
    of the returned value should not be changed in any way. Doing so will
    result in erratic behavior.
*/
char *i_prompt(interface *it) {
    if (!it) return NULL;
    if (!it->cmd) return NULL;
    return it->cmd->prompt;
}


/*
    Function: char *i_prompt_set(interface *it, char *prompt)
    
    Changes the prompt of the command window of the interface it. Returns
    0 if the paramter it is invalid or if the interface has no command
    window, 1 if everything worked out fine.
    
    NOTE: the function will make a copy of the prompt for internal use. 
    After this call, the program will retain ownership of the parameter and 
    will be responsible to release it if necessary.
*/
int i_prompt_set(interface *it, char *prompt) {
    if (!it) return 0;
    if (!it->cmd) return 0;
    if (it->cmd->prompt) free(it->cmd->prompt);
    it->cmd->prompt = strdup(prompt);
}


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
int i_write(interface *it, int win_no, int r, int c, char *str, int wcrop) {
    if (!it) return -1;
    if (win_no < 0 || win_no >= it->n_window) return -1;
    return _winwrite(it->w[win_no], str, r, c, wcrop);
}
 

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
int i_write_wrap(interface *it, int win_no, int r, int c, char *str) {
    if (!it) return -1;
    if (win_no < 0 || win_no >= it->n_window) return -1;
    window *w = it->w[win_no];
    if (r<0 || c<0 || r>=it->w[win_no]->nr || c>=it->w[win_no]->nc) return 0;
    int space = it->w[win_no]->nc - c;
   
    char **lns = _wrap(str, space);
    char **l = lns;
    int written = 0;
    while (*l && _winwrite(it->w[win_no], *l, r+written, c, 0) >= 0) {
        l++;
        written++;
    }
    for (l=lns; *l; l++)
        free(*l);
    free(lns);
    return written;
}



/*
    Function: void i_erase(interface *it)

    Clears the screen and redraws the interface it. Note that, unlike
    i_redraw, this function clears the whole terminal screen. The difference
    is evident only if the interface does not cover the whole screen.
*/
void i_erase(interface *it) {
    if (!it) return;
    _init(it, 1);
    return;
}


/*
    Function: void i_redraw(interface *it)

    Eedraws the interface it. Note that, unlike i_erase, this function 
    does no clear the whole terminal screen. The difference
    is evident only if the interface does not cover the whole screen.
*/
void i_redraw(interface *it) {
    if (!it) return;
    _init(it, 0);
    return;
}


/*
    Public function: i_plot(interface *it, int win_no, char **plot, int r)

    Receives an array of r strings (possible of various lenghts). Draws 
    it on the window win_no of the interface it, centering it with respect to 
    the borders of the window. The number of columns of the array will be 
    considered as the length of the longest string.
    
    If the array is too big to fit in the window, it will be cropped 
    accordingly in all four sides to fit in the window and still be centered.

    Returns:
        1: the whole plot has been frawn
        2: the plot has been cropped
        0: error in the paramters or empty plot
        
    Note about the local variables: 
        r1, c1 are the row and column of the window where the plot begins
        pr, pc are the forst row and column of the plot that are being drawn
    
        If the plot fits in the window, then pr=pc=0 and r1, c1 have positive
        values. If the plot doewn´t fit, then r1=c1=0 and pr, pc have positive
        values.
*/
int i_plot(interface *it, int win_no, char **plot, int r) {
    if (!it) return 0;
    if (win_no < 0 || win_no >= it->n_window) return 0;
    return _do_plot(it->w[win_no], plot, r);    
}


/*
    Temporary function, defined in interface.c, that debugs the interface.
    The function will be redefined during development and in the end will be 
    a complete test suite for the module
*/
void i_debug() {
    char *lines[4];
    lines[0] = strdup("*******************************");
    lines[1] = strdup("*******************************");
    lines[2] = strdup("*******************************");
    lines[3] = strdup("*******************************");

    
    printf("%c[2J", 27);
    FILE *fp = fopen("test.txt", "r");
    interface *it = _read(fp);
    _init(it, 1);
    _winwrite(it->w[0], "A spectre is haunting Europe. The spectre of communism", 3, 2, 1);
    i_plot(it, 2, lines, 3);
    while(1) {
        char *cmd = _cmd_read(it->cmd);
        if (!strcmp(cmd, "quit")) {
            free(cmd);
            return;
        }
        else {
            char buf[100];
            _clear(it->w[0]);
            int r = i_write_wrap(it, 0, 3, 5, cmd);
            sprintf(buf, "lines: %d", r);
            free(cmd);
            _clear(it->w[1]);
            i_write(it, 1, 1, 2, buf, 0);
        }
   }
    return;
}
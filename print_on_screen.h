#ifndef print_on_screen_h
#define print_on_screen_h

int change_color(char *color);
int change_cursor(int x, int y);
int print_on_position(int x, int y, char *text);

#endif
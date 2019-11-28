#include "read_keys.h"
#include <stdio.h>

char read_key() {
  char choice;
  choice = fgetc(stdin);


  if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
    choice = fgetc(stdin);

    switch(choice) {
    case('A'):
      return 'w';
    case('B'):
      return 's';
    case('C'):
      return 'd';
    case('D'):
      return 'a';
    default:
      return ' ';
    }
  }
  else
    return choice;
}
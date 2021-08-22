#include<stdbool.h>

#ifndef ACTIVATOR
#define ACTIVATOR
typedef struct activator{
  int pad_x;
  int pad_y;
  unsigned char id;
  unsigned char event_class;
  unsigned char state;
}activator;
#endif

#ifndef KEY
#define KEY
typedef struct key{
  char* keyboard_command;
  struct activator* activate;
  struct activator* deactivate;
  int id;
  bool is_pressed;
}key;
#endif

#ifndef KEYBOARD
#define KEYBOARD
typedef struct keyboard{
  key** keys;
  unsigned int num_of_keys;
}keyboard;
#endif

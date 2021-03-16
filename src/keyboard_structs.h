#include<stdbool.h>
#include"animation_structs.h"

#ifndef ACTIVATOR
#define ACTIVATOR
typedef struct activator{
  unsigned char event_class;
  int pad_x;
  int pad_y;
  unsigned char id;
  unsigned char state;
}activator;
#endif

#ifndef KEY
#define KEY
typedef struct key{
  char* keyboard_command;
  bool is_pressed;
  struct activator* activate;
  struct activator* deactivate;
  animation* animation_action;
  int animation_type;
  int animation_x;
  int animation_y;
  unsigned char animation_color;
  unsigned int animation_layer;
  int id;
}key;
#endif

#ifndef KEYBOARD
#define KEYBOARD
typedef struct keyboard{

  key** keys;
  unsigned int num_of_keys;
}keyboard;
#endif

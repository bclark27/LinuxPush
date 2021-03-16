#include<stdbool.h>
#include"animation_structs.h"
#include"animation_triger_structs.h"

#ifndef LAYER
#define LAYER
typedef struct layer{
  struct layer* next_layer;
  struct layer* prev_layer;
  unsigned char* pad_color_const;
  unsigned char* pad_blink_const;
  int layer_num;
  bool is_active;
  bool is_in_trash;
  int width;
  int height;
  animation* animation_head;
  animation_triger* animation_triger_head;
}layer;
#endif

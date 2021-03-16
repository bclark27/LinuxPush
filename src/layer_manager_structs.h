#include"layer_structs.h"
#include<stdbool.h>

#ifndef LAYER_MANAGER_STRUCT
#define LAYER_MANAGER_STRUCT
typedef struct layer_manager{
  unsigned char* color_buff;
  unsigned char* blink_buff;
  layer* layer_trash_head;
  layer* layers;
  int width;
  int height;
  bool cleared;
}layer_manager;
#endif

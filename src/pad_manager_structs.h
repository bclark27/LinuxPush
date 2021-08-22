#include"layer_structs.h"
#include"linked_list_structs.h"

#ifndef PAD_MANAGER_STRUCTS
#define PAD_MANAGER_STRUCTS
typedef struct pad_manager{
  layer** layer_array;
  unsigned char* color_buff;
  unsigned char* blink_buff;
  unsigned int layer_array_size;
  unsigned int size_x;
  unsigned int size_y;
  unsigned int active_layers_count;

}pad_manager;
#endif

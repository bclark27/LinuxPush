#include<stdio.h>
#include<stdlib.h>
#include"animation_triger_structs.h"
#include"animation_structs.h"
#include"animation.h"
#include"event_structs.h"

animation_triger* create_animation_triger(int animation_type, int min_x, int max_x, int min_y, int max_y, unsigned char animation_color, int id){
  animation_triger* at = (animation_triger*)malloc(sizeof(animation_triger));
  at->next_animation_triger = NULL;
  at->prev_animation_triger = NULL;
  at->animation_type = animation_type;
  at->max_x = max_x;
  at->min_x = min_x;
  at->max_y = max_y;
  at->min_y = min_y;
  at->animation_color = animation_color;
  at->id = id;
  return at;
}

animation* parse_event_triger(animation_triger* at, event* e){
  if(e->pad_x >= at->min_x &&
    e->pad_x < at->max_x &&
    e->pad_y >= at->min_y &&
    e->pad_y < at->max_y){
    animation* a = create_animation(e->pad_x, e->pad_y, at->animation_color, at->animation_type);
    return a;
  }
  return NULL;
}

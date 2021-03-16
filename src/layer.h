#include"layer_structs.h"
#include"event_structs.h"

#ifndef LAYER_FUNCTIONS
#define LAYER_FUNCTIONS
layer* create_layer(int width, int height, int layer_num);
void free_layer(layer* l);
void add_animation_in_to_struct(layer* l, animation* a);
void update_animations(layer* l, unsigned char* color_buff, unsigned char* blink_buff);
void mark_to_delete(layer* l);
void clear_background_layer(layer* l);
void add_animation_triger(layer* l, animation_triger* at);
void parse_event_layer(layer* l, event* e);
animation_triger* get_animation_triger_layer(layer* l, int id);
#endif

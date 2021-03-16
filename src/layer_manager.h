#include"layer_manager_structs.h"
#include"layer.h"
#include"event_structs.h"

#ifndef LAYER_MANAGER_FUNTIONS
#define LAYER_MANAGER_FUNTIONS
layer_manager* create_layer_manager(int width, int height);
void update_layers(layer_manager* lm);
int add_animation_to_layer(layer_manager* lm, animation* a, int layer_num);
void clean_trash(layer_manager* lm);
void create_new_layer(layer_manager* lm, int layer_num);
void remove_layer_from_manager(layer_manager* lm, int layer_num);
layer* get_layer_from_manager(layer_manager* lm, int layer_num);
void clear_layer_backgrounds(layer_manager* lm);
void add_animation_triger_to_manager(layer_manager* lm, animation_triger* at, int layer_num);
void parse_event_lm(layer_manager* lm, event* e);
animation_triger* get_animation_triger_lm(layer_manager* lm, int layer_num, int id);
#endif

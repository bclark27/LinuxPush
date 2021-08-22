#include"pad_manager_structs.h"
#include"event_structs.h"

#ifndef PAD_MANAGER_FUNCTIONS
#define PAD_MANAGER_FUNCTIONS
pad_manager* create_pad_manager(unsigned int size_x, unsigned int size_y, unsigned int num_of_layers);
void new_layer(pad_manager* pm, unsigned int layer_num);
void move_layer(pad_manager* pm, unsigned int prev_position, unsigned int new_position);
void swap_layers(pad_manager* pm, unsigned int a, unsigned int b);
void delete_layer(pad_manager* pm, unsigned int layer_num);
void free_pad_manager(pad_manager* pm);

void update_layers(pad_manager* pm);
void parse_event_pm(pad_manager* pm, event* e);
#endif

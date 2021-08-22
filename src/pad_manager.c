#include"pad_manager_structs.h"
#include"layer_structs.h"
#include"layer.h"
#include"stdlib.h"
#include"colors.h"
#include"linked_list.h"
#include"linked_list_structs.h"
#include"event_structs.h"
#include"stdio.h"

pad_manager* create_pad_manager(unsigned int size_x, unsigned int size_y, unsigned int num_of_layers){
    pad_manager* pm = (pad_manager*)malloc(sizeof(pad_manager));
    pm->layer_array_size = num_of_layers;//default layer amount
    pm->layer_array = (layer**)malloc(sizeof(layer*) * pm->layer_array_size);

    for(int i = 0; i < pm->layer_array_size; i++){
      pm->layer_array[i] = NULL;
    }

    pm->active_layers_count = 1;
    pm->size_x = size_x;
    pm->size_y = size_y;

    pm->color_buff = (unsigned char*)malloc(size_x * size_y);
    pm->blink_buff = (unsigned char*)malloc(size_x * size_y);

    for(int i = 0; i < size_x * size_y; i++){
      pm->color_buff[i] = BLACK;
      pm->blink_buff[i] = BLINK_OFF;
    }
    
    return pm;
}

void move_layer(pad_manager* pm, unsigned int prev_position, unsigned int new_position){
  if(pm == NULL || prev_position >= pm->layer_array_size || prev_position < 0
                || new_position >= pm->layer_array_size || new_position < 0) return;

  if(pm->layer_array[new_position] != NULL){
    free_layer(pm->layer_array[new_position]);
    pm->active_layers_count--;
  }
  pm->layer_array[new_position] = pm->layer_array[prev_position];
  pm->layer_array[prev_position] = NULL;
}

void swap_layers(pad_manager* pm, unsigned int a, unsigned int b){
  if(pm == NULL || a >= pm->layer_array_size || a < 0
                || b >= pm->layer_array_size || b < 0) return;
  layer* a_ptr = pm->layer_array[a];
  layer* b_ptr = pm->layer_array[b];
  pm->layer_array[a] = b_ptr;
  pm->layer_array[b] = a_ptr;
}

void delete_layer(pad_manager* pm, unsigned int layer_num){
  if(pm == NULL || layer_num >= pm->layer_array_size || layer_num < 0 || pm->layer_array[layer_num] == NULL) return;

  free_layer(pm->layer_array[layer_num]);
  pm->layer_array[layer_num] = NULL;
  pm->active_layers_count--;

}

void new_layer(pad_manager* pm, unsigned int layer_num){
  if(pm == NULL || layer_num >= pm->layer_array_size || layer_num < 0) return;

  if(pm->layer_array[layer_num] == NULL){
    pm->active_layers_count++;
  } else {
    delete_layer(pm, layer_num);
  }

  layer* l = create_layer(pm->size_x, pm->size_y);
  pm->layer_array[layer_num] = l;
}

void free_pad_manager(pad_manager* pm){
  if(pm == NULL) return;
  free(pm->color_buff);
  free(pm->blink_buff);
  for(int i = 0; i < pm->layer_array_size; i++){
    if(pm->layer_array[i] != NULL) free_layer(pm->layer_array[i]);
  }

  free(pm);
}

void update_layers(pad_manager* pm){
  if(pm == NULL) return;

  for(int x = 0; x < pm->size_x; x++){
    for(int y = 0; y < pm->size_y; y++){
      pm->color_buff[x + y * pm->size_x] = CLEAR;
    }
  }

}

void parse_event_pm(pad_manager* pm, event* e){

}

#include"layer_structs.h"
#include"stdlib.h"
#include<stdbool.h>
#include"stdio.h"

layer* create_layer(unsigned int size_x, unsigned int size_y){
  layer* l = (layer*)malloc(sizeof(layer));
  l->size_x = size_x;
  l->size_y = size_y;
  l->is_active = true;
  return l;
}

void free_layer(layer* l){
  if(l == NULL) return;
  free(l);
}

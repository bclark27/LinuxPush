#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"layer_structs.h"
#include"layer.h"
#include"layer_manager_structs.h"
#include"animation_structs.h"
#include"animation.h"
#include"colors.h"
#include"event_structs.h"
#include"event.h"

layer_manager* create_layer_manager(int width, int height){
  layer_manager* lm = (layer_manager*)malloc(sizeof(layer_manager));
  lm->color_buff = (unsigned char*)malloc(width * height);
  lm->blink_buff = (unsigned char*)malloc(width * height);

  lm->width = width;
  lm->height = height;

  lm->layer_trash_head = NULL;
  lm->layers = NULL;

  lm->cleared = false;

  return lm;
}

layer* get_layer_from_manager(layer_manager* lm, int layer_num){
  layer* ptr = lm->layers;

  while(ptr != NULL){
    layer* next = ptr->next_layer;

    if(ptr->layer_num == layer_num){
      return ptr;
    }
    ptr = next;
  }
  return NULL;
}

void add_to_trash(layer_manager* lm, layer* l){
  l->prev_layer = NULL;
  if(lm->layer_trash_head == NULL){
    lm->layer_trash_head = l;
    l->next_layer = NULL;
    return;
  }
  l->next_layer = lm->layer_trash_head;
  lm->layer_trash_head->prev_layer = l;
  lm->layer_trash_head = l;
}

void remove_layer_from_manager(layer_manager* lm, int layer_num){
  layer* ptr = lm->layers;

  while(ptr != NULL){
    layer* next = ptr->next_layer;

    if(ptr->layer_num == layer_num){
      if(ptr->prev_layer == NULL && ptr->next_layer == NULL){
        lm->layer_trash_head = NULL;
        return;
      } else if(ptr->prev_layer == NULL){
        lm->layer_trash_head = ptr->next_layer;
      } else if(ptr->next_layer == NULL){
        ptr->prev_layer->next_layer = NULL;
      } else {
        ptr->prev_layer->next_layer = ptr->next_layer;
        ptr->next_layer->prev_layer = ptr->prev_layer;
      }
      mark_to_delete(ptr);
      add_to_trash(lm, ptr);
      return;
    }
    ptr = next;
  }
}

void create_new_layer(layer_manager* lm, int layer_num){

  layer* ptr = lm->layers;
  if(ptr == NULL){
    lm->layers = create_layer(lm->width, lm->height, layer_num);
    lm->layers->next_layer = NULL;
    lm->layers->prev_layer = NULL;
    return;
  }
  while(ptr != NULL){
    if(ptr->layer_num == layer_num) return;

    layer* next = ptr->next_layer;

    if(layer_num < ptr->layer_num){
      layer* l = create_layer(lm->width, lm->height, layer_num);
      if(ptr->prev_layer){
        l->next_layer = ptr;
        l->prev_layer = ptr->prev_layer;
        ptr->prev_layer->next_layer = l;
        ptr->prev_layer = l;
      } else {
        lm->layers = l;
        l->prev_layer = NULL;
        l->next_layer = ptr;
        ptr->prev_layer = l;
      }
      return;
    }
    if(next == NULL) break;
    ptr = next;
  }
  ptr->next_layer = create_layer(lm->width, lm->height, layer_num);
  ptr->next_layer->prev_layer = ptr;
  ptr->next_layer->next_layer = NULL;
}

void clean_trash(layer_manager* lm){
  if(lm->layer_trash_head == NULL) return;
  layer* ptr = lm->layer_trash_head;

  while(ptr != NULL){
    layer* next = ptr->next_layer;

    if(ptr->animation_head == NULL){
      if(ptr->prev_layer == NULL && ptr->next_layer == NULL){
        lm->layer_trash_head = NULL;
        return;
      } else if(ptr->prev_layer == NULL){
        lm->layer_trash_head = ptr->next_layer;
      } else if(ptr->next_layer == NULL){
        ptr->prev_layer->next_layer = NULL;
      } else {
        ptr->prev_layer->next_layer = ptr->next_layer;
        ptr->next_layer->prev_layer = ptr->prev_layer;
      }
      free_layer(ptr);
    } else {
      mark_to_delete(ptr);
    }
    ptr = next;
  }
}

void clear_layer_backgrounds(layer_manager* lm){
  layer* ptr = lm->layers;
  while(ptr != NULL){
    layer* next = ptr->next_layer;
    clear_background_layer(ptr);
    ptr = next;
  }
}

void update_layers(layer_manager* lm){
  if(!lm->cleared){
    for(int i = 0; i < lm->width * lm->height; i++){
      lm->color_buff[i] = 0;
      lm->blink_buff[i] = BLINK_OFF;
    }
    lm->cleared = true;
  }
  layer* ptr = lm->layers;
  if(ptr == NULL){
    return;
  }
  lm->cleared = false;
  while(ptr != NULL){
    layer* next = ptr->next_layer;
    for(int i = 0; i < lm->width * lm->height; i++){
      if(ptr->pad_color_const[i] != CLEAR){
        lm->color_buff[i] = ptr->pad_color_const[i];
        lm->blink_buff[i] = ptr->pad_blink_const[i];
      }
    }

    update_animations(ptr, lm->color_buff, lm->blink_buff);
    ptr = next;
  }
  clean_trash(lm);
}

int add_animation_to_layer(layer_manager* lm, animation* a, int layer_num){ //0 = fail 1 = yea
  layer* ptr = lm->layers;
  if(ptr == NULL){
    return 0;
  }
  while(ptr != NULL){
    layer* next = ptr->next_layer;
    if(ptr->layer_num == layer_num){
      add_animation_in_to_struct(ptr, a);
      return 1;
    }
    ptr = next;
  }
  return 0;
}

void add_animation_triger_to_manager(layer_manager* lm, animation_triger* at, int layer_num){
  layer* l = lm->layers;
  while(l != NULL){
    if(layer_num == l->layer_num){
      add_animation_triger(l, at);
      return;
    }
    l = l->next_layer;
  }
}

void parse_event_lm(layer_manager* lm, event* e){
  if(e->event_class != PAD_EVENT || e->pad_state != PAD_PRESS_EVENT) return;
  layer* l = lm->layers;

  while(l != NULL){
    parse_event_layer(l, e);
    l = l->next_layer;
  }
}

animation_triger* get_animation_triger_lm(layer_manager* lm, int layer_num, int id){
  if(lm == NULL) return NULL;
  layer* l = lm->layers;
  while(l != NULL){
    if(l->layer_num == layer_num){
      return get_animation_triger_layer(l, id);
    }
    l = l->next_layer;
  }
  return NULL;
}

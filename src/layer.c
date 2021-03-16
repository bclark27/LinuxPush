#include<stdlib.h>
#include<stdio.h>
#include"layer_structs.h"
#include"animation.h"
#include"animation_structs.h"
#include"colors.h"
#include"event_structs.h"
#include"animation_triger.h"
#include"animation_triger_structs.h"
#include"event.h"

layer* create_layer(int width, int height, int layer_num){
  layer* l = (layer*)malloc(sizeof(layer));
  //printf("%i\n", layer_num);
  l->next_layer = NULL;
  l->prev_layer = NULL;
  l->pad_color_const = (unsigned char*)malloc(width * height);
  l->pad_blink_const = (unsigned char*)malloc(width * height);
  for(int i = 0; i < width * height; i++){
    l->pad_color_const[i] = CLEAR;
    l->pad_blink_const[i] = BLINK_OFF;
  }
  l->layer_num = layer_num;
  l->is_active = true;
  l->is_in_trash = false;
  l->width = width;
  l->height = height;
  l->animation_head = NULL;
  l->animation_triger_head = NULL;
  return l;
}

void clear_background_layer(layer* l){
  for(int i = 0; i < l->width * l->height; i++){
    l->pad_color_const[i] = CLEAR;
    l->pad_blink_const[i] = BLINK_OFF;
  }
}

void add_animation_in_to_struct(layer* l, animation* a){
  a->prev_animation = NULL;
  if(l->animation_head == NULL){
    l->animation_head = a;
    a->next_animation = NULL;
    return;
  }

  a->next_animation = l->animation_head;
  l->animation_head->prev_animation = a;
  l->animation_head = a;
}

void free_layer(layer* l){
  free(l);
}

void _remove_animation(layer*l, animation* ptr){

  if(ptr->prev_animation == NULL && ptr->next_animation == NULL){
    l->animation_head = NULL;
    return;
  } else if(ptr->prev_animation == NULL){
    l->animation_head = ptr->next_animation;
  } else if(ptr->next_animation == NULL){
    ptr->prev_animation->next_animation = NULL;
  } else {
    ptr->prev_animation->next_animation = ptr->next_animation;
    ptr->next_animation->prev_animation = ptr->prev_animation;
  }
}

void update_animations(layer* l, unsigned char* color_buff, unsigned char* blink_buff){

  animation* ptr = l->animation_head;
  if(ptr == NULL){
    return;
  }
  int i = 0;
  while(ptr != NULL){
    animation* next = ptr->next_animation;

    if(ptr->done && ptr->ref_count <= 0){
      _remove_animation(l, ptr);
      free_animation(ptr);
    } else if(ptr->done && ptr->ref_count > 0){
      ptr->to_delete = true;
    } else if(!ptr->to_delete){
      update_animation(ptr);

      particle* p = ptr->particle_head;
      if(p != NULL){
        while(p != NULL){
          int pos = (int)p->x + ((int)p->y * l->height);
          if((int)p->x >= 0 && (int)p->x < l->width && (int)p->y >= 0 && (int)p->y < l->height){
            color_buff[pos] = p->color;
            blink_buff[pos] = BLINK_OFF;
          }
          p = p->next_particle;
        }
      }
    }
    ptr = next;
    i++;
  }
}

void mark_to_delete(layer* l){
  l->is_in_trash = true;
  l->is_active = false;
  animation* ptr = l->animation_head;
  if(ptr != NULL){
    while(ptr != NULL){
      animation* next = ptr->next_animation;
      if(ptr->ref_count <= 0){
        _remove_animation(l, ptr);
        free_animation(ptr);
      } else {
        ptr->to_delete = true;
      }
      ptr = next;
    }
  }
  animation_triger* ptr2 = l->animation_triger_head;
  if(ptr2 != NULL){
    while(ptr2 != NULL){
      animation_triger* next = ptr2->next_animation_triger;
      free(ptr2);
      ptr2 = next;
    }
  }
}

void add_animation_triger(layer* l, animation_triger* at){
  if(l->animation_triger_head == NULL){
    l->animation_triger_head = at;
    at->next_animation_triger = NULL;
    at->prev_animation_triger = NULL;
    return;
  }
  l->animation_triger_head->prev_animation_triger = at;
  at->next_animation_triger = l->animation_triger_head;
  at->prev_animation_triger = NULL;
  l->animation_triger_head = at;
}

void parse_event_layer(layer* l, event* e){
  if(l == NULL || e == NULL) return;
  animation_triger* at = l->animation_triger_head;
  while(at != NULL){
    animation* a = parse_event_triger(at, e);
    if(a){

      add_animation_in_to_struct(l, a);
    }
    at = at->next_animation_triger;
  }
}

animation_triger* get_animation_triger_layer(layer* l, int id){
  if(l == NULL) return NULL;
  animation_triger* at = l->animation_triger_head;
  while(at != NULL){
    if(at->id == id) return at;
    at = at->next_animation_triger;
  }
  return NULL;
}

#include<libusb-1.0/libusb.h>
#include<stdlib.h>
#include<string.h>
#include"usb.h"
#include"push_structs.h"
#include<stdlib.h>
#include<stdio.h>
#include"layer_structs.h"
#include"colors.h"
#include"signals.h"
#include"event.h"
#include"event_structs.h"
#include"keyboard_structs.h"
#include"keyboard.h"
#include"knob_structs.h"
#include"knob.h"
#include"stdbool.h"
#include"text_structs.h"
#include"text.h"
#include"animation.h"
#include"layer_manager_structs.h"
#include"layer_manager.h"
#include"layer_structs.h"
#include"layer.h"
#include"animation_triger_structs.h"
#include"animation_triger.h"

const int knob_text_len = 8;
const int chunk_size = 128;

animation_triger* get_animation_triger(push_device* push, int layer_num, int id){
  return get_animation_triger_lm(push->lm, layer_num, id);
}

void add_animation_triger_to_push(push_device* push, int animation_type, int min_x, int max_x, int min_y, int max_y, unsigned char animation_color, int layer_num, int id){
  animation_triger* at = create_animation_triger(animation_type, min_x, max_x, min_y, max_y, animation_color, id);
  add_animation_triger_to_manager(push->lm, at, layer_num);
}

void add_scrolling_text(push_device* push, int id, int x, int y, char* text, int size, float speed){
  scrolling_text* k = create_scrolling_text(id, x, y, text, size, speed);

  if(push->scrolling_text_objects == NULL){
    push->scrolling_text_objects = (scrolling_text**)malloc(sizeof(scrolling_text*));
    push->num_scrolling_text = 1;
    push->scrolling_text_objects[0] = k;
    return;
  }

  scrolling_text** prev = push->scrolling_text_objects;
  push->scrolling_text_objects = (scrolling_text**)malloc(sizeof(scrolling_text*) * push->num_scrolling_text);
  if(push->scrolling_text_objects == NULL) {printf("Malloc error\n"); return;}
  for(unsigned int i = 0; i < push->num_scrolling_text; i++){
    push->scrolling_text_objects[i] = prev[i];
  }

  push->scrolling_text_objects[push->num_scrolling_text] = k;
  push->num_scrolling_text++;
  free(prev);
}

void _place_text(push_device* push, char* text, int size, int x, int y){
  if(x > 67 || x < 0 || y < 0 || y > 3){
    return;
  }
  for(int i = 0; i < size && i + x < 68; i++){
    unsigned char out = (unsigned char) text[i];
    if(out == 0xff){out = 0x00;} //up arrow
    else if(out > 0x7f && out < 0xff){out = 0x20;}//death
    push->screen_text[y][i + x] = out;
  }
  push->text_change = true;
}

void place_text(push_device* push, char* text, int x, int y){
  int size = strlen(text);
  _place_text(push, text, size, x, y);
}

void update_all_scrolling_text(push_device* push){
  if(push->scrolling_text_objects == NULL) return;
  for(int i = 0; i < push->num_scrolling_text; i++){
    update_scrolling_text(push->scrolling_text_objects[i]);
    if(push->scrolling_text_objects[i]->dirty &&
      push->scrolling_text_objects[i]->active) place_text(push, (char*)push->scrolling_text_objects[i]->display_buf, push->scrolling_text_objects[i]->x, push->scrolling_text_objects[i]->y);
  }
}

void clear_text(push_device* push){
  if(push->scrolling_text_objects == NULL) return;
  for(int i = 0; i < 4; i++){
    _place_text(push, "                                                                    ", 68, 0, i);
  }
}

void place_text_knob_l(push_device* push, char* text, int knob, int y){
  if(knob > 7 || knob < 0){
    return;
  }
  int size = strlen(text);
  int x = (knob_text_len + 1) * ((knob + 1) / 2) + knob_text_len * (knob / 2); // gap between every other knob
  if (size > knob_text_len) size = knob_text_len;
  _place_text(push, text, size, x, y);
}

void place_text_knob(push_device* push, char* text, int knob, int y){
  char textr[8] = "        ";
  int size = strlen(text);
  if (size > knob_text_len) size = knob_text_len;
  strncpy(textr + 4 - (size / 2), text, size);
  place_text_knob_l(push, textr, knob, y);
}

void place_text_knob_r(push_device* push, char* text, int knob, int y){
  char textr[8] = "        ";
  int size = strlen(text);
  if (size > knob_text_len) size = knob_text_len;
  strncpy(textr + 8 - size, text, size);
  place_text_knob_l(push, textr, knob, y);
}

float read_knob(push_device* push, int refrence_id){
  if(push->knobs == NULL) return 0;
  for(unsigned int i = 0; i < push->num_knobs; i++){
    if(push->knobs[i]->refrence_id == refrence_id){
      return push->knobs[i]->value;
    }
  }
  return 0;
}

void add_knob(push_device* push, int refrence_id, int knob_id, float start_val, float min, float max, float step, bool loop){
  knob* k = create_knob(refrence_id, knob_id, start_val, min, max, step, loop);

  if(push->knobs == NULL){
    push->knobs = (knob**)malloc(sizeof(knob*));
    push->num_knobs = 1;
    push->knobs[0] = k;
    return;
  }

  knob** prev = push->knobs;
  push->knobs = (knob**)malloc(sizeof(knob*) * push->num_knobs);
  if(push->knobs == NULL) {printf("Malloc error\n"); return;}
  for(unsigned int i = 0; i < push->num_knobs; i++){
    push->knobs[i] = prev[i];
  }

  push->knobs[push->num_knobs] = k;
  push->num_knobs++;
  free(prev);
}

void add_pad_key_push(push_device* push, int key_id, char* keyboard_command, int layer, int pad_x, int pad_y, unsigned char color, int type, bool continues){
  add_pad_key(push, key_id, push->keyboard_manager, keyboard_command, layer, pad_x, pad_y, color, type, continues);
}

void add_btn_key_push(push_device* push, int key_id, char* keyboard_command, int id){
  add_btn_key(push, key_id, push->keyboard_manager, keyboard_command, id);
}

int add_animation(push_device* push, animation* a, int layer_num){
  return add_animation_to_layer(push->lm, a, layer_num);
}

void flatten_layers(push_device* push){

  int ny;
  int nx;
  int id;


  if(push->lm->layers == NULL){
    for(int x = 0; x < 8; x++){
      for(int y = 0; y < 8; y++){
        ny = y + push->dy;
        nx = x + push->dx;
        id = ny * push->height + nx;
        if(ny < 0 || nx < 0 || nx >= push->width || ny >= push->height){
          push->pad_color_new[y * 8 + x] = OUT_OF_BOUNDS_COLOR;
          push->pad_blink_new[y * 8 + x] = OUT_OF_BOUNDS_BLINK;
        } else {
          push->pad_color_new[y * 8 + x] = 0;
          push->pad_blink_new[y * 8 + x] = BLINK_OFF;
        }
      }
    }
    return;
  }

  update_layers(push->lm);

  for(int x = 0; x < 8; x++){
    for(int y = 0; y < 8; y++){
      ny = y + push->dy;
      nx = x + push->dx;
      id = ny * push->height + nx;
      if(ny < 0 || nx < 0 || nx >= push->width || ny >= push->height){
        push->pad_color_new[y * 8 + x] = OUT_OF_BOUNDS_COLOR;
        push->pad_blink_new[y * 8 + x] = OUT_OF_BOUNDS_BLINK;
      } else {
        push->pad_color_new[y * 8 + x] = push->lm->color_buff[id];
        push->pad_blink_new[y * 8 + x] = push->lm->blink_buff[id];
      }
    }
  }

}

void new_layer(push_device* push, int layer_num){
  create_new_layer(push->lm, layer_num);
}

void clear_all_backgrounds(push_device* push){
  clear_layer_backgrounds(push->lm);
}

void clear_layer_background(push_device* push, int layer_num){
  struct layer* l = get_layer_from_manager(push->lm, layer_num);
  if(l){
    clear_background_layer(l);
  }
}

void light_pad_id(push_device* push, int layer, unsigned char color, unsigned char blink, int id){

  if(push == NULL || id < 0 || id > push->width * push->height){
    return;
  }

  struct layer* l = get_layer_from_manager(push->lm, layer);
  if(l){
    l->pad_color_const[id] = color;
    l->pad_blink_const[id] = blink;
  }

}

void light_pad_xy(push_device* push, int layer, unsigned char color, unsigned char blink, int x, int y){

  if(x >= push->width || x < 0 || y >= push->height){
    return;
  }

  light_pad_id(push, layer, color, blink, x + y * 8);
}

void set_all_pads(push_device* push, int layer, unsigned char color, unsigned char blink){
  struct layer* l = get_layer_from_manager(push->lm, layer);
  if(l){
    for(int i = 0; i < l->width * l->height; i++){
      l->pad_color_const[i] = color;
      l->pad_blink_const[i] = blink;
    }
  }
}

void build_update(push_device* push){

  //EVERY TIME AN UPDATE IS BUILT THE OUT_SIGNAL IS UPDATED BUT ONLY TO CHANGE RECENT EVENTS SO AS TO SAVE SEND TIME
  unsigned int update_index = 0;  //index of the char currently on in the update

  //add colors to the update
  unsigned int val;
  for(int i = 0; i < 64; i++){

    if(push->pad_color_new[i] == CLEAR) push->pad_color_new[i] = 0;

    if( push->pad_color_new[i] != push->pad_color_curr[i] ||
        push->pad_blink_new[i] != push->pad_blink_curr[i] ||
        push->pad_status_new[i] != push->pad_status_curr[i])
    {

      val = 0;
      val ^= ((unsigned int)push->pad_color_new[i]) << 24;//color in byte 3
      val ^= (i + 0x24) << 16;//pad id in byte 2
      val ^= ((unsigned int)push->pad_blink_new[i]) << 8;
      val ^= ((unsigned int)push->pad_status_new[i]);//status in byte 0

      *(unsigned int*)(push->out_signal+update_index) = val;
      update_index += 4; //add 4 for an int

      //update the curr push data
      push->pad_color_curr[i] = push->pad_color_new[i];
      push->pad_blink_curr[i] = push->pad_blink_new[i];
      push->pad_status_curr[i] = push->pad_status_new[i];
    }
  }

  if(push->text_change){  //text render
    //printf("BUILDING TEXT\n");
    for(int i = 0; i < 4; i++){
      //dont touch
      *(push->out_signal+(update_index++)) = 0x04;
      *(push->out_signal+(update_index++)) = 0xf0;
      *(push->out_signal+(update_index++)) = 0x47;
      *(push->out_signal+(update_index++)) = 0x7f;

      *(push->out_signal+(update_index++)) = 0x04;
      *(push->out_signal+(update_index++)) = 0x15;
      *(push->out_signal+(update_index++)) = 0x18 + i;//line number + 24
      *(push->out_signal+(update_index++)) = 0x00;

      *(push->out_signal+(update_index++)) = 0x04;
      *(push->out_signal+(update_index++)) = 69;//size + 1
      *(push->out_signal+(update_index++)) = 0x00;//start x index
      *(push->out_signal+(update_index++)) = push->screen_text[i][0];

      //at the begining of each int is a 0x04 then some text bytes
      for(int k = 0; k < 22; k++){
        *(push->out_signal+(update_index++)) = 0x04;
        *(push->out_signal+(update_index++)) = push->screen_text[i][1 + (k * 3)];
        *(push->out_signal+(update_index++)) = push->screen_text[i][2 + (k * 3)];
        *(push->out_signal+(update_index++)) = push->screen_text[i][3 + (k * 3)];
      }

      //last int is prefixed with 0x06 then the last char
      *(push->out_signal+(update_index++)) = 0x06;
      *(push->out_signal+(update_index++)) = push->screen_text[i][67];
      *(push->out_signal+(update_index++)) = 0xf7;
      *(push->out_signal+(update_index++)) = 0x20;

    }
    push->text_change = false;
  }

  push->out_signal_size = update_index;
}

void send_update(push_device* push){
  //printf("sending %i bytes\n", push->out_signal_size);
  /*
  for(int i = 0; i < push->out_signal_size; ){
    printf("%02x ", push->out_signal[i++]);
    printf("%02x ", push->out_signal[i++]);
    printf("%02x ", push->out_signal[i++]);
    printf("%02x\n", push->out_signal[i++]);
  }
  */
  int i = 0;
  int size, left;
  while(i * chunk_size < push->out_signal_size){
    size = chunk_size;
    left = push->out_signal_size - (chunk_size * i);
    if(left < chunk_size){
      size = left;
    }
    send_data(push->device, push->out_signal + (chunk_size * i), size);
    i++;
  }
}

void get_update(push_device* push){
  int read = read_data(push->device, push->in_signal, push->in_max_size);
  //printf("INCOMING SIZE: %d\n", read);
  push->in_signal_size = read;
}

void update_push(push_device* push){
  update_all_scrolling_text(push);
  flatten_layers(push);
  build_update(push);
  send_update(push);
}

void queue_event(push_device* push, event* e){  //add event to the front of the queue
  if(push->event_head == NULL){
    push->event_head = e;
    push->event_tail = e;
    e->next_event = NULL;
    e->prev_event = NULL;
    return;
  }

  event* second_event = push->event_head;
  push->event_head = e;

  second_event->prev_event = e;
  e->next_event = second_event;
  e->prev_event = NULL;
}

event* pop_event(push_device* push){//remove event from the end of the queue
  if(push->event_tail == NULL){
    return NULL;
  }
  event* ret = push->event_tail;
  if(push->event_tail->prev_event == NULL){
    push->event_tail = NULL;
    push->event_head = NULL;
    ret->prev_event = NULL;
    ret->next_event = NULL;
    return ret;
  }
  ret->prev_event->next_event = NULL;
  push->event_tail = ret->prev_event;
  ret->prev_event = NULL;
  ret->next_event = NULL;
  return ret;
}

void parse_update(push_device* push){
  int update_int_len = push->in_signal_size / 4;  //get num of ints

  unsigned int* data_ptr = (unsigned int*)push->in_signal;

  for(int i = 0; i < update_int_len; i++){

    unsigned int data = data_ptr[i];
    if(data == 0) continue;
    event* e = create_event(data, push);
    queue_event(push, e);
  }

  push->in_signal_size = 0;
}

void process_events(push_device* push){
  if(push == NULL || push->event_head == NULL){
    return;
  }
  event* e = pop_event(push);
  while(e != NULL){

    //printf("0x%08x ", e->data);
    //STUFF HERE
    if(push->knobs){
      for(unsigned int i = 0; i < push->num_knobs; i++){
        update_knob(push->knobs[i], e);
      }
    }

    handle_key_event(push, e, push->keyboard_manager);
    parse_event_lm(push->lm, e);
    //print_event(e);
    e = pop_event(push);
  }
  //printf("\n");
}

void remove_layer(push_device* push, int layer_num){
  remove_layer_from_manager(push->lm, layer_num);
}

void receive_update(push_device* push){
  get_update(push);
  parse_update(push);
}

void free_events(push_device* push){
  event* a = pop_event(push);
  while(a != NULL){
    free_event(a);
    a = pop_event(push);
  }
}

void free_push(push_device* push){
  free_usb(push->device);

  free(push->pad_color_new);
  free(push->pad_blink_new);
  free(push->pad_status_new);

  free(push->pad_color_curr);
  free(push->pad_blink_curr);
  free(push->pad_status_curr);


  free(push->in_signal);
  free(push->out_signal);


  if(push->knobs){
    for(unsigned int i = 0; i < push->num_knobs; i++){
      free(push->knobs[i]);
    }
    free(push->knobs);
  }
  free_keyboard(push->keyboard_manager);
  free_events(push);
  free(push);
}

push_device* create_push_device(usb_device* dev, int width, int height){

  int PUSH_OUT_SIZE = 4096;
  int PUSH_IN_SIZE = 4096;
  push_device* push = (push_device*)malloc(sizeof(push_device));
  push->device = dev;

  push->pad_color_new = (unsigned char*)malloc(sizeof(unsigned char) * 64);
  push->pad_blink_new = (unsigned char*)malloc(sizeof(unsigned char) * 64);
  push->pad_status_new = (unsigned char*)malloc(sizeof(unsigned char) * 64);

  push->pad_color_curr = (unsigned char*)malloc(sizeof(unsigned char) * 64);
  push->pad_blink_curr = (unsigned char*)malloc(sizeof(unsigned char) * 64);
  push->pad_status_curr = (unsigned char*)malloc(sizeof(unsigned char) * 64);

  for(int i = 0; i < 64; i++){
    push->pad_color_new[i] = 0;
    push->pad_blink_new[i] = BLINK_OFF;
    push->pad_status_new[i] = DEFAULT;

    //MAKE CURR DIFFERENT IN ORDER TO FORCE INIT UPDATE
    push->pad_color_curr[i] = ~0;
    push->pad_blink_curr[i] = ~BLINK_OFF;
    push->pad_status_curr[i] = ~DEFAULT;
  }

  push->out_max_size = PUSH_OUT_SIZE;
  push->out_signal_size = 0;
  push->out_signal = (unsigned char*)malloc(PUSH_OUT_SIZE * sizeof(unsigned char*));

  push->in_max_size = PUSH_IN_SIZE;
  push->in_signal_size = 0;
  push->in_signal = (unsigned char*)malloc(PUSH_IN_SIZE * sizeof(unsigned char*));

  push->width = width;
  push->height = height;
  push->dx = 0;
  push->dy = 0;

  push->event_head = NULL;
  push->event_tail = NULL;

  push->keyboard_manager = create_keyboard();// = create_keyboard();

  push->knobs = NULL;
  push->num_knobs = 0;

  push->screen_text = (unsigned char**)malloc(sizeof(char*) * 4);
  for(int i = 0; i < 4; i++){
    push->screen_text[i] = (unsigned char*)malloc(sizeof(char) * 68);
    for(int k = 0; k < 68; k++){
      push->screen_text[i][k] = 0x20;
    }
  }
  push->text_change = true;

  push->scrolling_text_objects = NULL;
  push->num_scrolling_text = 0;

  push->lm = create_layer_manager(push->width, push->height);
  update_push(push);

  get_update(push);

  return push;
}

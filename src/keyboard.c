#include"keyboard_structs.h"
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include"event_structs.h"
#include"push_structs.h"
#include"animation.h"
#include"animation_structs.h"
#include"push.h"

bool check_activity(activator* a, event* e){

  if(a->event_class != e->event_class) return false;

  if(a->event_class == PAD_EVENT){  //check pad event
    if(a->state == e->pad_state && a->pad_x == e->pad_x && a->pad_y == e->pad_y){
      return true;
    }
  } else if(a->event_class == BUTTON_EVENT){  //check btn event
    if(a->state == e->btn_state && a->id == e->btn_id){
      return true;
    }
  }
  return false;
}


keyboard* create_keyboard(){
  keyboard* k = (keyboard*)malloc(sizeof(keyboard));

  k->keys = NULL;
  k->num_of_keys = 0;

  return k;
}

void add_key(keyboard* keyboard, key* new_key){
  if(keyboard->keys == NULL){
    keyboard->keys = (key**)malloc(sizeof(key*));
    keyboard->keys[0] = new_key;
    keyboard->num_of_keys = 1;
    return;
  }

  key** prev = keyboard->keys;
  keyboard->keys = (key**)malloc(sizeof(key*) * keyboard->num_of_keys);
  if(keyboard->keys == NULL) {printf("Malloc error\n"); return;}
  for(unsigned int i = 0; i < keyboard->num_of_keys; i++){
    keyboard->keys[i] = prev[i];
  }

  keyboard->keys[keyboard->num_of_keys] = new_key;
  keyboard->num_of_keys++;
  free(prev);
}

key* create_key(char* keyboard_command, int key_id, activator* activate, activator* deactivate){
  key* k = (key*)malloc(sizeof(key));
  k->is_pressed = false;
  k->keyboard_command = keyboard_command;
  k->activate = activate;
  k->deactivate = deactivate;
  k->id = key_id;
  k->animation_type = NO_ANIMATION;
  k->animation_action = NULL;
  return k;
}

void add_pad_key(push_device* push, int id, keyboard* keyboard, char* keyboard_command, int layer, int pad_x, int pad_y, unsigned char color, int type, bool continues){
  if(keyboard == NULL){
    return;
  }

  activator* a = (activator*)malloc(sizeof(activator));
  a->event_class = PAD_EVENT;
  a->pad_x = pad_x;
  a->pad_y = pad_y;
  a->state = PAD_PRESS_EVENT;

  activator* d = (activator*)malloc(sizeof(activator));
  d->event_class = PAD_EVENT;
  d->pad_x = pad_x;
  d->pad_y = pad_y;
  d->state = PAD_RELEASE_EVENT;

  key* new_key = create_key(keyboard_command, id, a, d);
  new_key->animation_type = type;
  new_key->animation_action = NULL;
  if(type != NO_ANIMATION){
    new_key->animation_x = pad_x;
    new_key->animation_y = pad_y;
    new_key->animation_layer = layer;
    new_key->animation_color = color;
  }
  add_key(keyboard, new_key);
}

void add_btn_key(push_device* push, int key_id, keyboard* keyboard, char* keyboard_command, int id){

  if(keyboard == NULL){
    return;
  }

  activator* a = (activator*)malloc(sizeof(activator));
  a->event_class = BUTTON_EVENT;
  a->id = id;
  a->state = BUTTON_PRESS_EVENT;

  activator* d = (activator*)malloc(sizeof(activator));
  d->event_class = BUTTON_EVENT;
  d->id = id;
  d->state = BUTTON_RELEASE_EVENT;

  key* new_key = create_key(keyboard_command, key_id, a, d);
  add_key(keyboard, new_key);
}

#define KEY_CMD_LEN 64
char KEY_UP_CMD_STR[KEY_CMD_LEN] = "xdotool keyup ";
char KEY_DOWN_CMD_STR[KEY_CMD_LEN] = "xdotool keydown ";

void preform_key_press(push_device* push, event* e, key* key){
  if(!key->is_pressed && check_activity(key->activate, e)){
    if(key->keyboard_command != NULL){
      for(unsigned int i = 0; i < strlen(key->keyboard_command); i++){
        KEY_DOWN_CMD_STR[i + 16] = key->keyboard_command[i];
      }
      KEY_DOWN_CMD_STR[16 + strlen(key->keyboard_command)] = '\n';
      for(unsigned int i = 16 + strlen(key->keyboard_command) + 1; i < KEY_CMD_LEN; i++){
        KEY_DOWN_CMD_STR[i] = 0;
      }
      int a = system(KEY_DOWN_CMD_STR);
    }
    //add animation
    if(key->animation_type != NO_ANIMATION){
      key->animation_action = create_animation(key->animation_x, key->animation_y, key->animation_color, key->animation_type);
      key->animation_action->ref_count++;
      int ret = add_animation(push, key->animation_action, key->animation_layer);
      if(ret == 0){
        free_animation(key->animation_action);
        key->animation_action = NULL;
      }
      //printf("ok\n");
    }

    key->is_pressed = true;
  } else if(check_activity(key->deactivate, e)){
    if(key->keyboard_command != NULL){
      for(unsigned int i = 0; i < strlen(key->keyboard_command); i++){
        KEY_UP_CMD_STR[i + 14] = key->keyboard_command[i];
      }
      KEY_UP_CMD_STR[14 + strlen(key->keyboard_command)] = '\n';
      for(unsigned int i = 14 + strlen(key->keyboard_command) + 1; i < KEY_CMD_LEN; i++){
        KEY_UP_CMD_STR[i] = 0;
      }
      int a = system(KEY_UP_CMD_STR);
    }
    //remove animation
    if(key->animation_type != NO_ANIMATION && key->animation_action != NULL){
      key->animation_action->ref_count--;
      key->animation_action->done = true;
      key->animation_action = NULL;
    }

    key->is_pressed = false;
  }
}

void handle_key_event(push_device* push, event* e, keyboard* keyboard){
  //check event against all keys. if match then stop and preform mey press update
  if(keyboard->keys == NULL) return;
  for(unsigned int i = 0; i < keyboard->num_of_keys; i++){
    key* key = keyboard->keys[i];
    if(key == NULL) continue;
    preform_key_press(push, e, key);
  }
}

void free_activator(activator* a){
  free(a);
}

void free_key(key* key){
  if(key == NULL) return;
  free_activator(key->activate);
  free_activator(key->deactivate);
  free(key);
}

void free_keyboard(keyboard* keyboard){
  if(keyboard == NULL) return;
  for(unsigned int i = 0; i < keyboard->num_of_keys; i++){
    free_key(keyboard->keys[i]);
  }
  free(keyboard->keys);
  free(keyboard);
}

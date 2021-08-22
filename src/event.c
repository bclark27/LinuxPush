#include"event_structs.h"
#include"push_structs.h"
#include<stdlib.h>
#include<stdio.h>

void print_event(event* e){
  printf("Event: 0x%08x\n", e->data);
  if(e->event_class == PAD_EVENT){
    printf("Class: Pad Event\nX: %i\nY: %i\nVel: %i\nState: %i\n", e->pad_x, e->pad_y, e->pad_velocity, e->pad_state);
  } else if(e->event_class == BUTTON_EVENT){
    printf("Class: Button Event\nID: %i\nState: %i\n", e->btn_id, e->btn_state);
  } else if(e->event_class == KNOB_EVENT){
    printf("Class: Knob Event\nID: %i\nDelta: %i\nState: %i\n", e->knob_id, e->knob_delta, e->knob_state);
  } else if(e->event_class == SLIDER_EVENT){
    printf("Class: Slider Event\nValue: %i\nPercent: %f\nState: %i\n", e->slider_value, e->slider_percent, e->slider_state);
  } else {
    printf("Class: Unknown\n");
  }
  printf("\n");
}

event* create_event(unsigned int data, push_device* push){
  event* e = (event*)malloc(sizeof(event));
  e->next_event = NULL;
  e->prev_event = NULL;
  e->data = data;

  unsigned char b1 = (e->data & 0xff000000) >> 24;
  unsigned char b2 = (e->data & 0x00ff0000) >> 16;
  unsigned char b3 = (e->data & 0x0000ff00) >> 8;
  unsigned char b4 = e->data & 0x000000ff;

  if(b2 >= 36 && b2 <= 99 && (b4 == PAD_PRESS_EVENT || b4 == PAD_HOLD_EVENT || b4 == PAD_RELEASE_EVENT)){ //event is pad
    e->event_class = PAD_EVENT;

    b2 -= 36;
    e->pad_x = (b2 % 8) + push->dx;
    e->pad_y = (b2 / 8) + push->dy;

    e->pad_state = b4;
    e->pad_velocity = b1;
    
  } else if((b1 == BUTTON_PRESS_EVENT || b1 == BUTTON_RELEASE_EVENT) && b4 == BUTTON_TAG && (b2 > 0x0f || b2 < 0x0e) && (b2 > 0x4f || b2 < 0x47)){//is a btn
    e->event_class = BUTTON_EVENT;
    e->btn_id = b2;
    e->btn_state = b1;
    //printf("B: 0x%08x\n", data);
  } else if((b4 == 0x1e && b3 == 0xe0) || (b1 == 0x00 && b2 == 0x0c && b3 == 0x90 && b4 == 0x19) || (b1 == 0x7f && b2 == 0x0c && b3 == 0x90 && b4 == 0x19)){//is a slider
    e->event_class = SLIDER_EVENT;

    if(b4 == SLIDER_HOLD_EVENT){
      //exit(1);
      e->slider_value = ((unsigned int)b1) << 8 | b2;
      e->slider_percent = (float)e->slider_value / 0x7f7f;
      e->slider_state = SLIDER_HOLD_EVENT;
    } else {
      e->slider_value = -1;
      e->slider_percent = -1;
      e->slider_state = b4;
    }
    //printf("S: 0x%08x\n", data);
  } else {//is a knob
    e->event_class = KNOB_EVENT;

    if(b4 == KNOB_TOUCH_TAG){
      e->knob_id = b2;
      e->knob_delta = 0;
      e->knob_state = b1;
    } else {
      e->knob_state = KNOB_TURN_EVENT;

      if(b1 < 0x50){
        e->knob_delta = b1;
      } else {
        e->knob_delta = -((int)0x7f - (int)b1 + 1);
      }

      if(b2 & 0xf0){
        e->knob_id = b2 - 0x47;
      } else {
        if(b2 == 0x0e){
          e->knob_id = 10;
        } else {
          e->knob_id = 9;
        }
      }
    }
  }

  return e;
}

void free_event(event* e){
  free(e);
}

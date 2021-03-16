#include<libusb-1.0/libusb.h>
#include"layer_structs.h"
#include"usb_structs.h"
#include"event_structs.h"
#include"keyboard_structs.h"
#include"knob_structs.h"
#include"text_structs.h"
#include"layer_manager_structs.h"

#ifndef PUSH_DEVICE
#define PUSH_DEVICE
typedef struct push_device{
  struct usb_device* device; //the output
  unsigned char* pad_color_new;
  unsigned char* pad_blink_new;
  unsigned char* pad_status_new;

  unsigned char* pad_color_curr;
  unsigned char* pad_blink_curr;
  unsigned char* pad_status_curr;

  int in_max_size;//MAKE SURE tHIS VALUE IS THE SAME AS UPDATE_SIGNAL SIZE IN CHARS
  int in_signal_size;//MAKE SURE tHIS VALUE IS THE SAME AS UPDATE_SIGNAL SIZE IN CHARS
  unsigned char* in_signal;

  int out_max_size;//MAKE SURE tHIS VALUE IS THE SAME AS UPDATE_SIGNAL SIZE IN CHARS
  int out_signal_size;//MAKE SURE tHIS VALUE IS THE SAME AS UPDATE_SIGNAL SIZE IN CHARS
  unsigned char* out_signal;

  int width;
  int height;
  int dx;
  int dy;

  event* event_head;
  event* event_tail;

  keyboard* keyboard_manager;

  knob** knobs;
  unsigned int num_knobs;

  unsigned char** screen_text;
  bool text_change;

  scrolling_text** scrolling_text_objects;
  int num_scrolling_text;

  layer_manager* lm;
}push_device;
#endif

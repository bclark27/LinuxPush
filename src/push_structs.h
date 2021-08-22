#include<libusb-1.0/libusb.h>
#include"usb_structs.h"
#include"event_structs.h"
#include"keyboard_structs.h"
#include"knob_structs.h"
#include"text_structs.h"
#include"pad_manager.h"
#include"pad_manager_structs.h"

#ifndef PUSH_DEVICE
#define PUSH_DEVICE
typedef struct push_device{
  struct usb_device* device; //the output
  knob** knobs;
  scrolling_text** scrolling_text_objects;
  struct pad_manager* pm;
  event* event_head;
  event* event_tail;
  keyboard* keyboard_manager;

  unsigned char** screen_text;
  unsigned char* pad_color_new;
  unsigned char* pad_blink_new;
  unsigned char* pad_status_new;
  unsigned char* pad_color_curr;
  unsigned char* pad_blink_curr;
  unsigned char* pad_status_curr;
  unsigned char* in_signal;
  unsigned char* out_signal;

  unsigned int num_knobs;
  unsigned int num_of_layers;
  int in_max_size;//MAKE SURE tHIS VALUE IS THE SAME AS UPDATE_SIGNAL SIZE IN CHARS
  int in_signal_size;//MAKE SURE tHIS VALUE IS THE SAME AS UPDATE_SIGNAL SIZE IN CHARS
  int out_max_size;//MAKE SURE tHIS VALUE IS THE SAME AS UPDATE_SIGNAL SIZE IN CHARS
  int out_signal_size;//MAKE SURE tHIS VALUE IS THE SAME AS UPDATE_SIGNAL SIZE IN CHARS
  int width;
  int height;
  int dx;
  int dy;
  int num_scrolling_text;

  bool text_change;
  bool turn_off;
}push_device;
#endif

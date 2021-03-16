#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<libusb-1.0/libusb.h>
#include"usb.h"
#include"push.h"
#include"colors.h"
#include"signals.h"
#include"keyboard.h"
#include"event.h"
#include"layer.h"

/////////////////////////////////////////////////
//MUST INSTALL xdotool FOR tHIS PROGRAM TO WORK//
/////////////////////////////////////////////////
/*
RUN WITH SUDO
*/

#define PUSH_INTERFACE 1
#define PUSH_PROD_ID 21
#define PUSH_VEND_ID 2536
#define msleep(x) usleep(x * 1000)

void hsb_demo(push_device* push){

  new_layer(push, 0);
  new_layer(push, 1);

  //light_pad_xy(push, 1, 5, BLINK_OFF, 4, 4);

  char *hue_lable = "Hue";
  char *sat_lable = "Sat";
  char *bri_lable = "Bright";

  char *hue_num = (char*)malloc(80 * sizeof(char));
  char *sat_num = (char*)malloc(80 * sizeof(char));
  char *bri_num = (char*)malloc(80 * sizeof(char));

  place_text_knob(push, hue_lable, 0, 0);
  place_text_knob(push, sat_lable, 1, 0);
  place_text_knob(push, bri_lable, 2, 0);

  add_knob(push, 0, 0, 0.5, 0, 1, 0.01, true);
  add_knob(push, 1, 1, 0.5, 0, 1, 0.01, false);
  add_knob(push, 2, 2, 0.5, 0, 1, 0.01, false);

  while(1) {
    receive_update(push);
    process_events(push);

    float h = read_knob(push, 0);
    float s = read_knob(push, 1);
    float b = read_knob(push, 2);

    sprintf(hue_num, "%.2f", h);
    sprintf(sat_num, "%.2f", s);
    sprintf(bri_num, "%.2f", b);

    place_text_knob(push, hue_num, 0, 1);
    place_text_knob(push, sat_num, 1, 1);
    place_text_knob(push, bri_num, 2, 1);
    place_text(push, "Hello Nichol", 0, 3);
    set_all_pads(push, 0, hsb(h, s, b), BLINK_OFF);

    update_push(push);
    //msleep(1);
  }
}

int main(){

  libusb_context *context = NULL;
  libusb_init(&context);
  usb_device* dev = create_usb_device(PUSH_INTERFACE, PUSH_PROD_ID, PUSH_VEND_ID, context);

  push_device* push = create_push_device(dev, 8, 8);

  //////////////
  //SETUP DONE//
  //////////////
  new_layer(push, 0);
  new_layer(push, 4);
  printf("Done Setup\n");
  //add_pad_key_push(push, 0, NULL, 0, 1, 1, 21, STAR_CROSS, true);
  add_animation_triger_to_push(push, PLUS_CROSS, 0, 8, 0, 8, 21, 4, 1);
  add_animation_triger_to_push(push, STAR_CROSS, 0, 8, 0, 8, 50, 4, 2);
  add_animation_triger_to_push(push, SQUARE, 0, 8, 0, 8, 50, 0, 3);

  char *hue_lable = "Hue";
  char *sat_lable = "Sat";
  char *bri_lable = "Bright";

  char *hue_num = (char*)malloc(80 * sizeof(char));
  char *sat_num = (char*)malloc(80 * sizeof(char));
  char *bri_num = (char*)malloc(80 * sizeof(char));

  place_text_knob(push, hue_lable, 0, 0);
  place_text_knob(push, sat_lable, 1, 0);
  place_text_knob(push, bri_lable, 2, 0);

  add_knob(push, 0, 0, 0.5, 0, 1, 0.01, true);
  add_knob(push, 1, 1, 0.5, 0, 1, 0.01, false);
  add_knob(push, 2, 2, 0.5, 0, 1, 0.01, false);

  while(1) {
    receive_update(push);
    process_events(push);

    float h = read_knob(push, 0);
    float s = read_knob(push, 1);
    float b = read_knob(push, 2);

    sprintf(hue_num, "%.2f", h);
    sprintf(sat_num, "%.2f", s);
    sprintf(bri_num, "%.2f", b);

    place_text_knob(push, hue_num, 0, 1);
    place_text_knob(push, sat_num, 1, 1);
    place_text_knob(push, bri_num, 2, 1);

    unsigned char color = hsb(h, s, b);
    get_animation_triger(push, 0, 3)->animation_color = color;
    get_animation_triger(push, 4, 1)->animation_color = (color + 5) % 256;
    get_animation_triger(push, 4, 2)->animation_color = (color + 5) % 256;

    update_push(push); // TODO: kill yourself
    msleep(1);
  }

  free_push(push);
  libusb_exit(context);
}

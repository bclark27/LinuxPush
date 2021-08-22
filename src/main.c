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
#include"linked_list_structs.h"
#include"linked_list.h"
#include"stdbool.h"
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

int main(){

  libusb_context *context = NULL;
  libusb_init(&context);
  usb_device* dev = create_usb_device(PUSH_INTERFACE, PUSH_PROD_ID, PUSH_VEND_ID, context);
  //QQQprint_device(dev->device);
  push_device* push = create_push_device(dev, 8, 8);

  //////////////
  //SETUP DONE//
  //////////////
  printf("Done Setup\n");
  for(int i = 0; i < 5; i++){
    create_new_layer(push, i);
  }

  add_pad_key_push(push, 0, "w", 1, 2);
  while(!push->turn_off) {
    //printf("TICK: %i\n", count++);
    receive_update(push);
    process_events(push);


    update_push(push);
    msleep(1);
  }

  free_push(push);
  libusb_exit(context);
  printf("Shuting Down\n");
}

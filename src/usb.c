#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include "usb.h"

#define WAIT_TIME 7

void print_device(libusb_device *dev){

  printf("\n");

  struct libusb_device_descriptor desc;
  struct libusb_config_descriptor *config;
  const struct libusb_interface *inter;
  const struct libusb_interface_descriptor *inter_desc;
  const struct libusb_endpoint_descriptor *endpoint_desc;

  int ret;
  printf("asd\n");
  ret = libusb_get_device_descriptor(dev, &desc);

  if(ret < 0){
    printf("Error could not get device descriptor\n");
    return;
  }

  printf("Num of posible configs: %d\n", desc.bNumConfigurations);
  printf("Device class: %d\n", desc.idVendor);
  printf("Product ID: %d\n", desc.idProduct);

  libusb_get_config_descriptor(dev, 0, &config);

  printf("Num of interfaces: %d\n", config->bNumInterfaces);

  for(int i = 0; i < config->bNumInterfaces; i++){
    printf("|\n");
    printf("+--Interface: %d\n", i);
    printf("|  |\n");
    inter = &config->interface[i];
    printf("|  +--Number of alt settings: %d\n", inter->num_altsetting);
    for(int j = 0; j < inter->num_altsetting; j++){
      printf("|     |\n");
      printf("|     +--Alt setting: %d\n", j);
      inter_desc = &inter->altsetting[j];
      printf("|     |  |\n");
      printf("|     |  +--Num of endpoints: %d\n", inter_desc->bNumEndpoints);
      for(int k = 0; k < inter_desc->bNumEndpoints; k++){
        endpoint_desc = &inter_desc->endpoint[k];
        printf("|     |  |  |\n");
        printf("|     |  |  +--Endpoint: %d\n", k);
        printf("|     |  |  |  |\n");
        printf("|     |  |  |  +--Desc type: %d\n", endpoint_desc->bDescriptorType);
        printf("|     |  |  |  +--Endpoint Address: %d\n", endpoint_desc->bEndpointAddress);

      }
    }
  }

  printf("\n");
  libusb_free_config_descriptor(config);
}

void print_list_of_devices(libusb_device **devs, int count){
  for(int i = 0; i < count; i++){
    print_device(devs[i]);
  }
}

int read_data(usb_device* dev, unsigned char* data, int size){
  if(size < 8){
    return 0;
  }

  int actual = 0; //used to find out how many bytes were written
  int r = libusb_bulk_transfer(dev->handle, (dev->endpoint_in_address | LIBUSB_ENDPOINT_IN), data, size, &actual, WAIT_TIME);
  //printf("Primer: %d\n", actual);

  if(actual == 0){
    return 0;
  }

  if(r == 0){
    //data read
  } else if(r == -7){
    //printf("Read request timed out\nAmount read: %d\n", actual);
    return 0;
  } else {
    printf("Read Error code: %d\nAmount read: %d\n", r, actual);
  }

  r = libusb_bulk_transfer(dev->handle, (dev->endpoint_in_address | LIBUSB_ENDPOINT_IN), data+4, size-4, &actual, WAIT_TIME);
  //printf("Second: %d\n", actual);
  if(r == 0){
    //data read
  } else if(r == -7){
    //time out, still return the first few bytes
  } else {
    printf("Read Error code: %d\nAmount read: %d\n", r, actual);
  }

  //printf("Return: %d\n", actual+4);
  return actual+4;
}

int send_data(usb_device* dev, unsigned char* data, int size){
  if(size == 0 || !dev){
    return 0;
  }
  int actual; //used to find out how many bytes were written
  int r = libusb_bulk_transfer(dev->handle, (dev->endpoint_out_address | LIBUSB_ENDPOINT_OUT), data, size, &actual, WAIT_TIME); //my device's out endpoint was 2, found with trial- the device had 2 endpoints: 2 and 129
//printf("%i\n", actual);
  if(r == 0 && actual == size){
    //data writen
  } else {
    printf("Write Error code: %d\nAmount sent: %d\n", r, actual);
  }
  return actual;
}

void free_usb(usb_device* dev){
  libusb_release_interface(dev->handle, dev->interface);
  libusb_close(dev->handle);
  free(dev);
}

usb_device* create_usb_device(int interface, int productID, int vendorID, libusb_context *context){
  usb_device* dev = (usb_device*)malloc(sizeof(usb_device));
  dev->interface = interface;
  dev->productID = productID;
  dev->vendorID = vendorID;
  //libusb_context *context = NULL;
  //libusb_init(&context);


  // discover devices
  libusb_device **list;

  ssize_t cnt = libusb_get_device_list(context, &list);
  if (cnt < 0){
      printf("NO LIST\n");
  }

  int found = 0;
  for (int i = 0; i < cnt; i++) {
    libusb_device *device = list[i];
    struct libusb_device_descriptor desc;

    libusb_get_device_descriptor(device, &desc);
    if(desc.idVendor == dev->vendorID && desc.idProduct == dev->productID){
      //print_device(device);
      dev->device = device;

      found = 1;
      break;
    }
  }

  if(!found){
    printf("not there bud\n");
    exit(1);
  }

  libusb_free_device_list(list, 1);

  dev->handle = libusb_open_device_with_vid_pid(context, dev->vendorID, dev->productID);
  if(!dev->handle){
    printf("could not get device handle (run with sudo?)\n");
    exit(1);
  }
  //detach kernel to claim interface
  libusb_detach_kernel_driver(dev->handle, dev->interface);
  //clain interface and read code
  int err = libusb_claim_interface(dev->handle, dev->interface);
  //printf("Claim interface code: %d\n", err);

  //get endpoint adresses
  struct libusb_config_descriptor *config;
  const struct libusb_interface *inter;
  const struct libusb_interface_descriptor *inter_desc;
  const struct libusb_endpoint_descriptor *endpoint_desc;

  libusb_get_config_descriptor(dev->device, 0, &config);
  inter = &config->interface[dev->interface];
  inter_desc = &inter->altsetting[0];
  endpoint_desc = &inter_desc->endpoint[0];
  dev->endpoint_out_address = endpoint_desc->bEndpointAddress;
  endpoint_desc = &inter_desc->endpoint[1];
  dev->endpoint_in_address = endpoint_desc->bEndpointAddress;
  //endpoint_desc->bEndpointAddress;

  return dev;
}

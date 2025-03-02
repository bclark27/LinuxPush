#ifndef USB_H_
#define USB_H_

#include "Common.h"

/////////////
//  TYPES  //
/////////////

typedef struct USB_handle
{
  libusb_device_handle* handle;
  libusb_device* device;
  
  int interface;
  int productID;
  int vendorID;
  int endpoint_in_address;
  int endpoint_out_address;

} USB_handle;

////////////////////////
//  PUPLIC FUNCTIONS  //
////////////////////////


USB_handle* USB_init(int interface, int productID, int vendorID);
void USB_free(USB_handle* handle);
void USB_destroy_context();

int USB_read_data(USB_handle* handle, void* data, int size);
int USB_send_data(USB_handle* handle, void* data, int size);

#endif

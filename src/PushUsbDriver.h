#ifndef PUSH_USB_DRIVER_H_
#define PUSH_USB_DRIVER_H_

#include<libusb-1.0/libusb.h>

///////////////
//   FUNCS   //
///////////////

int read_data(unsigned char* data, int size);
int send_data(unsigned char* data, int size);
void freeUsb();
void pushUsbDevice_init(int interface, int productID, int vendorID, libusb_context *context);

#endif

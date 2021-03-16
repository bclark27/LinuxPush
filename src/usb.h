#include<libusb-1.0/libusb.h>
#include"usb_structs.h"

#ifndef PRINT_DEVICE
#define PRINT_DEVICE
void print_device(libusb_device *dev);
#endif

#ifndef PRINT_DEVICES
#define PRINT_DEVICES
void print_list_of_devices(libusb_device **devs, int count);
#endif

#ifndef READ_DATA
#define READ_DATA
int read_data(usb_device* dev, unsigned char* data, int size);
#endif

#ifndef SEND_DATA
#define SEND_DATA
int send_data(usb_device* dev, unsigned char* data, int size);
#endif

#ifndef RELEASE_USB
#define RELEASE_USB
void free_usb(usb_device* dev);
#endif

#ifndef CREATE_USB_DEVICE
#define CREATE_USB_DEVICE
usb_device* create_usb_device(int interface, int productID, int vendorID, libusb_context *context);
#endif

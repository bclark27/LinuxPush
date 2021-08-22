#include <libusb-1.0/libusb.h>

#ifndef USB_DEVICE
#define USB_DEVICE
typedef struct usb_device {
  libusb_device_handle *handle;
  libusb_device *device;
  int interface;
  int productID;
  int vendorID;
  int endpoint_in_address;
  int endpoint_out_address;

}usb_device;
#endif

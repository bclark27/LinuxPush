#ifndef PUSH_USB_DRIVER_H_
#define PUSH_USB_DRIVER_H_

///////////////
//   FUNCS   //
///////////////

int PushUsbDriver_read_data(unsigned char* data, int size);
int PushUsbDriver_send_data(unsigned char* data, int size);
void PushUsbDriver_free();
void PushUsbDriver_init();

#endif

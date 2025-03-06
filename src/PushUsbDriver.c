#include "comm/USB.h"

#include "PushUsbDriver.h"


#define PUSH_INTERFACE 1
#define PUSH_PROD_ID 21
#define PUSH_VEND_ID 2536

/////////////
//  TYPES  //
/////////////

unsigned char USBURB_Format[128] = {
  0x80, 0xe4, 0x41, 0x24, 0xb6, 0x91, 0xff, 0xff, 0x53, 0x03, 0x01, 0x0d, 0x01, 0x00, 0x2d, 0x00, // [0-7] URB id, [8] URB type, [9] trans type, [10] direction, [11] dedvice, [12-13] bus id, [14] device setup req, [15] data present
  0x0e, 0xbf, 0xc7, 0x67, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x5c, 0x0e, 0x00, 0x8d, 0xff, 0xff, 0xff, // [0-7] URB sec, [8-11] URB usec, [12-15] URB status
  0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [0-3] URB len, [4-7] data len, [8-15] unused
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [0-3] interval, [4-7] start frame, [8-11] copy trans flags, [12-15] iso descriptors
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

typedef struct usbDriverState
{
  USB_handle* handle;
} usbDriverState;

////////////////////
//  PRIVATE VARS  //
////////////////////

static usbDriverState instance;
static usbDriverState * self = &instance;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////


///////////////////////
// PUBLIC FUNCTIONS  //
///////////////////////

void PushUsbDriver_init()
{
  
  memset(self, 0, sizeof(usbDriverState));

  self->handle = USB_init(PUSH_INTERFACE, PUSH_PROD_ID, PUSH_VEND_ID);


  /*

  memset(self, 0, sizeof(usbDevice));
  self->interface = interface;
  self->productID = productID;
  self->vendorID = vendorID;
  //libusb_context *context = NULL;
  //libusb_init(&context);
  // discover devices
  libusb_device **list;

  ssize_t cnt = libusb_get_device_list(context, &list);
  if (cnt < 0)
  {
      printf("NO LIST\n");
  }

  int found = 0;
  for (int i = 0; i < cnt; i++)
  {
    libusb_device *device = list[i];
    struct libusb_device_descriptor desc;

    libusb_get_device_descriptor(device, &desc);
    if(desc.idVendor == self->vendorID && desc.idProduct == self->productID){
      //print_device(device);
      self->device = device;

      found = 1;
      break;
    }
  }

  if(!found)
  {
    printf("not there bud\n");
    exit(1);
  }

  libusb_free_device_list(list, 1);

  self->handle = libusb_open_device_with_vid_pid(context, self->vendorID, self->productID);
  if(!self->handle)
  {
    printf("could not get device handle (run with sudo?)\n");
    exit(1);
  }
  //detach kernel to claim interface
  libusb_detach_kernel_driver(self->handle, self->interface);
  //clain interface and read code
  libusb_claim_interface(self->handle, self->interface);
  //printf("Claim interface code: %d\n", err);

  //get endpoint adresses
  struct libusb_config_descriptor *config;
  const struct libusb_interface *inter;
  const struct libusb_interface_descriptor *inter_desc;
  const struct libusb_endpoint_descriptor *endpoint_desc;

  libusb_get_config_descriptor(self->device, 0, &config);
  inter = &config->interface[self->interface];
  inter_desc = &inter->altsetting[0];
  endpoint_desc = &inter_desc->endpoint[0];
  self->endpoint_out_address = endpoint_desc->bEndpointAddress;
  endpoint_desc = &inter_desc->endpoint[1];
  self->endpoint_in_address = endpoint_desc->bEndpointAddress;
  //endpoint_desc->bEndpointAddress;
  //
  */
}

int PushUsbDriver_read_data(unsigned char* data, int size)
{
  return USB_read_data(self->handle, data, size);
  /*

  if(size < 8)
  {
    return 0;
  }

  int actual = 0; //used to find out how many bytes were written
  int r = libusb_bulk_transfer(self->handle, (self->endpoint_in_address | LIBUSB_ENDPOINT_IN), data, size, &actual, WAIT_TIME);
  //printf("Primer: %d\n", actual);

  if(actual == 0){
    return 0;
  }

  if(r == 0)
  {
    //data read
  }
  else if(r == -7)
  {
    //printf("Read request timed out\nAmount read: %d\n", actual);
    return 0;
  }
  else
  {
    printf("Read Error code: %d\nAmount read: %d\n", r, actual);
  }

  r = libusb_bulk_transfer(self->handle, (self->endpoint_in_address | LIBUSB_ENDPOINT_IN), data+4, size-4, &actual, WAIT_TIME);
  //printf("Second: %d\n", actual);
  if(r == 0)
  {
    //data read
  }
  else if(r == -7)
  {
    //time out, still return the first few bytes
  }
  else
  {
    printf("Read Error code: %d\nAmount read: %d\n", r, actual);
  }

  //printf("Return: %d\n", actual+4);
  return actual + 4;
  */
}

int fillInUsbUrbPacket(unsigned char* buffer, void* data, int size, int* amount)
{
  memcpy(buffer, USBURB_Format, 128);
  unsigned char s = (unsigned char)MIN(size, 64);
  memcpy(buffer + 64, data, s);

  buffer[32] = s;
  buffer[36] = s;

  *amount = s;

  return s + 64;
}

int PushUsbDriver_send_data(unsigned char* data, int size)
{
  unsigned char buffer[128];
  int processed = 0;
  int amount = 0;
  do
  {
    int s = fillInUsbUrbPacket(buffer, data + processed, size - processed, &amount);
    processed += amount;
    USB_send_data(self->handle, buffer, s);
  } while ((size - processed) > 0);
  




  return USB_send_data(self->handle, data, size);
  /*

  if(size == 0 || !self)
  {
    return 0;
  }
  int actual; //used to find out how many bytes were written
  int r = libusb_bulk_transfer(self->handle, (self->endpoint_out_address | LIBUSB_ENDPOINT_OUT), data, size, &actual, WAIT_TIME); //my device's out endpoint was 2, found with trial- the device had 2 endpoints: 2 and 129
//printf("%i\n", actual);
  if(r == 0 && actual == size){
    //data writen
  } else {
    printf("Write Error code: %d\nAmount sent: %d\n", r, actual);
  }
  return actual;

  */
}

void PushUsbDriver_free()
{
  USB_free(self->handle);
  self->handle = NULL;
  USB_destroy_context();
  /*

  if(self->handle)
  {
    if(self->interface)
    {
      libusb_release_interface(self->handle, self->interface);
    }
    libusb_close(self->handle);
  }

  */
}

//////////////////////////
//     PRIVATE FUNCS    //
//////////////////////////

/*

static void print_device(libusb_device *dev)
{
  printf("\n");

  struct libusb_device_descriptor desc;
  struct libusb_config_descriptor *config;
  const struct libusb_interface *inter;
  const struct libusb_interface_descriptor *inter_desc;
  const struct libusb_endpoint_descriptor *endpoint_desc;

  int ret;
  printf("asd\n");
  ret = libusb_get_device_descriptor(dev, &desc);

  if(ret < 0)
  {
    printf("Error could not get device descriptor\n");
    return;
  }

  printf("Num of posible configs: %d\n", desc.bNumConfigurations);
  printf("Device class: %d\n", desc.idVendor);
  printf("Product ID: %d\n", desc.idProduct);

  libusb_get_config_descriptor(dev, 0, &config);

  printf("Num of interfaces: %d\n", config->bNumInterfaces);

  for(int i = 0; i < config->bNumInterfaces; i++)
  {
    printf("|\n");
    printf("+--Interface: %d\n", i);
    printf("|  |\n");
    inter = &config->interface[i];
    printf("|  +--Number of alt settings: %d\n", inter->num_altsetting);
    for(int j = 0; j < inter->num_altsetting; j++)
    {
      printf("|     |\n");
      printf("|     +--Alt setting: %d\n", j);
      inter_desc = &inter->altsetting[j];
      printf("|     |  |\n");
      printf("|     |  +--Num of endpoints: %d\n", inter_desc->bNumEndpoints);
      for(int k = 0; k < inter_desc->bNumEndpoints; k++)
      {
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

*/

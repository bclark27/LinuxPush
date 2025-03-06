#include "OutputMessageBuilder.h"
#include "PushEventManager.h"
#include "PushManager.h"
#include "PushUsbDriver.h"

#define msleep(x) usleep(x * 1000)

////////////////////
//  PRIVATE VARS  //
////////////////////

void SetBtnOn(unsigned char id)
{
return; 
unsigned char update[68] = {
  0x80, 0xe4, 0x41, 0x24, 0xb6, 0x91, 0xff, 0xff, 0x53, 0x03, 0x01, 0x0d, 0x01, 0x00, 0x2d, 0x00, // [0-7] URB id, [8] URB type, [9] trans type, [10] direction, [11] dedvice, [12-13] bus id, [14] device setup req, [15] data present
  0x0e, 0xbf, 0xc7, 0x67, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x5c, 0x0e, 0x00, 0x8d, 0xff, 0xff, 0xff, // [0-7] URB sec, [8-11] URB usec, [12-15] URB status
  0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [0-3] URB len, [4-7] data len, [8-15] unused
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [0-3] interval, [4-7] start frame, [8-11] copy trans flags, [12-15] iso descriptors
  0x1b, 0xb0, id, 0x01, 
  };

  
  int fullSize = 68;//sizeof(unsigned int) * ic * kc * 4;
  int sent = 0;
  while (sent < fullSize)
  {
    sent += PushUsbDriver_send_data(((unsigned char*)update) + sent, fullSize - sent);
  }

}


static char stop = 0;
void CloseBtnListener(void * sub, void * args)
{
  AbletonPkt_button * pkt = args;
  if (pkt->btnId == 3)
  {
    stop = 1;
  }

  SetBtnOn(pkt->btnId);
}

ColorStates defaultPadColor(int x, int y)
{
  return ColorStates_WHITE;


  int val = x + y * 8;
  val -= (y / 2) * 3;
  
  if (y % 2 != 0)
  {
    val -= 5;
  }

  if (val % 7 == 0)
    return 24;
  
  return ColorStates_WHITE;
}

void initColors()
{
  for (int x = 0; x < 8; x++)
    for (int y = 0; y < 8; y++)
      outputMessageBuilder_setPadColor(x, y, defaultPadColor(x, y));
}


int main(int argc, char *argv[])
{
  if (argc <= 1 || argc >= 4) {
    fprintf(stderr, "Usage: %s <hosting service name> [controlling service name]\n", argv[0]);
    return 1;
  }

  // Access the arguments
  char *serviceName = argv[1];

  PushManager_Init();
  int s = PushManager_InitServer(serviceName);
  printf("server status: %d\n", s);

  if (argc == 3)
  {
    char *controllingName = argv[2];
    printf("Connecting to: %s\n", controllingName);
    
    s = -1;
    while (s != CC_SUCCESS)
    {
      s = PushManager_ReceiveCommandsFromService(controllingName);
      printf("connection status: %d\n", s);

      if (s != CC_SUCCESS)
        sleep(1);
    }
  }

  printf("cycling push\n");

  char btnHandler;
  pushEventManager_subscribeToNewButtonPackets(&btnHandler, CloseBtnListener);

  initColors();

/*
    padUpdatePacket |= ((unsigned char)0x0b) << 24;//color in byte 3
    padUpdatePacket |= ((unsigned char)0xb0) << 16;//pad id in byte 2
    padUpdatePacket |= ((unsigned char)i) << 8;
    padUpdatePacket |= ((unsigned char)0x69);//status in byte 0 always value 0x69

    */


  //PushManager_InitServerOnPort(9988);


/*
  unsigned char clear[512];
  memset(clear, 0, 512);
  PushUsbDriver_send_data(clear, 512);



  int kc = 1;
  int ic = 0x7f;
  unsigned int update[kc * ic];
  for (unsigned int k = 0; k < kc; k++)
  for (unsigned int i = 0x7f; i < ic; i++)
  {
    unsigned int padUpdatePacket = 0;
    padUpdatePacket |= ((unsigned char)0x1b) << 24;//color in byte 3
    padUpdatePacket |= ((unsigned char)0xb0) << 16;//pad id in byte 2
    padUpdatePacket |= ((unsigned char)i) << 8;
    padUpdatePacket |= ((unsigned char)0x4);//status in byte 0 always value 0x69
    update[i] = padUpdatePacket;
  }
  */

/*
  unsigned char update[128] = {
  0x80, 0xe4, 0x41, 0x24, 0xb6, 0x91, 0xff, 0xff, 0x53, 0x03, 0x01, 0x0d, 0x01, 0x00, 0x2d, 0x00, // [0-7] URB id, [8] URB type, [9] trans type, [10] direction, [11] dedvice, [12-13] bus id, [14] device setup req, [15] data present
  0x0e, 0xbf, 0xc7, 0x67, 0x00, 0x00, 0x00, 0x00, 0x8e, 0x5c, 0x0e, 0x00, 0x8d, 0xff, 0xff, 0xff, // [0-7] URB sec, [8-11] URB usec, [12-15] URB status
  0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [0-3] URB len, [4-7] data len, [8-15] unused
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // [0-3] interval, [4-7] start frame, [8-11] copy trans flags, [12-15] iso descriptors
  0x1b, 0xb0, 0x24, 0x00, 0x1b, 0xb0, 0x3d, 0x00, 0x1b, 0xb0, 0x58, 0x04, 0x1b, 0xb0, 0x3c, 0x00,
  0x1b, 0xb0, 0x31, 0x04, 0x1b, 0xb0, 0x1d, 0x00, 0x1b, 0xb0, 0x76, 0x04, 0x1b, 0xb0, 0x74, 0x04,
  0x1b, 0xb0, 0x3b, 0x04, 0x1b, 0xb0, 0x39, 0x01, 0x1b, 0xb0, 0x77, 0x04, 0x1b, 0xb0, 0x75, 0x00,
  0x1b, 0xb0, 0x5a, 0x01, 0x1b, 0xb0, 0x2d, 0x04, 0x1b, 0xb0, 0x2c, 0x00, 0x1b, 0xb0, 0x2f, 0x04,
  };

  
  int fullSize = 20;//sizeof(unsigned int) * ic * kc * 4;
  int sent = 0;
  while (sent < fullSize)
  {
    sent += PushUsbDriver_send_data(((unsigned char*)update) + sent + 64, fullSize - sent);
    printf("sent=%d\n", sent);
  }

*/

  //PushManager_Cycle();
  while (!stop)
  {
    PushManager_Cycle();
  }
  
  pushEventManager_unsubscribeToNewButtonPackets(&btnHandler);

  PushManager_Free();
}

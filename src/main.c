#include "OutputMessageBuilder.h"
#include "PushEventManager.h"
#include "PushManager.h"

#define msleep(x) usleep(x * 1000)

////////////////////
//  PRIVATE VARS  //
////////////////////

static char stop = 0;


void CloseBtnListener(void * sub, void * args)
{
  AbletonPkt_button * pkt = args;
  if (pkt->btnId == 3)
  {
    stop = 1;
  }
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

  //PushManager_InitServerOnPort(9988);

  while (!stop)
  {
    PushManager_Cycle();
  }
  
  pushEventManager_unsubscribeToNewButtonPackets(&btnHandler);

  PushManager_Free();
}

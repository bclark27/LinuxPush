#include "OutputMessageBuilder.h"
#include "PushEventManager.h"
#include "LightAndTextStates.h"
#include "PushManager.h"

#define msleep(x) usleep(x * 1000)

////////////////////
//  PRIVATE VARS  //
////////////////////

/*
TCPServer * server;

void padHandlerFunc(void * sub, void * args)
{
  padPacket * pkt = args;

  if (pkt->isHold) return;

  SocketUtils_TCPServerSend(server, pkt, sizeof(AbletonPkt_pad), TCP_PacketType_PAD);

  if (pkt->isPress)
  {
    outputMessageBuilder_setPadColor(pkt->padX, pkt->padY, ColorStates_DARK_RED);
  }
  else
  {
    outputMessageBuilder_setPadColor(pkt->padX, pkt->padY, defaultColor(pkt->id));
  }
}

void knobHandlerFunc(void * sub, void * args)
{
  knobPacket * pkt = args;
  SocketUtils_TCPServerSend(server, pkt, sizeof(AbletonPkt_knob), TCP_PacketType_KNOB);
}

void USB_Controler()
{
  const unsigned short port = 9988;
  printf("Waiting for client\n");
  server = SocketUtils_initTCPServer(port);

  SocketUtils_freeTCPServer(server);
}
*/

static char stop = 0;


void onBtnEvent(void * sub, void * args)
{
  AbletonPkt_button * pkt = args;
  if (pkt->btnId == 3)
  {
    stop = 1;
  }
}

ColorStates defaultPadColor(int x, int y)
{
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

void onPadEvent(void * sub, void * args)
{
  AbletonPkt_pad * pkt = args;
  
  if (pkt->isPress)
  {
    outputMessageBuilder_setPadColor(pkt->padX, pkt->padY, ColorStates_DARK_GREEN);
  }

  if (pkt->isRelease)
  {
    outputMessageBuilder_setPadColor(pkt->padX, pkt->padY, defaultPadColor(pkt->padX, pkt->padY));
  }
}

int main()
{
  PushManager_Init();

  char padHandler;
  char btnHandler;
  pushEventManager_subscribeToNewPadPackets(&padHandler, onPadEvent);
  pushEventManager_subscribeToNewButtonPackets(&btnHandler, onBtnEvent);

  initColors();

  //PushManager_InitServerOnPort(9988);

  while (!stop)
  {
    PushManager_Cycle();
  }
  
  pushEventManager_unsubscribeToNewPadPackets(&padHandler);
  pushEventManager_unsubscribeToNewButtonPackets(&btnHandler);

  PushManager_Free();
}

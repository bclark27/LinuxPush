#include "PushManager.h"

#include "comm/IPC.h"
#include "PushUsbDriver.h"

/////////////
// DEFINES //
/////////////

///////////
// TYPES //
///////////

typedef struct PushManager
{
  bool serverUp;
} PushManager;

////////////////////
//  PRIVATE VARS  //
////////////////////

static PushManager instance;
static PushManager * self = &instance;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void padHandlerFunc(void * sub, void * args);
void buttonHandlerFunc(void * sub, void * args);
void knobHandlerFunc(void * sub, void * args);
void sliderHandlerFunc(void * sub, void * args);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void PushManager_Init()
{
    self->serverUp = false;
    // init the events and also the output state

    PushUsbDriver_init();
    
    outputMessageBuilder_init();
    outputMessageBuilder_clearState();

    pushEventManager_init();
    
    pushEventManager_subscribeToNewPadPackets(self, padHandlerFunc);
    pushEventManager_subscribeToNewButtonPackets(self, buttonHandlerFunc);
    pushEventManager_subscribeToNewKnobPackets(self, knobHandlerFunc);
    pushEventManager_subscribeToNewSliderPackets(self, sliderHandlerFunc);
}

void PushManager_Free()
{
    PushManager_FreeServer();

    pushEventManager_unsubscribeToNewPadPackets(self);
    pushEventManager_unsubscribeToNewButtonPackets(self);
    pushEventManager_unsubscribeToNewKnobPackets(self);
    pushEventManager_unsubscribeToNewSliderPackets(self);

    pushEventManager_free();
    outputMessageBuilder_free();
    PushUsbDriver_free();
}

int PushManager_InitServer(const char* serverName)
{
  if (self->serverUp)
    return 0;

  int ret = IPC_StartService(serverName);
  self->serverUp = ret == 0;
  return ret;
  /*

    if (self->server)
        return;

    self->server = SocketUtils_initTCPServer(port);
  */
}

void PushManager_FreeServer()
{
  self->serverUp = false;
  IPC_CloseService();
  /*
    if (!self->server)
        return;
    
    SocketUtils_freeTCPServer(self->server);
    self->server = NULL;
  */
}

void PushManager_Cycle()
{
    pushEventManager_readNewUsbData();
    pushEventManager_useNewPackets();
    outputMessageBuilder_updatePush();
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

void padHandlerFunc(void * sub, void * args)
{
  if (!self->serverUp)
    return;

  IPC_PostMessage(MSG_TYPE_ABL_PAD, args, sizeof(AbletonPkt_pad));
  /*

    if (self->server)
        SocketUtils_TCPServerSend(self->server, args, sizeof(padPacket), TCP_PacketType_PAD);
  */
}

void buttonHandlerFunc(void * sub, void * args)
{
  if (!self->serverUp)
    return;

  IPC_PostMessage(MSG_TYPE_ABL_BUTTON, args, sizeof(AbletonPkt_button));

  /*

    if (self->server)
        SocketUtils_TCPServerSend(self->server, args, sizeof(buttonPacket), TCP_PacketType_BUTTON);
  */
}

void knobHandlerFunc(void * sub, void * args)
{
  if (!self->serverUp)
    return;

  IPC_PostMessage(MSG_TYPE_ABL_KNOB, args, sizeof(AbletonPkt_knob));

  /*

    if (self->server)
        SocketUtils_TCPServerSend(self->server, args, sizeof(knobPacket), TCP_PacketType_KNOB);
  */
}

void sliderHandlerFunc(void * sub, void * args)
{
  if (!self->serverUp)
    return;

  IPC_PostMessage(MSG_TYPE_ABL_SLIDER, args, sizeof(AbletonPkt_slider));

  /*

    if (self->server)
        SocketUtils_TCPServerSend(self->server, args, sizeof(sliderPacket), TCP_PacketType_SLIDER);
  */
}

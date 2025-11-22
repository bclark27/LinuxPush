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
  bool listeningToOtherService;
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


void onReceivedCommand(MessageType t, void* d, MessageSize s);
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
    self->listeningToOtherService = false;
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
  /*ReceivedCommand

    if (self->server)
        return;

    self->server = SocketUtils_initTCPServer(port);
  */
}

int PushManager_ReceiveCommandsFromService(const char* name)
{
  if (self->listeningToOtherService)
    return -1;
  return IPC_ConnectToService(name, onReceivedCommand);
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

void onReceivedCommand(MessageType t, void* d, MessageSize s)
{
  if (t < MSG_TYPE_ABL_CMD_PAD || t > MSG_TYPE_ABL_CMD_SYS)
    return;

  unsigned char color;
  bool useColor;
  switch (t)
  {
    case MSG_TYPE_ABL_CMD_PAD: ;
      AbletonPkt_Cmd_Pad* padCmd = d;

      color = padCmd->color;
      if (padCmd->useRgb && padCmd->setColor)
        color = outputMessageBuilder_rgbToColor(padCmd->rgb);

      useColor = padCmd->setColor | padCmd->useRgb;
      if (padCmd->setBlink && useColor)
      {
        outputMessageBuilder_setPadState(padCmd->x, padCmd->y, color, padCmd->blink);
      }
      else if (padCmd->setBlink)
      {
        outputMessageBuilder_setPadBlink(padCmd->x, padCmd->y, padCmd->blink);
      }
      else if (useColor)
      {
        outputMessageBuilder_setPadColor(padCmd->x, padCmd->y, color);
      }

      break;

    case MSG_TYPE_ABL_CMD_BUTTON: ;
      AbletonPkt_Cmd_Button* btnCmd = d;

      printf("ID: %d, B: %d\n", btnCmd->id, btnCmd->blink);

      outputMessageBuilder_setButtonBlink(btnCmd->id, btnCmd->blink);
      break;

    case MSG_TYPE_ABL_CMD_PAD_BUTTON: ;
      AbletonPkt_Cmd_PadButton* pbCmd = d;

      color = pbCmd->color;
      if (pbCmd->useRgb && pbCmd->setColor)
        color = outputMessageBuilder_rgbToColor(pbCmd->rgb);

      useColor = pbCmd->setColor | pbCmd->useRgb;
      if (pbCmd->setBlink && useColor)
      {
        outputMessageBuilder_setButtonPadState(pbCmd->id, color, pbCmd->blink);
      }
      else if (pbCmd->setBlink)
      {
        outputMessageBuilder_setButtonPadBlink(pbCmd->id, pbCmd->blink);
      }
      else if (useColor)
      {
        outputMessageBuilder_setButtonPadColor(pbCmd->id, color);
      }

      break;

    case MSG_TYPE_ABL_CMD_TEXT: ;
      AbletonPkt_Cmd_Text* textCmd = d;
      outputMessageBuilder_setText(textCmd->x, textCmd->y, textCmd->text, textCmd->length);
      break;

    case MSG_TYPE_ABL_CMD_SYS: ;
      AbletonPkt_Cmd_Sys* sysCmd = d;

      if (sysCmd->clear)
        outputMessageBuilder_clearState();

      if (sysCmd->update)
        outputMessageBuilder_updatePush();

      break;

    default:
      break;
  }
}

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

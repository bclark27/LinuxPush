#include "PushManager.h"
#include "sock/SocketUtils.h"

#include "PushUsbDriver.h"

/////////////
// DEFINES //
/////////////

#define PUSH_INTERFACE 1
#define PUSH_PROD_ID 21
#define PUSH_VEND_ID 2536

///////////
// TYPES //
///////////

typedef struct PushManager
{
    TCPServer * server;
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
    // init the events and also the output state

    libusb_context *context = NULL;
    libusb_init(&context);
    pushUsbDevice_init(PUSH_INTERFACE, PUSH_PROD_ID, PUSH_VEND_ID, context);
    
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
    freeUsb();
}

void PushManager_InitServerOnPort(unsigned short port)
{
    if (self->server)
        return;

    self->server = SocketUtils_initTCPServer(port);
}

void PushManager_FreeServer()
{
    if (!self->server)
        return;
    
    SocketUtils_freeTCPServer(self->server);
    self->server = NULL;
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
    if (self->server)
        SocketUtils_TCPServerSend(self->server, args, sizeof(padPacket), TCP_PacketType_PAD);
}

void buttonHandlerFunc(void * sub, void * args)
{
    if (self->server)
        SocketUtils_TCPServerSend(self->server, args, sizeof(buttonPacket), TCP_PacketType_BUTTON);
}

void knobHandlerFunc(void * sub, void * args)
{
    if (self->server)
        SocketUtils_TCPServerSend(self->server, args, sizeof(knobPacket), TCP_PacketType_KNOB);
}

void sliderHandlerFunc(void * sub, void * args)
{
    if (self->server)
        SocketUtils_TCPServerSend(self->server, args, sizeof(sliderPacket), TCP_PacketType_SLIDER);
}
#ifndef PUSH_EVENT_CREATOR_H_
#define PUSH_EVENT_CREATOR_H_

#include "EventSubscription.h"

///////////
// TYPES //
///////////

typedef enum PKT_CODES {
  PAD_PRESS = 0x19,
  PAD_HOLD = 0x1a,
  PAD_RELEASE = 0x18,

  BUTTON_PRESS = 0x7f,
  BUTTON_RELEASE = 0x00,
  BUTTON_TAG = 0x1b,

  KNOB_PRESS = 0x7f,
  KNOB_TURN = 0x1b,
  KNOB_RELEASE = 0x00,
  KNOB_TOUCH_TAG = 0x19,

  SLIDEING_TAG = 0x1e,
  SLIDER_PRESSED = 0x7f,
  SLIDER_RELEASE = 0x00,
  SLIDER_HOLD = 0x1e,
} PKT_CODES;

typedef enum PktType {
  PUSH_PKT_TYPE,
  PAD_PKT_TYPE,
  BUTTON_PKT_TYPE,
  KNOB_PKT_TYPE,
  SLIDER_PKT_TYPE,
} PktType;

typedef struct pushEventPacket {

  unsigned char pktType;
  float slider_percent;
  unsigned int slider_value;
  unsigned int data;
  int knob_delta;
  int pad_x;
  int pad_y;

  unsigned char pad_velocity;
  unsigned char pad_state;
  unsigned char knob_state;
  unsigned char knob_id;
  unsigned char btn_id;
  unsigned char btn_state;
  unsigned char slider_state;
  unsigned char event_class;
} pushEventPacket;

typedef struct padPacket
{
  unsigned char pktType;
  unsigned int data;
  int id;
  int padX;
  int padY;
  unsigned char isPress;
  unsigned char isHold;
  unsigned char isRelease;
  unsigned char padVelocity;
} padPacket;

typedef struct knobPacket
{
  unsigned char pktType;
  unsigned int data;
  unsigned char id;
  char direction;
  unsigned char isPress;
  unsigned char isTurning;
  unsigned char isRelease;
} knobPacket;

typedef struct buttonPacket
{
  unsigned char pktType;
  unsigned int data;
  unsigned char btnId;
  unsigned char isPress;
  unsigned char isRelease;

} buttonPacket;

typedef struct sliderPacket
{
  unsigned char pktType;
  unsigned int data;
  int value;
  float percent;
  unsigned char isSliding;
  unsigned char isPress;
  unsigned char isRelease;
} sliderPacket;

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

char pushEventManager_init();
void pushEventManager_free();
void pushEventManager_readNewUsbData();
void pushEventManager_useNewPackets();

char pushEventManager_subscribeToNewPushPackets(void * subscriber, EventHandle handle);
void pushEventManager_unsubscribeToNewPushPackets(void * subscriber);

char pushEventManager_subscribeToNewPadPackets(void * subscriber, EventHandle handle);
void pushEventManager_unsubscribeToNewPadPackets(void * subscriber);

char pushEventManager_subscribeToNewSliderPackets(void * subscriber, EventHandle handle);
void pushEventManager_unsubscribeToNewSliderPackets(void * subscriber);

char pushEventManager_subscribeToNewButtonPackets(void * subscriber, EventHandle handle);
void pushEventManager_unsubscribeToNewButtonPackets(void * subscriber);

char pushEventManager_subscribeToNewKnobPackets(void * subscriber, EventHandle handle);
void pushEventManager_unsubscribeToNewKnobPackets(void * subscriber);

PktType pushEventManager_packetType(void * pkt);

void pushEventManager_printPushPacket(void * pkt);

#endif

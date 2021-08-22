#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EventSubscription.h"
#include "PushUsbDriver.h"

#include "PushEventManager.h"

/////////////
//  TYPES  //
/////////////

#define DATA_IN_SIZE 4096
#define DATA_OUT_SIZE 4096

typedef struct EventManager
{
  unsigned char dataInBuffer[DATA_IN_SIZE];
  unsigned char dataOutBuffer[DATA_OUT_SIZE];
  int readAmount;

  SubscriptionChain * newEventPacketSubscriptionChain;
  List * pushEventPacketChain;

  SubscriptionChain * newPadSubscriptionChain;
  List * pushPadPacketChain;

  SubscriptionChain * newSliderSubscriptionChain;
  List * pushSliderPacketChain;

  SubscriptionChain * newKnobSubscriptionChain;
  List * pushKnobPacketChain;

  SubscriptionChain * newButtonSubscriptionChain;
  List * pushButtonPacketChain;

} EventManager;


enum PUSH_PKT_EVENT_CLASS {
  PAD_EVENT,
  BUTTON_EVENT,
  KNOB_EVENT,
  SLIDER_EVENT,
};

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

static pushEventPacket * createPushEventPacket(unsigned int data);
static padPacket * pushPktToPadPkt(pushEventPacket * pkt);
static knobPacket * pushPktToKnobPkt(pushEventPacket * pkt);
static sliderPacket * pushPktToSliderPkt(pushEventPacket * pkt);
static buttonPacket * pushPktToButtonPkt(pushEventPacket * pkt);
static void digestPacket(pushEventPacket * pkt);
static void parseInBufferData();
static pushEventPacket * popEventPacket();
static padPacket * popPadPacket();
static knobPacket * popKnobPacket();
static buttonPacket * popButtonPacket();
static sliderPacket * popSliderPacket();
static void freeEventPacket(void * pkt);
static void freePushEventPacketChain(List * chain);
static void sendPushPackets();
static void sendPadPackets();
static void sendKnobPackets();
static void sendSliderPackets();
static void sendButtonPackets();

////////////////////
//  PRIVATE VARS  //
////////////////////

static EventManager instance;
static EventManager * self = &instance;

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

char pushEventManagerInit()
{
  memset(self, 0, sizeof(EventManager));

  self->newEventPacketSubscriptionChain = subscriptionChainInit();
  self->pushEventPacketChain = listInit();

  self->newSliderSubscriptionChain = subscriptionChainInit();
  self->pushSliderPacketChain = listInit();

  self->newButtonSubscriptionChain = subscriptionChainInit();
  self->pushButtonPacketChain = listInit();

  self->newPadSubscriptionChain = subscriptionChainInit();
  self->pushPadPacketChain = listInit();

  self->newKnobSubscriptionChain = subscriptionChainInit();
  self->pushKnobPacketChain = listInit();

  if(self->newEventPacketSubscriptionChain && self->pushEventPacketChain &&
    self->newPadSubscriptionChain && self->pushPadPacketChain &&
    self->newSliderSubscriptionChain && self->pushSliderPacketChain &&
    self->newKnobSubscriptionChain && self->pushKnobPacketChain &&
    self->newButtonSubscriptionChain && self->pushButtonPacketChain)
  {
    return 1;
  }
  return 0;
}

void freePushEventManager()
{
  freeSubscriptionChain(self->newEventPacketSubscriptionChain);
  freeSubscriptionChain(self->newPadSubscriptionChain);
  freeSubscriptionChain(self->newButtonSubscriptionChain);
  freeSubscriptionChain(self->newSliderSubscriptionChain);
  freeSubscriptionChain(self->newKnobSubscriptionChain);

  freePushEventPacketChain(self->pushEventPacketChain);
  freePushEventPacketChain(self->pushPadPacketChain);
  freePushEventPacketChain(self->pushSliderPacketChain);
  freePushEventPacketChain(self->pushButtonPacketChain);
  freePushEventPacketChain(self->pushKnobPacketChain);
}

void readNewUsbData()
{
  self->readAmount = read_data(self->dataInBuffer, DATA_IN_SIZE);
  if(self->readAmount)
  {
    parseInBufferData();
  }
}

void useNewPackets()
{
  sendPushPackets();
  sendPadPackets();
  sendKnobPackets();
  sendButtonPackets();
  sendSliderPackets();
}

char subscribeToNewPushPackets(void * subscriber, EventHandle handle)
{
  return addSubscription(self->newEventPacketSubscriptionChain, subscriber, handle);
}

void unsubscribeToNewPushPackets(void * subscriber)
{
  removeSubscription(self->newEventPacketSubscriptionChain, subscriber);
}

char subscribeToNewPadPackets(void * subscriber, EventHandle handle)
{
  return addSubscription(self->newPadSubscriptionChain, subscriber, handle);
}

void unsubscribeToNewPadPackets(void * subscriber)
{
  removeSubscription(self->newPadSubscriptionChain, subscriber);
}

char subscribeToNewSliderPackets(void * subscriber, EventHandle handle)
{
  return addSubscription(self->newSliderSubscriptionChain, subscriber, handle);
}

void unsubscribeToNewSliderPackets(void * subscriber)
{
  removeSubscription(self->newSliderSubscriptionChain, subscriber);
}

char subscribeToNewButtonPackets(void * subscriber, EventHandle handle)
{
  return addSubscription(self->newButtonSubscriptionChain, subscriber, handle);
}

void unsubscribeToNewButtonPackets(void * subscriber)
{
  removeSubscription(self->newButtonSubscriptionChain, subscriber);
}

char subscribeToNewKnobPackets(void * subscriber, EventHandle handle)
{
  return addSubscription(self->newKnobSubscriptionChain, subscriber, handle);
}

void unsubscribeToNewKnobPackets(void * subscriber)
{
  removeSubscription(self->newKnobSubscriptionChain, subscriber);
}

PktType packetType(void * pkt)
{
  return *(unsigned char *)pkt;
}

void printPushPacket(void * pkt)
{
  unsigned char type = *(unsigned char *)pkt;
  switch(type)
  {
    case PUSH_PKT_TYPE: ;
      pushEventPacket * pushPkt = pkt;
      printf("Packet Type: General\n");
      printf("Packet Data: 0x%08x\n", pushPkt->data);
      if(pushPkt->event_class == PAD_EVENT){
        printf("Class: Pad\nX: %i\nY: %i\nVel: %i\nState: %i\n", pushPkt->pad_x, pushPkt->pad_y, pushPkt->pad_velocity, pushPkt->pad_state);
      } else if(pushPkt->event_class == BUTTON_EVENT){
        printf("Class: Button\nID: %i\nState: %i\n", pushPkt->btn_id, pushPkt->btn_state);
      } else if(pushPkt->event_class == KNOB_EVENT){
        printf("Class: Knob\nID: %i\nDelta: %i\nState: %i\n", pushPkt->knob_id, pushPkt->knob_delta, pushPkt->knob_state);
      } else if(pushPkt->event_class == SLIDER_EVENT){
        printf("Class: Slider\nValue: %i\nPercent: %f\nState: %i\n", pushPkt->slider_value, pushPkt->slider_percent, pushPkt->slider_state);
      } else {
        printf("Class: Unknown\n");
      }
    break;

    case PAD_PKT_TYPE: ;
      padPacket * pPacket = pkt;
      printf("Packet Type: Pad\n");
      printf("Packet Data: 0x%08x\n", pPacket->data);
      printf("ID: %i\nX: %i\nY: %i\nVel: %i\nPressed: %i\nHold: %i\nRelease: %i\n", pPacket->id, pPacket->padX, pPacket->padY, pPacket->padVelocity, pPacket->isPress, pPacket->isHold, pPacket->isRelease);
    break;

    case KNOB_PKT_TYPE: ;
      knobPacket * kPacket = pkt;
      printf("Packet Type: Knob\n");
      printf("Packet Data: 0x%08x\n", kPacket->data);
      printf("ID: %i\nDirection: %i\nPress: %i\nTurning: %i\nRelease: %i\n", kPacket->id, kPacket->direction, kPacket->isPress, kPacket->isTurning, kPacket->isRelease);
    break;

    case BUTTON_PKT_TYPE: ;
      buttonPacket * btnPacket = pkt;
      printf("Packet Type: Button\n");
      printf("Packet Data: 0x%08x\n", btnPacket->data);
      printf("ID: %i\nPress: %i\nRelease: %i\n", btnPacket->btnId, btnPacket->isPress, btnPacket->isRelease);
    break;

    case SLIDER_PKT_TYPE: ;
      sliderPacket * sPacket = pkt;
      printf("Packet Type: Slider\n");
      printf("Packet Data: 0x%08x\n", sPacket->data);
      printf("Value: %i\nPercent: %f\nSliding: %i\nPress: %i\nRelease: %i\n", sPacket->value, sPacket->percent, sPacket->isSliding, sPacket->isPress, sPacket->isRelease);
    break;

    default:
    return;
  }
  printf("\n");
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

static pushEventPacket * createPushEventPacket(unsigned int data)
{
  pushEventPacket * e = (pushEventPacket*)malloc(sizeof(pushEventPacket));
  e->pktType = PUSH_PKT_TYPE;
  e->data = data;

  unsigned char b1 = (e->data & 0xff000000) >> 24;
  unsigned char b2 = (e->data & 0x00ff0000) >> 16;
  unsigned char b3 = (e->data & 0x0000ff00) >> 8;
  unsigned char b4 = e->data & 0x000000ff;

  if(b2 >= 36 && b2 <= 99 && (b4 == PAD_PRESS || b4 == PAD_HOLD || b4 == PAD_RELEASE))
  { //event is pad
    e->event_class = PAD_EVENT;

    b2 -= 36;
    e->pad_x = (b2 % 8);
    e->pad_y = (b2 / 8);

    e->pad_state = b4;
    e->pad_velocity = b1;

  }
  else if((b1 == BUTTON_PRESS || b1 == BUTTON_RELEASE) && b4 == BUTTON_TAG && (b2 > 0x0f || b2 < 0x0e) && (b2 > 0x4f || b2 < 0x47))
  {//is a btn
    e->event_class = BUTTON_EVENT;
    e->btn_id = b2;
    e->btn_state = b1;
    //printf("B: 0x%08x\n", data);
  }
  else if((b4 == 0x1e && b3 == 0xe0) || (b1 == 0x00 && b2 == 0x0c && b3 == 0x90 && b4 == 0x19) || (b1 == 0x7f && b2 == 0x0c && b3 == 0x90 && b4 == 0x19))
  {//is a slider
    e->event_class = SLIDER_EVENT;

    if(b4 == SLIDER_HOLD)
    {
      //exit(1);
      e->slider_value = ((unsigned int)b1) << 8 | b2;
      e->slider_percent = (float)e->slider_value / 0x7f7f;
      e->slider_state = SLIDER_HOLD;
    }
    else
    {
      e->slider_value = -1;
      e->slider_percent = -1;
      if(b1 == SLIDER_PRESSED)
      {
        e->slider_state = SLIDER_PRESSED;
      }
      else if(b1 == SLIDER_RELEASE)
      {
        e->slider_state = SLIDER_RELEASE;
      }
      else
      {
        e->slider_state = SLIDER_RELEASE;
      }
    }
    //printf("S: 0x%08x\n", data);
  }
  else
  {//is a knob
    e->event_class = KNOB_EVENT;

    if(b4 == KNOB_TOUCH_TAG)
    {
      e->knob_id = b2;
      e->knob_delta = 0;
      e->knob_state = b1;
    }
    else
    {
      e->knob_state = KNOB_TURN;

      if(b1 < 0x50)
      {
        e->knob_delta = b1;
      }
      else
      {
        e->knob_delta = -((int)0x7f - (int)b1 + 1);
      }

      if(b2 & 0xf0)
      {
        e->knob_id = b2 - 0x47;
      } else {
        if(b2 == 0x0e)
        {
          e->knob_id = 10;
        } else {
          e->knob_id = 9;
        }
      }
    }
  }
  return e;
}

static padPacket * pushPktToPadPkt(pushEventPacket * pkt)
{
  padPacket * padPkt = malloc(sizeof(padPacket));
  if(!padPkt) return NULL;

  padPkt->pktType = PAD_PKT_TYPE;
  padPkt->data = pkt->data;
  padPkt->id = pkt->pad_x + 8 *pkt->pad_y;
  padPkt->padX = pkt->pad_x;
  padPkt->padY = pkt->pad_y;
  padPkt->isPress = pkt->pad_state == PAD_PRESS ? 1 : 0;
  padPkt->isHold = pkt->pad_state == PAD_HOLD ? 1 : 0;
  padPkt->isRelease = pkt->pad_state == PAD_RELEASE ? 1 : 0;
  padPkt->padVelocity = pkt->pad_velocity;

  return padPkt;
}

static knobPacket * pushPktToKnobPkt(pushEventPacket * pkt)
{
  knobPacket * knobPkt = malloc(sizeof(knobPacket));
  if(!knobPkt) return NULL;

  knobPkt->pktType = KNOB_PKT_TYPE;
  knobPkt->data = pkt->data;
  knobPkt->id = pkt->knob_id;
  knobPkt->direction = (char)pkt->knob_delta;
  knobPkt->isTurning = pkt->knob_state == KNOB_TURN ? 1 : 0;
  knobPkt->isPress = pkt->knob_state == KNOB_PRESS ? 1 : 0;
  knobPkt->isRelease = pkt->knob_state == KNOB_RELEASE ? 1 : 0;
  return knobPkt;
}

static sliderPacket * pushPktToSliderPkt(pushEventPacket * pkt)
{
  sliderPacket * sliderPkt = malloc(sizeof(sliderPacket));
  if(!sliderPkt) return NULL;

  sliderPkt->pktType = SLIDER_PKT_TYPE;
  sliderPkt->data = pkt->data;
  sliderPkt->value = pkt->slider_value;
  sliderPkt->percent = pkt->slider_percent;
  sliderPkt->isSliding = pkt->slider_state == SLIDER_HOLD ? 1 : 0;
  sliderPkt->isPress = pkt->slider_state == SLIDER_PRESSED ? 1 : 0;
  sliderPkt->isRelease = pkt->slider_state == SLIDER_RELEASE ? 1 : 0;

  return sliderPkt;
}

static buttonPacket * pushPktToButtonPkt(pushEventPacket * pkt)
{
  buttonPacket * buttonPkt = malloc(sizeof(buttonPacket));
  if(!buttonPkt) return NULL;

  buttonPkt->pktType = BUTTON_PKT_TYPE;
  buttonPkt->data = pkt->data;
  buttonPkt->btnId = pkt->btn_id;
  buttonPkt->isPress = pkt->btn_state == BUTTON_PRESS ? 1 : 0;
  buttonPkt->isRelease = pkt->btn_state == BUTTON_RELEASE ? 1 : 0;

  return buttonPkt;
}

static void digestPacket(pushEventPacket * pkt)
{
  switch(pkt->event_class)
  {
    case PAD_EVENT: ;
    padPacket * padPkt = pushPktToPadPkt(pkt);
    if(padPkt) queue(self->pushPadPacketChain, padPkt);
    return;

    case BUTTON_EVENT: ;
    buttonPacket * buttonPkt = pushPktToButtonPkt(pkt);
    if(buttonPkt) queue(self->pushButtonPacketChain, buttonPkt);
    return;

    case KNOB_EVENT: ;
    knobPacket * knobPkt = pushPktToKnobPkt(pkt);
    if(knobPkt) queue(self->pushKnobPacketChain, knobPkt);
    return;

    case SLIDER_EVENT: ;
    sliderPacket * sliderPkt = pushPktToSliderPkt(pkt);
    if(sliderPkt) queue(self->pushSliderPacketChain, sliderPkt);
    return;

    default:
    return;
  }
}

static void parseInBufferData()
{
  int updateLen = self->readAmount / 4;  //get num of ints

  unsigned int* data_ptr = (unsigned int*)self->dataInBuffer;

  for(int i = 0; i < updateLen; i++)
  {
    unsigned int data = data_ptr[i];
    if(data == 0) continue;
    pushEventPacket * pkt = createPushEventPacket(data);
    digestPacket(pkt);
    queue(self->pushEventPacketChain, pkt);
  }

  self->readAmount = 0;
}

static pushEventPacket * popEventPacket()
{
  return dequeue(self->pushEventPacketChain);
}

static padPacket * popPadPacket()
{
  return dequeue(self->pushPadPacketChain);
}

static knobPacket * popKnobPacket()
{
  return dequeue(self->pushKnobPacketChain);
}

static buttonPacket * popButtonPacket()
{
  return dequeue(self->pushButtonPacketChain);
}

static sliderPacket * popSliderPacket()
{
  return dequeue(self->pushSliderPacketChain);
}

static void freeEventPacket(void * pkt)
{
  free(pkt);
}

static void freePushEventPacketChain(List * chain)
{
  destroyList(chain, freeEventPacket);
}

static void sendPushPackets()
{
  if(self->pushEventPacketChain)
  {
    pushEventPacket * pkt = (pushEventPacket *)popEventPacket();
    while(pkt)
    {
      eventTrigger(self->newEventPacketSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (pushEventPacket *)popEventPacket();
    }
  }
}

static void sendPadPackets()
{
  if(self->pushPadPacketChain)
  {
    padPacket * pkt = (padPacket *)popPadPacket();
    while(pkt)
    {
      eventTrigger(self->newPadSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (padPacket *)popPadPacket();
    }
  }
}

static void sendKnobPackets()
{
  if(self->pushKnobPacketChain)
  {
    knobPacket * pkt = (knobPacket *)popKnobPacket();
    while(pkt)
    {
      eventTrigger(self->newKnobSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (knobPacket *)popKnobPacket();
    }
  }
}

static void sendSliderPackets()
{
  if(self->pushSliderPacketChain)
  {
    sliderPacket * pkt = (sliderPacket *)popSliderPacket();
    while(pkt)
    {
      eventTrigger(self->newSliderSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (sliderPacket *)popSliderPacket();
    }
  }
}

static void sendButtonPackets()
{
  if(self->pushButtonPacketChain)
  {
    buttonPacket * pkt = (buttonPacket *)popButtonPacket();
    while(pkt)
    {
      eventTrigger(self->newButtonSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (buttonPacket *)popButtonPacket();
    }
  }
}

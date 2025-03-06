#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "PushUsbDriver.h"
#include "comm/List.h"

#include "PushEventManager.h"

/////////////
//  TYPES  //
/////////////

#define MULT            8
#define DATA_IN_SIZE    (4096 * MULT)
#define DATA_OUT_SIZE   (4096 * MULT)

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

static AbletonPkt_pushEvent * createPushEventPacket(unsigned int data);
static AbletonPkt_pad * pushPktToPadPkt(AbletonPkt_pushEvent * pkt);
static AbletonPkt_knob * pushPktToKnobPkt(AbletonPkt_pushEvent * pkt);
static AbletonPkt_slider * pushPktToSliderPkt(AbletonPkt_pushEvent * pkt);
static AbletonPkt_button * pushPktToButtonPkt(AbletonPkt_pushEvent * pkt);
static void digestPacket(AbletonPkt_pushEvent * pkt);
static void parseInBufferData();
static AbletonPkt_pushEvent * popEventPacket();
static AbletonPkt_pad * popPadPacket();
static AbletonPkt_knob * popKnobPacket();
static AbletonPkt_button * popButtonPacket();
static AbletonPkt_slider * popSliderPacket();
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

char pushEventManager_init()
{
  memset(self, 0, sizeof(EventManager));

  self->newEventPacketSubscriptionChain = subChain_subscriptionChainInit();
  self->pushEventPacketChain = Queue_init(sizeof(void*), false, freeEventPacket);

  self->newSliderSubscriptionChain = subChain_subscriptionChainInit();
  self->pushSliderPacketChain = Queue_init(sizeof(void*), false, freeEventPacket);

  self->newButtonSubscriptionChain = subChain_subscriptionChainInit();
  self->pushButtonPacketChain = Queue_init(sizeof(void*), false, freeEventPacket);

  self->newPadSubscriptionChain = subChain_subscriptionChainInit();
  self->pushPadPacketChain = Queue_init(sizeof(void*), false, freeEventPacket);

  self->newKnobSubscriptionChain = subChain_subscriptionChainInit();
  self->pushKnobPacketChain = Queue_init(sizeof(void*), false, freeEventPacket);

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

void pushEventManager_free()
{
  subChain_freeSubscriptionChain(self->newEventPacketSubscriptionChain);
  subChain_freeSubscriptionChain(self->newPadSubscriptionChain);
  subChain_freeSubscriptionChain(self->newButtonSubscriptionChain);
  subChain_freeSubscriptionChain(self->newSliderSubscriptionChain);
  subChain_freeSubscriptionChain(self->newKnobSubscriptionChain);

  freePushEventPacketChain(self->pushEventPacketChain);
  freePushEventPacketChain(self->pushPadPacketChain);
  freePushEventPacketChain(self->pushSliderPacketChain);
  freePushEventPacketChain(self->pushButtonPacketChain);
  freePushEventPacketChain(self->pushKnobPacketChain);
}

void pushEventManager_readNewUsbData()
{
  self->readAmount = PushUsbDriver_read_data(self->dataInBuffer, DATA_IN_SIZE);
  if(self->readAmount)
  {
    parseInBufferData();
  }
}

void pushEventManager_useNewPackets()
{
  sendPushPackets();
  sendPadPackets();
  sendKnobPackets();
  sendButtonPackets();
  sendSliderPackets();
}

char pushEventManager_subscribeToNewPushPackets(void * subscriber, EventHandle handle)
{
  return subChain_addSubscription(self->newEventPacketSubscriptionChain, subscriber, handle);
}

void pushEventManager_unsubscribeToNewPushPackets(void * subscriber)
{
  subChain_removeSubscription(self->newEventPacketSubscriptionChain, subscriber);
}

char pushEventManager_subscribeToNewPadPackets(void * subscriber, EventHandle handle)
{
  return subChain_addSubscription(self->newPadSubscriptionChain, subscriber, handle);
}

void pushEventManager_unsubscribeToNewPadPackets(void * subscriber)
{
  subChain_removeSubscription(self->newPadSubscriptionChain, subscriber);
}

char pushEventManager_subscribeToNewSliderPackets(void * subscriber, EventHandle handle)
{
  return subChain_addSubscription(self->newSliderSubscriptionChain, subscriber, handle);
}

void pushEventManager_unsubscribeToNewSliderPackets(void * subscriber)
{
  subChain_removeSubscription(self->newSliderSubscriptionChain, subscriber);
}

char pushEventManager_subscribeToNewButtonPackets(void * subscriber, EventHandle handle)
{
  return subChain_addSubscription(self->newButtonSubscriptionChain, subscriber, handle);
}

void pushEventManager_unsubscribeToNewButtonPackets(void * subscriber)
{
  subChain_removeSubscription(self->newButtonSubscriptionChain, subscriber);
}

char pushEventManager_subscribeToNewKnobPackets(void * subscriber, EventHandle handle)
{
  return subChain_addSubscription(self->newKnobSubscriptionChain, subscriber, handle);
}

void pushEventManager_unsubscribeToNewKnobPackets(void * subscriber)
{
  subChain_removeSubscription(self->newKnobSubscriptionChain, subscriber);
}

PktType pushEventManager_packetType(void * pkt)
{
  return *(unsigned char *)pkt;
}

void pushEventManager_printPushPacket(void * pkt)
{
  unsigned char type = *(unsigned char *)pkt;
  switch(type)
  {
    case PUSH_PKT_TYPE: ;
      AbletonPkt_pushEvent * pushPkt = pkt;
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
      AbletonPkt_pad* pPacket = pkt;
      printf("Packet Type: Pad\n");
      printf("Packet Data: 0x%08x\n", pPacket->data);
      printf("ID: %i\nX: %i\nY: %i\nVel: %i\nPressed: %i\nHold: %i\nRelease: %i\n", pPacket->id, pPacket->padX, pPacket->padY, pPacket->padVelocity, pPacket->isPress, pPacket->isHold, pPacket->isRelease);
    break;

    case KNOB_PKT_TYPE: ;
      AbletonPkt_knob* kPacket = pkt;
      printf("Packet Type: Knob\n");
      printf("Packet Data: 0x%08x\n", kPacket->data);
      printf("ID: %i\nDirection: %i\nPress: %i\nTurning: %i\nRelease: %i\n", kPacket->id, kPacket->direction, kPacket->isPress, kPacket->isTurning, kPacket->isRelease);
    break;

    case BUTTON_PKT_TYPE: ;
      AbletonPkt_button* btnPacket = pkt;
      printf("Packet Type: Button\n");
      printf("Packet Data: 0x%08x\n", btnPacket->data);
      printf("ID: %i\nPress: %i\nRelease: %i\n", btnPacket->btnId, btnPacket->isPress, btnPacket->isRelease);
    break;

    case SLIDER_PKT_TYPE: ;
      AbletonPkt_slider* sPacket = pkt;
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

static AbletonPkt_pushEvent * createPushEventPacket(unsigned int data)
{
  AbletonPkt_pushEvent * e = (AbletonPkt_pushEvent*)malloc(sizeof(AbletonPkt_pushEvent));
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

static AbletonPkt_pad * pushPktToPadPkt(AbletonPkt_pushEvent * pkt)
{
  AbletonPkt_pad * padPkt = malloc(sizeof(AbletonPkt_pad));
  if(!padPkt) return NULL;

  padPkt->pktType = PAD_PKT_TYPE;
  padPkt->data = pkt->data;
  padPkt->id = pkt->pad_x + 8 * pkt->pad_y;
  padPkt->padX = pkt->pad_x;
  padPkt->padY = pkt->pad_y;
  padPkt->isPress = pkt->pad_state == PAD_PRESS ? 1 : 0;
  padPkt->isHold = pkt->pad_state == PAD_HOLD ? 1 : 0;
  padPkt->isRelease = pkt->pad_state == PAD_RELEASE ? 1 : 0;
  padPkt->padVelocity = pkt->pad_velocity;

  return padPkt;
}

static AbletonPkt_knob * pushPktToKnobPkt(AbletonPkt_pushEvent * pkt)
{
  AbletonPkt_knob * knobPkt = malloc(sizeof(AbletonPkt_knob));
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

static AbletonPkt_slider * pushPktToSliderPkt(AbletonPkt_pushEvent * pkt)
{
  AbletonPkt_slider * sliderPkt = malloc(sizeof(AbletonPkt_slider));
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

static AbletonPkt_button * pushPktToButtonPkt(AbletonPkt_pushEvent * pkt)
{
  AbletonPkt_button * buttonPkt = malloc(sizeof(AbletonPkt_button));
  if(!buttonPkt) return NULL;

  buttonPkt->pktType = BUTTON_PKT_TYPE;
  buttonPkt->data = pkt->data;
  buttonPkt->btnId = pkt->btn_id;
  buttonPkt->isPress = pkt->btn_state == BUTTON_PRESS ? 1 : 0;
  buttonPkt->isRelease = pkt->btn_state == BUTTON_RELEASE ? 1 : 0;
  buttonPkt->isPadBtn = (pkt->btn_id >= 20 && pkt->btn_id <= 27) || (pkt->btn_id >= 102 && pkt->btn_id <= 109);

  return buttonPkt;
}

static void digestPacket(AbletonPkt_pushEvent * pkt)
{
  switch(pkt->event_class)
  {
    case PAD_EVENT: ;
      AbletonPkt_pad* padPkt = pushPktToPadPkt(pkt);
      if(padPkt) Queue_queue(self->pushPadPacketChain, padPkt);
      return;

    case BUTTON_EVENT: ;
      AbletonPkt_button* buttonPkt = pushPktToButtonPkt(pkt);
      if(buttonPkt) Queue_queue(self->pushButtonPacketChain, buttonPkt);
      return;

    case KNOB_EVENT: ;
      AbletonPkt_knob* knobPkt = pushPktToKnobPkt(pkt);
      if(knobPkt) Queue_queue(self->pushKnobPacketChain, knobPkt);
      return;

    case SLIDER_EVENT: ;
      AbletonPkt_slider* sliderPkt = pushPktToSliderPkt(pkt);
      if(sliderPkt) Queue_queue(self->pushSliderPacketChain, sliderPkt);
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
    AbletonPkt_pushEvent* pkt = createPushEventPacket(data);
    digestPacket(pkt);
    Queue_queue(self->pushEventPacketChain, pkt);
  }

  self->readAmount = 0;
}

static AbletonPkt_pushEvent * popEventPacket()
{
  return Queue_dequeue(self->pushEventPacketChain);
}

static AbletonPkt_pad * popPadPacket()
{
  return Queue_dequeue(self->pushPadPacketChain);
}

static AbletonPkt_knob * popKnobPacket()
{
  return Queue_dequeue(self->pushKnobPacketChain);
}

static AbletonPkt_button * popButtonPacket()
{
  return Queue_dequeue(self->pushButtonPacketChain);
}

static AbletonPkt_slider * popSliderPacket()
{
  return Queue_dequeue(self->pushSliderPacketChain);
}

static void freeEventPacket(void * pkt)
{
  free(pkt);
}

static void freePushEventPacketChain(List * chain)
{
  Queue_free(chain);
}

static void sendPushPackets()
{
  if(self->pushEventPacketChain)
  {
    AbletonPkt_pushEvent * pkt = (AbletonPkt_pushEvent *)popEventPacket();
    while(pkt)
    {
      subChain_eventTrigger(self->newEventPacketSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (AbletonPkt_pushEvent *)popEventPacket();
    }
  }
}

static void sendPadPackets()
{
  if(self->pushPadPacketChain)
  {
    AbletonPkt_pad * pkt = (AbletonPkt_pad *)popPadPacket();
    while(pkt)
    {
      subChain_eventTrigger(self->newPadSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (AbletonPkt_pad *)popPadPacket();
    }
  }
}

static void sendKnobPackets()
{
  if(self->pushKnobPacketChain)
  {
    AbletonPkt_knob * pkt = (AbletonPkt_knob *)popKnobPacket();
    while(pkt)
    {
      subChain_eventTrigger(self->newKnobSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (AbletonPkt_knob *)popKnobPacket();
    }
  }
}

static void sendSliderPackets()
{
  if(self->pushSliderPacketChain)
  {
    AbletonPkt_slider * pkt = (AbletonPkt_slider *)popSliderPacket();
    while(pkt)
    {
      subChain_eventTrigger(self->newSliderSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (AbletonPkt_slider *)popSliderPacket();
    }
  }
}

static void sendButtonPackets()
{
  if(self->pushButtonPacketChain)
  {
    AbletonPkt_button * pkt = (AbletonPkt_button *)popButtonPacket();
    while(pkt)
    {
      subChain_eventTrigger(self->newButtonSubscriptionChain, pkt);
      freeEventPacket(pkt);
      pkt = (AbletonPkt_button *)popButtonPacket();
    }
  }
}

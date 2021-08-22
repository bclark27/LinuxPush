#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "PushEventManager.h"
#include "EventSubscription.h"
#include "PushStates.h"

#include "PhysicalPushInputState.h"

/////////////
//  TYPES  //
/////////////

typedef struct physicalPush
{
  padState padStates[64];
  buttonState buttonStates[120];
  knobState knobStates[11];
  theSliderlderState sliderState;
  SubscriptionChain * padSubscriptionChain;
  SubscriptionChain * buttonSubscriptionChain;
  SubscriptionChain * sliderSubscriptionChain;
  SubscriptionChain * knobSubscriptionChain;

} physicalPush;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

static void handlePadUpdates(void * subscriber, void * args);
static void handleButtonUpdates(void * subscriber, void * args);
static void handleSliderUpdates(void * subscriber, void * args);
static void handleKnobUpdates(void * subscriber, void * args);

////////////////////
//  PRIVATE VARS  //
////////////////////

static physicalPush instance;
static physicalPush * self = &instance;

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

char physicalPushStateInit()
{
  memset(self, 0, sizeof(physicalPush));

  memset(&(self->padStates), 0, sizeof(padState) * 64);
  padState * pad;
  for(int y = 0; y < 8; y++)
  {
    for(int x = 0; x < 8; x++)
    {
      pad = &(self->padStates[x + (y * 8)]);
      pad->x = x;
      pad->y = y;
    }
  }

  memset(&(self->buttonStates), 0, sizeof(buttonState) * 120);
  for(int i = 0; i < 120; i++)
  {
    self->buttonStates[i].id = i;
    self->buttonStates[i].isPadButton = 0;
    if((i >= 20 && i < 28) || (i >= 102 && i < 110))
    {
      self->buttonStates[i].isPadButton = 1;
    }
  }

  memset(&(self->knobStates), 0, sizeof(knobState) * 11);
  for(int i = 0; i < 11; i++)
  {
    self->knobStates[i].id = i;
  }

  self->sliderState.value = 0;
  self->sliderState.percent = 0;
  self->sliderState.isPressed = 0;

  self->padSubscriptionChain = subscriptionChainInit();
  self->buttonSubscriptionChain = subscriptionChainInit();
  self->sliderSubscriptionChain = subscriptionChainInit();
  self->knobSubscriptionChain = subscriptionChainInit();

  if( !self->padSubscriptionChain ||
      !self->buttonSubscriptionChain ||
      !self->sliderSubscriptionChain ||
      !self->knobSubscriptionChain)
  {
    return 0;
  }

  if( !subscribeToNewPadPackets(self, handlePadUpdates) ||
      !subscribeToNewSliderPackets(self, handleSliderUpdates) ||
      !subscribeToNewButtonPackets(self, handleButtonUpdates) ||
      !subscribeToNewKnobPackets(self, handleKnobUpdates))
  {
    return 0;
  }

  return 1;
}

void freephysicalPushModel()
{
  unsubscribeToNewPadPackets(self);
  unsubscribeToNewSliderPackets(self);
  unsubscribeToNewButtonPackets(self);
  unsubscribeToNewKnobPackets(self);

  freeSubscriptionChain(self->padSubscriptionChain);
  freeSubscriptionChain(self->buttonSubscriptionChain);
  freeSubscriptionChain(self->sliderSubscriptionChain);
  freeSubscriptionChain(self->knobSubscriptionChain);
}

char subscribeToPhysicalPad(void * subscriber, EventHandle handle)
{
  return addSubscription(self->padSubscriptionChain, subscriber, handle);
}

void unsubscribeToPhysicalPad(void * subscriber)
{
  removeSubscription(self->padSubscriptionChain, subscriber);
}

char subscribeToPhysicalSlider(void * subscriber, EventHandle handle)
{
  return addSubscription(self->sliderSubscriptionChain, subscriber, handle);
}

void unsubscribeToPhysicalSlider(void * subscriber)
{
  removeSubscription(self->sliderSubscriptionChain, subscriber);
}

char subscribeToPhysicalButton(void * subscriber, EventHandle handle)
{
  return addSubscription(self->buttonSubscriptionChain, subscriber, handle);
}

void unsubscribeToPhysicalButton(void * subscriber)
{
  removeSubscription(self->buttonSubscriptionChain, subscriber);
}

char subscribeToPhysicalKnob(void * subscriber, EventHandle handle)
{
  return addSubscription(self->knobSubscriptionChain, subscriber, handle);
}

void unsubscribeToPhysicalKnob(void * subscriber)
{
  removeSubscription(self->knobSubscriptionChain, subscriber);
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

static void handlePadUpdates(void * subscriber, void * args)
{
  padPacket * pkt = args;
  int x = pkt->padX;
  int y = pkt->padY;

  if(x < 0 || y < 0 || x >= 8 || y >= 8) return;

  char update = 0;
  unsigned char pressState = pkt->isPress | pkt->isHold;
  padState * pad = &(self->padStates[x + (y * 8)]);
  if(pressState != pad->isPressed)
  {
    pad->isPressed = pressState;
    update = 1;
  }

  if(pad->velocity != pkt->padVelocity)
  {
    pad->velocity = pkt->padVelocity;
    update = 1;
  }

  if(update)
  {
    eventTrigger(self->padSubscriptionChain, pad);
  }
}

static void handleButtonUpdates(void * subscriber, void * args)
{
  buttonPacket * pkt = args;
  char update = 0;

  buttonState * btn = &(self->buttonStates[pkt->btnId]);
  if(pkt->isPress)
  {
    if(!btn->isPressed)
    {
      btn->isPressed = 1;
      update = 1;
    }
  }
  else
  {
    if(btn->isPressed)
    {
      btn->isPressed = 0;
      update = 1;
    }
  }

  if(update)
  {
    eventTrigger(self->buttonSubscriptionChain, btn);
  }
}

static void handleSliderUpdates(void * subscriber, void * args)
{
  sliderPacket * pkt = args;
  char update = 0;

  unsigned char pressState = pkt->isPress | pkt->isSliding;
  if(pressState != self->sliderState.isPressed)
  {
    self->sliderState.isPressed = pressState;
    update = 1;
  }

  if(pkt->value != self->sliderState.value)
  {
    self->sliderState.value = pkt->value;
    self->sliderState.percent = pkt->percent;
    update = 1;
  }

  if(update)
  {
    eventTrigger(self->sliderSubscriptionChain, &(self->sliderState));
  }
}

static void handleKnobUpdates(void * subscriber, void * args)
{
  knobPacket * pkt = args;

  unsigned char pressState = pkt->isPress | pkt->isTurning;
  knobState * knob = &(self->knobStates[pkt->id]);
  if(pressState != knob->isPressed)
  {
    knob->isPressed = pressState;
  }

  knob->rotation = pkt->direction;
  int value = knob->value;
  value += pkt->direction;
  value = ((value > 255) * 255) + ((value <= 255 && value >= 0) * value);
  knob->value = value;

  eventTrigger(self->knobSubscriptionChain, knob);
}

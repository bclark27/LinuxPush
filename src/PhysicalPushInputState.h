#ifndef PHYSTICAL_PUSH_STATE_H_
#define PHYSTICAL_PUSH_STATE_H_

#include "PushStates.h"
#include "EventSubscription.h"

char physicalPushStateInit();
void freephysicalPushModel();

char subscribeToPhysicalPad(void * subscriber, EventHandle handle);
void unsubscribeToPhysicalPad(void * subscriber);

char subscribeToPhysicalSlider(void * subscriber, EventHandle handle);
void unsubscribeToPhysicalSlider(void * subscriber);

char subscribeToPhysicalButton(void * subscriber, EventHandle handle);
void unsubscribeToPhysicalButton(void * subscriber);

char subscribeToPhysicalKnob(void * subscriber, EventHandle handle);
void unsubscribeToPhysicalKnob(void * subscriber);

#endif

#ifndef EVENT_SUBSCRIPTION_H_
#define EVENT_SUBSCRIPTION_H_

#include "Common.h"
#include"List.h"

///////////
// TYPES //
///////////

typedef void (* EventHandle)(void * subscriber, void * args);

typedef struct Subscription
{
  void * subscriber;
  EventHandle eventHandle;
} Subscription;

typedef List SubscriptionChain;

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

SubscriptionChain * subChain_subscriptionChainInit();
void subChain_freeSubscriptionChain(SubscriptionChain * subChain);

char subChain_addSubscription(SubscriptionChain * subChain, void * subscriber, EventHandle handle);
void subChain_removeSubscription(SubscriptionChain * subChain, void * subscriber);
void subChain_eventTrigger(SubscriptionChain * subChain, void * args);

#endif

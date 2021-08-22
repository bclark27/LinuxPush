#ifndef EVENT_SUBSCRIPTION_H_
#define EVENT_SUBSCRIPTION_H_

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

#define SubscriptionChain List

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

SubscriptionChain * subscriptionChainInit();
void freeSubscriptionChain(SubscriptionChain * subChain);
char addSubscription(SubscriptionChain * subChain, void * subscriber, EventHandle handle);
void removeSubscription(SubscriptionChain * subChain, void * subscriber);
void eventTrigger(SubscriptionChain * subChain, void * args);

#endif

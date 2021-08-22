#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PushEventManager.h"
#include "EventSubscription.h"
#include "MemWatching.h"
#include "PhysicalPushOutputState.h"
#include "List.h"
#include "PhysicalPushInputState.h"
#include "Context.h"

#include "SystemManager.h"

/////////////
//  TYPES  //
/////////////

typedef struct SystemManager
{
  SubscriptionChain * input_padSubscriptionChain;
  SubscriptionChain * input_buttonSubscriptionChain;
  SubscriptionChain * input_sliderSubscriptionChain;
  SubscriptionChain * input_knobSubscriptionChain;

  SubscriptionChain * output_textSubscriptionChain;
  SubscriptionChain * output_padSubscriptionChain;
  SubscriptionChain * output_buttonSubscriptionChain;
  SubscriptionChain * output_padButtonSubscriptionChain;

  Context * mainMenu;
  Context * activeContext;
  List * contextList;

  char systemIsOn;

  char inMainMenuFlag;
} SystemManager;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

static char newContext(char * fileName);
static void removeContext(Context * context);
static char connectSystemToContext(Context * context);
static void disconnectSystemToContext(Context * context);
static void switchToContext(Context * context);

static char setupMainMenuContext();
static void destroyMainMenuContext();

static void input_handlePadUpdates(void * subscriber, void * args);
static void input_handleButtonUpdates(void * subscriber, void * args);
static void input_handleSliderUpdates(void * subscriber, void * args);
static void input_handleKnobUpdates(void * subscriber, void * args);

static void output_handlePadUpdates(void * subscriber, void * args);
static void output_handleTextUpdates(void * subscriber, void * args);
static void output_handleButtonUpdates(void * subscriber, void * args);
static void output_handlePadButtonUpdates(void * subscriber, void * args);

////////////////////
//  PRIVATE VARS  //
////////////////////

static SystemManager instance;
static SystemManager * self = &instance;

static unsigned long mainMenuConfigBinaryLength = 1;
static unsigned char mainMenuConfigBinary[1] = {0};
////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

char systemManagerInit()
{
  memset(self, 0, sizeof(SystemManager));
  self->inMainMenuFlag = 1;
  self->mainMenu = NULL;
  self->activeContext = NULL;

  char pass = 1;

  pass &= subscribeToPhysicalPad(self, input_handlePadUpdates);
  pass &= subscribeToPhysicalSlider(self, input_handleSliderUpdates);
  pass &= subscribeToPhysicalButton(self, input_handleButtonUpdates);
  pass &= subscribeToPhysicalKnob(self, input_handleKnobUpdates);

  self->input_padSubscriptionChain = subscriptionChainInit();
  self->input_buttonSubscriptionChain = subscriptionChainInit();
  self->input_sliderSubscriptionChain = subscriptionChainInit();
  self->input_knobSubscriptionChain = subscriptionChainInit();

  self->contextList = listInit();
  pass &= (self->contextList == NULL) ? 0 : 1;

  pass &= setupMainMenuContext();

  self->output_textSubscriptionChain = subscriptionChainInit();
  self->output_padSubscriptionChain = subscriptionChainInit();
  self->output_buttonSubscriptionChain = subscriptionChainInit();
  self->output_padButtonSubscriptionChain = subscriptionChainInit();


  self->systemIsOn = pass;
  return pass;
}

void destroySystemManager()
{

  freeSubscriptionChain(self->output_padButtonSubscriptionChain);
  freeSubscriptionChain(self->output_buttonSubscriptionChain);
  freeSubscriptionChain(self->output_padSubscriptionChain);
  freeSubscriptionChain(self->output_textSubscriptionChain);

  destroyMainMenuContext();

  for(Link * link = self->contextList->head; link; link = link->next)
  {
    Context * c = link->data;
    if(c == NULL) continue;
    if(self->activeContext == c) self->activeContext = NULL;
    disconnectSystemToContext(c);
    context_freeContext(c);
  }

  destroyListDynamic(self->contextList);

  freeSubscriptionChain(self->input_padSubscriptionChain);
  freeSubscriptionChain(self->input_buttonSubscriptionChain);
  freeSubscriptionChain(self->input_sliderSubscriptionChain);
  freeSubscriptionChain(self->input_knobSubscriptionChain);

  unsubscribeToPhysicalKnob(self);
  unsubscribeToPhysicalButton(self);
  unsubscribeToPhysicalSlider(self);
  unsubscribeToPhysicalPad(self);
}

void systemLoop()
{
  while(self->systemIsOn)
  {
    readNewUsbData();
    useNewPackets();

    context_runContext(self->activeContext);

    pushOutputState_updatePush();
  }
}

char SystemManager_subscribeToPadInputs(void * subscriber, EventHandle handle)
{
  return addSubscription(self->input_padSubscriptionChain, subscriber, handle);
}

void SystemManager_unsubscribeToPadInputs(void * subscriber)
{
  removeSubscription(self->input_padSubscriptionChain, subscriber);
}

char SystemManager_subscribeToKnobInputs(void * subscriber, EventHandle handle)
{
  return addSubscription(self->input_knobSubscriptionChain, subscriber, handle);
}

void SystemManager_unsubscribeToKnobInputs(void * subscriber)
{
  removeSubscription(self->input_knobSubscriptionChain, subscriber);
}

char SystemManager_subscribeToSliderInputs(void * subscriber, EventHandle handle)
{
  return addSubscription(self->input_sliderSubscriptionChain, subscriber, handle);
}

void SystemManager_unsubscribeToSliderInputs(void * subscriber)
{
  removeSubscription(self->input_sliderSubscriptionChain, subscriber);
}

char SystemManager_subscribeToButtonInputs(void * subscriber, EventHandle handle)
{
  return addSubscription(self->input_buttonSubscriptionChain, subscriber, handle);
}

void SystemManager_unsubscribeToButtonInputs(void * subscriber)
{
  removeSubscription(self->input_buttonSubscriptionChain, subscriber);
}

char SystemManager_subscribeToTextOutputs(void * subscriber, EventHandle handle)
{
  return addSubscription(self->output_textSubscriptionChain, subscriber, handle);
}

void SystemManager_unsubscribeToTextOutputs(void * subscriber)
{
  removeSubscription(self->output_textSubscriptionChain, subscriber);
}

char SystemManager_subscribeToPadOutputs(void * subscriber, EventHandle handle)
{
  return addSubscription(self->output_padSubscriptionChain, subscriber, handle);
}

void SystemManager_unsubscribeToPadOutputs(void * subscriber)
{
  removeSubscription(self->output_padSubscriptionChain, subscriber);
}

char SystemManager_subscribeToButtonOutputs(void * subscriber, EventHandle handle)
{
  return addSubscription(self->output_buttonSubscriptionChain, subscriber, handle);
}

void SystemManager_unsubscribeToButtonOutputs(void * subscriber)
{
  removeSubscription(self->output_buttonSubscriptionChain, subscriber);
}

char SystemManager_subscribeToPadButtonOutputs(void * subscriber, EventHandle handle)
{
  return addSubscription(self->output_padButtonSubscriptionChain, subscriber, handle);
}

void SystemManager_unsubscribeToPadButtonOutputs(void * subscriber)
{
  removeSubscription(self->output_padButtonSubscriptionChain, subscriber);
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

static char newContext(char * fileName)
{
  Context * context = NULL;

  if(context_initContext(context) &&
     context_loadContextConfig(context, fileName) &&
     queue(self->contextList, context) &&
     connectSystemToContext(context))
  {
    return 1;
  }

  context_freeContext(context);
  return 0;
}

static void removeContext(Context * context)
{
  if(context == NULL || !inList(self->contextList, context))
  {
    if(context == self->activeContext) self->activeContext = NULL;
    disconnectSystemToContext(context);
    deleteItem(self->contextList, context, context_freeContext);
  }
}

static char connectSystemToContext(Context * context)
{
  char pass = 1;

  pass &= context_subscribeToTextOutputs(context, self, output_handleTextUpdates);
  pass &= context_subscribeToPadOutputs(context, self, output_handlePadUpdates);
  pass &= context_subscribeToButtonOutputs(context, self, output_handleButtonUpdates);
  pass &= context_subscribeToPadButtonOutputs(context, self, output_handlePadButtonUpdates);

  return pass;
}

static void disconnectSystemToContext(Context * context)
{
  context_unsubscribeToTextOutputs(context, context);
  context_unsubscribeToPadOutputs(context, self);
  context_unsubscribeToButtonOutputs(context, self);
  context_unsubscribeToPadButtonOutputs(context, self);
}

static void switchToContext(Context * context)
{
  if(context == NULL) return;
  if(context->contextIsActive && self->activeContext == context) return;

  if(!(context == self->mainMenu || inList(self->contextList, context))) return;

  context_setToInactive(self->mainMenu);

  for(Link * link = self->contextList->head; link; link = link->next)
  {
    context_setToInactive(link->data);
  }

  context_setToActive(context);
  self->activeContext = context;
}

static char setupMainMenuContext()
{
  char pass = 1;

  self->mainMenu = malloc(sizeof(Context));
  pass &= (self->mainMenu == NULL) ? 0 : 1;
  pass &= context_initContext(self->mainMenu);
  pass &= context_loadContextBinary(self->mainMenu, mainMenuConfigBinary, mainMenuConfigBinaryLength);
  pass &= connectSystemToContext(self->mainMenu);
  switchToContext(self->mainMenu);

  return pass;
}

static void destroyMainMenuContext()
{
  if(self->mainMenu == NULL) return;
  if(self->activeContext == self->mainMenu) self->activeContext = NULL;
  disconnectSystemToContext(self->mainMenu);
  context_freeContext(self->mainMenu);
}

static void input_handleButtonUpdates(void * subscriber, void * args)
{
  buttonState * btn = args;

  if(btn->id == 3)
  {
    self->systemIsOn = 0;
  }

  eventTrigger(self->input_buttonSubscriptionChain, args);
}

static void input_handleSliderUpdates(void * subscriber, void * args)
{
  eventTrigger(self->input_sliderSubscriptionChain, args);
}

static void input_handleKnobUpdates(void * subscriber, void * args)
{
  eventTrigger(self->input_knobSubscriptionChain, args);
}

static void input_handlePadUpdates(void * subscriber, void * args)
{
  eventTrigger(self->input_padSubscriptionChain, args);
}

static void output_handlePadUpdates(void * subscriber, void * args)
{
  eventTrigger(self->output_padSubscriptionChain, args);
}

static void output_handleTextUpdates(void * subscriber, void * args)
{
  eventTrigger(self->output_textSubscriptionChain, args);
}

static void output_handleButtonUpdates(void * subscriber, void * args)
{
  eventTrigger(self->output_buttonSubscriptionChain, args);
}

static void output_handlePadButtonUpdates(void * subscriber, void * args)
{
  eventTrigger(self->output_padButtonSubscriptionChain, args);
}

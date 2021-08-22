#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Context.h"
#include "List.h"
#include "PushStateObjects.h"
#include "PhysicalPushOutputState.h"
#include "ConfigUtils.h"

#include "CMVirtualOut.h"

/////////////
//  TYPES  //
/////////////


typedef enum subHeaders
{
  subHeaders_layerConfigBuffer = (unsigned int) 0x01,
} subHeaders;

typedef struct layerConfigBuffer
{
  int id;
} layerConfigBuffer;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

static void handleSetTextState(void * subscriber, void * args);
static void handleSetPadState(void * subscriber, void * args);
static void handleSetButtonState(void * subscriber, void * args);
static void handleSetPadButtonState(void * subscriber, void * args);

static void cleanForBufferRead(CMVirtualOutput * self);
static void destroyLayer(void * ptr);

static char addNewLayer(struct Context * context, int id);
static char compareLayersById(void * a, void * b);
static char compareLayerToId(void * id, void * layerData);
static void removeLayer(struct Context * context, int id);

///////////////////////
//  PUBLIC FUNCTION  //
///////////////////////

char cmVirtualOutputInit(struct Context * context)
{
  if(context == NULL) return 0;
  context->virtualOutput = malloc(sizeof(CMVirtualOutput));
  if(context->virtualOutput == NULL) return 0;

  CMVirtualOutput * vr = context->virtualOutput;

  vr->context = context;

  char pass = 1;
  pass &= initPushStateObject(&vr->realPushState);
  pass &= initPushStateObject(&vr->workingPushState);

  vr->layerList = listInit();
  addNewLayer(context, 0);
  //here add subscriptions to other modules SUBSCRIBER IS CMVirtualOutput

  vr->output_textSubscriptionChain = subscriptionChainInit();
  vr->output_padSubscriptionChain = subscriptionChainInit();
  vr->output_buttonSubscriptionChain = subscriptionChainInit();
  vr->output_padButtonSubscriptionChain = subscriptionChainInit();

  if( vr->output_textSubscriptionChain &&
      vr->output_padSubscriptionChain &&
      vr->output_buttonSubscriptionChain &&
      vr->output_padButtonSubscriptionChain)
  {
    pass &= 1;
  }
  else
  {
    pass = 0;
  }

  return pass;
}

void cmVirtualOutputFree(struct Context * context)
{
  if(context == NULL) return;
  if(context->virtualOutput == NULL) return;
  CMVirtualOutput * vr = context->virtualOutput;

  freeSubscriptionChain(vr->output_padButtonSubscriptionChain);
  freeSubscriptionChain(vr->output_buttonSubscriptionChain);
  freeSubscriptionChain(vr->output_padSubscriptionChain);
  freeSubscriptionChain(vr->output_textSubscriptionChain);

  //here unsubscribe to ther modules SUBSCRIBER IS CMVirtualOutput

  free(vr);
}

char cmVirtualOutput_addLayer(struct Context * context, int id);
void cmVirtualOutput_removeLayer(struct Context * context, int id);

void cmVirtualOutput_setTextState(struct Context * context);
void cmVirtualOutput_setPadState(struct Context * context);
void cmVirtualOutput_setButtonState(struct Context * context);
void cmVirtualOutput_setPadButtonState(struct Context * context);

char cmVirtualOutput_subscribeToTextOutputs(struct Context * context, void * subscriber, EventHandle handle)
{
  return addSubscription(context->virtualOutput->output_textSubscriptionChain, subscriber, handle);
}

void cmVirtualOutput_unsubscribeToTextOutputs(struct Context * context, void * subscriber)
{
  removeSubscription(context->virtualOutput->output_textSubscriptionChain, subscriber);
}

char cmVirtualOutput_subscribeToPadOutputs(struct Context * context, void * subscriber, EventHandle handle)
{
  return addSubscription(context->virtualOutput->output_padSubscriptionChain, subscriber, handle);
}

void cmVirtualOutput_unsubscribeToPadOutputs(struct Context * context, void * subscriber)
{
  removeSubscription(context->virtualOutput->output_padSubscriptionChain, subscriber);
}

char cmVirtualOutput_subscribeToButtonOutputs(struct Context * context, void * subscriber, EventHandle handle)
{
  return addSubscription(context->virtualOutput->output_buttonSubscriptionChain, subscriber, handle);
}

void cmVirtualOutput_unsubscribeToButtonOutputs(struct Context * context, void * subscriber)
{
  removeSubscription(context->virtualOutput->output_buttonSubscriptionChain, subscriber);
}

char cmVirtualOutput_subscribeToPadButtonOutputs(struct Context * context, void * subscriber, EventHandle handle)
{
  return addSubscription(context->virtualOutput->output_padButtonSubscriptionChain, subscriber, handle);
}

void cmVirtualOutput_unsubscribeToPadButtonOutputs(struct Context * context, void * subscriber)
{
  removeSubscription(context->virtualOutput->output_padButtonSubscriptionChain, subscriber);
}

char cmVirtualOutput_readConfig(struct Context * context)
{
  unsigned int length;
  unsigned char * config = findModuleConfigInBuffer(context, ConfigHeaders_CMVirtualOutput, &length);
  unsigned char * start = config;
  if(config == NULL) return 0;

  cleanForBufferRead(context->virtualOutput);

  unsigned int header;

  while(config - start < length)
  {
    header = *(unsigned int *)config;
    config += 4;

    switch(header)
    {
      case subHeaders_layerConfigBuffer:
      {
        layerConfigBuffer * lcb = (layerConfigBuffer *)config;
        config += sizeof(layerConfigBuffer);

        addNewLayer(context, lcb->id);
      }
    }
  }
}

unsigned int cmVirtualOutput_writeConfig(struct Context * context, unsigned char * buffer)
{
  CMVirtualOutput * vr = context->virtualOutput;
  unsigned char * start = buffer;

  for(Link * link = vr->layerList->head; link; link = link->next)
  {
    if(link->data == NULL) continue;
    layerConfigBuffer lcb = { .id = ((VirtualOutputLayer*)(link->data))->id};
    unsigned int sh = subHeaders_layerConfigBuffer;

    memcpy(buffer, &sh, sizeof(unsigned int));
    buffer += sizeof(unsigned int);
    memcpy(buffer, &lcb, sizeof(layerConfigBuffer));
    buffer += sizeof(layerConfigBuffer);
  }

  return buffer - start;
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

static void handleSetTextState(void * subscriber, void * args)
{
  CMVirtualOutput * self = subscriber;
}

static void handleSetPadState(void * subscriber, void * args)
{
  CMVirtualOutput * self = subscriber;
}

static void handleSetButtonState(void * subscriber, void * args)
{
  CMVirtualOutput * self = subscriber;
}

static void handleSetPadButtonState(void * subscriber, void * args)
{
  CMVirtualOutput * self = subscriber;
}

static void cleanForBufferRead(CMVirtualOutput * self)
{
  clearList(self->layerList, destroyLayer);
  initPushStateObject(&self->realPushState);
  initPushStateObject(&self->workingPushState);
}

static void destroyLayer(void * ptr)
{
  free(ptr);
}

static char addNewLayer(struct Context * context, int id)
{
  VirtualOutputLayer * layer = malloc(sizeof(VirtualOutputLayer));
  if(layer == NULL) return 0;
  layer->id = id;
  layer->colors = malloc(sizeof(context->padWidth * context->padHeight));
  layer->blinks = malloc(sizeof(context->padWidth * context->padHeight));

  if(layer->colors == NULL || layer->blinks == NULL || findByProperty(context->virtualOutput->layerList, &id, compareLayerToId)) return 0;

  queue(context->virtualOutput->layerList, layer);
  sortList(context->virtualOutput->layerList, compareLayersById);
}

static char compareLayersById(void * a, void * b)
{
  VirtualOutputLayer * layerA = a;
  VirtualOutputLayer * layerB = b;

  if(layerA->id > layerB->id) return 1;
  if(layerA->id < layerB->id) return -1;
  return 0;
}

static char compareLayerToId(void * id, void * layerData)
{
  VirtualOutputLayer * layer = layerData;
  return (*(int*)id) == layer->id;
}

static void removeLayer(struct Context * context, int id)
{
  VirtualOutputLayer * layer = findByProperty(context->virtualOutput->layerList, &id, compareLayerToId);

  if(layer == NULL) return;

  deleteItem(context->virtualOutput->layerList, &id, destroyLayer);
}

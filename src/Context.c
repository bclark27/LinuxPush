#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "PushEventManager.h"
#include "PushStates.h"
#include "PhysicalPushInputState.h"
#include "PhysicalPushOutputState.h"
#include "List.h"
#include "SystemManager.h"
#include "LightAndTextStates.h"
#include "CMVirtualOut.h"
#include "ConfigUtils.h"

#include "Context.h"

/////////////
//  TYPES  //
/////////////

typedef union MemoryData
{
  unsigned char U1;
  unsigned short U2;
  unsigned int U4;
  unsigned long U8;
  unsigned char * USTRING;
} MemoryData;

typedef struct MemoryObject
{
  unsigned long id;
  MemoryType type;
  MemoryData * mem;
} MemoryObject;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

static void handlePadUpdates(void * subscriber, void * args);
static void handleButtonUpdates(void * subscriber, void * args);
static void handleSliderUpdates(void * subscriber, void * args);
static void handleKnobUpdates(void * subscriber, void * args);

static void handlePadVirtualOutputToContextOutput(void * subscriber, void * args);
static void handleTextVirtualOutputToContextOutput(void * subscriber, void * args);
static void handleButtonVirtualOutputToContextOutput(void * subscriber, void * args);
static void handlePadButtonVirtualOutputToContextOutput(void * subscriber, void * args);

static void destroyMemoryObject(void * args);
static void writeToMemory(Context * self, MemoryType type, unsigned long id, void * var);
static char compareMemoryObjectWithId(void * id, void * memoryObj);

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

char context_initContext(Context * self)
{
  if(self == NULL) return 0;

  memset(self, 0, sizeof(Context));
  self->contextIsLoaded = 0;
  self->memoryObjIdCounter = 0;
  self->memory = listInit();
  self->contextIsActive = 0;
  self->configBufer = NULL;
  self->configBufferLen = 0;

  char pass = 1;

  pass &= SystemManager_subscribeToPadInputs(self, handlePadUpdates);
  pass &= SystemManager_subscribeToSliderInputs(self, handleSliderUpdates);
  pass &= SystemManager_subscribeToButtonInputs(self, handleButtonUpdates);
  pass &= SystemManager_subscribeToKnobInputs(self, handleKnobUpdates);

  self->input_padSubscriptionChain = subscriptionChainInit();
  self->input_buttonSubscriptionChain = subscriptionChainInit();
  self->input_sliderSubscriptionChain = subscriptionChainInit();
  self->input_knobSubscriptionChain = subscriptionChainInit();

  //add other modules that sub to inputs here, before the virt

  pass &= cmVirtualOutputInit(self);

  cmVirtualOutput_subscribeToTextOutputs(self, self, handleTextVirtualOutputToContextOutput);
  cmVirtualOutput_subscribeToPadOutputs(self, self, handlePadVirtualOutputToContextOutput);
  cmVirtualOutput_subscribeToButtonOutputs(self, self, handleButtonVirtualOutputToContextOutput);
  cmVirtualOutput_subscribeToPadButtonOutputs(self, self, handlePadButtonVirtualOutputToContextOutput);

  self->output_textSubscriptionChain = subscriptionChainInit();
  self->output_padSubscriptionChain = subscriptionChainInit();
  self->output_buttonSubscriptionChain = subscriptionChainInit();
  self->output_padButtonSubscriptionChain = subscriptionChainInit();

  return pass;
}

void context_freeContext(void * context)
{
  if(context == NULL) return;
  Context * self = context;

  freeSubscriptionChain(self->output_padButtonSubscriptionChain);
  freeSubscriptionChain(self->output_buttonSubscriptionChain);
  freeSubscriptionChain(self->output_padSubscriptionChain);
  freeSubscriptionChain(self->output_textSubscriptionChain);

  cmVirtualOutput_unsubscribeToTextOutputs(self, self);
  cmVirtualOutput_unsubscribeToPadOutputs(self, self);
  cmVirtualOutput_unsubscribeToButtonOutputs(self, self);
  cmVirtualOutput_unsubscribeToPadButtonOutputs(self, self);

  cmVirtualOutputFree(self);

  //free other modules here AFTER the virt out

  freeSubscriptionChain(self->input_padSubscriptionChain);
  freeSubscriptionChain(self->input_buttonSubscriptionChain);
  freeSubscriptionChain(self->input_sliderSubscriptionChain);
  freeSubscriptionChain(self->input_knobSubscriptionChain);

  SystemManager_unsubscribeToPadInputs(self);
  SystemManager_unsubscribeToKnobInputs(self);
  SystemManager_unsubscribeToSliderInputs(self);
  SystemManager_unsubscribeToButtonInputs(self);

  destroyList(self->memory, destroyMemoryObject);

  if(self->configBufer) free(self->configBufer);
  self->contextIsLoaded = 0;
  self->contextIsActive = 0;

  free(self);
}

void context_runContext(Context * self)
{
  if(self->contextIsLoaded && self->contextIsActive)
  {

  }
}

void context_setToActive(Context * self)
{
  self->contextIsActive = 1;
}
void context_setToInactive(Context * self)
{
  self->contextIsActive = 0;
}

int context_getPushWidth(Context * self)
{
  if(self->contextIsLoaded) return self->padWidth;
  return 0;
}

int context_getPushHeight(Context * self)
{
  if(self->contextIsLoaded) return self->padHeight;
  return 0;
}

int context_getDeltaX(Context * self)
{
  if(self->contextIsLoaded) return self->deltaX;
  return 0;
}

int context_getDeltaY(Context * self)
{
  if(self->contextIsLoaded) return self->deltaY;
  return 0;
}

char context_loadContextConfig(Context * self, char * fileName)
{
  if(self == NULL) return 0;
  char pass = 1;

  FILE * f = fopen(fileName, "r");

  unsigned char * configBuffer;



  self->contextIsLoaded = pass;
  return pass;
}

char context_loadContextBinary(Context * self, unsigned char * buffer, unsigned long length)
{
  if(self == NULL) return 0;
  char pass = 1;

  self->contextIsLoaded = pass;
  return pass;
}

char context_exportContextConfig(Context * self, char * fileName)
{
  return 0;
}

char context_subscribeToPadInputs(Context * self, void * subscriber, EventHandle handle)
{
  return addSubscription(self->input_padSubscriptionChain, subscriber, handle);
}

void context_unsubscribeToPadInputs(Context * self, void * subscriber)
{
  removeSubscription(self->input_padSubscriptionChain, subscriber);
}

char context_subscribeToKnobInputs(Context * self, void * subscriber, EventHandle handle)
{
  return addSubscription(self->input_knobSubscriptionChain, subscriber, handle);
}

void context_unsubscribeToKnobInputs(Context * self, void * subscriber)
{
  removeSubscription(self->input_knobSubscriptionChain, subscriber);
}

char context_subscribeToSliderInputs(Context * self, void * subscriber, EventHandle handle)
{
  return addSubscription(self->input_sliderSubscriptionChain, subscriber, handle);
}

void context_unsubscribeToSliderInputs(Context * self, void * subscriber)
{
  removeSubscription(self->input_sliderSubscriptionChain, subscriber);
}

char context_subscribeToButtonInputs(Context * self, void * subscriber, EventHandle handle)
{
  return addSubscription(self->input_buttonSubscriptionChain, subscriber, handle);
}

void context_unsubscribeToButtonInputs(Context * self, void * subscriber)
{
  removeSubscription(self->input_buttonSubscriptionChain, subscriber);
}

char context_subscribeToTextOutputs(Context * self, void * subscriber, EventHandle handle)
{
  return addSubscription(self->output_textSubscriptionChain, subscriber, handle);
}

void context_unsubscribeToTextOutputs(Context * self, void * subscriber)
{
  removeSubscription(self->output_textSubscriptionChain, subscriber);
}

char context_subscribeToPadOutputs(Context * self, void * subscriber, EventHandle handle)
{
  return addSubscription(self->output_padSubscriptionChain, subscriber, handle);
}

void context_unsubscribeToPadOutputs(Context * self, void * subscriber)
{
  removeSubscription(self->output_padSubscriptionChain, subscriber);
}

char context_subscribeToButtonOutputs(Context * self, void * subscriber, EventHandle handle)
{
  return addSubscription(self->output_buttonSubscriptionChain, subscriber, handle);
}

void context_unsubscribeToButtonOutputs(Context * self, void * subscriber)
{
  removeSubscription(self->output_buttonSubscriptionChain, subscriber);
}

char context_subscribeToPadButtonOutputs(Context * self, void * subscriber, EventHandle handle)
{
  return addSubscription(self->output_padButtonSubscriptionChain, subscriber, handle);
}

void context_unsubscribeToPadButtonOutputs(Context * self, void * subscriber)
{
  removeSubscription(self->output_padButtonSubscriptionChain, subscriber);
}

char contextMem_memoryLookupFail(Context * self)
{
  return self->memoryLookupFail;
}

char contextMem_memoryWriteFail(Context * self)
{
  return self->memoryWriteFail;
}

char contextMem_memoryAllocateFail(Context * self)
{
  return self->memoryAllocateFail;
}

unsigned char contextMem_getU1(Context * self, unsigned long id)
{
  self->memoryLookupFail = 0;
  MemoryObject * memoryObj = findByProperty(self->memory, &id, compareMemoryObjectWithId);

  if(memoryObj && memoryObj->type == MemoryTypes_U1)
  {
    return (memoryObj->mem)->U1;
  }

  self->memoryLookupFail = 1;
  return 0;
}

unsigned short contextMem_getU2(Context * self, unsigned long id)
{
  self->memoryLookupFail = 0;
  MemoryObject * memoryObj = findByProperty(self->memory, &id, compareMemoryObjectWithId);

  if(memoryObj && memoryObj->type == MemoryTypes_U2)
  {
    return (memoryObj->mem)->U2;

  }
  self->memoryLookupFail = 1;
  return 0;
}

unsigned int contextMem_getU4(Context * self, unsigned long id)
{
  self->memoryLookupFail = 0;
  MemoryObject * memoryObj = findByProperty(self->memory, &id, compareMemoryObjectWithId);

  if(memoryObj && memoryObj->type == MemoryTypes_U4)
  {
    return (memoryObj->mem)->U4;
  }

  self->memoryLookupFail = 1;
  return 0;
}

unsigned long contextMem_getU8(Context * self, unsigned long id)
{
  self->memoryLookupFail = 0;
  MemoryObject * memoryObj = findByProperty(self->memory, &id, compareMemoryObjectWithId);

  if(memoryObj && memoryObj->type == MemoryTypes_U8)
  {
    return (memoryObj->mem)->U8;
  }

  self->memoryLookupFail = 1;
  return 0;
}

unsigned char * contextMem_getUSTRING(Context * self, unsigned long id)
{
  self->memoryLookupFail = 0;
  MemoryObject * memoryObj = findByProperty(self->memory, &id, compareMemoryObjectWithId);

  if(memoryObj && memoryObj->type == MemoryTypes_STRING)
  {
    return (memoryObj->mem)->USTRING;
  }

  self->memoryLookupFail = 1;
  return 0;
}

char contextMem_setU1(Context * self, unsigned char val, unsigned long id)
{
  writeToMemory(self, MemoryTypes_U1, id, &val);
  return self->memoryWriteFail;
}

char contextMem_setU2(Context * self, unsigned short val, unsigned long id)
{
  writeToMemory(self, MemoryTypes_U2, id, &val);
  return self->memoryWriteFail;
}

char contextMem_setU4(Context * self, unsigned int val, unsigned long id)
{
  writeToMemory(self, MemoryTypes_U4, id, &val);
  return self->memoryWriteFail;
}

char contextMem_setU8(Context * self, unsigned long val, unsigned long id)
{
  writeToMemory(self, MemoryTypes_U8, id, &val);
  return self->memoryWriteFail;
}

char contextMem_setUSTRING(Context * self, unsigned char * val, unsigned long id)
{
  writeToMemory(self, MemoryTypes_STRING, id, &val);
  return self->memoryWriteFail;
}


unsigned long contextMem_createMemoryObject(Context * self, MemoryType type)
{
  self->memoryAllocateFail = 0;
  MemoryObject * memoryObj = malloc(sizeof(MemoryObject));
  MemoryData * data = malloc(sizeof(MemoryData));

  if(memoryObj == NULL || data == NULL)
  {
    free(memoryObj);
    free(data);
    self->memoryAllocateFail = 1;
    return 0;
  }

  memset(data, 0, sizeof(MemoryData));

  self->memoryObjIdCounter++;
  memoryObj->id = self->memoryObjIdCounter;
  memoryObj->type = type;
  memoryObj->mem = data;

  if(!queue(self->memory, memoryObj))
  {
    destroyMemoryObject(memoryObj);
    self->memoryObjIdCounter--;
    self->memoryAllocateFail = 1;
    return 0;
  }

  return self->memoryObjIdCounter;
}

void contextMem_freeMemory(Context * self, unsigned long id)
{
  deleteItem(self->memory, findByProperty(self->memory, &id, compareMemoryObjectWithId), destroyMemoryObject);
}

MemoryType contextMem_getMemoryType(Context * self, unsigned long id)
{
  self->memoryLookupFail = 0;
  MemoryObject * memoryObj = findByProperty(self->memory, &id, compareMemoryObjectWithId);

  if(memoryObj == NULL)
  {
    self->memoryLookupFail = 1;
    return 0;
  }

  return memoryObj->type;
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

static void handleButtonUpdates(void * subscriber, void * args)
{
  Context * self = subscriber;
  if(self == NULL || !self->contextIsActive) return;

  if(self->contextIsLoaded)
    eventTrigger(self->input_buttonSubscriptionChain, args);
}

static void handleSliderUpdates(void * subscriber, void * args)
{
  Context * self = subscriber;
  if(self == NULL || !self->contextIsActive) return;

  if(self->contextIsLoaded)
    eventTrigger(self->input_sliderSubscriptionChain, args);
}

static void handleKnobUpdates(void * subscriber, void * args)
{
  Context * self = subscriber;
  if(self == NULL || !self->contextIsActive) return;

  if(self->contextIsLoaded)
    eventTrigger(self->input_knobSubscriptionChain, args);
}

static void handlePadUpdates(void * subscriber, void * args)
{
  Context * self = subscriber;
  if(self == NULL || !self->contextIsActive) return;

  if(self->contextIsLoaded)
  {
    padState * pkt = args;

    pkt->x += self->deltaX;
    pkt->y += self->deltaY;

    if(pkt->x >= 0 && pkt->y >= 0 && pkt->x < self->padWidth && pkt->y < self->padHeight)
    eventTrigger(self->input_padSubscriptionChain, args);
  }
}

static void handlePadVirtualOutputToContextOutput(void * subscriber, void * args)
{
  Context * self = subscriber;
  if(self == NULL || !self->contextIsActive) return;

  eventTrigger(self->output_padSubscriptionChain, args);
}

static void handleTextVirtualOutputToContextOutput(void * subscriber, void * args)
{
  Context * self = subscriber;
  if(self == NULL || !self->contextIsActive) return;

  eventTrigger(self->output_textSubscriptionChain, args);
}

static void handleButtonVirtualOutputToContextOutput(void * subscriber, void * args)
{
  Context * self = subscriber;
  if(self == NULL || !self->contextIsActive) return;

  eventTrigger(self->output_buttonSubscriptionChain, args);
}

static void handlePadButtonVirtualOutputToContextOutput(void * subscriber, void * args)
{
  Context * self = subscriber;
  if(self == NULL || !self->contextIsActive) return;

  eventTrigger(self->output_padButtonSubscriptionChain, args);
}

static void destroyMemoryObject(void * args)
{
  MemoryObject * m = args;
  switch(m->type)
  {
    case MemoryTypes_STRING:;
      free(m->mem->USTRING);
      free(m->mem);
      break;
    default:
      free(m->mem);
  }
  free(m);
}

static void writeToMemory(Context * self, MemoryType type, unsigned long id, void * var)
{
  self->memoryWriteFail = 0;
  MemoryObject * memObj = findByProperty(self->memory, &id, compareMemoryObjectWithId);

  if(memObj == NULL || memObj->type != type)
  {
    self->memoryWriteFail = 1;
    return;
  }

  switch(type)
  {
    case MemoryTypes_U1:;
      memObj->mem->U1 = *(unsigned char*)var;
      break;
    case MemoryTypes_U2:;
      memObj->mem->U2 = *(unsigned short*)var;
      break;
    case MemoryTypes_U4:;
      memObj->mem->U4 = *(unsigned int*)var;
      break;
    case MemoryTypes_U8:;
      memObj->mem->U8 = *(unsigned long*)var;
      break;
    case MemoryTypes_STRING:;
      memObj->mem->USTRING = *(unsigned char**)var;
      break;
    default:;
      self->memoryWriteFail = 1;
      return;
  }
}

static char compareMemoryObjectWithId(void * _id, void * _memoryObj)
{
  unsigned long id = *(unsigned long *)_id;
  MemoryObject * memoryObj = _memoryObj;

  if(memoryObj->id == id) return 1;
  return 0;
}

unsigned char * findModuleConfigInBuffer(Context * self, ConfigHeaders type, unsigned int * bufferLen)
{
  if(self->configBufer == NULL || self->configBufferLen == 0) return NULL;

  unsigned int * intPtr = (unsigned int *)self->configBufer;
  unsigned char * charPtr = self->configBufer;
  unsigned int header;
  int skip;

  while((unsigned char *)intPtr < (self->configBufer + self->configBufferLen))
  {
    header = *intPtr;

    if(header == type)
    {
      intPtr++;
      *bufferLen = *intPtr;
      intPtr++;
      return (unsigned char *)intPtr;
    }

    intPtr++;
    skip = *intPtr;
    charPtr += skip + TOTAL_CONFIG_HEADER_SIZE;
    intPtr = (unsigned int *)charPtr;
  }
}

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "ConfigUtils.h"
#include "EventSubscription.h"
#include "CMVirtualOut.h"

///////////////
//  DEFINES  //
///////////////

#define TOTAL_CONFIG_HEADER_SIZE  8

/////////////
//  TYPES  //
/////////////

typedef enum MemoryType
{
  MemoryTypes_U1,
  MemoryTypes_U2,
  MemoryTypes_U4,
  MemoryTypes_U8,
  MemoryTypes_STRING,

  MemoryTypes_Count,
} MemoryType;

typedef struct Context
{
  int padWidth;
  int padHeight;
  int deltaX;
  int deltaY;

  char contextIsActive;
  char contextIsLoaded;
  unsigned char * configBufer;
  unsigned int configBufferLen;

  SubscriptionChain * input_padSubscriptionChain;
  SubscriptionChain * input_buttonSubscriptionChain;
  SubscriptionChain * input_sliderSubscriptionChain;
  SubscriptionChain * input_knobSubscriptionChain;

  SubscriptionChain * output_textSubscriptionChain;
  SubscriptionChain * output_padSubscriptionChain;
  SubscriptionChain * output_buttonSubscriptionChain;
  SubscriptionChain * output_padButtonSubscriptionChain;

  List * memory;
  unsigned long memoryObjIdCounter;
  char memoryLookupFail;
  char memoryWriteFail;
  char memoryAllocateFail;

  // MODULES

  struct CMVirtualOutput * virtualOutput;

} Context;

typedef struct ContextInputHandleWrapper
{
  Context * self;
  void * args;
} ContextInputHandleWrapper;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

char context_initContext(Context * self);
void context_freeContext(void * context);
void context_runContext(Context * self);
void context_setToActive(Context * self);
void context_setToInactive(Context * self);

int context_getPushWidth(Context * self);
int context_getPushHeight(Context * self);
int context_getDeltaX(Context * self);
int context_getDeltaY(Context * self);

char context_loadContextConfig(Context * self, char * fileName);
char context_loadContextBinary(Context * self, unsigned char * buffer, unsigned long length);
char context_exportContextConfig(Context * self, char * fileName);

char context_subscribeToPadInputs(Context * self, void * subscriber, EventHandle handle);
void context_unsubscribeToPadInputs(Context * self, void * subscriber);
char context_subscribeToKnobInputs(Context * self, void * subscriber, EventHandle handle);
void context_unsubscribeToKnobInputs(Context * self, void * subscriber);
char context_subscribeToSliderInputs(Context * self, void * subscriber, EventHandle handle);
void context_unsubscribeToSliderInputs(Context * self, void * subscriber);
char context_subscribeToButtonInputs(Context * self, void * subscriber, EventHandle handle);
void context_unsubscribeToButtonInputs(Context * self, void * subscriber);

char context_subscribeToTextOutputs(Context * self, void * subscriber, EventHandle handle);
void context_unsubscribeToTextOutputs(Context * self, void * subscriber);
char context_subscribeToPadOutputs(Context * self, void * subscriber, EventHandle handle);
void context_unsubscribeToPadOutputs(Context * self, void * subscriber);
char context_subscribeToButtonOutputs(Context * self, void * subscriber, EventHandle handle);
void context_unsubscribeToButtonOutputs(Context * self, void * subscriber);
char context_subscribeToPadButtonOutputs(Context * self, void * subscriber, EventHandle handle);
void context_unsubscribeToPadButtonOutputs(Context * self, void * subscriber);

char contextMem_memoryLookupFail(Context * self);
char contextMem_memoryWriteFail(Context * self);
char contextMem_memoryAllocateFail(Context * self);
unsigned char contextMem_getU1(Context * self, unsigned long id);
unsigned short contextMem_getU2(Context * self, unsigned long id);
unsigned int contextMem_getU4(Context * self, unsigned long id);
unsigned long contextMem_getU8(Context * self, unsigned long id);
unsigned char * contextMem_getUSTRING(Context * self, unsigned long id);
char contextMem_setU1(Context * self, unsigned char val, unsigned long id);
char contextMem_setU2(Context * self, unsigned short val, unsigned long id);
char contextMem_setU4(Context * self, unsigned int val, unsigned long id);
char contextMem_setU8(Context * self, unsigned long val, unsigned long id);
char contextMem_setUSTRING(Context * self, unsigned char * val, unsigned long id);
unsigned long contextMem_createMemoryObject(Context * self, MemoryType type);
void contextMem_freeMemory(Context * self, unsigned long id);
MemoryType contextMem_getMemoryType(Context * self, unsigned long id);

unsigned char * findModuleConfigInBuffer(Context * self, ConfigHeaders type, unsigned int * bufferLen);
#endif

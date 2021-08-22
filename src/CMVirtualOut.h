#ifndef CM_VIRTUAL_OUT_H_
#define CM_VIRTUAL_OUT_H_

#include "Context.h"
#include "ConfigUtils.h"
#include "List.h"
#include "PushStateObjects.h"

/////////////
//  TYPES  //
/////////////

typedef struct VirtualOutputLayer
{
  int id;
  unsigned char * colors;
  unsigned char * blinks;
} VirtualOutputLayer;

typedef struct CMVirtualOutput
{
  SubscriptionChain * output_textSubscriptionChain;
  SubscriptionChain * output_padSubscriptionChain;
  SubscriptionChain * output_buttonSubscriptionChain;
  SubscriptionChain * output_padButtonSubscriptionChain;

  List * layerList;
  PushStateObject realPushState;
  PushStateObject workingPushState;

  struct Context * context;
} CMVirtualOutput;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

char cmVirtualOutputInit(struct Context * context);
void cmVirtualOutputFree(struct Context * context);

char cmVirtualOutput_addLayer(struct Context * context, int id);
void cmVirtualOutput_removeLayer(struct Context * context, int id);

void cmVirtualOutput_setTextState(struct Context * context);
void cmVirtualOutput_setPadState(struct Context * context);
void cmVirtualOutput_setButtonState(struct Context * context);
void cmVirtualOutput_setPadButtonState(struct Context * context);

char cmVirtualOutput_subscribeToTextOutputs(struct Context * context, void * subscriber, EventHandle handle);
void cmVirtualOutput_unsubscribeToTextOutputs(struct Context * context, void * subscriber);
char cmVirtualOutput_subscribeToPadOutputs(struct Context * context, void * subscriber, EventHandle handle);
void cmVirtualOutput_unsubscribeToPadOutputs(struct Context * context, void * subscriber);
char cmVirtualOutput_subscribeToButtonOutputs(struct Context * context, void * subscriber, EventHandle handle);
void cmVirtualOutput_unsubscribeToButtonOutputs(struct Context * context, void * subscriber);
char cmVirtualOutput_subscribeToPadButtonOutputs(struct Context * context, void * subscriber, EventHandle handle);
void cmVirtualOutput_unsubscribeToPadButtonOutputs(struct Context * context, void * subscriber);

char cmVirtualOutput_readConfig(struct Context * context);
unsigned int cmVirtualOutput_writeConfig(struct Context * context, unsigned char * buffer);
#endif

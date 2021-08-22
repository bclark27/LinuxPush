#ifndef SYSTEM_MANAGER_H_
#define SYSTEM_MANAGER_H_

#include "EventSubscription.h"

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

char systemManagerInit();
void destroySystemManager();
void systemLoop();

char SystemManager_subscribeToPadInputs(void * subscriber, EventHandle handle);
void SystemManager_unsubscribeToPadInputs(void * subscriber);
char SystemManager_subscribeToKnobInputs(void * subscriber, EventHandle handle);
void SystemManager_unsubscribeToKnobInputs(void * subscriber);
char SystemManager_subscribeToSliderInputs(void * subscriber, EventHandle handle);
void SystemManager_unsubscribeToSliderInputs(void * subscriber);
char SystemManager_subscribeToButtonInputs(void * subscriber, EventHandle handle);
void SystemManager_unsubscribeToButtonInputs(void * subscriber);

char SystemManager_subscribeToTextOutputs(void * subscriber, EventHandle handle);
void SystemManager_unsubscribeToTextOutputs(void * subscriber);
char SystemManager_subscribeToPadOutputs(void * subscriber, EventHandle handle);
void SystemManager_unsubscribeToPadOutputs(void * subscriber);
char SystemManager_subscribeToButtonOutputs(void * subscriber, EventHandle handle);
void SystemManager_unsubscribeToButtonOutputs(void * subscriber);
char SystemManager_subscribeToPadButtonOutputs(void * subscriber, EventHandle handle);
void SystemManager_unsubscribeToPadButtonOutputs(void * subscriber);

#endif

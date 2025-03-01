#ifndef PUSH_MANAGER_H_
#define PUSH_MANAGER_H_

#include "PushEventManager.h"
#include "OutputMessageBuilder.h"

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

void PushManager_Init();
void PushManager_Free();
void PushManager_InitServerOnPort(unsigned short port);
void PushManager_FreeServer();

void PushManager_Cycle();

#endif
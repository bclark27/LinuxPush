#include <stdio.h>
#include <stdlib.h>
#include "PushUsbDriver.h"
#include "SystemManager.h"
#include "PushEventManager.h"
#include "PhysicalPushInputState.h"
#include "PhysicalPushOutputState.h"

#define PUSH_INTERFACE 1
#define PUSH_PROD_ID 21
#define PUSH_VEND_ID 2536
#define msleep(x) usleep(x * 1000)

////////////////////
//  PRIVATE VARS  //
////////////////////

void initAll()
{
  char pass = 0;
  switch(pass)
  {
    case 0:
      if(pushEventManagerInit()) { pass++; }
      else { break; }
    case 100:
      if(physicalPushStateInit()) { pass++; }
      else { break; }
    case 101:
      if(systemManagerInit()) { pass++; }
      else { break; }
    case 102:
      if(pushOutputStateInit()) { pass++; }
      else { break; }
    default:
    break;
  }

  if(pass == 4) //all 4 of the inits pass
  {
    return;
  }

  printf("Init fail. attemting to exit\n");

  switch(pass)
  {
    case 3:
    freePushOutputState();
    case 2:
    destroySystemManager();
    case 1:
    freephysicalPushModel();
    case 0:
    freePushEventManager();
    default:
    break;
  }

  exit(1);
}

int main()
{
  libusb_context *context = NULL;
  libusb_init(&context);
  pushUsbDevice_init(PUSH_INTERFACE, PUSH_PROD_ID, PUSH_VEND_ID, context);

  initAll();

  systemLoop();

  freePushOutputState();
  destroySystemManager();
  freephysicalPushModel();
  freePushEventManager();

  freeUsb();
}

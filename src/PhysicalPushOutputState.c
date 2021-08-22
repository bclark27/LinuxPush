#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EventSubscription.h"
#include "LightAndTextStates.h"
#include "PushUsbDriver.h"
#include "MemWatching.h"
#include "Context.h"
#include "SystemManager.h"
#include "PushStateObjects.h"

#include "PhysicalPushOutputState.h"

///////////////
//  DEFINES  //
///////////////

#define OUTPUT_BUFFER_SIZE 8192
#define TEXT_PACKET_SIZE 108
#define textLine(stateObject, lineNumber) (((unsigned char *)(&(stateObject->text))) + (lineNumber * TEXT_LINE_SIZE))

/////////////
//  TYPES  //
/////////////

typedef struct PushOutputStateManager
{
  PushStateObject realPushState;
  PushStateObject workingPushState;
  unsigned char outputSignal[OUTPUT_BUFFER_SIZE];
  unsigned int outputSignalSize;
} PushOutputStateManager;

//////////////////////////////
//  FUNCTION DECELERATIONS  //
//////////////////////////////

static void handleContextTextUpdates(void * subscriber, void * args);
static void handleContextPadUpdates(void * subscriber, void * args);
static void handleContextButtonUpdates(void * subscriber, void * args);
static void handleContextPadButtonUpdates(void * subscriber, void * args);

static void buildUpdate(unsigned char forceFullUpdate);
static void sendUpdate();

////////////////////
//  PRIVATE VARS  //
////////////////////

static PushOutputStateManager instance;
static PushOutputStateManager * self = &instance;

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

char pushOutputStateInit()
{
  memset(self, 0, sizeof(PushOutputStateManager));
  memset(self->outputSignal, 0, OUTPUT_BUFFER_SIZE);
  self->outputSignalSize = 0;

  char pass = 1;
  pass &= initPushStateObject(&self->realPushState);
  pass &= initPushStateObject(&self->workingPushState);

  pass &= SystemManager_subscribeToTextOutputs(self, handleContextTextUpdates);
  pass &= SystemManager_subscribeToPadOutputs(self, handleContextPadUpdates);
  pass &= SystemManager_subscribeToButtonOutputs(self, handleContextButtonUpdates);
  pass &= SystemManager_subscribeToPadButtonOutputs(self, handleContextPadButtonUpdates);

  pushOutputState_clearState();
  return 1;
}

void freePushOutputState()
{
  SystemManager_unsubscribeToTextOutputs(self);
  SystemManager_unsubscribeToPadOutputs(self);
  SystemManager_unsubscribeToButtonOutputs(self);
  SystemManager_unsubscribeToPadButtonOutputs(self);
}

void pushOutputState_clearState()
{
  initPushStateObject(&self->workingPushState);
  buildUpdate(1);
  sendUpdate();
}

void pushOutputState_updatePush()
{
  buildUpdate(0);
  sendUpdate();
}

void pushOutputState_setPadState(int x, int y, unsigned char color, unsigned char blinkState)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    self->workingPushState.padStates[x + (y * 8)].color = color;
    self->workingPushState.padStates[x + (y * 8)].blinkState = blinkState;
  }
}

void pushOutputState_setPadColor(int x, int y, unsigned char color)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    self->workingPushState.padStates[x + (y * 8)].color = color;
  }
}

void pushOutputState_setPadBlink(int x, int y, unsigned char blinkState)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    self->workingPushState.padStates[x + (y * 8)].blinkState = blinkState;
  }
}

void pushOutputState_setButtonBlink(unsigned char id, unsigned char blinkState)
{
  if(id > 0 && id < 120)
  {
    self->workingPushState.buttonStates[id].blinkState = blinkState;
  }
}

void pushOutputState_setButtonPadState(unsigned char id, unsigned char color, unsigned char blinkState)
{
  if(id > 0 && id < 16)
  {
    self->workingPushState.buttonPadStates[id].color = color;
    self->workingPushState.buttonPadStates[id].blinkState = blinkState;
  }
}

void pushOutputState_setButtonPadColor(unsigned char id, unsigned char color)
{
  if(id > 0 && id < 16)
  {
    self->workingPushState.buttonPadStates[id].color = color;
  }
}

void pushOutputState_setButtonPadBlink(unsigned char id, unsigned char blinkState)
{
  if(id > 0 && id < 16)
  {
    self->workingPushState.buttonPadStates[id].blinkState = blinkState;
  }
}

void pushOutputState_setText(int x, int y, char * text, unsigned int length)
{
  if(x >= 0 && x < 68 && y >= 0 && y < 4)
  {
    memcpy(textLine((&self->workingPushState), y) + x, text, x + length <= 68 ? length : 68 - x);
  }
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

static void handleContextTextUpdates(void * subscriber, void * args)
{
  pushOutputState_TextPacket * pkt = args;
  pushOutputState_setText(pkt->x, pkt->y, pkt->text, pkt->length);
}

static void handleContextPadUpdates(void * subscriber, void * args)
{
  pushOutputState_PadPacket * pkt = args;
  pushOutputState_setPadState(pkt->x, pkt->y, pkt->color, pkt->blinkState);
}

static void handleContextButtonUpdates(void * subscriber, void * args)
{
  pushOutputState_ButtonPacket * pkt = args;
  pushOutputState_setButtonBlink(pkt->id, pkt->blinkState);
}

static void handleContextPadButtonUpdates(void * subscriber, void * args)
{
  pushOutputState_PadButtonPacket * pkt = args;
  pushOutputState_setButtonPadState(pkt->id, pkt->color, pkt->blinkState);
}

static void buildUpdate(unsigned char forceFullUpdate)
{

  //get all pad packets in the buffer
  unsigned int padUpdatePacket;
  for(int i = 0; i < 64; i++)
  {
    if(self->workingPushState.padStates[i].color == ColorStates_CLEAR) self->workingPushState.padStates[i].color = ColorStates_BLACK;

    if( self->workingPushState.padStates[i].color != self->realPushState.padStates[i].color ||
        self->workingPushState.padStates[i].blinkState != self->realPushState.padStates[i].blinkState ||
        self->workingPushState.padStates[i].status != self->realPushState.padStates[i].status ||
        forceFullUpdate)
    {
      padUpdatePacket = 0;
      padUpdatePacket ^= ((unsigned int)self->workingPushState.padStates[i].color) << 24;//color in byte 3
      padUpdatePacket ^= (i + 0x24) << 16;//pad id in byte 2
      padUpdatePacket ^= ((unsigned int)self->workingPushState.padStates[i].blinkState) << 8;
      padUpdatePacket ^= ((unsigned int)self->workingPushState.padStates[i].status);//status in byte 0 always value 0x69

      if(self->outputSignalSize + 4 > OUTPUT_BUFFER_SIZE) sendUpdate(); //send update erly if there will be an overflow

      *(unsigned int*)(self->outputSignal + self->outputSignalSize) = padUpdatePacket;
      self->outputSignalSize += 4; //add 4 for an int

      //update the curr push data
      memcpy(&(self->realPushState.padStates[i]), &(self->workingPushState.padStates[i]), sizeof(outputPadState));
    }
  }

  //put the text into the buffer as needed
  for(int line = 0; line < 4; line++)
  {
    if( memcmp(textLine((&self->workingPushState), line), textLine((&self->realPushState), line), TEXT_LINE_SIZE) ||
        forceFullUpdate)
    {
      if(self->outputSignalSize + TEXT_PACKET_SIZE > OUTPUT_BUFFER_SIZE) sendUpdate(); //send update erly if there will be an overflow

      *(self->outputSignal+(self->outputSignalSize++)) = 0x04;
      *(self->outputSignal+(self->outputSignalSize++)) = 0xf0;
      *(self->outputSignal+(self->outputSignalSize++)) = 0x47;
      *(self->outputSignal+(self->outputSignalSize++)) = 0x7f;

      *(self->outputSignal+(self->outputSignalSize++)) = 0x04;
      *(self->outputSignal+(self->outputSignalSize++)) = 0x15;
      *(self->outputSignal+(self->outputSignalSize++)) = 0x18 + line;//line number + 24
      *(self->outputSignal+(self->outputSignalSize++)) = 0x00;

      *(self->outputSignal+(self->outputSignalSize++)) = 0x04;
      *(self->outputSignal+(self->outputSignalSize++)) = 69;//size + 1
      *(self->outputSignal+(self->outputSignalSize++)) = 0x00;//start x index
      *(self->outputSignal+(self->outputSignalSize++)) = textLine((&self->workingPushState), line)[0];

      //at the begining of each int is a 0x04 then some text bytes
      for(int k = 0; k < 22; k++)
      {
        *(self->outputSignal+(self->outputSignalSize++)) = 0x04;
        *(self->outputSignal+(self->outputSignalSize++)) = textLine((&self->workingPushState), line)[1 + (k * 3)];
        *(self->outputSignal+(self->outputSignalSize++)) = textLine((&self->workingPushState), line)[2 + (k * 3)];
        *(self->outputSignal+(self->outputSignalSize++)) = textLine((&self->workingPushState), line)[3 + (k * 3)];
      }

      //last int is prefixed with 0x06 then the last char
      *(self->outputSignal+(self->outputSignalSize++)) = 0x06;
      *(self->outputSignal+(self->outputSignalSize++)) = textLine((&self->workingPushState), line)[67];
      *(self->outputSignal+(self->outputSignalSize++)) = 0xf7;
      *(self->outputSignal+(self->outputSignalSize++)) = 0x20;

      //copy working buffer to the real state
      memcpy(textLine((&self->realPushState), line), textLine((&self->workingPushState), line), TEXT_LINE_SIZE);
    }
  }
}

static void sendUpdate()
{
  if(self->outputSignalSize)
  {
    int sentAmount = send_data(self->outputSignal, self->outputSignalSize);
    if(sentAmount == self->outputSignalSize)
    {
      self->outputSignalSize = 0;
      return;
    }
    else
    {
      int leftOver = self->outputSignalSize - sentAmount;
      memcpy(self->outputSignal, self->outputSignal + sentAmount, leftOver);
      printf("Could not send full update. Size was %i, but only sent %i\n", self->outputSignalSize, sentAmount);
      self->outputSignalSize = leftOver;
    }
  }
}

#include <stdio.h>
#include <string.h>
#include "PushUsbDriver.h"

#include "OutputMessageBuilder.h"

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
  pushStateObject realPushState;
  pushStateObject workingPushState;
  unsigned char outputSignal[OUTPUT_BUFFER_SIZE];
  unsigned int outputSignalSize;
} PushOutputStateManager;

//////////////////////////////
//  FUNCTION DECELERATIONS  //
//////////////////////////////

bool btnIdIsBtnPad(unsigned char btnId);
unsigned char btnIdToBtnPadIdx(unsigned char btnId);
unsigned char btnPadIdxToBtnId(unsigned char btnPadIdx);

static void buildUpdate(unsigned char forceFullUpdate);
static void sendUpdate(void);
unsigned char rgbToPushColor(float r, float g, float b);

////////////////////
//  PRIVATE VARS  //
////////////////////

static PushOutputStateManager instance;
static PushOutputStateManager * self = &instance;

////////////////////////
//  PUBLIC FUNCTIONS  //
////////////////////////

char outputMessageBuilder_init()
{
  memset(self, 0, sizeof(PushOutputStateManager));
  memset(self->outputSignal, 0, OUTPUT_BUFFER_SIZE);
  self->outputSignalSize = 0;

  PushStates_initStateObj(&self->realPushState);
  PushStates_initStateObj(&self->workingPushState);

  return 1;
}

void outputMessageBuilder_free()
{

}

unsigned char outputMessageBuilder_rgbToColor(unsigned char rgb[])
{
  float rf = rgb[0] / (float)255;
  float gf = rgb[1] / (float)255;
  float bf = rgb[2] / (float)255;
  return rgbToPushColor(rf, gf, bf);
}

void outputMessageBuilder_matchStateObj(pushStateObject* ps)
{
  memcpy(&self->workingPushState, ps, sizeof(pushStateObject));
}

void outputMessageBuilder_clearState()
{
  PushStates_initStateObj(&self->workingPushState);
  buildUpdate(1);
  sendUpdate();
}

void outputMessageBuilder_updatePush()
{
  buildUpdate(0);
  sendUpdate();
}

void outputMessageBuilder_setPadState(int x, int y, unsigned char color, unsigned char blinkState)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    self->workingPushState.padStates[x + (y * 8)].color = color;
    self->workingPushState.padStates[x + (y * 8)].blinkState = blinkState;
  }
}

void outputMessageBuilder_setPadColor(int x, int y, unsigned char color)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    self->workingPushState.padStates[x + (y * 8)].color = color;
  }
}

void outputMessageBuilder_setPadBlink(int x, int y, unsigned char blinkState)
{
  if(x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    self->workingPushState.padStates[x + (y * 8)].blinkState = blinkState;
  }
}

void outputMessageBuilder_setButtonBlink(unsigned char id, unsigned char blinkState)
{
  if(id > 0 && id < 120)
  {
    self->workingPushState.buttonStates[id].blinkState = blinkState;
  }
}

void outputMessageBuilder_setButtonPadState(unsigned char id, unsigned char color, unsigned char blinkState)
{
  if(btnIdIsBtnPad(id))
  {
    unsigned char idx = btnIdToBtnPadIdx(id);
    self->workingPushState.buttonPadStates[idx].color = color;
    self->workingPushState.buttonPadStates[idx].blinkState = blinkState;
  }
}

void outputMessageBuilder_setButtonPadColor(unsigned char id, unsigned char color)
{
  if(btnIdIsBtnPad(id))
  {
    unsigned char idx = btnIdToBtnPadIdx(id);
    self->workingPushState.buttonPadStates[idx].color = color;
  }
}

void outputMessageBuilder_setButtonPadBlink(unsigned char id, unsigned char blinkState)
{
  if(btnIdIsBtnPad(id))
  {
    unsigned char idx = btnIdToBtnPadIdx(id);
    self->workingPushState.buttonPadStates[idx].blinkState = blinkState;
  }
}

void outputMessageBuilder_setText(int x, int y, char * text, unsigned int length)
{
  if(x >= 0 && x < 68 && y >= 0 && y < 4)
  {
    memcpy(textLine((&self->workingPushState), y) + x, text, x + length <= 68 ? length : 68 - x);
  }
}

/////////////////////////
//  PRIVATE FUNCTIONS  //
/////////////////////////

bool btnIdIsBtnPad(unsigned char btnId)
{
  return (btnId >= 20 && btnId <= 27) || (btnId >= 102 && btnId <= 109);
}

unsigned char btnIdToBtnPadIdx(unsigned char btnId)
{
  if (btnId >= 20 && btnId <= 27)
    return btnId - 20;

  if (btnId >= 102 && btnId <= 109)
    return (btnId - 102) + 8;

  return 0;
}

unsigned char btnPadIdxToBtnId(unsigned char btnPadIdx)
{
  if (btnPadIdx < 8)
    return btnPadIdx + 20;

  if (btnPadIdx < 16)
    return (btnPadIdx - 8) + 102;
  
  return 0;
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
      padUpdatePacket |= ((unsigned int)self->workingPushState.padStates[i].color) << 24;//color in byte 3
      padUpdatePacket |= (i + 0x24) << 16;//pad id in byte 2
      padUpdatePacket |= ((unsigned int)self->workingPushState.padStates[i].blinkState) << 8;
      padUpdatePacket |= ((unsigned int)self->workingPushState.padStates[i].status);//status in byte 0 always value 0x69

      if(self->outputSignalSize + 4 > OUTPUT_BUFFER_SIZE) sendUpdate(); //send update erly if there will be an overflow

      *(unsigned int*)(self->outputSignal + self->outputSignalSize) = padUpdatePacket;
      self->outputSignalSize += 4; //add 4 for an int

      //update the curr push data
      memcpy(&(self->realPushState.padStates[i]), &(self->workingPushState.padStates[i]), sizeof(outputPadState));
    }
  }

  for (int i = 0; i < 120; i++)
  {
    if (btnIdIsBtnPad(i))
      continue;

    if (self->workingPushState.buttonStates[i].blinkState != self->realPushState.buttonStates[i].blinkState)
    {

      if(self->outputSignalSize + 4 > OUTPUT_BUFFER_SIZE) sendUpdate(); //send update erly if there will be an overflow

      *(self->outputSignal+(self->outputSignalSize++)) = 0x1b;
      *(self->outputSignal+(self->outputSignalSize++)) = 0xb0;
      *(self->outputSignal+(self->outputSignalSize++)) = self->workingPushState.buttonStates[i].id;
      *(self->outputSignal+(self->outputSignalSize++)) = self->workingPushState.buttonStates[i].blinkState;
      self->realPushState.buttonStates[i].blinkState = self->workingPushState.buttonStates[i].blinkState;
    }
  }

  for (int i = 0; i < 16; i++)
  {
    unsigned char btnId = btnPadIdxToBtnId(i);

    if (self->workingPushState.buttonPadStates[i].blinkState != self->realPushState.buttonPadStates[i].blinkState ||
        self->workingPushState.buttonPadStates[i].color != self->realPushState.buttonPadStates[i].color) 
    {
      if(self->outputSignalSize + 4 > OUTPUT_BUFFER_SIZE) sendUpdate(); //send update erly if there will be an overflow

      *(self->outputSignal+(self->outputSignalSize++)) = 0x1b;
      *(self->outputSignal+(self->outputSignalSize++)) = self->workingPushState.buttonPadStates[i].blinkState;
      *(self->outputSignal+(self->outputSignalSize++)) = btnId;
      *(self->outputSignal+(self->outputSignalSize++)) = self->workingPushState.buttonPadStates[i].color;

      self->realPushState.buttonPadStates[i].color = self->workingPushState.buttonPadStates[i].color;
      self->realPushState.buttonPadStates[i].blinkState = self->workingPushState.buttonPadStates[i].blinkState;
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
    int sentAmount = PushUsbDriver_send_data(self->outputSignal, self->outputSignalSize);
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


unsigned char hsbToPushColor(float h, float s, float b)
{
  /*
  Convert a HSB to a Push Color
  param h: Hue from 0-1
  param s: Saturation from 0-1
  param b: Brightness from 0-1
  return: Push color that can be passed to pad_color functions
  */

  if(s < 0.2)
  {  //grayscale
    return (unsigned char)(int)(4 * b);
  }
  h += (float)1 / (float)28;
  if(h > 1)
  {
    h -= 1;
  }
  unsigned char color = (int)(h * 14) * 4 + 4;
  if(b < 0.5)
  {
    color += 2;
    if(s < 0.5)
    {
      color += 1;
    }
  }
  else if(s >= 0.5)
  {
    color += 1;
  }
  return color;
}

void rgbToHsb(float r, float g, float b, float *h, float *s, float *br) 
{
    float max, min, delta;

    // Find the maximum and minimum values among r, g, b.
    max = r;
    if (g > max) max = g;
    if (b > max) max = b;

    min = r;
    if (g < min) min = g;
    if (b < min) min = b;

    // Set brightness to the maximum value.
    *br = max;

    // Calculate the difference between the max and min values.
    delta = max - min;

    // Calculate saturation. If max is 0, the color is black.
    if (max != 0)
        *s = delta / max;
    else {
        *s = 0;
        *h = 0;  // Hue is undefined, conventionally set to 0.
        return;
    }

    // Calculate hue based on which color is the maximum.
    if (delta == 0)
        *h = 0;  // If there is no difference, hue is undefined.
    else if (r == max)
        *h = (g - b) / delta;
    else if (g == max)
        *h = 2.0f + (b - r) / delta;
    else  // b == max
        *h = 4.0f + (r - g) / delta;

    // Convert hue to degrees on the circle (0-360).
    *h *= 60;
    if (*h < 0)
        *h += 360;
}

unsigned char rgbToPushColor(float r, float g, float b)
{
  float h, s, br;
  rgbToHsb(r,g,b,&h,&s,&br);
  return hsbToPushColor(h,s,br);
}

#ifndef OUTPUT_MESSAGE_BUILDER_H_
#define OUTPUT_MESSAGE_BUILDER_H_

#include "comm/IPC.h"

#define TEXT_BUFFER_SIZE 272

/////////////
//  TYPES  //
/////////////

typedef struct outputPadState
{
  unsigned int id;
  unsigned int x;
  unsigned int y;
  unsigned char color;
  unsigned char blinkState;
  unsigned char status;
} outputPadState;

typedef struct outputButtonState
{
  unsigned char id;
  unsigned char blinkState;
} outputButtonState;

typedef struct outputButtonPadState
{
  unsigned char id;
  unsigned char color;
  unsigned char blinkState;
} outputButtonPadState;

typedef struct pushStateObject
{
  outputPadState padStates[64];
  outputButtonState buttonStates[120];
  outputButtonPadState buttonPadStates[16];
  unsigned char text[TEXT_BUFFER_SIZE];
} pushStateObject;


////////////////////////////
//  FUNCTION DEFINITIONS  //
////////////////////////////

char outputMessageBuilder_init();
void outputMessageBuilder_free();
void outputMessageBuilder_initStateObj();
unsigned char outputMessageBuilder_rgbToColor(unsigned char rgb[]);
void outputMessageBuilder_matchStateObj(pushStateObject* ps);
void outputMessageBuilder_clearState();
void outputMessageBuilder_updatePush();
void outputMessageBuilder_setPadState(int x, int y, unsigned char color, unsigned char blinkState);
void outputMessageBuilder_setPadColor(int x, int y, unsigned char color);
void outputMessageBuilder_setPadBlink(int x, int y, unsigned char blinkState);
void outputMessageBuilder_setButtonBlink(unsigned char id, unsigned char blinkState);
void outputMessageBuilder_setButtonPadState(unsigned char id, unsigned char color, unsigned char blinkState);
void outputMessageBuilder_setButtonPadColor(unsigned char id, unsigned char color);
void outputMessageBuilder_setButtonPadBlink(unsigned char id, unsigned char blinkState);
void outputMessageBuilder_setText(int x, int y, char * text, unsigned int length);

#endif

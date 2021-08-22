#ifndef PUSH_STATE_OBJECTS_H_
#define PUSH_STATE_OBJECTS_H_

///////////////
//  DEFINES  //
///////////////

#define TEXT_BUFFER_SIZE 272
#define TEXT_LINE_SIZE 68
#define textLine(stateObject, lineNumber) (((unsigned char *)(&(stateObject->text))) + (lineNumber * TEXT_LINE_SIZE))


/////////////
//  TYPES  //
/////////////

typedef struct pushOutputState_TextPacket
{
  int x;
  int y;
  char * text;
  unsigned int length;
} pushOutputState_TextPacket;

typedef struct pushOutputState_PadPacket
{
  int x;
  int y;
  unsigned char color;
  unsigned char blinkState;
} pushOutputState_PadPacket;

typedef struct pushOutputState_ButtonPacket
{
  int id;
  unsigned char blinkState;
} pushOutputState_ButtonPacket;

typedef struct pushOutputState_PadButtonPacket
{
  unsigned char id;
  unsigned char color;
  unsigned char blinkState;
} pushOutputState_PadButtonPacket;

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

typedef struct PushStateObject
{
  outputPadState padStates[64];
  outputButtonState buttonStates[120];
  outputButtonPadState buttonPadStates[16];
  unsigned char text[TEXT_BUFFER_SIZE];
} PushStateObject;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

char initPushStateObject(PushStateObject * ps);
#endif

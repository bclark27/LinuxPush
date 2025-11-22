#ifndef PUSH_STATES_H_
#define PUSH_STATES_H_

#define TEXT_BUFFER_SIZE 272
#define TEXT_LINE_SIZE 68

/////////////
//  TYPES  //
/////////////

typedef struct padState
{
  int x;
  int y;
  unsigned char isPressed;
  unsigned char velocity;
} padState;

typedef struct buttonState
{
  unsigned char id;
  unsigned char isPadButton;
  unsigned char isPressed;
} buttonState;

typedef struct knobState
{
  unsigned char id;
  unsigned char isPressed;
  char rotation;
  unsigned char value;
} knobState;

typedef struct theSliderlderState
{
  int value;
  float percent;
  unsigned char isPressed;
} theSliderlderState;

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


/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

void PushStates_initStateObj(pushStateObject* ps);
void PushStates_setPadState(pushStateObject* ps, int x, int y, unsigned char color, unsigned char blinkState);
void PushStates_setPadColor(pushStateObject* ps, int x, int y, unsigned char color);
void PushStates_setPadBlink(pushStateObject* ps, int x, int y, unsigned char blinkState);
void PushStates_setButtonBlink(pushStateObject* ps, unsigned char id, unsigned char blinkState);
void PushStates_setButtonPadState(pushStateObject* ps, unsigned char id, unsigned char color, unsigned char blinkState);
void PushStates_setButtonPadColor(pushStateObject* ps, unsigned char id, unsigned char color);
void PushStates_setButtonPadBlink(pushStateObject* ps, unsigned char id, unsigned char blinkState);
void PushStates_setText(pushStateObject* ps, int x, int y, char * text, unsigned int length);
#endif

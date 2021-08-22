#ifndef PUSH_STATES_H_
#define PUSH_STATES_H_

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

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

#endif

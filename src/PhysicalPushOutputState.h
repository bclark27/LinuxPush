#ifndef PHYSICAL_PUSH_OUTPUT_STATE_H_
#define PHYSICAL_PUSH_OUTPUT_STATE_H_

/////////////
//  TYPES  //
/////////////


////////////////////////////
//  FUNCTION DEFINITIONS  //
////////////////////////////

char pushOutputStateInit();
void freePushOutputState();
void pushOutputState_clearState();
void pushOutputState_updatePush();
void pushOutputState_setPadState(int x, int y, unsigned char color, unsigned char blinkState);
void pushOutputState_setPadColor(int x, int y, unsigned char color);
void pushOutputState_setPadBlink(int x, int y, unsigned char blinkState);
void pushOutputState_setButtonBlink(unsigned char id, unsigned char blinkState);
void pushOutputState_setButtonPadState(unsigned char id, unsigned char color, unsigned char blinkState);
void pushOutputState_setButtonPadColor(unsigned char id, unsigned char color);
void pushOutputState_setButtonPadBlink(unsigned char id, unsigned char blinkState);
void pushOutputState_setText(int x, int y, char * text, unsigned int length);

#endif

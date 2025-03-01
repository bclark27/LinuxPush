#ifndef OUTPUT_MESSAGE_BUILDER_H_
#define OUTPUT_MESSAGE_BUILDER_H_

/////////////
//  TYPES  //
/////////////


////////////////////////////
//  FUNCTION DEFINITIONS  //
////////////////////////////

char outputMessageBuilder_init();
void outputMessageBuilder_free();
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

#ifndef LIGHT_STATES_H_
#define LIGHT_STATES_H_

/////////////
//  TYPES  //
/////////////

enum ColorStates {
  ColorStates_CLEAR = (unsigned char)0xff,
  ColorStates_BLACK = (unsigned char)0,
  ColorStates_DARK_GRAY = (unsigned char)1,
  ColorStates_LIGHT_GRAY = (unsigned char)2,
  ColorStates_WHITE = (unsigned char)3,
  ColorStates_LIGHT_RED = (unsigned char)4,
  ColorStates_RED = (unsigned char)5,
  ColorStates_DARK_RED = (unsigned char)6,
  ColorStates_DIM_RED = (unsigned char)7,
  ColorStates_LIGHT_GREEN = (unsigned char)20,
  ColorStates_GREEN = (unsigned char)21,
  ColorStates_DARK_GREEN = (unsigned char)22,
  ColorStates_DIM_GREEN = (unsigned char)23,

  ColorStates_OutOfBounds = ColorStates_RED,
};

enum BlinkStates {
  BlinkStates_BlinkOff = (unsigned char)0,
  BlinkStates_BlinkShot0 = (unsigned char)1,
  BlinkStates_BlinkShot1 = (unsigned char)2,
  BlinkStates_BlinkShot2 = (unsigned char)3,
  BlinkStates_BlinkShot3 = (unsigned char)4,
  BlinkStates_BlinkShot4 = (unsigned char)5,
  BlinkStates_BlinkPulse0 = (unsigned char)6,
  BlinkStates_BlinkPulse1 = (unsigned char)7,
  BlinkStates_BlinkPulse2 = (unsigned char)8,
  BlinkStates_BlinkPulse3 = (unsigned char)9,
  BlinkStates_BlinkPulse4 = (unsigned char)10,
  BlinkStates_BlinkBlink0 = (unsigned char)11,
  BlinkStates_BlinkBlink1 = (unsigned char)12,
  BlinkStates_BlinkBlink2 = (unsigned char)13,
  BlinkStates_BlinkBlink3 = (unsigned char)14,
  BlinkStates_BlinkBlink4 = (unsigned char)15,

  OUT_OF_BOUNDS_BLINK = BlinkStates_BlinkOff,
};

enum TextCharacters {
    TextCharacters_Space = (unsigned char)0x20,
};

#endif

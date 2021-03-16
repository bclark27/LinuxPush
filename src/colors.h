#ifndef COLORS
#define COLORS
enum COLORS {
  CLEAR = (unsigned char)0xff,
  BLACK = (unsigned char)0,
  DARK_GRAY = (unsigned char)1,
  LIGHT_GRAY = (unsigned char)2,
  WHITE = (unsigned char)3,
  LIGHT_RED = (unsigned char)4,
  RED = (unsigned char)5,
  DARK_RED = (unsigned char)6,
  DIM_RED = (unsigned char)7,
  LIGHT_GREEN = (unsigned char)20,
  GREEN = (unsigned char)21,
  DARK_GREEN = (unsigned char)22,
  DIM_GREEN = (unsigned char)23,

  OUT_OF_BOUNDS_COLOR = RED,
};
#endif

#ifndef BLINK
#define BLINK
enum BLINK {
  BLINK_OFF = (unsigned char)0,
  BLINK_SHOT_0 = (unsigned char)1,
  BLINK_SHOT_1 = (unsigned char)2,
  BLINK_SHOT_2 = (unsigned char)3,
  BLINK_SHOT_3 = (unsigned char)4,
  BLINK_SHOT_4 = (unsigned char)5,
  BLINK_PULSE_0 = (unsigned char)6,
  BLINK_PULSE_1 = (unsigned char)7,
  BLINK_PULSE_2 = (unsigned char)8,
  BLINK_PULSE_3 = (unsigned char)9,
  BLINK_PULSE_4 = (unsigned char)10,
  BLINK_BLINK_0 = (unsigned char)11,
  BLINK_BLINK_1 = (unsigned char)12,
  BLINK_BLINK_2 = (unsigned char)13,
  BLINK_BLINK_3 = (unsigned char)14,
  BLINK_BLINK_4 = (unsigned char)15,

  OUT_OF_BOUNDS_BLINK = BLINK_OFF,
};
#endif

#ifndef HSB_CONVERSION
#define HSB_CONVERSION
unsigned char hsb(float h, float s, float b);
#endif

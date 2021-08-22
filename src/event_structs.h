#ifndef EVENT_TYPE
#define EVENT_TYPE
enum EVENT_TYPE{
  PAD_PRESS_EVENT = 0x19,
  PAD_HOLD_EVENT = 0x1a,
  PAD_RELEASE_EVENT = 0x18,

  BUTTON_PRESS_EVENT = 0x7f,
  BUTTON_RELEASE_EVENT = 0x00,
  BUTTON_TAG = 0x1b,

  KNOB_PRESS_EVENT = 0x7f,
  KNOB_TURN_EVENT = 0x1b,
  KNOB_RELEASE_EVENT = 0x00,
  KNOB_TOUCH_TAG = 0x19,

  SLIDEING_TAG = 0x1e,
  SLIDER_PRESSED_EVENT = 0x7f,
  SLIDER_RELEASE_EVENT = 0x00,
  SLIDER_HOLD_EVENT = 0x1e,
};
#endif

#ifndef EVENT_CLASS
#define EVENT_CLASS
enum EVENT_CLASS{
  PAD_EVENT = 0,
  BUTTON_EVENT = 1,
  KNOB_EVENT = 2,
  SLIDER_EVENT = 3,
};
#endif

#ifndef EVENT
#define EVENT
typedef struct event{
  struct event* next_event;
  struct event* prev_event;

  float slider_percent;
  unsigned int slider_value;
  unsigned int data;
  int knob_delta;
  int pad_x;
  int pad_y;

  unsigned char pad_velocity;
  unsigned char pad_state;
  unsigned char knob_state;
  unsigned char knob_id;
  unsigned char btn_id;
  unsigned char btn_state;
  unsigned char slider_state;
  unsigned char event_class;
}event;
#endif

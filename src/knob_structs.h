#include<stdbool.h>

#ifndef KNOB_STRUCT
#define KNOB_STRUCT
typedef struct knob{
  int refrence_id;
  unsigned int knob_id;
  float value;
  float min;
  float max;
  float step;
  bool loop;
}knob;
#endif

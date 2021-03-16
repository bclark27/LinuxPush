#include"knob_structs.h"
#include"event_structs.h"
#include<stdbool.h>
#include<stdlib.h>

knob* create_knob(int refrence_id, int knob_id, float start_val, float min, float max, float step, bool loop){
  knob* k = (knob*)malloc(sizeof(knob));
  k->refrence_id = refrence_id;
  k->knob_id = knob_id;
  k->value = start_val;
  k->min = min;
  k->max = max;
  k->step = step;
  k->loop = loop;

  return k;
}

void update_knob(knob* k, event* e){
  if(e->event_class != KNOB_EVENT || e->knob_state != KNOB_TURN_EVENT || e->knob_id != k->knob_id){
    return;
  }
  float val = k->step * e->knob_delta;
  k->value += val;

  if(k->loop){
    float range = k->max - k->min;
    if(k->value > k->max){k->value -= range;}
    else if(k->value < k->min){k->value += range;}
  } else {
    if(k->value > k->max){k->value = k->max;}
    else if(k->value < k->min){k->value = k->min;}
  }
}

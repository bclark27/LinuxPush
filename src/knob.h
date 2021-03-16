#include"knob_structs.h"
#include"event_structs.h"

#ifndef CREATE_KNOB
#define CREATE_KNOB
knob* create_knob(int refrence_id, int knob_id, float start_val, float min, float max, float step, bool loop);
#endif

#ifndef UPDATE_KNOB
#define UPDATE_KNOB
void update_knob(knob* k, event* e);
#endif

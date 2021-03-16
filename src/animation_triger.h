#include"animation_triger_structs.h"
#include"event_structs.h"
#include"animation_structs.h"

#ifndef ALL_ANIMATION_TRIGER_FUNCS
#define ALL_ANIMATION_TRIGER_FUNCS
animation_triger* create_animation_triger(int animation_type, int min_x, int max_x, int min_y, int max_y, unsigned char animation_color, int id);
animation* parse_event_triger(animation_triger* at, event* e);
#endif

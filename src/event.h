#include"event_structs.h"
#include"push_structs.h"

#ifndef PRINT_EVENT
#define PRINT_EVENT
void print_event(event* e);
#endif

#ifndef CREATE_EVENT
#define CREATE_EVENT
event* create_event(unsigned int data, push_device* push);
#endif

#ifndef FREE_EVENT
#define FREE_EVENT
void free_event(event* e);
#endif

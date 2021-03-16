#include <libusb-1.0/libusb.h>
#include "push_structs.h"

#ifndef SCROLLING_TEXT_PUSH
#define SCROLLING_TEXT_PUSH
void add_scrolling_text(push_device* push, int id, int x, int y, char* text, int size, float speed);
#endif

#ifndef PLACE_TEXT
#define PLACE_TEXT
void place_text(push_device* push, char* text, int x, int y);
void place_text_knob(push_device* push, char* text, int knob, int y);
void place_text_knob_l(push_device* push, char* text, int knob, int y);
void place_text_knob_r(push_device* push, char* text, int knob, int y);
void clear_text(push_device* push);
#endif

#ifndef KNOB_STUFF
#define KNOB_STUFF
float read_knob(push_device* push, int refrence_id);
void add_knob(push_device* push, int refrence_id, int knob_id, float start_val, float min, float max, float step, bool loop);
#endif

#ifndef ALL_BTN_STUFF
#define ALL_BTN_STUFF
void add_pad_key_push(push_device* push, int key_id, char* keyboard_command, int layer, int pad_x, int pad_y, unsigned char color, int type, bool continues);
void add_btn_key_push(push_device* push, char* keyboard_command, int id);
#endif

#ifndef ALL_EVENT_STUFF
#define ALL_EVENT_STUFF
void process_events(push_device* push);
void receive_update(push_device* push);
#endif

#ifndef LAYER_STUFF_IN_PUSH
#define LAYER_STUFF_IN_PUSH
void new_layer(push_device* push, int layer_num);
void remove_layer(push_device* push, int layer_num);
void add_animation_to_layer(push_device* push, animation* a, int layer_num);
void add_animation_triger_to_push(push_device* push, int animation_type, int min_x, int max_x, int min_y, int max_y, unsigned char animation_color, int layer_num, int id);
animation_triger* get_animation_triger(push_device* push, int layer_num, int id);
#endif

#ifndef LIGHT_PAD_STUFF
#define LIGHT_PAD_STUFF
int add_animation(push_device* push, animation* a, int layer_num);
void light_pad_xy(push_device* push, int layer, unsigned int color, unsigned char blink, int x, int y);
void light_pad_id(push_device* push, int layer, unsigned int color, unsigned char blink, int id);
void set_all_pads(push_device* push, int layer, unsigned int color, unsigned char blink);
void clear_all_backgrounds(push_device* push);
void clear_layer_background(push_device* push, int layer_num);
#endif

#ifndef BUILD_UPDATE
#define BUILD_UPDATE
void build_update(push_device* push);
#endif

#ifndef SEND_UPDATE
#define SEND_UPDATE
void send_update(push_device* push);
#endif

#ifndef GET_UPDATE
#define GET_UPDATE
void get_update(push_device* push);
#endif

#ifndef FORCE_UPDATE
#define FORCE_UPDATE
void update_push(push_device* push);
#endif

#ifndef CREATE_PUSH_DEVICE
#define CREATE_PUSH_DEVICE
push_device* create_push_device(usb_device* dev, int width, int height);
#endif

#ifndef FREE_PUSH
#define FREE_PUSH
void free_push(push_device* push);
#endif

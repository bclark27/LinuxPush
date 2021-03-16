#include"keyboard_structs.h"
#include"push_structs.h"

#ifndef CREAT_KEYBOARD
#define CREAT_KEYBOARD
keyboard* create_keyboard();
#endif

#ifndef CREAT_KEY
#define CREAT_KEY
key* create_key(char* keyboard_command, int key_id, activator* activate, activator* deactivate, int animation_type);
#endif

#ifndef ADD_PAD_KEY
#define ADD_PAD_KEY
void add_pad_key(push_device* push, int id, keyboard* keyboard, char* keyboard_command, int layer, int pad_x, int pad_y, unsigned char color, int type, bool continues);
#endif

#ifndef ADD_BTN_KEY
#define ADD_BTN_KEY
void add_btn_key(push_device* push, int key_id, keyboard* keyboard, char* keyboard_command, int id);
#endif

#ifndef HANDLE_KEY_EVENT
#define HANDLE_KEY_EVENT
void handle_key_event(push_device* push, event* e, keyboard* keyboard);
#endif

#ifndef FREE_KEYBOARD
#define FREE_KEYBOARD
void free_keyboard(keyboard* keyboard);
#endif

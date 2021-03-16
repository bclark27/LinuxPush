#include"text_structs.h"

#ifndef TEXT_FUNCTIONS
#define TEXT_FUNCTIONS
scrolling_text* create_scrolling_text(int id, int x, int y, char* text, int size, float speed);
void update_scrolling_text(scrolling_text* s);




#endif

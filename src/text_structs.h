#include<stdbool.h>

#ifndef SCROLLING_TEXT
#define SCROLLING_TEXT
typedef struct scrolling_text{
  int id;
  int x;
  int y;
  bool active;
  char* text;
  int size;
  float speed;
  unsigned int time;
  unsigned char* display_buf;

  bool dirty;
  int pos;
}scrolling_text;
#endif

#include<stdbool.h>

#ifndef SCROLLING_TEXT
#define SCROLLING_TEXT
typedef struct scrolling_text{
  unsigned char* display_buf;
  char* text;

  float speed;
  unsigned int time;
  int id;
  int x;
  int y;
  int size;
  int pos;

  bool active;
  bool dirty;
}scrolling_text;
#endif

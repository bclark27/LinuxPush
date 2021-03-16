#include"text_structs.h"
#include<stdlib.h>
#include<string.h>

scrolling_text* create_scrolling_text(int id, int x, int y, char* text, int size, float speed){
  scrolling_text* s = (scrolling_text*)malloc(sizeof(scrolling_text));
  s->id = id;
  s->x = x;
  s->y = y;
  s->active = true;
  s->dirty = true;
  s->pos = -1;
  s->text = text;
  s->size = size;
  s->speed = speed;
  s->time = 0;
  s->display_buf = (unsigned char*)malloc(size);
  return s;
}

void update_scrolling_text(scrolling_text* s){
  if(!s->active)return;
  int pos = s->size - ((int)(s->time * s->speed) % s->size);
  if(pos == s->pos){
    s->time++;
    s->dirty = false;
    return;
  }

  int str_size = strlen(s->text);
  for(int i = 0; i < s->size; i++) {
    s->display_buf[i] = 0x20;
  }

  int i;
  for(i = 0; i < str_size && i + pos < s->size; i++){
    s->display_buf[i + pos] = s->text[i];
  }
  if (i < str_size && str_size <= s->size) {
    for(int p = 0; i < str_size && i + pos < s->size; i++, p++){
      s->display_buf[p] = s->text[i];
    }
  }
  s->dirty = true;
  s->time++;
}

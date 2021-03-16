#ifndef ANIMATION_TRIGER
#define ANIMATION_TRIGER
typedef struct animation_triger{
  struct animation_triger* next_animation_triger;
  struct animation_triger* prev_animation_triger;
  int animation_type;
  int max_x;
  int min_x;
  int max_y;
  int min_y;
  unsigned char animation_color;
  int id;
}animation_triger;
#endif

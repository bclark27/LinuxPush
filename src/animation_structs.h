#include <stdbool.h>

#ifndef ANIMATION_TYPES
#define ANIMATION_TYPES
enum ANIMATION_TYPES{
  NO_ANIMATION = 0,
  STILL_LIGHT = 1,
  STAR_CROSS = 2,
  SQUARE = 3,
  PLUS_CROSS = 4,
};
#endif

#ifndef PARTICLE
#define PARTICLE
typedef struct particle{
  struct particle* next_particle;//next partical in the chain
  struct particle* prev_particle;//prev partical in the chain
  bool done;//is done and can be deleted
  unsigned char color;//color
  float x;//partical x
  float y;//partical y
  float vx;//velocity x
  float vy;//velocity y
  int id; //id of the particle
}particle;
#endif


#ifndef ANIMATION
#define ANIMATION
typedef struct animation{
  struct animation* next_animation;//pointer to next animation in the chain
  struct animation* prev_animation;//printer to the previous animation in the chain
  unsigned char color;
  int x;//animation x
  int y;//animation y
  unsigned int time;
  int type;
  struct particle* particle_head;//pointer to the first partical involved
  void (*update_function)(struct animation*);
  bool done;//is done and can be deleted
  int ref_count;//number of times refrenced, if not 0 then it cant be deleted
  bool to_delete;//marked to be deleted as a signal to other refrencing programs to let go of the pointers to this animation
}animation;
#endif

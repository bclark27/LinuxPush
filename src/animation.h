#include"animation_structs.h"

#ifndef ANIMATION_BEHAVIORS
#define ANIMATION_BEHAVIORS
void no_animation(animation* a);
#endif

#ifndef ANIMATION_STUFF
#define ANIMATION_STUFF
animation* create_animation(float x, float y, unsigned char color, int type);
void update_animation(animation* a);
#endif

#ifndef ADD_PARTICLE
#define ADD_PARTICLE
void add_particle(animation* a, particle* p);
#endif

#ifndef CREATE_PARTICLE
#define CREATE_PARTICLE
particle* create_particle(float x, float y, float vx, float vy, unsigned char color, int id);
#endif

#ifndef FREE_ANIMATION
#define FREE_ANIMATION
void free_animation(animation* a);
#endif

#ifndef FREE_PARTICLE
#define FREE_PARTICLE
void free_particle(particle* p);
#endif

#include "animation_structs.h"
#include <stdlib.h>
#include <stdio.h>

void add_particle(animation* a, particle* p){

  if(p == NULL) return;

  if(a->particle_head == NULL){
    a->particle_head = p;
    p->next_particle = NULL;
    p->prev_particle = NULL;
    return;
  }
  particle* ptr = a->particle_head;
  while(ptr->next_particle != NULL){
    ptr = ptr->next_particle;
  }
  ptr->next_particle = p;
  p->prev_particle = ptr;
}

void add_particle_tp_particle(particle* a, particle* p){
  if(p == NULL || a == NULL) return;

  if(a->next_particle == NULL){
    a->next_particle = p;
    p->next_particle = NULL;
    p->prev_particle = a;
    return;
  }
  p->next_particle = a->next_particle;
  p->prev_particle = a;
  a->next_particle->prev_particle = p;
  a->next_particle = p;
}

particle* create_particle(float x, float y, float vx, float vy, unsigned char color, int id){
  particle* p = (particle*)malloc(sizeof(particle));
  p->next_particle = NULL;
  p->prev_particle = NULL;
  p->done = false;
  p->color = color;
  p->x = x;
  p->y = y;
  p->vx = vx;
  p->vy = vy;
  p->id = id;
  return p;
}

void update_animation(animation* a){

  if(a->update_function == NULL){
    return;
  }
  a->update_function(a);
}

void free_particle(particle* p){
  free(p);
}

void remove_particle(animation* a, particle* p){
  if(a->particle_head == NULL || p == NULL){
    return;
  }
  particle* next = p->next_particle;
  particle* prev = p->prev_particle;

  if(next == NULL && prev == NULL){
    a->particle_head = NULL;
  } else if(next == NULL){
    prev->next_particle = NULL;
  } else if(prev == NULL){
    next->prev_particle = NULL;
    a->particle_head = next;
  } else {
    prev->next_particle = next;
    next->prev_particle = prev;
  }
  free_particle(p);
}

void free_animation(animation* a){

  particle* ptr = a->particle_head;

  while(ptr != NULL){
    particle* next = ptr->next_particle;
    free_particle(ptr);
    ptr = next;
  }
  free(a);
}

void still_light(animation* a){
  if(a->time == 0){ //init behavior
    particle* p = create_particle(a->x, a->y, 0, 0, a->color, 0);
    add_particle(a, p);
  } else {  //default behavior

  }
  a->time++;
}

void star_cross(animation* a){

  float vel = 1;
  if(a->time == 0){ //init behavior
    add_particle(a, create_particle(a->x+0.5, a->y+0.5, vel, vel, a->color, 0));
    add_particle(a, create_particle(a->x+0.5, a->y+0.5, vel, -vel, a->color, 1));
    add_particle(a, create_particle(a->x+0.5, a->y+0.5, -vel, vel, a->color, 2));
    add_particle(a, create_particle(a->x+0.5, a->y+0.5, -vel, -vel, a->color, 3));
  } else if(a->time >= 400){  //default behavior
    if(a->particle_head != NULL){
      while(a->particle_head != NULL){
        remove_particle(a, a->particle_head);
      }
      a->particle_head = NULL;
      a->done = true;
    }
  } else {
    if(a->time % 10 == 0){
      particle* p = a->particle_head;
      while(p != NULL){
        p->x += p->vx;
        p->y += p->vy;
        p = p->next_particle;
      }
    }
  }
  a->time++;

}

void square(animation* a){

  float vel = 1;
  if(a->time == 0){ //init behavior
    add_particle(a, create_particle(a->x, a->y, vel, vel, a->color, 0));
    add_particle(a, create_particle(a->x, a->y, vel, -vel, a->color, 1));
    add_particle(a, create_particle(a->x, a->y, -vel, vel, a->color, 2));
    add_particle(a, create_particle(a->x, a->y, -vel, -vel, a->color, 3));
  } else if(a->time >= 500){  //default behavior
    if(a->particle_head != NULL){
      while(a->particle_head != NULL){
        remove_particle(a, a->particle_head);
      }
      a->particle_head = NULL;
      a->done = true;
    }
  } else {
    if(a->time % 10 == 0){
      particle* p1 = a->particle_head;
      particle* p2 = p1->next_particle;
      particle* p3 = p2->next_particle;
      particle* p4 = p3->next_particle;
      particle* p = p1;
      while(p != NULL){
        p->x += p->vx;
        p->y += p->vy;
        p = p->next_particle;
      }

      add_particle_tp_particle(p4, create_particle(p1->x-1, p1->y, 0, vel, a->color, 0));
      add_particle_tp_particle(p4, create_particle(p1->x, p1->y-1, vel, 0, a->color, 0));

      add_particle_tp_particle(p4, create_particle(p2->x, p2->y+1, vel, 0, a->color, 0));
      add_particle_tp_particle(p4, create_particle(p2->x-1, p2->y, 0, -vel, a->color, 0));

      add_particle_tp_particle(p4, create_particle(p3->x+1, p3->y, 0, vel, a->color, 0));
      add_particle_tp_particle(p4, create_particle(p3->x, p3->y-1, -vel, 0, a->color, 0));

      add_particle_tp_particle(p4, create_particle(p4->x, p4->y+1, -vel, 0, a->color, 0));
      add_particle_tp_particle(p4, create_particle(p4->x+1, p4->y, 0, -vel, a->color, 0));
    }
  }
  a->time++;
  /*
  particle* p = a->particle_head;
  int i = 0;
  while(p != NULL){
    i++;
    p = p->next_particle;
  }
  printf("%i\n", i);
  */
  //getchar();
}

void plus_cross(animation* a){

  float vel = 1;
  if(a->time == 0){ //init behavior
    add_particle(a, create_particle(a->x, a->y, 0, vel, a->color, 0));
    add_particle(a, create_particle(a->x, a->y, 0, -vel, a->color, 1));
    add_particle(a, create_particle(a->x, a->y, vel, 0, a->color, 2));
    add_particle(a, create_particle(a->x, a->y, -vel, 0, a->color, 3));
  } else if(a->time >= 400){  //default behavior
    if(a->particle_head != NULL){
      while(a->particle_head != NULL){
        remove_particle(a, a->particle_head);
      }
      a->particle_head = NULL;
      a->done = true;
    }
  } else {
    if(a->time % 10 == 0){
      particle* p = a->particle_head;
      while(p != NULL){
        p->x += p->vx;
        p->y += p->vy;
        p = p->next_particle;
      }
    }
  }
  a->time++;
}

animation* create_animation(float x, float y, unsigned char color, int type){
  animation* a = (animation*)malloc(sizeof(animation));
  a->next_animation = NULL;
  a->prev_animation = NULL;

  a->color = color;
  a->x = x;
  a->y = y;
  a->time = 0;
  a->type = type;
  a->particle_head = NULL;

  //pish the correct animation function
  if(type == STILL_LIGHT){
    a->update_function = &still_light;
  } else if(type == STAR_CROSS){
    a->update_function = &star_cross;
  } else if(type == SQUARE){
    a->update_function = &square;
  } else if(type == PLUS_CROSS){
    a->update_function = &plus_cross;
  } else {
    a->update_function = NULL;
    a->done = true;
  }

  a->done = false;
  a->ref_count = 0;
  a->to_delete = false;
  return a;
}

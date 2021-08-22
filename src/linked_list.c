#include"linked_list_structs.h"
#include"stdlib.h"
#include"stdio.h"

linked_list* create_list(){
  linked_list* ll = (linked_list*)malloc(sizeof(linked_list));
  ll->head = NULL;
  ll->tail = NULL;
  return ll;
}

void append_to_front(linked_list* ll, void* item){
  if(ll == NULL) return;
  linked_list_link* link = (linked_list_link*)malloc(sizeof(linked_list_link));
  link->data = item;

  if(ll->head == NULL){
    link->prev = NULL;
    link->next = NULL;
    ll->head = link;
    ll->tail = link;
    return;
  }

  ll->head->prev = link;
  link->next = ll->head;
  link->prev = NULL;
  ll->head = link;
}

void append_to_end(linked_list* ll, void* item){
  if(ll == NULL) return;
  linked_list_link* link = (linked_list_link*)malloc(sizeof(linked_list_link));
  link->data = item;

  if(ll->head == NULL){
    link->prev = NULL;
    link->next = NULL;
    ll->head = link;
    ll->tail = link;
    return;
  }

  ll->tail->next = link;
  link->prev = ll->tail;
  link->next = NULL;
  ll->tail = link;
}

void delete_item(linked_list* ll, void* item){
  if(ll == NULL) return;
  linked_list_link* link = ll->head;

  while(link != NULL){
    if(link->data == item){
      if(link == ll->head && link == ll->tail){
        ll->head = NULL;
        ll->tail = NULL;
      } else if(link == ll->head){
        ll->head = ll->head->next;
      } else if(link == ll->tail){
        ll->tail = ll->tail->prev;
      } else {
        link->next->prev = link->prev;
        link->prev->next = link->next;
      }
      free(link);
      return;
    }
    link = link->next;
  }
}

void delete_link(linked_list* ll, linked_list_link* del_link){
  if(ll == NULL) return;
  linked_list_link* link = ll->head;

  while(link != NULL){
    if(link == del_link){
      if(link == ll->head && link == ll->tail){
        ll->head = NULL;
        ll->tail = NULL;
      } else if(link == ll->head){
        ll->head = ll->head->next;
      } else if(link == ll->tail){
        ll->tail = ll->tail->prev;
      } else {
        link->next->prev = link->prev;
        link->prev->next = link->next;
      }
      free(link);
      return;
    }
    link = link->next;
  }
}

void* pop(linked_list* ll){
  if(ll == NULL) return NULL;
  if(ll->tail == NULL) return NULL;
  linked_list_link* ret = ll->tail;
  void* data = ret->data;

  if(ret == ll->head && ret == ll->tail){
    ll->head = NULL;
    ll->tail = NULL;
  } else {
    ll->tail = ll->tail->prev;
  }

  free(ret);
  return data;
}

void push(linked_list* ll, void* item){
  append_to_end(ll, item);
}

void queue(linked_list* ll, void* item){
  append_to_front(ll, item);
}

linked_list_link* get_next_link(linked_list_link* this_link){
  if(this_link == NULL) return NULL;
  return this_link->next;
}

void free_list(linked_list* ll){
  linked_list_link* link = ll->head;
  while(link){
    free(link);
    link = get_next_link(link);
  }
}

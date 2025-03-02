#ifndef ENTITY_H_
#define ENTITY_H_

#include "Common.h"

#include "EntityProperty.h"
#include "List.h"

/////////////
//  TYPES  //
/////////////

typedef struct Entity
{
    char * name;
    List * properties;
    struct Entity * parent;
    struct Entity ** children;
    unsigned int childrenCount;
    unsigned int childrenArrLen;
} Entity;

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

Entity * Entity_init(char * name);
void Entity_free(Entity * e);

bool Entity_PropertyExists(Entity * e, char * propName);
bool Entity_AddProperty(Entity * e, char * propName);
void Entity_RemoveProperty(Entity * e, char * propName);
void Entity_RemoveAllProperties(Entity * e);
void * Entity_GetPropertyValue(Entity * e, char * propName);
void Entity_SetPropertyValue(Entity * e, char * propName, void * value);
bool Entity_NameExists(Entity * e, char * name);
bool Entity_AddChildEntity(Entity * p, Entity * c);
bool Entity_RemoveChildEntity(Entity * p, Entity * c);

void Entity_LinkProperties(Entity * src_e, char * src_p, Entity * dest_e, char * dest_p);

#endif
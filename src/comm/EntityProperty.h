#ifndef ENTITY_PROPERTY_H_
#define ENTITY_PROPERTY_H_

#include "Common.h"

#include "List.h"
#include "ArrayList.h"

/////////////
//  TYPES  //
/////////////

typedef struct EntityProperty
{
    char * name;
    void * value;

    // Entity struct
    void * owner;
    struct EntityProperty * srcProp;
    struct List * destProps;

    // called with arguments (EntityProperty * self, ArrayList * propagationPath)
    CallbackFunction onValueChanged;
} EntityProperty;

/////////////////////////////
//  FUNDTION DECLERATIONS  //
/////////////////////////////

EntityProperty * EntityProperty_init(void * owner, char * name);
void EntityProperty_initInPlace(EntityProperty * ep, void * owner, char * name);
void EntityProperty_free(EntityProperty * ep);
void EntityProperty_freeInPlace(EntityProperty * ep);

void EntityProperty_RefreshValue(EntityProperty * self);
void EntityProperty_SetValue(EntityProperty * self, void * value);

// -1: a is src, 1: b is src, 0 no linkage
U4 EntityProperty_AreLinked(EntityProperty * a, EntityProperty * b);
void EntityProperty_Link(EntityProperty * src, EntityProperty * dest);
void EntityProperty_UnLink(EntityProperty * src, EntityProperty * dest);
void EntityProperty_UnLinkSource(EntityProperty * self);
void EntityProperty_UnlinkDestinations(EntityProperty * src);
void EntityProperty_print(EntityProperty * self);

#endif
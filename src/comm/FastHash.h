#ifndef FAST_HASH_H_
#define FAST_HASH_H_

#include "Common.h"

///////////////
//  DEFINESz  //
///////////////

#define HASH_SEED                   0x12fa
#define HASH_ROUNDS                 8
#define FEEDBACK_VAL                0x87654321

/////////////////////////////
//  FUNDTION DECLERATIONS  //
/////////////////////////////

U4 FastHash_hash(void * key, U4 keyLen);

#endif

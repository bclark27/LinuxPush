/*
To use the mem tracker, define TRACK_MEM at the very top of any file you want
to track. If you define it at the top of a header, it will them track all files
that header is included in. If malloc is called in a file that is being tracked,
but then that memory is freed in a file where memory is not being tracked, then
the tracker will assume that the memory is still in use. Free can still be called
on pointers that have not been tracked, but the tracker will tell you it thinks
this is a bad free.
*/

#ifndef MEM_H_
#define MEM_H_

#include <stdlib.h>

///////////////
//  DEFINES  //
///////////////

#ifdef TRACK_MEMasdfasdf

#define malloc(s)       mem_malloc(s, __FILE__, __LINE__)
#define calloc(n, s)    mem_calloc(n, s, __FILE__, __LINE__)
#define realloc(p, s)   mem_realloc(p, s, __FILE__, __LINE__)
#define free(p)         mem_free(p, __FILE__, __LINE__)

#endif


/////////////////////////////
//  FUNDTION DECLERATIONS  //
/////////////////////////////

void mem_init(void);
void mem_freeTracker(void);
void mem_print(void);
void * mem_malloc(size_t size, const char * fileName, int lineNum) __attribute__ ((hot));
void * mem_calloc(size_t nmemb, size_t size, const char * fileName, int lineNum) __attribute__ ((hot));
void * mem_realloc(void *ptr, size_t size, const char * fileName, int lineNum) __attribute__ ((hot));
void mem_free(void * ptr, const char * fileName, int lineNum)__attribute__ ((hot));

#endif

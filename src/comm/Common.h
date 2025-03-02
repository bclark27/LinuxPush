#ifndef COMMON_H_
#define COMMON_H_

#define TRACK_MEM

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "Mem.h"
#include <stdarg.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stddef.h>

#include <libusb-1.0/libusb.h>

///////////////
//  DEFINES  //
///////////////

#define U1 unsigned char
#define U2 unsigned short
#define U4 unsigned int
#define U8 unsigned long

#define I1 char
#define I2 short
#define I4 int
#define I8 long

#define R4 float
#define R8 double
#define R16 long double

#define RAND_DOUBLE (((rand() / (double)RAND_MAX) * 2) - 1)
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define KB_SIZE         1024
#define MB_SIZE         (KB_SIZE * KB_SIZE)
#define PAGE_SIZE       (4 * KB_SIZE)
#define L1_CACHE_SIZE   (32 * PAGE_SIZE)
#define L2_CACHE_SIZE   (64 * PAGE_SIZE)
#define L3_CACHE_SIZE   (2304 * PAGE_SIZE)

#define BIT_0     (1 << 0)
#define BIT_1     (1 << 1)
#define BIT_2     (1 << 2)
#define BIT_3     (1 << 3)
#define BIT_4     (1 << 4)
#define BIT_5     (1 << 5)
#define BIT_6     (1 << 6)
#define BIT_7     (1 << 7)
#define BIT_8     (1 << 8)
#define BIT_9     (1 << 9)
#define BIT_10    (1 << 10)
#define BIT_11    (1 << 11)
#define BIT_12    (1 << 12)
#define BIT_13    (1 << 13)
#define BIT_14    (1 << 14)
#define BIT_15    (1 << 15)
#define BIT_16    (1 << 16)
#define BIT_17    (1 << 17)
#define BIT_18    (1 << 18)
#define BIT_19    (1 << 19)
#define BIT_20    (1 << 20)
#define BIT_21    (1 << 21)
#define BIT_22    (1 << 22)
#define BIT_23    (1 << 23)
#define BIT_24    (1 << 24)
#define BIT_25    (1 << 25)
#define BIT_26    (1 << 26)
#define BIT_27    (1 << 27)
#define BIT_28    (1 << 28)
#define BIT_29    (1 << 29)
#define BIT_30    (1 << 30)
#define BIT_31    (1 << 31)

#define BIT_SIZE(value)                 (sizeof(value) * 8)
#define BIT_GEN_MASK(bit)               (1 << (bit))
#define BIT_MASK(value, mask)           ((value) & (mask))
#define BIT_GET(value, bit)             (((value) & BIT_GEN_MASK(bit)) >> (bit))
#define BIT_SET(value, bit)             ((value) | BIT_GEN_MASK(bit))
#define BIT_CLEAR(value, bit)           ((value) & ~BIT_GEN_MASK(bit))
#define BIT_FLIP(value, bit)            ((value) ^ BIT_GEN_MASK(bit))

#define BIT_ARR_GET_BYTE(arr, bitIdx)           (((U1*)(arr))[(bitIdx) / 8])
#define BIT_ARR_SET_BYTE(arr, byt, bitIdx)      ((U1*)(arr))[(bitIdx) / 8] = (byt)

#define BIT_ARR_GET_BIT(arr, bitIdx)            BIT_GET(BIT_ARR_GET_BYTE(arr, bitIdx), (bitIdx) % 8)
#define BIT_ARR_SET_BIT(arr, bitIdx)            BIT_ARR_SET_BYTE(arr, BIT_SET(BIT_ARR_GET_BYTE(arr, bitIdx), (bitIdx) % 8), bitIdx)
#define BIT_ARR_CLEAR_BIT(arr, bitIdx)          BIT_ARR_SET_BYTE(arr, BIT_CLEAR(BIT_ARR_GET_BYTE(arr, bitIdx), (bitIdx) % 8), bitIdx)
#define BIT_ARR_FLIP_BIT(arr, bitIdx)           BIT_ARR_SET_BYTE(arr, BIT_FLIP(BIT_ARR_GET_BYTE(arr, bitIdx), (bitIdx) % 8), bitIdx)

#define my_assert(e) ((e) ? true : \
    (my_debug("%s:%s,%d: assertion '%s' failed\n", \
               __FILE__, __FUNCTION__, __LINE__, #e), false))

/////////////
//  TYPES  //
/////////////

typedef void (* FreeDataFunction)(void *);
typedef U1 (* CompareFunction)(void *, void *);
typedef void ( *CallbackFunction)(void *, void *);
typedef U4 ( *HashFunction)(void *, U4);

/////////////////////////////
//  FUNCTION DECLERATIONS  //
/////////////////////////////

#ifndef __MY_DEBUG
#define __MY_DEBUG

static inline bool my_debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    return false;
}

#endif

#endif

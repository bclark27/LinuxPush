#ifndef __md5_h__
#define __md5_h__

#ifndef uint8
#define uint8 unsigned char
#endif

#ifndef uint32
#define uint32 unsigned long int
#endif

typedef struct {
  uint32 total[2];
  uint32 state[4];
  uint8 buffer[64];
} MD5_context;

void MD5_starts(MD5_context *ctx);
void MD5_update(MD5_context *ctx, uint8 *input, uint32 length);
void MD5_finish(MD5_context *ctx, uint8 digest[16]);

#endif // __md5_h__

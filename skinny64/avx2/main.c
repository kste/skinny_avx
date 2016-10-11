/*
Implementation of Skinny64/128 with AVX2.

This code verifies the test vectors for Skinny and can
also be used to run benchmarks.
*/
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "Skinny64128AVX2.h"
#include "timing.h"

#define NUM_TIMINGS 2000

extern void unpack_and_store_message(unsigned char *out, u256 x[16]);
extern void pack_message(u256 x[16], const unsigned char *in);

//Skinny encryption using 64 blocks
#include "skinny64.c"

int crypto_stream_skinny64128ecb_avx2(
  unsigned char *out,
  unsigned char *in,
  unsigned long long inlen,
  const unsigned char *k
) 
{
  int i, j;
  u256 rk[36][8];
  u256 x[16];
  u256 key;

  if (!inlen) return 0;

  key_schedule(k, rk);

  while(inlen >= 512){
    pack_message(x, in);
    encrypt_64blocks(x, rk);
    unpack_and_store_message(out, x);
    
    inlen -= 512;
    in += 512;
    out += 512;
  }

  return 0;
}

void check_testvector() {
  unsigned char *in,*out;
  unsigned long long inlen;

  int i;

  //Encrypt the test vector
  unsigned char plaintext[8] = {0xcf, 0x16, 0xcf, 0xe8, 0xfd, 0x0f, 0x98, 0xaa};
  unsigned char k[16] = {0x9e, 0xb9, 0x36, 0x40, 0xd0, 0x88, 0xda, 0x63,
                         0x76, 0xa3, 0x9d, 0x1c, 0x8b, 0xea, 0x71, 0xe1};
  unsigned ciphertext[8] = {0x6c, 0xed, 0xa1, 0xf4, 0x3d, 0xe9, 0x2b, 0x9e};

  //Generate 64 blocks of plaintext
  inlen=512;                      
  in = malloc(512);
  out = malloc(512);

  for(i = 0; i < 64; i++)
    memcpy(in + 8*i, &plaintext, 8);

  //Generate the output stream
  crypto_stream_skinny64128ecb_avx2(out,in,inlen,k);

  //Validate outputstream
  for(i = 0; i < 512; i++) {
    if(out[i] != ciphertext[i % 8]) {
      printf("ERROR: Outputstream does not match test vector at position %i!\n", i);
    }
  } 

  free(out);
  free(in);
}

int cmp_dbl(const void *x, const void *y)
{
  double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy) return -1;
  if (xx > yy) return  1;
  return 0;
}

int main() {
  check_testvector();
  
  //Benchmark Skinny
  unsigned char *in, *out, *k;
  unsigned long long inlen;
  u64 timer = 0;
  double timings[NUM_TIMINGS];

  int i,j;

  srand(0);
  inlen = 512 * NUM_TIMINGS;
  in = malloc(inlen);
  out = malloc(inlen);
  k = malloc(16);

  inlen = 512 * NUM_TIMINGS;

  for(i =- 100; i < NUM_TIMINGS; i++){
    //Get random input
    for(j = 0; j < inlen; j++) 
      in[j] = rand() & 0xff;
    for(j = 0; j < 16; j++) 
      k[j] = rand() & 0xff;

    timer = start_rdtsc();
    crypto_stream_skinny64128ecb_avx2(out,in,inlen,k);
    timer = end_rdtsc() - timer;

    if(i >= 0 && i < NUM_TIMINGS) 
      timings[i] = ((double)timer) / inlen;
  }

  //Get Median
  qsort(timings, NUM_TIMINGS, sizeof(double), cmp_dbl);
  printf("Skinny64128: %f cycles per byte\n", timings[NUM_TIMINGS / 2]);

  free(k);
  free(out);
  free(in);
}

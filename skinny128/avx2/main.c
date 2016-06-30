/*
Implementation of Skinny128/128 with AVX2.

This code verifies the test vectors for Skinny and can
also be used to run benchmarks.
*/
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "Skinny128128AVX2.h"
#include "timing.h"

#define NUM_TIMINGS 2000

extern void unpack_and_store_message(unsigned char *out, u256 x[32]);
extern void pack_message(u256 x[32], const unsigned char *in);

//Skinny encryption using 64 blocks
#include "skinny128.c"

int crypto_stream_skinny128128ecb_avx2(
  unsigned char *out,
  unsigned char *in,
  unsigned long long inlen,
  const unsigned char *k
) 
{
  int i, j;
  u256 rk[40][16];
  u256 x[32];
  u256 key;

  if (!inlen) return 0;

  key_schedule(k, rk);

  while(inlen >= 1024){
      pack_message(x, in);
      encrypt_64blocks(x, rk);
      unpack_and_store_message(out, x);
    
    inlen -= 1024;
    in += 1024;
    out += 1024;
  }

  return 0;
}

void check_testvector() {
  unsigned char *in,*out;
  unsigned long long inlen;

  int i;

  //Encrypt the test vector
  unsigned char plaintext[16] = {0xf2,0x0a,0xdb,0x0e,0xb0,0x8b,0x64,0x8a,
                                 0x3b,0x2e,0xee,0xd1,0xf0,0xad,0xda,0x14};
  unsigned char k[16] = {0x4f,0x55,0xcf,0xb0,0x52,0x0c,0xac,0x52,
                         0xfd,0x92,0xc1,0x5f,0x37,0x07,0x3e,0x93};
  unsigned ciphertext[16] = {0x22,0xff,0x30,0xd4,0x98,0xea,0x62,0xd7,
                             0xe4,0x5b,0x47,0x6e,0x33,0x67,0x5b,0x74};

  //Generate 64 blocks of plaintext
  inlen = 1024;
  in = malloc(1024);
  out = malloc(1024);

  for(i = 0; i < 64; i++)
    memcpy(in + 16*i, &plaintext, 16);

  //Generate the output stream
  crypto_stream_skinny128128ecb_avx2(out,in,inlen,k);

  //Validate outputstream
  for(i = 0; i < 1024; i++) {
    if(out[i] != ciphertext[i % 16]) {
      printf("ERROR: Outputstream does not match test vector at position %i!\n", i);
    }
  } 

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
  inlen = 1024 * NUM_TIMINGS;
  in = malloc(inlen);
  out = malloc(inlen);
  k = malloc(16);

  inlen = 1024 * NUM_TIMINGS;

  for(i =- 100; i < NUM_TIMINGS; i++){
    //Get random input
    for(j = 0; j < inlen; j++) 
      in[j] = rand() & 0xff;
    for(j = 0; j < 16; j++) 
      k[j] = rand() & 0xff;

    timer = start_rdtsc();
    crypto_stream_skinny128128ecb_avx2(out,in,inlen,k);
    timer = end_rdtsc() - timer;

    if(i >= 0 && i < NUM_TIMINGS) 
      timings[i] = ((double)timer) / inlen;
  }

  //Get Median
  qsort(timings, NUM_TIMINGS, sizeof(double), cmp_dbl);
  printf("Skinny128128: %f cycles per byte\n", timings[NUM_TIMINGS / 2]);
}

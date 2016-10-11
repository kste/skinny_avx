#include "Skinny64128AVX2.h"

void pack_message(u256 x[16], const unsigned char *in) {
  int i;

  for(i = 0; i < 16; i++) {
    x[i] = LOAD(in + i*32);
  }

 
  //Seperate bits for S-box
  SWAPMOVE(x[0], x[1], MASKa, 1);
  SWAPMOVE(x[2], x[3], MASKa, 1);
  SWAPMOVE(x[0], x[2], MASKc, 2);
  SWAPMOVE(x[1], x[3], MASKc, 2);

  SWAPMOVE(x[4], x[5], MASKa, 1);
  SWAPMOVE(x[6], x[7], MASKa, 1);
  SWAPMOVE(x[4], x[6], MASKc, 2);
  SWAPMOVE(x[5], x[7], MASKc, 2);  

  SWAPMOVE(x[8], x[9], MASKa, 1);
  SWAPMOVE(x[10], x[11], MASKa, 1);
  SWAPMOVE(x[8], x[10], MASKc, 2);
  SWAPMOVE(x[9], x[11], MASKc, 2);

  SWAPMOVE(x[12], x[13], MASKa, 1);
  SWAPMOVE(x[14], x[15], MASKa, 1);
  SWAPMOVE(x[12], x[14], MASKc, 2);
  SWAPMOVE(x[13], x[15], MASKc, 2);    

  //Group the rows for efficient MixColumns implementation
  SWAPMOVE(x[4], x[0], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[12], x[8], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[8], x[0], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[12], x[4], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[5], x[1], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[13], x[9], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[9], x[1], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[13], x[5], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[6], x[2], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[14], x[10], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[10], x[2], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[14], x[6], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[7], x[3], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[15], x[11], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[11], x[3], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[15], x[7], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  //Rearrange nibbles for efficient ShiftRows
  for(i = 0; i < 16; i++) {
    SWAPMOVE(x[i], x[i], _mm256_set_epi32(0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000), 12);
  }
}

void pack_key(u256 x[16], const unsigned char *in) {
  int i;

  for(i = 0; i < 16; i++) {
    x[i] = LOAD(in); //Load same key for all blocks
  }

  //Seperate bits for S-box
  SWAPMOVE(x[0], x[1], MASKa, 1);
  SWAPMOVE(x[2], x[3], MASKa, 1);
  SWAPMOVE(x[0], x[2], MASKc, 2);
  SWAPMOVE(x[1], x[3], MASKc, 2);

  SWAPMOVE(x[4], x[5], MASKa, 1);
  SWAPMOVE(x[6], x[7], MASKa, 1);
  SWAPMOVE(x[4], x[6], MASKc, 2);
  SWAPMOVE(x[5], x[7], MASKc, 2);  

  SWAPMOVE(x[8], x[9], MASKa, 1);
  SWAPMOVE(x[10], x[11], MASKa, 1);
  SWAPMOVE(x[8], x[10], MASKc, 2);
  SWAPMOVE(x[9], x[11], MASKc, 2);

  SWAPMOVE(x[12], x[13], MASKa, 1);
  SWAPMOVE(x[14], x[15], MASKa, 1);
  SWAPMOVE(x[12], x[14], MASKc, 2);
  SWAPMOVE(x[13], x[15], MASKc, 2);    

  //Group the rows for efficient MixColumns implementation
  SWAPMOVE(x[4], x[0], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[12], x[8], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[8], x[0], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[12], x[4], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[5], x[1], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[13], x[9], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[9], x[1], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[13], x[5], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[6], x[2], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[14], x[10], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[10], x[2], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[14], x[6], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[7], x[3], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[15], x[11], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[11], x[3], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[15], x[7], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  //Rearrange nibbles for efficient ShiftRows
  for(i = 0; i < 16; i++) {
    SWAPMOVE(x[i], x[i], _mm256_set_epi32(0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000), 12);
  }
}

//Unpacking
void unpack_and_store_message(unsigned char *out, u256 x[16]) {
  int i;

  //Rearrange nibbles for efficient ShiftRows
  for(i = 0; i < 16; i++) {
    SWAPMOVE(x[i], x[i], _mm256_set_epi32(0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000, 0x0f0f0000), 12);
  }

  //Group the rows for efficient MixColumns implementation
  SWAPMOVE(x[4], x[0], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[12], x[8], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[8], x[0], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[12], x[4], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[5], x[1], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[13], x[9], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[9], x[1], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[13], x[5], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[6], x[2], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[14], x[10], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[10], x[2], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[14], x[6], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  SWAPMOVE(x[7], x[3], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[15], x[11], _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000), 16);
  SWAPMOVE(x[11], x[3], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);
  SWAPMOVE(x[15], x[7], _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000), 32);

  //Seperate bits for S-box
  SWAPMOVE(x[0], x[1], MASKa, 1);
  SWAPMOVE(x[2], x[3], MASKa, 1);
  SWAPMOVE(x[0], x[2], MASKc, 2);
  SWAPMOVE(x[1], x[3], MASKc, 2);

  SWAPMOVE(x[4], x[5], MASKa, 1);
  SWAPMOVE(x[6], x[7], MASKa, 1);
  SWAPMOVE(x[4], x[6], MASKc, 2);
  SWAPMOVE(x[5], x[7], MASKc, 2);  

  SWAPMOVE(x[8], x[9], MASKa, 1);
  SWAPMOVE(x[10], x[11], MASKa, 1);
  SWAPMOVE(x[8], x[10], MASKc, 2);
  SWAPMOVE(x[9], x[11], MASKc, 2);

  SWAPMOVE(x[12], x[13], MASKa, 1);
  SWAPMOVE(x[14], x[15], MASKa, 1);
  SWAPMOVE(x[12], x[14], MASKc, 2);
  SWAPMOVE(x[13], x[15], MASKc, 2);

  STORE(out + 32*0,  x[0]);
  STORE(out + 32*1,  x[1]);
  STORE(out + 32*2,  x[2]);
  STORE(out + 32*3,  x[3]);
  STORE(out + 32*4,  x[4]);
  STORE(out + 32*5,  x[5]);
  STORE(out + 32*6,  x[6]);
  STORE(out + 32*7,  x[7]);
  STORE(out + 32*8,  x[8]);
  STORE(out + 32*9,  x[9]);
  STORE(out + 32*10, x[10]);
  STORE(out + 32*11, x[11]);
  STORE(out + 32*12, x[12]);
  STORE(out + 32*13, x[13]);
  STORE(out + 32*14, x[14]);
  STORE(out + 32*15, x[15]);
}

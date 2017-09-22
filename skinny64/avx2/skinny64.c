#include "Skinny64128AVX2.h"
#include <string.h>

extern void pack_key(u256 x[16], const unsigned char *in);

const unsigned char RC[62] = {
    0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F,
    0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 0x1D, 0x3A, 0x35, 0x2B,
    0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E,
    0x1C, 0x38, 0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A,
    0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04, 0x09, 0x13,
    0x26, 0x0c, 0x19, 0x32, 0x25, 0x0a, 0x15, 0x2a, 0x14, 0x28,
    0x10, 0x20};

void encrypt_64blocks(u256 x[16], u256 rk[36][8]) {

  int i, j;
  u256 rc, tmp[3];
  rc = _mm256_set_epi64x(0xFF000000FF0000ull,
                         0xFF000000FF0000ull,
                         0xFF000000FF0000ull,
                         0xFF000000FF0000ull);

  //Flip bits for using equivalent S-box
  for(i = 0; i < 16; i++)
    x[i] = NOT(x[i]);

  for(i = 0; i < 36; i++){
    //SubBytes
    //Note that there is no NOR operation available therefore we use an
    //equivalent S-box representation which only requires AND and XOR.
    //However, this S-box computes (~a, ~b, ~c, ~d) -> (~a, ~b, ~c, ~d)
    //requiring some additional NOT operations to get the correct result.
    for(j = 0; j < 4; j++) {
      tmp[0] = AND(x[1 + 4*j], x[2 + 4*j]);
      tmp[0] = XOR(x[0 + 4*j], tmp[0]);
      x[0 + 4*j] = AND(x[0 + 4*j], x[1 + 4*j]);
      x[0 + 4*j] = XOR(x[0 + 4*j], x[3 + 4*j]);
      x[3 + 4*j] = AND(x[2 + 4*j], x[0 + 4*j]);
      x[1 + 4*j] = XOR(x[1 + 4*j], x[3 + 4*j]);
      x[3 + 4*j] = AND(tmp[0], x[0 + 4*j]);
      x[2 + 4*j] = XOR(x[3 + 4*j], x[2 + 4*j]);

      //Change order
      x[3 + 4*j] = x[2 + 4*j];
      x[2 + 4*j] = x[1 + 4*j];
      x[1 + 4*j] = tmp[0];
    }
    
    //AddConstant
    //This only adds c2. The other constants are added with the key
    x[10] = XOR(x[10], rc);
  
    //AddKey
    x[0] = XOR(x[0], rk[i][0]);
    x[1] = XOR(x[1], rk[i][1]);
    x[2] = XOR(x[2], rk[i][2]);
    x[3] = XOR(x[3], rk[i][3]);
    x[4] = XOR(x[4], rk[i][4]);
    x[5] = XOR(x[5], rk[i][5]);
    x[6] = XOR(x[6], rk[i][6]);
    x[7] = XOR(x[7], rk[i][7]);

    //ShiftRows
    x[4] = SR1(x[4]); x[8]  = SR2(x[8]);  x[12] = SR3(x[12]);
    x[5] = SR1(x[5]); x[9]  = SR2(x[9]);  x[13] = SR3(x[13]);
    x[6] = SR1(x[6]); x[10] = SR2(x[10]); x[14] = SR3(x[14]);
    x[7] = SR1(x[7]); x[11] = SR2(x[11]); x[15] = SR3(x[15]);

    //MixColumns
    tmp[0] = x[12]; tmp[1] = x[13]; tmp[2] = x[14]; tmp[3] = x[15];

    x[12] = XOR(x[8], x[0]);  x[13] = XOR(x[9], x[1]);
    x[14] = XOR(x[10], x[2]); x[15] = XOR(x[11], x[3]);

    x[8] = XOR(x[4], x[8]);   x[9] = XOR(x[5], x[9]);
    x[10] = XOR(x[6], x[10]); x[11] = XOR(x[7], x[11]);

    x[4] = x[0]; x[5] = x[1];
    x[6] = x[2]; x[7] = x[3];

    x[0] = XOR(tmp[0], x[12]); x[1] = XOR(tmp[1], x[13]);
    x[2] = XOR(tmp[2], x[14]); x[3] = XOR(tmp[3], x[15]);


    //Flip bits in 3rd and 4th row to correct S-box input for next round.
    x[8] = NOT(x[8]); x[9] = NOT(x[9]); x[10] = NOT(x[10]); x[11] = NOT(x[11]);
    x[12] = NOT(x[12]); x[13] = NOT(x[13]); x[14] = NOT(x[14]); x[15] = NOT(x[15]);
  }

  //Flip bits for using equivalent S-box
  for(i = 0; i < 16; i++)
    x[i] = NOT(x[i]);
}

void key_schedule(const unsigned char *k, u256 rk[36][8]) {
  int i, j;
  u256 tk1[16], tk2[16], tmp[16];

  unsigned char *tmp_key = malloc(32);

  for(i = 0; i < 4; i++)
    memcpy(tmp_key + 8*i, k, 8);
  pack_key(tk1, tmp_key);

  for(i = 0; i < 4; i++)
    memcpy(tmp_key + 8*i, k + 8, 8);
  pack_key(tk2, tmp_key);

  for(j = 0; j < 36; j++) {
    //Extract round key
    for(i = 0; i < 8; i++){
      rk[j][i] = XOR(tk1[i], tk2[i]);
    }

    //Add constant into key
    u256 rc = _mm256_set_epi64x(0xFF000000FF0000ull,
                                0xFF000000FF0000ull,
                                0xFF000000FF0000ull,
                                0xFF000000FF0000ull);

    if(RC[j]>>5 & 1)
      rk[j][6] = XOR(rk[j][6], rc);
    if(RC[j]>>4 & 1)
      rk[j][7] = XOR(rk[j][7], rc);
    if(RC[j]>>3 & 1)
      rk[j][0] = XOR(rk[j][0], rc);
    if(RC[j]>>2 & 1)
      rk[j][1] = XOR(rk[j][1], rc);
    if(RC[j]>>1 & 1)
      rk[j][2] = XOR(rk[j][2], rc);
    if(RC[j]>>0 & 1)
      rk[j][3] = XOR(rk[j][3], rc);
    
    //Update TK1
    for(i = 0; i < 8; i++){
      tmp[8 + i] = tk1[0 + i];
    }

    //Apply bit permutation
    for(i = 0; i < 4; i++){
      tmp[0 + i] = XOR(_mm256_shuffle_epi8(tk1[8   + i], _mm256_set_epi8(30,28,0xff,0xff,26,24,0xff,0xff,22,20,0xff,0xff,18,16,0xff,0xff,14,12,0xff,0xff,10,8,0xff,0xff,6,4,0xff,0xff,2,0,0xff,0xff)),
                       _mm256_shuffle_epi8(tk1[12  + i], _mm256_set_epi8(0xff,0xff,28,29,0xff,0xff,24,25,0xff,0xff,20,21,0xff,0xff,16,17,0xff,0xff,12,13,0xff,0xff,8,9,0xff,0xff,4,5,0xff,0xff,0,1)));
      tmp[4 + i] = XOR(_mm256_shuffle_epi8(tk1[12  + i], _mm256_set_epi8(30,0xff,0xff,31,26,0xff,0xff,27,22,0xff,0xff,23,18,0xff,0xff,19,14,0xff,0xff,15,10,0xff,0xff,11,6,0xff,0xff,7,2,0xff,0xff,3)),
                       _mm256_shuffle_epi8(tk1[8   + i], _mm256_set_epi8(0xff,31,29,0xff,0xff,27,25,0xff,0xff,23,21,0xff,0xff,19,17,0xff,0xff,15,13,0xff,0xff,11,9,0xff,0xff,7,5,0xff,0xff,3,1,0xff)));
    }

    for(i = 0; i < 16; i++){
      tk1[i] = tmp[i];
    }


    //Update TK2
    for(i = 0; i < 8; i++){
      tmp[8 + i] = tk2[0 + i];
    }

    //Apply bit permutation
    for(i = 0; i < 4; i++){
      tmp[0 + i] = XOR(_mm256_shuffle_epi8(tk2[8   + i], _mm256_set_epi8(30,28,0xff,0xff,26,24,0xff,0xff,22,20,0xff,0xff,18,16,0xff,0xff,14,12,0xff,0xff,10,8,0xff,0xff,6,4,0xff,0xff,2,0,0xff,0xff)),
                       _mm256_shuffle_epi8(tk2[12  + i], _mm256_set_epi8(0xff,0xff,28,29,0xff,0xff,24,25,0xff,0xff,20,21,0xff,0xff,16,17,0xff,0xff,12,13,0xff,0xff,8,9,0xff,0xff,4,5,0xff,0xff,0,1)));
      tmp[4 + i] = XOR(_mm256_shuffle_epi8(tk2[12  + i], _mm256_set_epi8(30,0xff,0xff,31,26,0xff,0xff,27,22,0xff,0xff,23,18,0xff,0xff,19,14,0xff,0xff,15,10,0xff,0xff,11,6,0xff,0xff,7,2,0xff,0xff,3)),
                       _mm256_shuffle_epi8(tk2[8   + i], _mm256_set_epi8(0xff,31,29,0xff,0xff,27,25,0xff,0xff,23,21,0xff,0xff,19,17,0xff,0xff,15,13,0xff,0xff,11,9,0xff,0xff,7,5,0xff,0xff,3,1,0xff)));
    }

    //LFSR for first two rows
    tk2[3] = XOR(tmp[1], tmp[0]);
    tk2[2] = tmp[3];
    tk2[1] = tmp[2];
    tk2[0] = tmp[1];
    tk2[7] = XOR(tmp[5], tmp[4]);
    tk2[6] = tmp[7];
    tk2[5] = tmp[6];
    tk2[4] = tmp[5];
    
    for(i = 8; i < 16; i++){
      tk2[i] = tmp[i];
    }    
  }
  free(tmp_key);
}

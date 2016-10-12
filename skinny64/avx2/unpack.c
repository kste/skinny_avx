#include "Skinny64128AVX2.h"

void pack_message(u256 x[16], const unsigned char *in) {
  int i;

  for(i = 0; i < 16; i++) {
    x[i] = LOAD(in + i*32);
  }
 
  //Seperate bits for S-box
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[4*i + 0], x[4*i + 1], MASK1, 1);
    SWAPMOVE(x[4*i + 2], x[4*i + 3], MASK1, 1);
    SWAPMOVE(x[4*i + 0], x[4*i + 2], MASK2, 2);
    SWAPMOVE(x[4*i + 1], x[4*i + 3], MASK2, 2);    
  }

  //Group nibbles to bytes for ShiftRows
  SWAPMOVE(x[4], x[0], MASK4, 4);
  SWAPMOVE(x[5], x[1], MASK4, 4);
  SWAPMOVE(x[6], x[2], MASK4, 4);
  SWAPMOVE(x[7], x[3], MASK4, 4);
  SWAPMOVE(x[12], x[8], MASK4, 4);
  SWAPMOVE(x[13], x[9], MASK4, 4);
  SWAPMOVE(x[14], x[10], MASK4, 4);
  SWAPMOVE(x[15], x[11], MASK4, 4);    

  //Group the rows for efficient MixColumns implementation
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[i + 4], x[i + 0], MASK16, 16);
    SWAPMOVE(x[i + 12], x[i + 8], MASK16, 16);
    SWAPMOVE(x[i + 8], x[i + 0], MASK32, 32);
    SWAPMOVE(x[i + 12], x[i + 4],MASK32, 32);    
  }
}

void pack_key(u256 x[16], const unsigned char *in) {
  int i;

  for(i = 0; i < 16; i++) {
    x[i] = LOAD(in); //Load same key for all blocks
  }

  //Seperate bits for S-box
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[4*i + 0], x[4*i + 1], MASK1, 1);
    SWAPMOVE(x[4*i + 2], x[4*i + 3], MASK1, 1);
    SWAPMOVE(x[4*i + 0], x[4*i + 2], MASK2, 2);
    SWAPMOVE(x[4*i + 1], x[4*i + 3], MASK2, 2);    
  }

  //Group nibbles to bytes for ShiftRows
  SWAPMOVE(x[4], x[0], MASK4, 4);
  SWAPMOVE(x[5], x[1], MASK4, 4);
  SWAPMOVE(x[6], x[2], MASK4, 4);
  SWAPMOVE(x[7], x[3], MASK4, 4);
  SWAPMOVE(x[12], x[8], MASK4, 4);
  SWAPMOVE(x[13], x[9], MASK4, 4);
  SWAPMOVE(x[14], x[10], MASK4, 4);
  SWAPMOVE(x[15], x[11], MASK4, 4);    

  //Group the rows for efficient MixColumns implementation
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[i + 4], x[i + 0], MASK16, 16);
    SWAPMOVE(x[i + 12], x[i + 8], MASK16, 16);
    SWAPMOVE(x[i + 8], x[i + 0], MASK32, 32);
    SWAPMOVE(x[i + 12], x[i + 4],MASK32, 32);    
  }
}

//Unpacking
void unpack_and_store_message(unsigned char *out, u256 x[16]) {
  int i;

  //Group the rows for efficient MixColumns implementation
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[i + 4], x[i + 0], MASK16, 16);
    SWAPMOVE(x[i + 12], x[i + 8], MASK16, 16);
    SWAPMOVE(x[i + 8], x[i + 0], MASK32, 32);
    SWAPMOVE(x[i + 12], x[i + 4],MASK32, 32);    
  }

  //Group nibbles to bytes for ShiftRows
  SWAPMOVE(x[4], x[0], MASK4, 4);
  SWAPMOVE(x[5], x[1], MASK4, 4);
  SWAPMOVE(x[6], x[2], MASK4, 4);
  SWAPMOVE(x[7], x[3], MASK4, 4);
  SWAPMOVE(x[12], x[8], MASK4, 4);
  SWAPMOVE(x[13], x[9], MASK4, 4);
  SWAPMOVE(x[14], x[10], MASK4, 4);
  SWAPMOVE(x[15], x[11], MASK4, 4);    

  //Seperate bits for S-box
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[4*i + 0], x[4*i + 1], MASK1, 1);
    SWAPMOVE(x[4*i + 2], x[4*i + 3], MASK1, 1);
    SWAPMOVE(x[4*i + 0], x[4*i + 2], MASK2, 2);
    SWAPMOVE(x[4*i + 1], x[4*i + 3], MASK2, 2);    
  }

  for(i = 0; i < 16; i++) {
    STORE(out + 32*i,  x[i]);
  }
}

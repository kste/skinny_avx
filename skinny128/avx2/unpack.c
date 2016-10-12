#include "Skinny128128AVX2.h"

void pack_message(u256 x[32], const unsigned char *in) {
  int i;

  for(i = 0; i < 32; i++) {
    x[i] = LOAD(in + i*32);
  }

  //Seperate bits for S-box
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[8*i + 0], x[8*i + 1], MASK1, 1);
    SWAPMOVE(x[8*i + 2], x[8*i + 3], MASK1, 1);
    SWAPMOVE(x[8*i + 4], x[8*i + 5], MASK1, 1);
    SWAPMOVE(x[8*i + 6], x[8*i + 7], MASK1, 1);

    SWAPMOVE(x[8*i + 0], x[8*i + 2], MASK2, 2);
    SWAPMOVE(x[8*i + 1], x[8*i + 3], MASK2, 2);
    SWAPMOVE(x[8*i + 4], x[8*i + 6], MASK2, 2);
    SWAPMOVE(x[8*i + 5], x[8*i + 7], MASK2, 2);

    SWAPMOVE(x[8*i + 0], x[8*i + 4], MASK4, 4);
    SWAPMOVE(x[8*i + 1], x[8*i + 5], MASK4, 4);
    SWAPMOVE(x[8*i + 2], x[8*i + 6], MASK4, 4);
    SWAPMOVE(x[8*i + 3], x[8*i + 7], MASK4, 4);    
  }

  //Group the rows for efficient MixColumns implementation
  for(i = 0; i < 8; i++) {
    SWAPMOVE(x[i + 8], x[i + 0], MASK32, 32);
    SWAPMOVE(x[i + 24], x[i + 16], MASK32, 32);
    
    SWAPMOVEBY64(x[i + 16], x[i + 0], MASK64);
    SWAPMOVEBY64(x[i + 24], x[i + 8], MASK64);    
  }
}

void pack_key(u256 x[32], const unsigned char *in) {
  int i;

  //Load same key for all blocks
  for(i = 0; i < 32; i++) {
    x[i] = LOAD(in);
  }

  //Seperate bits for S-box
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[8*i + 0], x[8*i + 1], MASK1, 1);
    SWAPMOVE(x[8*i + 2], x[8*i + 3], MASK1, 1);
    SWAPMOVE(x[8*i + 4], x[8*i + 5], MASK1, 1);
    SWAPMOVE(x[8*i + 6], x[8*i + 7], MASK1, 1);

    SWAPMOVE(x[8*i + 0], x[8*i + 2], MASK2, 2);
    SWAPMOVE(x[8*i + 1], x[8*i + 3], MASK2, 2);
    SWAPMOVE(x[8*i + 4], x[8*i + 6], MASK2, 2);
    SWAPMOVE(x[8*i + 5], x[8*i + 7], MASK2, 2);

    SWAPMOVE(x[8*i + 0], x[8*i + 4], MASK4, 4);
    SWAPMOVE(x[8*i + 1], x[8*i + 5], MASK4, 4);
    SWAPMOVE(x[8*i + 2], x[8*i + 6], MASK4, 4);
    SWAPMOVE(x[8*i + 3], x[8*i + 7], MASK4, 4);    
  }

  //Group the rows for efficient MixColumns implementation
  for(i = 0; i < 8; i++) {
    SWAPMOVE(x[i + 8], x[i + 0], MASK32, 32);
    SWAPMOVE(x[i + 24], x[i + 16], MASK32, 32);
    
    SWAPMOVEBY64(x[i + 16], x[i + 0], MASK64);
    SWAPMOVEBY64(x[i + 24], x[i + 8], MASK64);    
  }
}

//Unpacking
void unpack_and_store_message(unsigned char *out, u256 x[32]) {
  int i;

  //Group the rows for efficient MixColumns implementation
  for(i = 0; i < 8; i++) {
    SWAPMOVE(x[i + 8], x[i + 0], MASK32, 32);
    SWAPMOVE(x[i + 24], x[i + 16], MASK32, 32);
    
    SWAPMOVEBY64(x[i + 16], x[i + 0], MASK64);
    SWAPMOVEBY64(x[i + 24], x[i + 8], MASK64);    
  }

  //Seperate bits for S-box
  for(i = 0; i < 4; i++) {
    SWAPMOVE(x[8*i + 0], x[8*i + 1], MASK1, 1);
    SWAPMOVE(x[8*i + 2], x[8*i + 3], MASK1, 1);
    SWAPMOVE(x[8*i + 4], x[8*i + 5], MASK1, 1);
    SWAPMOVE(x[8*i + 6], x[8*i + 7], MASK1, 1);

    SWAPMOVE(x[8*i + 0], x[8*i + 2], MASK2, 2);
    SWAPMOVE(x[8*i + 1], x[8*i + 3], MASK2, 2);
    SWAPMOVE(x[8*i + 4], x[8*i + 6], MASK2, 2);
    SWAPMOVE(x[8*i + 5], x[8*i + 7], MASK2, 2);

    SWAPMOVE(x[8*i + 0], x[8*i + 4], MASK4, 4);
    SWAPMOVE(x[8*i + 1], x[8*i + 5], MASK4, 4);
    SWAPMOVE(x[8*i + 2], x[8*i + 6], MASK4, 4);
    SWAPMOVE(x[8*i + 3], x[8*i + 7], MASK4, 4);    
  }  

  for(i = 0; i < 32; i++) {
    STORE(out + 32*i, x[i]);
  }
}
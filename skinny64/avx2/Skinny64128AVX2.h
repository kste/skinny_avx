/*
Defintions and intrinsics used for the implementation of 
Skinny64/128.
*/

#ifndef SKINNY64128AVX2_H
#define SKINNY64128AVX2_H
#include "immintrin.h"

//Types
#define u16 unsigned short
#define u32 unsigned
#define u64 unsigned long long
#define u256 __m256i

//Intrinsics
#define XOR _mm256_xor_si256
#define AND _mm256_and_si256
#define ANDNOT _mm256_andnot_si256
#define OR _mm256_or_si256
#define NOT(x) _mm256_xor_si256(x, _mm256_set_epi32(-1, -1, -1, -1, -1, -1, -1, -1))
#define NOR(x, y) _mm256_xor_si256(_mm256_or_si256(x, y), _mm256_set_epi32(-1, -1, -1, -1, -1, -1, -1, -1))
#define SHIFTR64(x, y) _mm256_srli_epi64(x, y)
#define SHIFTL64(x, y) _mm256_slli_epi64(x, y)

#define SR1(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(28,29,31,30,24,25,27,26,20,21,23,22,16,17,19,18,12,13,15,14,8,9,11,10,4,5,7,6,0,1,3,2))
#define SR2(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(30,31,28,29,26,27,24,25,22,23,20,21,18,19,16,17,14,15,12,13,10,11,8,9,6,7,4,5,2,3,0,1))
#define SR3(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(29,28,30,31,25,24,26,27,21,20,22,23,17,16,18,19,13,12,14,15,9,8,10,11,5,4,6,7,1,0,2,3))

#define LOAD(src) _mm256_loadu_si256((__m256i *)(src))
#define STORE(dest,src) _mm256_storeu_si256((__m256i *)(dest),src)

#define MASK1 _mm256_set_epi32(0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa)
#define MASK2 _mm256_set_epi32(0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc)
#define MASK4 _mm256_set_epi32(0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0)
#define MASK16 _mm256_set_epi32(0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000, 0xffff0000)
#define MASK32 _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000)


#define SWAPMOVE(a, b, mask, shift) \
{ \
	u256 T = AND(XOR(SHIFTL64(a, shift), b), mask); \
	b = XOR(b, T); \
    a = XOR(a, SHIFTR64(T, shift)); \
}

#endif
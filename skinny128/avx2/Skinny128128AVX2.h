/*
Constants, parameters and intrinsics used for the implementation of 
Skinny128/128.
*/

#ifndef SKINNY128128AVX2_H
#define SKINNY128128AVX2_H
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

#define SR1(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(30,29,28,31,26,25,24,27,22,21,20,23,18,17,16,19,14,13,12,15,10,9,8,11,6,5,4,7,2,1,0,3))
#define SR2(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2))
#define SR3(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(28,31,30,29,24,27,26,25,20,23,22,21,16,19,18,17,12,15,14,13,8,11,10,9,4,7,6,5,0,3,2,1))

#define LOAD(src) _mm256_loadu_si256((__m256i *)(src))
#define STORE(dest,src) _mm256_storeu_si256((__m256i *)(dest),src)

#define MASK1 _mm256_set_epi32(0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa)
#define MASK2 _mm256_set_epi32(0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc, 0xcccccccc)
#define MASK4 _mm256_set_epi32(0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0, 0xf0f0f0f0)
#define MASK32 _mm256_set_epi32(0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000, 0xffffffff, 0x00000000)
#define MASK64 _mm256_set_epi32(0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0x00000000, 0x00000000)

#define SWAPMOVE(a, b, mask, shift) \
{ \
	u256 T = AND(XOR(SHIFTL64(a, shift), b), mask); \
	b = XOR(b, T); \
    a = XOR(a, SHIFTR64(T, shift)); \
}

//Swap move for shifting by 64
#define SWAPMOVEBY64(a, b, mask) \
{ \
	u256 T = AND(XOR(_mm256_shuffle_epi8(a, _mm256_set_epi8(23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,31,30,29,28,27,26,25,24)), b), mask); \
	b = XOR(b, T); \
    a = XOR(a, _mm256_shuffle_epi8(T, _mm256_set_epi8(7,6,5,4,3,2,1,0,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8))); \
}

#endif
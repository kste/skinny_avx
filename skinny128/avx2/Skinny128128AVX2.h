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

#define SR1(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,31))
#define SR2(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,31,30))
#define SR3(x) _mm256_shuffle_epi8(x, _mm256_set_epi8(28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,31,30,29))

#define LOAD(src) _mm256_loadu_si256((__m256i *)(src))
#define STORE(dest,src) _mm256_storeu_si256((__m256i *)(dest),src)


#endif
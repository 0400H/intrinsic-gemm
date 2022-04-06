#ifndef INTRINSIC_AVX2_H
#define INTRINSIC_AVX2_H

#include "intrinsic_utils.h"

#include <emmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <smmintrin.h>
// #include <x86intrin.h>

// float32
// https://stackoverflow.com/questions/13219146/how-to-sum-m256-horizontally
static inline float _mm256_extract_add_ps(__m256 x) {
    // (x3+x7, x2+x6, x1+x5, x0+x4), cycle 6
    const __m128 x128 = _mm_add_ps(_mm256_extractf128_ps(x, 1), _mm256_castps256_ps128(x));
    // (_, _, x1+x3+x5+x7, x0+x2+x4+x6), cycle 4
    const __m128 x64 = _mm_add_ps(x128, _mm_movehl_ps(x128, x128));
    // (_, _, _, x0+x1+x2+x3+x4+x5+x6+x7), cycle 4
    const __m128 x32 = _mm_add_ss(x64, _mm_shuffle_ps(x64, x64, 0x55));
    // cycle 1
    return _mm_cvtss_f32(x32);
}

static inline float _mm256_permute_add_ps(__m256 x) {
    // (_, _, _, _, x7+x3, x6+x2, x5+x1, x4+x0), cycle 6
    x = _mm256_add_ps(x, _mm256_permute2f128_ps(x, x, 0x01));
    // (_, _, _, _, _, _, x7+x3+x6+x2, x5+x1+x4+x0), cycle undefined
    x = _mm256_hadd_ps(x, x);
    // (_, _, _, _, _, _, x7+x3+x6+x2+x5+x1+x4+x0), cycle undefined
    x = _mm256_hadd_ps(x, x);
    // cycle _mm256_cvtss_f32
    return _mm256_cvtss_f32(x);
}

// x8
static inline __m256i _mm256_sumepi32_epi32(__m256i x) {
    // (_, _, _, _, x7+x3, x6+x2, x5+x1, x4+x0), cycle 6
    x = _mm256_add_epi32(x, _mm256_permute2x128_si256(x, x, 0x81));
    // (_, _, _, _, _, _, x7+x3+x6+x2, x5+x1+x4+x0), cycle undefined
    x = _mm256_add_epi32(x, _mm256_srli_si256(x, 8));
    // (_, _, _, _, _, _, x7+x3+x6+x2+x5+x1+x4+x0), cycle undefined
    return _mm256_add_epi32(x, _mm256_srli_si256(x, 4));
}

static inline int _mm256_sumepi32_si32(__m256i x) {
    return _mm256_extract_epi32(_mm256_sumepi32_epi32(x), 0);
}

static inline __m256i _mm256_madd16epi8_epi32(
    __m256i vdst, __m256i va, __m256i vb, __m256i vc, const int8_t *b) {
    vb = _mm256_cvtepi8_epi16(_mm_loadu_si128((__m128i*) b));
    vc = _mm256_madd_epi16(va, vb);
    return _mm256_add_epi32(vc, vdst);
}

static inline __m256i _mm256_madd8epi8_epi32(
    __m256i vdst, __m256i va, __m256i vb, __m256i vc, const int8_t *b) {
    vb = _mm256_cvtepi8_epi32(_mm_loadl_epi64((__m128i*) b));
    vc = _mm256_mullo_epi32(va, vb);
    return _mm256_add_epi32(vc, vdst);
}

#endif // INTRINSIC_AVX2_H

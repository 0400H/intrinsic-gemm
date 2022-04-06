#ifndef SGEMM_X8S8S32_AVX2_H
#define SGEMM_X8S8S32_AVX2_H

#include "intrinsic_utils.h"

inline void block8x8_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc);

inline void block8x4_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int32_t stride);

inline void block8x1_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int32_t stride);

inline void block4x8_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int32_t stride);

inline void block4x4_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc);

inline void block4x2_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);

inline void block4x1_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);

inline void block2x8_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);
inline void block2x4_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);

inline void block2x2_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc);

inline void block2x1_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);

void block1x8_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);

void block1x4_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);

void block1x2_kernel_avx2(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);

void block1x1_kernel_avx2(const int32_t k, const int8_t* a, const int8_t* b, int* c);

inline void block4x2_kernel_avx2_me(
    const int32_t k, const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb, int* c, const int32_t ldc, const int stride);

inline void avx_s8s8s32_gemm_2x4_packed(
    const int32_t m, const int32_t n, const int32_t k,
    const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb,
    int32_t* c, const int32_t ldc);

void chgemm_c_c_n_t_avx2(
    const int32_t m, const int32_t n, const int32_t k,
    const int8_t* a, const int32_t lda,
    const int8_t* b, const int32_t ldb,
    int32_t* c, const int32_t ldc);

#endif //SGEMM_X8S8S32_AVX2_H
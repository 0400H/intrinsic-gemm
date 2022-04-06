#ifndef SGEMM_F32F32F32_AVX2_H
#define SGEMM_F32F32F32_AVX2_H

#include "intrinsic_utils.h"

void block1x1_kernel_avx2(const float* a, const float* b, float* c, const int32_t k);

void block1x2_kernel_avx2(const float* a, const int32_t lda,
                          const float* b, const int32_t ldb,
                          float* c, const int32_t ldc,
                          const int32_t stride, const int32_t k);

inline void block2x1_kernel_avx2(const float* a, const int32_t lda,
                                 const float* b, const int32_t ldb,
                                 float* c, const int32_t ldc,
                                 const int32_t stride, const int32_t k);

inline void block2x2_kernel_avx2(const float* a, const int32_t lda,
                                 const float* b, const int32_t ldb,
                                 float* c, const int32_t ldc, const int32_t k);

void block1x4_kernel_avx2(const float* a, const int32_t lda,
                          const float* b, const int32_t ldb,
                          float* c, const int32_t ldc,
                          const int32_t stride, const int32_t k);

void block4x1_kernel_avx2(const float* a, const int32_t lda,
                          const float* b, const int32_t ldb,
                          float* c, const int32_t ldc,
                          const int32_t stride, const int32_t k);

void sgemm_avx2(const int32_t m, const int32_t n, const int32_t k,
                const float* a, const int32_t lda,
                const float* b, const int32_t ldb,
                float* c, const int32_t ldc);

#endif // SGEMM_F32F32F32_AVX2_H

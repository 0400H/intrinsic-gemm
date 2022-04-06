#include "intrinsic_avx2.h"

void block1x1_kernel_avx2(const float* a, const float* b, float* c, const int32_t k) {
    // printf("block1x1_kernel_avx2\n");
    const float* pa0 = a;
    const float* pb0 = b;
    float* pc0 = c;

    size_t k_loop = k >> 3; // k / 8
    size_t k_leftover = k - (k_loop << 3); // k % 8

    __m256 ma0;
    __m256 mb0;
    __m256 sum0 = _mm256_setzero_ps();

    for (size_t k = 0; k < k_loop; ++k) {
        ma0 = _mm256_loadu_ps(pa0);
        mb0 = _mm256_loadu_ps(pb0);
        sum0 = _mm256_fmadd_ps(ma0, mb0, sum0);
        pa0 += 8;
        pb0 += 8;
    }

    size_t leftover = k_leftover & 0x07;

    if (leftover) {
        float ga0[8] __attribute__((aligned(32))) = {0, 0, 0, 0, 0, 0, 0, 0};
        float gb0[8] __attribute__((aligned(32))) = {0, 0, 0, 0, 0, 0, 0, 0};

        for (size_t i = 0; i < leftover; ++i) {
            ga0[i] = pa0[i];
            gb0[i] = pb0[i];
        }

        mb0 = _mm256_loadu_ps(gb0);
        ma0 = _mm256_loadu_ps(ga0);
        sum0 = _mm256_fmadd_ps(ma0, mb0, sum0);
    }

    pc0[0] = _mm256_extract_add_ps(sum0);
    // pc0[0] = _mm256_permute_add_ps(sum0);
}

void block1x2_kernel_avx2(const float* a, const int32_t lda,
                          const float* b, const int32_t ldb,
                          float* c, const int32_t ldc,
                          const int32_t stride, const int32_t k) {
}

inline void block2x1_kernel_avx2(const float* a, const int32_t lda,
                                 const float* b, const int32_t ldb,
                                 float* c, const int32_t ldc,
                                 const int32_t stride, const int32_t k) {
}

inline void block2x2_kernel_avx2(const float* a, const int32_t lda,
                                 const float* b, const int32_t ldb,
                                 float* c, const int32_t ldc, const int32_t k) {
}

void block1x4_kernel_avx2(const float* a, const int32_t lda,
                          const float* b, const int32_t ldb,
                          float* c, const int32_t ldc,
                          const int32_t stride, const int32_t k) {
}

void block4x1_kernel_avx2(const float* a, const int32_t lda,
                          const float* b, const int32_t ldb,
                          float* c, const int32_t ldc,
                          const int32_t stride, const int32_t k) {
}

void sgemm_avx2(const int32_t m, const int32_t n, const int32_t k,
                const float* a, const int32_t lda,
                const float* b, const int32_t ldb,
                float* c, const int32_t ldc) {
    for (auto m_idx = 0; m_idx < m; ++m_idx) {
        for (auto n_idx = 0; n_idx < n; ++n_idx) {
            block1x1_kernel_avx2(a + m_idx * lda,
                                 b + n_idx * ldb,
                                 c + m_idx * ldc + n_idx, k);
        }
    }
}

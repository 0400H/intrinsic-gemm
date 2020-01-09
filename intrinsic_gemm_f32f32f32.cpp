#include "intrinsic_gemm.hpp"

namespace IceSword {

#if defined(__AVX2__)

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

template <>
IceSwordStatus IntrinsicGemm< float, float, float >::init(
    const bool trans_a, const bool trans_b,
    const int m, const int n, const int k) {
    // CHECK_EQ(trans_a, false) << "only support no trans";
    // CHECK_EQ(trans_b, false) << "only support no trans";
    _lda = (!trans_a) ? k : m;
    _ldb = (!trans_b) ? k : n;
    _ldc = n;
    _m = m;
    _n = n;
    _k = k;
    _trans_a = trans_a ? 'T' : 'N';
    _trans_b = trans_b ? 'T' : 'N';
    return IceSwordSuccess;
}

template <>
IceSwordStatus IntrinsicGemm< float, float, float>::dispatch(
    const float alpha, const float beta,
    const float* ptr_a, const float* ptr_b, float* ptr_c) {
    sgemm_avx2(_m, _n, _k, ptr_a, _lda, ptr_b, _ldb, ptr_c, _ldc);
    return IceSwordSuccess;
}

#else

template <>
IceSwordStatus IntrinsicGemm< float, float, float >::init(
    const bool trans_a, const bool trans_b,
    const int m, const int n, const int k) {
    std::cout << __LINE__ << ": not impl!" << std::endl;
    return IceSwordUnImplError;
}

template <>
IceSwordStatus IntrinsicGemm< float, float, float >::dispatch(
    const float alpha, const float beta,
    const float* ptr_a, const float* ptr_b, float* ptr_c) {
    std::cout << __LINE__ << ": not impl!" << std::endl;
    return IceSwordUnImplError;
}

#endif

}
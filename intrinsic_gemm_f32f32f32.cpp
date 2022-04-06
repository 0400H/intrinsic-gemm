#include "intrinsic_gemm.hpp"
#include "sgemm_f32f32f32_avx2.h"

namespace IceSword {

#if defined(__AVX2__)

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
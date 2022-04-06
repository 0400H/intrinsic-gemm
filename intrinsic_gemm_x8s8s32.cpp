#include "intrinsic_gemm.hpp"
#include "chgemm_x8s8s32_avx2.h"

namespace IceSword {

#if defined(__AVX2__)

template <>
IceSwordStatus IntrinsicGemm< char, char, int >::init(
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
IceSwordStatus IntrinsicGemm< char, char, int>::dispatch(
    const float alpha, const float beta,
    const char* ptr_a, const  char* ptr_b, int* ptr_c) {
    // CHECK(ptr_a != nullptr);
    // CHECK(ptr_b != nullptr);
    // CHECK(ptr_c != nullptr);
    // std::cout<< "chgemm_c_c_n_t_avx2 dispatch";
    // std::cout<<_m<< ","<<_n<< ","<<_k<< ","<< ","<<_lda<< ","<< ","<<_ldb<< ","<<_ldc;
    chgemm_c_c_n_t_avx2(_m, _n, _k, (const int8_t*)ptr_a, _lda, (const int8_t*)ptr_b, _ldb, ptr_c, _ldc);
    // std::cout<< "chgemm_c_c_n_t_avx2 end";
    // avx_s8s8s32_gemm_2x4_packed(_m,_n,_k,ptr_a,_lda,ptr_b,_ldb,ptr_c,_ldc);
    // exit(0);
    return IceSwordSuccess;
}

#else

template <>
IceSwordStatus IntrinsicGemm< char, char, int >::init(
        const bool trans_a, const bool trans_b,
        const int m, const int n, const int k) {
    std::cout << "not impl" << std::endl;
    return IceSwordUnImplError;
}

template <>
IceSwordStatus IntrinsicGemm< char, char, int >::dispatch(
        const float alpha, const float beta,
        const  char* ptr_a, const  char* ptr_b, int* ptr_c) {
    std::cout << "not impl" << std::endl;
    return IceSwordUnImplError;
}

#endif

}

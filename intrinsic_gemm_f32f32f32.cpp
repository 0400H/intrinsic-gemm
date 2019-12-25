#include "intrinsic_gemm.hpp"

namespace IceSword {

#if defined(__AVX2__)

template <>
IceSwordStatus IntrinsicGemm< float, float, float >::init(
    const bool trans_a, const bool trans_b,
    const int m, const int n, const int k) {
    std::cout << "not impl" << std::endl;
    return IceSwordUnImplError;
}

template <>
IceSwordStatus IntrinsicGemm< float, float, float>::dispatch(
    const float alpha, const float beta,
    const float* ptr_a, const float* ptr_b, float* ptr_c) {
    std::cout << "not impl" << std::endl;
    return IceSwordUnImplError;
}

#else

template <>
IceSwordStatus IntrinsicGemm< float, float, float >::init(
        const bool trans_a, const bool trans_b,
        const int m, const int n, const int k) {
    std::cout << "not impl" << std::endl;
    return IceSwordUnImplError;
}

template <>
IceSwordStatus IntrinsicGemm< float, float, float >::dispatch(
        const float alpha, const float beta,
        const float* ptr_a, const float* ptr_b, float* ptr_c) {
    std::cout << "not impl" << std::endl;
    return IceSwordUnImplError;
}

#endif

}
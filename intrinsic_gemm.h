#ifndef INTRINSIC_GEMM_H
#define INTRINSIC_GEMM_H

#include "intrinsic_types.h"

namespace IceSword {

// using namespace IceSword;

template<typename inDtype_A,
         typename inDtype_B,
         typename outDtype>
class IntrinsicGemm {

public:
    IntrinsicGemm() = default;
    ~IntrinsicGemm() {}

    IceSwordStatus init(const bool trans_a, const bool trans_b,
                        const int m, const int n, const int k);

    IceSwordStatus dispatch(const float alpha, const float beta,
                            const inDtype_A* a, const inDtype_B* b,
                            outDtype* c);

private:
    int _m{-1};
    int _n{-1};
    int _k{-1};
    int _lda{-1};
    int _ldb{-1};
    int _ldc{-1};
    float _alpha{1.f};
    float _beta{0.f};
    char _trans_a{'N'};
    char _trans_b{'N'};
    char _offset_c_flag{'F'};
    int8_t _offset_a{0};
    int8_t _offset_b{0};
    int32_t _offset_c{0};
};

}

#endif //INTRINSIC_GEMM_H
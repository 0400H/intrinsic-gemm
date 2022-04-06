#ifndef INTRINSIC_GEMM_H
#define INTRINSIC_GEMM_H

#include "intrinsic_types.hpp"

extern "C" {

    void * get_instance(IceSword::DataType type_a, IceSword::DataType type_b, IceSword::DataType type_c);

    IceSword::IceSwordStatus instance_init(void* instance_ptr, const bool trans_a, const bool trans_b,
                                           const int m, const int n, const int k);

    IceSword::IceSwordStatus instance_dispatch(void* instance_ptr, const float alpha,
                                               const float beta, const void* a,
                                               const void* b, void* c);

}

#endif // INTRINSIC_GEMM_H
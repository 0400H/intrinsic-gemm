#include "gemm_intrinsic.hpp"
#include "gemm_intrinsic.h"

extern "C" {

    typedef struct {
        void * instance_ptr = nullptr;
        IceSword::DataType dtype_a;
        IceSword::DataType dtype_b;
        IceSword::DataType dtype_c;
    } InstanceHandle;

    void * get_instance(IceSword::DataType type_a, IceSword::DataType type_b, IceSword::DataType type_c) {
        InstanceHandle *instance_handle = new InstanceHandle;
        if (type_b == IceSword::DT_INT8) {
            if (type_a == IceSword::DT_INT8) {
                instance_handle->instance_ptr = reinterpret_cast<void *>(new IceSword::IntrinsicGemm<char, char, int>);
            } else {
            }
        } else if (type_b == IceSword::DT_FLOAT) {
            instance_handle->instance_ptr = reinterpret_cast<void *>(new IceSword::IntrinsicGemm<float, float, float>);
        }
        instance_handle->dtype_a = type_a;
        instance_handle->dtype_b = type_b;
        instance_handle->dtype_c = type_c;
        return (void *)instance_handle;
    }

    IceSword::S_Status instance_init(void* instance_handle, const bool trans_a, const bool trans_b,
                                           const int m, const int n, const int k) {
        auto handle = reinterpret_cast<InstanceHandle *>(instance_handle);
        auto status = IceSword::S_UnImplError;

        if (handle->dtype_b == IceSword::DT_INT8) {
            if (handle->dtype_a == IceSword::DT_INT8) {
                status = reinterpret_cast<IceSword::IntrinsicGemm<char, char, int> *>(handle->instance_ptr)->init(trans_a, trans_b, m, n, k);
            } else {
            }
        } else if (handle->dtype_b == IceSword::DT_FLOAT) {
            status = reinterpret_cast<IceSword::IntrinsicGemm<float, float, float> *>(handle->instance_ptr)->init(trans_a, trans_b, m, n, k);
        }
        return status;
    }

    IceSword::S_Status instance_dispatch(void* instance_handle, const float alpha,
                                               const float beta, const void* a,
                                               const void* b, void* c) {
        auto handle = reinterpret_cast<InstanceHandle *>(instance_handle);
        auto status = IceSword::S_UnImplError;

        if (handle->dtype_b == IceSword::DT_INT8) {
            if (handle->dtype_a == IceSword::DT_INT8) {
                auto a_ptr = reinterpret_cast<const char *>(a);
                auto b_ptr = reinterpret_cast<const char *>(b);
                auto c_ptr = reinterpret_cast<int *>(c);
                status = reinterpret_cast<IceSword::IntrinsicGemm<char, char, int> *>(handle->instance_ptr)->dispatch(alpha, beta, a_ptr, b_ptr, c_ptr);
            } else {
            }
        } else if (handle->dtype_b == IceSword::DT_FLOAT) {
            auto a_ptr = reinterpret_cast<const float *>(a);
            auto b_ptr = reinterpret_cast<const float *>(b);
            auto c_ptr = reinterpret_cast<float *>(c);
            status = reinterpret_cast<IceSword::IntrinsicGemm<float, float, float> *>(handle->instance_ptr)->dispatch(alpha, beta, a_ptr, b_ptr, c_ptr);
        }
        return status;
    }

}
#include "intrinsic_gemm.h"
#include <iostream>
#include <cmath>

using namespace IceSword;

template<typename matrix_type>
void fill_matrix(matrix_type matrix, size_t dim_x, size_t dim_y) {
    for (size_t x_idx = 0; x_idx < dim_x; ++x_idx) {
        for (size_t y_idx = 0; y_idx < dim_y; ++y_idx) {
            auto matrix_idx = x_idx * dim_y + y_idx;
            matrix[matrix_idx] = matrix_idx;
        }
    }
}

template<typename matrix_type>
void print_matrix(matrix_type matrix, size_t dim_x, size_t dim_y) {
    for (size_t x_idx = 0; x_idx < dim_x; ++x_idx) {
        for (size_t y_idx = 0; y_idx < dim_y; ++y_idx) {
            auto matrix_idx = x_idx * dim_y + y_idx;
            std::cout << float(matrix[matrix_idx]) << ",";
        }
        std::cout << std::endl;
    }
}

template<typename matrix_type_a,
         typename matrix_type_b,
         typename matrix_type_c>
void gemm_reference(matrix_type_a matrix_a, matrix_type_b matrix_b,
                    matrix_type_c matrix_c, size_t m, size_t n, size_t k) {
    for (size_t m_idx = 0; m_idx < m; ++m_idx) {
        for (size_t n_idx = 0; n_idx < n; ++n_idx) {
            for (size_t k_idx = 0; k_idx < k; ++k_idx) {
                matrix_c[m_idx*n+n_idx] += matrix_a[m_idx*k+k_idx] * matrix_b[n_idx*k+k_idx];
            }
        }
    }
}

template<typename matrix_type>
float cout_error_num(matrix_type matrix_a, matrix_type matrix_b,
                     size_t length, float loss_rate=1e-4, bool debug=false) {
    size_t error_num = 0;
    float zero_offset = 1e-10;
    for (size_t idx = 0; idx < length; ++idx) {
        auto cur_loss_rate = abs(float(matrix_b[idx] - matrix_a[idx]))
                           / (abs(float(matrix_a[idx]))+zero_offset);
        if (cur_loss_rate > loss_rate) {
            ++error_num;
        }
        if (debug == true) {
            std::cout << idx << ","
                      << float(matrix_a[idx]) << ","
                      << float(matrix_b[idx]) << std::endl;
        }
    }
    return error_num;
}

template<DataType dtype_a,
         DataType dtype_b,
         DataType dtype_c>
void test_func(size_t m, size_t n, size_t k, bool debug=false, float loss_rate=1e-4) {
    typedef typename DataTrait<dtype_a>::Dtype AType;
    typedef typename DataTrait<dtype_b>::Dtype BType;
    typedef typename DataTrait<dtype_c>::Dtype CType;

    auto matrix_a = reinterpret_cast<AType *>(calloc(m*k, sizeof(AType)));
    auto matrix_b = reinterpret_cast<BType *>(calloc(n*k, sizeof(BType)));
    auto matrix_c = reinterpret_cast<CType *>(calloc(m*n, sizeof(CType)));
    auto matrix_c_ref = reinterpret_cast<CType *>(calloc(m*n, sizeof(CType)));

    fill_matrix(matrix_a, m, k);
    fill_matrix(matrix_b, n, k);

    auto gemm_engine = get_instance(dtype_a, dtype_b, dtype_c);
    auto status = instance_init(gemm_engine, false, false, m, n, k);
    if (status != IceSwordSuccess) {
        std::cout << "init failed!\n" << std::endl;
    } else {
        status = instance_dispatch(gemm_engine, 1.0, 1.0, matrix_a, matrix_b, matrix_c);
        if (status != IceSwordSuccess) {
            std::cout << "dispatch failed!\n" << std::endl;
        } else {
            gemm_reference(matrix_a, matrix_b, matrix_c_ref, m, n, k);
            auto error_num = cout_error_num(matrix_c_ref, matrix_c, m*n, loss_rate, debug);
            std::cout << "error_num: " << error_num << "\n" << std::endl;
        }
    }
}

int main(int argc, const char** argv) {
    // https://stackoverflow.com/questions/28939652/how-to-detect-sse-sse2-avx-avx2-avx-512-avx-128-fma-kcvi-availability-at-compile
    #if defined(__AVX2__)
        std::cout << "AVX256 enabled!\n" << std::endl;

        test_func<DT_UINT8, DT_INT8, DT_INT32>(47, 47, 17);
        test_func<DT_INT8, DT_INT8, DT_INT32>(47, 47, 17);
        test_func<DT_FLOAT, DT_FLOAT, DT_FLOAT>(47, 47, 17, false);

    #else
        std::cout << "AVX256 disabled!\n" << std::endl;
    #endif
    return 0;
}

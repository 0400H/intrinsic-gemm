#include "intrinsic_gemm.h"
#include <iostream>
#include<cmath>

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

template<typename matrix_type_a,typename matrix_type_b,typename matrix_type_c>
void gemm_reference(matrix_type_a matrix_a, matrix_type_b matrix_ab,
                    matrix_type_c matrix_c, size_t m, size_t n, size_t k) {
    for (size_t m_idx = 0; m_idx < m; ++m_idx) {
        for (size_t n_idx = 0; n_idx < n; ++n_idx) {
            for (size_t k_idx = 0; k_idx < k; ++k_idx) {
                matrix_c[m_idx*n+n_idx] += matrix_a[m_idx*k+k_idx] * matrix_a[n_idx*k+k_idx];
            }
        }
    }
}

template<typename matrix_type>
float cout_diff(matrix_type matrix_a, matrix_type matrix_b, size_t length, float loss_rate=1e-4, bool debug=false) {
    size_t diff_num = 0;
    float zero_offset = 1e-10;
    for (size_t idx = 0; idx < length; ++idx) {
        auto cur_loss_rate = (abs(float(matrix_b[idx] - matrix_a[idx]))+zero_offset)
                           / (abs(float(matrix_a[idx]))+zero_offset);
        if (cur_loss_rate > loss_rate) {
            ++diff_num;
        }
        if (debug == true) {
            std::cout << idx << ","
                      << float(matrix_a[idx]) << ","
                      << float(matrix_b[idx]) << std::endl;
        }
    }
    return float(diff_num) / float(length);
}

int main(int argc, const char** argv) {
    // https://stackoverflow.com/questions/28939652/how-to-detect-sse-sse2-avx-avx2-avx-512-avx-128-fma-kcvi-availability-at-compile
    #if defined(__AVX2__)
        std::cout << "AVX256 enabled" << std::endl;

        const int m {47}, n{47}, k{32768};
        auto matrix_a = reinterpret_cast<char *>(calloc(m*k, sizeof(char)));
        auto matrix_b = reinterpret_cast<char *>(calloc(n*k, sizeof(char)));
        auto matrix_c = reinterpret_cast<int *>(calloc(m*n, sizeof(int)));
        auto matrix_c_ref = reinterpret_cast<int *>(calloc(m*n, sizeof(int)));

        fill_matrix(matrix_a, m, k);
        fill_matrix(matrix_b, n, k);

        auto gemm_engine = get_instance(DT_INT8, DT_INT8, DT_INT8);
        auto status = instance_init(gemm_engine, false, false, m, n, k);
        if (status != IceSwordSuccess) {
            std::cout << "init failed!" << std::endl;
        }
        status = instance_dispatch(gemm_engine, 1.0, 1.0, matrix_a, matrix_b, matrix_c);
        if (status != IceSwordSuccess) {
            std::cout << "dispatch failed!" << std::endl;
        }
        gemm_reference(matrix_a, matrix_b, matrix_c_ref, m, n, k);

        auto error_rate = cout_diff(matrix_c_ref, matrix_c, m*n, 1e-4, false);
        std::cout << "error_rate: " << error_rate << std::endl;

    #else
        std::cout << "AVX256 disabled" << std::endl;
    #endif
    return 0;
}

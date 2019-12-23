#include "intrinsic_gemm.h"
#include <iostream>

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
            std::cout << matrix[matrix_idx] << ",";
        }
        std::cout << std::endl;
    }
}

template<typename matrix_type_a,typename matrix_type_b,typename matrix_type_c>
void gemm_test(matrix_type_a matrix_a, matrix_type_b matrix_ab, matrix_type_c matrix_c,
               size_t m, size_t n, size_t k) {
    for (size_t m_idx = 0; m_idx < m; ++m_idx) {
        for (size_t n_idx = 0; n_idx < n; ++n_idx) {
            for (size_t k_idx = 0; k_idx < k; ++k_idx) {
                matrix_c[m_idx*m+n_idx] += matrix_a[m_idx*k+k_idx] * matrix_a[n_idx*k+k_idx];
            }
        }
    }
}

template<typename matrix_type>
float cout_diff(matrix_type matrix_a, matrix_type matrix_b, size_t length, float loss_rate=1e-4) {
    size_t diff_num = 0;
    for (size_t idx = 0; idx < length; ++idx) {
        auto cur_loss_rate = float(matrix_b[idx] - matrix_a[idx]) / float(length);
        if (cur_loss_rate > loss_rate) {
            ++diff_num;
        }
    }
    return float(diff_num) / float(length);
}

int main(int argc, const char** argv) {
    // https://stackoverflow.com/questions/28939652/how-to-detect-sse-sse2-avx-avx2-avx-512-avx-128-fma-kcvi-availability-at-compile
    #if defined(__AVX2__)
        std::cout << "AVX256 enabled" << std::endl;

        const int m {2}, n{2}, k{3};
        auto gemm_engine = get_instance();
        auto status = instance_init(gemm_engine, false, false, m, n, k);

        auto matrix_a = reinterpret_cast<char *>(calloc(m*k, sizeof(char)));
        auto matrix_b = reinterpret_cast<char *>(calloc(n*k, sizeof(char)));
        auto matrix_c = reinterpret_cast<int *>(calloc(m*n, sizeof(char)));
        auto matrix_c_ref = reinterpret_cast<int *>(calloc(m*n, sizeof(int)));

        fill_matrix(matrix_a, m, k);
        fill_matrix(matrix_b, n, k);

        status = instance_dispatch(gemm_engine, 1.0, 1.0, matrix_a, matrix_b, matrix_c);
        gemm_test(matrix_a, matrix_b, matrix_c_ref, m, n, k);

        auto error_rate = cout_diff(matrix_c_ref, matrix_c, m*n);
        std::cout << "error_rate: " << error_rate << std::endl;

        print_matrix(matrix_c_ref, m, n);
    #else
        std::cout << "AVX256 disabled" << std::endl;
    #endif
    return 0;
}
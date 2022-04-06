import unittest
import ctypes
import time
import numpy as np
import numba as nb

@nb.jit(nopython=True)
def cout_error(matrix_a, matrix_b, length, debug=False, loss_rate=1e-4):
    error_num = 0
    zero_offset = 1e-10
    for idx in range(length):
        cur_loss_rate = abs(float(matrix_b[idx] - matrix_a[idx])) \
                      / (abs(float(matrix_a[idx]))+zero_offset)
        if cur_loss_rate > loss_rate:
            error_num += 1
        if debug == True:
            print(idx, matrix_a[idx], matrix_b[idx])
    return error_num

@nb.jit(nopython=True)
def gemm_reference(matrix_a, matrix_b, matrix_c, m, n, k):
    for m_idx in range(m):
        for n_idx in range(n):
            for k_idx in range(k):
                matrix_c[m_idx*n+n_idx] += matrix_a[m_idx*k+k_idx] * matrix_b[n_idx*k+k_idx]
    return None

def intrinsic_gemm_impl(matrix_a, matrix_b, matrix_c, m, n, k, type_a, type_b, type_c):
    type_dict = {"<class 'numpy.float32'>": 1,
                 "<class 'numpy.int8'>": 3,
                 "<class 'numpy.int32'>": 5,
                 "<class 'numpy.uint8'>": 7}

    get_instance_func = intrinsic_gemm_so.get_instance
    get_instance_func.argtypes = (ctypes.c_int, ctypes.c_int, ctypes.c_int)
    get_instance_func.restype = ctypes.c_void_p

    instance_init_func = intrinsic_gemm_so.instance_init
    instance_init_func.argtypes = (ctypes.c_void_p, ctypes.c_bool, ctypes.c_bool,
                                   ctypes.c_int, ctypes.c_int, ctypes.c_int)
    instance_init_func.restype = ctypes.c_int

    instance_dispatch_func = intrinsic_gemm_so.instance_dispatch
    instance_dispatch_func.argtypes = (ctypes.c_void_p, ctypes.c_float, ctypes.c_float,
                                       ctypes.c_void_p, ctypes.c_void_p, ctypes.c_void_p)
    instance_dispatch_func.restype = ctypes.c_int

    intrinnsic_gemm_engine = get_instance_func(type_dict[str(type_a)], type_dict[str(type_b)], type_dict[str(type_c)])
    status = instance_init_func(intrinnsic_gemm_engine, False, False, m, n, k)
    t = time.time()
    status = instance_dispatch_func(intrinnsic_gemm_engine, 1.0, 1.0,
                                    matrix_a.ctypes.data_as(ctypes.c_void_p),
                                    matrix_b.ctypes.data_as(ctypes.c_void_p),
                                    matrix_c.ctypes.data_as(ctypes.c_void_p))
    t = time.time() - t
    return t, status

def test_error_num(m, n, k, a_type, b_type, c_type, debug=False, loss_rate=1e-4):
    if debug == True:
        matrix_a = np.ones(m*k, dtype=a_type)
        matrix_b = np.ones(n*k, dtype=b_type)
    else:
        matrix_a = np.arange(0, m*k, 1, dtype=a_type)
        matrix_b = np.arange(0, n*k, 1, dtype=b_type)
    matrix_c= np.zeros(shape=(m*n), dtype=c_type)
    matrix_c_ref = np.zeros(shape=(m*n), dtype=np.float)

    t1, _ = intrinsic_gemm_impl(matrix_a, matrix_b, matrix_c, m, n, k, a_type, b_type, c_type)

    matrix_a_ref = matrix_a.copy().astype(np.float)
    matrix_b_ref = matrix_b.copy().astype(np.float)
    t2 = time.time()
    gemm_reference(matrix_a_ref, matrix_b_ref, matrix_c_ref, m, n, k)
    t2 = time.time() - t2

    print("time: intrinsic_gemm {}, gemm_reference {}, scale {}".format(t1*1000, t2*1000, t2/t1))

    error_num = cout_error(matrix_c_ref, matrix_c, m*n, debug, loss_rate)
    return error_num

class intrinsic_gemm_unit_test(unittest.TestCase):
    def setUp(self):
        self.loss_rate = 1e-3
        self.test_m = [1, 2, 4, 8, 16, 32, 64]
        self.test_n = [1, 2, 4, 8, 16, 32, 64]
        self.test_k = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536]

    def test_s8s8s32(self):
        for m in self.test_m:
            for n in self.test_n:
                for k in self.test_k:
                    error_num = test_error_num(m, n, k, np.int8, np.int8, np.int32, False, self.loss_rate)
                    msg = 's8s8s32, m:{}, n:{}, k:{}, error_num: {}'.format(m, n, k, error_num)
                    print(msg)
                    self.assertEqual(0, error_num, msg)

    def test_f32f32f32(self):
        for m in self.test_m:
            for n in self.test_n:
                for k in self.test_k:
                    error_num = test_error_num(m, n, k, np.float32, np.float32, np.float32, False, self.loss_rate)
                    msg = 'f32f32f32, m:{}, n:{}, k:{}, error_num: {}'.format(m, n, k, error_num)
                    print(msg)
                    self.assertEqual(0, error_num, msg)
    pass

if __name__ == '__main__':
    intrinsic_gemm_so = ctypes.CDLL('./libintrinsic_gemm.so')
    unittest.main()

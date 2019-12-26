make clean
make all
export LD_LIBRARY_PATH=${pwd}:$LD_LIBRARY_PATH
./intrinsic_gemm_test.out
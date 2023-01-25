clear && clear

make clean
make all -nlinuxli
make all -j `nproc`

LD_LIBRARY_PATH=${pwd}:$LD_LIBRARY_PATH ./gemm_test.out

taskset -c 1-1 python3 -u ./gemm_test.py

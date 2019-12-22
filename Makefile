CC = /usr/bin/gcc
C++ = /usr/bin/g++

MAKEFILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR = $(dir $(MAKEFILE_PATH))

$(info $(MAKEFILE_DIR))

intrinsic_gemm.i:intrinsic_gemm.cpp
	@$(CC) -o intrinsic_gemm.i -E intrinsic_gemm.cpp -I $(MAKEFILE_DIR) -std=c++11 -O3 -Wall
intrinsic_gemm.s:intrinsic_gemm.i
	@$(C++) -o intrinsic_gemm.s -S intrinsic_gemm.i -fverbose-asm -std=c++11 -O3 -Wall
intrinsic_gemm.o:intrinsic_gemm.s
	@$(CC) -o intrinsic_gemm.o -c intrinsic_gemm.s
libintrinsic_gemm.a:intrinsic_gemm.o
	ar rcs libintrinsic_gemm.a intrinsic_gemm.o
libintrinsic_gemm.so:intrinsic_gemm.s
	@$(CC) -o libintrinsic_gemm.so -c intrinsic_gemm.s -shared -fPIC -std=c++11 -Wall
intrinsic_gemm_test:libintrinsic_gemm.so	libintrinsic_gemm.a
	@$(C++) -o intrinsic_gemm_test intrinsic_gemm_test.cpp -L $(MAKEFILE_DIR) -l intrinsic_gemm -std=c++11 -march=native -Wall

all:intrinsic_gemm_test
clean:
	rm -rf *.i *.s *.o *.a *.so intrinsic_gemm_test
run:
	./intrinsic_gemm_test
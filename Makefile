CC = /usr/bin/gcc
C++ = /usr/bin/g++

MAKEFILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR = $(dir $(MAKEFILE_PATH))

$(info $(MAKEFILE_DIR))

intrinsic_gemm.i:intrinsic_gemm.cpp
	@$(C++) -o intrinsic_gemm.i -E intrinsic_gemm.cpp -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall
intrinsic_gemm.s:intrinsic_gemm.i
	@$(C++) -o intrinsic_gemm.s -S intrinsic_gemm.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall
intrinsic_gemm.o:intrinsic_gemm.s
	@$(C++) -o intrinsic_gemm.o -c intrinsic_gemm.s -std=c++11 -O0 -Wall
libintrinsic_gemm.so:intrinsic_gemm.o
	@$(C++) -o libintrinsic_gemm.so intrinsic_gemm.o -shared -std=c++11 -O0 -Wall
# libintrinsic_gemm.so:intrinsic_gemm.cpp
# 	@$(C++) -o libintrinsic_gemm.so intrinsic_gemm.cpp -shared -fPIC -std=c++11 -march=native -O0 -Wall
libintrinsic_gemm.a:intrinsic_gemm.o
	ar rcs libintrinsic_gemm.a intrinsic_gemm.o
intrinsic_gemm_test:libintrinsic_gemm.so	libintrinsic_gemm.a
	@$(C++) -o intrinsic_gemm_test intrinsic_gemm_test.cpp -L $(MAKEFILE_DIR) -l intrinsic_gemm -std=c++11 -march=native -Wall

clean:
	rm -rf *.i *.s *.o *.a *.so intrinsic_gemm_test
all:intrinsic_gemm_test

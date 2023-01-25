CC = /usr/bin/gcc
C++ = /usr/bin/g++

MAKEFILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR = $(dir $(MAKEFILE_PATH))

$(info $(MAKEFILE_DIR))

chgemm_x8s8s32_avx2.i:chgemm_x8s8s32_avx2.c
	@$(C++) -o chgemm_x8s8s32_avx2.i -E chgemm_x8s8s32_avx2.c -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall -g0
chgemm_x8s8s32_avx2.s:chgemm_x8s8s32_avx2.i
	@$(C++) -o chgemm_x8s8s32_avx2.s -S chgemm_x8s8s32_avx2.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall -g3
chgemm_x8s8s32_avx2.o:chgemm_x8s8s32_avx2.s
	@$(C++) -o chgemm_x8s8s32_avx2.o -c chgemm_x8s8s32_avx2.s -std=c++11 -O0 -Wall -g3

sgemm_f32f32f32_avx2.i:sgemm_f32f32f32_avx2.c
	@$(C++) -o sgemm_f32f32f32_avx2.i -E sgemm_f32f32f32_avx2.c -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall -g0
sgemm_f32f32f32_avx2.s:sgemm_f32f32f32_avx2.i
	@$(C++) -o sgemm_f32f32f32_avx2.s -S sgemm_f32f32f32_avx2.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall -g3
sgemm_f32f32f32_avx2.o:sgemm_f32f32f32_avx2.s
	@$(C++) -o sgemm_f32f32f32_avx2.o -c sgemm_f32f32f32_avx2.s -std=c++11 -O0 -Wall -g3

gemm_intrinsic.i:gemm_intrinsic.c
	@$(C++) -o gemm_intrinsic.i -E gemm_intrinsic.c -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall -g0
gemm_intrinsic.s:gemm_intrinsic.i
	@$(C++) -o gemm_intrinsic.s -S gemm_intrinsic.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall -g3
gemm_intrinsic.o:gemm_intrinsic.s
	@$(C++) -o gemm_intrinsic.o -c gemm_intrinsic.s -std=c++11 -O0 -Wall -g3

gemm_intrinsic_x8s8s32.i:gemm_intrinsic_x8s8s32.cpp
	@$(C++) -o gemm_intrinsic_x8s8s32.i -E gemm_intrinsic_x8s8s32.cpp -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall -g0
gemm_intrinsic_x8s8s32.s:gemm_intrinsic_x8s8s32.i
	@$(C++) -o gemm_intrinsic_x8s8s32.s -S gemm_intrinsic_x8s8s32.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall -g3
gemm_intrinsic_x8s8s32.o:gemm_intrinsic_x8s8s32.s
	@$(C++) -o gemm_intrinsic_x8s8s32.o -c gemm_intrinsic_x8s8s32.s -std=c++11 -O0 -Wall -g3

gemm_intrinsic_f32f32f32.i:gemm_intrinsic_f32f32f32.cpp
	@$(C++) -o gemm_intrinsic_f32f32f32.i -E gemm_intrinsic_f32f32f32.cpp -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall -g0
gemm_intrinsic_f32f32f32.s:gemm_intrinsic_f32f32f32.i
	@$(C++) -o gemm_intrinsic_f32f32f32.s -S gemm_intrinsic_f32f32f32.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall -g3
gemm_intrinsic_f32f32f32.o:gemm_intrinsic_f32f32f32.s
	@$(C++) -o gemm_intrinsic_f32f32f32.o -c gemm_intrinsic_f32f32f32.s -std=c++11 -O0 -Wall -g3

libgemm_intrinsic.a:chgemm_x8s8s32_avx2.o sgemm_f32f32f32_avx2.o gemm_intrinsic_x8s8s32.o gemm_intrinsic_f32f32f32.o gemm_intrinsic.o
	ar rcs libgemm_intrinsic.a *.o

libgemm_intrinsic.so:chgemm_x8s8s32_avx2.o sgemm_f32f32f32_avx2.o gemm_intrinsic_x8s8s32.o gemm_intrinsic_f32f32f32.o gemm_intrinsic.o
	@$(C++) -o libgemm_intrinsic.so *.o -shared -std=c++11 -O0 -Wall

gemm_test.out:libgemm_intrinsic.so libgemm_intrinsic.a
	@$(C++) -o gemm_test.out gemm_test.cpp -I $(MAKEFILE_DIR) -L $(MAKEFILE_DIR) -l gemm_intrinsic -std=c++11 -march=native -Wall -g3

clean:
	rm -rf *.i *.ii *.s *.o *.a *.so *.out

all:gemm_test.out

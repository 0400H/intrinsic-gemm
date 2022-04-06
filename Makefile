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

intrinsic_gemm.i:intrinsic_gemm.c
	@$(C++) -o intrinsic_gemm.i -E intrinsic_gemm.c -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall -g0
intrinsic_gemm.s:intrinsic_gemm.i
	@$(C++) -o intrinsic_gemm.s -S intrinsic_gemm.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall -g3
intrinsic_gemm.o:intrinsic_gemm.s
	@$(C++) -o intrinsic_gemm.o -c intrinsic_gemm.s -std=c++11 -O0 -Wall -g3

intrinsic_gemm_x8s8s32.i:intrinsic_gemm_x8s8s32.cpp
	@$(C++) -o intrinsic_gemm_x8s8s32.i -E intrinsic_gemm_x8s8s32.cpp -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall -g0
intrinsic_gemm_x8s8s32.s:intrinsic_gemm_x8s8s32.i
	@$(C++) -o intrinsic_gemm_x8s8s32.s -S intrinsic_gemm_x8s8s32.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall -g3
intrinsic_gemm_x8s8s32.o:intrinsic_gemm_x8s8s32.s
	@$(C++) -o intrinsic_gemm_x8s8s32.o -c intrinsic_gemm_x8s8s32.s -std=c++11 -O0 -Wall -g3

intrinsic_gemm_f32f32f32.i:intrinsic_gemm_f32f32f32.cpp
	@$(C++) -o intrinsic_gemm_f32f32f32.i -E intrinsic_gemm_f32f32f32.cpp -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall -g0
intrinsic_gemm_f32f32f32.s:intrinsic_gemm_f32f32f32.i
	@$(C++) -o intrinsic_gemm_f32f32f32.s -S intrinsic_gemm_f32f32f32.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall -g3
intrinsic_gemm_f32f32f32.o:intrinsic_gemm_f32f32f32.s
	@$(C++) -o intrinsic_gemm_f32f32f32.o -c intrinsic_gemm_f32f32f32.s -std=c++11 -O0 -Wall -g3

libintrinsic_gemm.a:chgemm_x8s8s32_avx2.o sgemm_f32f32f32_avx2.o intrinsic_gemm_x8s8s32.o intrinsic_gemm_f32f32f32.o intrinsic_gemm.o
	ar rcs libintrinsic_gemm.a *.o

libintrinsic_gemm.so:chgemm_x8s8s32_avx2.o sgemm_f32f32f32_avx2.o intrinsic_gemm_x8s8s32.o intrinsic_gemm_f32f32f32.o intrinsic_gemm.o
	@$(C++) -o libintrinsic_gemm.so *.o -shared -std=c++11 -O0 -Wall

gemm_test.out:libintrinsic_gemm.so libintrinsic_gemm.a
	@$(C++) -o gemm_test.out gemm_test.cpp -I $(MAKEFILE_DIR) -L $(MAKEFILE_DIR) -l intrinsic_gemm -std=c++11 -march=native -Wall -g3

clean:
	rm -rf *.i *.ii *.s *.o *.a *.so *.out

all:gemm_test.out

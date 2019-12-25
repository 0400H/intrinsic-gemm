CC = /usr/bin/gcc
C++ = /usr/bin/g++

MAKEFILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR = $(dir $(MAKEFILE_PATH))

$(info $(MAKEFILE_DIR))

intrinsic_gemm_x8s8s32.i:intrinsic_gemm_x8s8s32.cpp
	@$(C++) -o intrinsic_gemm_x8s8s32.i -E intrinsic_gemm_x8s8s32.cpp -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall
intrinsic_gemm_x8s8s32.s:intrinsic_gemm_x8s8s32.i
	@$(C++) -o intrinsic_gemm_x8s8s32.s -S intrinsic_gemm_x8s8s32.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall
intrinsic_gemm_x8s8s32.o:intrinsic_gemm_x8s8s32.s
	@$(C++) -o intrinsic_gemm_x8s8s32.o -c intrinsic_gemm_x8s8s32.s -std=c++11 -O0 -Wall

intrinsic_gemm_f32f32f32.i:intrinsic_gemm_f32f32f32.cpp
	@$(C++) -o intrinsic_gemm_f32f32f32.i -E intrinsic_gemm_f32f32f32.cpp -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall
intrinsic_gemm_f32f32f32.s:intrinsic_gemm_f32f32f32.i
	@$(C++) -o intrinsic_gemm_f32f32f32.s -S intrinsic_gemm_f32f32f32.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall
intrinsic_gemm_f32f32f32.o:intrinsic_gemm_f32f32f32.s
	@$(C++) -o intrinsic_gemm_f32f32f32.o -c intrinsic_gemm_f32f32f32.s -std=c++11 -O0 -Wall

intrinsic_gemm.i:intrinsic_gemm.c
	@$(C++) -o intrinsic_gemm.i -E intrinsic_gemm.c -I $(MAKEFILE_DIR) -std=c++11 -march=native -O0 -Wall
intrinsic_gemm.s:intrinsic_gemm.i
	@$(C++) -o intrinsic_gemm.s -S intrinsic_gemm.i -fverbose-asm -fPIC -std=c++11 -march=native -O0 -Wall
intrinsic_gemm.o:intrinsic_gemm.s
	@$(C++) -o intrinsic_gemm.o -c intrinsic_gemm.s -std=c++11 -O0 -Wall

libintrinsic_gemm.a:intrinsic_gemm.o intrinsic_gemm_x8s8s32.o intrinsic_gemm_f32f32f32.o
	ar rcs libintrinsic_gemm.a intrinsic_gemm.o intrinsic_gemm_x8s8s32.o intrinsic_gemm_f32f32f32.o

libintrinsic_gemm.so:intrinsic_gemm.o intrinsic_gemm_x8s8s32.o intrinsic_gemm_f32f32f32.o
	@$(C++) -o libintrinsic_gemm.so intrinsic_gemm.o intrinsic_gemm_x8s8s32.o intrinsic_gemm_f32f32f32.o -shared -std=c++11 -O0 -Wall
libintrinsic_gemm.so:intrinsic_gemm.c intrinsic_gemm_x8s8s32.cpp intrinsic_gemm_f32f32f32.cpp
	@$(C++) -o libintrinsic_gemm.so intrinsic_gemm.c intrinsic_gemm_x8s8s32.cpp intrinsic_gemm_f32f32f32.cpp -shared -fPIC -std=c++11 -march=native -O0 -Wall

intrinsic_gemm_test:libintrinsic_gemm.so libintrinsic_gemm.a
	@$(C++) -o intrinsic_gemm_test intrinsic_gemm_test.cpp -L $(MAKEFILE_DIR) -l intrinsic_gemm -std=c++11 -march=native -Wall

clean:
	rm -rf *.i *.s *.o *.a *.so intrinsic_gemm_test
all:intrinsic_gemm_test
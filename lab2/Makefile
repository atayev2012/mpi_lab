CC=gcc
CCOMP=gcc
CFLAGS_OMP=-fopenmp -Wall -Wextra -Werror -lm

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
    # macOS specific settings
	# need to be installed via brew: llvm libomp
    CCOMP=clang
	CFLAGS_OMP=-Xpreprocessor -fopenmp -L/opt/homebrew/opt/llvm/lib -I/opt/homebrew/opt/llvm/include -I/opt/homebrew/Cellar/libomp/21.1.8/include -lm -lomp
endif

CFLAGS_PTHREAD=-Wall -Wextra -Werror -pthread -lm

TASK1_SRC=./task_1/mandelbrot.c
TASK1_TARGET=./build/task1_mandelbrot

TASK2_SRC_OMP=./task_2/nbody_omp.c
TASK2_TARGET_OMP=./build/task2_nbody_omp

TASK3_SRC=./task_3/pth_ll_rwl.c ./task_3/my_rand.c ./task_3/rwl.c
TASK3_MY_FLAGS=-DCUSTOM_RWLOCK
TASK3_ORIG_TARGET=./build/task3_orig_rwl
TASK3_MY_TARGET=./build/task3_my_rwl

all: create_build_dir build_task1 build_task2_omp build_task3_orig build_task3_my

build_task1: $(TASK1_SRC)
	$(CCOMP) $(CFLAGS_OMP) $(TASK1_SRC) -o $(TASK1_TARGET)

build_task2_omp: $(TASK2_SRC_OMP)
	$(CCOMP) $(CFLAGS_OMP) $(TASK2_SRC_OMP) -o $(TASK2_TARGET_OMP)

build_task3_orig: $(TASK3_SRC)
	$(CC) $(CFLAGS_PTHREAD) $(TASK3_SRC) -o $(TASK3_ORIG_TARGET)

build_task3_my: $(TASK3_SRC)
	$(CC) $(CFLAGS_PTHREAD) $(TASK3_MY_FLAGS) $(TASK3_SRC) -o $(TASK3_MY_TARGET)

create_build_dir:
	mkdir -p build

clean:
	rm -rf build

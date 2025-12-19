CC=gcc
CFLAGS=-Wall -Wextra -Werror -pthread -lm

TASK3_SRC=./task_3/pth_ll_rwl.c ./task_3/my_rand.c ./task_3/rwl.c
TASK3_MY_FLAGS=-DCUSTOM_RWLOCK
TASK3_ORIG_TARGET=./build/task3_orig_rwl
TASK3_MY_TARGET=./build/task3_my_rwl

all: create_build_dir build_task3_orig build_task3_my

build_task3_orig: $(TASK3_SRC)
	$(CC) $(CFLAGS) $(TASK3_SRC) -o $(TASK3_ORIG_TARGET)

build_task3_my: $(TASK3_SRC)
	$(CC) $(CFLAGS) $(TASK3_MY_FLAGS) $(TASK3_SRC) -o $(TASK3_MY_TARGET)

create_build_dir:
	mkdir -p build

clean:
	rm -rf build

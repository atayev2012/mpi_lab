CC = mpicc

SOURCES = timer.c

TARGET_TASK_1 = ./build/task1
SRC_TASK_1 = ./task_1/task1.c

TARGET_TASK_2_ROWS = ./build/task2_rows
SRC_TASK_2_ROWS = ./task_2/task2_rows.c ./task_2/task2_common.c 

TARGET_TASK_3 = ./build/task3_canon
SRC_TASK_3 = ./task_3/canon.c

all: create_build_dir build_task_1 build_task_2_rows build_task_3

build_task_1: $(SRC_TASK_1) ${SOURCES}
	${CC} $(SRC_TASK_1) $(SOURCES) -o $(TARGET_TASK_1)

build_task_2_rows: $(SRC_TASK_2_ROWS) ${SOURCES}
	${CC} $(SRC_TASK_2_ROWS) $(SOURCES) -o $(TARGET_TASK_2_ROWS)

build_task_3: $(SRC_TASK_3) ${SOURCES}
	${CC} $(SRC_TASK_3) $(SOURCES) -o $(TARGET_TASK_3) -lm

create_build_dir:
	mkdir -p build
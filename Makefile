CC = mpicc

SOURCES = timer.c

TARGET_TASK_1 = ./build/task1
SRC_TASK_1 = ./task_1/main.c

TARGET_TASK_2_ROWS = ./build/task2_rows
SRC_TASK_2_ROWS = ./task_2/task2_rows.c

all: create_build_dir build_task_1 build_task_2_rows

build_task_1: $(SRC_TASK_1) ${SOURCES}
	${CC} $(SRC_TASK_1) $(SOURCES) -o $(TARGET_TASK_1)

build_task_2_rows: $(SRC_TASK_2_ROWS) ${SOURCES}
	${CC} $(SRC_TASK_2_ROWS) $(SOURCES) -o $(TARGET_TASK_2_ROWS)

create_build_dir:
	mkdir -p build
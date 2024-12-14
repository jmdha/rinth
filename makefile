EXE_NAME   = rinth
BENCH_NAME = benchmark
TEST_NAME  = testrunner
SRC_DIR    = src
BENCH_DIR  = bench
TEST_DIR   = test
LIB_DIR    = third_party
BIN_DIR    = bin
WFLAGS     = -Wall -Wextra -Wshadow
LIBS       = -lm
CFLAGS     = -I$(SRC_DIR) -I$(LIB_DIR) -ggdb -O3 -flto -std=gnu17 -march=native 
SRCS       = $(shell find $(SRC_DIR) -type f -iname '*.c' ! -iname 'main.c')
BENCHS     = $(shell find $(BENCH_DIR) -type f -iname '*.c' ! -iname 'main.c')
TESTS      = $(shell find $(TEST_DIR) -type f -iname '*.c' ! -iname 'main.c')

.PHONY: all bench test

for all: CFLAGS += -D LOG_INFO -D LOG_TRACE
all: 
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(SRCS) $(SRC_DIR)/main.c $(LIBS)

bench:
	gcc $(WFLAGS) $(CFLAGS) -o $(BENCH_NAME) $(SRCS) $(BENCHS) $(BENCH_DIR)/main.c $(LIBS)
	./$(BENCH_NAME)

for test: CFLAGS += -fsanitize=address -O0
test:
	gcc $(WFLAGS) $(CFLAGS) -o $(TEST_NAME) $(SRCS) $(TESTS) $(TEST_DIR)/main.c $(LIBS)
	./$(TEST_NAME)

clean:
	rm -f rinth
	rm -f bench
	rm -f test_runner

EXE_NAME   = rinth
BENCH_NAME = bench
TEST_NAME  = test
SRC_DIR    = src
LIB_DIR    = third_party
BIN_DIR    = bin
WFLAGS     = -Wall -Wextra -Wshadow
CFLAGS     = -I$(SRC_DIR) -I$(LIB_DIR) -ggdb -O3 -flto -std=gnu17 -march=native 
SRCS       = $(shell find src -type f -iname '*.c' ! -iname '*test.c'  ! -iname '*bench.c')
SRCS_TEST  = $(shell find src -type f -iname '*.c' ! -iname '*bench.c' ! -iname 'main.c')
SRCS_BENCH = $(shell find src -type f -iname '*.c' ! -iname '*test.c'  ! -iname 'main.c')

.PHONY: all bench test

for all: CFLAGS += -D LOG_INFO -D LOG_ERROR -D LOG_TRACE
all: 
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(SRCS)

bench:
	gcc $(WFLAGS) $(CFLAGS) -o $(BENCH_NAME) $(SRCS_BENCH) -lm
	./$(BENCH_NAME)

for test: CFLAGS += -fsanitize=address -O0
test:
	gcc $(WFLAGS) $(CFLAGS) -o $(TEST_NAME) $(SRCS_TEST)
	./$(TEST_NAME)

clean:
	rm -f rinth
	rm -f bench
	rm -f test_runner

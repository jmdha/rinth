EXE_NAME   = rinth
BENCH_NAME = benchmark
TEST_NAME  = testrunner
SRC_DIR    = src
BENCH_DIR  = bench
TEST_DIR   = test
LIB_DIR    = third_party
BIN_DIR    = bin
WFLAGS     = -Wall -Wextra -Wshadow
LIBS       = -lm -lsqlite3
CFLAGS     = -I$(SRC_DIR) -I$(LIB_DIR) -ggdb -O0 -flto -std=gnu17 -march=native
SAFETY    += -fstack-protector-strong -fstack-clash-protection -fcf-protection -fsanitize=address -fno-omit-frame-pointer -fno-semantic-interposition
SRCS       = $(shell find $(SRC_DIR) -type f -iname '*.c' ! -iname 'main.c')
BENCHS     = $(shell find $(BENCH_DIR) -type f -iname '*.c' ! -iname 'main.c')
TESTS      = $(shell find $(TEST_DIR) -type f -iname '*.c' ! -iname 'main.c')

.PHONY: all bench test

for all: CFLAGS += -D LOG_INFO -D LOG_TRACE
all: 
	gcc $(WFLAGS) $(CFLAGS) $(SAFETY) -o $(EXE_NAME) $(SRCS) $(SRC_DIR)/main.c $(LIBS)

bench:
	gcc $(WFLAGS) $(CFLAGS) -o $(BENCH_NAME) $(SRCS) $(BENCHS) $(BENCH_DIR)/main.c $(LIBS)
	./$(BENCH_NAME)

test:
	gcc $(WFLAGS) $(CFLAGS) -o $(TEST_NAME) $(SRCS) $(TESTS) $(TEST_DIR)/main.c $(LIBS)
	./$(TEST_NAME)

clean:
	rm -f $(EXE_NAME)
	rm -f $(BENCH_NAME)
	rm -f $(TEST_NAME)

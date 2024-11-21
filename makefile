EXE_NAME   = rinth
BENCH_NAME = bench
TEST_NAME  = test_runner
SRC_DIR    = src
BIN_DIR    = bin
WFLAGS     = -Wall -Wextra -Wshadow
CFLAGS     = -I$(SRC_DIR) -Ithird_party -g -O3 -flto -std=c17 -march=native 
SRCS       = $(shell find src -type f -iname '*.c' ! -iname '*.test.c')
SRCS_TEST  = $(shell find src -type f -iname '*.test.c')

all: 
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(SRCS)

for dbuild: CFLAGS += -fsanitize=address -D LOG_DEBUG -O0
dbuild:
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(SRCS)

for tbuild: CFLAGS += -fsanitize=address -D LOG_TRACE
tbuild:
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(SRCS)

for bench: CFLAGS += -fsanitize=address -O0
bench:
	gcc $(WFLAGS) $(CFLAGS) -o $(BENCH_NAME) $(BIN_DIR)/bench.c $(SRCS) -lcriterion

for test: CFLAGS += -fsanitize=address -O0
test:
	gcc $(WFLAGS) $(CFLAGS) -o $(TEST_NAME) $(BIN_DIR)/test.c $(SRCS) $(SRCS_TEST) -lcriterion
	./$(TEST_NAME)

clean:
	rm -f rinth
	rm -f test_runner

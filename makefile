EXE_NAME = rinth
SRC_DIR = src
BIN_DIR = bin
TEST_DIR = tests

WFLAGS = -Wall -Wextra -Wshadow -Werror
CFLAGS  = -I$(SRC_DIR) -g -O3 -flto -std=c17 -march=native 

SRCS = $(shell find src -type f -iname '*.c' ! -iname '*.test.c')
OBJS = $(SRCS:.c=.o)
SRCS_TEST = $(shell find src -type f -iname '*.test.c')
OBJS_TEST = $(SRCS_TEST:.c=.o)

.objs: $(OBJS)
.objs_test: $(OBJS_TEST)

all: .objs
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(OBJS)

for dbuild: CFLAGS += -fsanitize=address -D LOG_DEBUG -O0
dbuild: .objs
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(OBJS)

for tbuild: CFLAGS += -fsanitize=address -D LOG_TRACE
tbuild:
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(OBJS)

for test: CFLAGS += -fsanitize=address -O0
test: .objs .objs_test
	gcc $(WFLAGS) $(CFLAGS) -o test_runner $(BIN_DIR)/test.c $(OBJS) $(OBJS_TEST) -lcriterion
	./test_runner

clean:
	rm -f rinth
	find . -name "*.o" -type f -delete

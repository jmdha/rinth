EXE_NAME = rinth
SRC_DIR = src
BIN_DIR = bin
TEST_DIR = tests

WFLAGS = -Wall -Wextra -Wshadow -pedantic
CFLAGS  = -I$(SRC_DIR) -g -O3 -flto -std=c17 -march=native 

SRCS = $(shell find src -type f -iname '*.c')
OBJS = $(SRCS:.c=.o)

.objs: $(OBJS)

for dbuild: CFLAGS += -fsanitize=address -D LOG_DEBUG -O0
dbuild: .objs
	gcc $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(OBJS)

for tbuild: CFLAGS += -fsanitize=address -D LOG_TRACE
tbuild:
	gcc $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(OBJS)

for test: CFLAGS += -fsanitize=address -O0
test: .objs
	gcc $(CFLAGS) -o test_runner $(TEST_DIR)/*.c $(OBJS) -lcriterion
	./test_runner

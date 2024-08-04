EXE_NAME = rinth
SRC_DIR = src
BIN_DIR = bin

WFLAGS = -Wall -Wextra -Wshadow -Werror
CFLAGS  = -I$(SRC_DIR) -g -O3 -flto -std=c17 -march=native 

SRCS = $(shell find src -type f -iname '*.c' ! -iname '*.test.c')
SRCS_TEST = $(shell find src -type f -iname '*.test.c')

.objs: $(OBJS)
.objs_test: $(OBJS_TEST)

all: .objs
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(SRCS)

for dbuild: CFLAGS += -fsanitize=address -D LOG_DEBUG -O0
dbuild: .objs
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(SRCS)

for tbuild: CFLAGS += -fsanitize=address -D LOG_TRACE
tbuild:
	gcc $(WFLAGS) $(CFLAGS) -o $(EXE_NAME) $(BIN_DIR)/main.c $(SRCS)

for test: CFLAGS += -fsanitize=address -O0
test: .objs .objs_test
	gcc $(WFLAGS) $(CFLAGS) -o test_runner $(BIN_DIR)/test.c $(SRCS) $(SRCS_TEST) -lcriterion
	./test_runner

clean:
	rm -f rinth
	rm -f test_runner

EXE_NAME  = rinth
BIN_DIR = bin
SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build
EXE_PATH  = $(BUILD_DIR)/$(EXE_NAME)
TEST_PATH  = $(BUILD_DIR)/test

CFLAGS  = -I$(SRC_DIR) -g -O3 -Wall -Wextra -flto -std=c17 -march=native 
SOURCES = $(shell find $(SRC_DIR) -type f -iname '*.c')

args = `arg="$(filter-out $@,$(MAKECMDGOALS))" && echo $${arg:-${1}}`

.PHONY: build
build:
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -o $(EXE_PATH) $(BIN_DIR)/main.c $(SOURCES)

for dbuild: CFLAGS += -O0 -D LOG_DEBUG
dbuild:
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -o $(EXE_PATH) $(BIN_DIR)/main.c $(SOURCES)

for tbuild: CFLAGS += -fsanitize=address -D LOG_TRACE
tbuild:
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -o $(EXE_PATH) $(BIN_DIR)/main.c $(SOURCES)

for test: CFLAGS += -O0
test:
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -o $(TEST_PATH) $(TEST_DIR)/*.c $(SOURCES) -lcriterion
	$(TEST_PATH)

clean:
	rm -r $(BUILD_DIR)

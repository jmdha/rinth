CFLAGS  = -Isrc -g -O3 -Wall -Wextra -pedantic -flto -std=c17 -fsanitize=address -march=native
HEADERS = src/log.h src/types.h src/temp.h
SOURCES = src/log.c

.PHONY: build

EXE_NAME  = rinth
TEST_DIR = tests
BUILD_DIR = build
EXE_PATH  = $(BUILD_DIR)/$(EXE_NAME)
TEST_PATH  = $(BUILD_DIR)/test

all: build test

build:
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -o $(EXE_PATH) src/main.c $(SOURCES)

run: build
	$(EXE_PATH)

# Do note: gcc when faced with multiple optimization flags will use the last
for gdb: CFLAGS += -O0
gdb: build
	gdb $(EXE_PATH)

test:
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -o $(TEST_PATH) $(TEST_DIR)/*.c $(SOURCES)
	$(TEST_PATH)

clean:
	rm -r $(BUILD_DIR)
